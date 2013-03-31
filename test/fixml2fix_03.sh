#!/bin/sh

tmpout=`mktemp`
"${top_builddir}/src/fixml2fix" -f fix50sp2 -x "${srcdir}/fixml_ex_06.xml" | \
	xmllint --format - > "${tmpout}"

diff "${srcdir}/fixml_ex_06.xml" "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
