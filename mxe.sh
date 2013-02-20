#!/bin/bash

export LDFLAGS="-Wl,-subsystem,console"
export LIBS="-lgnurx -lpthread"

#export PKG_CONFIG_SYSROOT_DIR=;
#export PKG_CONFIG_LIBDIR=/$(MXE_TC)/lib/pkgconfig;

./dist-config.sh --target=mxe -- \
	--target=i686-pc-mingw32 \
	--prefix= \
	--tcroot=opt/csjp/mxe/i686-pc-mingw32 \
	--gnu-source \
	--win32 \
	--stlcompatible

export PATH=${PATH}:${PWD}/precise-x-mxe/opt/csjp/mxe/i686-pc-mingw32/lib
pushd precise-x-mxe > /dev/null || exit $?
make $1
let RET=$?
popd > /dev/null
exit ${RET}
