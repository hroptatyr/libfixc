#!/bin/sh

tmpout=`mktemp`
"${top_builddir}/src/fixml2fix" "${srcdir}/fixml_ex_02.xml" > "${tmpout}"

diff "${srcdir}/fix_ex_02.fix" "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
