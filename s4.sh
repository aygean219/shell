#!/bin/bash
# SCRIETI UN SCRIPT CARE PRIMESTE ORICATE ARGUMENTE IN LINIA DE COMANDA
# ARGUMENTELE VOR FI PROCESATE IN FELUL URMATOR:
# --DACA ARGUMENTUL ESTE UN NUME DE UTILIZATOR DIN SISTEM SE VA AFISA PE ECRAN
# UN MESAJ CARE SA CONFIRME DACA UTILIZATORUL ARE SAU NU ARE SESIUNI ACTIVE
# IN MOMENTUL DE FATA
# --DACA ARGUMENTUL ESTE DIRECTOR SE VA AFISA PE ECRAN NUMARUL MEDIU DE CUVINTE
# PER LINIE DIN FIECARE FISIER ASCUNS DIN IERARHIA DIRECTORULUI
if [ $# -eq 0 ]
then
        echo "UTILIZARE : NUME_UTILIZATOR/DIRECTOR,NUME_UTILIZATOR/DIRECTOR..."
        exit 1
fi
for ARG
do
        if grep -Eq "$ARG" /etc/passwd
        then
                if who | grep -Eq "$ARG"
                then
                        echo  "$ARG este user si are sesiuni active"
                else
                        echo "$ARG este user si NU are sesiuni active"
                fi
        elif [ -d $ARG ]
        then
                echo "$ARG este director"
                FILES=$( find $ARG -type f |grep -E ".*") 
		for file in $FILES
		do 
                if [ -f $file ]
                then
                        medie=$(cat $file | awk 'BEGIN { nr=0 } {nr=nr+NF} END { print nr/NR}')
                        echo "Numarul medu de cuvinte pe linie din fisierul $file este $medie"
                fi
	        done
        else
        	echo "$ARG nu este nici director ,nici user"
        fi
done       
