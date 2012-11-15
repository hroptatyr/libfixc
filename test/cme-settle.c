/*** cme-settle.c -- dealing with them files in settle/
 *
 * Copyright (C) 2012 Sebastian Freundt
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


struct sym_s {
	const char *xch;
	const char *sym;
	const char *mdt;
	const char *spx;
	const char *mmy;
	const char *sty;
	char poc;
};

struct ins_s {
	const char *id;
	const char *cfi;
	struct sym_s undly;
};

struct pxnfo_s {
	/* tag 268 */
	size_t nentries;
	/* that's stl, close, high, low, high_b, low_a, vol, oi, pai */
	struct {
		char ent_typ;
		const char *px;
	} entries[9];
};

struct snarf_s {
	struct sym_s s;
	struct ins_s i;
	struct pxnfo_s pxi;
};

static void
__snarf_fld(struct snarf_s *tgt, fixc_msg_t msg, size_t idx)
{
	const char *tmp;

	if (UNLIKELY((tmp = fixc_get_tag(msg, idx)) == NULL)) {
		return;
	}
	switch (msg->flds[idx].tag) {
	case FIXML_ATTR_SecurityExchange:
		tgt->s.xch = tmp;
		break;
	case FIXML_ATTR_Symbol:
		tgt->s.sym = tmp;
		break;
	case FIXML_ATTR_MaturityDate:
		tgt->s.mdt = tmp;
		break;
	case FIXML_ATTR_MaturityMonthYear:
		tgt->s.mmy = tmp;
		break;
	case FIXML_ATTR_PutOrCall:
		switch (*tmp) {
		case '0':
			tgt->s.poc = 'P';
			break;
		case '1':
			tgt->s.poc = 'C';
			break;
		default:
			break;
		}
		break;
	case FIXML_ATTR_StrikePrice:
		tgt->s.spx = tmp;
		break;
	case FIXML_ATTR_SecurityType:
		tgt->s.sty = tmp;
		break;
	case FIXML_ATTR_SecurityID:
		tgt->i.id = tmp;
		break;
	case FIXML_ATTR_CFICode:
		tgt->i.cfi = tmp;
		break;

	case FIXML_ATTR_UnderlyingSecurityExchange:
		tgt->i.undly.xch = tmp;
		break;
	case FIXML_ATTR_UnderlyingSecurityID:
		tgt->i.undly.sym = tmp;
		break;
	case FIXML_ATTR_UnderlyingSecurityType:
		tgt->i.undly.sty = tmp;
		break;
	case FIXML_ATTR_UnderlyingMaturityMonthYear:
		tgt->i.undly.mmy = tmp;
		break;
	case FIXML_ATTR_UnderlyingPutOrCall:
		switch (*tmp) {
		case '0':
			tgt->i.undly.poc = 'P';
			break;
		case '1':
			tgt->i.undly.poc = 'C';
			break;
		default:
			break;
		}
		break;

	default:
		break;
	}
	return;
}

static ssize_t
__snarf_mdent(struct snarf_s *tgt, fixc_msg_t msg, size_t idx, size_t nent)
{
	for (ssize_t j = -1; idx < msg->nflds; idx++) {
		const char *tmp;

		if (UNLIKELY((tmp = fixc_get_tag(msg, idx)) == NULL)) {
			continue;
		}
		switch (msg->flds[idx].tag) {
		case FIXML_ATTR_MDEntryType:
			if (j++ >= (ssize_t)nent) {
				return idx;
			}
			tgt->pxi.entries[j].ent_typ = *tmp;
			break;
		case FIXML_ATTR_MDEntryPx:
			tgt->pxi.entries[j].px = tmp;
			break;
		default:
			break;
		}
	}
	return idx;
}

static ssize_t
__snarf(struct snarf_s *tgt, fixc_msg_t msg, size_t idx)
{
	struct snarf_s res = {};

	for (; idx < msg->nflds; idx++) {
		switch (msg->flds[idx].tag) {
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
		case FIXML_ATTR_SecurityID:
		case FIXML_ATTR_CFICode:

		case FIXML_ATTR_UnderlyingSecurityExchange:
		case FIXML_ATTR_UnderlyingSecurityID:
		case FIXML_ATTR_UnderlyingSecurityType:
		case FIXML_ATTR_UnderlyingMaturityMonthYear:
		case FIXML_ATTR_UnderlyingStrikePrice:
		case FIXML_ATTR_UnderlyingPutOrCall:
			__snarf_fld(&res, msg, idx);
			break;

		case FIXML_ATTR_NoMDEntries: {
			/* field type should be INT already */
			size_t nent;

			if ((nent = msg->flds[idx].i32)) {
				res.pxi.nentries = nent;
				idx = __snarf_mdent(&res, msg, idx + 1, nent);
			}
			break;
		}
		default:
			/* just overread them */
			break;
		}
	}
out:
	*tgt = res;
	return idx - 1;
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

static void
__massage_spx(char *restrict tgt, const char *src)
{
	size_t poff;
	size_t ndgt;

	if (*src == '-') {
		*tgt++ = *src++;
	}
	/* try and find the decimal point */
	for (poff = 0; poff < 10 && *src != '\0' && *src != '.'; src++, poff++);
	/* fill up tgt with 0 */
	for (size_t i = 0; i + poff < 5; i++) {
		*tgt++ = '0';
	}
	/* rewind src and tgt */
	src -= poff;
	for (ndgt = 0; ndgt <= poff + 6 && *src; ndgt++) {
		*tgt++ = *src++;
	}
	/* and fill up with 0 */
	for (; ndgt <= poff + 6; ndgt++) {
		*tgt++ = '0';
	}
	*tgt = '\0';
	return;
}

static int
pr_sym(struct snarf_s *snf)
{
	if (snf->s.sym == NULL) {
		return -1;
	} else if (strchr(snf->s.sym, ' ') != NULL) {
		/* fuck!!! */
		fprintf(stderr, "symbol `%s' invalid\n", snf->s.sym);
		return -1;
	} else if (snf->s.sty == NULL) {
		return -1;
	} else if (!strcmp(snf->s.sty, "FUT") || !strcmp(snf->s.sty, "FWD")) {
		fprintf(stdout, "%s_%s_%s",
			snf->s.xch, snf->s.sym, snf->s.mmy);
	} else if (!strcmp(snf->s.sty, "OOC") || !strcmp(snf->s.sty, "OOF")) {
		/* we need to massage the strike price, and also the mdt */
		static char mdt[9];
		static char spx[14];

		__massage_mdt(mdt, snf->s.mdt);
		__massage_spx(spx, snf->s.spx);
		fprintf(stdout, "%s_%s_%s%c%s",
			snf->s.xch, snf->s.sym, mdt, snf->s.poc, spx);
	} else {
		fprintf(stderr, "don't know how to print sectyp %s\n",
			snf->s.sty);
		return -1;
	}
	return 0;
}

static int
pr_ins(struct snarf_s *snf)
{
	if (snf->s.sym == NULL) {
		return -1;
	} else if (strchr(snf->s.sym, ' ') != NULL) {
		/* fuck!!! */
		fprintf(stderr, "symbol `%s' invalid\n", snf->s.sym);
		return -1;
	} else if (!strcmp(snf->s.sym, snf->i.id)) {
		fprintf(stdout, "\
Sym=%s Exch=%s CFI=%s SecTyp=%s MMY=%s MatDt=%s",
			snf->s.sym, snf->s.xch, snf->i.cfi,
			snf->s.sty, snf->s.mmy, snf->s.mdt);
	} else {
		fprintf(stdout, "\
Sym=%s ID=%s Exch=%s CFI=%s SecTyp=%s MMY=%s MatDt=%s",
			snf->s.sym, snf->i.id, snf->s.xch, snf->i.cfi,
			snf->s.sty, snf->s.mmy, snf->s.mdt);
	}
	if (!strcmp(snf->s.sty, "OOC") || !strcmp(snf->s.sty, "OOF")) {
		static const char c[] = "Call";
		static const char p[] = "Put";
		const char *pc = NULL;

		switch (snf->s.poc) {
		case 'C':
			pc = c;
			break;
		case 'P':
			pc = p;
			break;
		default:
			break;
		}
		fprintf(stdout, " StrkPx=%s %s  over: ", snf->s.spx, pc);
	}
	if (snf->i.undly.sty) {
		fprintf(stdout, "%s %s %s %s",
			snf->i.undly.sym,
			snf->i.undly.xch,
			snf->i.undly.sty,
			snf->i.undly.mmy);
	}
	return 0;
}

static int
meta(struct snarf_s *snf)
{
	fputc('\t', stdout);
	(void)pr_ins(snf);
	return 0;
}

static int
rinse(struct snarf_s *snf)
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

static int
work(fixc_msg_t msg, unsigned int mode)
{
#define OPT_SYMS		(0x0)
#define OPT_RINSE		('r' + 0x100)
#define OPT_META		('m' + 0x100)
#define FIXML_MSG_MKTSNAP	(FIXML_MSG_MarketDataSnapshotFullRefresh)
	struct snarf_s snf[1];
	size_t i = 0;

	if (msg->f35.mtyp == FIXML_MSG_MKTSNAP) {
		i = -1UL;
		goto snarf;
	} else if (msg->f35.mtyp != FIXC_MSGT_BATCH) {
		/* something weird */
		return -1;
	}
	/* otherwise loop over all f35s */
	do {
		if (msg->flds[i].tag == FIXML_ATTR_MsgType &&
		    msg->flds[i].typ == FIXC_TYP_MSGTYP &&
		    msg->flds[i].mtyp == FIXML_MSG_MKTSNAP) {
			/* lovely */
		snarf:
			i = __snarf(snf, msg, i + 1);

			if (pr_sym(snf) < 0) {
				continue;
			}

			switch (mode) {
			case OPT_RINSE:
				rinse(snf);
				break;
			case OPT_META:
				meta(snf);
				break;
			case OPT_SYMS:
			default:
				break;
			}

			/* conclude the line */
			fputc('\n', stdout);
		}
	} while (++i < msg->nflds);
	return 0;
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
		case OPT_SYMS:
		case OPT_RINSE:
		case OPT_META:
			if ((msg = file2msg(arg)) == NULL) {
				/* next file please */
				break;
			}
			/* the actual work */
			work(msg, mode);
			/* make sure we leave no stains */
			free_fixc(msg);
			break;
		default:
		mode_conflict:
			fputs("\
Only one of --rinse and --meta must be specified.\n", stderr);
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
