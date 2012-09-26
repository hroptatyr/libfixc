#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "fixml-comp.h"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

static fixc_comp_t
lookup(const char *comp)
{
	for (size_t i = 0; i < countof(comps); i++) {
		if (strcmp(comps[i], comp) == 0) {
			return cids[i];
		}
	}
	return FIXC_COMP_UNK;
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
		size_t idx = gen() % countof(comps);
		const char *comp = comps[idx];
		fixc_comp_t cid = lookup(comp);

		if (cid == FIXC_COMP_UNK) {
			res = 1;
		}
	}
	return res;
}

/* fixml_to_fixc_hash_07.c ends here */
