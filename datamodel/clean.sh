#!/bin/bash

function delete_file()
{
	filename=$1
	if test ! -e $filename -a ! -h $filename; then
		echo Error: file $filename does not exists
		return
	fi

	echo -n "	$filename ... "
	rm -fr $filename && {
		echo done
	} || {
		echo failed
	}
}

echo Deleting target result files :
for tfile in $(find ./dist/src/ | grep "libbutler_datamodel\.so[\.0123456789]*"); do
	delete_file $tfile
done
delete_file "./test/test_datamodel"

echo Deleting makefiles :
for mkfile in $(find . | grep Makefile); do
	delete_file $mkfile
done

echo Deleting object files :
for ofile in $(find . | grep "\.o$"); do
	delete_file $ofile
done

echo Deleting editor backup files :
for bakfile in $(find . | grep "~$"); do
	delete_file $bakfile
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

echo datamodel clean done

