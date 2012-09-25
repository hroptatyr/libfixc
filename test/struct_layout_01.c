#include <stdint.h>
#include "fix.h"

int
main(void)
{
	const size_t fldz = sizeof(struct fixc_fld_s);
	size_t should_be_z = 0;

	/* a fixc_fld_s should be 2 ui16s and 1 union of max size ui32 */
	should_be_z += sizeof(uint16_t);
	should_be_z += sizeof(uint16_t);
	should_be_z += sizeof(uint32_t);

	if (fldz != should_be_z) {
		return 1;
	}
	return 0;
}

/* struct_layout.01.c ends here */
