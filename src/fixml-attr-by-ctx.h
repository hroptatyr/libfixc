#if !defined INCLUDED_fixml_attr_by_ctx_h_
#define INCLUDED_fixml_attr_by_ctx_h_

#include <stdint.h>
#include <stddef.h>
#include "fixml-attr.h"
#include "fixml-comp.h"
#include "fix-msg-type.h"

extern fixc_attr_t fixc_get_aid(
	union __attribute__((transparent_union)) {
		unsigned int i;
		uint16_t ui16;
		fixc_comp_t comp;
		fixc_msg_type_t msg;
	}, const char *attr, size_t alen);

#endif	/* INCLUDED_fixml_attr_by_ctx_h_ */
