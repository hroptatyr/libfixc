#if !defined INCLUDED_fixml_comp_sub_h_
#define INCLUDED_fixml_comp_sub_h_

#include <stddef.h>
#include <stdint.h>

struct fixc_comp_sub_s {
	uint16_t ctx;
	int8_t min;
	int8_t max;
	size_t nsubs;
	uint16_t subs[];
};

#endif	/* INCLUDED_fixml_comp_sub_h_ */
