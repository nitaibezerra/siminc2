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

multiplicos(){
	qtCampos=$1;
	tamanho=$2;
	texto=$3;
	n=0;
	x=0;
	valores='';
	#echo 'texto:'$texto;
	#while [ $n -le $qtCampos ]; do
	while [ $n -lt $qtCampos ]; do
		valor=${texto:$x:$tamanho};
		if [ -z "$valor" ]; then
			valor=${NULO};
		fi
		#echo "Valor->"$valor"<-";
		x=$x+$tamanho;
		if [ $n -eq 0 ]; then
				valores=$valor;
			else
				valores=$valores${TAB}$valor;
		fi
		let n++;
	done
}
multiplicosFloat(){
	qtCampos=$1;
	qtInteiros=$2;
	qtCasasDecimais=$3;
	texto=$4;
	n=0;
	x=0;
	valores='';
	#echo 'texto:'$texto;
	while [ $n -lt $qtCampos ]; do
		valor=${texto:$x:$qtInteiros};
		if [ -z "$valor" ]; then
			valor=${NULO};
		else
			valor=${texto:$qtInteiros:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99 ]; then
				valor="-"${stValor:0:15}"."${texto:$qtInteiros:1}$operadorValor
			else
				valor=${stValor:0:15}"."${texto:$qtInteiros:2}
			fi
		fi
		x=$x+$qtInteiros;
		if [ $n -eq 0 ]; then
				valores=$valor;
			else
				valores=$valores${TAB}$valor;
		fi
		let n++;
	done
}

# Início da geração do script !!!
echo -e "SET client_encoding TO 'LATIN5'; " >> ${FILE}.sql;
echo -e  "COPY siafi.nd ( cpf_usuario${TAB},terminal_usuario${TAB},data_transacao${TAB},hora_transacao${TAB},codigo_ug_operador${TAB},numero_nd${TAB},data_emissao${TAB},instrumento_legal${TAB},numero_instrumento_legal${TAB},data_publicacao${TAB},nota_dotacao${TAB},codigo_ug_favorecida${TAB},codigo_gestao_favorecida${TAB},observacao${TAB},codigo_evento_01${TAB},codigo_evento_02${TAB},codigo_evento_03${TAB},codigo_evento_04${TAB},codigo_evento_05${TAB},codigo_evento_06${TAB},codigo_evento_07${TAB},codigo_evento_08${TAB},codigo_evento_09${TAB},codigo_evento_10${TAB},codigo_evento_11${TAB},codigo_evento_12${TAB},esfera_orcamentaria_01${TAB},esfera_orcamentaria_02${TAB},esfera_orcamentaria_03${TAB},esfera_orcamentaria_04${TAB},esfera_orcamentaria_05${TAB},esfera_orcamentaria_06${TAB},esfera_orcamentaria_07${TAB},esfera_orcamentaria_08${TAB},esfera_orcamentaria_09${TAB},esfera_orcamentaria_10${TAB},esfera_orcamentaria_11${TAB},esfera_orcamentaria_12${TAB},ptres_01${TAB},ptres_02${TAB},ptres_03${TAB},ptres_04${TAB},ptres_05${TAB},ptres_06${TAB},ptres_07${TAB},ptres_08${TAB},ptres_09${TAB},ptres_10${TAB},ptres_11${TAB},ptres_12${TAB},fonte_recurso_01${TAB},fonte_recurso_02${TAB},fonte_recurso_03${TAB},fonte_recurso_04${TAB},fonte_recurso_05${TAB},fonte_recurso_06${TAB},fonte_recurso_07${TAB},fonte_recurso_08${TAB},fonte_recurso_09${TAB},fonte_recurso_10${TAB},fonte_recurso_11${TAB},fonte_recurso_12${TAB},natureza_despesa_01${TAB},natureza_despesa_02${TAB},natureza_despesa_03${TAB},natureza_despesa_04${TAB},natureza_despesa_05${TAB},natureza_despesa_06${TAB},natureza_despesa_07${TAB},natureza_despesa_08${TAB},natureza_despesa_09${TAB},natureza_despesa_10${TAB},natureza_despesa_11${TAB},natureza_despesa_12${TAB},codigo_ug_responsavel_01${TAB},codigo_ug_responsavel_02${TAB},codigo_ug_responsavel_03${TAB},codigo_ug_responsavel_04${TAB},codigo_ug_responsavel_05${TAB},codigo_ug_responsavel_06${TAB},codigo_ug_responsavel_07${TAB},codigo_ug_responsavel_08${TAB},codigo_ug_responsavel_09${TAB},codigo_ug_responsavel_10${TAB},codigo_ug_responsavel_11${TAB},codigo_ug_responsavel_12${TAB},plano_interno_01${TAB},plano_interno_02${TAB},plano_interno_03${TAB},plano_interno_04${TAB},plano_interno_05${TAB},plano_interno_06${TAB},plano_interno_07${TAB},plano_interno_08${TAB},plano_interno_09${TAB},plano_interno_10${TAB},plano_interno_11${TAB},plano_interno_12${TAB},idoc_01${TAB},idoc_02${TAB},idoc_03${TAB},idoc_04${TAB},idoc_05${TAB},idoc_06${TAB},idoc_07${TAB},idoc_08${TAB},idoc_09${TAB},idoc_10${TAB},idoc_11${TAB},idoc_12${TAB},it_in_resultado_01${TAB},it_in_resultado_02${TAB},it_in_resultado_03${TAB},it_in_resultado_04${TAB},it_in_resultado_05${TAB},it_in_resultado_06${TAB},it_in_resultado_07${TAB},it_in_resultado_08${TAB},it_in_resultado_09${TAB},it_in_resultado_10${TAB},it_in_resultado_11${TAB},it_in_resultado_12${TAB},it_in_tipo_credito_01${TAB},it_in_tipo_credito_02${TAB},it_in_tipo_credito_03${TAB},it_in_tipo_credito_04${TAB},it_in_tipo_credito_05${TAB},it_in_tipo_credito_06${TAB},it_in_tipo_credito_07${TAB},it_in_tipo_credito_08${TAB},it_in_tipo_credito_09${TAB},it_in_tipo_credito_10${TAB},it_in_tipo_credito_11${TAB},it_in_tipo_credito_12${TAB},valor_transacao_01${TAB},valor_transacao_02${TAB},valor_transacao_03${TAB},valor_transacao_04${TAB},valor_transacao_05${TAB},valor_transacao_06${TAB},valor_transacao_07${TAB},valor_transacao_08${TAB},valor_transacao_09${TAB},valor_transacao_10${TAB},valor_transacao_11${TAB},valor_transacao_12${TAB},mes_lancamento${TAB},detalhamento_modalidade${TAB},it_in_especie_detalhamento${TAB},it_co_subitem_01${TAB},it_co_subitem_02${TAB},it_co_subitem_03${TAB},it_co_subitem_04${TAB},it_co_subitem_05${TAB},it_co_subitem_06${TAB},it_co_subitem_07${TAB},it_co_subitem_08${TAB},it_co_subitem_09${TAB},it_co_subitem_10${TAB},it_co_subitem_11${TAB},it_co_subitem_12${TAB},it_op_cambial ) FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^ND*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
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
	
	# 00038 00023  ALFANUM NUMERO DA ND
	CAMPO06=${LINHA:37:23}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	
	# 00061 00008  NUM     DATA DE EMISSAO(DDMMAAAA)
	CAMPO07=${LINHA:64:4}${LINHA:62:2}${LINHA:60:2}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi

	#  00069 00001  NUM     INSTRUMENTO LEGAL
	CAMPO08=${LINHA:68:1}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	
	#  00070 00006  ALFANUM NUMERO DO INSTRUMENTO LEGAL
	CAMPO09=${LINHA:69:6}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	
	#  00076 00008  NUM     DATA DA PUBLICACAO(DDMMAAAA)
	CAMPO10=${LINHA:79:4}${LINHA:77:2}${LINHA:75:2}; if [ "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
	#  00084 00002  ALFANUM NOTA DE DOTACAO
	CAMPO11=${LINHA:83:2}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	
	#  00086 00006  NUM     CODIGO DA UG FAVORECIDA
	CAMPO12=${LINHA:85:6}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
	
	#  00092 00005  NUM     CODIGO DA GESTAO FAVORECIDA
	CAMPO13=${LINHA:91:5}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
	
	#  00097 00234  ALFANUM OBSERVACAO
	CAMPO14=${LINHA:96:234}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
	
	#  00331 00072  NUM     CODIGO DO EVENTO__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
	multiplicos 12 6 ${LINHA:330:72}; CAMPO15=$valores;
	
	#  00403 00012  NUM     ESFERA ORCAMENTARIA__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 1 BYTE
	multiplicos 12 1 ${LINHA:402:12}; CAMPO16=$valores;
	
	#  00415 00072  NUM     PROGRAMA DE TRABALHO RESUMIDO - PTRES OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
	multiplicos 12 6 ${LINHA:414:72}; CAMPO17=$valores;
	
	#  00487 00120  NUM     FONTE DE RECURSO__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 10 BYTES
	multiplicos 12 10 ${LINHA:486:120}; CAMPO18=$valores;
	
	#  00607 00072  NUM     NATUREZA DE DESPESA__OBS.:OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
	multiplicos 12 6 ${LINHA:606:72}; CAMPO19=$valores;
	
	#  00679 00072  NUM     CODIGO DA UG RESPONSAVEL__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
	multiplicos 12 6 ${LINHA:678:72}; CAMPO20=$valores;
	
	#  00751 00132  ALFANUM PLANO INTERNO__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 11 BYTES
	multiplicos 12 6 ${LINHA:750:132}; CAMPO21=$valores;
	
	#  00883 00048  NUM     IDOC__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 4 BYTES
	multiplicos 12 4 ${LINHA:882:48}; CAMPO22=$valores;
	
	#  00931 00012  ALFANUM IT-IN-RESULTADO OCORRE 12 VEZES TAMANHO DE CADA CAMPO 1 BYTE
	multiplicos 12 1 ${LINHA:930:12}; CAMPO23=$valores;
	
	#  00943 00012  ALFANUM IT-IN-TIPO-CREDITO OCORRE 12 VEZES TAMANHO DE CADA CAMPO 1 BYTE
	multiplicos 12 1 ${LINHA:942:12}; CAMPO24=$valores;
	
	#  00955 00204  NUM     VALOR DA TRANSACAO(N15,2)__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 17 BYTES
	multiplicosFloat 12 15 2 ${LINHA:954:204}; CAMPO25=$valores;
	
	#  01159 00002  NUM     MES DE LANCAMENTO
	CAMPO26=${LINHA:1158:2}; if [ -z "$CAMPO26" ]; then CAMPO26=${NULO}; fi
	
	#  01161 00001  NUM     DETALHAMENTO DA MODALIDADE
	CAMPO27=${LINHA:1160:1}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
	
	#  01162 00001  NUM     IT-IN-ESPECIE-DETALHAMENTO
	CAMPO28=${LINHA:1161:1}; if [ -z "$CAMPO28" ]; then CAMPO28=${NULO}; fi
	
	#  01163 00024  ALFANUM IT-CO-SUBITEM  TAM 02 C/12 OCORRENCIAS
	multiplicos 12 2 ${LINHA:1162:24}; CAMPO29=$valores;
	
	#  01187 00010  NUM     IT-OP-CAMBIAL (FORMATO N6,4)
	multiplicosFloat 1 6 4 ${LINHA:1186:10}; CAMPO30=$valores;
	
	
echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26${TAB}$CAMPO27${TAB}$CAMPO28${TAB}$CAMPO29${TAB}$CAMPO30 >> ${FILE}.sql;


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