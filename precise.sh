#!/bin/bash

./dist-config.sh -- \
	--pkg-config-path=/opt/csjp/lib/pkgconfig \
	--prefix=opt/csjp \
	--fpic \
	--gnu-source || exit $?

pushd precise > /dev/null || exit $?
./configure.sh && make $@
let RET=$?
popd > /dev/null
exit ${RET}

