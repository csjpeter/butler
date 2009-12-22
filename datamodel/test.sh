#!/bin/bash

if test "x$1" = "x--help"; then
	echo Available benchmark backends: -tickcounter -callgrind -eventcounter
	exit
fi

BENCHMARK_BACKEND=$1

for tdir in $(ls -1 ./test/); do
	if test ! -d ./test/$tdir; then
		continue;
	fi
	LD_LIBRARY_PATH=./dist/src:$LD_LIBRARY_PATH ./test/$tdir/$tdir $BENCHMARK_BACKEND
done


