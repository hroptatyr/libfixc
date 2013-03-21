/*** fixml2fix.c -- fixml reader and fix writer
 *
 * Copyright (C) 2010-2013 Sebastian Freundt
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
#include <stdarg.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <errno.h>

#include "fix.h"
#include "nifty.h"

#if defined DEBUG_FLAG
# define FIXC_DEBUG(args...)	fprintf(stderr, args)
#else  /* !DEBUG_FLAG */
# define FIXC_DEBUG(args...)
#endif	/* DEBUG_FLAG */

static int verbp = 0;
static int fixmlp = 0;
static char tabc = '\t';

static int
__attribute__((format(printf, 1, 2)))
error(const char *fmt, ...)
{
	va_list vap;
	va_start(vap, fmt);
	vfprintf(stderr, fmt, vap);
	va_end(vap);
	if (errno) {
		fputc(':', stderr);
		fputc(' ', stderr);
		fputs(strerror(errno), stderr);
	}
	fputc('\n', stderr);
	return errno;
}

static void
pr_fld(int num, struct fixc_fld_s fld)
{
	fprintf(stderr, "FLD[%i] = {\n\
  .tag = %hu,\n\
  .typ = %hu,\n\
  .tpc = %hu,\n\
  .cnt = %hu,\n\
  .val = 0x%lx,\n\
};\n", num, fld.tag, fld.typ, fld.tpc, fld.cnt, fld.i64);
	return;
}

static int
proc1(const char *file)
{
	struct stat st;
	int fd;
	void *p;
	fixc_msg_t msg;
	int res = 0;
	struct fixc_rndr_s rbuf;

	if (stat(file, &st) < 0) {
		return error("cannot find file `%s'", file);
	}
	/* otherwise open the file ... */
	if ((fd = open(file, O_RDONLY)) < 0) {
		return error("cannot open file `%s'", file);
	}
	/* ... and mmap the file ... */
	p = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		res = error("cannot read file `%s'", file);
		goto clos_out;
	}
	/* ... and call the reader */
	if ((msg = make_fixc_from_fixml(p, st.st_size)) == NULL) {
		res = error("cannot parse file `%s'", file);
		goto munm_out;
	}
	if (UNLIKELY(verbp)) {
		pr_fld(-4, msg->f8);
		pr_fld(-3, msg->f9);
		pr_fld(-2, msg->f35);
		for (size_t i = 0; i < msg->nflds; i++) {
			pr_fld(i, msg->flds[i]);
		}
		pr_fld(-1, msg->f10);
	}
	/* render the result */
	if (!fixmlp) {
		rbuf = fixc_render_fix_rndr(msg);
	} else {
		rbuf = fixc_render_fixml_rndr(msg);
	}

	/* actually print the whole shebang, escape stuff on ttys */
	if (isatty(STDOUT_FILENO)) {
		for (char *q = rbuf.str; (q = strchr(q, '\001'));) {
			/* the actual character could be configurable no? */
			*q = tabc;
		}
	}
	write(STDOUT_FILENO, rbuf.str, rbuf.len);
	write(STDOUT_FILENO, "\n", 1);

	/* free our resources */
	fixc_free_rndr(rbuf);
	free_fixc(msg);
munm_out:
	munmap(p, st.st_size);
clos_out:
	close(fd);
	return res;
}


static void
pr_usage(FILE *whither)
{
	static char help[] = "\
fixml2fix " PACKAGE_VERSION "\n\
\n\
Usage: fixml2fix [OPTION]... [FILE]...\n\
\n\
  -h                Print help and exit\n\
  -V                Print version and exit\n\
\n\
  -v                Verbose mode, show internal states\n\
  -x                Output FIXML again\n\
\n\
  -t CHAR           Use CHAR as tabbing character to separate\n\
                    fix fields (only on a tty)\n\
";

	fwrite(help, 1, sizeof(help) - 1, whither);
	return;
}

static void
pr_version(FILE *whither)
{
	static char vrsn[] = "fixml2fix " PACKAGE_VERSION "\n";

	fwrite(vrsn, 1, sizeof(vrsn) - 1, whither);
	return;
}

int
main(int argc, char *argv[])
{
	static const char esc_map[] = "\a\bcd\e\fghijklm\nopq\rs\tu\v";
	int res = 0;

	for (int opt; (opt = getopt(argc, argv, "hxvVt:")) != -1;) {
		switch (opt) {
		case 'h':
			pr_usage(stdout);
			goto out;
		case 'v':
			verbp = 1;
			break;
		case 'x':
			fixmlp = 1;
			break;
		case 't':
			if ((tabc = *optarg++) != '\\') {
				;
			} else if (*optarg < 'a' || *optarg > 'v') {
				tabc = '\t';
			} else {
				tabc = esc_map[*optarg - 'a'];
			}
			break;
		case 'V':
			pr_version(stdout);
			goto out;
		default:
			fputc('\n', stderr);
			pr_usage(stderr);
			goto out;
		}
	}

	for (int i = optind; i < argc; i++) {
		res -= proc1(argv[i]);
	}
out:
	return res;
}

/* fixml2fix.c ends here */
