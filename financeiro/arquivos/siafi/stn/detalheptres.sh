#!/bin/bash
PGUSER=zeca
export PGUSER
PGPASSWORD=zeca
export PGPASSWORD
PGHOST=localhost
export PGHOST
PGDB=simec
export PGDB
PGSCHEMA=simecfinanceiro
export PGSCHEMA

TAB='\t';
NULO='\\N';
LIDOS='lidos/';
SQLCOPY='sqlCopy/';


FILE=$1;
echo -e "SET client_encoding TO 'LATIN5'; \n" >> ../${FILE}.sql;
echo  "COPY financeiro.ptres(ptrcod, unicod, ptrprgtrabalho, ptrapuracaosit) FROM stdin WITH NULL AS '${NULO}';" >> ${FILE}.sql;

#file_length=`wc -l '${FILE}' | cut -c1-5`;


cat ${FILE} | grep ^[^PT*] | while read LINHA;
do 

    ITCOUSUARIO=${LINHA:0:11};
    ITCOUSUARIO=`echo "$ITCOUSUARIO" | sed "s/ //g"`;
	test -z $ITCOUSUARIO;
	if [ $? == 0 ]; then
	ITCOUSUARIO=${NULO};
	fi    
    ITINOPERACAO=${LINHA:11:1};
    ITINOPERACAO=`echo "$ITINOPERACAO" | sed "s/ //g"`;
	test -z $ITINOPERACAO;
	if [ $? == 0 ]; then
	ITINOPERACAO=${NULO};
	fi
    DATATRANSACAO=${LINHA:12:8};
    DATATRANSACAO=`echo "$DATATRANSACAO" | sed "s/ //g"`;
	test -z $DATATRANSACAO;
	if [ $? == 0 ]; then
	DATATRANSACAO=${NULO};
	fi
    ITCOPROGRAMATRABALHORESUMIDO=${LINHA:20:6};
    ITCOPROGRAMATRABALHORESUMIDO=`echo "$ITCOPROGRAMATRABALHORESUMIDO" | sed "s/ //g"`;
	test -z $ITCOPROGRAMATRABALHORESUMIDO;
	if [ $? == 0 ]; then
	ITCOPROGRAMATRABALHORESUMIDO=${NULO};
	fi
    GRUNIDADEORCAMENTARIA=${LINHA:26:5};
    GRUNIDADEORCAMENTARIA=`echo "$GRUNIDADEORCAMENTARIA" | sed "s/ //g"`;
	test -z $GRUNIDADEORCAMENTARIA;
	if [ $? == 0 ]; then
	GRUNIDADEORCAMENTARIA=${NULO};
	fi
    GRPROGRAMATRABALHOA=${LINHA:31:17};
    GRPROGRAMATRABALHOA=`echo "$GRPROGRAMATRABALHOA" | sed "s/ //g"`;
	test -z $GRPROGRAMATRABALHOA;
	if [ $? == 0 ]; then
	GRPROGRAMATRABALHOA=${NULO};
	fi
    ITINRESULTADOLEI=${LINHA:48:1};
    ITINRESULTADOLEI=`echo "$ITINRESULTADOLEI" | sed "s/ //g"`;
	test -z $ITINRESULTADOLEI;
	if [ $? == 0 ]; then
	ITINRESULTADOLEI=${NULO};
	fi
    ITINTIPOCREDITO=${LINHA:49:1};
    ITINTIPOCREDITO=`echo "$ITINTIPOCREDITO" | sed "s/ //g"`;
	test -z $ITINTIPOCREDITO;
	if [ $? == 0 ]; then
	ITINTIPOCREDITO=${NULO};
	fi

    echo -e $ITCOPROGRAMATRABALHORESUMIDO${TAB}$GRUNIDADEORCAMENTARIA${TAB}$GRPROGRAMATRABALHOA${TAB}$ITINTIPOCREDITO >> ${FILE}.sql;

done
mv ${FILE} ${LIDOS};
tar -cf ${LIDOS}${FILE}".tar.gz" ${LIDOS}${FILE}
rm ${LIDOS}${FILE}
mv ${FILE}.sql ${SQLCOPY}/${FILE}.sql;

psql -h $PGHOST -U $PGUSER -d $PGDB -f ${SQLCOPY}/${FILE}.sql

