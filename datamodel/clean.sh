#!/bin/bash

function delete_file()
{
	filename=$1
	if test ! -e $filename; then
		return
	fi

	echo -n "	$filename ... "
	rm -fr $filename && {
		echo done
	} || {
		echo failed
	}
}

echo Deleting makefiles :
for mkfile in $(find . | grep Makefile); do
	delete_file $mkfile
done

echo Deleting object files :
for ofile in $(find . | grep "\.o$"); do
	delete_file $ofile
done

echo Deleting generated source files :
if test ! -e "./dist/src/tmp"; then
	for srcfile in $(find ./dist/src/tmp/ | grep "\.cpp$"); do
		delete_file $srcfile
	done
fi

delete_file "./dist/src/tmp"
delete_file "./test/tmp"

echo Deleting preinstalled trees under debian directory :
delete_file "./debian/butler-datamodel"
delete_file "./debian/butler-datamodel-dbg"

echo Deleting some generated file under debian directory :
delete_file "./debian/butler-datamodel.substvars"
delete_file "./debian/files"

echo Done

