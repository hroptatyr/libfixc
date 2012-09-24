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

typedef uint16_t fixc_fld_t;
typedef struct fixc_msg_s *fixc_msg_t;

enum {
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

struct fixc_fld_s {
	/** numerical representation of the fix field */
	uint16_t tag;
	/** fix field type, or generally 0 if not computed or known */
	uint16_t typ;
	/** offset (from the beginning of the message) to the value */
	uint32_t off;
};

struct fixc_msg_s {
	/** printed representation */
	char *pr;
	/** length of the printed representation */
	size_t pz;

	/** number of fields */
	size_t nflds;
	struct fixc_fld_s flds[];
};

extern fixc_msg_t make_fixc_msg(const char *msg, size_t msglen);

#endif	/* INCLUDED_fix_h_ */
