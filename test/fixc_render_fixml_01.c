#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"
#include "fixml-msg-fix50sp2.h"
#include "fixml-attr-fix50sp2.h"

#define countof(x)	(sizeof(x) / sizeof(*x))

int
main(void)
{
/* starting out with an empty message, add 2 Quot's */
#if defined HAVE_ANON_STRUCTS_INIT
	static char proto[] = "\
<?xml version=\"1.0\"?>\n\
<FIXML xmlns=\"http://www.fixprotocol.org/FIXML-5-0-SP2\" v=\"5.0 SP2\">\
<Batch><Quot QID=\"1117\"/><Quot QID=\"1117\"/></Batch></FIXML>\n";
	static const struct fixc_fld_s f_quot = {
		.tag = FIXC_MSG_TYPE,
		.typ = FIXC_TYP_MSGTYP,
		.mtyp = (fixc_msgt_t)FIXML_MSG_Quote,
	};
	static const struct fixc_fld_s f_qid = {
		.tag = FIXML_ATTR_QuoteID,
		.typ = FIXC_TYP_INT,
		.i32 = 1117,
	};
	static char buf[4096];
	fixc_eng_t eng;
	fixc_msg_t msg;
	size_t bsz;
	int res = 0;

	/* load the 50sp2 engine */
	if ((eng = fixc_open_eng("fix50sp2")) == NULL) {
		res = 1;
		/* don't even try */
		goto out;
	}

	msg = make_fixc_msg((fixc_msgt_t)FIXC_MSGT_BATCH);

	fixc_add_fld(msg, f_quot);
	fixc_add_fld(msg, f_qid);

	/* and again */
	fixc_add_fld(msg, f_quot);
	fixc_add_fld(msg, f_qid);

	/* render him */
	bsz = fixc_render_fixml(buf, sizeof(buf), msg);

	if (memcmp(buf, proto, bsz)) {
		fputs("rendered buffer differs, should be\n", stderr);
		fwrite(proto, 1, sizeof(proto) - 1, stderr);
		fputs("is\n", stderr);
		fwrite(buf, 1, bsz, stderr);
		res = 1;
	}

	free_fixc(msg);
	fixc_close_eng(eng);
out:
	return res;
#else  /* !HAVE_ANON_STRUCTS_INIT */
	return 0;
#endif	/* HAVE_ANON_STRUCTS_INIT */
}

/* fixc_render_fixml_01.c ends here */
