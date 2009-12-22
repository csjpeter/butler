#!/bin/bash

if test "x$1" = "x--help"; then
	echo Available benchmark backends: -tickcounter -callgrind -eventcounter
	exit
fi

BENCHMARK_BACKEND=$1

# avoid debug messages for function enter/leave and leak suspections
export SUPPRESS_RUNTIME_BACKTRACE=

for tdir in $(ls -1 ./test/); do
	if test ! -d ./test/$tdir; then
		continue;
	fi
	LD_LIBRARY_PATH=./dist/src:$LD_LIBRARY_PATH ./test/$tdir/$tdir $BENCHMARK_BACKEND
done


