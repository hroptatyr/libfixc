#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "fix-msg-type.h"
#include "fixml-msg-type.c"

#include "fixml_to_fixc_prep.c"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
	int res = 0;

	for (size_t i = 0; i < countof(msgtyps); i++) {
		const char *msgtyp = msgtyps[i];
		const size_t mtlen = strlen(msgtyp);
		const struct fixml_msg_type_s *p =
			__fixml_mtypify(msgtyp, mtlen);

		if (p->mty != mtys[i]) {
			res = 1;
		}
	}
	return res;
}

/* fixml_to_fixc_hash_10.c ends here */
