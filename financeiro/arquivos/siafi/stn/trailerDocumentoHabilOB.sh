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
    
    QUANTREGISTROSGUARDADO=${LINHA:2:8	}

    FILLER=${LINHA:10:90}

    echo -e $CONSTANTE${TAB}$QUANTREGISTROSGUARDADO${TAB}$FILLER >> ${FILE}.sql;

done
mv ${FILE} ${LIDOS};
tar -cf ${LIDOS}${FILE}".tar.gz" ${LIDOS}${FILE}
rm ${LIDOS}${FILE}
mv ${FILE}.sql ${SQLCOPY}/${FILE}.sql;

