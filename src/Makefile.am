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
libfixc_la_SOURCES += fixml-comp-sub.xsl fixml-comp-sub.h
libfixc_la_SOURCES += fixml-comp-fld.xsl fixml-comp-fld.h
libfixc_la_SOURCES += fixml-fld-ctx.xsl fixml-fld-ctx.h
libfixc_la_SOURCES += fixml-nsuri.gperf fix-ver.gperf fix-nsuri.h
libfixc_la_SOURCES += libfixc_0_1_funs.xsl
libfixc_la_SOURCES += fixml-attr-by-ctx.xsl
libfixc_la_SOURCES += fixml-canon-attr.gperf fixml-canon-attr.h
libfixc_la_SOURCES += fixml-attr-by-ctx.c fixml-attr-by-ctx.h
libfixc_la_SOURCES += fixml-comp-by-ctx.xsl
libfixc_la_SOURCES += fixml-comp-by-ctx.c fixml-comp-by-ctx.h
libfixc_la_SOURCES += fixml-attr.xsl fixml-attr-rev.xsl
libfixc_la_SOURCES += fixml-comp.xsl fixml-comp-rev.xsl
libfixc_la_SOURCES += fixml-msg.xsl fixml-msg-rev.xsl
libfixc_la_CPPFLAGS = $(AM_CPPFLAGS)
libfixc_la_CPPFLAGS += $(expat_CFLAGS)
libfixc_la_LDFLAGS = $(AM_LDFLAGS)
libfixc_la_LDFLAGS += $(expat_LIBS)
BUILT_SOURCES += fixml-comp-sub.c $(fix_repo)-comp-sub.c
BUILT_SOURCES += fixml-comp-fld.c $(fix_repo)-comp-fld.c
BUILT_SOURCES += fixml-fld-ctx.c $(fix_repo)-fld-ctx.c
BUILT_SOURCES += fixml-nsuri.c
BUILT_SOURCES += fix-ver.c
BUILT_SOURCES += fixml-nsuri-rev.c
BUILT_SOURCES += $(fix_repo)-msg.gperf
BUILT_SOURCES += fixml-msg.c $(fix_repo)-msg.c
BUILT_SOURCES += fixml-msg.h $(fix_repo)-msg.h
BUILT_SOURCES += fixml-comp.h $(fix_repo)-comp.h
BUILT_SOURCES += fixml-attr.h $(fix_repo)-attr.h
BUILT_SOURCES += $(fix_repo)-attr-by-ctx.c
BUILT_SOURCES += fixml-canon-attr.c
BUILT_SOURCES += $(fix_repo)-comp-by-ctx.c
BUILT_SOURCES += fixml-attr-rev.c $(fix_repo)-attr-rev.c
BUILT_SOURCES += fixml-comp-rev.c $(fix_repo)-comp-rev.c
BUILT_SOURCES += fixml-msg-rev.c $(fix_repo)-msg-rev.c

header_HEADERS += fix.h
header_HEADERS += fix-nsuri.h
header_HEADERS += fixml-canon-comp.h
header_HEADERS += fixml-canon-msgt.h
header_HEADERS += fixml-canon-ctxt.h

## still needed for the test/ suite
EXTRA_DIST += fix-msg-type.h

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

include $(DEPDIR)/fixml-attr-by-ctx.Po
include $(DEPDIR)/fixml-comp-by-ctx.Po

fixml-attr-by-ctx.c: $(fix_repo)-attr-by-ctx.c
	$(LN_S) $< $@
$(fix_repo)-attr-by-ctx.c: fixml-attr-by-ctx.xsl \
		$(fix_repo).xml $(fix_repo)-attr-by-ctx.gperfs
	xsltproc -o $@ $< $(word 2,$+)

$(fix_repo)-attr-by-ctx.gperfs: fixml-attr-by-ctx.xsl $(fix_repo).xml
	xsltproc --stringparam MT $(DEPDIR)/fixml-attr-by-ctx.Po $+ > $@

fixml-comp-by-ctx.c: $(fix_repo)-comp-by-ctx.c
	$(LN_S) $< $@
$(fix_repo)-comp-by-ctx.c: fixml-comp-by-ctx.xsl \
		$(fix_repo).xml $(fix_repo)-comp-by-ctx.gperfs
	xsltproc -o $@ $< $(word 2,$+)

$(fix_repo)-comp-by-ctx.gperfs: fixml-comp-by-ctx.xsl $(fix_repo).xml
	xsltproc --stringparam MT $(DEPDIR)/fixml-comp-by-ctx.Po $+ > $@

dist-hook: $(fix_repo)-attr-by-ctx.gperfs $(fix_repo)-comp-by-ctx.gperfs
	for i in `cat $+`; do cp -a "`dirname $<`"/"$${i}" "$(distdir)/"; done
	cp -a $+ "$(distdir)/"

fixml-attr-rev.c: $(fix_repo)-attr-rev.c
	$(LN_S) $< $@
$(fix_repo)-attr-rev.c: fixml-attr-rev.xsl $(fix_repo).xml
	xsltproc -o $@ $+

fixml-comp-rev.c: $(fix_repo)-comp-rev.c
	$(LN_S) $< $@
$(fix_repo)-comp-rev.c: fixml-comp-rev.xsl $(fix_repo).xml
	xsltproc -o $@ $+

fixml-msg-rev.c: $(fix_repo)-msg-rev.c
	$(LN_S) $< $@
$(fix_repo)-msg-rev.c: fixml-msg-rev.xsl $(fix_repo).xml
	xsltproc -o $@ $+

fixml-comp-fld.c: $(fix_repo)-comp-fld.c
	$(LN_S) $< $@
$(fix_repo)-comp-fld.c: fixml-comp-fld.xsl $(fix_repo).xml
	xsltproc -o $@ $+

fixml-comp-sub.c: $(fix_repo)-comp-sub.c
	$(LN_S) $< $@
$(fix_repo)-comp-sub.c: fixml-comp-sub.xsl $(fix_repo).xml
	xsltproc -o $@ $+

fixml-fld-ctx.c: $(fix_repo)-fld-ctx.c
	$(LN_S) $< $@
$(fix_repo)-fld-ctx.c: fixml-fld-ctx.xsl $(fix_repo).xml
	xsltproc -o $@ $+

fixml-attr.h: $(fix_repo)-attr.h
	$(LN_S) $< $@
$(fix_repo)-attr.h: fixml-attr.xsl $(fix_repo).xml
	xsltproc -o $@ $+

fixml-msg.h: $(fix_repo)-msg.h
	$(LN_S) $< $@
fixml-msg.c: $(fix_repo)-msg.c
	$(LN_S) $< $@
$(fix_repo)-msg.h: fixml-msg.xsl $(fix_repo).xml
	xsltproc -o $@ $+

fixml-comp.h: $(fix_repo)-comp.h
	$(LN_S) $< $@
$(fix_repo)-comp.h: fixml-comp.xsl $(fix_repo).xml
	xsltproc -o $@ $+

$(fix_repo)-comp.gperf: fixml-comp.xsl $(fix_repo).xml
	xsltproc -o $@ --stringparam MT $(DEPDIR)/$*.Po $+

$(fix_repo)-msg.gperf: fixml-msg.xsl $(fix_repo).xml
	xsltproc -o $@ --stringparam MT $(DEPDIR)/$*.Po $+


## short cuts for stuff that's in the spec/ dir and named idiotically
fix50sp2.xml: $(top_srcdir)/spec/FIX.5.0SP2.xml
	$(LN_S) $< $@

fix50sp2ep98.xml: $(top_srcdir)/spec/FIX.5.0SP2.EP98.xml
	$(LN_S) $< $@

fix50sp2ep99.xml: $(top_srcdir)/spec/FIX.5.0SP2.EP99.xml
	$(LN_S) $< $@

fix50sp2ep100.xml: $(top_srcdir)/spec/FIX.5.0SP2.EP100.xml
	$(LN_S) $< $@

fix50sp2ep101.xml: $(top_srcdir)/spec/FIX.5.0SP2.EP101.xml
	$(LN_S) $< $@

fix50sp2ep102.xml: $(top_srcdir)/spec/FIX.5.0SP2.EP102.xml
	$(LN_S) $< $@

fix50sp2ep103.xml: $(top_srcdir)/spec/FIX.5.0SP2.EP103.xml
	$(LN_S) $< $@

fix50sp2ep104.xml: $(top_srcdir)/spec/FIX.5.0SP2.EP104.xml
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


# 
# Help the developers get nice post-processed source files

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<
