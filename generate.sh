#!/bin/bash

test -d graph || mkdir graph
rm graph/* 2> /dev/null

for file in $(ls dot/*.dot); do
	file=$(basename $file);
	file=${file%.*};
#	filepp dot/${file}.dot | dot -Tsvg -o graph/${file}.svg
	filepp dot/${file}.dot | dot -Tpng -o graph/${file}.png
#	filepp dot/${file}.dot | dot -Tpdf -o graph/${file}.pdf
	filepp dot/${file}.dot | dot -Tps -o graph/${file}.ps
done

