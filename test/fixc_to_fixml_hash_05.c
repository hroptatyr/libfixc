#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix-msg-type.h"
#include "fixml-msg-type-rev.c"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

static int
fixml50sp2_p(fixc_msg_type_t mty)
{
	switch (mty) {
	default:
		return 1;
	}
}

int
main(void)
{
	int res = 0;

	for (size_t i = 0; i < countof(mtys); i++) {
		const fixc_msg_type_t mty = mtys[i];
		const char *msgtyp;

		if (!fixml50sp2_p(mty)) {
			continue;
		}

		msgtyp = __mty_fixmlify(mty);
		if (strcmp(msgtyps[i], msgtyp)) {
			switch (mty) {
			case FIXC_MSGTYP_CONTRARY_INTENTION_REPORT:
				break;
			default:
				fprintf(stderr, "\
is \"%s\"\t\
should_be \"%s\"\n",
					msgtyp, msgtyps[i]);
				res = 1;
			}
		}
	}
	return res;
}

/* fixc_to_fixml_hash_05.c ends here */
