#!/bin/bash
# SA SE CRIE UN SCRIPT SHELL CARE:
# --VA PRIMI CA ARGUMENTE UN SIR REPREZENTAND DREPTURILE DE ACCES (DE FORMA : rw-) URMAT 
# DE ORICATE NUME DE DIRECTOARE
# --DACA UTILIZATORUL NU A FURNIZAT NUMARUL NECESAR DE ARGUMENTE ,VA AFISA:
# Numar insuficient de argumente.
# Utilizare ./practic.sh rw- dir1 dir2 ...
# SI ISI VA INCHEIA EXECUTIA
# PENTRU FIECARE DIRECTOR FURNIZAT CA ARGUMENT
# a) DACA DIRECTORUL DAT NU EXISTA ,SCRIPTUL VA AFISA:
# Director : /home/.../ceva
# Disrectorul dat nu exista
# b) DACA DIRECTORUL DAT EXISTA:
# --PENTRU FIECARE FISIER AL CARUI PROPRIETAR ARE  DREPTURILE DE ACCES DATE SCRIPTUL VA CREA O COPIE 
# DE SIGURANTA CU ACELASI NUME SI EXTENSIA '.bak'
# --DREPTURILE DE ACCES ALE COPIEI VOR FII MODIFICATE ASTFEL INCAT NUMAI PROPRIETARUL FISIERULUI SA AIBA
# DREPTUL DE A CITI FISIERUL
# --PENTRU FIECARE FISIER CU DREPTURILE DE ACCES SPECIFICATE,SCRIPTUL VA AFISA URMATOARELE INFORMATII
# Permisiuni : rw-
# Director: /home/../test
# Fisier: /home/../test/file1
# Permisiuni : -rw-r--r--
# Copie: /home/../test/file1.bak
# Permisiuni: -r-------
# Director: ...
if [ $# -eq 0 ]
then
	echo "Numar insuficient de argumente."
	echo "Utilizare : ./s5.sh rw- dir1 dir2 ..."
	exit 1
fi
ACCES=$1
shift 1
for DIR
do
	if [ ! -d $DIR ]
	then
		echo "Director : $DIR"
		echo "Directorul dat nu exista sau nu este director!!"
	else
		FILES=$(ls -l |grep -E "$ACCES" |rev | cut -d" " -f 1 |rev) #NUMELE LA FISIERELE CU $ACCES
		for FILE in $FILES
		do
			NEW_NAME=$( echo "$FILE" | cut -d"." -f 1) #taie pana la punct
			cp $FILE "$NEW_NAME".bak
			chmod 400 "$NEW_NAME".bak
			echo "-------------------------------------------------------------"
			echo " Permisiuni : $ACCES"
			echo " Director : $DIR"
			echo " Fisier: $FILE"
			echo " Permisiuni: $(ls -l $FILE|cut -d" " -f 1)"
			echo " Copie: $NEW_NAME.bak"
			echo " Permisiuni: $(ls -l "$NEW_NAME".bak|cut -d" " -f 1)"
		done
	fi
done
