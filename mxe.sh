#!/bin/bash

#export LDFLAGS="-Wl,-subsystem,console"
#export LIBS="-lpthread"
#export LIBS="-lgnurx"

#export PKG_CONFIG_SYSROOT_DIR=;
#export PKG_CONFIG_LIBDIR=/$(MXE_TC)/lib/pkgconfig;
#--tcroot=opt/csjp/mxe/i686-pc-mingw32 \

./dist-config.sh --target=mxe -- \
	--target=i686-pc-mingw32 \
	--prefix=butler0.1-win32 \
	--gnu-source \
	--static -static-libgcc -static-libstdc++ \
	--ldflags="-Wl,-subsystem,console" \
	--stlcompatible || exit $?

pushd precise-x-mxe > /dev/null || exit $?
./configure && make $@
let RET=$?
popd > /dev/null
exit ${RET}
