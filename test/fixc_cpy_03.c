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
	fixc_msg_t msg = make_fixc_from_fix(test, sizeof(test));
	static char buf[4096];
	size_t nwr;

	if ((nwr = fixc_msg_cpy(buf, sizeof(buf), msg)) == 0) {
		fprintf(stderr, "no bytes written, should have been %zu\n",
			fixc_msg_z(msg));
		res = 1;
	}

	/* compare the two */
	{
		fixc_msg_t tmsg = (void*)buf;
		if (tmsg->nflds != msg->nflds) {
			fprintf(stderr, "number of fields differ %zu v %zu\n",
				tmsg->nflds, msg->nflds);
			res = 1;
		}
		if (tmsg->f35.tag != msg->f35.tag) {
			fprintf(stderr, "message types differ %hu v %hu\n",
				tmsg->f35.tag, msg->f35.tag);
			res = 1;
		}
		for (size_t i = 0; i < tmsg->nflds; i++) {
			if (tmsg->flds[i].tag != msg->flds[i].tag) {
				fprintf(stderr, "fld %zu differs: %hu v %hu\n",
					i, tmsg->flds[i].tag, msg->flds[i].tag);
			}
		}
	}

	free_fixc(msg);
	return res;
}

/* fixc_cpy_03.c ends here */
