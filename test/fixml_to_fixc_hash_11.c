#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "fix-msg-type.h"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

static fixc_msg_type_t
lookup(const char *msgtyp)
{
	for (size_t i = 0; i < countof(msgtyps); i++) {
		if (strcmp(msgtyps[i], msgtyp) == 0) {
			return mtys[i];
		}
	}
	return FIXC_MSGTYP_UNK;
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
		const char *msgtyp = msgtyps[idx];
		fixc_msg_type_t mty = lookup(msgtyp);

		if (mty == FIXC_MSGTYP_UNK) {
			res = 1;
		}
	}
	return res;
}

/* fixml_to_fixc_hash_11.c ends here */
