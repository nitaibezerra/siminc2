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
echo -e  "copy siafi.pf (it_co_usuario${TAB},it_terminal_usuario${TAB},it_da_transacao${TAB},it_ho_transacao${TAB},it_co_ug_operador${TAB},it_da_emissao${TAB},it_co_ug_favorecida${TAB},it_co_ug_gestao_favorecida${TAB},it_especie_pf${TAB},it_observacao${TAB},it_mes_lancamento${TAB},it_ug_gestao_num_receb1${TAB},it_ug_gestao_num_receb2${TAB},it_ug_gestao_num_pag1${TAB},it_ug_gestao_num_pag2${TAB},it_co_evento_01${TAB},it_co_evento_02${TAB},it_co_evento_03${TAB},it_co_evento_04${TAB},it_co_evento_05${TAB},it_co_evento_06${TAB},it_co_evento_07${TAB},it_co_evento_08${TAB},it_co_evento_09${TAB},it_co_evento_10${TAB},it_co_evento_11${TAB},it_co_evento_12${TAB},it_co_situacao_01${TAB},it_co_situacao_02${TAB},it_co_situacao_03${TAB},it_co_situacao_04${TAB},it_co_situacao_05${TAB},it_co_situacao_06${TAB},it_co_situacao_07${TAB},it_co_situacao_08${TAB},it_co_situacao_09${TAB},it_co_situacao_10${TAB},it_co_situacao_11${TAB},it_co_situacao_12${TAB},it_fonte_recurso_01${TAB},it_fonte_recurso_02${TAB},it_fonte_recurso_03${TAB},it_fonte_recurso_04${TAB},it_fonte_recurso_05${TAB},it_fonte_recurso_06${TAB},it_fonte_recurso_07${TAB},it_fonte_recurso_08${TAB},it_fonte_recurso_09${TAB},it_fonte_recurso_10${TAB},it_fonte_recurso_11${TAB},it_fonte_recurso_12${TAB},it_vincu_pagamento_01${TAB},it_vincu_pagamento_02${TAB},it_vincu_pagamento_03${TAB},it_vincu_pagamento_04${TAB},it_vincu_pagamento_05${TAB},it_vincu_pagamento_06${TAB},it_vincu_pagamento_07${TAB},it_vincu_pagamento_08${TAB},it_vincu_pagamento_09${TAB},it_vincu_pagamento_10${TAB},it_vincu_pagamento_11${TAB},it_vincu_pagamento_12${TAB},it_categoria_gasto_01${TAB},it_categoria_gasto_02${TAB},it_categoria_gasto_03${TAB},it_categoria_gasto_04${TAB},it_categoria_gasto_05${TAB},it_categoria_gasto_06${TAB},it_categoria_gasto_07${TAB},it_categoria_gasto_08${TAB},it_categoria_gasto_09${TAB},it_categoria_gasto_10${TAB},it_categoria_gasto_11${TAB},it_categoria_gasto_12${TAB},it_da_previsao_pf_01${TAB},it_da_previsao_pf_02${TAB},it_da_previsao_pf_03${TAB},it_da_previsao_pf_04${TAB},it_da_previsao_pf_05${TAB},it_da_previsao_pf_06${TAB},it_da_previsao_pf_07${TAB},it_da_previsao_pf_08${TAB},it_da_previsao_pf_09${TAB},it_da_previsao_pf_10${TAB},it_da_previsao_pf_11${TAB},it_da_previsao_pf_12${TAB},it_vl_transacao_01${TAB},it_vl_transacao_02${TAB},it_vl_transacao_03${TAB},it_vl_transacao_04${TAB},it_vl_transacao_05${TAB},it_vl_transacao_06${TAB},it_vl_transacao_07${TAB},it_vl_transacao_08${TAB},it_vl_transacao_09${TAB},it_vl_transacao_10${TAB},it_vl_transacao_11${TAB},it_vl_transacao_12${TAB},it_numero_pf${TAB},it_co_inscricao2_01${TAB},it_co_inscricao2_02${TAB},it_co_inscricao2_03${TAB},it_co_inscricao2_04${TAB},it_co_inscricao2_05${TAB},it_co_inscricao2_06${TAB},it_co_inscricao2_07${TAB},it_co_inscricao2_08${TAB},it_co_inscricao2_09${TAB},it_co_inscricao2_10${TAB},it_co_inscricao2_11${TAB},it_co_inscricao2_12${TAB},it_gr_classificacao2_01${TAB},it_gr_classificacao2_02${TAB},it_gr_classificacao2_03${TAB},it_gr_classificacao2_04${TAB},it_gr_classificacao2_05${TAB},it_gr_classificacao2_06${TAB},it_gr_classificacao2_07${TAB},it_gr_classificacao2_08${TAB},it_gr_classificacao2_09${TAB},it_gr_classificacao2_10${TAB},it_gr_classificacao2_11${TAB},it_gr_classificacao2_12${TAB},it_in_tipo_recurso_01${TAB},it_in_tipo_recurso_02${TAB},it_in_tipo_recurso_03${TAB},it_in_tipo_recurso_04${TAB},it_in_tipo_recurso_05${TAB},it_in_tipo_recurso_06${TAB},it_in_tipo_recurso_07${TAB},it_in_tipo_recurso_08${TAB},it_in_tipo_recurso_09${TAB},it_in_tipo_recurso_10${TAB},it_in_tipo_recurso_11${TAB},it_in_tipo_recurso_12${TAB},it_favorecido_ob${TAB},it_co_favorecido_ob ) FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^PF*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
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
	
	# 00038 00008  NUM     DATA DA EMISSAO(DDMMAAAA)
	CAMPO06=${LINHA:41:4}${LINHA:39:2}${LINHA:37:2}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	
	# 00046 00006  NUM     CODIGO DA UG FAVORECIDA
	CAMPO07=${LINHA:45:6}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	
	# 00052 00005  NUM     CODIGO DA GESTAO FAVORECIDA
	CAMPO08=${LINHA:51:5}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	
	# 00057 00001  NUM     ESPECIE PF
	CAMPO09=${LINHA:56:1}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	
	# 00058 00234  ALFANUM OBSERVACAO
	CAMPO10=${LINHA:57:234}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
	# 00292 00002  NUM     MES DE LANCAMENTO
	CAMPO11=${LINHA:291:2}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	
	# 00294 00023  ALFANUM UG GESTAO DO NUMERO DE RECEBIMENTO1-CPR
	CAMPO12=${LINHA:293:23}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
	
	# 00317 00023  ALFANUM UG GESTAO DO NUMERO DE RECEBIMENTO2-CPR
	CAMPO13=${LINHA:316:23}; if [ -z "$CAMPO13" ]; then CAMPO13=${NULO}; fi
	
	# 00340 00023  ALFANUM UG GESTAO DO NUMERO DE PAGAMENTO1-CPR
	CAMPO14=${LINHA:339:23}; if [ -z "$CAMPO14" ]; then CAMPO14=${NULO}; fi
	
	# 00363 00023  ALFANUM UG GESTAO DO NUMERO DE PAGAMENTO2-CPR
	CAMPO15=${LINHA:362:23}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
	
	# 00386 00072  NUM     CODIGO DO EVENTO   OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
	multiplicos 12 6 ${LINHA:385:72}; CAMPO16=$valores;
	
	# 00458 00024  ALFANUM TIPO DA SITUACAO  OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 2 BYTES
	multiplicos 12 2 ${LINHA:457:24}; CAMPO17=$valores;
	
	# 00482 00120  ALFANUM FONTE DE RECURSO   OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 10 BYTES
	multiplicos 12 10 ${LINHA:481:120}; CAMPO18=$valores;
	
	# 00602 00036  NUM     VINCULACAO DE PAGAMENTO   OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 3 BYTES
	multiplicos 12 3 ${LINHA:601:36}; CAMPO19=$valores;
	
	# 00638 00012  ALFANUM CATEGORIA GASTO__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 1 BYTE
	multiplicos 12 1 ${LINHA:637:12}; CAMPO20=$valores;
	
	# 00650 00096  NUM     DATA DA PREVISAO PF_OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 8 BYTES
	multiplicos 12 8 ${LINHA:649:96}; CAMPO21=$valores;
	
	# 00746 00204  NUM     VALOR DA TRANSACAO__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 17 BYTES (15,2)
	multiplicosFloat 12 17 2 ${LINHA:745:204}; CAMPO22=$valores;
	
	# 00950 00023  ALFANUM NUMERO DA PF
	CAMPO23=${LINHA:949:23}; if [ -z "$CAMPO23" ]; then CAMPO23=${NULO}; fi
	
	# 00973 00168  ALFANUM IT-CO-INSCRICAO2 (TAM. 14 BYTES OCORR. 12 VEZES)
	multiplicos 12 14 ${LINHA:972:168}; CAMPO24=$valores;
	
	# 01141 00108  NUM     GR-CLASSIFICACAO2 (TAM. 09 BYTES OCORR. 12 VEZES)
	multiplicos 12 9 ${LINHA:1140:108}; CAMPO25=$valores;
	
	# 01249 00012  NUM     IT-IN-TIPO-RECURSO (TAM. 01 BYTES OCORR. 12 VEZES)
	multiplicos 12 1 ${LINHA:1248:12}; CAMPO26=$valores;
	
	# 01261 00001  NUM     FAVORECIDO OB
	CAMPO27=${LINHA:1260:1}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
	
	# 01262 00014  ALFANUM CODIGO DO FAVORECIDO OB
	CAMPO28=${LINHA:1261:14}; if [ "$CAMPO28" ]; then CAMPO28=${NULO}; fi
echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22${TAB}$CAMPO23${TAB}$CAMPO24${TAB}$CAMPO25${TAB}$CAMPO26${TAB}$CAMPO27${TAB}$CAMPO28 >> ${FILE}.sql;


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