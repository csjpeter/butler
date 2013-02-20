#!/bin/bash

./dist-config.sh -- --fpic --gnu-source

pushd precise > /dev/null || exit $?
make $1
let RET=$?
popd > /dev/null
exit ${RET}

