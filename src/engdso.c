/*** engdso.c -- module stuff
 *
 * Copyright (C) 2005-2013 Sebastian Freundt
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
 ***/
#if defined HAVE_CONFIG_H
# include "config.h"
#endif	/* HAVE_CONFIG_H */
#include <unistd.h>
#include <string.h>
#include <ltdl.h>
#include <limits.h>
#include "engdso.h"

#if defined DEBUG_FLAG
# include <stdio.h>
# define EDEBUG(args...)	fprintf(stderr, args)
# else	/* !DEBUG_FLAG */
# define EDEBUG(args...)
#endif	/* DEBUG_FLAG */
#if !defined UN
# define UN(x)			x __attribute__((unused))
#endif	/* !UN */


/* stuff we provide until a module is so kind as to override this */
#include "fixml-comp-sub.h"

__attribute__((weak)) fixc_comp_sub_t
fixc_get_comp_sub(fixc_ctxt_t UN(ctx))
{
	EDEBUG("fixc_get_comp_sub() husk called.  Load an engine\n");
	return 0U;
}

#include "fixml-comp-rptb.h"

__attribute__((weak)) fixc_attr_t
fixc_comp_rptb(fixc_ctxt_t UN(ctx))
{
	EDEBUG("fixc_get_comp_rptb() husk called.  Load an engine\n");
	return FIXC_ATTR_UNK;
}

#include "fixml-comp-orb.h"

__attribute__((weak)) fixc_comp_t
fixc_get_comp_orb(fixc_ctxt_t UN(ctx))
{
	EDEBUG("fixc_get_comp_orb() husk called.  Load an engine\n");
	return FIXC_COMP_UNK;
}

#include "fixml-fld-ctx.h"

__attribute__((weak)) fixc_fld_ctx_t
fixc_get_fld_ctx(fixc_ctxt_t UN(ctx))
{
	EDEBUG("fixc_get_fld_ctx() husk called.  Load an engine\n");
	return 0U;
}

#include "fixml-canon-msgt.h"

__attribute__((weak)) const char*
fixc_msgt_fixmlify(fixc_msgt_t UN(m))
{
	EDEBUG("fixc_msgt_fixmlify() husk called.  Load an engine\n");
	return NULL;
}

#include "fixml-canon-comp.h"

__attribute__((weak)) const char*
fixc_comp_fixmlify(fixc_comp_t UN(c))
{
	EDEBUG("fixc_comp_fixmlify() husk called.  Load an engine\n");
	return NULL;
}

#include "fixml-canon-attr.h"

__attribute__((weak)) const char*
fixc_attr_fixmlify(fixc_ctxt_t UN(ctx), fixc_attr_t UN(a))
{
	EDEBUG("fixc_attr_fixmlify() husk called.  Load an engine\n");
	return NULL;
}

#include "fixml-attr-by-ctx.h"

__attribute__((weak)) fixc_attr_t
fixc_get_aid(fixc_ctxt_t UN(ctx), const char *UN(attr), size_t UN(alen))
{
	EDEBUG("fixc_get_aid() husk called.  Load an engine\n");
	return FIXC_ATTR_UNK;
}

#include "fixml-comp-by-ctx.h"

__attribute__((weak)) fixc_comp_t
fixc_get_cid(fixc_ctxt_t UN(ctx), const char *UN(elem), size_t UN(elen))
{
	EDEBUG("fixc_get_cid() husk called.  Load an engine\n");
	return FIXC_COMP_UNK;
}


static void
add_myself(void)
{
	static const char myself[] = "/proc/self/exe";
	static const char moddir[] = PKGLIBDIR;
	static const char eprefix[] = "${exec_prefix}";
	static const char prefix[] = "${prefix}";
	const char *relmoddir;
	char wd[PATH_MAX], *dp;
	size_t sz;

	sz = readlink(myself, wd, sizeof(wd));
	wd[sz] = '\0';
	if ((dp = strrchr(wd, '/')) == NULL) {
		return;
	}
	/* add the path where the binary resides */
	*dp = '\0';
	EDEBUG("adding %s\n", wd);
	lt_dladdsearchdir(wd);

#define MEMCMPLIT(a, b)	memcmp((a), (b), sizeof(b) - 1)
	if (moddir[0] == '/') {
		/* absolute libdir, add him */
		lt_dladdsearchdir(moddir);
	} else if (moddir[0] == '.') {
		/* relative libdir? relative to what? */
		return;
	} else if (memcmp(moddir, eprefix, sizeof(eprefix) - 1) == 0) {
		/* take the bit after EPREFIX for catting later on */
		relmoddir = moddir + sizeof(eprefix) - 1;
	} else if (memcmp(moddir, prefix, sizeof(prefix) - 1) == 0) {
		/* take the bit after PREFIX for catting later on */
		relmoddir = moddir + sizeof(prefix) - 1;
	} else {
		/* don't know, i guess i'll leave ya to it */
		return;
	}

	/* go back one level in dp */
	if ((dp = strrchr(wd, '/')) == NULL) {
		return;
	} else if (strcmp(dp, "/bin") && strcmp(dp, "/sbin")) {
		/* dp doesn't end in /bin nor /sbin */
		return;
	}

	/* good, now we're ready to cat relmoddir to dp */
	strncpy(dp, relmoddir, sizeof(wd) - (dp - wd));
	EDEBUG("adding %s\n", wd);
	lt_dladdsearchdir(wd);
	return;
}

static lt_dlhandle
my_dlopen(const char *filename)
{
	lt_dlhandle handle = 0;
	lt_dladvise advice[1];

	if (!lt_dladvise_init(advice) &&
	    !lt_dladvise_ext(advice) &&
	    !lt_dladvise_global(advice)) {
		handle = lt_dlopenadvise(filename, advice[0]);
	}
	lt_dladvise_destroy(advice);
	return handle;
}


/* pub funs */
fixc_eng_t
fixc_open_eng(const char *file)
{
	static int mod_initted_p = 0;

	/* speed-dating singleton */
	if (!mod_initted_p) {
		/* initialise the dl system */
		lt_dlinit();
		/* add moddir to search path */
		add_myself();
		/* and just so we are a proper singleton */
		mod_initted_p = 1;
	}
	return my_dlopen(file);
}

void
fixc_close_eng(fixc_eng_t eng)
{
	lt_dlclose(eng);
	return;
}

/* engdso.c ends here */
