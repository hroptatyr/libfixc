#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "fixml-attr.h"
#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
	if (countof(attrs) != countof(tags)) {
		return 1;
	}
	return 0;
}

/* fixml_to_fixc_hash_01.c ends here */
