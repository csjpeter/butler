#!/bin/bash

source config

source /etc/lsb-release

BUILD_DIST=${DISTRIB_CODENAME}
HOST_DIST=${DISTRIB_CODENAME}
TARGET_DIST=${DISTRIB_CODENAME}
DEBIAN_ARCH=$(dpkg-architecture -qDEB_BUILD_ARCH)
CURRENT_DATE=$(LANG=en date +"%a, %d %b %Y %H:%M:%S %z")
CURRENT_YEAR=$(LANG=en date +"%Y")

while ! test "x$1" = "x"; do
	! test "x$1" = "x--" || {
		shift
		break
	}

	KEY=$(echo $1 | sed 's|=.*||')
	VALUE=$(echo $1 | sed 's|.*=||')
	case ${KEY} in
		(--build)
			! test "x${VALUE}" = "x" || {
				echo "Missing build architecture value."
				exit $?
			}
			BUILD_DIST="${VALUE}"
		;;
		(--host)
			! test "x${VALUE}" = "x" || {
				echo "Missing host architecture value."
				exit $?
			}
			HOST_DIST="${VALUE}"
		;;
		(--target)
			! test "x${VALUE}" = "x" || {
				echo "Missing target architecture value."
				exit $?
			}
			TARGET_DIST="${VALUE}"
		;;
		(*)
			echo "Invalid argument ($1) found on command line."
			exit 1
		;;
	esac
	shift
done

PRECONFIGURATION=$*

test "x${HOST_DIST}" = "x${TARGET_DIST}" && {
	PKGNAME=${PKGNAME_BASE}
	DIST_DIR=${HOST_DIST}
} || {
	PKGNAME=${TARGET_DIST}-${PKGNAME_BASE}
	DIST_DIR=${HOST_DIST}-x-${TARGET_DIST}
}

# param 1 : inflie
# param 2 : outfile
function generate ()
{
	cat $1 | sed \
		-e "s|@PRJNAME@|${PRJNAME}|g" \
		-e "s|@PRJDESC@|${PRJDESC}|g" \
		-e "s|@PRJDESCRIPTION@|${PRJDESCRIPTION}|g" \
		-e "s|@PKGNAME@|${PKGNAME}|g" \
		-e "s|@VERSION_MAJOR@|${VERSION_MAJOR}|g" \
		-e "s|@VERSION_MINOR@|${VERSION_MINOR}|g" \
		-e "s|@VERSION_PATCH@|${VERSION_PATCH}|g" \
		-e "s|@VERSION_API@|${VERSION_API}|g" \
		-e "s|@VERSION@|${VERSION}|g" \
		-e "s|@DEVELOPMENT_START_YEAR@|${DEVELOPMENT_START_YEAR}|g" \
		-e "s|@AUTHOR@|${AUTHOR}|g" \
		-e "s|@EMAIL@|${EMAIL}|g" \
		-e "s|@TARGET_DIST@|${TARGET_DIST}|g" \
		-e "s|@HOST_DIST@|${HOST_DIST}|g" \
		-e "s|@DEBIAN_ARCH@|${DEBIAN_ARCH}|g" \
		-e "s|@CURRENT_DATE@|${CURRENT_DATE}|g" \
		-e "s|@CURRENT_YEAR@|${CURRENT_YEAR}|g" \
                -e "s|@PRECONFIGURATION@|${PRECONFIGURATION}|g" \
		> $2 || exit $?
}

#
# project directory
#

test -d ${DIST_DIR} || { mkdir -p ${DIST_DIR} || exit $? ; }

cp -p config ${DIST_DIR}/config || exit $?
generate configure.in ${DIST_DIR}/configure.sh || exit $?
chmod u+x ${DIST_DIR}/configure.sh || exit $?
generate Makefile.in ${DIST_DIR}/Makefile.in || exit $?
generate license.in ${DIST_DIR}/license.binary || exit $?
generate srclicense.in ${DIST_DIR}/license.source || exit $?
generate doxyfile.in ${DIST_DIR}/doxyfile.in || exit $?
generate butler.desktop.in ${DIST_DIR}/butler.desktop.in || exit $?
generate butler.man.in ${DIST_DIR}/butler.man.in || exit $?
generate config.h.in ${DIST_DIR}/config.h.in || exit $?

make -f source.mk DIST_DIR=${DIST_DIR} source -s

#
# debian packaging directory
#
test -d ${DIST_DIR}/debian || { mkdir -p ${DIST_DIR}/debian || exit $? ; }

generate license.in ${DIST_DIR}/debian/copyright || exit $?
generate debian/changelog.in ${DIST_DIR}/debian/changelog || exit $?
generate debian/control.in ${DIST_DIR}/debian/control || exit $?
test "x${BUILD_DIST}" = "x${TARGET_DIST}" && {
	generate debian/rules.native.in ${DIST_DIR}/debian/rules || exit $?
} || {
	generate debian/rules.cross.in ${DIST_DIR}/debian/rules || exit $?
}
chmod u+x ${DIST_DIR}/debian/rules || exit $?
echo "5" > ${DIST_DIR}/debian/compat || exit $?

test -d ${DIST_DIR}/debian/source || { mkdir -p ${DIST_DIR}/debian/source || exit $? ; }
echo "1.0" > ${DIST_DIR}/debian/source/format || exit $?

generate debian/pkg.install.in ${DIST_DIR}/debian/${PKGNAME}.install.in || exit $?
generate debian/dbg.install.in ${DIST_DIR}/debian/${PKGNAME}-dbg.install.in || exit $?
