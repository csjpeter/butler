#!/bin/bash

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

echo Deleting target result files ...
for tfile in $(find ./src/ | grep "libbutler-datamodel\.so[\.0123456789]*"); do
	delete_file $tfile
done
for tdir in $(ls -1 ./test/); do
	if test ! -d ./test/$tdir; then
		continue;
	fi
	delete_file "./test/$tdir/$tdir"
done
for file in $(ls -1 ./src/*.prf 2> /dev/null); do
	delete_file $file
done

echo Deleting makefiles ...
for mkfile in $(find . | grep Makefile); do
	delete_file $mkfile
done

echo Deleting object files ...
for ofile in $(find . | grep "\.o$"); do
	delete_file $ofile
done

echo Deleting generated source files ...
for srcfile in $(find . | grep "tmp/moc_.*\.cpp$"); do
	delete_file $srcfile
done

echo Deleting generated moc files ...
for srcfile in $(find . | grep "tmp/.*\.moc$"); do
	delete_file $srcfile
done

echo Deleting coverage assistant files ...
for srcfile in $(find . | grep "tmp/.*\.gcda$"); do
	delete_file $srcfile
done
for srcfile in $(find . | grep "tmp/.*\.gcno$"); do
	delete_file $srcfile
done

echo Deleting coverage result files ...
for srcfile in $(find . | grep "\.gcov$"); do
	delete_file $srcfile
done

echo Deleting directories used for compilation ...
for srcfile in $(find . | grep "/tmp$"); do
	delete_file $srcfile
done

echo Deleting editor backup files ...
for bakfile in $(find . | grep "~$"); do
	delete_file $bakfile
done

echo
echo Datamodel clean done
echo
