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

#include "fix.h"
#include "nifty.h"


/* fix guts */
#define SOH	"\001"
#define SOHC	(*SOH)

static uint8_t
fix_chksum(const char *str, size_t len)
{
        unsigned int res = 0;
        for (const char *p = str, *ep = str + len; p < ep; res += *p++);
        return (uint8_t)(res & 0xff);
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
				long unsigned int tmp;
				size_t i = res->nflds;

				/* finish string q */
				*p = '\0';
				if ((tmp = strtoul(q, NULL, 10)) < 65536) {
					res->flds[i].tag = tmp;
					res->flds[i].off = p + 1 - res->pr;
					res->flds[i].beg = q - (p + 1);
				} else {
					res->flds[i].tag = 0;
				}

				/* switch to value state */
				kv_state = VALUE;
				q = p + 1;
			}
			break;
		case VALUE:
			if (*p == SOHC || *p == '\0') {
				size_t i = res->nflds;

				if (res->flds[i].tag) {
					/* finish string q */
					*p = '\0';

					/* add this field */
					res->nflds++;
				}

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


#if defined STANDALONE
int
main(void)
{
	static char foo[] = "8=FIXT.1.1" SOH "9=0004" SOH
		"35=S" SOH "117=112" SOH
		"132=1.03" SOH "133=1.04" SOH "10=0";
	fixc_msg_t msg = make_fixc_msg(foo, sizeof(foo) - 1);

	fprintf(stdout, "%zu fields\n", msg->nflds);
	for (size_t i = 0; i < msg->nflds; i++) {
		fprintf(stdout, "+ field %zu: %hu=%s\n",
			i, msg->flds[i].tag, msg->pr + msg->flds[i].off);
	}

	free(msg);
	return 0;
}
#endif	/* STANDALONE */

/* fix.c ends here */
