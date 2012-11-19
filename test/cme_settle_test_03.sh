#!/bin/sh

tmpout=`mktemp`
"./cme-settle" --rinse "${srcdir}/cbt.settle.20121105.s.xml" > "${tmpout}"

diff "${srcdir}/cbt.settle.20121105.s.rinse" "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
