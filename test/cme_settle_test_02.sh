#!/bin/sh

tmpout=`mktemp`
"./cme-settle" --meta "${srcdir}/cbt.settle.20121105.s.xml" > "${tmpout}"

diff "${srcdir}/cbt.settle.20121105.s.meta" "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
