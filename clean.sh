#!/bin/bash

subdirs="datamodel sqlite"
packages="butler-datamodel butler-sqlite"

for dir in $subdirs; do
	pushd $dir > /dev/null
	echo Cleaning in $dir
	echo
	./clean.sh || {
		echo Error during $dir/clean.sh
		echo Aborting cleaning
		exit
	}
	popd > /dev/null
done


function delete_file()
{
	filename=$1
	if test ! -e $filename -a ! -h $filename; then
#		echo Error: file $filename does not exists
		return
	fi

	echo -n "	$filename ... "
	rm -fr $filename && {
		echo done
	} || {
		echo failed
	}
}

echo Deleting package files in main directory...
for file in $packages; do
	for pfile in $(ls -d $file* 2> /dev/null); do
		if test -d $pfile; then
			continue;
		fi
		delete_file $pfile
	done
done

echo
echo All clean done
echo

