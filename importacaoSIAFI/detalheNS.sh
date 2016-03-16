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
echo  "COPY siafi.ns(cpf_usuario, terminal_usuario, data_transacao, hora_transacao, codigo_ug_operador, numero_ns, data_emissao, data_valorizacao, titulo_credito, data_vencimento, operacao_cambial, inversao_saldo_doc, observacao, favorecido, codigo_favorecido, codigo_evento_01, codigo_evento_02, codigo_evento_03, codigo_evento_04, codigo_evento_05, codigo_evento_06, codigo_evento_07, codigo_evento_08, codigo_evento_09, codigo_evento_10, codigo_evento_11, codigo_evento_12, codigo_inscricao1_01, codigo_inscricao1_02, codigo_inscricao1_03, codigo_inscricao1_04, codigo_inscricao1_05, codigo_inscricao1_06, codigo_inscricao1_07, codigo_inscricao1_08, codigo_inscricao1_09, codigo_inscricao1_10, codigo_inscricao1_11, codigo_inscricao1_12, codigo_inscricao2_01, codigo_inscricao2_02, codigo_inscricao2_03, codigo_inscricao2_04, codigo_inscricao2_05, codigo_inscricao2_06, codigo_inscricao2_07, codigo_inscricao2_08, codigo_inscricao2_09, codigo_inscricao2_10, codigo_inscricao2_11, codigo_inscricao2_12, classificacao1_01, classificacao1_02, classificacao1_03, classificacao1_04, classificacao1_05, classificacao1_06, classificacao1_07, classificacao1_08, classificacao1_09, classificacao1_10, classificacao1_11, classificacao1_12, classificacao2_01, classificacao2_02, classificacao2_03, classificacao2_04, classificacao2_05, classificacao2_06, classificacao2_07, classificacao2_08, classificacao2_09, classificacao2_10, classificacao2_11, classificacao2_12, debito_credito_01, debito_credito_02, debito_credito_03, debito_credito_04, debito_credito_05, debito_credito_06, debito_credito_07, debito_credito_08, debito_credito_09, debito_credito_10, debito_credito_11, debito_credito_12, codigo_conta_01, codigo_conta_02, codigo_conta_03, codigo_conta_04, codigo_conta_05, codigo_conta_06, codigo_conta_07, codigo_conta_08, codigo_conta_09, codigo_conta_10, codigo_conta_11, codigo_conta_12, conta_corrente_contabil_01, conta_corrente_contabil_02, conta_corrente_contabil_03, conta_corrente_contabil_04, conta_corrente_contabil_05, conta_corrente_contabil_06, conta_corrente_contabil_07, conta_corrente_contabil_08, conta_corrente_contabil_09, conta_corrente_contabil_10, conta_corrente_contabil_11, conta_corrente_contabil_12, valor_transacao_01, valor_transacao_02, valor_transacao_03, valor_transacao_04, valor_transacao_05, valor_transacao_06, valor_transacao_07, valor_transacao_08, valor_transacao_09, valor_transacao_10, valor_transacao_11, valor_transacao_12, tipo_nota_lancamento_sistema, opcao_lancamento, mes_lancamento, codigo_sistema_origem, apuracao_resultado, gr_ug_gestao_cancelamento, gr_an_nu_ob_cancelamento, va_evento_sistema, it_co_finalidade_spb, it_co_operacao_spb, it_nu_operacao_spb, it_co_conta_ajuste, it_nu_lista_nssaldobt)   FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^NS*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
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

NUMERODANS=${LINHA:37:23};
	if [ -z $NUMERODANS ]; then
	NUMERODANS=${NULO};
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
	if [ -z $TITULODOCREDITO ]; then
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
		strinCodigoEvento=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=6;
		strinCodigoEvento='';
		while [ $n -le 11 ]; do
			campo=${CODIGODOEVENTOOBS:$x:$y};
			if [ -z $campo ]; then
				strinCodigoEvento=${strinCodigoEvento}${NULO}'\t'
			else
				strinCodigoEvento=${strinCodigoEvento}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CODIGODAINSCRICAO1OBS=${LINHA:428:168};
	if [ -z "$CODIGODAINSCRICAO1OBS" ]; then
	strinCodigoInscricao1=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=14;
		strinCodigoInscricao1='';
		while [ $n -le 11 ]; do
			campo=${CODIGODAINSCRICAO:$x:$y};
			if [ -z $campo ]; then
				strinCodigoInscricao1=${strinCodigoInscricao1}${NULO}'\t'
			else
				strinCodigoInscricao1=${strinCodigoInscricao1}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done

	fi

CODIGODAINSCRICAO2OBS=${LINHA:596:168};
	if [ -z "$CODIGODAINSCRICAO2OBS" ]; then
	strinCodigoInscricao2=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=14;
		strinCodigoInscricao2='';
		while [ $n -le 11 ]; do
			campo=${CODIGODAINSCRICAO2OBS:$x:$y};
			if [ -z "$campo" ]; then
				strinCodigoInscricao2=${strinCodigoInscricao2}${NULO}'\t'
			else
				strinCodigoInscricao2=${strinCodigoInscricao2}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CLASSIFICACAO1___OBS=${LINHA:764:108};
	if [ -z $CLASSIFICACAO1___OBS ]; then
	strinClassificacao1=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=9;
		strinClassificacao1='';
		while [ $n -le 11 ]; do
			campo=${CLASSIFICACAO1___OBS:$x:$y};
			if [ -z $campo ]; then
				strinClassificacao1=${strinClassificacao1}${NULO}'\t'
			else
				strinClassificacao1=${strinClassificacao1}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CLASSIFICACAO2___OBS=${LINHA:872:108};
	if [ -z $CLASSIFICACAO2___OBS ]; then
	strinClassificacao2=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=9;
		strinClassificacao2='';
		while [ $n -le 11 ]; do
			campo=${CLASSIFICACAO2___OBS:$x:$y};
			if [ -z $campo ]; then
				strinClassificacao2=${strinClassificacao2}${NULO}'\t'
			else
				strinClassificacao2=${strinClassificacao2}${campo}'\t'
			fi

		x=$x+$y;
		let n++;
		done
	fi

DEBITOCREDITO__OBS=${LINHA:980:12};
	if [ -z $DEBITOCREDITO__OBS ]; then
	strinDebitoCredito=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=1;
		strinDebitoCredito='';
		while [ $n -le 11 ]; do
			campo=${DEBITOCREDITO__OBS:$x:$y};
			if [ -z $campo ]; then
				strinDebitoCredito=${strinDebitoCredito}${NULO}'\t'
			else
				strinDebitoCredito=${strinDebitoCredito}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done

	fi

CODIGOCONTA__OBS=${LINHA:992:108};
	if [ -z $CODIGOCONTA__OBS ]; then
	strinCodigoConta=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=9;
		strinCodigoConta='';
		while [ $n -le 11 ]; do
			campo=${CODIGOCONTA__OBS:$x:$y};
			if [ -z $campo ]; then
				strinCodigoConta=${strinCodigoConta}${NULO}'\t'
			else
				strinCodigoConta=${strinCodigoConta}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done

	fi

CONTACORRENTECONTABIL__OBS=${LINHA:1100:516};
	if [ -z "$CONTACORRENTECONTABIL__OBS" ]; then
	strinContaCorrenteContabil=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=43;
		strinContaCorrenteContabil='';
		while [ $n -le 11 ]; do
			campo=${CONTACORRENTECONTABIL__OBS:$x:$y};
			if [ -z "$campo" ]; then
				strinContaCorrenteContabil=${strinContaCorrenteContabil}${NULO}'\t'
			else
				strinContaCorrenteContabil=${strinContaCorrenteContabil}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done

	fi

VALORDATRANSACAO_OBS=${LINHA:1616:204};
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

TIPODANOTADELANCAMENTODESISTEMA=${LINHA:1820:2};
	if [ -z $TIPODANOTADELANCAMENTODESISTEMA ]; then
	TIPODANOTADELANCAMENTODESISTEMA=${NULO};
	fi

OPCAODELANCAMENTO=${LINHA:1822:1};
	if [ -z $OPCAODELANCAMENTO ]; then
	OPCAODELANCAMENTO=${NULO};
	fi

MESDELANCAMENTO=${LINHA:1823:2};
	if [ -z $MESDELANCAMENTO ]; then
	MESDELANCAMENTO=${NULO};
	fi

CODIGODOSISTEMADEORIGEM=${LINHA:1825:10};
	if [ -z $CODIGODOSISTEMADEORIGEM ]; then
	CODIGODOSISTEMADEORIGEM=${NULO};
	fi

APURACAODORESULTADO=${LINHA:1835:1};
	if [ -z $APURACAODORESULTADO ]; then
	APURACAODORESULTADO=${NULO};
	fi

GRUGGESTAOCANCELAMENTO=${LINHA:1836:11};
	if [ -z $GRUGGESTAOCANCELAMENTO ]; then
	GRUGGESTAOCANCELAMENTO=${NULO};
	fi

GRANNUOBCANCELAMENTO=${LINHA:1847:12};
	if [ -z $GRANNUOBCANCELAMENTO ]; then
	GRANNUOBCANCELAMENTO=${NULO};
	fi

VAEVENTOSISTEMA=${LINHA:1859:17};
	if [ -z $VAEVENTOSISTEMA ]; then
	VAEVENTOSISTEMA=${NULO};
	else
	VAEVENTOSISTEMA=${LINHA:1859:15}"."${LINHA:1874:2}
	fi

ITCOFINALIDADESPB=${LINHA:1876:3};
	if [ -z $ITCOFINALIDADESPB ]; then
	ITCOFINALIDADESPB=${NULO};
	fi

ITCOOPERACAOSPB=${LINHA:1879:3};
	if [ -z $ITCOOPERACAOSPB ]; then
	ITCOOPERACAOSPB=${NULO};
	fi

ITNUOPERACAOSPB=${LINHA:1882:23};
	if [ -z $ITNUOPERACAOSPB ]; then
	ITNUOPERACAOSPB=${NULO};
	fi

ITCOCONTAAJUSTE=${LINHA:1905:9};
	if [ -z $ITCOCONTAAJUSTE ]; then
	ITCOCONTAAJUSTE=${NULO};
	fi

ITNULISTANSSALDOBT=${LINHA:1914:23};
	if [ -z $ITNULISTANSSALDOBT ]; then
	ITNULISTANSSALDOBT=${NULO};
	fi

echo -e $CPFDOUSUARIO${TAB}$TERMINALDOUSUARIO${TAB}$DATADETRANSACAO${TAB}$HORATRANSACAO${TAB}$CODIGODAUGDOOPERADOR${TAB}$NUMERODANS${TAB}$DATADEEMISSAO${TAB}$DATAVALORIZACAO${TAB}$TITULODOCREDITO${TAB}$DATADOVENCIMENTO${TAB}$OPERACAOCAMBIAL${TAB}$INVERSAOSALDODOC${TAB}$OBSERVACAO${TAB}$FAVORECIDO${TAB}$CODIGODOFAVORECIDO${TAB}${strinCodigoEvento}${strinCodigoInscricao1}${strinCodigoInscricao2}${strinClassificacao2}${strinClassificacao2}${strinDebitoCredito}${strinCodigoConta}${strinContaCorrenteContabil}${strinValorTransacao}$TIPODANOTADELANCAMENTODESISTEMA${TAB}$OPCAODELANCAMENTO${TAB}$MESDELANCAMENTO${TAB}$CODIGODOSISTEMADEORIGEM${TAB}$APURACAODORESULTADO${TAB}$GRUGGESTAOCANCELAMENTO${TAB}$GRANNUOBCANCELAMENTO${TAB}$VAEVENTOSISTEMA${TAB}$ITCOFINALIDADESPB${TAB}$ITCOOPERACAOSPB${TAB}$ITNUOPERACAOSPB${TAB}$ITCOCONTAAJUSTE${TAB}$ITNULISTANSSALDOBT >> ${FILE}.sql;
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