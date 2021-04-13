#!/bin/bash
#----------------------------------------------------------------------------------------------------
# SA ASE SCRIE UN SCRIPT CARE PRIMESTE CA ARGUMENT LA LINIA DE COMANDA UN NUME DE DIRECTOR
# SCRIPTUL VA CAUT RECURSIV IN DIRECTOR SI VA AFISA:
# -NUMELE FISIERELOR SURSA C CARE AU EXTENSIA ".c"
# -NUMARUL TOTAL DE LINII DE COD DIN TOATE FISIERELE SURSA C CARE NU AU EXTENSIA ".c"
if [ $# -eq 0 ]
then
        echo "Utilizare introduceti un director"
	exit 1
fi
if [ ! -d $1 ]
then
	echo "TREBUIE SA FIE FISIER!!"
	exit 1
fi
for FILE in $(find "$1" -type f | grep -E "\.c")
do
	if file $FILE |grep -Eq "C source"
	then
		echo $FILE
	fi
done
exit 1
SUM=0
for FILE in $(find "$1" -type f | grep -Ev "\.c")
do
	if file $FILE | grep -Eq "C source"
	then
		LINES=$(wc -l $FILE)
		SUM=$(( $SUM + $LINES))
	fi
done
echo "NR TOTAL LINII :$SUM"
