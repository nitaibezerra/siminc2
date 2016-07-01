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
echo  "COPY siafi.pe(cpf_usuario, terminal_usuario, data_transacao, hora_transacao, codigo_ug_operador, numero_pe, data_emissao, data_limite, ug_favorecida, gestao_favorecida, observacao, codigo_evento, esfera_orcamentaria, ptres, fonte_recurso, natureza_despesa, codigo_ug_responsavel, plano_interno, valor_transacao, codigo_ug_doc_referencia, codigo_gestao_doc_referencia, mes_lancamento, saldo, cancelamento_pe, pe_original, pe_cancelamento, it_op_cambial)   FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^PE*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
CPFDOUSUARIO=${LINHA:0:11};
	if [ -z "$CPFDOUSUARIO" ]; then
	CPFDOUSUARIO=${NULO};
	fi

TERMINALDOUSUARIO=${LINHA:11:8};
	if [ -z "$TERMINALDOUSUARIO" ]; then
	TERMINALDOUSUARIO=${NULO};
	fi

DATADETRANSACAO=${LINHA:19:8};
	if [ -z "$DATADETRANSACAO" ]; then
	DATADETRANSACAO=${NULO};
	else
	DATADETRANSACAO=${LINHA:19:2}"/"${LINHA:21:2}"/"${LINHA:23:4};
	fi

HORATRANSACAO=${LINHA:27:4};
	if [ -z "$HORATRANSACAO" ]; then
	HORATRANSACAO=${NULO};
	else
	HORATRANSACAO=${LINHA:27:2}":"${LINHA:29:2};
	fi

CODIGODAUGDOOPERADOR=${LINHA:31:6};
	if [ -z "$CODIGODAUGDOOPERADOR" ]; then
	CODIGODAUGDOOPERADOR=${NULO};
	fi

NUMERODAPE=${LINHA:37:23};
	if [ -z "$NUMERODAPE" ]; then
	NUMERODAPE=${NULO};
	fi

DATADEEMISSAO=${LINHA:60:8};
	if [ -z $DATADEEMISSAO ]; then
	DATADEEMISSAO=${NULO};
	else
	DATADEEMISSAO=${LINHA:60:2}"/"${LINHA:62:2}"/"${LINHA:64:4};
	fi

DATALIMITE=${LINHA:68:8};
	if [ -z $DATALIMITE ]; then
	DATALIMITE=${NULO};
	else
	DATALIMITE=${LINHA:68:2}"/"${LINHA:70:2}"/"${LINHA:72:4};
	fi

UGFAVORECIDA=${LINHA:76:6};
	if [ -z "$UGFAVORECIDA" ]; then
	UGFAVORECIDA=${NULO};
	fi

GESTAOFAVORECIDA=${LINHA:82:5};
	if [ -z "$GESTAOFAVORECIDA" ]; then
	GESTAOFAVORECIDA=${NULO};
	fi

OBSERVACAO=${LINHA:87:234};
	if [ -z "$OBSERVACAO" ]; then
	OBSERVACAO=${NULO};
	fi

CODIGODOEVENTO=${LINHA:321:6};
	if [ -z "$CODIGODOEVENTO" ]; then
	CODIGODOEVENTO=${NULO};
	fi

ESFERAORCAMENTARIA=${LINHA:327:1};
	if [ -z "$ESFERAORCAMENTARIA" ]; then
	ESFERAORCAMENTARIA=${NULO};
	fi

PROGRAMADETRABALHORESUMIDO=${LINHA:328:6};
	if [ -z "$PROGRAMADETRABALHORESUMIDO" ]; then
	PROGRAMADETRABALHORESUMIDO=${NULO};
	fi

FONTEDERECURSO=${LINHA:334:10};
	if [ -z "$FONTEDERECURSO" ]; then
	FONTEDERECURSO=${NULO};
	fi

NATUREZADEDESPESA=${LINHA:344:6};
	if [ -z "$NATUREZADEDESPESA" ]; then
	NATUREZADEDESPESA=${NULO};
	fi

CODIGODAUGRESPONSAVEL=${LINHA:350:6};
	if [ -z "$CODIGODAUGRESPONSAVEL" ]; then
	CODIGODAUGRESPONSAVEL=${NULO};
	fi

PLANOINTERNO=${LINHA:356:11};
	if [ -z "$PLANOINTERNO" ]; then
	PLANOINTERNO=${NULO};
	fi

VALORDATRANSACAO=${LINHA:367:17};
	if [ -z "$VALORDATRANSACAO" ]; then
	VALORDATRANSACAO=${NULO};
	else
	VALORDATRANSACAO=${LINHA:367:15}"."${LINHA:382:2};
	fi

CODIGODAUGDODOC=${LINHA:384:6};
	if [ -z "$CODIGODAUGDODOC" ]; then
	CODIGODAUGDODOC=${NULO};
	fi

CODIGODAGESTAODODOC=${LINHA:390:5};
	if [ -z "$CODIGODAGESTAODODOC" ]; then
	CODIGODAGESTAODODOC=${NULO};
	fi

MESDELANCAMENTO=${LINHA:395:2};
	if [ -z "$MESDELANCAMENTO" ]; then
	MESDELANCAMENTO=${NULO};
	fi

SALDO=${LINHA:397:1};
	if [ -z "$SALDO" ]; then
	SALDO=${NULO};
	fi

CANCELAMENTOPE=${LINHA:398:1};
	if [ -z "$CANCELAMENTOPE" ]; then
	CANCELAMENTOPE=${NULO};
	fi

PEORIGINAL=${LINHA:399:6};
	if [ -z "$PEORIGINAL" ]; then
	PEORIGINAL=${NULO};
	fi

PECANCELAMENTO=${LINHA:405:6};
	if [ -z "$PECANCELAMENTO" ]; then
	PECANCELAMENTO=${NULO};
	fi

ITOPCAMBIAL=${LINHA:411:10};
	if [ -z "$ITOPCAMBIAL" ]; then
	ITOPCAMBIAL=${NULO};
	fi

echo -e $CPFDOUSUARIO${TAB}$TERMINALDOUSUARIO${TAB}$DATADETRANSACAO${TAB}$HORATRANSACAO${TAB}$CODIGODAUGDOOPERADOR${TAB}$NUMERODAPE${TAB}$DATADEEMISSAO${TAB}$DATALIMITE${TAB}$UGFAVORECIDA${TAB}$GESTAOFAVORECIDA${TAB}$OBSERVACAO${TAB}$CODIGODOEVENTO${TAB}$ESFERAORCAMENTARIA${TAB}$PROGRAMADETRABALHORESUMIDO${TAB}$FONTEDERECURSO${TAB}$NATUREZADEDESPESA${TAB}$CODIGODAUGRESPONSAVEL${TAB}$PLANOINTERNO${TAB}$VALORDATRANSACAO${TAB}$CODIGODAUGDODOC${TAB}$CODIGODAGESTAODODOC${TAB}$MESDELANCAMENTO${TAB}$SALDO${TAB}$CANCELAMENTOPE${TAB}$PEORIGINAL${TAB}$PECANCELAMENTO${TAB}$ITOPCAMBIAL >> ${FILE}.sql;
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