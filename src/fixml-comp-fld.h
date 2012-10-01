#if !defined INCLUDED_fixml_comp_fld_h_
#define INCLUDED_fixml_comp_fld_h_

#include <stddef.h>
#include <stdint.h>

typedef const struct fixc_comp_fld_s *fixc_comp_fld_t;

struct fixc_comp_fld_s {
	uint16_t ctx;
	int8_t min;
	int8_t max;
	size_t nflds;
	uint16_t flds[];
};

extern fixc_comp_fld_t fixc_get_comp_fld(uint16_t ctx);

#endif	/* INCLUDED_fixml_comp_fld_h_ */
