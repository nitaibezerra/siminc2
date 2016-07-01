#!/bin/bash

TAB='\t';
NULO='\\N';
LIDOS='lidos/';
SQLCOPY='sqlCopy/';

FILE=$1;
echo "COPY financeiro.*************(***********) FROM stdin WITH NULL AS '${NULO}';" >> ${FILE}.sql;

#file_length=`wc -l '${FILE}' | cut -c1-5`;


cat ${FILE} | grep ^[^SC*] | while read LINHA;
do 

    CONSTANTE=${CONSTANTE:0:2};
    
    DTINICIOMOVIMENTO=${LINHA:2:8}

    DTFIMCIOMOVIMENTO=${LINHA:10:8}

    FILLER=${LINHA:18:8782}

    echo -e $CONSTANTE${TAB}$DTINICIOMOVIMENTO${TAB}$DTFIMCIOMOVIMENTO${TAB}$FILLER >> ${FILE}.sql;

done
mv ${FILE} ${LIDOS};
tar -cf ${LIDOS}${FILE}".tar.gz" ${LIDOS}${FILE}
rm ${LIDOS}${FILE}
mv ${FILE}.sql ${SQLCOPY}/${FILE}.sql;

