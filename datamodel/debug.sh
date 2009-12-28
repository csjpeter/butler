#!/bin/bash

echo Deleteing coverage result files...
for srcfile in $(find . | grep "tmp/.*\.gcda$"); do
	echo "	"$srcfile
	rm $srcfile
done
echo "	"done

target=$1

LD_LIBRARY_PATH=./src:$LD_LIBRARY_PATH ./test/$target/$target

