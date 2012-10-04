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
libfixc_la_SOURCES += fixml-comp.xsl fixml-msg.xsl
libfixc_la_SOURCES += fixml-attr-by-ctx.xsl
libfixc_la_SOURCES += fixml-attr-by-ctx.c fixml-attr-by-ctx.h
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
BUILT_SOURCES += fix40-msg.h fix40-comp.h
BUILT_SOURCES += fix41-msg.h fix41-comp.h
BUILT_SOURCES += fix42-msg.h fix42-comp.h
BUILT_SOURCES += fix43-msg.h fix43-comp.h
BUILT_SOURCES += fix44-msg.h fix44-comp.h
BUILT_SOURCES += fix50-msg.h fix50-comp.h
BUILT_SOURCES += fix50sp1-msg.h fix50sp1-comp.h
BUILT_SOURCES += fix50sp2-msg.h fix50sp2-comp.h
BUILT_SOURCES += fixml-attr-by-ctx.c

header_HEADERS += fix.h
header_HEADERS += fix-msg-type.h

## just for debugging and fun
noinst_PROGRAMS += fixml2fix
fixml2fix_CPPFLAGS = $(AM_CPPFLAGS)
fixml2fix_CPPFLAGS += -DSTANDALONE
fixml2fix_LDADD = libfixc.la

noinst_PROGRAMS += fix2fixml
fix2fixml_CPPFLAGS = $(AM_CPPFLAGS)
fix2fixml_CPPFLAGS += -DSTANDALONE
fix2fixml_LDADD = libfixc.la


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

include $(DEPDIR)/fixml-attr-by-ctx.Po
fixml-attr-by-ctx.c: fixml-attr-by-ctx.xsl fix50sp2.xml fixml-attr-by-ctx.gperfs
	xsltproc -o $@ $< $(word 2,$+)

fixml-attr-by-ctx.gperfs: fixml-attr-by-ctx.xsl fix50sp2.xml
	xsltproc --stringparam MT $(DEPDIR)/fixml-attr-by-ctx.Po $+ > $@

dist-hook: fixml-attr-by-ctx.gperfs
	for i in `cat $<`; do cp -a "`dirname $<`"/"$${i}" "$(distdir)/"; done
	cp -a $< "$(distdir)/"

fix50sp2.xml: $(top_srcdir)/spec/FIX.5.0SP2.xml
	$(LN_S) $< $@

fix50sp1.xml: $(top_srcdir)/spec/FIX.5.0SP1.xml
	$(LN_S) $< $@

fix50.xml: $(top_srcdir)/spec/FIX.5.0.xml
	$(LN_S) $< $@

fix44.xml: $(top_srcdir)/spec/FIX.4.4.xml
	$(LN_S) $< $@

fix43.xml: $(top_srcdir)/spec/FIX.4.3.xml
	$(LN_S) $< $@

fix42.xml: $(top_srcdir)/spec/FIX.4.2.xml
	$(LN_S) $< $@

fix41.xml: $(top_srcdir)/spec/FIX.4.1.xml
	$(LN_S) $< $@

fix40.xml: $(top_srcdir)/spec/FIX.4.0.xml
	$(LN_S) $< $@

%-msg.h: fixml-msg.xsl %.xml
	xsltproc -o $@ $+

%-comp.h: fixml-comp.xsl %.xml
	xsltproc -o $@ $+


# 
# Help the developers get nice post-processed source files

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<
