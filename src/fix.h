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

#include <stddef.h>
#include <stdint.h>

#include "fix-nsuri.h"
#include "fixml-canon-ctxt.h"
#include "fixml-canon-msgt.h"
#include "fixml-canon-comp.h"
#include "fixml-canon-attr.h"

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
	/** tag type is a unsigned char (printed as number) */
	FIXC_TYP_UCHAR,
	/** tag type is a signed char (printed as char) */
	FIXC_TYP_CHAR,
	/** tag type is a (32b) integer */
	FIXC_TYP_INT,
	/** tag type is a (64b) integer */
	FIXC_TYP_LONG,
	/** for tag #35 (MsgType) */
	FIXC_TYP_MSGTYP,
	/** for field f35 but store a (more) generic context */
	FIXC_TYP_CTXT,
	/** for hierarchical messages */
	FIXC_TYP_PTR,
};

struct fixc_fld_s {
	/** numerical representation of the fix field */
	uint16_t tag;
	/** fix field type, or generally 0 if not computed or known */
	uint16_t typ;
	/** TAG's parent's context */
	uint16_t tpc;
	/** occurrence counter */
	uint16_t cnt;

	/** offset (from the beginning of the message) to the value */
	union {
		ptrdiff_t off;
		int32_t i32;
		int64_t i64;
		uint8_t u8;
		char c;
		fixc_ver_t ver;
		fixc_msgt_t mtyp;
		fixc_ctxt_t ctx;
		void *ptr;
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

/** for passing in and out actual tag data along with its size. */
struct fixc_tag_data_s {
	const char *s;
	size_t z;
};


/**
 * Generate a fixc message from a FIX message string in MSG of length MSGLEN */
extern fixc_msg_t make_fixc_from_fix(const char *msg, size_t msglen);

/**
 * Generate a fixc message from a FIXML document in DOC of length DOCLEN */
extern fixc_msg_t make_fixc_from_fixml(const char *doc, size_t doclen);

/**
 * Generate an empty fixc message. */
extern fixc_msg_t make_fixc_msg(fixc_ctxt_t ctx);

/**
 * Free a fixc message MSG and all its resources. */
extern void free_fixc(fixc_msg_t);

/**
 * Render MSG into a fix string in BUF (of size BSZ). */
extern size_t fixc_render_fix(char *restrict buf, size_t bsz, fixc_msg_t msg);

/**
 * Render MSG into a fixml object in BUF (of size BSZ). */
extern size_t fixc_render_fixml(char *restrict buf, size_t bsz, fixc_msg_t msg);

/**
 * Add FLD to MSG.
 * Upon failure a value <0 is returned. */
extern int fixc_add_fld(fixc_msg_t, struct fixc_fld_s fld);

/**
 * Like `fixc_add_fld()' but insert FLD before field IDX. */
extern int fixc_add_fld_at(fixc_msg_t, struct fixc_fld_s fld, size_t idx);

/**
 * Add TAG to MSG copying VAL (of size VSZ) to representation space.
 * Upon failure a value <0 is returned. */
extern int
fixc_add_tag(fixc_msg_t, fixc_attr_t, const char *val, size_t vsz);

/**
 * Like `fixc_add_tag()' but insert VAL (VSZ) before field I.
 * Upon failure a value <0 is returned. */
extern int
fixc_add_tag_at(fixc_msg_t, fixc_attr_t, const char *val, size_t vsz, size_t i);

/**
 * Delete field N in MSG.*/
extern void fixc_del_fld(fixc_msg_t, size_t n);

/**
 * Return a pointer to the data of a tag, or NULL if the tag is not of type
 * FIXC_TYP_OFF, or NULL if the index IDX is out of bounds..
 * This is somewhat the inverse of fixc_add_tag(). */
extern const char *fixc_get_tag(fixc_msg_t, size_t idx);

/**
 * Like `fixc_get_tag()' but also return the size of the string. */
extern struct fixc_tag_data_s fixc_get_tag_data(fixc_msg_t, size_t idx);

/**
 * Extract the N-th occurrence of context CTX in MSG. */
extern fixc_msg_t fixc_extr_ctxt(fixc_msg_t msg, fixc_ctxt_t ctx, int n);

/**
 * Extract the N-th occurrence of context CTX (and all subcontexts) in MSG. */
extern fixc_msg_t fixc_extr_ctxt_deep(fixc_msg_t msg, fixc_ctxt_t ctx, int n);

/** return the allocated size for MSG. */
extern size_t fixc_msg_z(fixc_msg_t);

/** return the optimal size needed to hold MSG. */
extern size_t fixc_msg_optz(fixc_msg_t);

/** return the minimum size needed to hold MSG. */
extern size_t fixc_msg_minz(fixc_msg_t);

/**
 * Put a (shallow) copy of MSG into TGT of size TSZ and return the number
 * of bytes written. */
extern size_t fixc_msg_cpy(void *restrict tgt, size_t tsz, fixc_msg_t);

#endif	/* INCLUDED_fix_h_ */
