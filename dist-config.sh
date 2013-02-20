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

test "x${HOST_DIST}" = "x${TARGET_DIST}" && {
	PKGNAME=${PKGNAME_BASE}
	DIST_DIR=${HOST_DIST}
} || {
	PKGNAME=${TARGET_DIST}-${PKGNAME_BASE}
	DIST_DIR=${HOST_DIST}-x-${TARGET_DIST}
}

# param 1 : inflie
# param 2 : outfile
function configure ()
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
		> $2 || exit $?
}

#
# project directory
#

test -d ${DIST_DIR} || { mkdir -p ${DIST_DIR} || exit $? ; }

cp -p config ${DIST_DIR}/config || exit $?
configure configure.in ${DIST_DIR}/configure.sh || exit $?
chmod u+x ${DIST_DIR}/configure.sh || exit $?
configure Makefile.in ${DIST_DIR}/Makefile.in || exit $?
configure license.in ${DIST_DIR}/license.binary || exit $?
configure srclicense.in ${DIST_DIR}/license.source || exit $?
configure doxyfile.in ${DIST_DIR}/doxyfile.in || exit $?
configure pc.in ${DIST_DIR}/pc.in || exit $?
configure test.man.in ${DIST_DIR}/test.man.in || exit $?

for f in $(find src -name *.h); do
	make -f source.mk DIST_DIR=${DIST_DIR} ${DIST_DIR}/$f -s
done
for f in $(find src -name *.cpp); do
	make -f source.mk DIST_DIR=${DIST_DIR} ${DIST_DIR}/$f -s
done

#
# debian packaging directory
#
test -d ${DIST_DIR}/debian || { mkdir -p ${DIST_DIR}/debian || exit $? ; }

configure license.in ${DIST_DIR}/debian/copyright || exit $?
configure debian/changelog.in ${DIST_DIR}/debian/changelog || exit $?
configure debian/control.in ${DIST_DIR}/debian/control || exit $?
test "x${BUILD_DIST}" = "x${TARGET_DIST}" && {
	configure debian/rules.native.in ${DIST_DIR}/debian/rules || exit $?
} || {
	configure debian/rules.cross.in ${DIST_DIR}/debian/rules || exit $?
}
chmod u+x ${DIST_DIR}/debian/rules || exit $?
echo "5" > ${DIST_DIR}/debian/compat || exit $?

test -d ${DIST_DIR}/debian/source || { mkdir -p ${DIST_DIR}/debian/source || exit $? ; }
echo "1.0" > ${DIST_DIR}/debian/source/format || exit $?

configure debian/pkg.install.in ${DIST_DIR}/debian/${PKGNAME}.install.in || exit $?
configure debian/dbg.install.in ${DIST_DIR}/debian/${PKGNAME}-dbg.install.in || exit $?
configure debian/test.install.in ${DIST_DIR}/debian/${PKGNAME}-test.install.in || exit $?
configure debian/dev.install.in ${DIST_DIR}/debian/${PKGNAME}-dev.install.in || exit $?
configure debian/doc.install.in ${DIST_DIR}/debian/${PKGNAME}-doc.install.in || exit $?

# auto calling configure
test "x$1" = "x" || { cd ${DIST_DIR} && ./configure.sh "$@" || exit $? ; }

