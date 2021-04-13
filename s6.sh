#!/bin/bash
# SA SE SCRIE UN SCRIPT SHELL CARE :
# --VA PRIMI CA ARGUMENT NUMELE UNUI FISIER CARE CONTINE CUVINTELE DATE;
# -DACA UTILIZATORUL NU A FURNIZAT NICIUN ARGUMENT VA AFISA:
# Numar insuficient de argumente .
# Utilizare: ./script.sh fisier_cuvinte
# si isi va incheia executia
# SCRIPTUL:
# --VA CITI DE LA TASTATURA UN NUMAR N ( 0< N < 10 ) PANA SE INTRODUCE 0;
# --VA CITI DE LA TASTATURA UN NUME DE FISIER SI VA CREA UN FISIER CU ACEL NUME;
# --VA CITI CUVINTE DIN FISIERUL FURNIZAT CA ARGUMENT ,VA FORMA DIN CUVINTELE CITITE PROPOZITII
# CARE CONTIN EXACT N CUVINTE SI LE VA SCRIE IN FISIERUL CREAT ANTERIOR
# --DUPA CE A TERMINAT DE CITIT TOATE CUVINTELE,VA DETERMINA SI VA AFISA NUMARUL DE LINII,NUMARUL DE CUVINTE
# SI NUMARUL DE CARACTERE DIN FISIERUL FINAL,ASTFEL:
# Fisier: propoztii.txt
# Linii: 5, Cuvinte: 16, Caractere: 90
if [ $# -eq 0 ]
then
	echo "Numar insuficient de argumente"
	echo "Utilizare : ./script.sh fisier_cuvinte"
	exit 1
fi
read -p "Numele de fisier in care doriti sa se introduce propozitiile: " FILE
touch "$FILE"
read -p "N= " N
while [ ! $N -eq 0 ]
do
	if [ $N -le 0 -o $N -ge 10 ]
	then
		echo "Numarul trebuie sa fie mai mare decat 0 si mai mic decat 10!!"
	fi
	CUVINTE=0
	PROPOZITIE=""
	cat $1 | while read CUVANT 
	do	
		PROPOZITIE=$PROPOZITIE" "$CUVANT
		CUVINTE=$(( $CUVINTE + 1))
		if [ $CUVINTE -ge $N ]
		then
			echo $PROPOZITIE >>  aux.txt
			exit 1
		fi	
	done
	read -p "N= " N
done
	cat aux.txt > $FILE
	NR_LINII=$(cat "$FILE" | wc -l)
	NR_CUVINTE=$(cat "$FILE" | wc -w )
	NR_CARACTERE=$(cat "$FILE" | wc -m)
	echo "Fisier : $FILE"
	echo " Linii : $NR_LINII -- CUVINTE: $NR_CUVINTE -- CARACTERE : $NR_CARACTERE"
