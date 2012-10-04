#if !defined INCLUDED_fixml_attr_by_ctx_h_
#define INCLUDED_fixml_attr_by_ctx_h_

extern fixc_attr_t fixc_get_aid(
	union __attribute__((transparent_union)) {
		uint16_t ctx;
		fixc_comp_t comp;
		fixc_msg_t msg;
	}, const char *attr, size_t alen);

#endif	/* INCLUDED_fixml_attr_by_ctx_h_ */
