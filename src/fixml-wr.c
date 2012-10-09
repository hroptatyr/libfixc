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
static int
__attr_in_ctx_p(fixc_attr_t a, fixc_ctxt_t ctx)
{
/* return non-0 if tag A is a member of component CTX or msg-type CTX. */
#if 1
	fixc_comp_fld_t fld = fixc_get_comp_fld(ctx);

	for (size_t i = 0; i < fld->nflds; i++) {
		if (a == fld->flds[i]) {
			return 1;
		}
	}
	return 0;
#else  /* !0 */
	fixc_fld_ctx_t fcm = fixc_get_fld_ctx((unsigned int)a);

	for (size_t i = 0; i < fcm->nmsgs + fcm->ncomps; i++) {
		if (ctx == fcm->ctxs[i]) {
			return 1;
		}
	}
	return 0;
#endif	/* 0 */
}

static size_t
__render_attr(
	char *restrict const buf, size_t bsz,
	fixc_ctxt_t ctx, const char *b, struct fixc_fld_s fld)
{
	const char *attr = fixc_attr_fixmlify(ctx, (fixc_attr_t)fld.tag);
	size_t alen = strlen(attr);
	char *p = buf;
	const char *ep = buf + bsz;

	if (!alen) {
		/* probably fubar'd or an unknown attr */
		return 0UL;
	}

	p = sputc(p, ep, ' ');
	p = sncpy(p, ep, attr, alen);
	p = sputc(p, ep, '=');
	p = sputc(p, ep, '"');
	switch (fld.typ) {
		size_t len;
	case FIXC_TYP_OFF:
		len = strlen(b + fld.off);
		p = sncpy(p, ep, b + fld.off, len);
		break;
	case FIXC_TYP_UCHAR:
		p += snprintf(p, ep - p - 1, "%03" PRIu8, fld.u8);
		break;
	case FIXC_TYP_CHAR:
		p = sputc(p, ep, fld.c);
		break;
	case FIXC_TYP_INT:
		p += snprintf(p, ep - p - 1, "%" PRIi32, fld.i32);
		break;

	case FIXC_TYP_VER:
	case FIXC_TYP_MSGTYP:
	default:
		/* huh? */
		return 0UL;
	}
	p = sputc(p, ep, '"');
	return p - buf;
}

/* fwd decl */
static size_t
__render_comp(
	char *restrict const buf, size_t bsz,
	fixc_msg_t msg, fixc_comp_t cid);

static size_t
__render_ctx(
	char *restrict const buf, size_t bsz,
	fixc_msg_t msg, fixc_ctxt_t ctx,
	const char *elem, size_t elen)
{
	char *p = buf;
	const char *ep = buf + bsz;
	fixc_comp_sub_t sub;
	int nattr = 0;
	int nsub = 0;

	/* start the tag */
	/* start the comp tag */
	p = sputc(p, ep, '<');
	p = sncpy(p, ep, elem, elen);

	/* all them attrs belonging in context CID */
	for (size_t i = 0; i < msg->nflds && p < ep; i++) {
		fixc_attr_t aid = (fixc_attr_t)msg->flds[i].tag;
		if (__attr_in_ctx_p(aid, ctx)) {
			nattr++;
			p += __render_attr(
				p, ep - p, ctx, msg->pr, msg->flds[i]);
		}
	}

	/* closing tag */
	sub = fixc_get_comp_sub(ctx);
	/* otherwise finish the tag */
	p = sputc(p, ep, '>');

	/* sub components */
	for (size_t i = 0; i < sub->nsubs; i++) {
		fixc_comp_t cid = (fixc_comp_t)sub->subs[i];
		size_t add;

		if ((add = __render_comp(p, ep - p, msg, cid)) > 0) {
			p += add;
			nsub++;
		}
	}
	/* check if any sub elements have been added */
	if (!nsub && !nattr) {
		/* we printed nothing */
		return 0UL;

	} else if (!nsub) {
		/* no subs, but attributes, rewind p a bit */
		p--;
		p = sputc(p, ep, '/');
		p = sputc(p, ep, '>');

	} else {
		/* we printed at least one sub-tag so finish this component */
		p = sputc(p, ep, '<');
		p = sputc(p, ep, '/');
		p = sncpy(p, ep, elem, elen);
		p = sputc(p, ep, '>');
	}
	return p - buf;
}

static size_t
__render_comp(
	char *restrict const buf, size_t bsz,
	fixc_msg_t msg, fixc_comp_t cid)
{
	const char *comp = fixc_comp_fixmlify(cid);
	size_t clen = strlen(comp);

	if (!clen) {
		/* must be fubar'd */
		return 0UL;
	}
	return __render_ctx(buf, bsz, msg, cid, comp, clen);
}

static size_t
__render_msgtyp(char *restrict const buf, size_t bsz, fixc_msg_t msg)
{
	fixc_msgt_t mty = msg->f35.mtyp;
	const char *mstr = fixc_msgt_fixmlify((fixc_msgt_t)mty);
	size_t mlen = strlen(mstr);

	if (!mlen) {
		/* probably fubar'd or an unknown msgtyp */
		return 0UL;
	}
	return __render_ctx(buf, bsz, msg, mty, mstr, mlen);
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


/* public functions */
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

/* fixml-wr.c ends here */