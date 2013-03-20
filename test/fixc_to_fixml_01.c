#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "fix.h"

int
main(void)
{
	static char res[] = "\
<?xml version=\"1.0\"?>\n\
<FIXML xmlns=\"http://www.fixprotocol.org/FIXML-5-0-SP2\" v=\"5.0 SP2\">\
<Batch></Batch>\
</FIXML>\n\
";
	fixc_msg_t msg;
	struct fixc_rndr_s r;

	/* start a fix msg for that */
	msg = make_fixc_msg((fixc_msgt_t)FIXC_MSGT_BATCH);

	/* render the whole shebang */
	r = fixc_render_fixml_rndr(msg);
	free_fixc(msg);

	if (r.len != sizeof(res) - 1) {
		fprintf(stderr, "sizes differ %zu v %zu\n%s\n  v\n%s\n",
			r.len, sizeof(res) - 1,
			r.str, res);
		return 1;
	} else if (memcmp(r.str, res, r.len)) {
		fprintf(stderr, "buffers differ:\n%s\n  v\n%s\n", r.str, res);
		return 1;
	}
	return 0;
}

/* fixc_to_fixml_01.c ends here */
