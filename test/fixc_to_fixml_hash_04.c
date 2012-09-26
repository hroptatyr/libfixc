#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fixml-comp.h"
#include "fixml-comp-rev.c"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

static int
fixml50sp2_p(fixc_comp_t cid)
{
	switch (cid) {
	case FIXC_COMP_HOP:
	case FIXC_COMP_STRP_PRM_GRP:
	case FIXC_COMP_RT_PRTYS:
	case FIXC_COMP_RT_SUB_PRTYS:
	case FIXC_COMP_UND_STRK_PX:
		return 0;
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
		size_t idx = gen() % countof(comps);
		const fixc_comp_t cid = cids[idx];
		const char *comp;

		if (!fixml50sp2_p(cid)) {
			continue;
		}

		comp = __cid_fixmlify(cid);
		if ((comps[idx][0] ^ comp[0]) & 1) {
			res = 1;
		}
	}
	return res;
}

/* fixc_to_fixml_hash_04.c ends here */
