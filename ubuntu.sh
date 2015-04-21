#!/bin/bash

JOBS=$(expr $(cat /proc/cpuinfo | grep processor | wc -l) + 1)

# ubuntu ditributions
trusty=14.04
raring=13.04
quantal=12.10
precise=12.04
oneiric=11.10
lucid=10.04
hardy=8.04

# debian distributions
squeeze=6.0
lenny=5.0
etch=4.0

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

function config ()
{
	local DIST=$1
	shift
	eval VERSION_POSTFIX=\$${DIST}-${DIST}
	./dist-config.sh \
		--build=${DIST} \
		--host=${DIST} \
		--target=${DIST} \
		--version-postfix=${VERSION_POSTFIX} \
		-- \
		--cflags=\\\"-DPQSQL\\\" \
		--pkg-config-path=/opt/extras.ubuntu.com/csjp/lib/pkgconfig \
		--prefix=usr \
		--cflags=\\\"-fno-candidate-functions\\\" \
		--libs=\\\"-lpq\\\" \
		--appsdir=usr/share/applications \
		--fpic \
		--gnu-source \
		$@ \
		--relative-path \
		|| exit $?

		#--prefix=opt/${PKGNAME_BASE}

	exec_in_dir ${DIST} ./configure || exit $?
}

function release ()
{
	local DIST=$1
	local ARCH=$2

	test "x${DIST}" = "x" && DIST=${DISTRIB_CODENAME}
	test "x${ARCH}" = "x" && ARCH=$(dpkg-architecture -qDEB_BUILD_ARCH)
	test "x${ARCH}" = "x$(dpkg-architecture -qDEB_BUILD_ARCH)" && {
		BASE_TGZ=$HOME/pbuilder/${DIST}-base.tgz
	} || {
		BASE_TGZ=$HOME/pbuilder/${DIST}-${ARCH}-base.tgz
	}
	test -e ${BASE_TGZ} || {
		pbuilder-dist ${DIST} ${ARCH} create || exit $?
		cat <<EOF > pbuilder-${DIST}-${ARCH}-init.sh
#!/bin/sh
echo "deb http://ppa.launchpad.net/csjpeter/ppa/ubuntu ${DIST} main" >> /etc/apt/sources.list
apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 55A478F4F4F90D5A
apt-get update
apt-get install libcsjp*
EOF
		chmod u+x pbuilder-${DIST}-${ARCH}-init.sh
		pbuilder-dist ${DIST} ${ARCH} execute --save-after-exec -- \
			pbuilder-${DIST}-${ARCH}-init.sh
	}

	config ${DIST} || exit $?
	exec_in_dir ${DIST} debuild --no-tgz-check -S -us -uc || exit$?
	pbuilder-dist ${DIST} ${ARCH} *.dsc --buildresult ${DIST}/ || exit $?
#		--othermirror "deb http://ppa.launchpad.net/csjpeter/ppa/ubuntu precise main"
}

CMD=$1

case "${CMD}" in
	(release)
		shift
		release $1 $2
	;;
	(release-all)
		shift
		release precise amd64
		release precise i386
		release quantal amd64
		release quantal i386
	;;
	(pump)
		shift
		config ${DISTRIB_CODENAME} || exit $?
		exec_in_dir ${DISTRIB_CODENAME} pump make CXX=distcc $@ || exit $?
	;;
	(debian)
		shift
		config ${DISTRIB_CODENAME} || exit $?
		exec_in_dir ${DISTRIB_CODENAME} debuild \
			--no-tgz-check \
			--preserve-envvar PATH \
			--preserve-envvar PKG_CONFIG_LIBDIR \
			--preserve-envvar PKG_CONFIG_PATH \
			-B $@ \
			--lintian-opts --no-lintian || exit $?
	;;
	(code)
		shift
		config ${DISTRIB_CODENAME} --debug || exit $?
		exec_in_dir ${DISTRIB_CODENAME} make -j1 $@ || exit $?
	;;
	(*)
		config ${DISTRIB_CODENAME} --debug || exit $?
		exec_in_dir ${DISTRIB_CODENAME} make -j${JOBS} $@ || exit $?
	;;
esac

