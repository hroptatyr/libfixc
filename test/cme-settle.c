/*** fix.c -- guts for fix fields and messages
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
#undef __STRICT_ANSI__
#include <unistd.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <argp.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "fix.h"
#include "fixml-msg.h"
#include "fixml-attr.h"

/* won't exist out of our package */
#include "nifty.h"

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
__massage_mdt(char *restrict tgt, const char *src)
{
	while ((*tgt = *src++)) {
		if (*tgt >= '0' && *tgt <= '9') {
			tgt++;
		}
	}
	return;
}

static ssize_t
__meta(fixc_msg_t msg, size_t idx)
{
	struct sym_s {
		const char *xch;
		const char *sym;
		const char *mdt;
		const char *spx;
		const char *mmy;
		const char *sty;
		char poc;
	};
	struct sym_s res = {};

	for (; idx < msg->nflds; idx++) {
		struct fixc_fld_s fld = msg->flds[idx];

		switch (fld.tag) {
			const char *tmp;
		case FIXML_ATTR_MsgType:
			/* that's it, no more to be parsed */
			goto out;
		case FIXML_ATTR_SecurityExchange:
		case FIXML_ATTR_Symbol:
		case FIXML_ATTR_MaturityDate:
		case FIXML_ATTR_MaturityMonthYear:
		case FIXML_ATTR_PutOrCall:
		case FIXML_ATTR_StrikePrice:
		case FIXML_ATTR_SecurityType:
			if (UNLIKELY((tmp = fixc_get_tag(msg, idx)) == NULL)) {
				break;
			}
			switch (fld.tag) {
			case FIXML_ATTR_SecurityExchange:
				res.xch = tmp;
				break;
			case FIXML_ATTR_Symbol:
				res.sym = tmp;
				break;
			case FIXML_ATTR_MaturityDate:
				res.mdt = tmp;
				break;
			case FIXML_ATTR_MaturityMonthYear:
				res.mmy = tmp;
				break;
			case FIXML_ATTR_PutOrCall:
				switch (*tmp) {
				case '0':
					res.poc = 'P';
					break;
				case '1':
					res.poc = 'C';
					break;
				default:
					break;
				}
				break;
			case FIXML_ATTR_StrikePrice:
				res.spx = tmp;
				break;
			case FIXML_ATTR_SecurityType:
				res.sty = tmp;
				break;
			}
			break;
		default:
			/* just overread them */
			break;
		}
	}
out:
	if (res.sym == NULL) {
		;
	} else if (strchr(res.sym, ' ') != NULL) {
		/* fuck!!! */
		fprintf(stderr, "symbol `%s' invalid\n", res.sym);
	} else if (res.sty == NULL) {
		;
	} else if (!strcmp(res.sty, "FUT") || !strcmp(res.sty, "FWD")) {
		fprintf(stdout, "%s_%s_%s\n", res.xch, res.sym, res.mmy);
	} else if (!strcmp(res.sty, "OOC") || !strcmp(res.sty, "OOF")) {
		/* we need to massage the strike price, and also the mdt */
		double spx = strtod(res.spx, NULL);
		static char mdt[9];

		__massage_mdt(mdt, res.mdt);
		fprintf(stdout, "%s_%s_%s%c%012.6f\n",
			res.xch, res.sym, mdt, res.poc, spx);
	} else {
		fprintf(stderr, "don't know how to print sectyp %s\n", res.sty);
	}
	return idx - 1;
}

static int
meta(fixc_msg_t msg)
{
#define FIXML_MSG_MKTSNAP	(FIXML_MSG_MarketDataSnapshotFullRefresh)
	if (msg->f35.mtyp == FIXML_MSG_MKTSNAP) {
		return __meta(msg, 0) < 0 ? -1 : 0;
	} else if (msg->f35.mtyp != FIXC_MSGT_BATCH) {
		/* something weird */
		return -1;
	}
	/* otherwise loop over all f35s */
	for (size_t i = 0; i < msg->nflds; i++) {
		if (msg->flds[i].tag == FIXML_ATTR_MsgType &&
		    msg->flds[i].typ == FIXC_TYP_MSGTYP &&
		    msg->flds[i].mtyp == FIXML_MSG_MKTSNAP) {
			/* lovely */
			i = __meta(msg, i + 1);
		}
	}
	return 0;
}

static int
rinse(fixc_msg_t msg)
{
	return 0;
}

static fixc_msg_t
file2msg(const char *fn)
{
	struct stat st;
	fixc_msg_t res = NULL;
	void *p;
	int fd;

	/* first off, do we actually talk about a file? */
	if (stat(fn, &st) < 0) {
		error("cannot find file `%s'", fn);
		return NULL;
	}
	/* otherwise open the file ... */
	if ((fd = open(fn, O_RDONLY)) < 0) {
		error("cannot open file `%s'", fn);
		return NULL;
	}
	/* ... and mmap the file ... */
	p = mmap(NULL, st.st_size, PROT_READ, MAP_SHARED, fd, 0);
	if (p == MAP_FAILED) {
		error("cannot read file `%s'", fn);
		goto clos_out;
	}
	/* ... and call the reader */
	if ((res = make_fixc_from_fixml(p, st.st_size)) == NULL) {
		error("cannot parse file `%s'", fn);
		goto munm_out;
	}
munm_out:
	munmap(p, st.st_size);
clos_out:
	close(fd);
	return res;
}


const char *argp_program_version = "cme-settle v0.1";
const char *argp_program_bug_address = "\
http://github.com/hroptatyr/libfixc/issues/";

static void
argp_state_version(FILE *f, struct argp_state *state)
{
	fputs(argp_program_version, f);
	fputc('\n', f);
	if (!(state->flags & ARGP_NO_EXIT)) {
		exit(0);
	}
	return;
}

static error_t
parse_opt(int key, char *arg, struct argp_state *state)
{
#if !defined OPT_USAGE
# define OPT_USAGE	('u' + 0x100)
#endif	/* OPT_USAGE */
#define OPT_RINSE	('r' + 0x100)
#define OPT_META	('m' + 0x100)
	static unsigned int help = ARGP_HELP_USAGE;
	static unsigned int mode = 0U;

	switch (key) {
	case 'h':
		help |= ARGP_HELP_STD_HELP;
	case OPT_USAGE:
		help |= ARGP_HELP_EXIT_OK;
		argp_state_help(state, stdout, help);
		break;
	case 'V':
		argp_state_version(stdout, state);
		break;

	case OPT_RINSE:
		if ((mode |= OPT_RINSE) != OPT_RINSE) {
			goto mode_conflict;
		}
		break;
	case OPT_META:
		if ((mode |= OPT_META) != OPT_META) {
			goto mode_conflict;
		}
		break;

	case ARGP_KEY_ARG:
		switch (mode) {
			fixc_msg_t msg;
		case OPT_RINSE:
		case OPT_META:
			if ((msg = file2msg(arg)) == NULL) {
				/* next file please */
				break;
			}
			switch (mode) {
			case OPT_RINSE:
				rinse(msg);
				break;
			case OPT_META:
				meta(msg);
				break;
			default:
				abort();
			}
			/* make sure we leave no stains */
			free_fixc(msg);
			break;
		default:
		mode_conflict:
			fputs("\
Exactly one of --rinse and --meta must be specified.\n", stderr);
			help |= ARGP_HELP_EXIT_ERR;
			argp_state_help(state, stderr, help);
			break;
		}
		break;

	case ARGP_KEY_END:
		if (state->arg_num == 0) {
		case ARGP_KEY_NO_ARGS:
			/* not enough arguments */
			argp_usage(state);
		}
		break;
	default:
		return ARGP_ERR_UNKNOWN;
	}
	return 0;
}

int
main(int argc, char *argv[])
{
	static const char doc[] = "\
Generate meta data from cme settlement files";
	static char args_doc[] = "FILE...";
	static struct argp_option options[] = {
		{"help", 'h', NULL, OPTION_NO_USAGE, "\
Print this help list", 0},
		{"usage", OPT_USAGE, NULL, OPTION_NO_USAGE | OPTION_HIDDEN, "\
Print a short usage message", 0},
		{"version", 'V', NULL, OPTION_NO_USAGE, "\
Print program version", 0},
		{"rinse", OPT_RINSE, NULL, 0, "\
Output rinsed data", 1},
		{"meta", OPT_META, NULL, 0, "\
Output meta data", 1},
		{0},
	};
	static const struct argp argp = {
		options,
		parse_opt,
		args_doc,
		doc
	};

	/* parse args
	 * this also calls the worker funs */
	argp_parse(&argp, argc, argv, ARGP_NO_HELP, NULL, NULL);

	return 0;
}

/* cme-settle-meta.c ends here */
