/*** fix.h -- guts for fix fields and messages
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
#if !defined INCLUDED_fix_h_
#define INCLUDED_fix_h_

#include <stdint.h>

typedef struct fixc_fld_s *fixc_fld_t;
typedef struct fixc_msg_s *fixc_msg_t;

/** known tags */
enum {
	FIXC_TAG_UNK,
	/* header */
	FIXC_BEGIN_STRING = 8,
	FIXC_BODY_LENGTH = 9,
	FIXC_MSG_TYPE = 35,
	FIXC_SENDER_COMP_ID = 49,
	FIXC_TARGET_COMP_ID = 56,
	FIXC_MSG_SEQ_NUM = 34,
	FIXC_MSG_SENDING_TIME = 52,

	/* trailer */
	FIXC_CHECK_SUM = 10,

	/* message specific */
	FIXC_REF_SEQ_NUM = 45,
	FIXC_ENCRYPT_METHOD = 98,
	FIXC_HEART_BT_INT = 108,
	FIXC_DEFAULT_APPL_VER_ID = 1137,
};

/** tag types */
enum {
	/** tag type unknown */
	FIXC_TYP_UNK,
	/** tag type corresponds to off slot */
	FIXC_TYP_OFF = FIXC_TYP_UNK,
	/** tag type is a version number */
	FIXC_TYP_VER,
	/** tag type is a unsigned char */
	FIXC_TYP_UCHAR,
	/** tag type is a signed char */
	FIXC_TYP_CHAR,
	/** tag type is a (32b) integer */
	FIXC_TYP_INT,
};

/** version numbers we support */
enum {
	FIXC_VER_UNK,
	FIXC_VER_40,
	FIXC_VER_41,
	FIXC_VER_42,
	FIXC_VER_43,
	FIXC_VER_44,
	FIXC_VER_50,
	FIXC_VER_50_SP1,
	FIXC_VER_50_SP2,
	FIXC_VER_T11,
};

struct fixc_fld_s {
	/** numerical representation of the fix field */
	uint16_t tag;
	/** fix field type, or generally 0 if not computed or known */
	uint16_t typ;
	/** offset (from the beginning of the message) to the value */
	union {
		uint32_t off;
		int32_t i32;
		uint8_t u8;
		int8_t i8;
		unsigned int ver;
	};
};

struct fixc_msg_s {
	/** printed representation */
	char *pr;
	/** length of the printed representation */
	size_t pz;

	/* fixed fields, must be present in every msg */
	struct fixc_fld_s f8;
	struct fixc_fld_s f9;
	struct fixc_fld_s f35;
	struct fixc_fld_s f10;

	/** number of fields */
	size_t nflds;
	fixc_fld_t flds;
	struct fixc_fld_s these[];
};

extern fixc_msg_t make_fixc_msg(const char *msg, size_t msglen);

extern size_t fixc_render_msg(char *restrict buf, size_t bsz, fixc_msg_t msg);

#endif	/* INCLUDED_fix_h_ */
