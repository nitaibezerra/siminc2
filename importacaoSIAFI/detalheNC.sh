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
		"J") operadorValor=1; ;;
		"K") operadorValor=2; ;;
		"L") operadorValor=3; ;;
		"M") operadorValor=4; ;;
		"N") operadorValor=5; ;;
		"O") operadorValor=6; ;;
		"P") operadorValor=7; ;;
		"Q") operadorValor=8; ;;
		"R") operadorValor=9; ;;
		"}") operadorValor=0; ;;
		*) operadorValor=99; ;;
	esac
}

# Início da geração do script !!!
echo -e "SET client_encoding TO 'LATIN5'; " >> ${FILE}.sql;
echo -e  "COPY siafi.nc ( cpf_usuario${TAB},terminal_usuario${TAB},data_transacao${TAB},hora_transacao${TAB},codigo_ug_operador${TAB},numero_nc${TAB},data_emissao${TAB},codigo_ug_favorecida${TAB},codigo_gestao_favorecida${TAB},operacao_cambial${TAB},observacao${TAB},codigo_evento_01${TAB},codigo_evento_02${TAB},codigo_evento_03${TAB},codigo_evento_04${TAB},codigo_evento_05${TAB},codigo_evento_06${TAB},codigo_evento_07${TAB},codigo_evento_08${TAB},codigo_evento_09${TAB},codigo_evento_10${TAB},codigo_evento_11${TAB},codigo_evento_12${TAB},esfera_orcamentaria_01${TAB},esfera_orcamentaria_02${TAB},esfera_orcamentaria_03${TAB},esfera_orcamentaria_04${TAB},esfera_orcamentaria_05${TAB},esfera_orcamentaria_06${TAB},esfera_orcamentaria_07${TAB},esfera_orcamentaria_08${TAB},esfera_orcamentaria_09${TAB},esfera_orcamentaria_10${TAB},esfera_orcamentaria_11${TAB},esfera_orcamentaria_12${TAB},ptres_01${TAB},ptres_02${TAB},ptres_03${TAB},ptres_04${TAB},ptres_05${TAB},ptres_06${TAB},ptres_07${TAB},ptres_08${TAB},ptres_09${TAB},ptres_10${TAB},ptres_11${TAB},ptres_12${TAB},fonte_recurso_01${TAB},fonte_recurso_02${TAB},fonte_recurso_03${TAB},fonte_recurso_04${TAB},fonte_recurso_05${TAB},fonte_recurso_06${TAB},fonte_recurso_07${TAB},fonte_recurso_08${TAB},fonte_recurso_09${TAB},fonte_recurso_10${TAB},fonte_recurso_11${TAB},fonte_recurso_12${TAB},natureza_despesa_01${TAB},natureza_despesa_02${TAB},natureza_despesa_03${TAB},natureza_despesa_04${TAB},natureza_despesa_05${TAB},natureza_despesa_06${TAB},natureza_despesa_07${TAB},natureza_despesa_08${TAB},natureza_despesa_09${TAB},natureza_despesa_10${TAB},natureza_despesa_11${TAB},natureza_despesa_12${TAB},codigo_ug_responsavel_01${TAB},codigo_ug_responsavel_02${TAB},codigo_ug_responsavel_03${TAB},codigo_ug_responsavel_04${TAB},codigo_ug_responsavel_05${TAB},codigo_ug_responsavel_06${TAB},codigo_ug_responsavel_07${TAB},codigo_ug_responsavel_08${TAB},codigo_ug_responsavel_09${TAB},codigo_ug_responsavel_10${TAB},codigo_ug_responsavel_11${TAB},codigo_ug_responsavel_12${TAB},plano_interno_01${TAB},plano_interno_02${TAB},plano_interno_03${TAB},plano_interno_04${TAB},plano_interno_05${TAB},plano_interno_06${TAB},plano_interno_07${TAB},plano_interno_08${TAB},plano_interno_09${TAB},plano_interno_10${TAB},plano_interno_11${TAB},plano_interno_12${TAB},valor_transacao_01${TAB},valor_transacao_02${TAB},valor_transacao_03${TAB},valor_transacao_04${TAB},valor_transacao_05${TAB},valor_transacao_06${TAB},valor_transacao_07${TAB},valor_transacao_08${TAB},valor_transacao_09${TAB},valor_transacao_10${TAB},valor_transacao_11${TAB},valor_transacao_12${TAB},mes_lancamento${TAB},it_nu_original${TAB},it_co_sistema_origem${TAB},it_co_subitem_01${TAB},it_co_subitem_02${TAB},it_co_subitem_03${TAB},it_co_subitem_04${TAB},it_co_subitem_05${TAB},it_co_subitem_06${TAB},it_co_subitem_07${TAB},it_co_subitem_08${TAB},it_co_subitem_09${TAB},it_co_subitem_10${TAB},it_co_subitem_11${TAB},it_co_subitem_12 ) FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^NC*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do

	# 00001 00011  NUM     CPF DO USUARIO
	CAMPO01=${LINHA:0:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00012 00008  ALFANUM TERMINAL DO USUARIO
	CAMPO02=${LINHA:11:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	# 00020 00008  NUM     DATA DE TRANSACAO(DDMMAAAA)
	CAMPO03=${LINHA:23:4}${LINHA:21:2}${LINHA:19: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	# 00028 00004  NUM     HORA TRANSACAO(HHMM)
	CAMPO04=${LINHA:27:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00032 00006  NUM     CODIGO DA UG DO OPERADOR
	CAMPO05=${LINHA:31:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
	# 00038 00023  ALFANUM NUMERO DA NC
	CAMPO06=${LINHA:37:23}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	
	# 00061 00008  NUM     DATA DE EMISSAO(DDMMAAAA)
	CAMPO07=${LINHA:64:4}${LINHA:62:2}${LINHA:60:2}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	
	# 00069 00006  NUM     CODIGO DA UG FAVORECIDA
	CAMPO08=${LINHA:68:6}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	
	# 00075 00005  NUM     CODIGO DA GESTAO FAVORECIDA
	CAMPO09=${LINHA:74:5}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	
	# 00080 00010  NUM     OPERACAO CAMBIAL
	CAMPO10=${LINHA:79:10}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
	# 00090 00234  ALFANUM OBSERVACAO
	CAMPO11=${LINHA:89:234}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	
	
	# 00324 00072  NUM     CODIGO DO EVENTO__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
		CAMPO12_1=${LINHA:323:6}; if [ -z "$CAMPO12_1" ]; then CAMPO12_1=${NULO}; fi
		CAMPO12_2=${LINHA:329:6}; if [ -z "$CAMPO12_2" ]; then CAMPO12_2=${NULO}; fi
		CAMPO12_3=${LINHA:335:6}; if [ -z "$CAMPO12_3" ]; then CAMPO12_3=${NULO}; fi
		CAMPO12_4=${LINHA:341:6}; if [ -z "$CAMPO12_4" ]; then CAMPO12_4=${NULO}; fi
		CAMPO12_5=${LINHA:347:6}; if [ -z "$CAMPO12_5" ]; then CAMPO12_5=${NULO}; fi
		CAMPO12_6=${LINHA:353:6}; if [ -z "$CAMPO12_6" ]; then CAMPO12_6=${NULO}; fi
		CAMPO12_7=${LINHA:359:6}; if [ -z "$CAMPO12_7" ]; then CAMPO12_7=${NULO}; fi
		CAMPO12_8=${LINHA:365:6}; if [ -z "$CAMPO12_8" ]; then CAMPO12_8=${NULO}; fi
		CAMPO12_9=${LINHA:371:6}; if [ -z "$CAMPO12_9" ]; then CAMPO12_9=${NULO}; fi
		CAMPO12_10=${LINHA:377:6}; if [ -z "$CAMPO12_10" ]; then CAMPO12_10=${NULO}; fi
		CAMPO12_11=${LINHA:383:6}; if [ -z "$CAMPO12_11" ]; then CAMPO12_11=${NULO}; fi
		CAMPO12_12=${LINHA:389:6}; if [ -z "$CAMPO12_12" ]; then CAMPO12_12=${NULO}; fi
		
	# 00396 00012  NUM ESFERA ORCAMENTARIA__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 1 BYTE
		CAMPO13_1=${LINHA:395:1}; if [ -z "$CAMPO13_1" ]; then CAMPO13_1=${NULO}; fi
		CAMPO13_2=${LINHA:396:1}; if [ -z "$CAMPO13_2" ]; then CAMPO13_2=${NULO}; fi
		CAMPO13_3=${LINHA:397:1}; if [ -z "$CAMPO13_3" ]; then CAMPO13_3=${NULO}; fi
		CAMPO13_4=${LINHA:398:1}; if [ -z "$CAMPO13_4" ]; then CAMPO13_4=${NULO}; fi
		CAMPO13_5=${LINHA:399:1}; if [ -z "$CAMPO13_5" ]; then CAMPO13_5=${NULO}; fi
		CAMPO13_6=${LINHA:400:1}; if [ -z "$CAMPO13_6" ]; then CAMPO13_6=${NULO}; fi
		CAMPO13_7=${LINHA:401:1}; if [ -z "$CAMPO13_7" ]; then CAMPO13_7=${NULO}; fi
		CAMPO13_8=${LINHA:402:1}; if [ -z "$CAMPO13_8" ]; then CAMPO13_8=${NULO}; fi
		CAMPO13_9=${LINHA:403:1}; if [ -z "$CAMPO13_9" ]; then CAMPO13_9=${NULO}; fi
		CAMPO13_10=${LINHA:404:1}; if [ -z "$CAMPO13_10" ]; then CAMPO13_10=${NULO}; fi
		CAMPO13_11=${LINHA:405:1}; if [ -z "$CAMPO13_11" ]; then CAMPO13_11=${NULO}; fi
		CAMPO13_12=${LINHA:406:1}; if [ -z "$CAMPO13_12" ]; then CAMPO13_12=${NULO}; fi
		
		
	# 00408 00072  NUM PROGRAMA DE TRABALHO RESUMIDO - PTRES OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
		CAMPO14_1=${LINHA:407:6}; if [ -z "$CAMPO14_1" ]; then CAMPO14_1=${NULO}; fi
		CAMPO14_2=${LINHA:413:6}; if [ -z "$CAMPO14_2" ]; then CAMPO14_2=${NULO}; fi
		CAMPO14_3=${LINHA:419:6}; if [ -z "$CAMPO14_3" ]; then CAMPO14_3=${NULO}; fi
		CAMPO14_4=${LINHA:425:6}; if [ -z "$CAMPO14_4" ]; then CAMPO14_4=${NULO}; fi
		CAMPO14_5=${LINHA:431:6}; if [ -z "$CAMPO14_5" ]; then CAMPO14_5=${NULO}; fi
		CAMPO14_6=${LINHA:437:6}; if [ -z "$CAMPO14_6" ]; then CAMPO14_6=${NULO}; fi
		CAMPO14_7=${LINHA:443:6}; if [ -z "$CAMPO14_7" ]; then CAMPO14_7=${NULO}; fi
		CAMPO14_8=${LINHA:449:6}; if [ -z "$CAMPO14_8" ]; then CAMPO14_8=${NULO}; fi
		CAMPO14_9=${LINHA:455:6}; if [ -z "$CAMPO14_9" ]; then CAMPO14_9=${NULO}; fi
		CAMPO14_10=${LINHA:461:6}; if [ -z "$CAMPO14_10" ]; then CAMPO14_10=${NULO}; fi
		CAMPO14_11=${LINHA:467:6}; if [ -z "$CAMPO14_11" ]; then CAMPO14_11=${NULO}; fi
		CAMPO14_12=${LINHA:473:6}; if [ -z "$CAMPO14_12" ]; then CAMPO14_12=${NULO}; fi
		
	# 00480 00120  NUM FONTE DE RECURSO__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 10 BYTES
		CAMPO15_1=${LINHA:479:10}; if [ -z $CAMPO15_1 ]; then CAMPO15_1=${NULO}; fi
		CAMPO15_2=${LINHA:489:10}; if [ -z $CAMPO15_2 ]; then CAMPO15_2=${NULO}; fi
		CAMPO15_3=${LINHA:499:10}; if [ -z $CAMPO15_3 ]; then CAMPO15_3=${NULO}; fi
		CAMPO15_4=${LINHA:509:10}; if [ -z $CAMPO15_4 ]; then CAMPO15_4=${NULO}; fi
		CAMPO15_5=${LINHA:519:10}; if [ -z $CAMPO15_5 ]; then CAMPO15_5=${NULO}; fi
		CAMPO15_6=${LINHA:529:10}; if [ -z $CAMPO15_6 ]; then CAMPO15_6=${NULO}; fi
		CAMPO15_7=${LINHA:539:10}; if [ -z $CAMPO15_7 ]; then CAMPO15_7=${NULO}; fi
		CAMPO15_8=${LINHA:549:10}; if [ -z $CAMPO15_8 ]; then CAMPO15_8=${NULO}; fi
		CAMPO15_9=${LINHA:559:10}; if [ -z $CAMPO15_9 ]; then CAMPO15_9=${NULO}; fi
		CAMPO15_10=${LINHA:569:10}; if [ -z $CAMPO15_10 ]; then CAMPO15_10=${NULO}; fi
		CAMPO15_11=${LINHA:579:10}; if [ -z $CAMPO15_11 ]; then CAMPO15_11=${NULO}; fi
		CAMPO15_12=${LINHA:589:10}; if [ -z $CAMPO15_12 ]; then CAMPO15_12=${NULO}; fi
		
	# 00600 00072  NUM NATUREZA DE DESPESA__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
		CAMPO16_1=${LINHA:599:6}; if [ -z "$CAMPO16_1" ]; then CAMPO16_1=${NULO}; fi
		CAMPO16_2=${LINHA:605:6}; if [ -z "$CAMPO16_2" ]; then CAMPO16_2=${NULO}; fi
		CAMPO16_3=${LINHA:611:6}; if [ -z "$CAMPO16_3" ]; then CAMPO16_3=${NULO}; fi
		CAMPO16_4=${LINHA:617:6}; if [ -z "$CAMPO16_4" ]; then CAMPO16_4=${NULO}; fi
		CAMPO16_5=${LINHA:623:6}; if [ -z "$CAMPO16_5" ]; then CAMPO16_5=${NULO}; fi
		CAMPO16_6=${LINHA:629:6}; if [ -z "$CAMPO16_6" ]; then CAMPO16_6=${NULO}; fi
		CAMPO16_7=${LINHA:635:6}; if [ -z "$CAMPO16_7" ]; then CAMPO16_7=${NULO}; fi
		CAMPO16_8=${LINHA:641:6}; if [ -z "$CAMPO16_8" ]; then CAMPO16_8=${NULO}; fi
		CAMPO16_9=${LINHA:647:6}; if [ -z "$CAMPO16_9" ]; then CAMPO16_9=${NULO}; fi
		CAMPO16_10=${LINHA:653:6}; if [ -z "$CAMPO16_10" ]; then CAMPO16_10=${NULO}; fi
		CAMPO16_11=${LINHA:659:6}; if [ -z "$CAMPO16_11" ]; then CAMPO16_11=${NULO}; fi
		CAMPO16_12=${LINHA:665:6}; if [ -z "$CAMPO16_12" ]; then CAMPO16_12=${NULO}; fi
	
	#  00672 00072  NUM CODIGO DA UG RESPONSAVEL__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
		CAMPO17_1=${LINHA:671:6}; if [ -z "$CAMPO17_1" ]; then CAMPO17_1=${NULO}; fi
		CAMPO17_2=${LINHA:677:6}; if [ -z "$CAMPO17_2" ]; then CAMPO17_2=${NULO}; fi
		CAMPO17_3=${LINHA:683:6}; if [ -z "$CAMPO17_3" ]; then CAMPO17_3=${NULO}; fi
		CAMPO17_4=${LINHA:689:6}; if [ -z "$CAMPO17_4" ]; then CAMPO17_4=${NULO}; fi
		CAMPO17_5=${LINHA:695:6}; if [ -z "$CAMPO17_5" ]; then CAMPO17_5=${NULO}; fi
		CAMPO17_6=${LINHA:701:6}; if [ -z "$CAMPO17_6" ]; then CAMPO17_6=${NULO}; fi
		CAMPO17_7=${LINHA:707:6}; if [ -z "$CAMPO17_7" ]; then CAMPO17_7=${NULO}; fi
		CAMPO17_8=${LINHA:713:6}; if [ -z "$CAMPO17_8" ]; then CAMPO17_8=${NULO}; fi
		CAMPO17_9=${LINHA:719:6}; if [ -z "$CAMPO17_9" ]; then CAMPO17_9=${NULO}; fi
		CAMPO17_10=${LINHA:725:6}; if [ -z "$CAMPO17_10" ]; then CAMPO17_10=${NULO}; fi
		CAMPO17_11=${LINHA:731:6}; if [ -z "$CAMPO17_11" ]; then CAMPO17_11=${NULO}; fi
		CAMPO17_12=${LINHA:737:6}; if [ -z "$CAMPO17_12" ]; then CAMPO17_12=${NULO}; fi
	
	# 00744 00132 ALFANUM PLANO INTERNO__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 11 BYTES
		CAMPO18_1=${LINHA:743:11}; if [ -z $CAMPO18_1 ]; then CAMPO18_1=${NULO}; fi
		CAMPO18_2=${LINHA:754:11}; if [ -z $CAMPO18_2 ]; then CAMPO18_2=${NULO}; fi
		CAMPO18_3=${LINHA:765:11}; if [ -z $CAMPO18_3 ]; then CAMPO18_3=${NULO}; fi
		CAMPO18_4=${LINHA:776:11}; if [ -z $CAMPO18_4 ]; then CAMPO18_4=${NULO}; fi
		CAMPO18_5=${LINHA:787:11}; if [ -z $CAMPO18_5 ]; then CAMPO18_5=${NULO}; fi
		CAMPO18_6=${LINHA:798:11}; if [ -z $CAMPO18_6 ]; then CAMPO18_6=${NULO}; fi
		CAMPO18_7=${LINHA:809:11}; if [ -z $CAMPO18_7 ]; then CAMPO18_7=${NULO}; fi
		CAMPO18_8=${LINHA:820:11}; if [ -z $CAMPO18_8 ]; then CAMPO18_8=${NULO}; fi
		CAMPO18_9=${LINHA:831:11}; if [ -z $CAMPO18_9 ]; then CAMPO18_9=${NULO}; fi
		CAMPO18_10=${LINHA:842:11}; if [ -z $CAMPO18_10 ]; then CAMPO18_10=${NULO}; fi
		CAMPO18_11=${LINHA:853:11}; if [ -z $CAMPO18_11 ]; then CAMPO18_11=${NULO}; fi
		CAMPO18_12=${LINHA:864:11}; if [ -z $CAMPO18_12 ]; then CAMPO18_12=${NULO}; fi
	
	#  00876 00204  NUM VALOR DA TRANSACAO(N15,2)__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 17 BYTES
		## 19_1
		CAMPO19_1=${LINHA:875:17};
		if [ -z "$CAMPO19_1" ]; then
			CAMPO19_1=${NULO};
		else
			stValor=$CAMPO19_1;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_1="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_1=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		
		## 19_2
		CAMPO19_2=${LINHA:892:17};
		if [ -z "$CAMPO19_2" ]; then
			CAMPO19_2=${NULO};
		else
			stValor=$CAMPO19_2;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_2="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_2=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_3
		CAMPO19_3=${LINHA:909:17};
		if [ -z "$CAMPO19_3" ]; then
			CAMPO19_3=${NULO};
		else
			stValor=$CAMPO19_3;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_3="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_3=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_4
		CAMPO19_4=${LINHA:926:17};
		if [ -z "$CAMPO19_4" ]; then
			CAMPO19_4=${NULO};
		else
			stValor=$CAMPO19_4;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_4="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_4=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_5
		CAMPO19_5=${LINHA:943:17};
		if [ -z "$CAMPO19_5" ]; then
			CAMPO19_5=${NULO};
		else
			stValor=$CAMPO19_5;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_5="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_5=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_6
		CAMPO19_6=${LINHA:960:17};
		if [ -z "$CAMPO19_2" ]; then
			CAMPO19_6=${NULO};
		else
			stValor=$CAMPO19_6;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_6="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_6=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_7
		CAMPO19_7=${LINHA:977:17};
		if [ -z "$CAMPO19_7" ]; then
			CAMPO19_7=${NULO};
		else
			stValor=$CAMPO19_7;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_7="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_7=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_8
		CAMPO19_8=${LINHA:994:17};
		if [ -z "$CAMPO19_8" ]; then
			CAMPO19_8=${NULO};
		else
			stValor=$CAMPO19_8;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_8="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_8=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_9
		CAMPO19_9=${LINHA:1011:17};
		if [ -z "$CAMPO19_9" ]; then
			CAMPO19_9=${NULO};
		else
			stValor=$CAMPO19_9;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_9="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_9=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_10
		CAMPO19_10=${LINHA:1028:17};
		if [ -z "$CAMPO19_10" ]; then
			CAMPO19_10=${NULO};
		else
			stValor=$CAMPO19_10;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_10="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_10=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_11
		CAMPO19_11=${LINHA:1045:17};
		if [ -z "$CAMPO19_11" ]; then
			CAMPO19_11=${NULO};
		else
			stValor=$CAMPO19_11;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_11="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_11=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
		## 19_12
		CAMPO19_12=${LINHA:1062:17};
		if [ -z "$CAMPO19_12" ]; then
			CAMPO19_12=${NULO};
		else
			stValor=$CAMPO19_12;
			valor=${stValor:16:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				CAMPO19_12="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
			else
				CAMPO19_12=${stValor:0:15}"."${stValor:16:2}
			fi
		fi
	
	# 01080 00002 NUM MES DE LANCAMENTO
	CAMPO20=${LINHA:1079:2}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
	
	# 01082 00020  ALFANUM IT-NU-ORIGINAL
	CAMPO21=${LINHA:1081:20}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi

	# 01102 00010  ALFANUM IT-CO-SISTEMA-ORIGEM
	CAMPO22=${LINHA:1101:10}; if [ -z $CAMPO22 ]; then CAMPO22=${NULO}; fi
	
	# 01112  01135  00024  ALFANUM IT-CO-SUBITEM   TAM 02 C/12 OCORRENCIAS
		CAMPO23_1=${LINHA:1111:2}; if [ -z $CAMPO23_1 ]; then CAMPO23_1=${NULO}; fi
		CAMPO23_2=${LINHA:1113:2}; if [ -z $CAMPO23_2 ]; then CAMPO23_2=${NULO}; fi
		CAMPO23_3=${LINHA:1115:2}; if [ -z $CAMPO23_3 ]; then CAMPO23_3=${NULO}; fi
		CAMPO23_4=${LINHA:1117:2}; if [ -z $CAMPO23_4 ]; then CAMPO23_4=${NULO}; fi
		CAMPO23_5=${LINHA:1119:2}; if [ -z $CAMPO23_5 ]; then CAMPO23_5=${NULO}; fi
		CAMPO23_6=${LINHA:1121:2}; if [ -z $CAMPO23_6 ]; then CAMPO23_6=${NULO}; fi
		CAMPO23_7=${LINHA:1123:2}; if [ -z $CAMPO23_7 ]; then CAMPO23_7=${NULO}; fi
		CAMPO23_8=${LINHA:1125:2}; if [ -z $CAMPO23_8 ]; then CAMPO23_8=${NULO}; fi
		CAMPO23_9=${LINHA:1127:2}; if [ -z $CAMPO23_9 ]; then CAMPO23_9=${NULO}; fi
		CAMPO23_10=${LINHA:1129:2}; if [ -z $CAMPO23_10 ]; then CAMPO23_10=${NULO}; fi
		CAMPO23_11=${LINHA:1131:2}; if [ -z $CAMPO23_11 ]; then CAMPO23_11=${NULO}; fi
		CAMPO23_12=${LINHA:1133:2}; if [ -z $CAMPO23_12 ]; then CAMPO23_12=${NULO}; fi
		
echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12_1${TAB}$CAMPO12_2${TAB}$CAMPO12_3${TAB}$CAMPO12_4${TAB}$CAMPO12_5${TAB}$CAMPO12_6${TAB}$CAMPO12_7${TAB}$CAMPO12_8${TAB}$CAMPO12_9${TAB}$CAMPO12_10${TAB}$CAMPO12_11${TAB}$CAMPO12_12${TAB}$CAMPO13_1${TAB}$CAMPO13_2${TAB}$CAMPO13_3${TAB}$CAMPO13_4${TAB}$CAMPO13_5${TAB}$CAMPO13_6${TAB}$CAMPO13_7${TAB}$CAMPO13_8${TAB}$CAMPO13_9${TAB}$CAMPO13_10${TAB}$CAMPO13_11${TAB}$CAMPO13_12${TAB}$CAMPO14_1${TAB}$CAMPO14_2${TAB}$CAMPO14_3${TAB}$CAMPO14_4${TAB}$CAMPO14_5${TAB}$CAMPO14_6${TAB}$CAMPO14_7${TAB}$CAMPO14_8${TAB}$CAMPO14_9${TAB}$CAMPO14_10${TAB}$CAMPO14_11${TAB}$CAMPO14_12${TAB}$CAMPO15_1${TAB}$CAMPO15_2${TAB}$CAMPO15_3${TAB}$CAMPO15_4${TAB}$CAMPO15_5${TAB}$CAMPO15_6${TAB}$CAMPO15_7${TAB}$CAMPO15_8${TAB}$CAMPO15_9${TAB}$CAMPO15_10${TAB}$CAMPO15_11${TAB}$CAMPO15_12${TAB}$CAMPO16_1${TAB}$CAMPO16_2${TAB}$CAMPO16_3${TAB}$CAMPO16_4${TAB}$CAMPO16_5${TAB}$CAMPO16_6${TAB}$CAMPO16_7${TAB}$CAMPO16_8${TAB}$CAMPO16_9${TAB}$CAMPO16_10${TAB}$CAMPO16_11${TAB}$CAMPO16_12${TAB}$CAMPO17_1${TAB}$CAMPO17_2${TAB}$CAMPO17_3${TAB}$CAMPO17_4${TAB}$CAMPO17_5${TAB}$CAMPO17_6${TAB}$CAMPO17_7${TAB}$CAMPO17_8${TAB}$CAMPO17_9${TAB}$CAMPO17_10${TAB}$CAMPO17_11${TAB}$CAMPO17_12${TAB}$CAMPO18_1${TAB}$CAMPO18_2${TAB}$CAMPO18_3${TAB}$CAMPO18_4${TAB}$CAMPO18_5${TAB}$CAMPO18_6${TAB}$CAMPO18_7${TAB}$CAMPO18_8${TAB}$CAMPO18_9${TAB}$CAMPO18_10${TAB}$CAMPO18_11${TAB}$CAMPO18_12${TAB}$CAMPO19_1${TAB}$CAMPO19_2${TAB}$CAMPO19_3${TAB}$CAMPO19_4${TAB}$CAMPO19_5${TAB}$CAMPO19_6${TAB}$CAMPO19_7${TAB}$CAMPO19_8${TAB}$CAMPO19_9${TAB}$CAMPO19_10${TAB}$CAMPO19_11${TAB}$CAMPO19_12${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23_1${TAB}$CAMPO23_2${TAB}$CAMPO23_3${TAB}$CAMPO23_4${TAB}$CAMPO23_5${TAB}$CAMPO23_6${TAB}$CAMPO23_7${TAB}$CAMPO23_8${TAB}$CAMPO23_9${TAB}$CAMPO23_10${TAB}$CAMPO23_11${TAB}$CAMPO23_12 >> ${FILE}.sql;


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