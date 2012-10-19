#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"
#include "fixml-msg.h"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
/* starting out with an empty message, add 2 Quot's */
	fixc_msg_t msg = make_fixc_msg((fixc_msgt_t)FIXC_MSGT_UNK);
	struct fixc_fld_s fld = {
		.tag = 35,
		.typ = FIXC_TYP_MSGTYP,
	};
	int res = 0;

	fld.mtyp = (fixc_msgt_t)FIXML_MSG_Quote;
	fixc_add_fld(msg, fld);

	/* and again */
	fixc_add_fld(msg, fld);
	fixc_fixup(msg);

	for (size_t i = 0; i < msg->nflds; i++) {
		if (msg->flds[i].tpc != FIXC_MSGT_BATCH) {
			fprintf(stderr, "oooh, parent ctx not Batch but %hu\n",
				msg->flds[i].tpc);
			res = 1;
		}
		if (msg->flds[i].cnt) {
			fprintf(stderr, "counter should be 0, is %hu\n",
				msg->flds[i].cnt);
			res = 1;
		}
	}

	free_fixc(msg);
	return res;
}

/* fixc_add_01.c ends here */
