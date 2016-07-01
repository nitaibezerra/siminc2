#!/bin/bash

PGUSER=''
PGPASSWORD=''
PGHOST=''
PGDB=''
PGSCHEMA=''
export PGUSER PGPASSWORD PGHOST PGDB PGSCHEMA

ls -la --scontext *.gz | while read LINHA;
do 
	#gzip -d $LINHA;
	FILETXT="${LINHA/.gz/}"
	#echo $FILETXT;
	
	if [ ${LINHA:0:2} == 'SC' ]; then
		gzip -d $LINHA;
		./detalheSadoContabil.sh $FILETXT
	fi

	if [ ${LINHA:0:2} == 'PT' ]; then
		gzip -d $LINHA;
		./detalheptres.sh $FILETXT
	fi

	if [ ${LINHA:0:2} == 'NE' ]; then
		gzip -d $LINHA;
		./detalheNotasEmpenhoNE.sh $FILETXT
	fi
done
