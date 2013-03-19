#!/bin/bash

source /etc/lsb-release

./dist-config.sh \
	--host=${DISTRIB_CODENAME} \
	--target=${DISTRIB_CODENAME} \
	-- \
	--debug \
	--pkg-config-path=/opt/extras.ubuntu.com/csjp \
	--prefix=opt/butler0.1 \
	--appsdir=usr/share/applications \
	--fpic \
	--gnu-source || exit $?

pushd ${DISTRIB_CODENAME} > /dev/null || exit $?
./configure && make $@
let RET=$?
popd > /dev/null
test ${RET} -eq 0 || exit ${RET}

