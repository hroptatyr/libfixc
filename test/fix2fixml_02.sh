#!/bin/sh

tmpout=`mktemp`
"${top_builddir}/src/fix2fixml" -f fix50sp2 "${srcdir}/fix_ex_02.fix" | \
	sed -e 's/v="[.0-9A-Z ]*"//' | \
	xmllint --format - > "${tmpout}"

xmllint --format "${srcdir}/fixml_ex_02.xml" | diff - "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
