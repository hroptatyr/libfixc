#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "fixml-attr.h"
#include "fixml-attr.c"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

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
		size_t idx = gen() % countof(attrs);
		const char *attr = attrs[idx];
		const size_t alen = strlen(attr);
		const struct fixml_attr_s *p = __fixml_aiddify(attr, alen);

		if (p->aid != tags[idx]) {
			res = 1;
		}
	}
	return res;
}

/* fixml_to_fixc_hash_02.c ends here */
