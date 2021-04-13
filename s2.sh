#!/bin/bash
# SCRIETI UN SCRIPT SHELL CARE PRIMESTE CA SI ARGUMENTE IN LINIA DE COMANDA
# ORICATE PERECHI DE TIP : FISIER(f) SI NUMAR(n).SCRIPTUL VA AFISA PE ECRAN 
# PRIMUL SI ULTIMUL CUVANT DE A n-A LINIE DIN FISIERUL f DIN FIECARE PERECHE
if [ $# -eq 0 ]
then
	echo "Utilizare : fisier numar, fisier numar, fisier numar..."
fi
while [ ! $# -eq 0 ]
do
	FISIER=$1
	NUMAR=$2
	shift 2
	if [ ! -f $FISIER ]
	then
		echo "NU ESTE FISIER $FISIER"
		exit 1
	fi
	if [ $NUMAR -gt $(cat $FISIER|wc -l) ]
	then
		echo "NUMARUL ESTE PREA MARE"
		exit 1
	fi
	awk -v n=$NUMAR '{if(NR==n) print $1 "---" $NF}' $FISIER
done
