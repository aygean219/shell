#!/bin/bash


if [ $# -eq 0 ] 
then	
	echo "Utilizare" $0 DIRECTOR
	exit 1	
fi
DIR=$1
if [ ! -d $DIR ]
then
	echo "$DIR NU E DIRECTOR!!"
	exit 1
fi

FILES=$(find $DIR -type f)
for FILE in $FILES
do
	if cat $FILE | grep -qE '[0-9]{5,}'
	then
		echo "Fisier: " $FILE
	fi

done

