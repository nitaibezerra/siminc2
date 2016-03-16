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
echo  "COPY siafi.empenhorestopagar(it_co_usuario,it_co_terminal_usuario,it_da_transacao,it_ho_transacao,it_co_ug_operacao,gr_ug_gestao_an_numero_neuq,it_da_emissao,it_in_favorecido,it_co_favorecido,it_tx_observacao,gr_codigo_evento,it_in_esfera_orcamentaria,it_co_programa_trab_resumido,gr_fonte_recurso,gr_natureza_despesa,it_co_ug_responsavel,it_co_plano_interno,it_va_transacao,it_in_modalidade_licitacao,it_in_empenho,it_tx_referencia_dispensa,it_origem_material,it_num_processo,co_uf_beneficiada,it_co_municipio_beneficiado,it_co_inciso,it_tx_amparo_legal,gr_an_nu_documento_referencia,it_in_emissao,it_me_lancamento,it_va_cronograma_1,it_va_cronograma_2,it_va_cronograma_3,it_va_cronograma_4,it_va_cronograma_5,it_va_cronograma_6,it_va_cronograma_7,it_va_cronograma_8,it_va_cronograma_9,it_va_cronograma_10,it_va_cronograma_11,it_va_cronograma_12,it_co_ug_doc_referencia,it_co_gestao_doc_referencia,it_in_contra_entrega_ne,it_in_situacao_credor_sicaf,it_da_vencimento_1,it_da_vencimento_2,it_da_vencimento_3,it_da_vencimento_4,it_da_vencimento_5,it_da_vencimento_6,it_da_vencimento_7,it_da_vencimento_8,it_da_vencimento_9,it_da_vencimento_10,it_da_pagamento_1,it_da_pagamento_2,it_da_pagamento_3,it_da_pagamento_4,it_da_pagamento_5,it_da_pagamento_6,it_da_pagamento_7,it_da_pagamento_8,it_da_pagamento_9,it_da_pagamento_10,it_da_pagamento_11,it_da_pagamento_12,it_va_cronogramado_1,it_va_cronogramado_2,it_va_cronogramado_3,it_va_cronogramado_4,it_va_cronogramado_5,it_va_cronogramado_6,it_va_cronogramado_7,it_va_cronogramado_8,it_va_cronogramado_9,it_va_cronogramado_10,it_va_cronogramado_11,it_va_cronogramado_12,it_co_msg_documento,it_in_pagamento_precatorio,it_nu_precatorio,it_nu_original,it_da_atualizacao,it_in_atualizacao,it_nu_lista_1,it_nu_lista_2,it_nu_lista_3,it_nu_lista_4,it_nu_lista_5,it_nu_lista_6,it_nu_lista_7,it_nu_lista_8,it_nu_lista_9,it_nu_lista_10,it_in_liquidacao,it_co_sistema_origem,it_di_cronograma_1,it_di_cronograma_2,it_di_cronograma_3,it_di_cronograma_4,it_di_cronograma_5,it_di_cronograma_6,it_di_cronograma_7,it_di_cronograma_8,it_di_cronograma_9,it_di_cronograma_10,it_di_cronograma_11,it_di_cronograma_12,it_qt_lancamento) FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^RP*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
	#00001 00011  NUM     IT-CO-USUARIO
	CAMPO01=${LINHA:0:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	#00012 00008  NUM     IT-CO-TERMINAL-USUARIO
	CAMPO02=${LINHA:11:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	#00020 00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
	CAMPO03=${LINHA: 24: 4}${LINHA:  22: 2}${LINHA:  20: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	#00028 00004  NUM     IT-HO-TRANSACAO
	CAMPO04=${LINHA:27:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	#00032 00006  ALFANUM IT-CO-UG-OPERADOR
	CAMPO05=${LINHA:31:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
	#00038 00023  NUM     GR-UG-GESTAO-AN-NUMERO-NEUQ
	CAMPO06=${LINHA:37:23}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	
	#00061 00008  NUM     IT-DA-EMISSAO
	CAMPO07=${LINHA:60:8}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	
	#00069 00001  NUM     IT-IN-FAVORECIDO
	CAMPO08=${LINHA:68:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	
	#00070 00014  NUM     IT-CO-FAVORECIDO
	CAMPO09=${LINHA:69:14}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	
	#00084 00234  NUM     IT-TX-OBSERVACAO
	CAMPO10=${LINHA:83:234}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
	#00318 00006  NUM     GR-CODIGO-EVENTO
	CAMPO11=${LINHA:317:6}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	
	#00324 00001  NUM     IT-IN-ESFERA-ORCAMENTARIA
	CAMPO12=${LINHA:323:1}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
	
	#00325 00006  NUM     IT-CO-PROGRAMA-TRABALHO-RESUMIDO
	CAMPO13=${LINHA:324:6}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
	
	#00331 00010  NUM     GR-FONTE-RECURSO
	CAMPO14=${LINHA:330:10}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
	
	#00341 00006  NUM     GR-NATUREZA-DESPESA
	CAMPO15=${LINHA:340:6}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
	
	#00347 00006  NUM     IT-CO-UG-RESPONSAVEL
	CAMPO16=${LINHA:346:6}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
	
	#00353 00011  ALFANUM IT-CO-PLANO-INTERNO
	CAMPO17=${LINHA:352:11}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
	
	#00364 00017  NUM     IT-VA-TRANSACAO (N15,2)
	CAMPO18=${LINHA:363:17}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi
	
	#00381 00002  NUM     IT-IN-MODALIDADE-LICITACAO
	CAMPO19=${LINHA:380:2}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi
	
	#00383 00001  NUM     IT-IN-EMPENHO
	CAMPO20=${LINHA:382:1}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
	
	#00384 00020  ALFANUM IT-TX-REFERENCIA-DISPENSA
	CAMPO21=${LINHA:383:20}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
	
	#00404 00001  NUM     IT-IN-ORIGEM-MATERIAL
	CAMPO22=${LINHA:403:1}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi
	
	#00405 00020  ALFANUM IT-NU-PROCESSO
	CAMPO23=${LINHA:404:20}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi
	
	#00425 00002  ALFANUM IT-CO-UF-BENEFICIADA
	CAMPO24=${LINHA:424:2}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi
	
	#00427 00004  NUM     IT-CO-MUNICIPIO-BENEFICIADO
	CAMPO25=${LINHA:426:4}; if [ -z "$CAMPO25" ]; then CAMPO25=${NULO}; fi
	
	#00431 00002  ALFANUM IT-CO-INCISO
	CAMPO26=${LINHA:430:2}; if [ -z "$CAMPO26" ]; then CAMPO26=${NULO}; fi
	
	#00433 00008  ALFANUM IT-TX-AMPARO-LEGAL
	CAMPO27=${LINHA:432:8}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
	
	#00441 00012  ALFANUM GR-AN-NU-DOCUMENTO-REFERENCIA
	CAMPO28=${LINHA:440:12}; if [ -z "$CAMPO28" ]; then CAMPO28=${NULO}; fi
	
	#00453 00001  NUM     IT-IN-EMISSAO
	CAMPO29=${LINHA:452:1}; if [ -z "$CAMPO29" ]; then CAMPO29=${NULO}; fi
	
	#00454 00002  NUM     IT-ME-LANCAMENTO
	CAMPO30=${LINHA:453:2}; if [ -z "$CAMPO30" ]; then CAMPO30=${NULO}; fi
	
	#00456 00204  NUM     IT-VA-CRONOGRAMA - 12 VEZES DE (N15,2)
			## 31_1
			CAMPO31_1=${LINHA:455:17};
			if [ -z "$CAMPO31_1" ]; then
				CAMPO31_1=${NULO};
			else
				stValor=CAMPO31_1;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_1="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_1=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			
			## 31_2
			CAMPO31_2=${LINHA:471:17};
			if [ -z "$CAMPO31_2" ]; then
				CAMPO31_2=${NULO};
			else
				stValor=CAMPO31_2;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_2="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_2=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_3
			CAMPO31_3=${LINHA:487:17};
			if [ -z "$CAMPO31_3" ]; then
				CAMPO31_3=${NULO};
			else
				stValor=CAMPO31_3;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_3="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_3=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_4
			CAMPO31_4=${LINHA:504:17};
			if [ -z "$CAMPO31_4" ]; then
				CAMPO31_4=${NULO};
			else
				stValor=CAMPO31_4;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_4="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_4=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_5
			CAMPO31_5=${LINHA:521:17};
			if [ -z "$CAMPO31_5" ]; then
				CAMPO31_5=${NULO};
			else
				stValor=CAMPO31_5;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_5="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_5=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_6
			CAMPO31_6=${LINHA:536:17};
			if [ -z "$CAMPO31_2" ]; then
				CAMPO31_6=${NULO};
			else
				stValor=CAMPO31_6;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_6="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_6=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_7
			CAMPO31_7=${LINHA:553:17};
			if [ -z "$CAMPO31_7" ]; then
				CAMPO31_7=${NULO};
			else
				stValor=CAMPO31_7;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_7="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_7=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_8
			CAMPO31_8=${LINHA:568:17};
			if [ -z "$CAMPO31_8" ]; then
				CAMPO31_8=${NULO};
			else
				stValor=CAMPO31_8;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_8="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_8=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_9
			CAMPO31_9=${LINHA:585:17};
			if [ -z "$CAMPO31_9" ]; then
				CAMPO31_9=${NULO};
			else
				stValor=CAMPO31_9;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_9="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_9=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_10
			CAMPO31_10=${LINHA:600:17};
			if [ -z "$CAMPO31_10" ]; then
				CAMPO31_10=${NULO};
			else
				stValor=CAMPO31_10;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_10="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_10=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_11
			CAMPO31_11=${LINHA:616:17};
			if [ -z "$CAMPO31_11" ]; then
				CAMPO31_11=${NULO};
			else
				stValor=CAMPO31_11;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_11="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_11=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 31_12
			CAMPO31_12=${LINHA:633:17};
			if [ -z "$CAMPO31_12" ]; then
				CAMPO31_12=${NULO};
			else
				stValor=CAMPO31_12;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO31_12="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO31_12=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
		
	
	#00660 00006  NUM     IT-CO-UG-DOC-REFERENCIA
	CAMPO31=${LINHA:659:6}; if [ -z "$CAMPO31" ]; then CAMPO31=${NULO}; fi
	
	#00666 00005  NUM     IT-CO-GESTAO-DOC-REFERENCIA
	CAMPO32=${LINHA:665:5}; if [ -z "$CAMPO32" ]; then CAMPO32=${NULO}; fi
	
	#00671 00001  NUM     IT-IN-CONTRA-ENTREGA-NE
	CAMPO33=${LINHA:670:1}; if [ -z "$CAMPO33" ]; then CAMPO33=${NULO}; fi
	
	#00672 00002  NUM     IT-IN-SITUACAO-CREDOR-SICAF
	CAMPO34=${LINHA:671:2}; if [ -z "$CAMPO34" ]; then CAMPO34=${NULO}; fi
	
	#00674 00096  NUM     IT-DA-VENCIMENTO - 12 DE (N8) (DDMMAAAA)
		CAMPO35_1=${LINHA:677:4}${LINHA:675:2}${LINHA:673:2}; if [ -z "$CAMPO35_1" ]; then CAMPO35_1=${NULO}; fi
		CAMPO35_2=${LINHA:684:4}${LINHA:682:2}${LINHA:680:2}; if [ -z "$CAMPO35_2" ]; then CAMPO35_2=${NULO}; fi
		CAMPO35_3=${LINHA:691:4}${LINHA:689:2}${LINHA:687:2}; if [ -z "$CAMPO35_3" ]; then CAMPO35_3=${NULO}; fi
		CAMPO35_4=${LINHA:698:4}${LINHA:696:2}${LINHA:694:2}; if [ -z "$CAMPO35_4" ]; then CAMPO35_4=${NULO}; fi
		CAMPO35_5=${LINHA:705:4}${LINHA:703:2}${LINHA:701:2}; if [ -z "$CAMPO35_5" ]; then CAMPO35_5=${NULO}; fi
		CAMPO35_6=${LINHA:712:4}${LINHA:710:2}${LINHA:708:2}; if [ -z "$CAMPO35_6" ]; then CAMPO35_6=${NULO}; fi
		CAMPO35_7=${LINHA:719:4}${LINHA:717:2}${LINHA:715:2}; if [ -z "$CAMPO35_7" ]; then CAMPO35_7=${NULO}; fi
		CAMPO35_8=${LINHA:726:4}${LINHA:724:2}${LINHA:722:2}; if [ -z "$CAMPO35_8" ]; then CAMPO35_8=${NULO}; fi
		CAMPO35_9=${LINHA:723:4}${LINHA:731:2}${LINHA:729:2}; if [ -z "$CAMPO35_9" ]; then CAMPO35_0=${NULO}; fi
		CAMPO35_10=${LINHA:730:4}${LINHA:728:2}${LINHA:726:2}; if [ -z "$CAMPO35_10" ]; then CAMPO35_10=${NULO}; fi
		CAMPO35_11=${LINHA:737:4}${LINHA:735:2}${LINHA:733:2}; if [ -z "$CAMPO35_11" ]; then CAMPO35_11=${NULO}; fi
		CAMPO35_12=${LINHA:744:4}${LINHA:742:2}${LINHA:740:2}; if [ -z "$CAMPO35_12" ]; then CAMPO35_12=${NULO}; fi
		
	#00770 00096  NUM     IT-DA-PAGAMENTO - 12 DE (N8) (DDMMAAAA)
		CAMPO36_1=${LINHA:773:4}${LINHA:771:2}${LINHA:769:2}; if [ -z "$CAMPO36_1" ]; then CAMPO36_1=${NULO}; fi
		CAMPO36_2=${LINHA:780:4}${LINHA:778:2}${LINHA:776:2}; if [ -z "$CAMPO36_2" ]; then CAMPO36_2=${NULO}; fi
		CAMPO36_3=${LINHA:787:4}${LINHA:785:2}${LINHA:783:2}; if [ -z "$CAMPO36_3" ]; then CAMPO36_3=${NULO}; fi
		CAMPO36_4=${LINHA:794:4}${LINHA:792:2}${LINHA:790:2}; if [ -z "$CAMPO36_4" ]; then CAMPO36_4=${NULO}; fi
		CAMPO36_5=${LINHA:801:4}${LINHA:799:2}${LINHA:797:2}; if [ -z "$CAMPO36_5" ]; then CAMPO36_5=${NULO}; fi
		CAMPO36_6=${LINHA:808:4}${LINHA:806:2}${LINHA:804:2}; if [ -z "$CAMPO36_6" ]; then CAMPO36_6=${NULO}; fi
		CAMPO36_7=${LINHA:815:4}${LINHA:813:2}${LINHA:811:2}; if [ -z "$CAMPO36_7" ]; then CAMPO36_7=${NULO}; fi
		CAMPO36_8=${LINHA:822:4}${LINHA:820:2}${LINHA:818:2}; if [ -z "$CAMPO36_8" ]; then CAMPO36_8=${NULO}; fi
		CAMPO36_9=${LINHA:829:4}${LINHA:827:2}${LINHA:825:2}; if [ -z "$CAMPO36_9" ]; then CAMPO36_0=${NULO}; fi
		CAMPO36_10=${LINHA:836:4}${LINHA:834:2}${LINHA:832:2}; if [ -z "$CAMPO36_10" ]; then CAMPO36_10=${NULO}; fi
		CAMPO36_11=${LINHA:844:4}${LINHA:842:2}${LINHA:839:2}; if [ -z "$CAMPO36_11" ]; then CAMPO36_11=${NULO}; fi
		CAMPO36_12=${LINHA:852:4}${LINHA:849:2}${LINHA:847:2}; if [ -z "$CAMPO36_12" ]; then CAMPO36_12=${NULO}; fi
	
	#00866 00204  NUM     IT-VA-CRONOGRAMADO - 12 VEZES DE (N15,2)
			## 37_1
			CAMPO37_1=${LINHA:455:17};
			if [ -z "$CAMPO37_1" ]; then
				CAMPO37_1=${NULO};
			else
				stValor=CAMPO37_1;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_1="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_1=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			
			## 37_2
			CAMPO37_2=${LINHA:471:17};
			if [ -z "$CAMPO37_2" ]; then
				CAMPO37_2=${NULO};
			else
				stValor=CAMPO37_2;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_2="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_2=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_3
			CAMPO37_3=${LINHA:487:17};
			if [ -z "$CAMPO37_3" ]; then
				CAMPO37_3=${NULO};
			else
				stValor=CAMPO37_3;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_3="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_3=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_4
			CAMPO37_4=${LINHA:504:17};
			if [ -z "$CAMPO37_4" ]; then
				CAMPO37_4=${NULO};
			else
				stValor=CAMPO37_4;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_4="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_4=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_5
			CAMPO37_5=${LINHA:521:17};
			if [ -z "$CAMPO37_5" ]; then
				CAMPO37_5=${NULO};
			else
				stValor=CAMPO37_5;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_5="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_5=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_6
			CAMPO37_6=${LINHA:536:17};
			if [ -z "$CAMPO37_2" ]; then
				CAMPO37_6=${NULO};
			else
				stValor=CAMPO37_6;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_6="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_6=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_7
			CAMPO37_7=${LINHA:553:17};
			if [ -z "$CAMPO37_7" ]; then
				CAMPO37_7=${NULO};
			else
				stValor=CAMPO37_7;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_7="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_7=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_8
			CAMPO37_8=${LINHA:568:17};
			if [ -z "$CAMPO37_8" ]; then
				CAMPO37_8=${NULO};
			else
				stValor=CAMPO37_8;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_8="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_8=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_9
			CAMPO37_9=${LINHA:585:17};
			if [ -z "$CAMPO37_9" ]; then
				CAMPO37_9=${NULO};
			else
				stValor=CAMPO37_9;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_9="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_9=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_10
			CAMPO37_10=${LINHA:600:17};
			if [ -z "$CAMPO37_10" ]; then
				CAMPO37_10=${NULO};
			else
				stValor=CAMPO37_10;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_10="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_10=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_11
			CAMPO37_11=${LINHA:616:17};
			if [ -z "$CAMPO37_11" ]; then
				CAMPO37_11=${NULO};
			else
				stValor=CAMPO37_11;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_11="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_11=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
			## 37_12
			CAMPO37_12=${LINHA:633:17};
			if [ -z "$CAMPO37_12" ]; then
				CAMPO37_12=${NULO};
			else
				stValor=CAMPO37_12;
				valor=${stValor:16:1};
				contasNum $valor;
				if  [ $operadorValor -ne 99 ]; then
					CAMPO37_12="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
				else
					CAMPO37_12=${stValor:0:15}"."${stValor:16:2}
				fi
			fi
	
	#01070 00003  NUM     IT-CO-MSG-DOCUMENTO
	CAMPO38=${LINHA:1069:3}; if [ -z "$CAMPO38" ]; then CAMPO38=${NULO}; fi
	
	#01073 00001  ALFANUM IT-IN-PAGAMENTO-PRECATORIO
	CAMPO39=${LINHA:1072:1}; if [ -z "$CAMPO39" ]; then CAMPO39=${NULO}; fi
	
	#01074 00020  ALFANUM IT-NU-PRECATORIO
	CAMPO40=${LINHA:1073:20}; if [ -z "$CAMPO40" ]; then CAMPO40=${NULO}; fi
	
	#01094 00020  ALFANUM IT-NU-ORIGINAL
	CAMPO41=${LINHA:1093:20}; if [ -z "$CAMPO41" ]; then CAMPO41=${NULO}; fi
	
	#01114 00008  NUM     IT-DA-ATUALIZACAO (DDMMAAAA)
	CAMPO42=${LINHA: 1117: 4}${LINHA:  1115: 2}${LINHA:  1113: 2}; if [ -z "$CAMPO42" ]; then CAMPO42=${NULO}; fi
	
	#01122 00001  NUM     IT-IN-ATUALIZACAO
	CAMPO43=${LINHA:1121:1}; if [ -z "$CAMPO43" ]; then CAMPO43=${NULO}; fi
	
	#01123 00120  ALFANUM IT-NU-LISTA - 10 VEZES DE 12 BYTES
		CAMPO44_1=${LINHA:1122:12}; if [ -z "$CAMPO44_1" ]; then CAMPO44_1=${NULO}; fi
		CAMPO44_2=${LINHA:1133:12}; if [ -z "$CAMPO44_2" ]; then CAMPO44_2=${NULO}; fi
		CAMPO44_3=${LINHA:1144:12}; if [ -z "$CAMPO44_3" ]; then CAMPO44_3=${NULO}; fi
		CAMPO44_4=${LINHA:1155:12}; if [ -z "$CAMPO44_4" ]; then CAMPO44_4=${NULO}; fi
		CAMPO44_5=${LINHA:1166:12}; if [ -z "$CAMPO44_5" ]; then CAMPO44_5=${NULO}; fi
		CAMPO44_6=${LINHA:1177:12}; if [ -z "$CAMPO44_6" ]; then CAMPO44_6=${NULO}; fi
		CAMPO44_7=${LINHA:1188:12}; if [ -z "$CAMPO44_7" ]; then CAMPO44_7=${NULO}; fi
		CAMPO44_8=${LINHA:1199:12}; if [ -z "$CAMPO44_8" ]; then CAMPO44_8=${NULO}; fi
		CAMPO44_9=${LINHA:1210:12}; if [ -z "$CAMPO44_9" ]; then CAMPO44_9=${NULO}; fi
		CAMPO44_10=${LINHA:1221:12}; if [ -z "$CAMPO44_10" ]; then CAMPO44_10=${NULO}; fi
	
	#01243 00001  NUM     IT-IN-LIQUIDACAO
	CAMPO45=${LINHA:1242:1}; if [ -z "$CAMPO45" ]; then CAMPO45=${NULO}; fi
	
	#01244 00010  ALFANUM IT-CO-SISTEMA-ORIGEM
	CAMPO46=${LINHA:1243:10}; if [ -z "$CAMPO46" ]; then CAMPO46=${NULO}; fi
	
	#01254 00024  NUM     IT-DI-CRONOGRAMA - 12 VEZES DE 2 BYTES
		CAMPO47_1=${LINHA:1253:2}; if [ -z "$CAMPO47_1" ]; then CAMPO47_1=${NULO}; fi
		CAMPO47_2=${LINHA:1255:2}; if [ -z "$CAMPO47_2" ]; then CAMPO47_2=${NULO}; fi
		CAMPO47_3=${LINHA:1257:2}; if [ -z "$CAMPO47_3" ]; then CAMPO47_3=${NULO}; fi
		CAMPO47_4=${LINHA:1259:2}; if [ -z "$CAMPO47_4" ]; then CAMPO47_4=${NULO}; fi
		CAMPO47_5=${LINHA:1261:2}; if [ -z "$CAMPO47_5" ]; then CAMPO47_5=${NULO}; fi
		CAMPO47_6=${LINHA:1262:2}; if [ -z "$CAMPO47_6" ]; then CAMPO47_6=${NULO}; fi
		CAMPO47_7=${LINHA:1264:2}; if [ -z "$CAMPO47_7" ]; then CAMPO47_7=${NULO}; fi
		CAMPO47_8=${LINHA:1266:2}; if [ -z "$CAMPO47_8" ]; then CAMPO47_8=${NULO}; fi
		CAMPO47_9=${LINHA:1268:2}; if [ -z "$CAMPO47_9" ]; then CAMPO47_9=${NULO}; fi
		CAMPO47_10=${LINHA:1270:2}; if [ -z "$CAMPO47_10" ]; then CAMPO47_10=${NULO}; fi
		CAMPO47_11=${LINHA:1272:2}; if [ -z "$CAMPO47_11" ]; then CAMPO47_11=${NULO}; fi
		CAMPO47_12=${LINHA:1274:2}; if [ -z "$CAMPO47_12" ]; then CAMPO47_12=${NULO}; fi
	
	#01278 00003  NUM     IT-QT-LANCAMENTO
	CAMPO48=${LINHA:1277:3}; if [ -z "$CAMPO48" ]; then CAMPO48=${NULO}; fi
	
echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26${TAB}$CAMPO27${TAB}$CAMPO28${TAB}$CAMPO29${TAB}$CAMPO30${TAB}$CAMPO031_1${TAB}$CAMPO031_2${TAB}$CAMPO031_3${TAB}$CAMPO031_4${TAB}$CAMPO031_5${TAB}$CAMPO031_6${TAB}$CAMPO031_7${TAB}$CAMPO031_8${TAB}$CAMPO031_9${TAB}$CAMPO031_10${TAB}$CAMPO031_11${TAB}$CAMPO031_12${TAB}$CAMPO32${TAB}$CAMPO33${TAB}$CAMPO34${TAB}$CAMPO035_1${TAB}$CAMPO035_2${TAB}$CAMPO035_3${TAB}$CAMPO035_4${TAB}$CAMPO035_5${TAB}$CAMPO035_6${TAB}$CAMPO035_7${TAB}$CAMPO035_8${TAB}$CAMPO035_9${TAB}$CAMPO035_10${TAB}$CAMPO035_11${TAB}$CAMPO035_12${TAB}$CAMPO036_1${TAB}$CAMPO036_2${TAB}$CAMPO036_3${TAB}$CAMPO036_4${TAB}$CAMPO036_5${TAB}$CAMPO036_6${TAB}$CAMPO036_7${TAB}$CAMPO036_8${TAB}$CAMPO036_9${TAB}$CAMPO036_10${TAB}$CAMPO036_11${TAB}$CAMPO036_12${TAB}$CAMPO037_1${TAB}$CAMPO037_2${TAB}$CAMPO037_3${TAB}$CAMPO037_4${TAB}$CAMPO037_5${TAB}$CAMPO037_6${TAB}$CAMPO037_7${TAB}$CAMPO037_8${TAB}$CAMPO037_9${TAB}$CAMPO037_10${TAB}$CAMPO037_11${TAB}$CAMPO037_12${TAB}$CAMPO38${TAB}$CAMPO39${TAB}$CAMPO40${TAB}$CAMPO41${TAB}$CAMPO42${TAB}$CAMPO43${TAB}$CAMPO044_1${TAB}$CAMPO044_2${TAB}$CAMPO044_3${TAB}$CAMPO044_4${TAB}$CAMPO044_5${TAB}$CAMPO044_6${TAB}$CAMPO044_7${TAB}$CAMPO044_8${TAB}$CAMPO044_9${TAB}$CAMPO044_10${TAB}$CAMPO45${TAB}$CAMPO46${TAB}$CAMPO047_1${TAB}$CAMPO047_2${TAB}$CAMPO047_3${TAB}$CAMPO047_4${TAB}$CAMPO047_5${TAB}$CAMPO047_6${TAB}$CAMPO047_7${TAB}$CAMPO047_8${TAB}$CAMPO047_9${TAB}$CAMPO047_10${TAB}$CAMPO047_11${TAB}$CAMPO047_12${TAB}$CAMPO48 >> ${FILE}.sql;
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