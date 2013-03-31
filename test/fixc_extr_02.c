#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
static char test[] = "\
8=FIX.5.0.SP2\0019=632\00135=d\001964=7\001715=2012-10-09\00155=QG:QG\00148=QG\001107=EMINI NATURAL GAS COMBO\001167=MLEG\00122=H\001762=SP\001200=201211\001541=2012-10-26\001207=NYMEX\0011198=0\0011197=FUT\0011227=ENRGY\001965=1\001969=5\001454=3\001455=QGX2-QGN4\001456=101\001455=QGX2-QGN4\001456=H\001455=828957\001456=103\001864=2\001865=5\001866=2008-11-24\001865=7\001866=2012-10-26\001555=2\001602=QG\001603=H\001610=201211\001616=NYMEX\001624=1\001623=1\001609=FUT\001604=4\001605=QG 201211\001606=100\001605=QGX2\001606=101\001605=QG 201211\001606=H\001605=852093\001606=103\001602=QG\001603=H\001610=201407\001616=NYMEX\001624=2\001623=1\001609=FUT\001604=4\001605=819909\001606=103\001605=QGN4\001606=101\001605=QG 201407\001606=100\001605=QG 201407\001606=H\0011310=1\0011301=CME\0011300=ALL\0011205=1\0011206=-9999\0011207=9999\0011208=0.005\0011209=0\00110=164";
	int res = 0;
	fixc_eng_t e = fixc_open_eng("fix50sp2");
	fixc_msg_t msg = make_fixc_from_fix(test, sizeof(test));
	fixc_msg_t sub = fixc_extr_ctxt(msg, 96U, 2);

	if (sub->nflds != 2) {
		fprintf(stderr, "sub nflds: %zu  should be: 2\n", sub->nflds);
		res = 1;
	}

	free_fixc(msg);
	free_fixc(sub);
	if (e) {
		fixc_close_eng(e);
	} else {
		res = 1;
	}
	return res;
}

/* fixc_extr_01.c ends here */
