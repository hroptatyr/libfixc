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
/* check for me? */
#include <sys/mman.h>

#include "fix.h"
#include "fix-private.h"
#include "nifty.h"

#include "fixml-comp-sub.h"
#include "fixml-comp-sub.c"
#include "fixml-comp-fld.h"
#include "fixml-comp-fld.c"
#include "fixml-comp-rptb.h"
#include "fixml-comp-orb.h"
#include "fixml-comp-orb.c"
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
# define FIXC_DEBUG_WR(args...)
#else  /* !DEBUG_FLAG */
# define FIXC_DEBUG(args...)
# define FIXC_DEBUG_WR(args...)
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
	const char *attr;
	size_t alen;

	/* comb out everything that mustn't be rendered */
	switch (fld.typ) {
	case FIXC_TYP_VER:
	case FIXC_TYP_MSGTYP:
	case FIXC_TYP_CTXT:
		return;
	default:
		break;
	}

	/* otherwise, go along with the attr finding */
	attr = fixc_attr_fixmlify(t, (fixc_attr_t)fld.tag);
	if (UNLIKELY((alen = strlen(attr)) == 0UL)) {
		/* probably fubar'd or an unknown attr */
		static char miss[8];
		alen = snprintf(miss, sizeof(miss), "f%hu", fld.tag);
		attr = miss;
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
	case FIXC_TYP_CTXT:
	default:
		/* huh? */
		break;
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
	size_t tsz;

	if (UNLIKELY(ctx.i == FIXC_MSGT_BATCH)) {
		tag = "Batch";
	} else if (ctx.i > 0x2000) {
		tag = fixc_msgt_fixmlify(ctx.msgt);
	} else {
		tag = fixc_comp_fixmlify(ctx.comp);
	}
	if (UNLIKELY((tsz = strlen(tag)) == 0UL)) {
		static char miss[8];
		tsz = snprintf(miss, sizeof(miss), "g%u", ctx.i);
		tag = miss;
	}
	/* render him */
	return sncpy(p, ep, tag, tsz);
}

static int
__childp(fixc_comp_sub_t parent, fixc_ctxt_t child)
{
/* Return non-0 if CHILD is a child of PARENT. */
	fixc_comp_t tmp;

retry:
	for (size_t i = 0; i < parent->nsubs; i++) {
		if (parent->subs[i] == child.ui16) {
			return 1;
		}
	}
	/* see if we're falling for optimised implicit blocks */
	if (UNLIKELY((tmp = fixc_get_comp_orb((fixc_comp_t)parent->ctx)))) {
		/* one of them optimised implict blocks */
		parent = fixc_get_comp_sub(tmp);
		goto retry;
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
	if (UNLIKELY(fixc_get_comp_orb(otag))) {
		return;
	} else if (otag.ui16 == FIXC_MSGT_BATCH) {
		/* batch tags are inserted globally and once only */
		return;
	} else if (LIKELY(ctx->state != NULL)) {
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
	FIXC_DEBUG_WR("completely unwound, probably buggered FIX message\n");
	goto new_chld;
}


/* fixing stuff up */
static int
fld_ctx_p(fixc_fld_ctx_t fc, fixc_ctxt_t a)
{
	for (size_t j = 0; j < fc->nmsgs + fc->ncomps; j++) {
		if (fc->ctxs[j] == a.i) {
			/* yay, that was easy innit? */
			return 1;
		}
	}
	return 0;
}

static fixc_comp_t
fu_ancest_p(fixc_fld_ctx_t fc, fixc_ctxt_t c)
{
	/* oh, could be one of them comp-only blocks in between */
	fixc_comp_sub_t sub = fixc_get_comp_sub(c);

	/* first level is breadth-first, otherwise depth first */
	for (size_t i = 0; i < sub->nsubs; i++) {
		fixc_comp_t xcmp = (fixc_comp_t)sub->subs[i];

		if (fld_ctx_p(fc, xcmp)) {
			return xcmp;
		}
	}

	for (size_t i = 0; i < sub->nsubs; i++) {
		/* unrolled */
		fixc_comp_t xcmp = (fixc_comp_t)sub->subs[i];
		fixc_comp_sub_t xpar = fixc_get_comp_sub(xcmp);

		for (size_t j = 0; j < xpar->nsubs; j++) {
			fixc_comp_t ycmp = (fixc_comp_t)xpar->subs[j];
			fixc_comp_sub_t ypar;

			if (fld_ctx_p(fc, ycmp)) {
				return ycmp;
			}

			ypar = fixc_get_comp_sub(ycmp);
			for (size_t k = 0; k < ypar->nsubs; k++) {
				fixc_comp_t zcmp = (fixc_comp_t)ypar->subs[k];

				if (fld_ctx_p(fc, zcmp)) {
					return zcmp;
				}
			}
		}
	}
	return FIXC_COMP_UNK;
}

static void
fixc_fixup_some(fixc_msg_t msg)
{
	fixc_ctxt_t last = {FIXC_MSGT_UNK};
	unsigned int streak = 1U;

	for (size_t i = 0; i < msg->nflds; i++, streak++) {
		if (!msg->flds[i].tpc && !msg->flds[i].cnt) {
			msg->flds[i].tpc = (uint16_t)last.ui16;
			msg->flds[i].cnt = (uint16_t)streak;
		} else if (!msg->flds[i].cnt && msg->flds[i].tpc == last.ui16) {
			/* assume the unknown field disrupted our streak */
			msg->flds[i].cnt = (uint16_t)streak;
		} else if (!msg->flds[i].cnt) {
			/* nah, complete reset now */
			streak = 0;
			last.ui16 = msg->flds[i].tpc;
		}
	}
	return;
}

void
fixc_fixup(fixc_msg_t msg)
{
	/* previously known ctx */
	static struct {
		fixc_ctxt_t ctx;
		unsigned int idx;
	} stk[16];
	ssize_t nstk = -1;
	unsigned int streak = 0;
	int out_of_stack_p = 0;

#define rset()		(streak = 0)
#define push(x, i)	(rset(),				\
			 nstk++,				\
			 stk[nstk].idx = i,			\
			 stk[nstk].ctx = (fixc_ctxt_t){x})
#define peeki()		stk[nstk].idx
#define peek()		stk[nstk].ctx
#define pop()		(streak = 0, stk[nstk--].ctx)
#define streak()	(streak++)

	push(msg->f35.mtyp, 0);
	for (size_t i = 0; i < msg->nflds; i++) {
		fixc_attr_t ma = (fixc_attr_t)msg->flds[i].tag;
		fixc_fld_ctx_t fc = fixc_get_fld_ctx(ma);

		do {
			fixc_comp_t anc;

			if (fld_ctx_p(fc, peek())) {
				goto succ;
			} else if (UNLIKELY(ma == FIXC_MSG_TYPE)) {
				/* ok, those guys take precedence inasmuchas
				 * they're their own parent's context */
				push(msg->flds[i].mtyp, i);
				goto succ;
			} else if ((anc = fu_ancest_p(fc, peek()))) {
				/* otherwise go through subs of lctx */
				fixc_comp_t tmp;

				if (UNLIKELY((tmp = fixc_get_comp_orb(anc)))) {
					/* fixup optimised repeating blocks */
					anc = tmp;
				}
				push(anc, i);
				goto succ;
			}
			/* go back then? */
			(void)pop();
		} while (nstk >= 0);

		FIXC_DEBUG("couldn't find context for %hu\n", msg->flds[i].tag);
		push(msg->f35.mtyp, 0);
		out_of_stack_p = 1;
		continue;
	succ:
		msg->flds[i].tpc = (uint16_t)peek().ui16;
		if (i > peeki() + 1 &&
		    msg->flds[peeki() + 1].tag == msg->flds[i].tag) {
			rset();
		}
		msg->flds[i].cnt = (uint16_t)streak();
	}

	/* plain operation, but only if we encountered out_of_stack */
	if (UNLIKELY(out_of_stack_p)) {
		fixc_fixup_some(msg);
	}
	return;
}

#if defined DEBUG_FLAG
static void
pr_fld(int num, struct fixc_fld_s fld)
{
	fprintf(stderr, "FLD[%i] = {\n\
  .tag = %hu,\n\
  .typ = %hu,\n\
  .tpc = %hu,\n\
  .cnt = %hu,\n\
  .val = 0x%lx,\n\
};\n", num, fld.tag, fld.typ, fld.tpc, fld.cnt, fld.i64);
	return;
}
#endif	/* DEBUG_FLAG */

void
fixc_dump(fixc_msg_t msg)
{
#if defined DEBUG_FLAG
	pr_fld(-4, msg->f8);
	pr_fld(-3, msg->f9);
	pr_fld(-2, msg->f35);
	for (size_t i = 0; i < msg->nflds; i++) {
		pr_fld(i, msg->flds[i]);
	}
	pr_fld(-1, msg->f10);
#else  /* !DEBUG_FLAG */
	if (msg == NULL) {
		;
	}
#endif	/* DEBUG_FLAG */
	return;
}


/* helpers */
#define FIXML_TAG	"FIXML"

static char*
__render_hdr(char *restrict const buf, const char *const ep, fixc_msg_t msg)
{
	static const char xml_pre[] = "\
<?xml version=\"1.0\"?>";
	char *restrict p = buf;

	p = sncpy(p, ep, xml_pre, sizeof(xml_pre) - 1);
	/* newline this one, all other tags will have no indentation */
	*p++ = '\n';
	/* ... and open our tag */
	p = sputc(p, ep, '<');
	p = sncpy(p, ep, FIXML_TAG, sizeof(FIXML_TAG) - 1);

	/* fill in xmlns uri */
	p += __render_xmlns(p, ep - p, msg);

	/* fill in v attr */
	p += __render_v(p, ep - p, msg);

	/* eo FIXML tag start */
	p = sputc(p, ep, '>');

	/* see if we need to produce the Batch tag */
	if (msg->f35.mtyp == FIXC_MSGT_BATCH) {
		p = sputc(p, ep, '<');
		p = __fixmlify(p, ep, FIXC_MSGT_BATCH);
		p = sputc(p, ep, '>');
	}
	return p;
}

static char*
__render_ftr(char *restrict const buf, const char *const ep, fixc_msg_t msg)
{
	char *restrict p = buf;

	/* see if we need to close the Batch tag */
	if (msg->f35.mtyp == FIXC_MSGT_BATCH) {
		p = sputc(p, ep, '<');
		p = sputc(p, ep, '/');
		p = __fixmlify(p, ep, FIXC_MSGT_BATCH);
		p = sputc(p, ep, '>');
	}
	/* final verdict */
	p = sputc(p, ep, '<');
	p = sputc(p, ep, '/');
	p = sncpy(p, ep, FIXML_TAG, sizeof(FIXML_TAG) - 1);
	p = sputc(p, ep, '>');
	p = sputc(p, ep, '\n');
	return p;
}

#undef FIXML_TAG


/* public functions */
/* new system with tag parent context and counter */
size_t
fixc_render_fixml(char *restrict const buf, size_t bsz, fixc_msg_t msg)
{
	const char *const ep = buf + bsz;
	char *restrict p = buf;
	struct __ctx_s ctx = {0};
	fixc_ctxt_t otpc = {0};

	if (fixc_msg_needs_fixup_p(msg)) {
		/* just in case the reader hasn't given us contexts */
		fixc_fixup(msg);
	}

	/* the usual stuff upfront, xml PI */
	p = __render_hdr(p, ep, msg);

	/* set up our stack */
	ptx_init(&ctx, p, ep);
	/* traverse the message only once */
	for (size_t i = 0; i < msg->nflds; i++) {
		fixc_ctxt_t ictx = {(unsigned int)msg->flds[i].tpc};

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

	/* close everything */
	p = __render_ftr(p, ep, msg);
	*p = '\0';
	return p - buf;
}

#define MMAP_THRESH	(65536UL)

static inline size_t
__round_to_mmap_thresh(size_t x)
{
	return ((x + MMAP_THRESH - 1UL) / MMAP_THRESH) * MMAP_THRESH;
}

static void
resz_rndr(char **buf, size_t *bsz)
{
	/* just double the whole thing */
	if (2 * *bsz < MMAP_THRESH) {
		/* just use realloc */
		*buf = realloc(*buf, 2 * *bsz);
		*bsz *= 2;
	} else if (*bsz < MMAP_THRESH) {
		/* start mmap page */
		size_t naz = __round_to_mmap_thresh(2 * *bsz);
		char *nu = mmap(NULL, naz, PROT_MEM, MAP_MEM, -1, 0);
		memcpy(nu, *buf, *bsz);
		free(*buf);
		*buf = nu;
		*bsz = naz;
	} else {
		/* simples */
		size_t oaz = __round_to_mmap_thresh(*bsz);
		size_t naz = __round_to_mmap_thresh(2 * *bsz);

#if defined MREMAP_MAYMOVE
		*buf = mremap(*buf, oaz, naz, MREMAP_MAYMOVE);
#else  /* !MREMAP_MAYMOVE */
		char *nu = mmap(NULL, naz, PROT_MEM, MAP_MEM, -1, 0);
		memcpy(nu, *buf, *bsz);
		munmap(*buf, oaz);
		*buf = nu;
		*bsz = naz;
#endif	/* MREMAP_MAYMOVE */
	}
	return;
}

struct fixc_rndr_s
fixc_render_fixml_rndr(fixc_msg_t msg)
{
	const char *ep;
	char *restrict p;
	struct __ctx_s ctx = {0};
	fixc_ctxt_t otpc = {0};
	char *buf;
	size_t bsz;

	if (fixc_msg_needs_fixup_p(msg)) {
		/* just in case the reader hasn't given us contexts */
		fixc_fixup(msg);
	}

	/* get some initial space, average tag length of sp2ep104 is
	 * 2586 / 261, average attr length is 11245 / 1119 and
	 * typically it's 1 context per 4 attrs */
#define ATTR_LEN	(11245U / 1119U + 1U)
#define COMP_LEN	(2586U / 261U + 1U)
	bsz = (4 + msg->nflds) * (ATTR_LEN + 3/*SPC before and quotes*/) +
		(4 + msg->nflds) / 4 * (2 * COMP_LEN + 2 + 3/*<> and </>*/) +
		/* try and be helpful if there's pr space */
		msg->pz + 16;
	if (bsz < MMAP_THRESH) {
		/* malloc must do */
		buf = malloc(bsz);
	} else {
		/* aaah, prefer mmap() */
		bsz = __round_to_mmap_thresh(bsz);
		buf = mmap(NULL, bsz, PROT_MEM, MAP_MEM, -1, 0);
	}

	/* assign boundary helper vars */
	ep = buf + bsz;
	p = buf;

	/* the usual stuff upfront, xml PI */
	p = __render_hdr(p, ep, msg);

	/* set up our stack */
	ptx_init(&ctx, p, ep);
	/* traverse the message only once */
	for (size_t i = 0; i < msg->nflds; i++) {
		fixc_ctxt_t ictx = {(unsigned int)msg->flds[i].tpc};

		/* check if the buffer needs resizing */
		if (UNLIKELY((ep - p) * 10UL / bsz < 1)) {
			/* oh fuck, we're in the last 10% */
			off_t off = p - buf;

			resz_rndr(&buf, &bsz);
			p = buf + off;
			ep = buf + bsz;
		}

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
	/* close everything */
	p = __render_ftr(p, ep, msg);
	*p = '\0';

	/* if mmap is in place, downsize to multiple of totz */
	if (bsz >= MMAP_THRESH) {
#if defined MREMAP_MAYMOVE
		size_t naz = __round_to_mmap_thresh(p - buf);
		buf = mremap(buf, bsz, naz, MREMAP_MAYMOVE);
#else  /* !MREMAP_MAYMOVE */
		/* um, good question, another mmap? :O */
		;
#endif	/* MREMAP_MAYMOVE */
	}
	return (struct fixc_rndr_s){.str = buf, .len = p - buf};
}

/* fixml-wr.c ends here */
