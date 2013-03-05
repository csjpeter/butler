#!/bin/bash

./dist-config.sh -- \
	--pkg-config-path=/opt/csjp/lib/pkgconfig \
	--prefix=opt/butler \
	--fpic \
	--gnu-source || exit $?

pushd precise > /dev/null || exit $?
./configure && make $@
let RET=$?
popd > /dev/null
exit ${RET}

