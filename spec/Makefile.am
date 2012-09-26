## mostly code generators in here

BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST = $(BUILT_SOURCES)
noinst_DATA =

EXTRA_DIST += fixml_attr.44
EXTRA_DIST += fixml_attr.50
EXTRA_DIST += fixml_attr.50SP1
EXTRA_DIST += fixml_attr.50SP2

EXTRA_DIST += fixml_comp.44
EXTRA_DIST += fixml_comp.50
EXTRA_DIST += fixml_comp.50SP1
EXTRA_DIST += fixml_comp.50SP2

EXTRA_DIST += uncamel.sed

BUILT_SOURCES += fixml_attr.all
noinst_DATA += fixml_attr.all

BUILT_SOURCES += fixml_attr.all
noinst_DATA += fixml_literal.all

fixml_attr.all: fixml_attr.44 fixml_attr.50 fixml_attr.50SP1 fixml_attr.50SP2
	cat $? | sort -k1n -k2 | uniq > $@

fixml_literal.all: fixml_attr.all uncamel.sed
	cat $< | \
	sed -f $(word 2,$+) -e 's/@/FIXC_ATTR/' | \
	tr 'a-z' 'A-Z' > $@

## Makefile.am ends here
