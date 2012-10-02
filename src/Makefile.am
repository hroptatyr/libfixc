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
libfixc_la_SOURCES += fixml-wr.c
libfixc_la_SOURCES += fixml-rd.c
libfixc_la_SOURCES += nifty.h
libfixc_la_SOURCES += fixml-attr.gperf fixml-attr.h
libfixc_la_SOURCES += switch-group-attr.awk
libfixc_la_SOURCES += fixml-comp.gperf fixml-comp.h
libfixc_la_SOURCES += switch-group-comp.awk
libfixc_la_SOURCES += fixml-comp-sub.xsl fixml-comp-sub.h
libfixc_la_SOURCES += fixml-comp-fld.xsl fixml-comp-fld.h
libfixc_la_SOURCES += fixml-fld-ctx.xsl fixml-fld-ctx.h
libfixc_la_SOURCES += fixml-msg-type.gperf fix-msg-type.h
libfixc_la_SOURCES += fixml-nsuri.gperf fix-nsuri.h
libfixc_la_CPPFLAGS = $(AM_CPPFLAGS)
libfixc_la_CPPFLAGS += $(expat_CFLAGS)
libfixc_la_LDFLAGS = $(AM_LDFLAGS)
libfixc_la_LDFLAGS += $(expat_LIBS)
BUILT_SOURCES += fixml-attr.c
BUILT_SOURCES += fixml-attr-rev.c
BUILT_SOURCES += fixml-comp.c
BUILT_SOURCES += fixml-comp-rev.c
BUILT_SOURCES += fixml-comp-sub.c
BUILT_SOURCES += fixml-comp-fld.c
BUILT_SOURCES += fixml-fld-ctx.c
BUILT_SOURCES += fixml-msg-type.c
BUILT_SOURCES += fixml-msg-type-rev.c
BUILT_SOURCES += fixml-nsuri.c
BUILT_SOURCES += fixml-nsuri-rev.c

header_HEADERS += fix.h
header_HEADERS += fix-msg-type.h

## just for debugging and fun
noinst_PROGRAMS += fix
fix_CPPFLAGS = $(AM_CPPFLAGS)
fix_CPPFLAGS += -DSTANDALONE

## just for debugging and fun
noinst_PROGRAMS += fixml-wr
fixml_wr_CPPFLAGS = $(AM_CPPFLAGS)
fixml_wr_CPPFLAGS += -DSTANDALONE
fixml_wr_LDADD = libfixc.la



## our rule for gperf
%.c: %.gperf
	$(AM_V_GEN) $(GPERF) $< --output-file $@

fixml-attr-rev.c: $(top_builddir)/spec/fixml_attr.50SP2 switch-group-attr.awk
	sort -k2 $< | \
	awk -f $(word 2,$+) > $@

fixml-comp-rev.c: $(top_builddir)/spec/fixml_comp.50SP2 \
		$(top_srcdir)/spec/uncamel.sed \
		switch-group-comp.awk
	cut -f2 $< | sort -u > fixml-comp-rev.tgt
	cat fixml-comp-rev.tgt | sed -f $(word 2,$+) -e 's/^/FIXC_COMP_/' | \
		tr 'a-z' 'A-Z' > fixml-comp-rev.src
	paste fixml-comp-rev.src fixml-comp-rev.tgt | \
		awk -f $(word 3,$+) > $@
	$(RM) fixml-comp-rev.src fixml-comp-rev.tgt

%.c: $(top_srcdir)/spec/FIX.5.0SP2.xml %.xsl $(top_srcdir)/spec/uncamel.sed
	xsltproc $(word 2,$+) $< | \
		sed -f $(word 3,$+) \
			-e 's/FIXC_ATTR_.*/\U&/' \
			-e 's/FIXC_MSGTYP_.*/\U&/' \
			-e 's/FIXC_COMP_.*/\U&/' > $@

# 
# Help the developers get nice post-processed source files

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<
