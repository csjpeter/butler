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
	LD_LIBRARY_PATH=./src:$LD_LIBRARY_PATH ./test/$tdir/$tdir $BENCHMARK_BACKEND > test_result
	num_of_failed="$(grep Total test_result | awk '{printf("%d", $4)}')"
	cat test_result
	rm test_result
	if test ! "x$num_of_failed" = "x0"; then
		echo Aborting tests because failed test found...
		exit -1
	fi
done

COVERAGE_AWK_PRG='{if($1=="#####:")uncovered++;else if($1=="-:");else covered++;} END{codelines = covered + uncovered; printf("covered %d lines of %d (%2f)\n", covered, codelines, covered*100/codelines);}'

cd src
gcov -o tmp/ *.cpp > /dev/null
echo Test coverage of .cpp files...
for cppfile in $(ls -1 *.cpp); do
	if test ! -e $cppfile.gcov; then
		continue;
	fi
	echo -n "	"$cppfile" : "
	cat $cppfile.gcov | awk "$COVERAGE_AWK_PRG"
done
cd ..

