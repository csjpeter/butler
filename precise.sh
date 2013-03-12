#!/bin/bash

./dist-config.sh -- \
	--pkg-config-path=/opt/csjp/lib/pkgconfig \
	--prefix=opt/butler0.1 \
	--appsdir=usr/share/applications \
	--fpic \
	--gnu-source || exit $?

pushd precise > /dev/null || exit $?
./configure && make $@
let RET=$?
popd > /dev/null
exit ${RET}

