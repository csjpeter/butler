#!/bin/bash

./dist-config.sh -- \
	--pkg-config-path=/opt/ppa/lib/pkgconfig \
	--prefix=opt/ppa \
	--fpic \
	--gnu-source || exit $?

pushd precise > /dev/null || exit $?
./configure.sh && make $@
let RET=$?
popd > /dev/null
exit ${RET}

