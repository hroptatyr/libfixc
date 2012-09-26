#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "fix-msg-type.h"
#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
	if (countof(msgtyps) != countof(mtys)) {
		return 1;
	}
	return 0;
}

/* fixml_to_fixc_hash_09.c ends here */
