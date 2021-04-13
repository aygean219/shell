#!/bin/bash
for f in $*
do
	sort $f | uniq -c |sort -n | tail -1 > file.txt
done
sort -o file.txt  file.txt
