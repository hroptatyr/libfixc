#if !defined INCLUDED_fixml_fld_ctx_h_
#define INCLUDED_fixml_fld_ctx_h_

#include <stddef.h>
#include <stdint.h>

#include "fixml-canon-ctxt.h"

typedef const struct fixc_fld_ctx_s *fixc_fld_ctx_t;

struct fixc_fld_ctx_s {
	uint16_t fld;
	int8_t min;
	int8_t max;
	size_t nmsgs;
	size_t ncomps;
	uint16_t ctxs[];
};

/**
 * Given a context CTX return all fields */
extern fixc_fld_ctx_t fixc_get_fld_ctx(fixc_ctxt_t ctx);

#endif	/* INCLUDED_fixml_fld_ctx_h_ */
