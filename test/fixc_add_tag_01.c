#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"
#include "fixml-msg-fix50sp2.h"

#define SOH	"\001"

int
main(void)
{
/* create a secdef message, then create an empty message and add all fields
 * of the secdef message */
#if defined HAVE_ANON_STRUCTS_INIT
	static const char sd[] = "\
8=FIX.5.0.SP2" SOH "9=96" SOH "35=d" SOH
"15=USD" SOH "55=AUD.USD" SOH "48=14433401" SOH "22=M" SOH "167=CASH" SOH
"207=IDEALPRO" SOH "107=Australian dollar" SOH "969=0.000050" SOH "10=125";
	static const struct fixc_fld_s f_sd = {
		.tag = FIXC_MSG_TYPE,
		.typ = FIXC_TYP_MSGTYP,
		.mtyp = (fixc_msgt_t)FIXML_MSG_SecurityDefinition,
	};
	fixc_msg_t sdm = make_fixc_from_fix(sd, sizeof(sd) - 1);
	fixc_msg_t msg = make_fixc_msg((fixc_msgt_t)FIXC_MSGT_UNK);
	int res = 0;

	/* add the message type */
	fixc_add_fld(msg, f_sd);

	/* now add all fields of SDM by _tag */
	for (size_t i = 0; i < sdm->nflds; i++) {
		struct fixc_fld_s sdmfld = sdm->flds[i];
		const char *v = sdm->pr + sdmfld.off;
		size_t vz = strlen(v);

		fixc_add_tag(msg, (fixc_attr_t)sdmfld.tag, v, vz);
	}

	if (msg->nflds != 8) {
		fprintf(stderr, "expected 8 fields, got %zu\n", msg->nflds);
		res = 1;
	}

	if (msg->f35.mtyp != FIXML_MSG_SecurityDefinition) {
		fprintf(stderr, "expected message to be SecDef, got %u\n",
			(unsigned int)msg->f35.mtyp);
		res = 1;
	}

	free_fixc(sdm);
	free_fixc(msg);
	return res;
#else  /* !HAVE_ANON_STRUCTS_INIT */
	return 0;
#endif	/* HAVE_ANON_STRUCTS_INIT */
}

/* fixc_add_tag_01.c ends here */
