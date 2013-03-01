#!/bin/bash

#export LDFLAGS="-Wl,-subsystem,console"
#export LIBS="-lpthread"
#export LIBS="-lgnurx"

#export PKG_CONFIG_SYSROOT_DIR=;
#export PKG_CONFIG_LIBDIR=/$(MXE_TC)/lib/pkgconfig;

./dist-config.sh --target=mxe -- \
	--target=i686-pc-mingw32 \
	--prefix= \
	--tcroot=opt/csjp/mxe/i686-pc-mingw32 \
	--gnu-source \
	--win32 \
	--static \
	--stlcompatible || exit $?

pushd precise-x-mxe > /dev/null || exit $?
./configure.sh && make $@
let RET=$?
popd > /dev/null
exit ${RET}
