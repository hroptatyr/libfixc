/*** fix.c -- guts for fix fields and messages
 *
 * Copyright (C) 2010-2013 Sebastian Freundt
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
/* check for me? */
#include <sys/mman.h>

#if defined HAVE_ZLIB_H
# include <zlib.h>
#endif	/* HAVE_ZLIB_H */

#include "fix.h"
#include "fix-private.h"
#include "fixml-comp-sub.h"
#include "nifty.h"

#include "fixml-canon-attr.h"

/* to map version strings to fixc_ver_t objects */
#include "fix-ver.c"
/* to map fixc_ver_t objects to strings */
#include "fixml-nsuri-rev.c"

#if defined DEBUG_FLAG
# include <assert.h>
# define FIXC_DEBUG(args...)	fprintf(stderr, args)
# define FIXC_DEBUG_MEM(args...)
#else  /* !DEBUG_FLAG */
# define assert(args...)
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

typedef enum {
	KEY,
	VALUE,
	ERROR,
} kv_state_t;


/* fix guts */
#define SOH	"\001"
#define SOHC	(*SOH)

static inline size_t
__next_2power(size_t v)
{
/* round N to the next 2-power */
	v--;
	v |= v >> 1U;
	v |= v >> 2U;
	v |= v >> 4U;
	v |= v >> 8U;
	v |= v >> 16U;
#if SIZEOF_SIZE_T >= 8
	v |= v >> 32U;
#endif	/* SIZEOF_SIZE_T >= 8 */
#if SIZEOF_SIZE_T >= 16
	v |= v >> 64U;
#endif	/* SIZEOF_SIZE_T >= 16 */
	return ++v;
}

static inline size_t
__allocd_vspc(fixc_msg_t msg)
{
/* return space allocated for values (in bytes) */
	return __next_2power(msg->pz + 1);
}

static inline size_t
__allocd_fspc(fixc_msg_t msg)
{
/* return space allocated for fields (in numbers) */
	return (msg->pr - (char*)msg->flds) / sizeof(*msg->flds);
}

static void
check_size(fixc_msg_t msg, size_t add_flds, size_t add_vspc)
{
/* check if MSG can hold ADD_FLDS additional fields and ADD_VSPC
 * additional pr size, if not, resize */
	size_t fspc, fspc_nu;
	size_t vspc, vspc_nu;
	size_t UNUSED(old_sz);
	size_t new_sz;

	/* let's hope msg->pr is aligned, fingers crossed */
	fspc = __allocd_fspc(msg);
	/* determine the size of the pr section, multiple of VSPC_RND */
	vspc = __allocd_vspc(msg);

	if (add_flds + msg->nflds < fspc &&
	    add_vspc + msg->pz < vspc) {
		/* nothing to do, time for trip home */
		return;
	}

	/* find out how big the whole dynamic room was */
	old_sz = vspc + fspc * sizeof(*msg->flds);
	/* just to make sure we're talking the same sizes */
	vspc_nu = __next_2power(vspc + add_vspc + 1);
	fspc_nu = __next_2power(msg->nflds + add_flds);
	new_sz = vspc_nu + fspc_nu * sizeof(*msg->flds);

	/* grrr, otherwise there's lots of work to do :/ */
	FIXC_DEBUG_MEM("resz %zu %zu -> ~%zu ~%zu  i.e. %zub -> %zub\n",
		       fspc, vspc, fspc_nu, vspc_nu, old_sz, new_sz);

	{
		/* malloc them guys */
		size_t mvz = msg->nflds * sizeof(*msg->flds);
		fixc_fld_t new_flds;
		void *new_pr;

		assert(msg->nflds < fspc_nu);
		assert(mvz + msg->pz < new_sz);

		if (msg->nflds < fspc) {
			/* the extra message in mvz is for anal()'s state */
			mvz += sizeof(*msg->flds);
		}

		new_flds = malloc(new_sz);
		memcpy(new_flds, msg->flds, mvz);

		/* also move the pr stuff a bit */
		new_pr = new_flds + fspc_nu;
		memcpy(new_pr, msg->pr, msg->pz);

		/* make sure not to free the flexible array */
		if (msg->flds != msg->these) {
			free(msg->flds);
		}
		/* reass and out */
		msg->flds = new_flds;
		msg->pr = new_pr;
	}
	FIXC_DEBUG_MEM("=> %p [%zu]  %p [%zu]\n",
		       msg->flds, msg->nflds, msg->pr, msg->pz);
	return;
}

static uint16_t
__strtoui16(const char *s, size_t z)
{
	unsigned int res = 0U;

	for (size_t i = 0; i < z; i++) {
		res *= 10U;
		res += *s++ - '0';
	}
	return (uint16_t)(res < 65536U ? res : 0U);
}

static struct fixc_fld_s
fixc_parse_tag(const char *str, size_t len)
{
	struct fixc_fld_s res = {0};

	res.tag = __strtoui16(str, len);
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
		{
			const struct fixc_vstr_s *p = __fix_verify(str, len);
			/* we should properly parse this */
			msg->f8.ver = p != NULL ? p->ver : FIXC_VER_UNK;
		}
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


/* ctors and dtors */
fixc_msg_t
make_fixc_msg(fixc_ctxt_t ctx)
{
	fixc_msg_t res;

	/* generate the husk */
	res = calloc(1, sizeof(*res));

	res->f35.tag = FIXC_MSG_TYPE;
	if (LIKELY(ctx.ui16 > 0x2000)) {
		res->f35.typ = FIXC_TYP_MSGTYP;
	} else {
		res->f35.typ = FIXC_TYP_CTXT;
	}
	res->f35.ctx = ctx;
	return res;
}

#if defined HAVE_ZLIB_H
/* forward for anal() */
static int __fixc_from_fixz();
#endif	/* HAVE_ZLIB_H */

static kv_state_t
anal(fixc_msg_t res, char *str, size_t *ssz, kv_state_t st)
{
	const char *const ep = str + *ssz;
	const char *q = str;

#define CUR_FLD		(res->flds[res->nflds])
	for (char *p = str; p < ep;) {
		switch (st) {
		case KEY:
			while (*p++ != '=' && p < ep);

			if (p < ep) {
				/* finish string q */
				p[-1] = '\0';
				CUR_FLD = fixc_parse_tag(q, p - 1 - q);

				/* switch to value state */
				st = VALUE;
				q = p;
			} else {
				goto out;
			}
			break;
		case VALUE:
			while (*p++ != SOHC && p < ep);

			if (p < ep) {
				/* finish string q */
				p[-1] = '\0';

			succ:
				/* add this field */
				fixc_parse_fld(res, q, p - 1 - q);

				/* switch to key state */
				st = KEY;
				q = p;
			} else if (CUR_FLD.tag == FIXC_CHECK_SUM) {
				/* last field in a message, always succeed */
				goto succ;
			} else {
				goto out;
			}
			break;
		case ERROR:
		default:
			/* don't think we should recover from an error, aye? */
			goto out;
		}

		/* check for compressed message */
		if (res->f8.ver == FIXC_VER_COMP && res->f9.i32 > 0) {
			/* must be FIXC_VER_COMP then */
#if defined HAVE_ZLIB_H
			size_t rz = *ssz - (q - res->pr);

			FIXC_DEBUG("\
compressed message (of size %db v %zu) detected\n", res->f9.i32, rz);
			if (__fixc_from_fixz(res, q, rz) < 0) {
				return ERROR;
			}
			return KEY;
#else  /* HAVE_ZLIB_H */
			FIXC_DEBUG("\
compressed message (of size %db) detected but no zlib support\n", res->f9.i32);
			return ERROR;
#endif	/* HAVE_ZLIB_H */
		}
	}
#undef CUR_FLD
out:
	*ssz = q - str;
	return st;
}

#if defined HAVE_ZLIB_H
static int
__fixc_from_fixz(fixc_msg_t res, char *msg, size_t msglen)
{
	kv_state_t st = KEY;
	/* if msg was completely of the form N=V\1 we'd have 4 chars per fld */
	struct z_stream_s strm[1];
	static char tb[1024];
	size_t nproc;
	size_t unproc = 0UL;
	size_t inproc = 0UL;
	int finp = 0;

	/* reset fields */
	res->f8.ver = FIXC_VER_UNK;
	res->f9.i32 = 0;

	res->pz = 0UL;

	/* initialise the zlib stuff */
	strm->zalloc = NULL;
	strm->zfree = NULL;
	strm->opaque = NULL;

	if (inflateInit(strm) != Z_OK) {
		return -1;
	}

	do {
		strm->next_in = (Bytef*)msg + inproc;
		strm->avail_in = msglen - inproc;

		strm->next_out = (Bytef*)tb;
		strm->avail_out = sizeof(tb);

		switch (inflate(strm, Z_SYNC_FLUSH)) {
		case Z_OK:
			/* yay */
			FIXC_DEBUG("partial\n");
			break;
		case Z_STREAM_END:
			if (inflateEnd(strm) == Z_OK) {
				FIXC_DEBUG("complete\n");
				finp = 1;
				break;
			}
		default:
			return -1;
		}

		/* reset them fields */
		nproc = sizeof(tb) - strm->avail_out;
		inproc = msglen - strm->avail_in;
		res->f8.ver = FIXC_VER_UNK;
		res->f9.i32 = 0;

		FIXC_DEBUG("run +%zu  <-%zu  ->%zu\n", res->pz, inproc, nproc);

		/* check i/o sizes */
		check_size(res, nproc / 8, nproc);

		/* generate the husk */
		memcpy(res->pr + res->pz, tb, nproc);
		res->pr[res->pz + nproc] = '\0';

		/* update prevproc */
		res->pz += nproc;

		/* try and parse */
		nproc = res->pz - unproc;
		st = anal(res, res->pr + unproc, &nproc, st);

		/* update unproc */
		unproc += nproc;
	} while (!finp);

	res->pz = unproc;
	return 0;
}
#endif	/* HAVE_ZLIB_H */

fixc_msg_t
make_fixc_from_fix(const char *msg, size_t msglen)
{
#define ROUNDv(x)	ROUND(x, sizeof(void*))
	kv_state_t st = KEY;
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

	if (anal(res, res->pr, &msglen, st)) {
		/* error case, only entered upon zlib handling */
		free_fixc(res);
		res = NULL;
	}
	return res;
}

void
free_fixc(fixc_msg_t msg)
{
	if (msg->flds != NULL && msg->flds != msg->these) {
		free(msg->flds);
		msg->flds = NULL;
	}
	free(msg);
	return;
}


/* rendering */
static uint8_t
fixc_chksum(const char *str, size_t len)
{
        unsigned int res = 0;
        for (const char *p = str, *ep = str + len; p < ep; res += *p++);
        return (uint8_t)(res & 0xff);
}

static size_t
__ui16tostr(char *const buf, size_t bsz, uint16_t v)
{
	char *restrict p = buf;
	unsigned int log = 0U;

	if (UNLIKELY(v >= 10000)) {
		log = bsz >= 6U ? 5U : 0U;
	} else if (v >= 1000 && bsz >= 5U) {
		log = bsz >= 5U ? 4U : 0U;
	} else if (v >= 100) {
		log = bsz >= 4U ? 3U : 0U;
	} else if (v >= 10) {
		log = bsz >= 3U ? 2U : 0U;
	} else {
		log = bsz >= 2U ? 1U : 0U;
	}

	switch (log) {
	case 5:
		*p++ = (char)((v / 10000U % 10U) + '0');
	case 4:
		*p++ = (char)((v / 1000U % 10U) + '0');
	case 3:
		*p++ = (char)((v / 100U % 10U) + '0');
	case 2:
		*p++ = (char)((v / 10U % 10U) + '0');
	case 1:
		*p++ = (char)((v / 1U % 10U) + '0');
		*p++ = '=';
		break;
	case 0:
	default:
		*p = '\0';
		break;
	}
	return p - buf;
}

static size_t
__ui8tostr(char *const buf, size_t bsz, uint16_t v)
{
	char *restrict p = buf;

	if (bsz < 3U) {
		return 0UL;
	}

	*p++ = (char)((v / 100U % 10U) + '0');
	*p++ = (char)((v / 10U % 10U) + '0');
	*p++ = (char)((v / 1U % 10U) + '0');
	return 3UL;
}

static size_t
fixc_render_fld(
	char *restrict buf, size_t bsz, const char *b, struct fixc_fld_s fld)
{
	size_t stz;
	size_t res = 0;

	res = __ui16tostr(buf, bsz, fld.tag);;
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
		res += __ui8tostr(buf + res, bsz - res, fld.u8);
		break;
	case FIXC_TYP_CHAR:
		buf[res++] = fld.c;
		break;
	case FIXC_TYP_INT:
		res += snprintf(buf + res, bsz - res, "%" PRIi32, fld.i32);
		break;
	case FIXC_TYP_LONG:
		res += snprintf(buf + res, bsz - res, "%" PRIi64, fld.i64);
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
	case FIXC_TYP_PTR:
		/* implement me */
		abort();

	case FIXC_TYP_CTXT:
		/* shall we allow that? it's a msg snippet as opposed to
		 * a proper msg.  well do what default would do */
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
		totz += fixc_render_fld(
			buf + totz, bsz - totz, msg->pr, msg->f10);
		/* no final SOH here */
		totz--;
	}

	buf[totz] = '\0';
	return totz;
}

static inline struct fixc_rndr_s
__round_to_align(struct fixc_rndr_s rbuf)
{
	intptr_t x = (intptr_t)rbuf.str;
	size_t off;

	if ((off = x % (2 * sizeof(void*)))) {
		rbuf.str = (char*)(x - off);
		rbuf.len += off;
	}
	return rbuf;
}

static void
resz_rndr(char **buf, size_t *bsz)
{
	/* just double the whole thing */
	if (*buf == NULL) {
		/* start mmap page */
		size_t naz = *bsz;
		char *nu = mmap(NULL, naz, PROT_MEM, MAP_MEM, -1, 0);
		*buf = nu;
		*bsz = naz;
	} else {
		/* simples */
		size_t oaz = *bsz;
		size_t naz = 2 * *bsz;

#if defined MREMAP_MAYMOVE
		*buf = mremap(*buf, oaz, naz, MREMAP_MAYMOVE);
#else  /* !MREMAP_MAYMOVE */
		char *nu = mmap(NULL, naz, PROT_MEM, MAP_MEM, -1, 0);
		memcpy(nu, *buf, *bsz);
		munmap(*buf, oaz);
		*buf = nu;
#endif	/* MREMAP_MAYMOVE */
		*bsz = naz;
	}
	return;
}

struct fixc_rndr_s
fixc_render_fix_rndr(fixc_msg_t msg)
{
	char *buf;
	size_t bsz;
	size_t hdrz;
	size_t lenz;
	size_t totz;
	size_t blen = 0;

	/* try and estimate the size of the buffer
	 * each field will have a SOH, a `=' and some tag number */
	bsz = (4 + msg->nflds) * (5/*tag number*/ + 1/*=*/ + 1/*SOH*/ + 1) +
		/* slight optimisation for msgs spanning the pr space already */
		msg->pz +
		/* for the size (tag #9) and the checksum (tag #10) */
		16 + 3;

	/* aaah, prefer mmap() */
	buf = mmap(NULL, bsz, PROT_MEM, MAP_MEM, -1, 0);

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
		/* need resize? */
		if (UNLIKELY((bsz - blen) * 10UL / bsz < 1)) {
			/* oh fuck, we're in the last 10% */
			resz_rndr(&buf, &bsz);
		}

		blen += fixc_render_fld(
			buf + totz + blen, bsz - blen, msg->pr, msg->flds[i]);
	}
	/* undo room leaving */
	bsz += totz;

	/* go back to fld #9 and paste the right length
	 * instead of moving the whole shebang, just move the beginning
	 * of the buffer */
	lenz = snprintf(buf + hdrz + 2/*for 9=*/, lenz - 1, "%zu", blen - 1);
	buf[hdrz += 2 + lenz] = SOHC;
	{
		size_t off = totz - hdrz - 1;

		/* compute new totz now */
		totz = hdrz + 1 + blen;

#if 0
/* downsizing is no more */
		/* if mmap is in place, downsize to multiple of totz */
		{
#if defined MREMAP_MAYMOVE
			size_t naz = totz + 6/*for footer*/;
			buf = mremap(buf, bsz, naz, MREMAP_MAYMOVE);
			bsz = naz;
#else  /* !MREMAP_MAYMOVE */
			/* um, good question, another mmap? :O */
			;
#endif	/* MREMAP_MAYMOVE */
		}
#endif	/* 0 */

		/* memmove the buffer so it conincides with the start */
		memmove(buf + off, buf, hdrz + 1);
		buf += off;
	}

	/* compute and paste checksum */
	{
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
		totz += fixc_render_fld(
			buf + totz, bsz - totz, msg->pr, msg->f10);
		/* no final SOH here */
		totz--;
	}

	buf[totz] = '\0';
	return (struct fixc_rndr_s){.str = buf, .len = totz, .z = bsz};
}

void
fixc_free_rndr(struct fixc_rndr_s rbuf)
{
	/* compute the right rbuf */
	rbuf = __round_to_align(rbuf);

	/* we mmapped it */
	munmap(rbuf.str, rbuf.z);
	return;
}


/* adding stuff */
size_t
fixc_msg_z(fixc_msg_t msg)
{
	/* field space */
	size_t fspc;
	/* value space */
	size_t vspc;
	size_t res;

	/* let's hope msg->pr is aligned, fingers crossed */
	fspc = (msg->pr - (char*)msg->flds) / sizeof(*msg->flds);
	/* determine the size of the pr section, multiple of VSPC_RND */
	vspc = ROUND(msg->pz + 1, VSPC_RND);

	res = vspc + fspc * sizeof(*msg->flds) + sizeof(*msg);
	return res;
}

size_t
fixc_msg_optz(fixc_msg_t msg)
{
/* like fixc_msg_z() but return the optimal space needed to hold MSG */
	/* field space */
	size_t fspc;
	/* value space */
	size_t vspc;
	size_t res;

	/* let's hope msg->pr is aligned, fingers crossed */
	fspc = ROUND(msg->nflds, FSPC_RND);
	/* determine the size of the pr section, multiple of VSPC_RND */
	vspc = ROUND(msg->pz + 1, VSPC_RND);

	res = vspc + fspc * sizeof(*msg->flds) + sizeof(*msg);
	return res;
}

size_t
fixc_msg_minz(fixc_msg_t msg)
{
/* like fixc_msg_z() but return the minimum space needed to hold MSG */
	/* field space */
	size_t fspc;
	/* value space */
	size_t vspc;
	size_t res;

	/* let's hope msg->pr is aligned, fingers crossed */
	fspc = msg->nflds;
	/* determine the size of the pr section, multiple of VSPC_RND */
	vspc = msg->pz + 1;

	res = vspc + fspc * sizeof(*msg->flds) + sizeof(*msg);
	return res;
}

size_t
fixc_msg_cpy(void *restrict tgt, size_t tsz, fixc_msg_t msg)
{
	struct fixc_msg_s *restrict tmsg = tgt;
	size_t fspc = msg->nflds * sizeof(*msg->flds);
	size_t tailz = fixc_msg_minz(msg);

	if (UNLIKELY(tsz < tailz)) {
		return 0UL;
	}

	if (msg->flds == msg->these) {
		/* great, it's just one big shlong, but we'll optimise
		 * the whole shebang anyway */
		memcpy(tmsg, msg, sizeof(*msg) + fspc);
	} else {
		/* chunk copies */
		/* copy the head-bit */
		memcpy(tmsg, msg, sizeof(*msg));

		/* copy them fields */
		memcpy(tmsg->flds = tmsg->these, msg->flds, fspc);
	}
	/* reset the new pr */
	tmsg->pr = (void*)(tmsg->flds + msg->nflds);
	memcpy(tmsg->pr, msg->pr, msg->pz + 1);
	return tailz;
}

int
fixc_add_fld(fixc_msg_t msg, struct fixc_fld_s fld)
{
	return fixc_add_fld_at(msg, fld, msg->nflds);
}

int
fixc_add_fld_at(fixc_msg_t msg, struct fixc_fld_s fld, size_t idx)
{
	/* see if someone wants us to add offset fields */
	if (fld.typ == FIXC_TYP_OFF) {
		return -1;
	} else if (idx > msg->nflds) {
		return -1;
	}

	/* see if someone tricks us into adding the special fields */
	switch (fld.tag) {
	case FIXC_TAG_UNK:
		return -1;
	case FIXC_MSG_TYPE:
		if (msg->f35.mtyp == FIXC_MSGT_UNK) {
			/* if f35 isn't batch, just overwrite the f35 slot */
			msg->f35 = fld;
			break;
		} else if (msg->f35.mtyp != FIXC_MSGT_BATCH) {
			/* nice one */
			struct fixc_fld_s f35 = msg->f35;

			/* firstly reassign f35 slot in msg so
			 * the recursion downstairs can succeed */
			msg->f35.tpc = msg->f35.cnt = 0;
			msg->f35.mtyp = FIXC_MSGT_BATCH;

			f35.cnt = 0;
			f35.tpc = FIXC_MSGT_BATCH;
			fixc_add_fld_at(msg, f35, 0);
		}
	default:
		/* check if there's enough for one more field */
		check_size(msg, 1UL, 0UL);

		/* move all fields from idx to nflds out of the way */
		if (LIKELY(idx < msg->nflds)) {
			size_t nmv = (msg->nflds - idx);
			size_t nmv_b = nmv * sizeof(*msg->flds);
			memmove(msg->flds + idx + 1, msg->flds + idx, nmv_b);
		}

		/* in total the number got up'd by 1 */
		msg->nflds++;

		/* finally time to adopt this fld */
		msg->flds[idx] = fld;
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
	}
	return 0;
}

int
fixc_add_tag(fixc_msg_t msg, fixc_attr_t tag, const char *val, size_t vsz)
{
	return fixc_add_tag_at(msg, tag, val, vsz, msg->nflds);
}

int
fixc_add_tag_at(
	fixc_msg_t msg, fixc_attr_t tag,
	const char *val, size_t vsz, size_t idx)
{
	/* check that it's not non-sense */
	if (UNLIKELY(idx > msg->nflds)) {
		return -1;
	}

	/* see if someone tricks us into adding the special fields */
	switch ((unsigned int)tag) {
	case FIXC_TAG_UNK:
	case FIXC_BEGIN_STRING:
	case FIXC_BODY_LENGTH:
	case FIXC_CHECK_SUM:
		return -1;
	case FIXC_MSG_TYPE:
		if (msg->f35.mtyp == FIXC_MSGT_UNK) {
			/* if f35 isn't batch, just overwrite the f35 slot */
			msg->f35.tag = FIXC_MSG_TYPE;
			msg->f35.typ = FIXC_TYP_MSGTYP;
			msg->f35.tpc = 0;
			msg->f35.cnt = 0;
#define _(x, y)		((fixc_msgt_t)((x) * 256U + (y) * 1U))
			if (vsz == 1) {
				msg->f35.mtyp = _(val[0], 0);
			} else if (vsz == 2) {
				msg->f35.mtyp = _(val[0], val[1]);
			} else {
				/* um, we should die really */
				;
			}
#undef _
			break;
		} else if (msg->f35.mtyp != FIXC_MSGT_BATCH) {
			/* fuck */
			struct fixc_fld_s f35 = msg->f35;

			/* firstly reassign f35 slot in msg so
			 * the recursion downstairs can succeed */
			msg->f35.tpc = msg->f35.cnt = 0;
			msg->f35.mtyp = FIXC_MSGT_BATCH;

			f35.cnt = 0;
			f35.tpc = FIXC_MSGT_BATCH;
			fixc_add_fld_at(msg, f35, 0);
		}
	default:
		/* check if there's enough for one more field and vsz bytes */
		check_size(msg, 1UL, vsz + 1UL);

		/* move all fields from idx to nflds out of the way */
		if (LIKELY(idx < msg->nflds)) {
			size_t nmv = (msg->nflds - idx);
			size_t nmv_b = nmv * sizeof(*msg->flds);
			memmove(msg->flds + idx + 1, msg->flds + idx, nmv_b);
		}

		/* in total the number of fields is up by 1 */
		msg->nflds++;

		/* finally time to adopt this fld */
		msg->flds[idx].tag = (uint16_t)tag;
		msg->flds[idx].typ = FIXC_TYP_OFF;
		msg->flds[idx].off = msg->pz;
		msg->flds[idx].tpc = msg->flds[idx].cnt = 0U;
		memcpy(msg->pr + msg->pz, val, vsz);
		msg->pr[msg->pz += vsz] = '\0';
		msg->pz++;
		break;
	}
	return 0;
}

void
fixc_del_fld(fixc_msg_t msg, size_t n)
{
	if (n >= msg->nflds) {
		return;
	} else if (n + 1 == msg->nflds) {
		msg->nflds--;
	}
	msg->flds[n].tag = FIXC_ATTR_UNK;
	return;
}

const char*
fixc_get_tag(fixc_msg_t msg, size_t idx)
{
	if (UNLIKELY(idx >= msg->nflds)) {
		return NULL;
	} else if (UNLIKELY(msg->flds[idx].typ != FIXC_TYP_OFF)) {
		return NULL;
	}
	/* otherwise go ahead */
	return msg->pr + msg->flds[idx].off;
}

struct fixc_tag_data_s
fixc_get_tag_data(fixc_msg_t msg, size_t idx)
{
	static const struct fixc_tag_data_s nul = {
		.s = NULL,
		.z = 0UL,
	};

	if (UNLIKELY(idx >= msg->nflds)) {
		return nul;
	} else if (UNLIKELY(msg->flds[idx].typ != FIXC_TYP_OFF)) {
		return nul;
	}
	/* otherwise go ahead */
	{
		size_t off = msg->flds[idx].off;
		struct fixc_tag_data_s res = {
			.s = msg->pr + off,
		};

		/* SLIGHT optimisation of just pulling off strlen() */
		if (idx + 1 == msg->nflds) {
			res.z = msg->pz - off - 1;
		} else if (msg->flds[idx + 1].typ == FIXC_TYP_OFF) {
			/* let's hope they're consecutive */
			res.z = msg->flds[idx + 1].off - off - 1;
		} else {
			res.z = strlen(res.s);
		}
		return res;
	}
}


/* extraction */
static void
__cpy_fld(fixc_msg_t tgt, fixc_msg_t src, size_t idx)
{
	struct fixc_fld_s fld = src->flds[idx];

	if (fld.typ == FIXC_TYP_OFF) {
		struct fixc_tag_data_s d = fixc_get_tag_data(src, idx);
		size_t jdx = tgt->nflds;

		fixc_add_tag(tgt, (fixc_attr_t)fld.tag, d.s, d.z);
		/* copy them .tpc and .cnt slots if possible */
		tgt->flds[jdx].tpc = fld.tpc;
		tgt->flds[jdx].cnt = fld.cnt;
	} else {
		fixc_add_fld(tgt, fld);
	}
	return;
}

fixc_msg_t
fixc_extr_ctxt(fixc_msg_t msg, fixc_ctxt_t ctx, int n)
{
	fixc_msg_t res = make_fixc_msg(ctx);

	/* check if we've got tpcs and cnts */
	if (fixc_msg_needs_fixup_p(msg)) {
		fixc_fixup(msg);
	}

	/* all them attrs belonging in context CID */
	for (size_t i = 0, cnt = 0, last = 0; i < msg->nflds; i++) {
		struct fixc_fld_s fld = msg->flds[i];

		if (fld.tpc == ctx.ui16) {
			if (fld.cnt < last) {
				cnt++;
			}
			/* keep track of consecutive counter */
			last = fld.cnt;

			/* check if we're the N-th occurrence really */
			if (n >= 0) {
				if (cnt > (size_t)n) {
					break;
				} else if (cnt < (size_t)n) {
					continue;
				}
			}

			/* oh yes we are, snarf it */
			__cpy_fld(res, msg, i);
		}
	}
	return res;
}

static bool
__ancestp(fixc_comp_sub_t ancest, fixc_ctxt_t desc)
{
	if (ancest->ctx == desc.ui16) {
		return true;
	}
	for (size_t i = 0; i < ancest->nsubs; i++) {
		if (ancest->subs[i] == desc.ui16) {
			return true;
		}
	}
	return false;
}

fixc_msg_t
fixc_extr_ctxt_deep(fixc_msg_t msg, fixc_ctxt_t ctx, int n)
{
	fixc_msg_t res = make_fixc_msg(ctx);
	fixc_comp_sub_t chld = fixc_get_comp_sub(ctx);
	int just_been_in = 0;

	/* check if we've got tpcs and cnts */
	if (fixc_msg_needs_fixup_p(msg)) {
		fixc_fixup(msg);
	}

	/* all them attrs belonging in context CID */
	for (size_t i = 0, cnt = 0, last = 0; i < msg->nflds; i++) {
		struct fixc_fld_s fld = msg->flds[i];

		if (fld.tpc == ctx.ui16) {
			if (fld.cnt < last) {
				cnt++;
			}
			/* keep track of consecutive counter */
			last = fld.cnt;

			/* check if we're the N-th occurrence really */
			if (n >= 0) {
				if (cnt > (size_t)n) {
					break;
				} else if (cnt < (size_t)n) {
					continue;
				}
			}

			/* oh yes we are, snarf it */
			__cpy_fld(res, msg, i);

			/* set the just_been_in yield */
			just_been_in = 1;

		} else if (just_been_in) {
			while (fld.tpc != ctx.ui16 &&
			       __ancestp(chld, (fixc_comp_t)fld.tpc)) {
				/* splendid! */
				uint16_t chctx = fld.tpc;

				while (fld.tpc == chctx) {
					__cpy_fld(res, msg, i);
					fld = msg->flds[++i];
				}
			}
			/* reset the yield */
			just_been_in = 0;
			/* rewind */
			i--;
		}
	}
	return res;
}

/* fix.c ends here */
