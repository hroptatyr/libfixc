#if !defined INCLUDED_fixml_comp_sub_h_
#define INCLUDED_fixml_comp_sub_h_

#include <stddef.h>
#include <stdint.h>

#include "fixml-canon-ctxt.h"

typedef const struct fixc_comp_sub_s *fixc_comp_sub_t;

struct fixc_comp_sub_s {
	uint16_t ctx;
	int8_t min;
	int8_t max;
	size_t nsubs;
	uint16_t subs[];
};

/**
 * Given a context CTX return all sub-components */
extern fixc_comp_sub_t fixc_get_comp_sub(fixc_ctxt_t);

#endif	/* INCLUDED_fixml_comp_sub_h_ */
