#if !defined INCLUDED_fixml_fld_ctx_h_
#define INCLUDED_fixml_fld_ctx_h_

#include <stddef.h>
#include <stdint.h>

typedef const struct fixc_fld_ctx_s *fixc_fld_ctx_t;

struct fixc_fld_ctx_s {
	uint16_t fld;
	int8_t min;
	int8_t max;
	size_t nctxs;
	uint16_t ctxs[];
};

extern fixc_fld_ctx_t fixc_get_fld_ctx(uint16_t fld);

#endif	/* INCLUDED_fixml_fld_ctx_h_ */
