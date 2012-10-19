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
EXTRA_DIST += fixml-attr.h
EXTRA_DIST += fixml-comp.h
fixml_tests += fixml_to_fixc_hash_01
#fixml_tests += fixml_to_fixc_hash_02
fixml_tests += fixml_to_fixc_hash_03
#fixml_tests += fixml_to_fixc_hash_04
fixml_tests += fixml_to_fixc_hash_05
#fixml_tests += fixml_to_fixc_hash_06
fixml_tests += fixml_to_fixc_hash_07
#fixml_tests += fixml_to_fixc_hash_08
fixml_tests += fixml_to_fixc_hash_09
#fixml_tests += fixml_to_fixc_hash_10
fixml_tests += fixml_to_fixc_hash_11
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
api_tests += fixc_cpy_01
fixc_cpy_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_cpy_02
fixc_cpy_02_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_cpy_03
fixc_cpy_03_LDADD = $(top_builddir)/src/libfixc.la
#api_tests += fixc_add_01
#fixc_add_01_LDADD = $(top_builddir)/src/libfixc.la
api_tests += fixc_add_02
fixc_add_02_LDADD = $(top_builddir)/src/libfixc.la

rd_tests += fixml_rd_01
fixml_rd_01_LDADD = $(top_builddir)/src/libfixc.la

TESTS_ENVIRONMENT += top_builddir=$(top_builddir)
EXTRA_DIST += $(bin_tests)
bin_tests += fixml2fix_01.sh
bin_tests += fixml2fix_02.sh
bin_tests += fix2fixml_01.sh
bin_tests += fix2fixml_02.sh

## Makefile.am ends here
