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
MinPxIncr=\"0.000050\"/>\
</SecDef>\
</FIXML>\n";
	fixc_msg_t msg = make_fixc_from_fixml(sdx, sizeof(sdx) - 1);
	int res = 0;

	/* check fields for tpc and cnt slots */
	for (size_t i = 0; i < msg->nflds; i++) {
		struct fixc_fld_s msgf = msg->flds[i];

		if (msgf.tag == 0) {
			fprintf(stderr, "fld[%zu].tag is naught\n", i);
			res = 1;
		}
		if (msgf.tpc == 0) {
			fprintf(stderr, "fld[%zu].tpc is naught\n", i);
			res = 1;
		}			
		if (msgf.cnt == 0 && i && msg->flds[i - 1].tpc == msgf.tpc) {
			fprintf(stderr, "fld[%zu].cnt is naught\n", i);
			res = 1;
		}			
	}

	free_fixc(msg);
	return res;
}

/* fixc_from_fixml_01.c ends here */
