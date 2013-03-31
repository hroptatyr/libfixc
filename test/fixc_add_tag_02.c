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
	static char buf[4096];
	static const char sd[] = "\
8=FIX.5.0.SP2" SOH "9=96" SOH "35=d" SOH
"15=USD" SOH "55=AUD.USD" SOH "48=14433401" SOH "22=M" SOH "167=CASH" SOH
"207=IDEALPRO" SOH "107=Australian dollar" SOH "969=0.000050" SOH "10=125";
	static const char sdx[] = "\
<?xml version=\"1.0\"?>\n\
<FIXML xmlns=\"http://www.fixprotocol.org/FIXML-5-0-SP2\" v=\"5.0 SP2\">\
<SecDef Ccy=\"USD\">\
<Instrmt Sym=\"AUD.USD\" ID=\"14433401\" Src=\"M\" \
SecTyp=\"CASH\" Exch=\"IDEALPRO\" Desc=\"Australian dollar\" \
MinPxIncr=\"0.000050\"/>\
</SecDef>\
</FIXML>\n";
	static const struct fixc_fld_s f_sd = {
		.tag = FIXC_MSG_TYPE,
		.typ = FIXC_TYP_MSGTYP,
		.mtyp = (fixc_msgt_t)FIXML_MSG_SecurityDefinition,
	};
	fixc_msg_t sdm = make_fixc_from_fix(sd, sizeof(sd) - 1);
	fixc_msg_t msg = make_fixc_msg((fixc_msgt_t)FIXC_MSGT_UNK);
	fixc_eng_t eng;
	size_t bsz;
	int res = 0;

	/* add the message type */
	fixc_add_fld(msg, f_sd);

	/* now add all fields of SDM by _tag */
	for (size_t i = 0; i < sdm->nflds; i++) {
		struct fixc_fld_s sdmfld = sdm->flds[i];
		const char *v = fixc_get_tag(sdm, i);
		size_t vz = strlen(v);

		fixc_add_tag(msg, (fixc_attr_t)sdmfld.tag, v, vz);
	}

	/* load him dso */
	eng = fixc_open_eng("fix50sp2");
	/* render it */
	bsz = fixc_render_fixml(buf, sizeof(buf), msg);

	if (memcmp(buf, sdx, bsz)) {
		fputs("rendered buffer differs, should be\n", stderr);
		fwrite(sdx, 1, sizeof(sdx) - 1, stderr);
		fputs("is\n", stderr);
		fwrite(buf, 1, bsz, stderr);
		res = 1;
	}

	if (eng) {
		fixc_close_eng(eng);
	}

	free_fixc(sdm);
	free_fixc(msg);
	return res;
#else  /* !HAVE_ANON_STRUCTS_INIT */
	return 0;
#endif	/* HAVE_ANON_STRUCTS_INIT */
}

/* fixc_add_tag_02.c ends here */
