#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"
#include "fixml-msg.h"
#include "fixml-attr.h"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
/* starting out with an empty message, add 2 Quot's */
	static const struct fixc_fld_s f_qid = {
		.tag = FIXML_ATTR_QuoteID,
		.typ = FIXC_TYP_INT,
		.i32 = 1717,
	};
	fixc_msg_t msg = make_fixc_msg((fixc_msgt_t)FIXML_MSG_Quote);
	int res = 0;

	/* just insert the currency tag */
	fixc_add_tag(msg, (fixc_attr_t)FIXML_ATTR_Currency, "EUR", 3);

	/* insert the QID field */
	fixc_add_fld_at(msg, f_qid, 0);

	if (msg->nflds != 2) {
		fprintf(stderr, "expected 2 fields, got %zu\n", msg->nflds);
		res = 1;
	}

	if (msg->f35.mtyp != FIXML_MSG_Quote) {
		fprintf(stderr, "expected message to be Quot, got %u\n",
			(unsigned int)msg->f35.mtyp);
		res = 1;
	}

	if (msg->flds[0].tag != FIXML_ATTR_QuoteID) {
		fprintf(stderr, "expected fld[0] to be QID, got %hu\n",
			msg->flds[0].tag);
		res = 1;
	}

	if (msg->flds[1].tag != FIXML_ATTR_Currency) {
		fprintf(stderr, "expected fld[1] to be Ccy, got %hu\n",
			msg->flds[1].tag);
		res = 1;
	}

	free_fixc(msg);
	return res;
}

/* fixc_add_fld_04.c ends here */
