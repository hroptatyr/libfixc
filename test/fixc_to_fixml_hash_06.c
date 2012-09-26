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

static unsigned int
gen(void)
{
/* simple congruential generator */
	static unsigned int seed = 0x31415926U;
	static const unsigned int mx = 3141601;
	static const unsigned int nn = 314159311;

	seed = mx * seed + nn;
	return seed;
}

int
main(void)
{
	int res = 0;

	/* 100000 lookups */
	for (size_t i = 0; i < 100000; i++) {
		size_t idx = gen() % countof(msgtyps);
		const fixc_msg_type_t mty = mtys[idx];
		const char *msgtyp;

		if (!fixml50sp2_p(mty)) {
			continue;
		}

		msgtyp = __mty_fixmlify(mty);
		if ((msgtyps[idx][0] ^ msgtyp[0]) & 1) {
			switch (mty) {
			case FIXC_MSGTYP_CONTRARY_INTENTION_REPORT:
				break;
			default:
				res = 1;
			}
		}
	}
	return res;
}

/* fixc_to_fixml_hash_06.c ends here */
