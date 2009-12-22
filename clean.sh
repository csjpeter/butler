#!/bin/bash

subdirs="datamodel "

for dir in $subdirs; do
	pushd $dir
	echo
	echo Cleaning in $dir
	echo
	./clean.sh || {
		echo Error during $dir/clean.sh
		echo Aborting cleaning
		exit
	}
	popd
done

echo
echo All clean done
echo

