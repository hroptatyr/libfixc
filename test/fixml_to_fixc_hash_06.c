#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "fixml-comp.h"
#include "fixml-comp.c"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
	int res = 0;

	for (size_t i = 0; i < countof(comps); i++) {
		const char *comp = comps[i];
		const size_t alen = strlen(comp);
		const struct fixml_comp_s *p = __fixml_ciddify(comp, alen);

		if (p->cid != cids[i]) {
			res = 1;
		}
	}
	return res;
}

/* fixml_to_fixc_hash_06.c ends here */
