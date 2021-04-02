#!/bin/bash


if [ $# -eq 0 ] 
then	
	echo "Utilizare" $0 N
	exit 1	
fi
N=$1
X=1
while [ $X -le $N ]
do
	touch "file_$X.txt"
	sed -n ''$X',+5p' /etc/passwd >"file_$X.txt"
	X=$(($X+1))
	
done

