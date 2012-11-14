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
#include <stdlib.h>
#include <stdio.h>
#include <argp.h>
#include "fix.h"

#if !defined UNUSED
# define UNUSED(_x)	_x __attribute__((unused))
#endif	/* !UNUSED */

static int
meta(const char *UNUSED(fn))
{
	return 0;
}

static int
rinse(const char *UNUSED(fn))
{
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
		case OPT_RINSE:
			rinse(arg);
			break;
		case OPT_META:
			meta(arg);
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
