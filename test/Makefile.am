# Help the Developers and yourself. Just use the C locale and settings
# for the compilation. They can still be overriden by make LANG=<whatever>
# but that is general a not very good idea
LANG = C
LC_ALL = C

libsrcdir = $(top_srcdir)/src
libblddir = $(top_builddir)/src

AM_CPPFLAGS = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
AM_CPPFLAGS += -DTEST -I$(libsrcdir) -I$(libblddir)

TESTS =
TESTS_ENVIRONMENT =
TEST_EXTENSIONS =
check_PROGRAMS =
CLEANFILES = $(check_PROGRAMS)
EXTRA_DIST =

noinst_PROGRAMS =

TESTS += $(struct_tests)
check_PROGRAMS += $(struct_tests)
struct_tests =

TESTS += $(fixml_tests)
check_PROGRAMS += $(fixml_tests)
fixml_tests =

TESTS += $(rd_tests)
check_PROGRAMS += $(rd_tests)
rd_tests =

TESTS += $(api_tests)
check_PROGRAMS += $(api_tests)
api_tests =

TESTS += $(bin_tests)
bin_tests =

struct_tests += struct_layout_01

## some fixml example files
EXTRA_DIST += fixml_ex_01.xml
EXTRA_DIST += fixml_ex_02.xml
EXTRA_DIST += fix_ex_01.fix
EXTRA_DIST += fix_ex_02.fix

EXTRA_DIST += fixml_to_fixc_prep.c
#fixml_tests += fixml_to_fixc_hash_01
#fixml_tests += fixml_to_fixc_hash_02
#fixml_tests += fixml_to_fixc_hash_03
#fixml_tests += fixml_to_fixc_hash_04
#fixml_tests += fixml_to_fixc_hash_05
#fixml_tests += fixml_to_fixc_hash_06
#fixml_tests += fixml_to_fixc_hash_07
#fixml_tests += fixml_to_fixc_hash_08
#fixml_tests += fixml_to_fixc_hash_09
#fixml_tests += fixml_to_fixc_hash_10
#fixml_tests += fixml_to_fixc_hash_11
#fixml_tests += fixml_to_fixc_hash_12

#fixml_tests += fixc_to_fixml_hash_01
#fixml_tests += fixc_to_fixml_hash_02
#fixml_tests += fixc_to_fixml_hash_03
#fixml_tests += fixc_to_fixml_hash_04
#fixml_tests += fixc_to_fixml_hash_05
#fixml_tests += fixc_to_fixml_hash_06

api_tests += fixc_extr_01
fixc_extr_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_extr_02
fixc_extr_02_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_extr_deep_01
fixc_extr_deep_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_cpy_01
fixc_cpy_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_cpy_02
fixc_cpy_02_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_cpy_03
fixc_cpy_03_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_01
fixc_add_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_02
fixc_add_02_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_render_fixml_01
fixc_render_fixml_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_render_fixml_02
fixc_render_fixml_02_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_fld_01
fixc_add_fld_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_fld_02
fixc_add_fld_02_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_fld_03
fixc_add_fld_03_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_fld_04
fixc_add_fld_04_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_fld_05
fixc_add_fld_05_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_fld_06
fixc_add_fld_06_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_tag_01
fixc_add_tag_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_tag_02
fixc_add_tag_02_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_msg_cpy_01
fixc_msg_cpy_01_LDADD = $(top_builddir)/src/libfixc.la

TESTS_ENVIRONMENT += MALLOC_PERTURB_=64
rd_tests += fixml_rd_01
fixml_rd_01_LDADD = $(top_builddir)/src/libfixc.la

rd_tests += fixc_from_fix_01
fixc_from_fix_01_LDADD = $(top_builddir)/src/libfixc.la
## same thing but static
rd_tests += fixc_from_fix_02
fixc_from_fix_02_SOURCES = fixc_from_fix_01.c
fixc_from_fix_02_LDFLAGS = $(AM_LDFLAGS) -static
fixc_from_fix_02_LDADD = $(top_builddir)/src/libfixc.la

rd_tests += fixc_from_fixml_01
fixc_from_fixml_01_LDADD = $(top_builddir)/src/libfixc.la
## same thing but static
rd_tests += fixc_from_fixml_02
fixc_from_fixml_02_SOURCES = fixc_from_fixml_01.c
fixc_from_fixml_02_LDFLAGS = $(AM_LDFLAGS) -static
fixc_from_fixml_02_LDADD = $(top_builddir)/src/libfixc.la

rd_tests += fixc_from_fixml_03
fixc_from_fixml_03_LDADD = $(top_builddir)/src/libfixc.la
## same thing but static
rd_tests += fixc_from_fixml_04
fixc_from_fixml_04_SOURCES = fixc_from_fixml_03.c
fixc_from_fixml_04_LDFLAGS = $(AM_LDFLAGS) -static
fixc_from_fixml_04_LDADD = $(top_builddir)/src/libfixc.la

rd_tests += fixc_from_fixml_05
fixc_from_fixml_05_LDADD = $(top_builddir)/src/libfixc.la
## same thing but static
rd_tests += fixc_from_fixml_06
fixc_from_fixml_06_SOURCES = fixc_from_fixml_05.c
fixc_from_fixml_06_LDFLAGS = $(AM_LDFLAGS) -static
fixc_from_fixml_06_LDADD = $(top_builddir)/src/libfixc.la

TESTS_ENVIRONMENT += top_builddir=$(top_builddir)
EXTRA_DIST += $(bin_tests)
bin_tests += fixml2fix_01.sh
bin_tests += fixml2fix_02.sh
bin_tests += fix2fixml_01.sh
bin_tests += fix2fixml_02.sh
bin_tests += fixml2fix_03.sh
EXTRA_DIST += fixml_ex_06.xml

## check behaviour with unkown/custom tags
EXTRA_DIST += fix_ex_05.fix
EXTRA_DIST += fixml_ex_07.xml
#bin_tests += fix2fixml_03.sh

## testing them CME settlement files, requisites
noinst_PROGRAMS += cme-settle
cme_settle_SOURCES = cme-settle.c
cme_settle_CPPFLAGS = $(AM_CPPFLAGS)
cme_settle_CPPFLAGS += -D_GNU_SOURCE
cme_settle_LDFLAGS = $(AM_LDFLAGS)
cme_settle_LDADD = $(top_builddir)/src/libfixc.la

## Makefile.am ends here
