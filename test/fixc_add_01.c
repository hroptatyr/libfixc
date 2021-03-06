#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"
#include "fixml-msg-fix50sp2.h"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
/* starting out with an empty message, add 2 Quot's */
	fixc_msg_t msg = make_fixc_msg((fixc_msgt_t)FIXC_MSGT_BATCH);
	struct fixc_fld_s fld = {
		.tag = 35,
		.typ = FIXC_TYP_MSGTYP,
	};
	int res = 0;

	fld.mtyp = (fixc_msgt_t)FIXML_MSG_Quote;
	fixc_add_fld(msg, fld);

	/* and again */
	fixc_add_fld(msg, fld);

	if (msg->f35.mtyp != FIXC_MSGT_BATCH) {
		fprintf(stderr, "promotion to batch message broken: %u\n",
			msg->f35.mtyp);
		res = 1;
	}
	if (msg->nflds != 2) {
		fprintf(stderr, "expected 2 fields, got %zu\n", msg->nflds);
		res = 1;
	}

	free_fixc(msg);
	return res;
}

/* fixc_add_01.c ends here */
