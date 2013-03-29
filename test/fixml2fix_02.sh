#!/bin/sh

tmpout=`mktemp`
"${top_builddir}/src/fixml2fix" -f fix50sp2 "${srcdir}/fixml_ex_02.xml" > "${tmpout}"

diff "${srcdir}/fix_ex_02.fix" "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
