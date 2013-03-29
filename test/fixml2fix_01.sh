#!/bin/sh

tmpout=`mktemp`
"${top_builddir}/src/fixml2fix" -f fix50sp2 "${srcdir}/fixml_ex_01.xml" > "${tmpout}"

diff "${srcdir}/fix_ex_01.fix" "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
