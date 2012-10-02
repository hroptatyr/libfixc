#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
	static char foo[] = "\
<?xml version=\"1.0\"?>\n\
<FIXML xmlns=\"http://www.fixprotocol.org/FIXML-5-0-SP2\">\n\
  <SecDef Ccy=\"USD\">\n\
    <Instrmt Sym=\"GBP.USD\" ID=\"12087797\" Src=\"M\" SecTyp=\"CASH\"\
      Exch=\"IDEALPRO\" Desc=\"British pound\" MinPxIncr=\"0.000050\"/>\n\
  </SecDef>\n\
</FIXML>\n\
";
	fixc_msg_t msg = make_fixc_from_fixml(foo, sizeof(foo) - 1);
	int res = 0;

	/* make sure it's 7 fields we generate */
	if (msg->nflds != 7) {
		fputs("nflds != 7\n", stderr);
		res = 1;
	}

	if (msg->f35.tag != FIXC_MSG_TYPE) {
		fputs("f35 is not MSG_TYPE\n", stderr);
		res = 1;
	}

	if (msg->f35.typ != FIXC_TYP_MSGTYP) {
		fputs("f35 is not of type TYP_MSGTYP\n", stderr);
		res = 1;
	}

	if (msg->f35.mtyp != FIXC_MSGTYP_SECURITY_DEFINITION) {
		fputs("f35 is not SECURITY_DEFINITION\n", stderr);
		res = 1;
	}
	return res;
}

/* fixml_rd_01.c ends here */
