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

int
main(void)
{
	int res = 0;

	for (size_t i = 0; i < countof(cids); i++) {
		const fixc_comp_t cid = cids[i];
		const char *comp;

		if (!fixml50sp2_p(cid)) {
			continue;
		}

		comp = __cid_fixmlify(cid);
		if (strcmp(comps[i], comp)) {
			fprintf(stderr, "\
is \"%s\"\t\
should_be \"%s\"\n",
				comp, comps[i]);
			res = 1;
		}
	}
	return res;
}

/* fixc_to_fixml_hash_03.c ends here */
