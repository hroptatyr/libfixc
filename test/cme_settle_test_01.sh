#!/bin/sh

tmpout=`mktemp`
"./cme-settle" "${srcdir}/cbt.settle.20121105.s.xml" > "${tmpout}"

diff "${srcdir}/cbt.settle.20121105.s.syms" "${tmpout}"
rc=${?}

rm -f -- "${tmpout}"
exit ${rc}
