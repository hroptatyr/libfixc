#!/bin/sh

tmpout=`mktemp`
"${top_builddir}/src/fix2fixml" "${srcdir}/fix_ex_02.fix" | \
	xmllint --format - > "${tmpout}"

xmllint --format "${srcdir}/fixml_ex_02.xml" | diff - "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
