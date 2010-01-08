#!/bin/bash

if test "x$1" = "x--help"; then
	echo Available benchmark backends: -tickcounter -callgrind -eventcounter
	exit
fi

BENCHMARK_BACKEND=$1

COVERAGE_AWK_PRG='{if($1=="#####:")uncovered++;else if($1=="-:");else covered++;} END{codelines = covered + uncovered; if(codelines==0) codelines=1; printf("covered %d lines of %d (%2.0f%%)\n", covered, codelines, covered*100/codelines);}'
# avoid debug messages for function enter/leave and leak suspections
export SUPPRESS_RUNTIME_BACKTRACE=

function run_test()
{
	test_name=$1
	source_file=$2

	echo $source_file

	if test ! -d ./test/$test_name; then
		echo "	"Error : directory ./test/$test_name does not exists
		exit -1
	fi
	if test ! -x ./test/$test_name/$test_name; then
		echo "	"Error : file ./test/$test_name/$test_name does not exists or not executable
		exit -1
	fi

	LD_LIBRARY_PATH=./src:$LD_LIBRARY_PATH ./test/$test_name/$test_name $BENCHMARK_BACKEND > test_result
	num_of_failed="$(grep Total test_result | awk '{printf("%d", $4)}')"
	if test ! "x$num_of_failed" = "x0"; then
		echo "	"Aborting tests because failed test found...
		cat test_result
		rm test_result
		exit -1
	fi
	echo "	"$(grep Total test_result)
	rm test_result

	is_header=${source_file/*.h/yes};
#	echo "is_header is :"$is_header
	if test "xyes" = "x$is_header"; then
		echo "	"no coverage informations for header files
		return 0
	fi

	cd src
#	echo gcov -o tmp/ $source_file \> /dev/null
	gcov -o tmp/ $source_file > /dev/null
	if test ! -e $source_file.gcov; then
		echo "	"Error : could not found $source_file.gcov after gcov executed
		exit -1
	fi
	echo -n "	coverage : "
	cat $source_file.gcov | awk "$COVERAGE_AWK_PRG"
	cd ..
}


echo -n Deleteing coverage result files...
for srcfile in $(find . | grep "tmp/.*\.gcda$"); do
#	echo "	"$srcfile
	rm $srcfile
done
echo " "done

echo
echo Testing files in order :
echo ~~~~~~~~~~~~~~~~~~~~~~~~
run_test	dbifc			butler_db.h
run_test	orderedset		butler_orderedset.h
run_test	tag			butler_tag.cpp
run_test	tagset			butler_tag_set.cpp
run_test	query			butler_query.cpp
run_test	queryset		butler_query_set.cpp
run_test	item			butler_item.cpp
run_test	itemset			butler_item_set.cpp

