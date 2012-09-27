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

#include "fixml-comp.h"
#include "fixml-comp.c"
#include "fixml-comp-rev.c"

#include "fix-msg-type.h"
#include "fixml-msg-type.c"
#include "fixml-msg-type-rev.c"

#if defined DEBUG_FLAG
# define FIXC_DEBUG(args...)	fprintf(stderr, args)
#else  /* !DEBUG_FLAG */
# define FIXC_DEBUG(args...)
#endif	/* DEBUG_FLAG */

#if !defined CHAR_BIT
# define CHAR_BIT	(8U)
#endif	/* !CHAR_BIT */

static char*
sncpy(char *restrict buf, const char *eb, const char *s, size_t slen)
{
/* copy S (of size SLEN) to BUF but don't go beyond EB. */
	if (UNLIKELY(buf + slen > eb)) {
		slen = eb - buf;
	}
	memcpy(buf, s, slen);
	return buf + slen;
}


/* fixml guts */
static size_t
__render_msgtyp(char *restrict const buf, size_t bsz, fixc_msg_t msg)
{
	const char *mty = __mty_fixmlify(msg->f35.mtyp);
	size_t mtylen = strlen(mty);
	char *p = buf;
	const char *ep = buf + bsz;

	if (!mtylen) {
		/* probably fubar'd or an unknown msgtyp */
		return 0UL;
	}

	/* start the msg tag */
	*p++ = '<';
	p = sncpy(p, ep, mty, mtylen);

	/* all msg attributes here */
	;
	/* close the start-tag */
	*p++ = '>';

	/* all components of the message */
	;

	/* closing tag */
	*p++ = '<';
	*p++ = '/';
	p = sncpy(p, ep, mty, mtylen);
	*p++ = '>';
	return p - buf;
}

size_t
fixc_render_fixml(char *restrict const buf, size_t bsz, fixc_msg_t msg)
{
	static const char xml_pre[] = "\
<?xml version=\"1.0\"?>";
	static const char fixml_pre[] = "\
<FIXML xmlns=\"http://www.fixprotocol.org/FIXML-5-0-SP2\">";
	static const char fixml_post[] = "\
</FIXML>";
	const char *ep = buf + bsz;
	char *restrict p = buf;

	/* the usual stuff upfront, xml PI */
	p = sncpy(p, ep, xml_pre, sizeof(xml_pre) - 1);
	/* newline this one, all other tags will have no indentation */
	*p++ = '\n';
	/* ... and open our tag */
	p = sncpy(p, ep, fixml_pre, sizeof(fixml_pre) - 1);

	/* there ought to be just one message in there innit? */
	p += __render_msgtyp(p, ep - p, msg);

	/* final verdict */
	p = sncpy(p, ep, fixml_post, sizeof(fixml_post));
	return p - buf - 1/*final \nul*/;
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
