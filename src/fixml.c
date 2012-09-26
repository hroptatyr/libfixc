/*** fixml.c -- guts for fixml elements and attributes
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

#include "fixml-attr.h"
#include "fixml-attr.c"
#include "fixml-attr-rev.c"

#if defined DEBUG_FLAG
# define FIXC_DEBUG(args...)	fprintf(stderr, args)
#else  /* !DEBUG_FLAG */
# define FIXC_DEBUG(args...)
#endif	/* DEBUG_FLAG */

#if !defined CHAR_BIT
# define CHAR_BIT	(8U)
#endif	/* !CHAR_BIT */


/* generic rendering */
static size_t
__render_attr(
	char *restrict buf, size_t bsz, const char *b, struct fixc_fld_s fld)
{
	size_t res = 0UL;

	switch (fld.tag) {
	case 117/*QID*/:
		memcpy(buf + res, " QID=\"", sizeof(" QID=\"") - 1);
		res += sizeof(" QID=\"") - 1;

		buf[res++] = '"';
		break;
	default:
		break;
	}
	return res;
}


static int
__quot_attr_p(struct fixc_fld_s fld)
{
	return 1;
}

static size_t
fixc_render_quote(char *restrict buf, size_t bsz, fixc_msg_t msg)
{
	static const char quot_pre[] = "<Quot";
	static const char quot_post[] = "</Quot>";
	size_t res = 0UL;

	/* let's open the tag */
	memcpy(buf + res, quot_pre, sizeof(quot_pre) - 1);
	res += sizeof(quot_pre) - 1;

	for (size_t i = 0; i < msg->nflds && res < bsz; i++) {
		if (__quot_attr_p(msg->flds[i])) {
			res += __render_attr(
				buf + res, bsz - res, msg->pr, msg->flds[i]);
		}
	}

	/* close the tag */
	buf[res++] = '>';

	/* and we're off */
	memcpy(buf + res, quot_post, sizeof(quot_post));
	res += sizeof(quot_post) - 1;
	return res;
}


/* fixml guts */
size_t
fixc_render_fixml(char *restrict buf, size_t bsz, fixc_msg_t msg)
{
	static const char xml_pre[] = "\
<?xml version=\"1.0\"?>";
	static const char fixml_pre[] = "\
<FIXML xmlns=\"http://www.fixprotocol.org/FIXML-5-0-SP2\">";
	static const char fixml_post[] = "\
</FIXML>";
	size_t totz = 0;

	/* just the usual stuff */
	memcpy(buf + totz, xml_pre, sizeof(xml_pre) - 1);
	buf[(totz += sizeof(xml_pre)) - 1] = '\n';
	memcpy(buf + totz, fixml_pre, sizeof(fixml_pre) - 1);
	totz += sizeof(fixml_pre) - 1;

	if (msg->f35.typ != FIXC_TYP_MSGTYP) {
		goto grrr;
	}
	switch (msg->f35.mtyp) {
	default:
	case FIXC_MSGTYP_UNK:
	grrr:
		/* do fuckall */
		break;
	case FIXC_MSGTYP_QUOTE:
		totz += fixc_render_quote(buf + totz, bsz - totz, msg);
		break;
	}

	/* final verdict */
	memcpy(buf + totz, fixml_post, sizeof(fixml_post));
	totz += sizeof(fixml_post);
	return totz;
}


#if defined STANDALONE
#define SOH	"\001"
#define SOHC	(*SOH)

int
main(void)
{
	static char foo[] = "8=FIXT.1.1" SOH "9=0004" SOH
		"35=S" SOH "117=112" SOH
		"132=1.03" SOH "133=1.04" SOH "10=0";
	char test[256];
	fixc_msg_t msg = make_fixc_from_fix(foo, sizeof(foo) - 1);

	fprintf(stdout, "%zu fields\n", msg->nflds);
	for (size_t i = 0; i < msg->nflds; i++) {
		fprintf(stdout, "+ field %zu: %hu=%s\n",
			i, msg->flds[i].tag, msg->pr + msg->flds[i].off);
	}

	fixc_render_fixml(test, sizeof(test), msg);
	fputs(test, stdout);
	fputc('\n', stdout);

	fixc_add_fld(msg, (struct fixc_fld_s){
				 .tag = 54/*Side*/,
					 .typ = FIXC_TYP_UCHAR,
					 .u8 = '1'
					 });
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket") - 1);
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket") - 1);
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_tag(msg, 55/*Sym*/, "EURbasket", sizeof("EURbasket"));
	fixc_add_fld(msg, (struct fixc_fld_s){
				 .tag = 54/*Side*/,
					 .typ = FIXC_TYP_UCHAR,
					 .u8 = '2'
					 });
	fixc_render_fixml(test, sizeof(test), msg);
	fputs(test, stdout);
	fputc('\n', stdout);

	free_fixc(msg);
	return 0;
}
#endif	/* STANDALONE */

/* fixml.c ends here */
