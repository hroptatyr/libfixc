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
	static const struct fixc_fld_s f_quot = {
		.tag = FIXC_MSG_TYPE,
		.typ = FIXC_TYP_MSGTYP,
		.mtyp = (fixc_msgt_t)FIXML_MSG_Quote,
	};
	fixc_msg_t msg = make_fixc_msg((fixc_msgt_t)FIXC_MSGT_UNK);
	int res = 0;

	/* now adding a tag 35 should result in occupying slot f35 */
	fixc_add_fld_at(msg, f_quot, 0);

	if (msg->nflds != 0) {
		fprintf(stderr, "expected 0 fields, got %zu\n", msg->nflds);
		res = 1;
	}

	if (msg->f35.mtyp != FIXML_MSG_Quote) {
		fprintf(stderr, "expected static f35 to be Quote, got %u\n",
			(unsigned int)msg->f35.mtyp);
		res = 1;
	}

	free_fixc(msg);
	return res;
#else  /* !HAVE_ANON_STRUCTS_INIT */
	return 0;
#endif	/* HAVE_ANON_STRUCTS_INIT */
}

/* fixc_add_fld_03.c ends here */
