#include "fix.h"

int
main(void)
{
	static char tst[] = "\
8=FIXT.1.1\0019=155\00135=W\00134=2\00149=ABFX\00152=20080722-16:37:11.234\00156=X2RV1\00155=EUR/USD\001262=CAP00000112\001268=2\001269=1\001270=1.5786\001271=500000\001272=20080724\001269=1\001271=500000\001272=20080724\00110=218\001";

	for (size_t i = 0; i < 1000000; i++) {
		fixc_msg_t msg = make_fixc_from_fix(tst, sizeof(tst) - 1);

		free_fixc(msg);
	}
	return 0;
}

