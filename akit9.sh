#!/bin/bash

TCROOT=opt/csjp/android-9-arm/arm-linux-androideabi

#	--cflags=\"-flto -fwhole-program\" \
#	--cflags=\"--sysroot=/${TCROOT}/sysroot\" \
#	--ldflags=\"--sysroot=/${TCROOT}/sysroot\" \

./dist-config.sh --target=akit9 -- \
	--target=akit9 \
	--prefix= \
	--tcroot=${TCROOT} \
	--fpic \
	--non-gnu-source \
	--cflags=-Wno-psabi \
	--cflags=\"-D__STDC_INT64__ -DSTDC_INT64 -DSTDC_VERSION -DSTRICT_ANSI\" \
	--cflags=\"-I/opt/csjp/android-9-arm/sysroot/usr/include\" \
	--ldflags=\"-L usr/lib\" \
	--static || exit $?

pushd precise-x-akit9 > /dev/null || exit $?
./configure && make $1
let RET=$?
popd > /dev/null
exit ${RET}

AKIT9_NDK_HOME=/opt/csjp/android-ndk-r8
AKIT9_SDK_HOME=/opt/csjp/android-sdk-linux

APK_BUILDER=${AKIT9_SDK_HOME}/tools/apkbuilder
AKIT9_APT=${AKIT9_SDK_HOME}/platform-tools/aapt
AKIT9_DEX=${AKIT9_SDK_HOME}/platform-tools/dx
AKIT9_JARSIGNER=$(which jarsigner 2> /dev/null)

# am start -a android.intent.action.MAIN -n csjpeter.butler/org.kde.necessitas.origo.QtActivity

