/*** fixml-wr.c -- guts for fixml element and attribute rendering
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
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <inttypes.h>

#include "fix.h"
#include "nifty.h"

#include "fixml-comp-sub.h"
#include "fixml-comp-sub.c"
#include "fixml-comp-fld.h"
#include "fixml-comp-fld.c"
#include "fixml-comp-rptb.h"
#include "fixml-fld-ctx.h"
#include "fixml-fld-ctx.c"

#include "fixml-canon-ctxt.h"
#include "fixml-canon-comp.h"
#include "fixml-canon-msgt.h"
#include "fixml-canon-attr.h"

/* resolves fixc_attr_t to attr string */
#include "fixml-attr-rev.c"
/* resolves fixc_comp_t to component element string */
#include "fixml-comp-rev.c"
/* resolves fixc_msgt_t to message element string */
#include "fixml-msg-rev.c"

/* resolves fixc_ver_t objects to various strings, hand-crafted */
#include "fixml-nsuri-rev.c"

#if defined DEBUG_FLAG
# define FIXC_DEBUG(args...)	fprintf(stderr, args)
#else  /* !DEBUG_FLAG */
# define FIXC_DEBUG(args...)
#endif	/* DEBUG_FLAG */

#if !defined CHAR_BIT
# define CHAR_BIT	(8U)
#endif	/* !CHAR_BIT */

typedef struct __ctx_s *__ctx_t;
typedef struct ptx_ctxcb_s *ptx_ctxcb_t;

struct ptx_ctxcb_s {
	/* for a linked list */
	ptx_ctxcb_t next;

	/* navigation info, stores the context */
	fixc_ctxt_t osub;
	/* non-0 if attrs have been rendered */
	uint16_t cnta;
	/* non-0 if sub components have been opened */
	uint16_t cntc;

	ptx_ctxcb_t old_state;
};

struct __ctx_s {
	/* parser state, for contextual callbacks */
	ptx_ctxcb_t state;
	/* pool of context trackers, implies maximum parsing depth */
	struct ptx_ctxcb_s ctxcb_pool[16];
	ptx_ctxcb_t ctxcb_head;

	char *restrict p;
	const char *ep;
};

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


static char*
sputc(char *restrict buf, const char *eob, char c)
{
/* put C into BUF but don't go beyond EOB. */
	if (UNLIKELY(buf + 1 >= eob)) {
		return buf;
	}
	*buf++ = c;
	return buf;
}

static char*
sncpy(char *restrict buf, const char *eob, const char *s, size_t slen)
{
/* copy S (of size SLEN) to BUF but don't go beyond EOB. */
	if (UNLIKELY(buf + slen >= eob)) {
		slen = eob - buf;
	}
	memcpy(buf, s, slen);
	return buf + slen;
}


/* fixml guts */
static void
__render_attr(__ctx_t ctx, fixc_ctxt_t t, const char *b, struct fixc_fld_s fld)
{
	const char *attr = fixc_attr_fixmlify(t, (fixc_attr_t)fld.tag);
	size_t alen = strlen(attr);

	if (!alen) {
		/* probably fubar'd or an unknown attr */
		return;
	}

	ctx->p = sputc(ctx->p, ctx->ep, ' ');
	ctx->p = sncpy(ctx->p, ctx->ep, attr, alen);
	ctx->p = sputc(ctx->p, ctx->ep, '=');
	ctx->p = sputc(ctx->p, ctx->ep, '"');
	switch (fld.typ) {
		size_t len;
	case FIXC_TYP_OFF:
		len = strlen(b + fld.off);
		ctx->p = sncpy(ctx->p, ctx->ep, b + fld.off, len);
		break;
	case FIXC_TYP_UCHAR:
		ctx->p += snprintf(
			ctx->p, ctx->ep - ctx->p - 1,
			"%03u", (unsigned int)fld.u8);
		break;
	case FIXC_TYP_CHAR:
		ctx->p = sputc(ctx->p, ctx->ep, fld.c);
		break;
	case FIXC_TYP_INT:
		ctx->p += snprintf(
			ctx->p, ctx->ep - ctx->p - 1,
			"%" PRIi32, fld.i32);
		break;

	case FIXC_TYP_VER:
	case FIXC_TYP_MSGTYP:
	default:
		/* huh? */
		return;
	}
	ctx->p = sputc(ctx->p, ctx->ep, '"');
	/* we should up the attr counter here */
	ctx->state->cnta++;
	return;
}

static size_t
__render_xmlns(char *restrict const buf, size_t bsz, fixc_msg_t msg)
{
	static const char xmlns[] = "xmlns";
	char *p = buf;
	const char *ep = buf + bsz;
	fixc_ver_t ver = msg->f8.ver;
	const char *nsuri;

	if (msg->f8.typ != FIXC_TYP_VER || ver == FIXC_VER_UNK) {
		ver = FIXC_VER_50_SP2;
	}
	/* obtain the uri */
	nsuri = __ver_fixmlify(ver);

	/* stoop to printing the whole shebang */
	p = sputc(p, ep, ' ');
	p = sncpy(p, ep, xmlns, sizeof(xmlns) - 1);
	p = sputc(p, ep, '=');
	p = sputc(p, ep, '"');
	p = sncpy(p, ep, nsuri, strlen(nsuri));
	p = sputc(p, ep, '"');
	return p - buf;
}

static size_t
__render_v(char *restrict const buf, size_t bsz, fixc_msg_t msg)
{
	static const char v[] = "v";
	char *p = buf;
	const char *ep = buf + bsz;
	fixc_ver_t ver = msg->f8.ver;
	const char *vstr;

	if (msg->f8.typ != FIXC_TYP_VER || ver < FIXC_VER_44) {
		ver = FIXC_VER_50_SP2;
	}
	/* obtain the uri */
	vstr = __ver_fixml_v_ify(ver);

	/* stoop to printing the whole shebang */
	p = sputc(p, ep, ' ');
	p = sncpy(p, ep, v, sizeof(v) - 1);
	p = sputc(p, ep, '=');
	p = sputc(p, ep, '"');
	p = sncpy(p, ep, vstr, strlen(vstr));
	p = sputc(p, ep, '"');
	return p - buf;
}

static fixc_ctxt_t
pop_state(__ctx_t ctx)
{
/* restore the previous current state */
	ptx_ctxcb_t curr = ctx->state;
	fixc_ctxt_t res;

	if (UNLIKELY(curr == NULL)) {
		return (fixc_ctxt_t){0U};
	}
	/* otherwise store res for later use */
	res = curr->osub;
	ctx->state = curr->old_state;
	/* queue him in our pool */
	push_ctxcb(ctx, curr);
	return res;
}

static void
push_state(__ctx_t ctx, fixc_ctxt_t otag)
{
	ptx_ctxcb_t res;
	fixc_comp_sub_t sub = fixc_get_comp_sub(otag);

	if (UNLIKELY(sub->min == 0 && sub->max == -1)) {
		/* one of them optimised implict blocks */
		sub = fixc_get_comp_sub((fixc_comp_t)sub->subs[0]);
	}

	/* now for real */
	res = pop_ctxcb(ctx);
	/* stuff it with the object we want to keep track of */
	res->osub = otag;
	res->cnta = 0;
	res->cntc = 0;
	/* fiddle with the states in our context */
	res->old_state = ctx->state;
	ctx->state = res;
	return;
}

static void
ptx_init(__ctx_t ctx, char *restrict p, const char *ep)
{
	/* initialise the ctxcb pool */
	init_ctxcb(ctx);
	ctx->p = p;
	ctx->ep = ep;
	return;
}

static char*
__fixmlify(char *restrict p, const char *ep, fixc_ctxt_t ctx)
{
	const char *tag;

	if (ctx.i > 0x2000) {
		tag = fixc_msgt_fixmlify(ctx.msgt);
	} else {
		tag = fixc_comp_fixmlify(ctx.comp);
	}
	return sncpy(p, ep, tag, strlen(tag));
}

static int
__childp(fixc_comp_sub_t parent, fixc_ctxt_t child)
{
/* Return non-0 if CHILD is a child of PARENT. */
	for (size_t i = 0; i < parent->nsubs; i++) {
		if (parent->subs[i] == child.ui16) {
			return 1;
		}
	}
	return 0;
}

static __attribute__((unused)) fixc_comp_t
__ancestp(fixc_comp_sub_t ancest, fixc_ctxt_t descend)
{
/* Return non-0 if DESCEND is a grand-child of ANCEST. */
	/* oh, could be one of them comp-only blocks in between */
	for (size_t i = 0; i < ancest->nsubs; i++) {
		fixc_comp_t xcmp = (fixc_comp_t)ancest->subs[i];
		fixc_comp_sub_t xpar = fixc_get_comp_sub(xcmp);

		/* unrolled */
		for (size_t j = 0; j < xpar->nsubs; j++) {
			fixc_comp_t ycmp = (fixc_comp_t)xpar->subs[j];
			fixc_comp_sub_t ypar;

			if (ycmp == descend.ui16) {
				return xcmp;
			}

			ypar = fixc_get_comp_sub(ycmp);
			for (size_t k = 0; k < ypar->nsubs; k++) {
				fixc_comp_t zcmp = (fixc_comp_t)ypar->subs[k];

				if (zcmp == descend.ui16) {
					return ycmp;
				}
			}
		}
	}
	return FIXC_COMP_UNK;
}

static void
push_rndr_state(__ctx_t ctx, fixc_ctxt_t otag)
{
	if (LIKELY(ctx->state != NULL)) {
		if (ctx->state->cntc++ == 0) {
			/* finish the parent's opening tag */
			ctx->p = sputc(ctx->p, ctx->ep, '>');
		}
	}
	/* render the attr */
	ctx->p = sputc(ctx->p, ctx->ep, '<');
	ctx->p = __fixmlify(ctx->p, ctx->ep, otag);

	/* push the state onto our stack */
	push_state(ctx, otag);
	return;
}

static fixc_ctxt_t
pop_rndr_state(__ctx_t ctx)
{
	if (UNLIKELY(ctx->state == NULL)) {
		return (fixc_ctxt_t){0U};
	}

	if (ctx->state->cntc) {
		fixc_ctxt_t otag = ctx->state->osub;

		ctx->p = sputc(ctx->p, ctx->ep, '<');
		ctx->p = sputc(ctx->p, ctx->ep, '/');
		ctx->p = __fixmlify(ctx->p, ctx->ep, otag);
		ctx->p = sputc(ctx->p, ctx->ep, '>');
	} else {
		ctx->p = sputc(ctx->p, ctx->ep, '/');
		ctx->p = sputc(ctx->p, ctx->ep, '>');
	}
	return pop_state(ctx);
}

static fixc_comp_t
__ancest_rndr(__ctx_t ctx, fixc_comp_sub_t ancest, fixc_ctxt_t descend)
{
/* Return non-0 if DESCEND is a grand-child of ANCEST. */
	/* oh, could be one of them comp-only blocks in between */
	for (size_t i = 0; i < ancest->nsubs; i++) {
		fixc_comp_t xcmp = (fixc_comp_t)ancest->subs[i];
		fixc_comp_sub_t xpar = fixc_get_comp_sub(xcmp);

		if (xcmp == descend.ui16) {
			return xcmp;
		}

		/* unrolled */
		for (size_t j = 0; j < xpar->nsubs; j++) {
			fixc_comp_t ycmp = (fixc_comp_t)xpar->subs[j];
			fixc_comp_sub_t ypar;

			if (ycmp == descend.ui16) {
				push_rndr_state(ctx, xcmp);
				return ycmp;
			}

			ypar = fixc_get_comp_sub(ycmp);
			for (size_t k = 0; k < ypar->nsubs; k++) {
				fixc_comp_t zcmp = (fixc_comp_t)ypar->subs[k];

				if (zcmp == descend.ui16) {
					push_rndr_state(ctx, xcmp);
					push_rndr_state(ctx, ycmp);
					return zcmp;
				}
			}
		}
	}
	return FIXC_COMP_UNK;
}

static void
__change_ctx(__ctx_t ctx, fixc_ctxt_t new)
{
	/* open a tag, question is child or sibling */
	while (ctx->state != NULL) {
		fixc_comp_sub_t sub = fixc_get_comp_sub(ctx->state->osub);

		if (__childp(sub, new) || __ancest_rndr(ctx, sub, new)) {
		new_chld:
			/* it's a child, yay */
			push_rndr_state(ctx, new);
			return;
		}

		/* otherwise, must be a sibling or a cousin */
		pop_rndr_state(ctx);
	}
	FIXC_DEBUG("completely unwound, probably buggered FIX message\n");
	goto new_chld;
}


/* public functions */
#if 0
size_t
fixc_render_fixml(char *restrict const buf, size_t bsz, fixc_msg_t msg)
{
	static const char xml_pre[] = "\
<?xml version=\"1.0\"?>";
	static const char fixml[] = "FIXML";
	const char *ep = buf + bsz;
	char *restrict p = buf;

	/* the usual stuff upfront, xml PI */
	p = sncpy(p, ep, xml_pre, sizeof(xml_pre) - 1);
	/* newline this one, all other tags will have no indentation */
	*p++ = '\n';
	/* ... and open our tag */
	p = sputc(p, ep, '<');
	p = sncpy(p, ep, fixml, sizeof(fixml) - 1);

	/* fill in xmlns uri */
	p += __render_xmlns(p, ep - p, msg);

	/* fill in v attr */
	p += __render_v(p, ep - p, msg);

	/* eo FIXML tag start */
	p = sputc(p, ep, '>');

	/* there ought to be just one message in there innit? */
	p += __render_msgtyp(p, ep - p, msg);

	/* final verdict */
	p = sputc(p, ep, '<');
	p = sputc(p, ep, '/');
	p = sncpy(p, ep, fixml, sizeof(fixml) - 1);
	p = sputc(p, ep, '>');
	*p = '\0';
	return p - buf;
}
#else
/* new system with tag parent context and counter */
size_t
fixc_render_fixml(char *restrict const buf, size_t bsz, fixc_msg_t msg)
{
	static const char xml_pre[] = "\
<?xml version=\"1.0\"?>";
	static const char fixml[] = "FIXML";
	const char *ep = buf + bsz;
	char *restrict p = buf;
	struct __ctx_s ctx = {0};
	fixc_ctxt_t otpc = {0};

	/* the usual stuff upfront, xml PI */
	p = sncpy(p, ep, xml_pre, sizeof(xml_pre) - 1);
	/* newline this one, all other tags will have no indentation */
	*p++ = '\n';
	/* ... and open our tag */
	p = sputc(p, ep, '<');
	p = sncpy(p, ep, fixml, sizeof(fixml) - 1);

	/* fill in xmlns uri */
	p += __render_xmlns(p, ep - p, msg);

	/* fill in v attr */
	p += __render_v(p, ep - p, msg);

	/* eo FIXML tag start */
	p = sputc(p, ep, '>');

	/* set up our stack */
	ptx_init(&ctx, p, ep);
	/* traverse the message only once */
	for (size_t i = 0; i < msg->nflds; i++) {
		fixc_ctxt_t ictx = {.ui16 = msg->flds[i].tpc};

		/* several edge triggers here:
		 * - whenever the .tpc (parent ctx) changes
		 * - whenever the .cnt (field counter) goes back to 0 */
		if (UNLIKELY(msg->flds[i].tag == fixc_comp_rptb(ictx))) {
			/* don't even render this guy */
			continue;
		} else if (msg->flds[i].tpc != otpc.ui16) {
			/* let's see what to do to our stack */
			__change_ctx(&ctx, ictx);
			otpc.ui16 = msg->flds[i].tpc;
		} else if (msg->flds[i].cnt == 0) {
			/* consecutive counter reset */
			ictx = pop_rndr_state(&ctx);
			push_rndr_state(&ctx, ictx);
		}
		/* of course the attr needs rendering */
		__render_attr(&ctx, ictx, msg->pr, msg->flds[i]);
	}

	while (pop_rndr_state(&ctx).i);

	/* copy the context pointer back */
	p = ctx.p;
	/* final verdict */
	p = sputc(p, ep, '<');
	p = sputc(p, ep, '/');
	p = sncpy(p, ep, fixml, sizeof(fixml) - 1);
	p = sputc(p, ep, '>');
	*p = '\0';
	return p - buf;
}
#endif

/* fixml-wr.c ends here */
