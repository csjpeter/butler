#!/bin/bash

#export LDFLAGS="-Wl,-subsystem,console"
#export LIBS="-lpthread"
#export LIBS="-lgnurx"

source /etc/lsb-release

function exec_in_dir ()
{
	pushd $1 > /dev/null || exit $?
	shift
	$@
	let RET=$?
	popd > /dev/null
	return ${RET}
}

TCROOT=opt/mxe

export PKG_CONFIG_LIBDIR=${PKG_CONFIG_LIBDIR}:/${TCROOT}/lib
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:/${TCROOT}/lib/pkgconfig

./dist-config.sh \
	--target=mxe \
	--exec-postfix=.exe \
	-- \
	--relative-path \
	--target=i686-pc-mingw32 \
	--prefix=butler0.1 \
	--gnu-source \
	--static \
	--ldflags=\"-static-libgcc -static-libstdc++\" \
	--ldflags=-Wl,-subsystem,windows \
	--stlcompatible || exit $?

#--ldflags=-Wl,-subsystem,console

exec_in_dir ${DISTRIB_CODENAME}-x-mxe ./configure || exit $?
exec_in_dir ${DISTRIB_CODENAME}-x-mxe make $@ || exit $?

exec_in_dir ${DISTRIB_CODENAME}-x-mxe unix2dos nsis/license.txt || exit $?
exec_in_dir ${DISTRIB_CODENAME}-x-mxe makensis nsis/@PKGNAME@.nsi

