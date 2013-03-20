#!/bin/bash
 
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

TCROOT=opt/android-9-arm

ANDROID_NDK_HOME=~/necessitas/android-ndk
ANDROID_SDK_HOME=~/necessitas/android-sdk

#--cflags=\"-flto -fwhole-program\"

export PKG_CONFIG_LIBDIR=${PKG_CONFIG_LIBDIR}:/${TCROOT}/lib
export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:/${TCROOT}/lib/pkgconfig

./dist-config.sh \
	--target=android-9-arm \
	-- \
	--target=arm-linux-androideabi \
	--prefix= \
	--tcroot=${TCROOT} \
	--fpic \
	--non-gnu-source \
	--cflags=-Wno-psabi \
	--cflags=\"-D__STDC_INT64__ -DSTDC_INT64 -DSTDC_VERSION -DSTRICT_ANSI\" \
	--cflags=\"-I${ANDROID_NDK_HOME}/platforms/android-9/arch-arm/usr/include\" \
	--cflags=\"-I${ANDROID_NDK_HOME}/sources/cxx-stl/gnu-libstdc++/4.4.3/include\" \
	--cflags=\"-I${ANDROID_NDK_HOME}/sources/cxx-stl/gnu-libstdc++/4.4.3/include/i686-linux-android\" \
	--ldflags=\"--sysroot ${ANDROID_NDK_HOME}/platforms/android-9/arch-arm\" \
	--libs=\"${ANDROID_NDK_HOME}/sources/cxx-stl/gnu-libstdc++/4.4.3/libs/armeabi/libsupc++.a\" \
	|| exit $?

exec_in_dir ${DISTRIB_CODENAME}-x-android-9-arm ./configure || exit $?
exec_in_dir ${DISTRIB_CODENAME}-x-android-9-arm make $@ || exit $?

APK_BUILDER=${ANDROID_SDK_HOME}/tools/apkbuilder
AAPT=${ANDROID_SDK_HOME}/platform-tools/aapt
DEX=${ANDROID_SDK_HOME}/platform-tools/dx
JARSIGNER=$(which jarsigner 2> /dev/null)

