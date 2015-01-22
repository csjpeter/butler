#!/bin/bash

JOBS=$(expr $(cat /proc/cpuinfo | grep processor | wc -l) + 1)
 
source /etc/lsb-release
source config

function exec_in_dir ()
{
	pushd $1 > /dev/null || exit $?
	shift
	$@
	let RET=$?
	popd > /dev/null
	return ${RET}
}

if test "x$ANDROID_NDK_HOME" = "x" -o "x$ANDROID_SDK_HOME" = "x"; then
	echo "You will need to have ANDROID_NDK_HOME and ANDROID_SDK_HOME"
	echo "environment variables to contain the path to your favoured"
	echo "android ndk."
	exit 1
fi

#--cflags=\"-flto -fwhole-program\"

function config ()
{
	local API=$1
	local ARCH=$2
	local DIST=$3
	shift 3
	TCROOT=opt/${API}-${ARCH}
	export PKG_CONFIG_LIBDIR=${PKG_CONFIG_LIBDIR}:/${TCROOT}/lib
	export PKG_CONFIG_PATH=${PKG_CONFIG_PATH}:/${TCROOT}/lib/pkgconfig
	./dist-config.sh \
		$@ \
		-- \
		--debug \
		--target=arm-linux-androideabi \
		--prefix= \
		--translations-prefix=assets:/ \
		--css-prefix=assets:/ \
		--icons-prefix=assets:/ \
		--tcroot=${TCROOT} \
		--fpic \
		--non-gnu-source \
		--cflags=-Wno-psabi \
		--cflags=\\\"-DQ_OS_ANDROID\\\" \
		--cflags=\\\"-D__STDC_INT64__ -DSTDC_INT64 -DSTDC_VERSION -DSTRICT_ANSI\\\" \
		--cflags=\\\"-I${ANDROID_NDK_HOME}/platforms/${API}/arch-${ARCH}/usr/include\\\" \
		--cflags=\\\"-I${ANDROID_NDK_HOME}/sources/cxx-stl/gnu-libstdc++/4.6/include\\\" \
		--cflags=\\\"-I${ANDROID_NDK_HOME}/sources/cxx-stl/gnu-libstdc++/4.6/include/i686-linux-android\\\" \
		--cflags=\\\"-I${ANDROID_NDK_HOME}/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/include\\\" \
		--ldflags=\\\"--sysroot ${ANDROID_NDK_HOME}/platforms/${API}/arch-${ARCH}\\\" \
		--libs=\\\"${ANDROID_NDK_HOME}/sources/cxx-stl/gnu-libstdc++/4.6/libs/armeabi-v7a/libsupc++.a\\\" \
		--libs=\\\"-llog\\\" \
		--libs=\\\"/${TCROOT}/lib/libcsjp0.3.a\\\" \
		|| exit $?

	exec_in_dir ${DIST} ./configure || exit $?
}

export ANDROID_SDK_API=android-14
ARCH=arm

CMD=$1

case "${CMD}" in
	(debian)
		shift
		test "x$1" = "x" || { export ANDROID_SDK_API=$1 ; shift ; }
		test "x$1" = "x" || { ARCH=$1 ; shift ; }
		DIST=${DISTRIB_CODENAME}-x-${ANDROID_SDK_API}-${ARCH}

		config ${ANDROID_SDK_API} ${ARCH} ${DIST} \
			--target=${ANDROID_SDK_API}-${ARCH} \
			--packaging=debian || exit $?
		exec_in_dir ${DIST} debuild \
			--no-tgz-check \
			--preserve-envvar PATH \
			--preserve-envvar QT_HOME \
			--preserve-envvar ANDROID_NDK_HOME \
			--preserve-envvar ANDROID_SDK_HOME \
			--preserve-envvar PKG_CONFIG_LIBDIR \
			--preserve-envvar PKG_CONFIG_PATH || exit $?
	;;
	(apk)
		shift
		test "x$1" = "x" || { export ANDROID_SDK_API=$1 ; shift ; }
		test "x$1" = "x" || { ARCH=$1 ; shift ; }
		DIST=${ANDROID_SDK_API}-${ARCH}

		#rm -fr ${DIST}/android # force cleanup ant files
		config ${ANDROID_SDK_API} ${ARCH} ${DIST} \
			--host=${ANDROID_SDK_API}-${ARCH} \
			--target=${ANDROID_SDK_API}-${ARCH} \
			--packaging=android || exit $?
		exec_in_dir ${DIST} make -j${JOBS} $@ || exit $?

		APKGNAME=${PRJNAME}-${VERSION}_${ANDROID_SDK_API}-${ARCH}
		for f in $(echo "libcrypto.so libpq.so libqsqlpsql.so libssl.so"); do
			cp /opt/${ANDROID_SDK_API}-${ARCH}/lib/$f \
				${DIST}/android/libs/armeabi-v7a/ || exit $?
		done
		test -h ${DIST}/${APKGNAME} || exec_in_dir \
				${DIST} ln -s android ${APKGNAME} || exit $?
		exec_in_dir ${DIST} tar -chzf ${APKGNAME}.tgz ${APKGNAME} || exit $?

		exec_in_dir ${DIST}/android ant debug || exit $?
	;;
	(install-apk)
		DIST=${ANDROID_SDK_API}-${ARCH}
		adb -d install -r ${DIST}/android/bin/${PKGNAME_BASE}-debug.apk
	;;
	(*)
		test "x$1" = "x" || { export ANDROID_SDK_API=$1 ; shift ; }
		test "x$1" = "x" || { ARCH=$1 ; shift ; }
		DIST=${ANDROID_SDK_API}-${ARCH}

		config ${ANDROID_SDK_API} ${ARCH} ${DIST} \
			--host=${ANDROID_SDK_API}-${ARCH} \
			--target=${ANDROID_SDK_API}-${ARCH} \
			--packaging=android || exit $?
		exec_in_dir ${DIST} make $@ || exit $?
	;;
esac

# Android examples:
# http://developer.android.com/resources/browser.html?tag=sample

# Building a sample project:
# 1 cd to sample directory
# 2 /opt/csjp/android-ndk-r8/ndk-build
# 3/a /opt/csjp/android-sdk-linux/tools/android update project --target 1 --path ./
# 3/b /opt/csjp/android-sdk-linux/tools/android update project -p . -s
# 4 ant debug

# Might be interesting later:
# http://groomws.info/index.php?title=AndroidNativeBinaries
# http://developer.nvidia.com/tegra-start

# Gentoo cross development
# http://www.gentoo.org/proj/en/base/embedded/handbook/

# First, create android-9 toolchains:
# /opt/csjp/android-ndk-r8/build/tools/make-standalone-toolchain.sh --arch=x86 --platform=android-9 --install-dir=/opt/csjp/android-9-x86
# /opt/csjp/android-ndk-r8/build/tools/make-standalone-toolchain.sh --arch=arm --platform=android-9 --install-dir=/opt/csjp/android-9-arm
#

# Build process:
# http://developer.android.com/guide/developing/building/index.html#detailed-build
#
# Signing:
# http://developer.android.com/guide/publishing/app-signing.html
#
# Generate key to sign with
# keytool -genkey -keystore release.keystore -alias release -keyalg RSA -keysize 2048 -validity 10000
#
# Sign the package
# jarsigner -verbose -sigalg MD5withRSA -digestalg SHA1 -keystore release.keystore file.apk alias_name
#
# Verify a package
# jarsigner -verify -verbose -certs my_application.apk
#
# Align to 4 bytes boundaries so that android can mmap the file for faster further operations.
# zipalign -v 4 your_project_name-unaligned.apk your_project_name.apk

# May be for a native application
# http://developer.android.com/reference/android/app/NativeActivity.html

# Use this below command to start adb daemon:
# sudo adb start-server

# Use this below command to (re)install to the android device connected to you computer:
# adb -d install -r $(ANDROID_PACKAGES)


# adb logcat *:W libbutler0.1.so:V SurfaceFlinger:S
