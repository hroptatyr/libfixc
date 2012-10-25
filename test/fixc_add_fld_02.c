#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
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
#if defined HAVE_ANON_STRUCTS_INIT
	static const struct fixc_fld_s f_qid = {
		.tag = FIXML_ATTR_QuoteID,
		.typ = FIXC_TYP_INT,
		.i32 = 1717,
	};
	fixc_msg_t msg = make_fixc_msg((fixc_msgt_t)FIXML_MSG_Quote);
	int res = 0;

	/* now adding a tag 35 should result in occupying slot f35 */
	fixc_add_fld(msg, f_qid);

	if (msg->nflds != 1) {
		fprintf(stderr, "expected 1 field, got %zu\n", msg->nflds);
		res = 1;
	}

	if (msg->flds[0].tag != FIXML_ATTR_QuoteID) {
		fprintf(stderr, "expected fld[0] to be QID, got %hu\n",
			msg->flds[0].tag);
		res = 1;
	}

	free_fixc(msg);
	return res;
#else  /* !HAVE_ANON_STRUCTS_INIT */
	return 0;
#endif	/* HAVE_ANON_STRUCTS_INIT */
}

/* fixc_add_fld_02.c ends here */
