#!/bin/bash
# SCRIETI UN SCRIPT SHELL CARE PRIMESTE CA SI ARGUMENTE IN LINIA DE COMANDA
# TRIPLETE DE TIP w,x,f (w SI x SUNT CUVINTE,f ESTE FISIER).PENTRU FIECARE
# TRIPLET SE VA AFISA PE ECRAN NUMELE FISIERULUI f DACA PRIMUL CUVANT DIN 
# FISIER ESTE w SI ULTIMUL CUVANT DIN FISIER ESTE x.
if [ $# -eq 0 ]
then
	echo "UTILIZARE : CUVANT CUVANT FISIER,CUVANT CUVANT FISIER..."
	exit 1
fi
while [ ! $# -eq 0 ]
do
	W=$1
	X=$2
	F=$3
	shift 3
	VERIFICA=0
	WORD1=$( head -1 $F | cut -d" " -f 1)
	WORD2=$( tail -1 $F | rev | cut -d" " -f 1 | rev)
	if [ $W = $WORD1 ] 
	then
		if [ $X = $WORD2 ]
		then
			echo "FISIER: $F"
		fi
	fi

done
