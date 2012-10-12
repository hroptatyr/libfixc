/*** fixml-rd.c -- fixml reader
 *
 * Copyright (C) 2010-2012 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@ga-group.nl>
 *
 * This file is part of libfixc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <inttypes.h>
#if defined HAVE_EXPAT_H
# include <expat.h>
#endif	/* HAVE_EXPAT_H */

#include "fix.h"
#include "nifty.h"

#include "fixml-comp.h"
#include "fixml-msg.h"
#include "fixml-msg.c"

#include "fixml-nsuri.c"

#include "fixml-comp-by-ctx.h"
#include "fixml-attr-by-ctx.h"

#include "fixml-comp-rptb.h"
#include "fixml-comp-rptb.c"

#if defined DEBUG_FLAG
# define FIXC_DEBUG(args...)	fprintf(stderr, args)
#else  /* !DEBUG_FLAG */
# define FIXC_DEBUG(args...)
#endif	/* DEBUG_FLAG */

/* value we like our vspc to be rounded to */
#define VSPC_RND	(128)
/* value we like our fspc (the fields) to be rounded to */
#define FSPC_RND	(32)

#if !defined CHAR_BIT
# define CHAR_BIT	(8U)
#endif	/* !CHAR_BIT */


/* contextual callbacks */
typedef struct __ctx_s *__ctx_t;
typedef struct ptx_ns_s *ptx_ns_t;
typedef struct ptx_ctxcb_s *ptx_ctxcb_t;

typedef unsigned int __tid_t;
typedef unsigned int __nsid_t;

struct ptx_ctxcb_s {
	/* for a linked list */
	ptx_ctxcb_t next;

	/* navigation info, stores the context */
	__tid_t otag;
	/* just a plain counter */
	unsigned int cnt;

	ptx_ctxcb_t old_state;
};

struct ptx_ns_s {
	char *pref;
	char *href;
	__nsid_t nsid;
};

struct __ctx_s {
	struct ptx_ns_s ns[16];
	size_t nns;
	/* stuff buf */
#define INITIAL_STUFF_BUF_SIZE	(4096)
	char *sbuf;
	size_t sbsz;
	size_t sbix;
	/* parser state, for contextual callbacks */
	ptx_ctxcb_t state;
	/* pool of context trackers, implies maximum parsing depth */
	struct ptx_ctxcb_s ctxcb_pool[16];
	ptx_ctxcb_t ctxcb_head;

	fixc_msg_t msg;
};

/* parsing guts */
static const char*
tag_massage(const char *tag)
{
/* return the real part of a (ns'd) tag or attribute,
 * i.e. foo:that_tag becomes that_tag */
	const char *p = strchr(tag, ':');

	if (p) {
		/* skip over ':' */
		return p + 1;
	}
	/* otherwise just return the tag as is */
	return tag;
}

static ptx_ns_t
__pref_to_ns(__ctx_t ctx, const char *pref, size_t pref_len)
{
	if (UNLIKELY(ctx->ns[0].nsid == FIXC_VER_UNK)) {
		/* bit of a hack innit? */
		return ctx->ns;

	} else if (LIKELY(pref_len == 0 && ctx->ns[0].pref == NULL)) {
		/* most common case when people use the default ns */
		return ctx->ns;
	}
	/* special service for us because we're lazy:
	 * you can pass pref = "foo:" and say pref_len is 4
	 * easier to deal with when strings are const etc. */
	if (pref[pref_len - 1] == ':') {
		pref_len--;
	}
	for (size_t i = (ctx->ns[0].pref == NULL); i < ctx->nns; i++) {
		if (strncmp(ctx->ns[i].pref, pref, pref_len) == 0) {
			return ctx->ns + i;
		}
	}
	return NULL;
}

static __nsid_t
__nsid_from_href(const char *href, size_t hlen)
{
	const struct fixc_nsuri_s *n = __fixml_nsiddify(href, hlen);
	return n != NULL ? n->nsid : FIXC_VER_UNK;
}

static fixc_msgt_t
__mty_from_elem(const char *elem, size_t elen)
{
	const struct fixml_msgt_s *p = __fixml_mtypify(elem, elen);
	return p != NULL ? (fixc_msgt_t)p->mty : FIXC_MSGT_UNK;
}

static size_t
__upd_rptb(fixc_msg_t msg, fixc_attr_t tag, fixc_ctxt_t ctx)
{
/* update the last counter tag of context CTX and report its value */
	size_t i = msg->nflds;
	struct fixc_fld_s rpbf;

	/* check the message so far, go backwards and try and find field TAG */
	while (i-- && msg->flds[i].tpc == ctx.ui16) {
		if (msg->flds[i].tag == tag) {
			/* found it, fiddle with it */
			return ++msg->flds[i].i32;
		}
	}

	/* otherwise, it must be the first such tag */
	rpbf.tag = (uint16_t)tag;
	rpbf.typ = FIXC_TYP_INT;
	rpbf.tpc = (uint16_t)ctx.ui16;
	rpbf.cnt = 0;
	/* should be at least one innit? */
	rpbf.i32 = 1;

	/* just add it then */
	fixc_add_fld(msg, rpbf);
	/* first occurrence so no dramas */
	return 1;
}


static void
init_ctxcb(__ctx_t ctx)
{
	memset(ctx->ctxcb_pool, 0, sizeof(ctx->ctxcb_pool));
	for (size_t i = 0; i < countof(ctx->ctxcb_pool) - 1; i++) {
		ctx->ctxcb_pool[i].next = ctx->ctxcb_pool + i + 1;
	}
	ctx->ctxcb_head = ctx->ctxcb_pool;
	return;
}

static ptx_ctxcb_t
pop_ctxcb(__ctx_t ctx)
{
	ptx_ctxcb_t res = ctx->ctxcb_head;

	if (LIKELY(res != NULL)) {
		ctx->ctxcb_head = res->next;
		memset(res, 0, sizeof(*res));
	}
	return res;
}

static void
push_ctxcb(__ctx_t ctx, ptx_ctxcb_t ctxcb)
{
	ctxcb->next = ctx->ctxcb_head;
	ctx->ctxcb_head = ctxcb;
	return;
}

static void
pop_state(__ctx_t ctx)
{
/* restore the previous current state */
	ptx_ctxcb_t curr = ctx->state;

	ctx->state = curr->old_state;
	/* queue him in our pool */
	push_ctxcb(ctx, curr);
	return;
}

static ptx_ctxcb_t
push_state(__ctx_t ctx, __tid_t otag)
{
	ptx_ctxcb_t res = pop_ctxcb(ctx);

	/* stuff it with the object we want to keep track of */
	res->otag = otag;
	/* reset the counter */
	res->cnt = 0U;
	/* fiddle with the states in our context */
	res->old_state = ctx->state;
	ctx->state = res;
	return res;
}

static void
ptx_init(__ctx_t ctx)
{
	/* initialise the ctxcb pool */
	init_ctxcb(ctx);
	return;
}

static void
ptx_reg_ns(__ctx_t ctx, const char *pref, const char *href)
{
	if (ctx->nns >= countof(ctx->ns)) {
		fputs("too many name spaces\n", stderr);
		return;
	}

	if (UNLIKELY(href == NULL)) {
		/* bollocks, user MUST be a twat */
		return;
	}

	/* get us those lovely ns ids */
	{
		const __nsid_t nsid = __nsid_from_href(href, strlen(href));

		switch (nsid) {
			size_t i;
		case FIXC_VER_50:
		case FIXC_VER_50_SP1:
		case FIXC_VER_50_SP2:
			if (UNLIKELY(ctx->ns[0].href != NULL)) {
				i = ctx->nns++;
				ctx->ns[i] = ctx->ns[0];
			}
			/* oh, it's our fave, make it the naught-th one */
			ctx->ns[0].pref = (pref ? strdup(pref) : NULL);
			ctx->ns[0].href = strdup(href);
			ctx->ns[0].nsid = nsid;
			break;

		case FIXC_VER_UNK:
		default:
			i = ctx->nns++;
			ctx->ns[i].pref = pref ? strdup(pref) : NULL;
			ctx->ns[i].href = strdup(href);
			ctx->ns[i].nsid = nsid;
			break;
		}
	}
	return;
}

static bool
ptx_pref_p(__ctx_t ctx, const char *pref, size_t pref_len)
{
	/* we sorted our namespaces so that ptx is always at index 0 */
	if (UNLIKELY(ctx->ns[0].href == NULL)) {
		return false;

	} else if (LIKELY(ctx->ns[0].pref == NULL)) {
		/* prefix must not be set here either */
		return pref == NULL || pref_len == 0;

	} else if (UNLIKELY(pref_len == 0)) {
		/* this node's prefix is "" but we expect a prefix of
		 * length > 0 */
		return false;

	} else {
		/* special service for us because we're lazy:
		 * you can pass pref = "foo:" and say pref_len is 4
		 * easier to deal with when strings are const etc. */
		if (pref[pref_len - 1] == ':') {
			pref_len--;
		}
		return memcmp(pref, ctx->ns[0].pref, pref_len) == 0;
	}
}


static void
check_rptblk(__ctx_t ctx, fixc_ctxt_t cid)
{
	fixc_attr_t rpba;
	size_t cnt;

	if (LIKELY((rpba = fixc_comp_rptb(cid)) == FIXC_ATTR_UNK)) {
		return;
	}
	/* otherwise it's update time */
	cnt = __upd_rptb(ctx->msg, rpba, cid);

	/* hm, i somehow feel bad about interacting with the
	 * proc_FIXML_attr() stuff through ctx, but what do I know */
	ctx->state->cnt = (typeof(ctx->state->cnt))cnt;
	return;
}

static void
bang_attr(__ctx_t ctx, fixc_attr_t tag, const char *val, size_t vsz)
{
	int fidx = ctx->msg->nflds - 1;

	/* avoid duplicate additions */
	if (UNLIKELY(ctx->msg->flds[fidx].tag == tag)) {
		/* we've found a dupe, there's basically two strategies now:
		 * - kill this tag and leave the previous one alone
		 * - overwrite the previous one and replace by this
		 * we chose the latter */
		FIXC_DEBUG("DUP!\n");
		ctx->msg->nflds--;
	}

	/* just use fix.c's add_tag thingie for this */
	if (LIKELY((fidx = fixc_add_tag(ctx->msg, tag, val, vsz)) >= 0)) {
		/* also set the field's parent context and whatnot */
		ctx->msg->flds[fidx].tpc = (uint16_t)ctx->state->otag;
		ctx->msg->flds[fidx].cnt = (uint16_t)ctx->state->cnt;
	}
	return;
}

static void
proc_FIXC_xmlns(__ctx_t ctx, const char *pref, const char *value)
{
	FIXC_DEBUG("reg'ging name space %s <- %s\n", pref, value);
	ptx_reg_ns(ctx, pref, value);
	return;
}

static void
proc_UNK_attr(__ctx_t ctx, const char *attr, const char *value)
{
	const char *rattr = tag_massage(attr);
	size_t alen;

	if (UNLIKELY(rattr > attr && !ptx_pref_p(ctx, attr, rattr - attr))) {
		alen = rattr - attr - 1;
	} else {
		alen = strlen(rattr);
	}

	/* aiddify */
	switch (fixc_get_aid(ctx->state ? ctx->state->otag : 0, attr, alen)) {
	case FIXC_ATTR_XMLNS:
	case FIXC_ATTR_V:
		proc_FIXC_xmlns(ctx, rattr == attr ? NULL : rattr, value);
		break;

	case FIXC_ATTR_UNK:
	default:
		FIXC_DEBUG("found unknown attr: %s (=%s)\n", attr, value);
		break;
	}
	return;
}

static void
proc_FIXML_attr(__ctx_t ctx, const char *attr, const char *value)
{
	const char *rattr = tag_massage(attr);
	unsigned int ctxid;
	fixc_attr_t aid;
	size_t alen;

	if (UNLIKELY(rattr > attr && !ptx_pref_p(ctx, attr, rattr - attr))) {
		alen = rattr - attr - 1;
	} else {
		alen = strlen(rattr);
	}
	/* aiddify */
	if (ctx->state == NULL || (ctxid = ctx->state->otag) == 0) {
		goto attr_unk;
	}
	switch ((aid = fixc_get_aid(ctxid, attr, alen))) {
	case FIXC_ATTR_XMLNS:
	case FIXC_ATTR_V:
		proc_FIXC_xmlns(ctx, rattr == attr ? NULL : rattr, value);
		break;

	case FIXC_ATTR_UNK:
	attr_unk:
		FIXC_DEBUG("found unknown FIXML attr: %s (=%s) in context %u\n",
			   attr, value, ctxid);
		break;
	default:
		bang_attr(ctx, aid, value, strlen(value));
		break;
	}
	return;
}


/* expat guts */
static void
sax_bo_FIXML_elt(__ctx_t ctx, const char *elem, const char **attr)
{
	const size_t elen = strlen(elem);
	unsigned int ctxid = 0;
	fixc_comp_t cid;

	/* all the stuff that needs a new sax handler */
	if (LIKELY(ctx->state != NULL)) {
		ctxid = ctx->state->otag;
	}
	switch ((cid = fixc_get_cid(ctxid, elem, elen))) {
	case FIXC_COMP_FIXML:
		ptx_init(ctx);
		push_state(ctx, FIXC_COMP_FIXML);
		break;

	case FIXC_COMP_UNK: {
		/* could be a message */
		const fixc_msgt_t mty = __mty_from_elem(elem, elen);

		if (!mty) {
			FIXC_DEBUG("neither cid nor mty: %s (in ctxt %u)\n",
				   elem, ctxid);
			break;
		} else if (ctxid == FIXC_MSGT_BATCH) {
			/* don't store the message type again,
			 * but store the field of course */
			struct fixc_fld_s fld = {
				.tag = FIXC_MSG_TYPE,
				.typ = FIXC_TYP_MSGTYP,
				/* parent is the batch brace */
				.tpc = FIXC_MSGT_BATCH,
				/* this is the 1-th occurrence
				 * compute me actually! */
				.cnt = 1,
#if defined HAVE_ANON_STRUCTS_INIT
				.mtyp = mty,
#endif	/* HAVE_ANON_STRUCTS_INIT */
			};
#if !defined HAVE_ANON_STRUCTS_INIT
			fld.mtyp = mty;
#endif	/* !HAVE_ANON_STRUCTS_INIT */
			fixc_add_fld(ctx->msg, fld);
		} else {
			ctx->msg->f35.tag = FIXC_MSG_TYPE;
			ctx->msg->f35.typ = FIXC_TYP_MSGTYP;
			ctx->msg->f35.mtyp = mty;
		}

		/* prepare for @fallthrough@ */
		cid = (fixc_comp_t)mty;
	}
	default:
		push_state(ctx, cid);

		/* oh oh oh, lest we forget, repeating block attr */
		check_rptblk(ctx, cid);

		for (const char **ap = attr; ap && *ap; ap += 2) {
			proc_FIXML_attr(ctx, ap[0], ap[1]);
		}
		break;
	}
	return;
}

static void
sax_eo_FIXML_elt(__ctx_t ctx, const char *elem)
{
	const size_t elen = strlen(elem);
	unsigned int ctxid = 0;

	/* get the previous context */
	if (LIKELY(ctx->state->old_state != NULL)) {
		ctxid = ctx->state->old_state->otag;
	}
	switch (fixc_get_cid(ctxid, elem, elen)) {
		/* top-levels */
	case FIXC_COMP_FIXML:
		(void)pop_state(ctx);
		break;

	case FIXC_COMP_UNK: {
		/* could be a message */
		const fixc_msgt_t mty = __mty_from_elem(elem, elen);

		if (!mty) {
			break;
		} else if (ctxid == FIXC_MSGT_BATCH) {
			;
		} else if (UNLIKELY(mty != ctx->msg->f35.mtyp)) {
			FIXC_DEBUG("mty was %u  f35.mtyp is %u\n",
				   mty, ctx->msg->f35.mtyp);
			abort();
		}
	}
	default:
		(void)pop_state(ctx);
		break;
	}
	return;
}

static void
el_sta(void *clo, const char *elem, const char **attr)
{
	__ctx_t ctx = clo;
	/* where the real element name starts, sans ns prefix */
	const char *relem = tag_massage(elem);
	ptx_ns_t ns = __pref_to_ns(ctx, elem, relem - elem);
	int retried = 0;

	if (UNLIKELY(ns == NULL)) {
		FIXC_DEBUG("unknown prefix in tag %s\n", elem);
		return;
	}

retry:
	switch (ns->nsid) {
	case FIXC_VER_44:
	case FIXC_VER_50:
	case FIXC_VER_50_SP1:
	case FIXC_VER_50_SP2:
		sax_bo_FIXML_elt(ctx, relem, attr);
		break;

	case FIXC_VER_UNK:
		for (const char **ap = attr; *ap != NULL; ap += 2) {
			proc_UNK_attr(ctx, ap[0], ap[1]);
		}
		/* for loop condition again :/ */
		if (*attr == NULL &&
		    *elem == 'F' && !strcmp(elem + 1, "IXML")) {
			/* just assume 5.0SP2 now, probably CME anyway */
			ctx->ns->nsid = FIXC_VER_50_SP2;
		}

		/* assume there was a version tag of some sort */
		ns = ctx->ns;
		/* assign the version to the msg too */
		ctx->msg->f8.tag = FIXC_BEGIN_STRING;
		ctx->msg->f8.typ = FIXC_TYP_VER;
		ctx->msg->f8.ver = (fixc_ver_t)ns->nsid;

		if (!retried++) {
			goto retry;
		}

	default:
		FIXC_DEBUG("unknown namespace %s ([%u] %s)\n",
			   elem, ns->nsid, ns->href);
		break;
	}
	return;
}

static void
el_end(void *clo, const char *elem)
{
	__ctx_t ctx = clo;
	/* where the real element name starts, sans ns prefix */
	const char *relem = tag_massage(elem);
	ptx_ns_t ns = __pref_to_ns(ctx, elem, relem - elem);

	switch (ns->nsid) {
	case FIXC_VER_44:
	case FIXC_VER_50:
	case FIXC_VER_50_SP1:
	case FIXC_VER_50_SP2:
		sax_eo_FIXML_elt(ctx, relem);
		break;

	case FIXC_VER_UNK:
	default:
		FIXC_DEBUG("unknown namespace %s (%s)\n", elem, ns->href);
		break;
	}
	return;
}


fixc_msg_t
make_fixc_from_fixml(const char *doc, size_t doclen)
{
#define ROUNDv(x)	ROUND(x, sizeof(void*))
	struct __ctx_s ctx = {0};
	XML_Parser hdl;
	fixc_msg_t res;
	size_t base = sizeof(*res);
	size_t fspc = ROUND(256, FSPC_RND);
	size_t vspc = ROUND(doclen / 2 + 1, VSPC_RND);
	size_t totz = ROUNDv(base + fspc * sizeof(*res->flds) + vspc);

	if (UNLIKELY((hdl = XML_ParserCreate(NULL)) == NULL)) {
		return NULL;
	}

	/* generate the husk */
	res = malloc(totz);
	memset(res, 0, sizeof(*res));
	res->flds = res->these;
	res->pr = (void*)(res->flds + fspc);
	res->pz = 0;
	ctx.msg = res;

	XML_SetElementHandler(hdl, el_sta, el_end);
	XML_SetUserData(hdl, &ctx);

	if (XML_Parse(hdl, doc, doclen, XML_TRUE) == XML_STATUS_ERROR) {
		free_fixc(res);
		return NULL;
	}

	/* get rid of resources */
	XML_ParserFree(hdl);
	return res;
}

/* fixml-rd.c ends here */
