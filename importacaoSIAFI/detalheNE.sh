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
echo  "COPY siafi.ne(cpf_usuario, terminal_usuario, data_transacao, hora_transacao, codigo_ug_operador, numero_ne, numero_doc_referencia, data_emissao, tipo_favorecido, codigo_favorecido, observacao, codigo_evento, esfera_orcamentaria, ptres, fonte_recurso, natureza_despesa, codigo_ug_responsavel, plano_interno, valor_transacao, modalidade_licitacao, tipo_ne, referencia_dispensa, origem_material, numero_processo, uf_beneficiada, municipio_beneficiado, inciso, amparo_legal, codigo_ug_doc_referencia, codigo_gestao_doc_referencia, controle_emissao, mes_lancamento, sistema_origem, indentificador_ne_contra_entrega, situacao_credor_sicaf, data_vencimento_01, data_vencimento_02, data_vencimento_03, data_vencimento_04, data_vencimento_05, data_vencimento_06, data_vencimento_07, data_vencimento_08, data_vencimento_09, data_vencimento_10, data_vencimento_11, data_vencimento_12, data_vencimento_13, data_vencimento_14, data_vencimento_15, data_vencimento_16, data_vencimento_17, data_vencimento_18, data_vencimento_19, data_vencimento_20, data_vencimento_21, data_vencimento_22, data_vencimento_23, data_vencimento_24, data_vencimento_25, data_vencimento_26, data_vencimento_27, data_vencimento_28, data_vencimento_29, data_vencimento_30, data_vencimento_31, data_vencimento_32, data_vencimento_33, data_vencimento_34, data_vencimento_35, data_vencimento_36, data_pagamento_01, data_pagamento_02, data_pagamento_03, data_pagamento_04, data_pagamento_05, data_pagamento_06, data_pagamento_07, data_pagamento_08, data_pagamento_09, data_pagamento_10, data_pagamento_11, data_pagamento_12, data_pagamento_13, data_pagamento_14, data_pagamento_15, data_pagamento_16, data_pagamento_17, data_pagamento_18, data_pagamento_19, data_pagamento_20, data_pagamento_21, data_pagamento_22, data_pagamento_23, data_pagamento_24, data_pagamento_25, data_pagamento_26, data_pagamento_27, data_pagamento_28, data_pagamento_29, data_pagamento_30, data_pagamento_31, data_pagamento_32, data_pagamento_33, data_pagamento_34, data_pagamento_35, data_pagamento_36, valor_cronogramado_01, valor_cronogramado_02, valor_cronogramado_03, valor_cronogramado_04, valor_cronogramado_05, valor_cronogramado_06, valor_cronogramado_07, valor_cronogramado_08, valor_cronogramado_09, valor_cronogramado_00, valor_cronogramado_11, valor_cronogramado_12, valor_cronogramado_13, valor_cronogramado_14, valor_cronogramado_15, valor_cronogramado_16, valor_cronogramado_17, valor_cronogramado_18, valor_cronogramado_19, valor_cronogramado_20, valor_cronogramado_21, valor_cronogramado_22, valor_cronogramado_23, valor_cronogramado_24, valor_cronogramado_25, valor_cronogramado_26, valor_cronogramado_27, valor_cronogramado_28, valor_cronogramado_29, valor_cronogramado_30, valor_cronogramado_31, valor_cronogramado_32, valor_cronogramado_33, valor_cronogramado_34, valor_cronogramado_35, valor_cronogramado_36, it_nu_original, it_co_msg_documento, it_nu_precatorio, it_in_pagamento_precatorio, it_nu_lista, it_in_liquidacao, it_op_cambial)   FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^NE*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
#echo ${#LINHA}
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

NUMERODANE=${LINHA:37:23};
	if [ -z $NUMERODANE ]; then
	NUMERODANE=${NULO};
	fi

NUMERODODOCREFERENCIA=${LINHA:60:12};
	if [ -z $NUMERODODOCREFERENCIA ]; then
	NUMERODODOCREFERENCIA=${NULO};
	fi

DATADEEMISSAO=${LINHA:72:8};
	if [ -z $DATADEEMISSAO ]; then
	DATADEEMISSAO=${NULO};
	else
	DATADEEMISSAO=${LINHA:72:2}"/"${LINHA:74:2}"/"${LINHA:76:4};
	fi

TIPODOFAVORECIDO=${LINHA:80:1};
	if [ -z $TIPODOFAVORECIDO ]; then
	TIPODOFAVORECIDO=${NULO};
	fi

CODIGODOFAVORECIDO=${LINHA:81:14};
	if [ -z $CODIGODOFAVORECIDO ]; then
	CODIGODOFAVORECIDO=${NULO};
	fi

OBSERVACAO=${LINHA:95:234};
	if [ -z "$OBSERVACAO" ]; then
	OBSERVACAO=${NULO};
	fi

CODIGODOEVENTO=${LINHA:329:6};
	if [ -z $CODIGODOEVENTO ]; then
	CODIGODOEVENTO=${NULO};
	fi

ESFERAORCAMENTARIA=${LINHA:335:1};
	if [ -z $ESFERAORCAMENTARIA ]; then
	ESFERAORCAMENTARIA=${NULO};
	fi

PROGRAMADETRABALHORESUMIDOPTRES=${LINHA:336:6};
	if [ -z $PROGRAMADETRABALHORESUMIDOPTRES ]; then
	PROGRAMADETRABALHORESUMIDOPTRES=${NULO};
	fi

FONTEDERECURSO=${LINHA:342:10};
	if [ -z $FONTEDERECURSO ]; then
	FONTEDERECURSO=${NULO};
	fi

NATUREZADEDESPESA=${LINHA:352:6};
	if [ -z $NATUREZADEDESPESA ]; then
	NATUREZADEDESPESA=${NULO};
	fi

CODIGODAUGRESPONSAVEL=${LINHA:358:6};
	if [ -z $CODIGODAUGRESPONSAVEL ]; then
	CODIGODAUGRESPONSAVEL=${NULO};
	fi

PLANOINTERNO=${LINHA:364:11};
	if [ -z "$PLANOINTERNO" ]; then
	PLANOINTERNO=${NULO};
	fi

VALORDATRANSACAO=${LINHA:375:17};
	if [ -z $VALORDATRANSACAO ]; then
	VALORDATRANSACAO=${NULO};
	else
	VALORDATRANSACAO=${LINHA:375:15}"."${LINHA:390:2};
	fi

MODALIDADEDELICITACAO=${LINHA:392:2};
	if [ -z $MODALIDADEDELICITACAO ]; then
	MODALIDADEDELICITACAO=${NULO};
	fi

TIPODENE=${LINHA:394:1};
	if [ -z $TIPODENE ]; then
	TIPODENE=${NULO};
	fi

REFERENCIADADISPENSA=${LINHA:395:20};
	if [ -z "$REFERENCIADADISPENSA" ]; then
	REFERENCIADADISPENSA=${NULO};
	fi

ORIGEMMATERIAL=${LINHA:415:1};
	if [ -z $ORIGEMMATERIAL ]; then
	ORIGEMMATERIAL=${NULO};
	fi

NUMERODOPROCESSO=${LINHA:416:20};
	if [ -z "$NUMERODOPROCESSO" ]; then
	NUMERODOPROCESSO=${NULO};
	fi

UFBENEFICIADA=${LINHA:436:2};
	if [ -z $UFBENEFICIADA ]; then
	UFBENEFICIADA=${NULO};
	fi

MUNICIPIOBENEFICIADO=${LINHA:438:4};
	if [ -z $MUNICIPIOBENEFICIADO ]; then
	MUNICIPIOBENEFICIADO=${NULO};
	fi

INCISO=${LINHA:442:2};
	if [ -z $INCISO ]; then
	INCISO=${NULO};
	fi

AMPAROLEGAL=${LINHA:444:8};
	if [ -z "$AMPAROLEGAL" ]; then
	AMPAROLEGAL=${NULO};
	fi

CODIGODAUGDODOC=${LINHA:452:6};
	if [ -z $CODIGODAUGDODOC ]; then
	CODIGODAUGDODOC=${NULO};
	fi

CODIGODAGESTAODODOC=${LINHA:458:5};
	if [ -z $CODIGODAGESTAODODOC ]; then
	CODIGODAGESTAODODOC=${NULO};
	fi

CONTROLEDEEMISSAO=${LINHA:463:1};
	if [ -z $CONTROLEDEEMISSAO ]; then
	CONTROLEDEEMISSAO=${NULO};
	fi

MESDELANCAMENTO=${LINHA:464:2};
	if [ -z $MESDELANCAMENTO ]; then
	MESDELANCAMENTO=${NULO};
	fi

SISTEMADEORIGEM=${LINHA:466:10};
	if [ -z "$SISTEMADEORIGEM" ]; then
	SISTEMADEORIGEM=${NULO};
	fi

INDENTIFICADORDENEDECONTRAENTREGA=${LINHA:476:1};
	if [ -z $INDENTIFICADORDENEDECONTRAENTREGA ]; then
	INDENTIFICADORDENEDECONTRAENTREGA=${NULO};
	fi

SITUACAODOCREDORNOSICAF=${LINHA:477:2};
	if [ -z $SITUACAODOCREDORNOSICAF ]; then
	SITUACAODOCREDORNOSICAF=${NULO};
	fi

DATADEVENCIMENTO=${LINHA:479:288};
	if [ -z $DATADEVENCIMENTO ]; then
	stringDataVencimento=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=8;
		stringDataVencimento='';
		while [ $n -le 35 ]; do
			campo=${DATADEVENCIMENTO:$x:$y};
			if [ -z $campo ]; then
				stringDataVencimento=${stringDataVencimento}${NULO}'\t'
			else
				stringDataVencimento=${stringDataVencimento}${campo:0:2}"/"${campo:2:2}"/"${campo:4:4}'\t'
			fi

		let n++;
		x=$x+$y;
		done

	fi

DATADEPAGAMENTO=${LINHA:767:288};
	if [ -z $DATADEPAGAMENTO ]; then
	stringDataPagamentp=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=8;
		stringDataPagamentp='';
		while [ $n -le 35 ]; do
			campo=${DATADEPAGAMENTO:$x:$y};
			if [ -z $campo ]; then
				stringDataPagamentp=${stringDataPagamentp}${NULO}'\t'
			else
				stringDataPagamentp=${stringDataPagamentp}${campo:0:2}"/"${campo:2:2}"/"${campo:4:4}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

VALORCRONOGRAMADO=${LINHA:1055:612};
	if [ -z $VALORCRONOGRAMADO ]; then
	strinValorCronograma=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=17;
		strinValorCronograma='';
		while [ $n -le 35 ]; do
			campo=${VALORCRONOGRAMADO:$x:$y};
			if [ -z $campo ]; then
				strinValorCronograma=${strinValorCronograma}${NULO}'\t'
			else
				strinValorCronograma=${strinValorCronograma}${VALORCRONOGRAMADO:$x:15}"."${VALORCRONOGRAMADO:$x+15:2}'\t'
			fi

		let n++;
		x=$x+$y;
		done

	fi

ITNUORIGINAL=${LINHA:1667:20};
	if [ -z "$ITNUORIGINAL" ]; then
	ITNUORIGINAL=${NULO};
	fi

ITCOMSGDOCUMENTO=${LINHA:1687:3};
	if [ -z $ITCOMSGDOCUMENTO ]; then
	ITCOMSGDOCUMENTO=${NULO};
	fi

ITNUPRECATORIO=${LINHA:1690:20};
	if [ -z $ITNUPRECATORIO ]; then
	ITNUPRECATORIO=${NULO};
	fi

ITINPAGAMENTOPRECATORIO=${LINHA:1710:1};
	if [ -z $ITINPAGAMENTOPRECATORIO ]; then
	ITINPAGAMENTOPRECATORIO=${NULO};
	fi

ITNULISTA=${LINHA:1711:12};
	if [ -z $ITNULISTA ]; then
	ITNULISTA=${NULO};
	fi

ITINLIQUIDACAO=${LINHA:1723:1};
	if [ -z $ITINLIQUIDACAO ]; then
	ITINLIQUIDACAO=${NULO};
	fi

ITOPCAMBIAL=${LINHA:1724:10};
	if [ -z $ITOPCAMBIAL ]; then
	ITOPCAMBIAL=${NULO};
	fi

echo -e $CPFDOUSUARIO${TAB}$TERMINALDOUSUARIO${TAB}$DATADETRANSACAO${TAB}$HORATRANSACAO${TAB}$CODIGODAUGDOOPERADOR${TAB}$NUMERODANE${TAB}$NUMERODODOCREFERENCIA${TAB}$DATADEEMISSAO${TAB}$TIPODOFAVORECIDO${TAB}$CODIGODOFAVORECIDO${TAB}$OBSERVACAO${TAB}$CODIGODOEVENTO${TAB}$ESFERAORCAMENTARIA${TAB}$PROGRAMADETRABALHORESUMIDOPTRES${TAB}$FONTEDERECURSO${TAB}$NATUREZADEDESPESA${TAB}$CODIGODAUGRESPONSAVEL${TAB}$PLANOINTERNO${TAB}$VALORDATRANSACAO${TAB}$MODALIDADEDELICITACAO${TAB}$TIPODENE${TAB}$REFERENCIADADISPENSA${TAB}$ORIGEMMATERIAL${TAB}$NUMERODOPROCESSO${TAB}$UFBENEFICIADA${TAB}$MUNICIPIOBENEFICIADO${TAB}$INCISO${TAB}$AMPAROLEGAL${TAB}$CODIGODAUGDODOC${TAB}$CODIGODAGESTAODODOC${TAB}$CONTROLEDEEMISSAO${TAB}$MESDELANCAMENTO${TAB}$SISTEMADEORIGEM${TAB}$INDENTIFICADORDENEDECONTRAENTREGA${TAB}$SITUACAODOCREDORNOSICAF${TAB}${stringDataVencimento}${stringDataPagamentp}${strinValorCronograma}$ITNUORIGINAL${TAB}$ITCOMSGDOCUMENTO${TAB}$ITNUPRECATORIO${TAB}$ITINPAGAMENTOPRECATORIO${TAB}$ITNULISTA${TAB}$ITINLIQUIDACAO${TAB}$ITOPCAMBIAL >> ${FILE}.sql;
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