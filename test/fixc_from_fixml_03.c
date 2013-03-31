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
	fixc_eng_t eng = fixc_open_eng("fix50sp2");
	fixc_msg_t msg = make_fixc_from_fixml(sdx, sizeof(sdx) - 1);
	const char *v;
	int res = eng == NULL;

	/* check fields for tpc and cnt slots */
	for (size_t i = 0; i < msg->nflds; i++) {
		struct fixc_fld_s msgf = msg->flds[i];

		if (i == 8) {
			/* rptb field */
			;
		} else if (msgf.typ != FIXC_TYP_OFF) {
			fprintf(stderr, "expected typ _OFF, got %hu\n",
				msgf.typ);
			res = 1;
		}
	}

	v = msg->pr + msg->flds[0].off;
	if (strcmp(v, "USD")) {
		fprintf(stderr, "expected `USD', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[1].off;
	if (strcmp(v, "AUD.USD")) {
		fprintf(stderr, "expected `AUD.USD', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[2].off;
	if (strcmp(v, "14433401")) {
		fprintf(stderr, "expected `14433401', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[3].off;
	if (strcmp(v, "M")) {
		fprintf(stderr, "expected `M', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[4].off;
	if (strcmp(v, "CASH")) {
		fprintf(stderr, "expected `CASH', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[5].off;
	if (strcmp(v, "IDEALPRO")) {
		fprintf(stderr, "expected `IDEALPRO', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[6].off;
	if (strcmp(v, "Australian dollar")) {
		fprintf(stderr, "expected `Australian dollar', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[7].off;
	if (strcmp(v, "0.000050")) {
		fprintf(stderr, "expected `0.000050', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[9].off;
	if (strcmp(v, "foobar")) {
		fprintf(stderr, "expected `foobar', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[10].off;
	if (strcmp(v, "1001")) {
		fprintf(stderr, "expected `1001', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[11].off;
	if (strcmp(v, "barbar")) {
		fprintf(stderr, "expected `barbar', got `%s'\n", v);
		res = 1;
	}

	v = msg->pr + msg->flds[12].off;
	if (strcmp(v, "1002")) {
		fprintf(stderr, "expected `1002', got `%s'\n", v);
		res = 1;
	}

	free_fixc(msg);
	fixc_close_eng(eng);
	return res;
}

/* fixc_from_fixml_03.c ends here */
