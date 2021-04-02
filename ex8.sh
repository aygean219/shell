#!/bin/bash


if [ $# -eq 0 ] 
then	
	echo "Utilizare" $0 FILE1 FILE2 FILE3
	exit 1	
fi

for FILE in $*
do
 	I=1
	LINII=$( cat $FILE)
	for LINIE in $LINII
	do
		TOTAL=1
		J=$(($I+1))
		NRL=$(cat $FILE|wc -l)
		while [ $J -le $NRL ]
		do
			LURMATOARE=$( sed -n ''$J'p' $FILE)
			if [ "$LINIE" = "$LURMATOARE" ]
			then
				TOTAL=$[$TOTAL+1]

			fi
			J=$[$J+1]
					
		done
		echo "LINIA $I , NR APARITII $TOTAL"
		I=$[$I+1] 
	done	
done
