#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "fixml-attr.h"
#include "fixml-attr.c"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))


int
main(void)
{
	int res = 0;

	for (size_t i = 0; i < countof(attrs); i++) {
		const char *attr = attrs[i];
		const size_t alen = strlen(attr);
		const struct fixml_attr_s *p = __fixml_aiddify(attr, alen);

		if (p->aid != tags[i]) {
			res = 1;
		}
	}
	return res;
}

/* fixml_to_fixc_hash_02.c ends here */
