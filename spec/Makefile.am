## mostly code generators in here

BUILT_SOURCES =
CLEANFILES =
EXTRA_DIST = $(BUILT_SOURCES)
noinst_DATA =

EXTRA_DIST += uncamel.sed

EXTRA_DIST += README
EXTRA_DIST += gen-spec.xsl

## we lack the prereq's of these as we're not allowed to ship the official
## FIX repository, however if it's there, generate a folder <foo> to have
## the script below generate <foo>.xml
BUILT_SOURCES += FIX.4.0.xml
BUILT_SOURCES += FIX.4.1.xml
BUILT_SOURCES += FIX.4.2.xml
BUILT_SOURCES += FIX.4.3.xml
BUILT_SOURCES += FIX.4.4.xml
BUILT_SOURCES += FIX.5.0.xml
BUILT_SOURCES += FIX.5.0SP1.xml
BUILT_SOURCES += FIX.5.0SP2.xml
BUILT_SOURCES += FIXT.1.1.xml


%.xml: gen-spec.xsl \
		%/Base/MsgType.xml \
		%/Base/MsgContents.xml \
		%/Base/Components.xml \
		%/Base/Fields.xml \
		%/Base/Enums.xml \
		%/Base/Categories.xml
	xsltproc -o $@ \
		--param contents "document('`readlink -f "$(word 3,$+)"`')" \
		--param components "document('`readlink -f "$(word 4,$+)"`')" \
		--param fields "document('`readlink -f "$(word 5,$+)"`')" \
		--param enums "document('`readlink -f "$(word 6,$+)"`')" \
		--param categories "document('`readlink -f "$(word 7,$+)"`')" \
		$< $(word 2,$+)

## Makefile.am ends here
