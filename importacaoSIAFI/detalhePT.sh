#!/bin/bash

# Definição de variáveis
TAB='\t';
NULO='\\N';
LIDOS='lidos/';
LENDO='Lendo/';
SQLCOPY='sqlCopy/';
LOGPG='logPGsql.log';
FILE=$1;
DATAI=`date +%Y-%m-%d`" "`date +%H:%M:%S`;

# Início do LOG
echo "####################################">> ${FILELOG};
echo "Inicio do arquivo $1:	"`date`>> ${FILELOG};

contasNum() {
	operadorValor="";
	case $1 in
       		"J")
		operadorValor=1;
        ;;
       		"K")
		operadorValor=2;
        ;;
       		"L")
		operadorValor=3;
        ;;
       		"M")
		operadorValor=4;
        ;;
       		"N")
		operadorValor=5;
        ;;
       		"O")
		operadorValor=6;
        ;;
       		"P")
		operadorValor=7;
        ;;
       		"Q")
		operadorValor=8;
        ;;
       		"R")
		operadorValor=9;
        ;;
       		"}")
		operadorValor=0;
        ;;
		*)
		operadorValor=99;
	;;
	esac
}

# Início da geração do script !!!
echo -e "SET client_encoding TO 'LATIN5'; " >> ${FILE}.sql;
echo  "COPY siafi.ptres(it_co_usuario, it_in_operacao, data_transacao, it_co_programa_trabalho_resumido, gr_unidade_orcamentaria, gr_programa_trabalho_a, it_in_resultado_lei, it_in_tipo_credito)   FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^PT*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
ITCOUSUARIO=${LINHA:0:11};
	if [ -z $ITCOUSUARIO ]; then
	ITCOUSUARIO=${NULO};
	fi

ITINOPERACAO=${LINHA:11:1};
	if [ -z $ITINOPERACAO ]; then
	ITINOPERACAO=${NULO};
	fi

DATATRANSACAO=${LINHA:12:8};
	if [ -z $DATATRANSACAO ]; then
	DATATRANSACAO=${NULO};
	else
	DATATRANSACAO=${LINHA:12:2}"/"${LINHA:14:2}"/"${LINHA:16:4};
	fi

ITCOPROGRAMATRABALHORESUMIDO=${LINHA:20:6};
	if [ -z $ITCOPROGRAMATRABALHORESUMIDO ]; then
	ITCOPROGRAMATRABALHORESUMIDO=${NULO};
	fi

GRUNIDADEORCAMENTARIA=${LINHA:26:5};
	if [ -z $GRUNIDADEORCAMENTARIA ]; then
	GRUNIDADEORCAMENTARIA=${NULO};
	fi

GRPROGRAMATRABALHOA=${LINHA:31:17};
	if [ -z $GRPROGRAMATRABALHOA ]; then
	GRPROGRAMATRABALHOA=${NULO};
	fi

ITINRESULTADOLEI=${LINHA:48:1};
	if [ -z $ITINRESULTADOLEI ]; then
	ITINRESULTADOLEI=${NULO};
	fi

ITINTIPOCREDITO=${LINHA:49:1};
	if [ -z $ITINTIPOCREDITO ]; then
	ITINTIPOCREDITO=${NULO};
	fi

echo -e $ITCOUSUARIO${TAB}$ITINOPERACAO${TAB}$DATATRANSACAO${TAB}$ITCOPROGRAMATRABALHORESUMIDO${TAB}$GRUNIDADEORCAMENTARIA${TAB}$GRPROGRAMATRABALHOA${TAB}$ITINRESULTADOLEI${TAB}$ITINTIPOCREDITO >> ${FILE}.sql;
done

# Compacta arquivo na pasta LIDOS
#tar -zcf ${LIDOS}${FILE}".tar.gz" ${FILE}

# Executa o script gerado
psql -h $PGHOST -U $PGUSER -d $PGDB -f ${FILE}".sql" >> ${FILELOG};

# Gera o LOG
echo "Linhas Processadas:	"`cat ${FILE}".sql" | wc -l` >> ${FILELOG};
echo -e $1${TAB}""`date +%Y-%m-%d`" "`date +%H:%M:%S`${TAB}$DATAI${TAB}""`date +%Y-%m-%d`" "`date +%H:%M:%S`${TAB}I${TAB}""`cat ${FILE}".sql" | wc -l` > ${SQLCOPY}logFileSql.sql;

# Compacta o script na pasta SQLCOPY
#tar -zcf ${SQLCOPY}${FILE}".sql.tar.gz" ${FILE}".sql"
#rm ${FILE}".sql"

# FIM
echo "Fim do arquivo $1:	"`date`>> ${FILELOG};
echo "####################################">> ${FILELOG};