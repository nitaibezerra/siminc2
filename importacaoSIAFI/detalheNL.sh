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
echo  "COPY siafi.nl(cpf_usuario, terminal_usuario, data_transacao, hora_transacao, codigo_ug_operador, numero_nl, data_emissao, data_valorizacao, titulo_credito, data_vencimento, operacao_cambial, inversao_saldo_doc, observacao, favorecido, codigo_favorecido, codigo_evento_01, codigo_evento_02, codigo_evento_03, codigo_evento_04, codigo_evento_05, codigo_evento_06, codigo_evento_07, codigo_evento_08, codigo_evento_09, codigo_evento_10, codigo_evento_11, codigo_evento_12, codigo_inscricao1_01, codigo_inscricao1_02, codigo_inscricao1_03, codigo_inscricao1_04, codigo_inscricao1_05, codigo_inscricao1_06, codigo_inscricao1_07, codigo_inscricao1_08, codigo_inscricao1_09, codigo_inscricao1_10, codigo_inscricao1_11, codigo_inscricao1_12, codigo_inscricao2_01, codigo_inscricao2_02, codigo_inscricao2_03, codigo_inscricao2_04, codigo_inscricao2_05, codigo_inscricao2_06, codigo_inscricao2_07, codigo_inscricao2_08, codigo_inscricao2_09, codigo_inscricao2_10, codigo_inscricao2_11, codigo_inscricao2_12, classificacao1_01, classificacao1_02, classificacao1_03, classificacao1_04, classificacao1_05, classificacao1_06, classificacao1_07, classificacao1_08, classificacao1_09, classificacao1_10, classificacao1_11, classificacao1_12, classificacao2_01, classificacao2_02, classificacao2_03, classificacao2_04, classificacao2_05, classificacao2_06, classificacao2_07, classificacao2_08, classificacao2_09, classificacao2_10, classificacao2_11, classificacao2_12, valor_transacao_01, valor_transacao_02, valor_transacao_03, valor_transacao_04, valor_transacao_05, valor_transacao_06, valor_transacao_07, valor_transacao_08, valor_transacao_09, valor_transacao_10, valor_transacao_11, valor_transacao_12, mes_lancamento, codigo_sistema_origem, numero_processo, it_da_leitura_auditor_spb, it_co_operacao_spb)    FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^NL*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
CPFDOUSUARIO=${LINHA:0:11};
	if [ -z $CPFDOUSUARIO ]; then
	CPFDOUSUARIO=${NULO};
	fi

TERMINALDOUSUARIO=${LINHA:11:8};
	if [ -z $TERMINALDOUSUARIO ]; then
	TERMINALDOUSUARIO=${NULO};
	fi

DATADETRANSACAO=${LINHA:19:8};
	if [ -z $DATADETRANSACAO ]; then
	DATADETRANSACAO=${NULO};
	else
	DATADETRANSACAO=${LINHA:19:2}"/"${LINHA:21:2}"/"${LINHA:23:4};
	fi

HORATRANSACAO=${LINHA:27:4};
	if [ -z $HORATRANSACAO ]; then
	HORATRANSACAO=${NULO};
	else
	HORATRANSACAO=${LINHA:27:2}":"${LINHA:29:2};
	fi

CODIGODAUGDOOPERADOR=${LINHA:31:6};
	if [ -z $CODIGODAUGDOOPERADOR ]; then
	CODIGODAUGDOOPERADOR=${NULO};
	fi

NUMERODANL=${LINHA:37:23};
	if [ -z $NUMERODANL ]; then
	NUMERODANL=${NULO};
	fi

DATADEEMISSAO=${LINHA:60:8};
	if [ -z $DATADEEMISSAO ]; then
	DATADEEMISSAO=${NULO};
	else
	DATADEEMISSAO=${LINHA:60:2}"/"${LINHA:62:2}"/"${LINHA:64:4};
	fi

DATAVALORIZACAO=${LINHA:68:8};
	if [ -z $DATAVALORIZACAO ]; then
	DATAVALORIZACAO=${NULO};
	else
	DATAVALORIZACAO=${LINHA:68:2}"/"${LINHA:70:2}"/"${LINHA:72:4};
	fi

TITULODOCREDITO=${LINHA:76:12};
	if [ -z "$TITULODOCREDITO" ]; then
	TITULODOCREDITO=${NULO};
	fi

DATADOVENCIMENTO=${LINHA:88:8};

	if [ -z $DATADOVENCIMENTO ]; then
	DATADOVENCIMENTO=${NULO};
	else
	DATADOVENCIMENTO=${LINHA:88:2}"/"${LINHA:90:2}"/"${LINHA:92:4};
	fi

OPERACAOCAMBIAL=${LINHA:96:10};
	if [ -z $OPERACAOCAMBIAL ]; then
	OPERACAOCAMBIAL=${NULO};
	fi

INVERSAOSALDODOC=${LINHA:106:1};
	if [ -z $INVERSAOSALDODOC ]; then
	INVERSAOSALDODOC=${NULO};
	fi

OBSERVACAO=${LINHA:107:234};
	if [ -z "$OBSERVACAO" ]; then
	OBSERVACAO=${NULO};
	fi

FAVORECIDO=${LINHA:341:1};
	if [ -z $FAVORECIDO ]; then
	FAVORECIDO=${NULO};
	fi

CODIGODOFAVORECIDO=${LINHA:342:14};
	if [ -z $CODIGODOFAVORECIDO ]; then
	CODIGODOFAVORECIDO=${NULO};
	fi

CODIGODOEVENTOOBS=${LINHA:356:72};
	if [ -z $CODIGODOEVENTOOBS ]; then
	stringCodigoEvento=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=6;
		stringCodigoEvento='';
		while [ $n -le 11 ]; do
			campo=${CODIGODOEVENTOOBS:$x:$y};
			if [ -z $campo ]; then
				stringCodigoEvento=${stringCodigoEvento}${NULO}'\t'
			else
				stringCodigoEvento=${stringCodigoEvento}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CODIGODAINSCRICAO1OBS=${LINHA:428:168};
	if [ -z "$CODIGODAINSCRICAO1OBS" ]; then
	stringInscricao=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=14;
		stringInscricao='';
		while [ $n -le 11 ]; do
			campo=${CODIGODAINSCRICAO1OBS:$x:$y};
			if [ -z $campo ]; then
				stringInscricao=${stringInscricao}${NULO}'\t'
			else
				stringInscricao=${stringInscricao}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CODIGODAINSCRICAO2OBS=${LINHA:596:168};
	if [ -z "$CODIGODAINSCRICAO2OBS" ]; then
	stringInscricao2=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=14;
		stringInscricao2='';
		while [ $n -le 11 ]; do
			campo=${CODIGODAINSCRICAO2OBS:$x:$y};
			if [ -z "$campo" ]; then
				stringInscricao2=${stringInscricao2}${NULO}'\t'
			else
				stringInscricao2=${stringInscricao2}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CLASSIFICACAO1___OBS=${LINHA:764:108};
	if [ -z $CLASSIFICACAO1___OBS ]; then
	stringClassificacao1=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=9;
		stringClassificacao1='';
		while [ $n -le 11 ]; do
			campo=${CLASSIFICACAO1___OBS:$x:$y};
			if [ -z $campo ]; then
				stringClassificacao1=${stringClassificacao1}${NULO}'\t'
			else
				stringClassificacao1=${stringClassificacao1}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CLASSIFICACAO2___OBS=${LINHA:872:108};
	if [ -z $CLASSIFICACAO2___OBS ]; then
	stringClassificacao2=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=9;
		stringClassificacao2='';
		while [ $n -le 11 ]; do
			campo=${CLASSIFICACAO2___OBS:$x:$y};
			if [ -z $campo ]; then
				stringClassificacao2=${stringClassificacao2}${NULO}'\t'
			else
				stringClassificacao2=${stringClassificacao2}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

VALORDATRANSACAO_OBS=${LINHA:980:204};
	if [ -z $VALORDATRANSACAO_OBS ]; then
	strinValorTransacao=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=17;
		strinValorTransacao='';
		while [ $n -le 11 ]; do
			campo=${VALORDATRANSACAO_OBS:$x:$y};
			if [ -z $campo ]; then
				strinValorTransacao=${strinValorTransacao}${NULO}'\t'
			else
				strinValorTransacao=${strinValorTransacao}${VALORDATRANSACAO_OBS:$x:15}"."${VALORDATRANSACAO_OBS:$x+15:2}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

MESDELANCAMENTO=${LINHA:1184:2};
	if [ -z $MESDELANCAMENTO ]; then
	MESDELANCAMENTO=${NULO};
	fi

CODIGODOSISTEMADEORIGEM=${LINHA:1186:10};
	if [ -z $CODIGODOSISTEMADEORIGEM ]; then
	CODIGODOSISTEMADEORIGEM=${NULO};
	fi

NUMERODOPROCESSO=${LINHA:1196:20};
	if [ -z $NUMERODOPROCESSO ]; then
	NUMERODOPROCESSO=${NULO};
	fi

ITDALEITURAAUDITORSPB=${LINHA:1216:8};
	if [ -z $ITDALEITURAAUDITORSPB ]; then
	ITDALEITURAAUDITORSPB=${NULO};
	fi

ITCOOPERACAOSPB=${LINHA:1224:3};
	if [ -z $ITCOOPERACAOSPB ]; then
	ITCOOPERACAOSPB=${NULO};
	fi

echo -e $CPFDOUSUARIO${TAB}$TERMINALDOUSUARIO${TAB}$DATADETRANSACAO${TAB}$HORATRANSACAO${TAB}$CODIGODAUGDOOPERADOR${TAB}$NUMERODANL${TAB}$DATADEEMISSAO${TAB}$DATAVALORIZACAO${TAB}$TITULODOCREDITO${TAB}$DATADOVENCIMENTO${TAB}$OPERACAOCAMBIAL${TAB}$INVERSAOSALDODOC${TAB}$OBSERVACAO${TAB}$FAVORECIDO${TAB}$CODIGODOFAVORECIDO${TAB}${stringCodigoEvento}${stringInscricao}${stringInscricao2}${stringClassificacao1}${stringClassificacao2}${strinValorTransacao}$MESDELANCAMENTO${TAB}$CODIGODOSISTEMADEORIGEM${TAB}$NUMERODOPROCESSO${TAB}$ITDALEITURAAUDITORSPB${TAB}$ITCOOPERACAOSPB >> ${FILE}.sql;
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