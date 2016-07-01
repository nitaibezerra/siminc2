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
echo -e  "COPY siafi.ol(it_co_usuario${TAB},it_co_terminal_usuario${TAB},it_da_transacao${TAB},it_ho_transacao${TAB},it_co_ug_operador${TAB},it_in_operacao${TAB},it_gr_gestao_an_numero_lo${TAB},it_nu_isn_doc_habil${TAB},it_in_situacao_lista${TAB},it_va_total_lista${TAB},it_co_favorecido_01${TAB},it_co_favorecido_02${TAB},it_co_favorecido_03${TAB},it_co_favorecido_04${TAB},it_co_favorecido_05${TAB},it_co_favorecido_06${TAB},it_co_favorecido_07${TAB},it_co_favorecido_08${TAB},it_co_favorecido_09${TAB},it_co_favorecido_10${TAB},it_co_favorecido_11${TAB},it_co_favorecido_12${TAB},it_co_favorecido_13${TAB},it_co_favorecido_14${TAB},it_co_favorecido_15${TAB},it_co_favorecido_16${TAB},it_co_favorecido_17${TAB},it_co_favorecido_18${TAB},it_co_favorecido_19${TAB},it_co_favorecido_20${TAB},it_co_favorecido_21${TAB},it_co_favorecido_22${TAB},it_co_favorecido_23${TAB},it_co_favorecido_24${TAB},it_co_favorecido_25${TAB},it_co_favorecido_26${TAB},it_co_favorecido_27${TAB},it_co_favorecido_28${TAB},it_co_favorecido_29${TAB},it_co_favorecido_30${TAB},it_co_favorecido_31${TAB},it_co_favorecido_32${TAB},it_co_gestao_01${TAB},it_co_gestao_02${TAB},it_co_gestao_03${TAB},it_co_gestao_04${TAB},it_co_gestao_05${TAB},it_co_gestao_06${TAB},it_co_gestao_07${TAB},it_co_gestao_08${TAB},it_co_gestao_09${TAB},it_co_gestao_10${TAB},it_co_gestao_11${TAB},it_co_gestao_12${TAB},it_co_gestao_13${TAB},it_co_gestao_14${TAB},it_co_gestao_15${TAB},it_co_gestao_16${TAB},it_co_gestao_17${TAB},it_co_gestao_18${TAB},it_co_gestao_19${TAB},it_co_gestao_20${TAB},it_co_gestao_21${TAB},it_co_gestao_22${TAB},it_co_gestao_23${TAB},it_co_gestao_24${TAB},it_co_gestao_25${TAB},it_co_gestao_26${TAB},it_co_gestao_27${TAB},it_co_gestao_28${TAB},it_co_gestao_29${TAB},it_co_gestao_30${TAB},it_co_gestao_31${TAB},it_co_gestao_32${TAB},it_gr_domicilio_favorecido_01${TAB},it_gr_domicilio_favorecido_02${TAB},it_gr_domicilio_favorecido_03${TAB},it_gr_domicilio_favorecido_04${TAB},it_gr_domicilio_favorecido_05${TAB},it_gr_domicilio_favorecido_06${TAB},it_gr_domicilio_favorecido_07${TAB},it_gr_domicilio_favorecido_08${TAB},it_gr_domicilio_favorecido_09${TAB},it_gr_domicilio_favorecido_10${TAB},it_gr_domicilio_favorecido_11${TAB},it_gr_domicilio_favorecido_12${TAB},it_gr_domicilio_favorecido_13${TAB},it_gr_domicilio_favorecido_14${TAB},it_gr_domicilio_favorecido_15${TAB},it_gr_domicilio_favorecido_16${TAB},it_gr_domicilio_favorecido_17${TAB},it_gr_domicilio_favorecido_18${TAB},it_gr_domicilio_favorecido_19${TAB},it_gr_domicilio_favorecido_20${TAB},it_gr_domicilio_favorecido_21${TAB},it_gr_domicilio_favorecido_22${TAB},it_gr_domicilio_favorecido_23${TAB},it_gr_domicilio_favorecido_24${TAB},it_gr_domicilio_favorecido_25${TAB},it_gr_domicilio_favorecido_26${TAB},it_gr_domicilio_favorecido_27${TAB},it_gr_domicilio_favorecido_28${TAB},it_gr_domicilio_favorecido_29${TAB},it_gr_domicilio_favorecido_30${TAB},it_gr_domicilio_favorecido_31${TAB},it_gr_domicilio_favorecido_32${TAB},it_nu_lista_01${TAB},it_nu_lista_02${TAB},it_nu_lista_03${TAB},it_nu_lista_04${TAB},it_nu_lista_05${TAB},it_nu_lista_06${TAB},it_nu_lista_07${TAB},it_nu_lista_08${TAB},it_nu_lista_09${TAB},it_nu_lista_10${TAB},it_nu_lista_11${TAB},it_nu_lista_12${TAB},it_nu_lista_13${TAB},it_nu_lista_14${TAB},it_nu_lista_15${TAB},it_nu_lista_16${TAB},it_nu_lista_17${TAB},it_nu_lista_18${TAB},it_nu_lista_19${TAB},it_nu_lista_20${TAB},it_nu_lista_21${TAB},it_nu_lista_22${TAB},it_nu_lista_23${TAB},it_nu_lista_24${TAB},it_nu_lista_25${TAB},it_nu_lista_26${TAB},it_nu_lista_27${TAB},it_nu_lista_28${TAB},it_nu_lista_29${TAB},it_nu_lista_30${TAB},it_nu_lista_31${TAB},it_nu_lista_32${TAB},it_co_ident_transferencia_01${TAB},it_co_ident_transferencia_02${TAB},it_co_ident_transferencia_03${TAB},it_co_ident_transferencia_04${TAB},it_co_ident_transferencia_05${TAB},it_co_ident_transferencia_06${TAB},it_co_ident_transferencia_07${TAB},it_co_ident_transferencia_08${TAB},it_co_ident_transferencia_09${TAB},it_co_ident_transferencia_10${TAB},it_co_ident_transferencia_11${TAB},it_co_ident_transferencia_12${TAB},it_co_ident_transferencia_13${TAB},it_co_ident_transferencia_14${TAB},it_co_ident_transferencia_15${TAB},it_co_ident_transferencia_16${TAB},it_co_ident_transferencia_17${TAB},it_co_ident_transferencia_18${TAB},it_co_ident_transferencia_19${TAB},it_co_ident_transferencia_20${TAB},it_co_ident_transferencia_21${TAB},it_co_ident_transferencia_22${TAB},it_co_ident_transferencia_23${TAB},it_co_ident_transferencia_24${TAB},it_co_ident_transferencia_25${TAB},it_co_ident_transferencia_26${TAB},it_co_ident_transferencia_27${TAB},it_co_ident_transferencia_28${TAB},it_co_ident_transferencia_29${TAB},it_co_ident_transferencia_30${TAB},it_co_ident_transferencia_31${TAB},it_co_ident_transferencia_32${TAB},it_gr_fonte_recurso_01${TAB},it_gr_fonte_recurso_02${TAB},it_gr_fonte_recurso_03${TAB},it_gr_fonte_recurso_04${TAB},it_gr_fonte_recurso_05${TAB},it_gr_fonte_recurso_06${TAB},it_gr_fonte_recurso_07${TAB},it_gr_fonte_recurso_08${TAB},it_gr_fonte_recurso_09${TAB},it_gr_fonte_recurso_10${TAB},it_gr_fonte_recurso_11${TAB},it_gr_fonte_recurso_12${TAB},it_gr_fonte_recurso_13${TAB},it_gr_fonte_recurso_14${TAB},it_gr_fonte_recurso_15${TAB},it_gr_fonte_recurso_16${TAB},it_gr_fonte_recurso_17${TAB},it_gr_fonte_recurso_18${TAB},it_gr_fonte_recurso_19${TAB},it_gr_fonte_recurso_20${TAB},it_gr_fonte_recurso_21${TAB},it_gr_fonte_recurso_22${TAB},it_gr_fonte_recurso_23${TAB},it_gr_fonte_recurso_24${TAB},it_gr_fonte_recurso_25${TAB},it_gr_fonte_recurso_26${TAB},it_gr_fonte_recurso_27${TAB},it_gr_fonte_recurso_28${TAB},it_gr_fonte_recurso_29${TAB},it_gr_fonte_recurso_30${TAB},it_gr_fonte_recurso_31${TAB},it_gr_fonte_recurso_32${TAB},it_co_vinc_pagamento_01${TAB},it_co_vinc_pagamento_02${TAB},it_co_vinc_pagamento_03${TAB},it_co_vinc_pagamento_04${TAB},it_co_vinc_pagamento_05${TAB},it_co_vinc_pagamento_06${TAB},it_co_vinc_pagamento_07${TAB},it_co_vinc_pagamento_08${TAB},it_co_vinc_pagamento_09${TAB},it_co_vinc_pagamento_10${TAB},it_co_vinc_pagamento_11${TAB},it_co_vinc_pagamento_12${TAB},it_co_vinc_pagamento_13${TAB},it_co_vinc_pagamento_14${TAB},it_co_vinc_pagamento_15${TAB},it_co_vinc_pagamento_16${TAB},it_co_vinc_pagamento_17${TAB},it_co_vinc_pagamento_18${TAB},it_co_vinc_pagamento_19${TAB},it_co_vinc_pagamento_20${TAB},it_co_vinc_pagamento_21${TAB},it_co_vinc_pagamento_22${TAB},it_co_vinc_pagamento_23${TAB},it_co_vinc_pagamento_24${TAB},it_co_vinc_pagamento_25${TAB},it_co_vinc_pagamento_26${TAB},it_co_vinc_pagamento_27${TAB},it_co_vinc_pagamento_28${TAB},it_co_vinc_pagamento_29${TAB},it_co_vinc_pagamento_30${TAB},it_co_vinc_pagamento_31${TAB},it_co_vinc_pagamento_32${TAB},it_va_favorecido_01${TAB},it_va_favorecido_02${TAB},it_va_favorecido_03${TAB},it_va_favorecido_04${TAB},it_va_favorecido_05${TAB},it_va_favorecido_06${TAB},it_va_favorecido_07${TAB},it_va_favorecido_08${TAB},it_va_favorecido_09${TAB},it_va_favorecido_10${TAB},it_va_favorecido_11${TAB},it_va_favorecido_12${TAB},it_va_favorecido_13${TAB},it_va_favorecido_14${TAB},it_va_favorecido_15${TAB},it_va_favorecido_16${TAB},it_va_favorecido_17${TAB},it_va_favorecido_18${TAB},it_va_favorecido_19${TAB},it_va_favorecido_20${TAB},it_va_favorecido_21${TAB},it_va_favorecido_22${TAB},it_va_favorecido_23${TAB},it_va_favorecido_24${TAB},it_va_favorecido_25${TAB},it_va_favorecido_26${TAB},it_va_favorecido_27${TAB},it_va_favorecido_28${TAB},it_va_favorecido_29${TAB},it_va_favorecido_30${TAB},it_va_favorecido_31${TAB},it_va_favorecido_32${TAB},it_nu_ob_01${TAB},it_nu_ob_02${TAB},it_nu_ob_03${TAB},it_nu_ob_04${TAB},it_nu_ob_05${TAB},it_nu_ob_06${TAB},it_nu_ob_07${TAB},it_nu_ob_08${TAB},it_nu_ob_09${TAB},it_nu_ob_10${TAB},it_nu_ob_11${TAB},it_nu_ob_12${TAB},it_nu_ob_13${TAB},it_nu_ob_14${TAB},it_nu_ob_15${TAB},it_nu_ob_16${TAB},it_nu_ob_17${TAB},it_nu_ob_18${TAB},it_nu_ob_19${TAB},it_nu_ob_20${TAB},it_nu_ob_21${TAB},it_nu_ob_22${TAB},it_nu_ob_23${TAB},it_nu_ob_24${TAB},it_nu_ob_25${TAB},it_nu_ob_26${TAB},it_nu_ob_27${TAB},it_nu_ob_28${TAB},it_nu_ob_29${TAB},it_nu_ob_30${TAB},it_nu_ob_31${TAB},it_nu_ob_32${TAB},it_nu_ob_cancelada_01${TAB},it_nu_ob_cancelada_02${TAB},it_nu_ob_cancelada_03${TAB},it_nu_ob_cancelada_04${TAB},it_nu_ob_cancelada_05${TAB},it_nu_ob_cancelada_06${TAB},it_nu_ob_cancelada_07${TAB},it_nu_ob_cancelada_08${TAB},it_nu_ob_cancelada_09${TAB},it_nu_ob_cancelada_10${TAB},it_nu_ob_cancelada_11${TAB},it_nu_ob_cancelada_12${TAB},it_nu_ob_cancelada_13${TAB},it_nu_ob_cancelada_14${TAB},it_nu_ob_cancelada_15${TAB},it_nu_ob_cancelada_16${TAB},it_nu_ob_cancelada_17${TAB},it_nu_ob_cancelada_18${TAB},it_nu_ob_cancelada_19${TAB},it_nu_ob_cancelada_20${TAB},it_nu_ob_cancelada_21${TAB},it_nu_ob_cancelada_22${TAB},it_nu_ob_cancelada_23${TAB},it_nu_ob_cancelada_24${TAB},it_nu_ob_cancelada_25${TAB},it_nu_ob_cancelada_26${TAB},it_nu_ob_cancelada_27${TAB},it_nu_ob_cancelada_28${TAB},it_nu_ob_cancelada_29${TAB},it_nu_ob_cancelada_30${TAB},it_nu_ob_cancelada_31${TAB},it_nu_ob_cancelada_32${TAB},it_gr_domicilio_pagadora_01${TAB},it_gr_domicilio_pagadora_02${TAB},it_gr_domicilio_pagadora_03${TAB},it_gr_domicilio_pagadora_04${TAB},it_gr_domicilio_pagadora_05${TAB},it_gr_domicilio_pagadora_06${TAB},it_gr_domicilio_pagadora_07${TAB},it_gr_domicilio_pagadora_08${TAB},it_gr_domicilio_pagadora_09${TAB},it_gr_domicilio_pagadora_10${TAB},it_gr_domicilio_pagadora_11${TAB},it_gr_domicilio_pagadora_12${TAB},it_gr_domicilio_pagadora_13${TAB},it_gr_domicilio_pagadora_14${TAB},it_gr_domicilio_pagadora_15${TAB},it_gr_domicilio_pagadora_16${TAB},it_gr_domicilio_pagadora_17${TAB},it_gr_domicilio_pagadora_18${TAB},it_gr_domicilio_pagadora_19${TAB},it_gr_domicilio_pagadora_20${TAB},it_gr_domicilio_pagadora_21${TAB},it_gr_domicilio_pagadora_22${TAB},it_gr_domicilio_pagadora_23${TAB},it_gr_domicilio_pagadora_24${TAB},it_gr_domicilio_pagadora_25${TAB},it_gr_domicilio_pagadora_26${TAB},it_gr_domicilio_pagadora_27${TAB},it_gr_domicilio_pagadora_28${TAB},it_gr_domicilio_pagadora_29${TAB},it_gr_domicilio_pagadora_30${TAB},it_gr_domicilio_pagadora_31${TAB},it_gr_domicilio_pagadora_32${TAB},it_gr_gestao_lista_origem${TAB},it_gr_gestao_lista_destino${TAB},it_gr_co_usuario_tran${TAB},it_gr_ug_operador_tran${TAB},it_gr_ug_gestao_transferencia${TAB},it_nu_lista_transferencia${TAB},filler ) FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^OL*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
 
	# 00001 00011  ALFANUM IT-CO-USUARIO
	CAMPO01=${LINHA:0:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
	
	# 00012 00008  ALFANUM TERMINAL DO USUARIO
	CAMPO02=${LINHA:11:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	# 00020 00008  NUM     DATA DE TRANSACAO(DDMMAAAA)
	CAMPO03=${LINHA:23:4}${LINHA:21:2}${LINHA:19: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	# 00028 00004  NUM     HORA TRANSACAO(HHMM)
	CAMPO04=${LINHA:27:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00032 00006  NUM     CODIGO DA UG DO OPERADOR
	CAMPO05=${LINHA:31:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi

	# 00038 00001  ALFANUM IT-IN-OPERACAO
	CAMPO06=${LINHA:37:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	
	# 00039 00023  ALFANUM GR-UG-GESTAO-AN-NUMERO-LO
	CAMPO07=${LINHA:38:23}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	
	# 00062 00008  NUM     IT-NU-ISN-DOC-HABIL
	CAMPO08=${LINHA:61:8}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	
	# 00070 00001  ALFANUM IT-IN-SITUACAO-LISTA
	CAMPO09=${LINHA:69:1}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	
	# 00071 00017  NUM     IT-VA-TOTAL-LISTA
	CAMPO10=${LINHA:70:17}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
	# 00088 00448  ALFANUM IT-CO-FAVORECIDO (32 OCORRENCIAS FORMATO A14)
	multiplicos 32 14 ${LINHA:87:448}; CAMPO11=$valores;
	
	# 00536 00168  NUM     IT-CO-GESTAO (32 OCORRENCIAS FORMATO N5)
	multiplicos 32 5 ${LINHA:535:168}; CAMPO12=$valores;
	
	# 00704 00544  ALFANUM GR-DOMICILIO-FAVORECIDO (32 OCORRENCIAS FORMATO A17)
	multiplicos 32 17 ${LINHA:703:544}; CAMPO13=$valores;
	
	# 01248 00384  ALFANUM IT-NU-LISTA (32 OCORRENCIAS FORMATO A12)
	multiplicos 32 12 ${LINHA:1248:384}; CAMPO14=$valores;
	
	# 01632 00800  ALFANUM IT-CO-IDENT-TRANSFERENCIA (32 OCORRENCIAS FORMATO A25)
	multiplicos 32 25 ${LINHA:1631:800}; CAMPO15=$valores;
	
	# 02432 01600  ALFANUM GR-FONTE-RECURSO (32 OCORRENCIAS FORMATO N50)
	multiplicos 32 50 ${LINHA:2431:1600}; CAMPO16=$valores;
	
	# 04032 00480  NUM     IT-CO-VINC-PAGAMENTO (32 OCORRENCIAS FORMATO N15)
	multiplicos 32 15 ${LINHA:4031:480}; CAMPO17=$valores;
	
	# 04512 02720  NUM     IT-VA-FAVORECIDO (32 OCORRENCIAS FORMATO N85)
	multiplicos 32 85 ${LINHA:4511:2720}; CAMPO18=$valores;
	
	# 07232 00384  ALFANUM IT-NU-OB (32 OCORRENCIAS FORMATO A12)
	multiplicos 32 12 ${LINHA:7231:384}; CAMPO19=$valores;
	
	# 07616 00384  ALFANUM IT-NU-OB-CANCELADA (32 OCORRENCIAS FORMATO A12)
	multiplicos 32 12 ${LINHA:7615:384}; CAMPO20=$valores;
	
	# 08000 00544  ALFANUM GR-DOMICILIO-PAGADORA (32 OCORRENCIAS FORMATO A17)
	multiplicos 32 17 ${LINHA:7999:544}; CAMPO21=$valores;
	
	# 08544 00023  ALFANUM GR-UG-GESTAO-LISTA-ORIGEM
	CAMPO22=${LINHA:8543:23}; if [ "$CAMPO22" ]; then CAMPO22=${NULO}; fi
	
	# 08567 00023  ALFANUM GR-UG-GESTAO-LISTA-DESTINO
	CAMPO23=${LINHA:8566:23}; if [ "$CAMPO23" ]; then CAMPO23=${NULO}; fi
	
	# 08590 00011  ALFANUM IT-CO-USUARIO-TRAN
	CAMPO24=${LINHA:8589:11}; if [ -z "$CAMPO24" ]; then CAMPO24=${NULO}; fi
	
	# 08601 00006  NUM     IT-CO-UG-OPERADOR-TRAN
	CAMPO25=${LINHA:8600:6}; if [ "$CAMPO25" ]; then CAMPO25=${NULO}; fi
	
	# 08607 00011  ALFANUM GR-UG-GESTAO-TRANSFERENCIA
	CAMPO26=${LINHA:8606:11}; if [ -z "$CAMPO26" ]; then CAMPO26=${NULO}; fi
	
	# 08618 00006  NUM     IT-NU-LISTA-TRANSFERENCIA
	CAMPO27=${LINHA:8617:6}; if [ -z "$CAMPO27" ]; then CAMPO27=${NULO}; fi
	
	# 08624 00177  ALFANUM FILLER
	CAMPO28=${LINHA:8623:177}; if [ -z "$CAMPO28" ]; then CAMPO28=${NULO}; fi
	
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