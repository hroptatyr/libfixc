#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"
#include "fix-private.h"
#include "fixml-msg-fix50sp2.h"

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
	fixc_eng_t eng;
	int res = 0;

	fld.mtyp = (fixc_msgt_t)FIXML_MSG_Quote;
	fixc_add_fld(msg, fld);

	/* and again */
	fixc_add_fld(msg, fld);

	/* load the dso */
	eng = fixc_open_eng("fix50sp2");

	/* fix'em up then */
	fixc_fixup(msg);

	for (size_t i = 0; i < msg->nflds; i++) {
		if (msg->flds[i].tpc != FIXML_MSG_Quote) {
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

	if (eng) {
		fixc_close_eng(eng);
	}
	free_fixc(msg);
	return res;
}

/* fixc_add_01.c ends here */
