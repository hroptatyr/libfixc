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

BUILT_SOURCES += fixml_comp.all
noinst_DATA += fixml_comp.all

BUILT_SOURCES += fixml_literal.all
noinst_DATA += fixml_literal.all


EXTRA_DIST += README
EXTRA_DIST += FIX40.xml
EXTRA_DIST += FIX41.xml
EXTRA_DIST += FIX42.xml
EXTRA_DIST += FIX44.xml
EXTRA_DIST += FIX50.xml
EXTRA_DIST += FIX50SP1.xml
EXTRA_DIST += FIX50SP2.xml
EXTRA_DIST += FIXT11.xml


## some rules to auto-gen some code
fixml_attr.all: fixml_attr.50SP2 fixml_attr.50SP1 fixml_attr.50 fixml_attr.44
	cat $+ | sort -k1n -k2 | uniq > $@

fixml_comp.all: fixml_comp.50SP2 fixml_comp.50SP1 fixml_comp.50 fixml_comp.44
	cat $+ | sort -k1 -k2 | uniq > $@

fixml_literal.all: fixml_attr.all uncamel.sed
	cat $< | \
	sed -f $(word 2,$+) -e 's/@/FIXC_ATTR/' | \
	tr 'a-z' 'A-Z' > $@

## Makefile.am ends here
