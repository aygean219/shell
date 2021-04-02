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
	if file $FILE | grep -qE 'ASCII text'
	then
		echo "FISIER : $FILE "
		NRL=$(cat $FILE|grep -c '.')
		if [ $NRL -lt 10 ]
		then
			cat $FILE
		else
			head -5 $FILE 
			tail -5 $FILE
		fi

	fi

done

