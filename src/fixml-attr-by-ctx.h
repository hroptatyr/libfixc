#if !defined INCLUDED_fixml_attr_by_ctx_h_
#define INCLUDED_fixml_attr_by_ctx_h_

#include <stdint.h>
#include <stddef.h>
#include "fixml-attr.h"
#include "fixml-canon-comp.h"
#include "fixml-canon-msg.h"

extern fixc_attr_t fixc_get_aid(
	union __attribute__((transparent_union)) {
		unsigned int i;
		uint16_t ui16;
		fixc_comp_t comp;
		fixc_msgt_t msg;
	}, const char *attr, size_t alen);

#endif	/* INCLUDED_fixml_attr_by_ctx_h_ */
