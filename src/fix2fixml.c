/*** fix2fixml.c -- fix reader and fixml writer
 *
 * Copyright (C) 2010-2012 Sebastian Freundt
 *
 * Author:  Sebastian Freundt <freundt@ga-group.nl>
 *
 * This file is part of libfixc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the author nor the names of any contributors
 *    may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 **/
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>

#include "fix.h"

#if defined DEBUG_FLAG
# define FIXC_DEBUG(args...)	fprintf(stderr, args)
#else  /* !DEBUG_FLAG */
# define FIXC_DEBUG(args...)
#endif	/* DEBUG_FLAG */

static int
proc1(const char *file)
{
	static char buf[65536];
	struct stat st;
	int fd;
	void *p;
	fixc_msg_t msg;
	int res = 0;

	if (stat(file, &st) < 0) {
		return -1;
	}
	/* otherwise open the file ... */
	if ((fd = open(file, O_RDONLY)) < 0) {
		perror("cannot open file");
		return -1;
	}
	/* ... and mmap the file ... */
	p = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		perror("cannot read file");
		res = -1;
		goto clos_out;
	}
	/* ... and call the reader */
	if ((msg = make_fixc_from_fix(p, st.st_size)) == NULL) {
		fputs("cannot parse file\n", stderr);
		res = -1;
		goto munm_out;
	}
	/* render the result */
	{
		size_t nwr = fixc_render_fixml(buf, sizeof(buf), msg);
		fwrite(buf, 1, nwr, stdout);
		fputc('\n', stdout);
	}

	/* free our resources */
	free_fixc(msg);
munm_out:
	munmap(p, st.st_size);
clos_out:
	close(fd);
	return res;
}


int
main(int argc, char *argv[])
{
	int res = 0;

	for (int i = 1; i < argc; i++) {
		res -= proc1(argv[i]);
	}
	return res;
}

/* fix2fixml.c ends here */
