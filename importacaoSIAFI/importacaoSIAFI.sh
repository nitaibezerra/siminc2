#!/bin/bash

# DEFINIÇÃO DE VARIÁVEIS
#ftp
USER='serpro'
PASSWD=WEB_SERVICE_SIOP_SENHA
#log
LOGPG='logs/';
FILELOG=$LOGPG"log_"`date +%Y%m%d`;
#banco
PGUSER=''
PGPASSWORD=''
PGHOST=''
PGDB=''
PGSCHEMA=''
#exportando para conexão com banco
export PGUSER PGPASSWORD PGHOST PGDB PGSCHEMA FILELOG
#tratamento de arquivos
ARQ=`ls *.TXT | grep .TXT`;
DIA="${ARQ:3:2}";
MES="${ARQ:5:2}";
ANO="20""${ARQ:7:2}";
COUNT=0

#log
echo "Inicio Da importação:	"`date`>> ${FILELOG};

#Busca arquivos por FTP
ftp -n 10.1.3.220 <<END_SCRIPT
quote USER $USER
quote PASS $PASSWD
cd /home/ftp/serpro
prompt
mget *.TXT
mdelete *.TXT
quit
END_SCRIPT

echo  "COPY siafi.importacao(imparquivo, impdata, imphorainicio, imphorafim, impstatus, impquantidade) FROM stdin;" > "logFileSql.sql";

LISTA="`ls *.TXT | grep -v .sql`"
for i in $LISTA; do

    echo Processando ---- $i

    while true; do
	COUNT=`ps ax | grep "./detalhe" | grep -v grep | wc -l`
	if [ $COUNT -lt 10 ]; then
    		#CONTABIL
    		if [ "${i:0:2}" = "SC" ]; then 
			./detalheSC.sh $i & 
		fi
#	    	if [ "${i:0:2}" = "LC" ]; then 
#			./detalheLC.sh $i & 
#		fi
	    	if [ "${i:0:2}" = "CD" ]; then 
			./detalheCD.sh $i & 
		fi
    
	    	#CONVENIOS
    		if [ "${i:0:2}" = "CC" ]; then 
			./detalheCC.sh $i & 
		fi
	    	if [ "${i:0:2}" = "CV" ]; then 
			./detalheCV.sh $i & 
		fi
#	    	if [ "${i:0:2}" = "DL" ]; then 
#			./detalheDL.sh $i & 
#		fi
	    	if [ "${i:0:2}" = "EC" ]; then 
			./detalheEC.sh $i & 
		fi
    	
	    	#DOCUMENTOS
    		if [ "${i:0:2}" = "NC" ]; then 
			./detalheNC.sh $i & 
		fi
#   		if [ "${i:0:2}" = "ND" ]; then 
#			./detalheND.sh $i & 
#		fi
	    	if [ "${i:0:2}" = "NE" ]; then 
			./detalheNE.sh $i & 
		fi
    		if [ "${i:0:2}" = "NL" ]; then 
			./detalheNL.sh $i & 
		fi
	    	if [ "${i:0:2}" = "NS" ]; then 
			./detalheNS.sh $i & 
		fi
    		if [ "${i:0:2}" = "OB" ]; then 
			./detalheOB.sh $i & 
		fi
	    	if [ "${i:0:2}" = "PE" ]; then 
			./detalhePE.sh $i & 
		fi
    		if [ "${i:0:2}" = "PT" ]; then 
			./detalhePT.sh $i & 
		fi
	    	if [ "${i:0:2}" = "RP" ]; then 
			./detalheRP.sh $i & 
		fi
		
	    	#TABELAS
    		if [ "${i:0:2}" = "TA" ]; then 
			./detalheTA.sh $i & 
		fi
	    	if [ "${i:0:2}" = "TB" ]; then 
			./detalheTB.sh $i & 
		fi
    		break
	fi   
    done

done

#Sobre informações de log para banco
psql -h $PGHOST -U $PGUSER -d $PGDB -f "logFileSql.sql";
echo "Fim Da importação:	"`date`>> ${FILELOG};

#Compacta e transfere os arquivos originais
tar -zcf cargadiaria/carga-diaria_$ANO-$MES-$DIA".txt.tar.gz" *.TXT
tar -zcf cargadiaria/carga-diaria_$ANO-$MES-$DIA".sql.tar.gz" *.sql
#rm *$DIA$MES*.TXT;
#rm *$DIA$MES*.sql;