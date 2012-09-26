### Makefile.am

AM_CPPFLAGS = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
AM_LDFLAGS =

headerdir = $(includedir)/libfixc

bin_PROGRAMS =
noinst_PROGRAMS =
lib_LTLIBRARIES =
noinst_LTLIBRARIES =
noinst_HEADERS =
header_HEADERS =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST = $(BUILT_SOURCES)

lib_LTLIBRARIES += libfixc.la
libfixc_la_SOURCES = fix.c fix.h
libfixc_la_SOURCES += fix-msg-type.h
libfixc_la_SOURCES += nifty.h
libfixc_la_SOURCES += fixml-attr.gperf fixml-attr.h
libfixc_la_CPPFLAGS = $(AM_CPPFLAGS)
libfixc_la_LDFLAGS = $(AM_LDFLAGS)
BUILT_SOURCES += fixml-attr.c

header_HEADERS += fix.h
header_HEADERS += fix-msg-type.h

## just for debugging and fun
noinst_PROGRAMS += fix
fix_CPPFLAGS = $(AM_CPPFLAGS)
fix_CPPFLAGS += -DSTANDALONE

## just for debugging and fun
noinst_PROGRAMS += fixml
fixml_CPPFLAGS = $(AM_CPPFLAGS)
fixml_CPPFLAGS += -DSTANDALONE
fixml_LDADD = libfixc.la

## our rule for gperf
%.c: %.gperf
	$(AM_V_GEN) $(GPERF) $< --output-file $@

# 
# Help the developers get nice post-processed source files

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<
