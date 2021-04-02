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

TOTAL=0
NRF=0
FILES=$(find $DIR -type f)
for FILE in $FILES
do
	if file $FILE | grep -qE 'ASCII text'
	then
		NRL=$(cat $FILE|wc -l )
		TOTAL=$(($TOTAL+$NRL))
		let NRF=NRF+1
	fi
done
	echo "Total linii: " $(($TOTAL/$NRF))
