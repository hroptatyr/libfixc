#if !defined INCLUDED_fixml_comp_by_ctx_h_
#define INCLUDED_fixml_comp_by_ctx_h_

#include <stdint.h>
#include <stddef.h>
#include "fixml-attr.h"
#include "fixml-canon-comp.h"
#include "fixml-canon-msgt.h"

typedef union __attribute__((transparent_union)) {
	unsigned int i;
	uint16_t ui16;
	fixc_comp_t comp;
	fixc_msgt_t msgt;
} fixc_ctxt_t;

extern fixc_comp_t fixc_get_cid(fixc_ctxt_t, const char *elem, size_t elen);

#endif	/* INCLUDED_fixml_comp_by_ctx_h_ */
