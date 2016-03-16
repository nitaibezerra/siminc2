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
echo -e  "COPY siafi.nt ( cpf_usuario${TAB},terminal_usuario${TAB},data_transacao${TAB},hora_transacao${TAB},codigo_ug_operador${TAB},numero_nc${TAB},data_emissao${TAB},codigo_ug_favorecida${TAB},codigo_gestao_favorecida${TAB},operacao_cambial${TAB},observacao${TAB},codigo_evento_01${TAB},codigo_evento_02${TAB},codigo_evento_03${TAB},codigo_evento_04${TAB},codigo_evento_05${TAB},codigo_evento_06${TAB},codigo_evento_07${TAB},codigo_evento_08${TAB},codigo_evento_09${TAB},codigo_evento_10${TAB},codigo_evento_11${TAB},codigo_evento_12${TAB},esfera_orcamentaria_01${TAB},esfera_orcamentaria_02${TAB},esfera_orcamentaria_03${TAB},esfera_orcamentaria_04${TAB},esfera_orcamentaria_05${TAB},esfera_orcamentaria_06${TAB},esfera_orcamentaria_07${TAB},esfera_orcamentaria_08${TAB},esfera_orcamentaria_09${TAB},esfera_orcamentaria_10${TAB},esfera_orcamentaria_11${TAB},esfera_orcamentaria_12${TAB},ptres_01${TAB},ptres_02${TAB},ptres_03${TAB},ptres_04${TAB},ptres_05${TAB},ptres_06${TAB},ptres_07${TAB},ptres_08${TAB},ptres_09${TAB},ptres_10${TAB},ptres_11${TAB},ptres_12${TAB},fonte_recurso_01${TAB},fonte_recurso_02${TAB},fonte_recurso_03${TAB},fonte_recurso_04${TAB},fonte_recurso_05${TAB},fonte_recurso_06${TAB},fonte_recurso_07${TAB},fonte_recurso_08${TAB},fonte_recurso_09${TAB},fonte_recurso_10${TAB},fonte_recurso_11${TAB},fonte_recurso_12${TAB},natureza_despesa_01${TAB},natureza_despesa_02${TAB},natureza_despesa_03${TAB},natureza_despesa_04${TAB},natureza_despesa_05${TAB},natureza_despesa_06${TAB},natureza_despesa_07${TAB},natureza_despesa_08${TAB},natureza_despesa_09${TAB},natureza_despesa_10${TAB},natureza_despesa_11${TAB},natureza_despesa_12${TAB},codigo_ug_responsavel_01${TAB},codigo_ug_responsavel_02${TAB},codigo_ug_responsavel_03${TAB},codigo_ug_responsavel_04${TAB},codigo_ug_responsavel_05${TAB},codigo_ug_responsavel_06${TAB},codigo_ug_responsavel_07${TAB},codigo_ug_responsavel_08${TAB},codigo_ug_responsavel_09${TAB},codigo_ug_responsavel_10${TAB},codigo_ug_responsavel_11${TAB},codigo_ug_responsavel_12${TAB},plano_interno_01${TAB},plano_interno_02${TAB},plano_interno_03${TAB},plano_interno_04${TAB},plano_interno_05${TAB},plano_interno_06${TAB},plano_interno_07${TAB},plano_interno_08${TAB},plano_interno_09${TAB},plano_interno_10${TAB},plano_interno_11${TAB},plano_interno_12${TAB},valor_transacao_01${TAB},valor_transacao_02${TAB},valor_transacao_03${TAB},valor_transacao_04${TAB},valor_transacao_05${TAB},valor_transacao_06${TAB},valor_transacao_07${TAB},valor_transacao_08${TAB},valor_transacao_09${TAB},valor_transacao_10${TAB},valor_transacao_11${TAB},valor_transacao_12${TAB},mes_lancamento${TAB},it_nu_original${TAB},it_co_sistema_origem${TAB},it_co_subitem_01${TAB},it_co_subitem_02${TAB},it_co_subitem_03${TAB},it_co_subitem_04${TAB},it_co_subitem_05${TAB},it_co_subitem_06${TAB},it_co_subitem_07${TAB},it_co_subitem_08${TAB},it_co_subitem_09${TAB},it_co_subitem_10${TAB},it_co_subitem_11${TAB},it_co_subitem_12 ) FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^NT*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
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
	
	# 00038 00023  ALFANUM NUMERO DA NT
	CAMPO06=${LINHA:37:23}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
	
	# 00061 00008  NUM     DATA DE EMISSAO(DDMMAAAA)
	CAMPO07=${LINHA:64:4}${LINHA:62:2}${LINHA:60:2}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	
	# 00069 00008  NUM     DATA VALORIZACAO(DDMMAAAA)
	CAMPO08=${LINHA:72:4}${LINHA:70:2}${LINHA:68:2}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	
 	# 00077 00017  ALFANUM NUMERO DO PROCESSO
	CAMPO09=${LINHA:76:17}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	
 	# 00094 00234  ALFANUM OBSERVACAO
	CAMPO10=${LINHA:93:234}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
 	# 00328 00072  NUM     CODIGO DO EVENTO  OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 6 BYTES
	CAMPO11=${LINHA:327:72}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
	
	#  00400 00048  NUM     CODIGO DA RECEITA OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 4 BYTES
	multiplicos 12 4 ${LINHA:399:48}; CAMPO12=$valores;
	
	#  00448 00168  ALFANUM NUMERO CGC OU CPF__OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 14 BYTES
	multiplicos 12 14 ${LINHA:447:168}; CAMPO13=$valores;
	
	#  00616 00096  NUM     DATA DE VENCIMENTO_OBS.: OCORRE 12 VEZES TAMANHO DE CADA CAMPO 8 BYTES
	multiplicos 12 8 ${LINHA:615:96}; CAMPO14=$valores;
	
	#  00712 00204  ALFANUM NUMERO DO PROCESSO DE COBRANCA OBS.:OCORRE 12 VEZES TAMANHO DE CADA CAMPO 17 BYTES
	multiplicos 12 17 ${LINHA:615:96}; CAMPO15=$valores;
	
	#  00916 00204  NUM     VALOR DA TRANSACAO__OBS.:OCORRE 12 VEZES TAMANHO DE CADA CAMPO 17 BYTES_(15,2)
	multiplicosFloat 12 15 2 ${LINHA:915:204}; CAMPO16=$valores;
	
	#  01120 00017  NUM     VALOR DO TOTAL DA RESTITUICAO_(15,2)
	multiplicosFloat 1 15 2 ${LINHA:1119:17}; CAMPO17=$valores;
	
	#  01137 00002  NUM     MES DE LANCAMENTO
	CAMPO18=${LINHA:1136:2}; if [ -z "$CAMPO18" ]; then CAMPO18=${NULO}; fi
	
	#  01139 00010  ALFANUM CODIGO DO SISTEMA DE ORIGEM
	CAMPO19=${LINHA:1138:10}; if [ -z "$CAMPO19" ]; then CAMPO19=${NULO}; fi
	
	#  01149 00001  NUM     CANCELAMENTO NT
	CAMPO20=${LINHA:1148:1}; if [ -z "$CAMPO20" ]; then CAMPO20=${NULO}; fi
	
	#  01150 00004  NUM     NUMERO DA REMESSA
	CAMPO21=${LINHA:1149:4}; if [ -z "$CAMPO21" ]; then CAMPO21=${NULO}; fi
	
	#  01154 00006  NUM     NT CANCELAMENTO
	CAMPO22=${LINHA:1153:6}; if [ -z "$CAMPO22" ]; then CAMPO22=${NULO}; fi
	
echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17${TAB}$CAMPO18${TAB}$CAMPO19${TAB}$CAMPO20${TAB}$CAMPO21${TAB}$CAMPO22 >> ${FILE}.sql;


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