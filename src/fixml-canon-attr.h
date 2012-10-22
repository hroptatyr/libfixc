/*** fixml-canon-attr.h -- fixml attributes
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
#if !defined INCLUDED_fixml_canon_attr_h_
#define INCLUDED_fixml_canon_attr_h_

#include "fixml-canon-ctxt.h"

typedef enum {
	/* these ones are our own */
	FIXC_ATTR_UNK,
	/* canonical xml name space declarator */
	FIXC_ATTR_XMLNS = 65535U,
	/* fixml's version declarator */
	FIXC_ATTR_V = 65534U,
} fixc_attr_t;

#define FIXC_ATTR_XMLNS		(fixc_attr_t)FIXC_ATTR_XMLNS
#define FIXC_ATTR_V		(fixc_attr_t)FIXC_ATTR_V

/**
 * Return fixml string for attribute AID. */
extern const char *fixc_attr_fixmlify(fixc_ctxt_t, fixc_attr_t);

#endif	/* INCLUDED_fixml_canon_attr_h_ */
