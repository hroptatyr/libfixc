#if !defined INCLUDED_fixml_comp_rptb_h_
#define INCLUDED_fixml_comp_rptb_h_

#include <stddef.h>
#include <stdint.h>

#include "fixml-canon-attr.h"

/**
 * Return CTX's noX attr if it is a repeating block, FIXC_ATTR_UNK otherwise. */
extern fixc_attr_t fixc_comp_rptb(fixc_ctxt_t ctx);

#endif	/* INCLUDED_fixml_comp_rptb_h_ */
