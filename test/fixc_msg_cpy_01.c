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
	static char buf[512];
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
	fixc_msg_t cpy = (void*)buf;
	size_t bsz;
	int res = 0;

	/* add the message type */
	if ((bsz = fixc_msg_cpy(buf, sizeof(buf), msg)) < sizeof(*msg)) {
		fprintf(stderr, "copy of msg won't fit, got %zu, need %zu\n",
			bsz, fixc_msg_minz(msg));
		res = 1;
		goto out;
	}

	if (msg->nflds != cpy->nflds) {
		fprintf(stderr, "number of fields differ %zu v %zu\n",
			msg->nflds, cpy->nflds);
		res = 1;
	}

	/* compare each field now */
	for (size_t i = 0; i < msg->nflds; i++) {
		struct fixc_fld_s msgf = msg->flds[i];
		struct fixc_fld_s cpyf = cpy->flds[i];

		if (msgf.tag != cpyf.tag) {
			fprintf(stderr, "fld[%zu].tag differ: %hu v %hu\n",
				i, msgf.tag, cpyf.tag);
		}
		if (msgf.tpc != cpyf.tpc) {
			fprintf(stderr, "fld[%zu].tpc differ: %hu v %hu\n",
				i, msgf.tpc, cpyf.tpc);
		}			
		if (msgf.cnt != cpyf.cnt) {
			fprintf(stderr, "fld[%zu].cnt differ: %hu v %hu\n",
				i, msgf.cnt, cpyf.cnt);
		}			
	}

out:
	free_fixc(msg);
	return res;
}

/* fixc_msg_cpy_01.c ends here */
