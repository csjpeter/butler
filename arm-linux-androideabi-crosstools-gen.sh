#!/bin/bash

if test "x$1" = "x"; then
	CROSSTOOLS_BIN=~/bin/crosstools
else
	CROSSTOOLS_BIN=$1
fi

if ! test -d ${CROSSTOOLS_BIN}; then
	mkdir -p ${CROSSTOOLS_BIN} || exit $?
fi

if test "x$QT_HOME" = "x"; then
	echo "You will need to have QT_HOME environment variable to contain the path to your favoured QT version."
	exit 1
fi

if test "x$ANDROID_NDK_HOME" = "x"; then
	echo "You will need to have ANDROID_NDK_HOME environment variable to contain the path to your favoured android ndk."
	exit 1
fi

test -d ${CROSSTOOLS_BIN} || mkdir ${CROSSTOOLS_BIN}

for f in $(ls -1 $ANDROID_NDK_HOME/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-${HOSTTYPE}/bin/arm-linux-androideabi-*); do
	b=$(basename $f)
	test -e ${CROSSTOOLS_BIN}/$b || {
		cat <<EOF > ${CROSSTOOLS_BIN}/$b
#!/bin/bash
\${ANDROID_NDK_HOME}/toolchains/arm-linux-androideabi-4.6/prebuilt/linux-${HOSTTYPE}/bin/$b \$@
exit \$?
EOF
		chmod u+x ${CROSSTOOLS_BIN}/$b
	}
done

for f in $(ls -1 $QT_HOME/5.4/android_armv5/bin/); do
	b=$(basename $f)
	test -e ${CROSSTOOLS_BIN}/arm-linux-androideabi-$b || {
		cat <<EOF > ${CROSSTOOLS_BIN}/arm-linux-androideabi-$b
#!/bin/bash
\${QT_HOME}/5.4/android_armv5/bin/$b \$@
exit \$?
EOF
		chmod u+x ${CROSSTOOLS_BIN}/arm-linux-androideabi-$b
	}
done

cat <<EOF > ${CROSSTOOLS_BIN}/arm-linux-androideabi-pkg-config
#!/bin/bash

export PKG_CONFIG_LIBDIR=\${PKG_CONFIG_LIBDIR}:\${QT_HOME}/5.4/android_armv5/lib
export PKG_CONFIG_PATH=\${PKG_CONFIG_PATH}:\${QT_HOME}/5.4/android_armv5/lib/pkgconfig
pkg-config \$@
exit \$?
EOF
chmod u+x ${CROSSTOOLS_BIN}/arm-linux-androideabi-pkg-config

