#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"
#include "fixml-msg.h"

#define SOH	"\001"

int
main(void)
{
/* create a secdef message from FIXML (including .cnt and .tpc info), then
 * copy the whole shebang and see if the .tpc and .cnt info is still there */
	static const char sdx[] = "\
<?xml version=\"1.0\"?>\n\
<FIXML xmlns=\"http://www.fixprotocol.org/FIXML-5-0-SP2\" v=\"5.0 SP2\">\
<SecDef Ccy=\"USD\">\
<Instrmt Sym=\"AUD.USD\" ID=\"14433401\" Src=\"M\" \
SecTyp=\"CASH\" Exch=\"IDEALPRO\" Desc=\"Australian dollar\" \
MinPxIncr=\"0.000050\">\
<AID AltID=\"foobar\" AltIDSrc=\"1001\"/>\
<AID AltID=\"barbar\" AltIDSrc=\"1002\"/>\
</Instrmt>\
</SecDef>\
</FIXML>\n";
	static const struct fixc_fld_s msgtyp = {
		.tag = FIXC_MSG_TYPE,
		.typ = FIXC_TYP_MSGTYP,
		.mtyp = (fixc_msgt_t)FIXML_MSG_SecurityDefinition,
	};
	fixc_msg_t msg = make_fixc_from_fixml(sdx, sizeof(sdx) - 1);
	fixc_msg_t cpy = make_fixc_msg(FIXC_MSGT_BATCH);
	size_t nflds;
	int res = 0;

	/* the message type */
	fixc_add_fld(cpy, msgtyp);

	/* check fields for tpc and cnt slots */
	for (size_t i = 0; i < msg->nflds; i++) {
		struct fixc_fld_s f = msg->flds[i];
		const char *v;

		if ((v = fixc_get_tag(msg, i)) != NULL) {
			size_t vz = strlen(v);
			fixc_add_tag(cpy, (fixc_attr_t)f.tag, v, vz);
		} else {
			/* it's no danger adding the whole field */
			fixc_add_fld(cpy, f);
		}
	}

	/* and again */
	fixc_add_fld(cpy, msgtyp);

	/* check fields for tpc and cnt slots */
	for (size_t i = 0; i < msg->nflds; i++) {
		struct fixc_fld_s f = msg->flds[i];
		const char *v;

		if ((v = fixc_get_tag(msg, i)) != NULL) {
			size_t vz = strlen(v);
			fixc_add_tag(cpy, (fixc_attr_t)f.tag, v, vz);
		} else {
			/* it's no danger adding the whole field */
			fixc_add_fld(cpy, f);
		}
	}

	/* just to have a reference */
	nflds = msg->nflds;

	/* cpy is filled, we don't need msg anymore */
	free_fixc(msg);

	if (cpy->nflds != nflds * 2 + 2) {
		fprintf(stderr, "nfld mismatch, %zu v %zu\n",
			cpy->nflds, nflds * 2 + 2);
		res = 1;
	}

	for (size_t i = 0; i < nflds; i++) {
		if (i == 0 || i == nflds + 1) {
			/* them 35-tags */
			;
		} else if (i == 9 || i == nflds + 1 + 9) {
			/* them AID rptb tags */
			;
		} else if (cpy->flds[i].typ != FIXC_TYP_OFF) {
			fprintf(stderr, "typ @%zu buggered, "
				"expected _OFF, got %hu\n",
				i, cpy->flds[i].typ);
			res = 1;
		}
	}

	for (size_t i = 0; i < cpy->nflds; i++) {
		const char *v;

		if (i == 0 || i == nflds + 1) {
			/* them 35-tags */
			;
		} else if (i == 9 || i == nflds + 1 + 9) {
			/* them AID rptb tags */
			;
		} else if ((v = fixc_get_tag(cpy, i)) == NULL) {
			fprintf(stderr, "uh oh, "
				"didn't expect no string data for fld %zu\n",
				i);
			res = 1;
		} else if (strlen(v) > 17) {
			fprintf(stderr, "string too long: %s\n", v);
			res = 1;
		}
	}

	free_fixc(cpy);
	return res;
}

/* fixc_from_fixml_05.c ends here */
