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

struct_tests += struct_layout_01

## some fixml example files
EXTRA_DIST += fixml_ex_01.xml
EXTRA_DIST += fixml_ex_02.xml

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

rd_tests += fixml_rd_01
fixml_rd_01_LDADD = $(top_builddir)/src/libfixc.la

## Makefile.am ends here
