#!/bin/bash

./dist-config.sh -- \
	--fpic \
	--gnu-source || exit $?

pushd precise > /dev/null || exit $?
./configure.sh && make $@
let RET=$?
popd > /dev/null
exit ${RET}

