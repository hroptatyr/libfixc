#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
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
/* create a secdef message from an empty FIXML document */
#if defined HAVE_ANON_STRUCTS_INIT
	static const char sdx[] = "<?xml version=\"1.0\"?>\n";
	fixc_msg_t msg = make_fixc_from_fixml(sdx, sizeof(sdx) - 1);
	int res = 0;

	if (msg != NULL) {
		if (msg->nflds > 0) {
			res = 1;
		}
		free_fixc(msg);
	}
	return res;
#else	/* !HAVE_ANON_STRUCTS_INIT */
	return 0;
#endif	/* HAVE_ANON_STRUCTS_INIT */
}

/* fixc_from_fixml_09.c ends here */
