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

/* to map fixc_ver_t objects to strings */
#include "fixml-nsuri-rev.c"

#if defined DEBUG_FLAG
# define FIXC_DEBUG(args...)	fprintf(stderr, args)
# define FIXC_DEBUG_MEM(args...)
#else  /* !DEBUG_FLAG */
# define FIXC_DEBUG(args...)
# define FIXC_DEBUG_MEM(args...)
#endif	/* DEBUG_FLAG */

/* value we like our vspc to be rounded to */
#define VSPC_RND	(128)
/* value we like our fspc (the fields) to be rounded to */
#define FSPC_RND	(32)

#if !defined CHAR_BIT
# define CHAR_BIT	(8U)
#endif	/* !CHAR_BIT */


/* fix guts */
#define SOH	"\001"
#define SOHC	(*SOH)

static struct fixc_fld_s
fixc_parse_tag(const char *str, size_t UNUSED(len))
{
	long unsigned int tmp;
	struct fixc_fld_s res = {0};

	/* will be replace with our own reader */
	if ((tmp = strtoul(str, NULL, 10)) < 65536) {
		res.tag = tmp;
	}
	return res;
}

static fixc_msgt_t
fixc_parse_mtyp(const char str[static 1], size_t len)
{
	unsigned int res = FIXC_MSGT_UNK;

	switch (len) {
	case 0:
	default:
		break;
	case 1:
	case 2:
		res |= (unsigned int)(str[0] & 0xff) << CHAR_BIT;
		res |= (unsigned int)(str[1] & 0xff) << 0U;
		break;
	}
	return (fixc_msgt_t)(res);
}

static void
fixc_parse_fld(fixc_msg_t msg, const char *str, size_t len)
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
		msg->f35.typ = FIXC_TYP_MSGTYP;
		msg->f35.mtyp = fixc_parse_mtyp(str, len);
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
make_fixc_from_fix(const char *msg, size_t msglen)
{
#define ROUNDv(x)	ROUND(x, sizeof(void*))
	enum {
		KEY,
		VALUE,
	} kv_state = KEY;
	/* if msg was completely of the form N=V\1 we'd have 4 chars per fld */
	fixc_msg_t res;
	size_t base = sizeof(*res);
	size_t fspc = ROUND(msglen / 4, FSPC_RND);
	size_t vspc = ROUND(msglen + 1, VSPC_RND);
	size_t totz = ROUNDv(base + fspc * sizeof(*res->flds) + vspc);

	/* generate the husk */
	res = malloc(totz);
	memset(res, 0, sizeof(*res));
	res->flds = res->these;
	res->pr = (void*)(res->flds + fspc);
	res->pz = msglen;
	memcpy(res->pr, msg, msglen);
	res->pr[msglen] = '\0';

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

void
free_fixc(fixc_msg_t msg)
{
	if (msg->flds && msg->flds != msg->these) {
		free(msg->flds);
		msg->flds = NULL;
	}
	free(msg);
	return;
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
	case FIXC_TYP_VER: {
		const char *vstr = __ver_fixify(fld.ver);
		size_t vlen = strlen(vstr);
		memcpy(buf + res, vstr, vlen);
		res += vlen;
		break;
	}
	case FIXC_TYP_UCHAR:
		buf[res++] = fld.u8;
		break;
	case FIXC_TYP_CHAR:
		buf[res++] = fld.i8;
		break;
	case FIXC_TYP_INT:
		res += snprintf(buf + res, bsz - res, "%" PRIi32, fld.i32);
		break;

	case FIXC_TYP_MSGTYP:
		/* coincidentally the value of mtyp corresponds to the
		 * ascii representation already */
		if ((buf[res] = (uint8_t)((fld.mtyp & 0xff00) >> CHAR_BIT))) {
			res++;
		}
		if ((buf[res] = (uint8_t)((fld.mtyp & 0x00ff) >> 0U))) {
			res++;
		}
		break;
	default:
		break;
	}
	buf[res] = SOHC;
	return res + 1;
}

size_t
fixc_render_fix(char *restrict buf, size_t bsz, fixc_msg_t msg)
{
	size_t hdrz;
	size_t lenz;
	size_t totz;
	size_t blen = 0;

	/* first 2 fields are unrolled */
	msg->f8.tag = FIXC_BEGIN_STRING;
	hdrz = fixc_render_fld(buf, bsz, msg->pr, msg->f8);
	msg->f9.tag = FIXC_BODY_LENGTH;
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
		msg->f10 = (struct fixc_fld_s){
			.tag = FIXC_CHECK_SUM,
			.typ = FIXC_TYP_UCHAR,
#if defined HAVE_ANON_STRUCTS_INIT
			.u8 = fixc_chksum(buf, totz),
#endif	/* HAVE_ANON_STRUCTS_INIT */
		};
#if !defined HAVE_ANON_STRUCTS_INIT
/* thanks gcc */
		msg->f10.u8 = fixc_chksum(buf, totz);
#endif	/* !HAVE_ANON_STRUCTS_INIT */
		fixc_render_fld(buf + totz, bsz - totz, msg->pr, msg->f10);
		/* no final SOH here */
		totz += 4;
	}

	buf[totz] = '\0';
	return totz;
}

static void
check_size(fixc_msg_t msg, size_t add_flds, size_t add_vspc)
{
/* check if MSG can hold ADD_FLDS additional fields and ADD_VSPC
 * additional pr size, if not, resize */
	size_t fspc;
	size_t vspc;
	size_t old_sz;
	size_t add_sz;
	void *old_flds = NULL;
	void *new_flds;
	void *new_pr;

	/* let's hope msg->pr is aligned, fingers crossed */
	fspc = (msg->pr - (char*)msg->flds) / sizeof(*msg->flds);
	/* determine the size of the pr section, multiple of VSPC_RND */
	vspc = ROUND(msg->pz + 1, VSPC_RND);

	if (add_flds + msg->nflds < fspc &&
	    add_vspc + msg->pz < vspc) {
		/* nothing to do, time for trip home */
		return;
	}
	/* grrr, otherwise there's lots of work to do :/ */
	FIXC_DEBUG_MEM("resz %zu %zu -> ~%zu ~%zu\n",
		       fspc, vspc, fspc + add_flds, vspc + add_vspc);

	/* find out how big the whole dynamic room was */
	old_sz = vspc + fspc * sizeof(*msg->flds);
	/* leave room for FSPC_RND new fields and VSPC_RND bytes msg */
	add_sz = ROUND(add_vspc, VSPC_RND) +
		ROUND(add_flds, FSPC_RND) * sizeof(*msg->flds);
	old_flds = NULL;

	/* make sure not to realloc the flexible array */
	if (msg->flds != msg->these) {
		old_flds = msg->flds;
	}
	/* final reallocing */
	new_flds = realloc(old_flds, ROUNDv(old_sz + add_sz));

	/* move the fields first */
	if (new_flds != old_flds) {
		size_t mvz = msg->nflds * sizeof(*msg->flds);
		memmove(new_flds, msg->flds, mvz);
		msg->flds = new_flds;
	}
	/* always move the pr */
	new_pr = msg->flds + ROUND(fspc + add_flds, FSPC_RND);
	memmove(new_pr, msg->pr, msg->pz);
	msg->pr = new_pr;
	return;
}

int
fixc_add_fld(fixc_msg_t msg, struct fixc_fld_s fld)
{
	/* see if someone wants us to add offset fields */
	if (fld.typ == FIXC_TYP_OFF) {
		return -1;
	}

	/* see if someone tricks us into adding the special fields */
	switch (fld.tag) {
	case FIXC_TAG_UNK:
		return -1;
	default:
	bang:
		/* check if there's enough room for another 4 msgs */
		check_size(msg, /*aribtrary hard-coded value*/4, 0);

		/* finally time to adopt this fld */
		msg->flds[msg->nflds++] = fld;
		break;
	case FIXC_BEGIN_STRING:
		/* don't bother checking the actual field */
		msg->f8 = fld;
		break;
	case FIXC_BODY_LENGTH:
		/* again, don't bother checking */
		msg->f9 = fld;
		break;
	case FIXC_CHECK_SUM:
		/* will be computed anyway */
		msg->f10 = fld;
		break;
	case FIXC_MSG_TYPE:
		if (msg->f35.mtyp == FIXC_MSGT_BATCH) {
			goto bang;
		}
		/* otherwise it's the main message type */
		msg->f35 = fld;
		break;
	}
	return 0;
}

int
fixc_add_tag(fixc_msg_t msg, uint16_t tag, const char *val, size_t vsz)
{
	/* see if someone tricks us into adding the special fields */
	switch (tag) {
		size_t cur;
	case FIXC_TAG_UNK:
	case FIXC_BEGIN_STRING:
	case FIXC_BODY_LENGTH:
	case FIXC_CHECK_SUM:
	case FIXC_MSG_TYPE:
		return -1;
	default:
		/* check if there's enough room for another 4 msgs */
		check_size(msg, /*aribtrary hard-coded value*/4, vsz + 1);

		/* finally time to adopt this fld */
		cur = msg->nflds++;
		msg->flds[cur].tag = tag;
		msg->flds[cur].typ = FIXC_TYP_OFF;
		msg->flds[cur].off = msg->pz;
		memcpy(msg->pr + msg->pz, val, vsz);
		msg->pr[msg->pz += vsz] = '\0';
		msg->pz++;
		break;
	}
	return 0;
}

/* fix.c ends here */
