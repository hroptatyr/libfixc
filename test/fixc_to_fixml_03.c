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
	static char buf[4096U];
	fixc_msg_t msg;
	size_t z;

	/* start a fix msg for that */
	msg = make_fixc_msg((fixc_msgt_t)FIXC_MSGT_BATCH);

	/* render the whole shebang */
	z = fixc_render_fixml(buf, sizeof(buf), msg);
	free_fixc(msg);

	if (z != sizeof(res) - 1) {
		fprintf(stderr, "sizes differ %zu v %zu\n%s\n  v\n%s\n",
			z, sizeof(res) - 1,
			buf, res);
		return 1;
	} else if (memcmp(buf, res, z)) {
		fprintf(stderr, "buffers differ:\n%s\n  v\n%s\n", buf, res);
		return 1;
	}
	return 0;
}

/* fixc_to_fixml_03.c ends here */
