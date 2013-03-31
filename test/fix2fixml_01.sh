#!/bin/sh

tmpout=`mktemp`
"${top_builddir}/src/fix2fixml" -f fix50sp2 "${srcdir}/fix_ex_01.fix" | \
	xmllint --format - > "${tmpout}"

xmllint --format "${srcdir}/fixml_ex_01.xml" | diff - "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
