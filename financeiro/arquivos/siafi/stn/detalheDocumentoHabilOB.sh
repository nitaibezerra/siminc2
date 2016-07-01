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

    GRUGGESTAOANNUMEROLO=${CONSTANTE:0:23};
    
    GRUGGESTAONUMERODHUQ=${LINHA:23:23}

    ITINSITUACAOLISTA=${LINHA:46:1}

    FILLER=${LINHA:48:53}

    echo -e $GRUGGESTAOANNUMEROLO${TAB}$GRUGGESTAONUMERODHUQ${TAB}$ITINSITUACAOLISTA${TAB}$FILLER >> ${FILE}.sql;

done
mv ${FILE} ${LIDOS};
tar -cf ${LIDOS}${FILE}".tar.gz" ${LIDOS}${FILE}
rm ${LIDOS}${FILE}
mv ${FILE}.sql ${SQLCOPY}/${FILE}.sql;

