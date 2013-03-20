#!/bin/bash

# ubuntu 13.04 raring
# ubuntu 12.10 quantal
# ubuntu 12.04 precise
# ubuntu 11.10 oneiric
# ubuntu 10.04 lucid
# ubuntu 8.04 hardy

# debian 6.0 squeeze
# debian 5.0 lenny
# debian 4.0 etch
# debian 3.1 sarge
# debian 3.0 woody
# debian 2.2 potato
# debian 2.1 slink
# debian 2.0 hamm
# debian 1.3 bo
# debian 1.2 rex
# debian 1.1 buzz

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

function config ()
{
	local DIST=$1
	./dist-config.sh \
		--build=${DIST} \
		--host=${DIST} \
		--target=${DIST} \
		-- \
		--debug \
		--pkg-config-path=/opt/extras.ubuntu.com/csjp \
		--prefix=opt/butler0.1 \
		--appsdir=usr/share/applications \
		--fpic \
		--gnu-source || exit $?

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
	exec_in_dir ${DIST} debuild -S -us -uc || exit$?
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
       (*)
               config ${DISTRIB_CODENAME} || exit $?
               exec_in_dir ${DISTRIB_CODENAME} make $@ || exit $?
       ;;
esac

