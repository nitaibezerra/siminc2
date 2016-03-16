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
echo  "COPY siafi.convenioliberacao(  it_co_usuario, it_co_terminal_usuario, it_da_transacao, it_ho_transacao, it_co_ug_operador, it_nu_convenio, it_nu_parcela, gr_ug_gestao_an_numero_doc, it_da_emissao, it_da_prevista_comprovacao, it_va_liberado, it_va_liberado_dolar, it_in_situacao_doc )   FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^DL*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
	# 00001  00011  NUM     IT-CO-USUARIO
	CAMPO01=${LINHA:0:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00012  00008  ALFANUM IT-CO-TERMINAL-USUARIO
	CAMPO02=${LINHA:11:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi

	# 00020  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
	CAMPO03=${LINHA:23:4}${LINHA:21: 2}${LINHA:19:2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi

	# 00028  00004  NUM     IT-HO-TRANSACAO
	CAMPO04=${LINHA:27:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00032  00006  NUM     IT-CO-UG-OPERADOR
	CAMPO05=${LINHA:31:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
 	# 00038  00006  NUM     IT-NU-CONVENIO
	CAMPO06=${LINHA:37:6}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi

	# 00044  00003  NUM     IT-NU-PARCELA
	CAMPO07=${LINHA:43:3}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi

 	# 00047  00023  ALFANUM GR-UG-GESTAO-AN-NUMERO-DOC
	CAMPO08=${LINHA:46:23}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi

	# 00070  00008  NUM     IT-DA-EMISSAO (DDMMAAAA)
	CAMPO09=${LINHA:73:4}${LINHA:71:2}${LINHA:69:2}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
 
 	# 00078  00008  NUM     IT-DA-PREVISTA-COMPROVACAO (DDMMAAAA)
	CAMPO10=${LINHA:81:4}${LINHA:79:2}${LINHA:77:2}; if [ -z $CAMPO10 ]; then CAMPO10=${NULO};  fi
 
	# 00086  00017  NUM     IT-VA-LIBERADO (N15,2)
	CAMPO11=${LINHA:85:17};
	if [ -z $CAMPO11 ]; then
	CAMPO11=${NULO};
	else
		valor=${LINHA:101:1};
		contasNum $valor;
		if  [ $operadorValor -ne 99  ]; then
			CAMPO11="-"${LINHA:85:15}"."${LINHA:100:1}$operadorValor
		else
			CAMPO11=${LINHA:85:15}"."${LINHA:100:2}
		fi
	fi 
 
 	# 00103  00018  NUM     IT-VA-LIBERADO-DOLAR (N14,4)
	CAMPO12=${LINHA:102:18};
	if [ -z $CAMPO12 ]; then
	CAMPO12=${NULO};
	else
		valor=${LINHA:119:1};
		contasNum $valor;
		if  [ $operadorValor -ne 99  ]; then
			CAMPO12="-"${LINHA:102:14}"."${LINHA:116:1}$operadorValor
		else
			CAMPO12=${LINHA:102:14}"."${LINHA:116:4}
		fi
	fi 

 	# 00121  00001  ALFANUM IT-IN-SITUACAO-DOC
 	CAMPO13=${LINHA:120:1};
	if [ -z "$CAMPO13" ]; then 
	CAMPO13=${NULO}; 
	else
	CAMPO13=${LINHA:120:1};
	fi
	
	REGBANCOORIGINAL=`expr $REGBANCOORIGINAL + 1`;
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13 >> ${FILE}.sql; 
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