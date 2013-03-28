### Makefile.am

AM_CPPFLAGS = -D_POSIX_C_SOURCE=200112L -D_XOPEN_SOURCE=600 -D_BSD_SOURCE
AM_LDFLAGS =
MOD_LDFLAGS = $(XCCLDFLAGS) -avoid-version -module

headerdir = $(pkgincludedir)

bin_PROGRAMS =
noinst_PROGRAMS =
lib_LTLIBRARIES =
pkglib_LTLIBRARIES =
noinst_LTLIBRARIES =
noinst_HEADERS =
header_HEADERS =
BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST = $(BUILT_SOURCES)

lib_LTLIBRARIES += libfixc.la
libfixc_la_SOURCES = fix.c fix.h
libfixc_la_SOURCES += fix-private.h
libfixc_la_SOURCES += fixml-wr.c
libfixc_la_SOURCES += fixml-rd.c
libfixc_la_SOURCES += nifty.h
libfixc_la_SOURCES += engdso.c engdso.h
libfixc_la_CPPFLAGS = $(AM_CPPFLAGS) -D_GNU_SOURCE
libfixc_la_CPPFLAGS += -DPKGLIBDIR="\"$(pkglibdir)\""
libfixc_la_CPPFLAGS += $(expat_CFLAGS)
libfixc_la_CPPFLAGS += $(zlib_CFLAGS)
libfixc_la_CPPFLAGS += $(INCLTDL)
libfixc_la_LDFLAGS = $(AM_LDFLAGS)
libfixc_la_LDFLAGS += $(expat_LIBS)
libfixc_la_LDFLAGS += $(zlib_LIBS)
libfixc_la_LDFLAGS += $(LIBLTDL)
BUILT_SOURCES += fixml-nsuri.c
BUILT_SOURCES += fix-ver.c
BUILT_SOURCES += fixml-nsuri-rev.c
BUILT_SOURCES += fixml-canon-attr.c

header_HEADERS += fix.h
header_HEADERS += fix-nsuri.h
header_HEADERS += fixml-canon-attr.h
header_HEADERS += fixml-canon-comp.h
header_HEADERS += fixml-canon-msgt.h
header_HEADERS += fixml-canon-ctxt.h
header_HEADERS += engdso.h

## still needed for the test/ suite
EXTRA_DIST += fix-msg-type.h

## just for debugging and fun
bin_PROGRAMS += fixml2fix
fixml2fix_CPPFLAGS = $(AM_CPPFLAGS)
fixml2fix_CPPFLAGS += -DSTANDALONE
fixml2fix_LDFLAGS = $(AM_LDFLAGS)
fixml2fix_LDFLAGS += -static
fixml2fix_LDADD = libfixc.la
fixml2fix_LDADD += fix50sp2.la

bin_PROGRAMS += fix2fixml
fix2fixml_CPPFLAGS = $(AM_CPPFLAGS)
fix2fixml_CPPFLAGS += -DSTANDALONE
fix2fixml_LDFLAGS = $(AM_LDFLAGS)
fix2fixml_LDFLAGS += -static
fix2fixml_LDADD = libfixc.la
fix2fixml_LDADD += fix50sp2.la

## prebuilt engines
pkglib_LTLIBRARIES += fix50sp2.la
fix50sp2_la_SOURCES = fix50sp2.xml
fix50sp2_la_SOURCES += fix-private.h
fix50sp2_la_SOURCES += nifty.h
fix50sp2_la_SOURCES += fixml-comp-sub.xsl fixml-comp-sub.h
fix50sp2_la_SOURCES += fixml-comp-fld.xsl fixml-comp-fld.h
fix50sp2_la_SOURCES += fixml-fld-ctx.xsl fixml-fld-ctx.h
fix50sp2_la_SOURCES += fixml-nsuri.gperf fix-ver.gperf fix-nsuri.h
fix50sp2_la_SOURCES += libfixc_0_1_funs.xsl
fix50sp2_la_SOURCES += fixml-canon-attr.gperf fixml-canon-attr.h
fix50sp2_la_SOURCES += fixml-attr-by-ctx.xsl fixml-attr-by-ctx.h
fix50sp2_la_SOURCES += fixml-attr-by-ctx-fix50sp2.c
fix50sp2_la_SOURCES += fixml-comp-by-ctx.xsl fixml-comp-by-ctx.h
fix50sp2_la_SOURCES += fixml-comp-by-ctx-fix50sp2.c
fix50sp2_la_SOURCES += fixml-attr.xsl fixml-attr-rev.xsl
fix50sp2_la_SOURCES += fixml-comp.xsl fixml-comp-rev.xsl
fix50sp2_la_SOURCES += fixml-msg.xsl fixml-msg.h
fix50sp2_la_SOURCES += fixml-msg-fix50sp2.c
fix50sp2_la_SOURCES += fixml-msg-rev.xsl
fix50sp2_la_SOURCES += fixml-comp-rptb.xsl fixml-comp-rptb.h
fix50sp2_la_SOURCES += fixml-comp-rptb-fix50sp2.c
fix50sp2_la_SOURCES += fixml-comp-orb.xsl fixml-comp-orb.h
fix50sp2_la_SOURCES += fixml-comp-orb-fix50sp2.c
fix50sp2_la_CPPFLAGS = $(AM_CPPFLAGS) -D_GNU_SOURCE
fix50sp2_la_LDFLAGS = $(MOD_LDFLAGS)
BUILT_SOURCES += fixml-comp-sub-fix50sp2.c
BUILT_SOURCES += fixml-comp-fld-fix50sp2.c
BUILT_SOURCES += fixml-fld-ctx-fix50sp2.c
BUILT_SOURCES += fixml-msg-fix50sp2.gperf
BUILT_SOURCES += fixml-msg-fix50sp2.c fixml-msg-fix50sp2.h
BUILT_SOURCES += fixml-comp-fix50sp2.gperf
BUILT_SOURCES += fixml-comp-fix50sp2.h
BUILT_SOURCES += fixml-attr-fix50sp2.h
BUILT_SOURCES += fixml-attr-by-ctx-fix50sp2.c
BUILT_SOURCES += fixml-comp-by-ctx-fix50sp2.c
BUILT_SOURCES += fixml-attr-rev-fix50sp2.c
BUILT_SOURCES += fixml-comp-rev-fix50sp2.c
BUILT_SOURCES += fixml-msg-rev-fix50sp2.c
BUILT_SOURCES += fixml-comp-rptb-fix50sp2.c
BUILT_SOURCES += fixml-comp-orb-fix50sp2.c

header_HEADERS += fixml-attr-fix50sp2.h
header_HEADERS += fixml-comp-fix50sp2.h
header_HEADERS += fixml-msg-fix50sp2.h

-include $(DEPDIR)/fixml-attr-by-ctx-fix50sp2.Po
-include $(DEPDIR)/fixml-comp-by-ctx-fix50sp2.Po

## just like fix50sp2 but with some CME EPs
pkglib_LTLIBRARIES += fix50sp2_cme.la
fix50sp2_cme_la_SOURCES = fix50sp2_cme.xml
fix50sp2_cme_la_SOURCES += fix-private.h
fix50sp2_cme_la_SOURCES += nifty.h
fix50sp2_cme_la_SOURCES += fixml-comp-sub.xsl fixml-comp-sub.h
fix50sp2_cme_la_SOURCES += fixml-comp-fld.xsl fixml-comp-fld.h
fix50sp2_cme_la_SOURCES += fixml-fld-ctx.xsl fixml-fld-ctx.h
fix50sp2_cme_la_SOURCES += fixml-nsuri.gperf fix-ver.gperf fix-nsuri.h
fix50sp2_cme_la_SOURCES += libfixc_0_1_funs.xsl
fix50sp2_cme_la_SOURCES += fixml-canon-attr.gperf fixml-canon-attr.h
fix50sp2_cme_la_SOURCES += fixml-attr-by-ctx.xsl fixml-attr-by-ctx.h
fix50sp2_cme_la_SOURCES += fixml-attr-by-ctx-fix50sp2_cme.c
fix50sp2_cme_la_SOURCES += fixml-comp-by-ctx.xsl fixml-comp-by-ctx.h
fix50sp2_cme_la_SOURCES += fixml-comp-by-ctx-fix50sp2_cme.c
fix50sp2_cme_la_SOURCES += fixml-attr.xsl fixml-attr-rev.xsl
fix50sp2_cme_la_SOURCES += fixml-comp.xsl fixml-comp-rev.xsl
fix50sp2_cme_la_SOURCES += fixml-msg.xsl fixml-msg.h
fix50sp2_cme_la_SOURCES += fixml-msg-fix50sp2_cme.c
fix50sp2_cme_la_SOURCES += fixml-msg-rev.xsl
fix50sp2_cme_la_SOURCES += fixml-comp-rptb.xsl fixml-comp-rptb.h
fix50sp2_cme_la_SOURCES += fixml-comp-rptb-fix50sp2_cme.c
fix50sp2_cme_la_SOURCES += fixml-comp-orb.xsl fixml-comp-orb.h
fix50sp2_cme_la_SOURCES += fixml-comp-orb-fix50sp2_cme.c
fix50sp2_cme_la_CPPFLAGS = $(AM_CPPFLAGS) -D_GNU_SOURCE
fix50sp2_cme_la_LDFLAGS = $(MOD_LDFLAGS)
BUILT_SOURCES += fixml-comp-sub-fix50sp2_cme.c
BUILT_SOURCES += fixml-comp-fld-fix50sp2_cme.c
BUILT_SOURCES += fixml-fld-ctx-fix50sp2_cme.c
BUILT_SOURCES += fixml-msg-fix50sp2_cme.gperf
BUILT_SOURCES += fixml-msg-fix50sp2_cme.c fixml-msg-fix50sp2_cme.h
BUILT_SOURCES += fixml-comp-fix50sp2_cme.gperf
BUILT_SOURCES += fixml-comp-fix50sp2_cme.h
BUILT_SOURCES += fixml-attr-fix50sp2_cme.h
BUILT_SOURCES += fixml-attr-by-ctx-fix50sp2_cme.c
BUILT_SOURCES += fixml-comp-by-ctx-fix50sp2_cme.c
BUILT_SOURCES += fixml-attr-rev-fix50sp2_cme.c
BUILT_SOURCES += fixml-comp-rev-fix50sp2_cme.c
BUILT_SOURCES += fixml-msg-rev-fix50sp2_cme.c
BUILT_SOURCES += fixml-comp-rptb-fix50sp2_cme.c
BUILT_SOURCES += fixml-comp-orb-fix50sp2_cme.c

header_HEADERS += fixml-attr-fix50sp2_cme.h
header_HEADERS += fixml-comp-fix50sp2_cme.h
header_HEADERS += fixml-msg-fix50sp2_cme.h

-include $(DEPDIR)/fixml-attr-by-ctx-fix50sp2_cme.Po
-include $(DEPDIR)/fixml-comp-by-ctx-fix50sp2_cme.Po


## our rule for gperf
%.c: %.gperf
	$(AM_V_GEN) $(GPERF) -L ANSI-C $< --output-file $@

## rework me!
dist-hook: fixml-attr-by-ctx.gperfs fixml-comp-by-ctx.gperfs \
		fixml-attr-by-ctx.c fixml-comp-by-ctx.c
	for i in `cat $(wordlist 1,2,$+)`; do \
		test -f "$(distdir)/$${i}" || \
			cp -a "`dirname $<`"/"$${i}" "$(distdir)/"; \
	done
	cp -a $(wordlist 1,2,$+) "$(distdir)/"


## short cuts for stuff that's in the spec/ dir and named idiotically
fix50sp2.xml: $(top_srcdir)/spec/FIX.5.0SP2.xml
	$(LN_S) -f $< $@

fix50sp1.xml: $(top_srcdir)/spec/FIX.5.0SP1.xml
	$(LN_S) -f $< $@

fix50.xml: $(top_srcdir)/spec/FIX.5.0.xml
	$(LN_S) -f $< $@

fix44.xml: $(top_srcdir)/spec/FIX.4.4.xml
	$(LN_S) -f $< $@

fix43.xml: $(top_srcdir)/spec/FIX.4.3.xml
	$(LN_S) -f $< $@

fix42.xml: $(top_srcdir)/spec/FIX.4.2.xml
	$(LN_S) -f $< $@

fix41.xml: $(top_srcdir)/spec/FIX.4.1.xml
	$(LN_S) -f $< $@

fix40.xml: $(top_srcdir)/spec/FIX.4.0.xml
	$(LN_S) -f $< $@

fixml-comp-by-ctx-%.c: fixml-comp-by-ctx.xsl %.xml fixml-comp-by-ctx-%.gperfs
	$(AM_V_GEN) xsltproc -o $@ $< $(word 2,$+)

fixml-comp-by-ctx-%.gperfs: fixml-comp-by-ctx.xsl %.xml
	$(AM_V_GEN) xsltproc --stringparam MT $(DEPDIR)/fixml-comp-by-ctx-$*.Po $+ > $@

fixml-attr-by-ctx-%.c: fixml-attr-by-ctx.xsl %.xml fixml-attr-by-ctx-%.gperfs
	$(AM_V_GEN) xsltproc -o $@ $< $(word 2,$+)

fixml-attr-by-ctx-%.gperfs: fixml-attr-by-ctx.xsl %.xml
	$(AM_V_GEN) xsltproc --stringparam MT $(DEPDIR)/fixml-attr-by-ctx-$*.Po $+ > $@

fixml-comp-rptb-%.c: fixml-comp-rptb.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-attr-rev-%.c: fixml-attr-rev.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-comp-rev-%.c: fixml-comp-rev.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-msg-rev-%.c: fixml-msg-rev.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-comp-fld-%.c: fixml-comp-fld.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-comp-sub-%.c: fixml-comp-sub.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-fld-ctx-%.c: fixml-fld-ctx.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-attr-%.h: fixml-attr.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-msg-%.h: fixml-msg.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-comp-%.h: fixml-comp.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-comp-%.gperf: fixml-comp.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ --stringparam MT $(DEPDIR)/fixml-comp-$*.Po $+

fixml-msg-%.gperf: fixml-msg.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ --stringparam MT $(DEPDIR)/fixml-msg-$*.Po $+

fixml-comp-rptb-%.c: fixml-comp-rptb.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

fixml-comp-orb-%.c: fixml-comp-orb.xsl %.xml
	$(AM_V_GEN) xsltproc -o $@ $+

# 
# Help the developers get nice post-processed source files

## Create preprocessor output (debugging purposes only)
.c.i:
	$(COMPILE) -E -o $@ $<

## Create assembler output (debugging purposes only)
.c.s:
	$(COMPILE) -S -c $(AM_CFLAGS) $<
