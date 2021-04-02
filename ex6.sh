#!/bin/bash


if [ $# -eq 0 ] 
then	
	echo "Utilizare" $0 NUME1 NUME2 NUME3
	exit 1	
fi

for NUME in $*
do
	if [ -d $NUME ]
	then
		NRF=$( find $NUME -type f | grep -cE '.' )
		echo "DIRECTOR --> $NUME" "NR FISIERE-->$NRF"
	elif [ -f $NUME ]
	then
		echo "FISIER: $NUME"
		NRC=$( cat $NUME|wc -m)
		NRL=$(cat $NUME| wc -l)
		echo "LINII: $NRL ,CARACTERE: $NRC"	
	fi
	
done
