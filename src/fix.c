/*** fix.c -- guts for fix fields and messages
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


/* fix guts */
#define SOH	"\001"
#define SOHC	(*SOH)

static struct fixc_fld_s
fixc_parse_tag(const char *str, size_t UNUSED(len))
{
	long unsigned int tmp;
	struct fixc_fld_s res;

	/* will be replace with our own reader */
	if ((tmp = strtoul(str, NULL, 10)) < 65536) {
		res.tag = tmp;
	} else {
		res.tag = FIXC_TAG_UNK;
	}
	return res;
}

static void
fixc_parse_fld(fixc_msg_t msg, const char *str, size_t UNUSED(len))
{
	size_t cur = msg->nflds;

	switch (msg->flds[cur].tag) {
	case FIXC_BEGIN_STRING:
		msg->f8.tag = FIXC_BEGIN_STRING;
		msg->f8.typ = FIXC_TYP_VER;
		/* we should properly parse this */
		msg->f8.ver = FIXC_VER_T11;
		break;
	case FIXC_BODY_LENGTH:
		msg->f9.tag = FIXC_BODY_LENGTH;
		msg->f9.typ = FIXC_TYP_INT;
		msg->f9.i32 = strtol(str, NULL, 10);
		break;
	case FIXC_CHECK_SUM:
		msg->f10.tag = FIXC_CHECK_SUM;
		msg->f10.typ = FIXC_TYP_UCHAR;
		msg->f10.u8 = str[0];
		break;
	case FIXC_MSG_TYPE:
		msg->f35.tag = FIXC_MSG_TYPE;
		msg->f35.typ = FIXC_TYP_OFF;
		msg->f35.off = str - msg->pr;
		break;
	default:
		msg->flds[cur].off = str - msg->pr;
		msg->nflds++;
	case FIXC_TAG_UNK:
		break;
	}
	return;
}

fixc_msg_t
make_fixc_msg(const char *msg, size_t msglen)
{
#define ROUNDv(x)	ROUND(x, sizeof(void*))
	enum {
		KEY,
		VALUE,
	} kv_state = KEY;
	/* if msg was completely of the form N=V\1 we'd have 4 chars per fld */
	fixc_msg_t res;
	size_t overhead = ROUNDv(msglen / 4 * sizeof(*res->flds) + sizeof(*res));
	size_t alloc_sz = ROUNDv(overhead + msglen + 1);

	/* generate the husk */
	res = malloc(alloc_sz);
	memset(res, 0, alloc_sz);
	res->pr = (char*)res->flds + overhead - sizeof(*res);
	res->pz = msglen;
	memcpy(res->pr, msg, msglen);

	for (char *p = res->pr, *ep = res->pr + msglen, *q = p; p <= ep; p++) {
		switch (kv_state) {
		case KEY:
			if (*p == '=') {
				size_t i = res->nflds;

				/* finish string q */
				*p = '\0';
				res->flds[i] = fixc_parse_tag(q, p - q);

				/* switch to value state */
				kv_state = VALUE;
				q = p + 1;
			}
			break;
		case VALUE:
			if (*p == SOHC || *p == '\0') {
				/* finish string q */
				*p = '\0';

				/* add this field */
				fixc_parse_fld(res, q, p - q);

				/* switch to key state */
				kv_state = KEY;
				q = p + 1;
			}
			break;
		default:
			break;
		}
	}
	return res;
}

static uint8_t
fixc_chksum(const char *str, size_t len)
{
        unsigned int res = 0;
        for (const char *p = str, *ep = str + len; p < ep; res += *p++);
        return (uint8_t)(res & 0xff);
}

static size_t
fixc_render_fld(
	char *restrict buf, size_t bsz, const char *b, struct fixc_fld_s fld)
{
	size_t stz;
	size_t res = 0;

	res = snprintf(buf, bsz, "%hu=", fld.tag);
	switch (fld.typ) {
	case FIXC_TYP_OFF:
		if ((stz = strlen(b + fld.off)) + 1 > bsz - res) {
			return 0;
		}
		memcpy(buf + res, b + fld.off, stz);
		res += stz;
		break;
	case FIXC_TYP_VER:
		memcpy(buf + res, "FIXT.1.1", 8);
		res += 8;
		break;
	case FIXC_TYP_UCHAR:
		buf[res++] = fld.u8;
		break;
	case FIXC_TYP_CHAR:
		buf[res++] = fld.i8;
		break;
	case FIXC_TYP_INT:
		res += snprintf(buf + res, bsz - res, "%" PRIi32, fld.i32);
		break;
	default:
		break;
	}
	buf[res] = SOHC;
	return res + 1;
}

size_t
fixc_render_msg(char *restrict buf, size_t bsz, fixc_msg_t msg)
{
	size_t hdrz;
	size_t lenz;
	size_t totz;
	size_t blen = 0;

	/* first 2 fields are unrolled */
	hdrz = fixc_render_fld(buf, bsz, msg->pr, msg->f8);
	lenz = fixc_render_fld(buf + hdrz, bsz - hdrz, msg->pr, msg->f9);
	/* just leave some room for this */
	totz = ROUND(hdrz + (lenz = ROUND(lenz, 8)), sizeof(void*));
	bsz -= totz;

	/* render message type */
	blen = fixc_render_fld(buf + totz, bsz, msg->pr, msg->f35);

	for (size_t i = 0; i < msg->nflds && blen < bsz; i++) {
		blen += fixc_render_fld(
			buf + totz + blen, bsz - blen, msg->pr, msg->flds[i]);
	}
	/* undo room leaving */
	bsz += totz;

	/* go back to fld #9 and paste the right length */
	lenz = snprintf(buf + hdrz + 2/*for 9=*/, lenz - 1, "%zu", blen - 1);
	buf[hdrz += 2 + lenz] = SOHC;
	memmove(buf + hdrz + 1, buf + totz, blen);

	/* compute totz now */
	totz = hdrz + 1 + blen;

	/* compute and paste checksum */
	if (totz + 5/*10=x\nul*/ < bsz) {
		msg->f10.u8 = fixc_chksum(buf, totz);
		fixc_render_fld(buf + totz, bsz - totz, msg->pr, msg->f10);
		/* no final SOH here */
		totz += 4;
	}

	buf[totz] = '\0';
	return totz;
}


#if defined STANDALONE
int
main(void)
{
	static char foo[] = "8=FIXT.1.1" SOH "9=0004" SOH
		"35=S" SOH "117=112" SOH
		"132=1.03" SOH "133=1.04" SOH "10=0";
	char test[256];
	fixc_msg_t msg = make_fixc_msg(foo, sizeof(foo) - 1);

	fprintf(stdout, "%zu fields\n", msg->nflds);
	for (size_t i = 0; i < msg->nflds; i++) {
		fprintf(stdout, "+ field %zu: %hu=%s\n",
			i, msg->flds[i].tag, msg->pr + msg->flds[i].off);
	}

	fixc_render_msg(test, sizeof(test), msg);
	fputs(test, stdout);
	fputc('\n', stdout);

	free(msg);
	return 0;
}
#endif	/* STANDALONE */

/* fix.c ends here */
