#!/bin/bash

#export LDFLAGS="-Wl,-subsystem,console"
#export LIBS="-lpthread"
#export LIBS="-lgnurx"

TCROOT=opt/mxe

export PKG_CONFIG_LIBDIR=${PKG_CONFIG_LIBDIR}:/${TCROOT}/lib
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:/${TCROOT}/lib/pkgconfig

./dist-config.sh --target=mxe \
	--exec-postfix=.exe \
	-- \
	--target=i686-pc-mingw32 \
	--prefix=butler0.1 \
	--gnu-source \
	--static \
	--ldflags=\"-static-libgcc -static-libstdc++\" \
	--ldflags=-Wl,-subsystem,console \
	--stlcompatible || exit $?

pushd precise-x-mxe > /dev/null || exit $?
./configure && make $@
let RET=$?
popd > /dev/null
exit ${RET}
