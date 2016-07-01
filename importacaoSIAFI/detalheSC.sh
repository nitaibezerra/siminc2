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
echo  "COPY siafi.saldocontabil(it_an_qdd, it_co_unidade_gestora, it_co_gestao, gr_codigo_conta, it_co_conta_corrente_contabil, it_va_debito_inicial, it_va_credito_inicial, it_da_transacao, it_ho_transacao, it_va_debito_inicial_cambio, it_va_credito_inicial_cambio, it_va_debito_mensal_01, it_va_debito_mensal_02, it_va_debito_mensal_03, it_va_debito_mensal_04, it_va_debito_mensal_05, it_va_debito_mensal_06, it_va_debito_mensal_07, it_va_debito_mensal_08, it_va_debito_mensal_09, it_va_debito_mensal_10, it_va_debito_mensal_11, it_va_debito_mensal_12, it_va_debito_mensal_13, it_va_debito_mensal_14, it_va_credito_mensal_01, it_va_credito_mensal_02, it_va_credito_mensal_03, it_va_credito_mensal_04, it_va_credito_mensal_05, it_va_credito_mensal_06, it_va_credito_mensal_07, it_va_credito_mensal_08, it_va_credito_mensal_09, it_va_credito_mensal_10, it_va_credito_mensal_11, it_va_credito_mensal_12, it_va_credito_mensal_13, it_va_credito_mensal_14, it_va_debito_mensal_cambio_01, it_va_debito_mensal_cambio_02, it_va_debito_mensal_cambio_03, it_va_debito_mensal_cambio_04, it_va_debito_mensal_cambio_05, it_va_debito_mensal_cambio_06, it_va_debito_mensal_cambio_07, it_va_debito_mensal_cambio_08, it_va_debito_mensal_cambio_09, it_va_debito_mensal_cambio_10, it_va_debito_mensal_cambio_11, it_va_debito_mensal_cambio_12, it_va_debito_mensal_cambio_13, it_va_debito_mensal_cambio_14, it_va_credito_mensal_cambio_01, it_va_credito_mensal_cambio_02, it_va_credito_mensal_cambio_03, it_va_credito_mensal_cambio_04, it_va_credito_mensal_cambio_05, it_va_credito_mensal_cambio_06, it_va_credito_mensal_cambio_07, it_va_credito_mensal_cambio_08, it_va_credito_mensal_cambio_09, it_va_credito_mensal_cambio_10, it_va_credito_mensal_cambio_11, it_va_credito_mensal_cambio_12, it_va_credito_mensal_cambio_13, it_va_credito_mensal_cambio_14)    FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^SC*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
ITANQDD=${LINHA:0:2};
	if [ -z $ITANQDD ]; then
	ITANQDD=${NULO};
	fi

ITCOUNIDADEGESTORA=${LINHA:2:6};
	if [ -z $ITCOUNIDADEGESTORA ]; then
	ITCOUNIDADEGESTORA=${NULO};
	fi

ITCOGESTAO=${LINHA:8:5};
	if [ -z $ITCOGESTAO ]; then
	ITCOGESTAO=${NULO};
	fi

GRCODIGOCONTA=${LINHA:13:9};
	if [ -z $GRCODIGOCONTA ]; then
	GRCODIGOCONTA=${NULO};
	fi

	ITCOCONTACORRENTECONTABIL=${LINHA:22:43};
	if [ -z "$ITCOCONTACORRENTECONTABIL" ]; then
		ITCOCONTACORRENTECONTABIL=${NULO};
	fi

ITVADEBITOINICIAL=${LINHA:65:18};
	if [ -z $ITVADEBITOINICIAL ]; then
	ITVADEBITOINICIAL=${NULO};
	else
		valor=${LINHA:82:1};
		contasNum $valor;
		if  [ $operadorValor -ne 99  ]; then
			ITVADEBITOINICIAL="-"${LINHA:65:16}"."${LINHA:81:1}$operadorValor
		else
			ITVADEBITOINICIAL=${LINHA:65:16}"."${LINHA:81:2}
		fi
	fi

ITVACREDITOINICIAL=${LINHA:83:18};
	if [ -z $ITVACREDITOINICIAL ]; then
	ITVACREDITOINICIAL=${NULO};
	else
		valor=${LINHA:100:1};
		contasNum $valor;
		if  [ $operadorValor -ne 99  ]; then
			ITVACREDITOINICIAL="-"${LINHA:83:16}"."${LINHA:99:1};$operadorValor
		else
			ITVACREDITOINICIAL=${LINHA:83:16}"."${LINHA:99:2}
		fi
	fi

ITVADEBITOMENSALOCORRE14VEZESDE=${LINHA:101:252};
	if [ -z $ITVADEBITOMENSALOCORRE14VEZESDE ]; then
	strinDebitoMensal=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=18;
		strinDebitoMensal='';
		while [ $n -le 13 ]; do
			campo=${ITVADEBITOMENSALOCORRE14VEZESDE:$x:$y};
			valor=${ITVADEBITOMENSALOCORRE14VEZESDE:$x+17:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99  ]; then
				strinDebitoMensal=${strinDebitoMensal}"-"${ITVADEBITOMENSALOCORRE14VEZESDE:$x:16}"."${ITVADEBITOMENSALOCORRE14VEZESDE:$x+16:1}$operadorValor'\t';
			else
				strinDebitoMensal=${strinDebitoMensal}${ITVADEBITOMENSALOCORRE14VEZESDE:$x:16}"."${ITVADEBITOMENSALOCORRE14VEZESDE:$x+16:2}'\t';
			fi
			let n++;
			x=$x+$y;
		done
	fi

ITVACREDITOMENSALOCORRE14VEZESDE=${LINHA:353:252};
	if [ -z $ITVACREDITOMENSALOCORRE14VEZESDE ]; then
	strinCreditoMensal=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=18;
		strinCreditoMensal='';
		while [ $n -le 13 ]; do
			campo=${ITVACREDITOMENSALOCORRE14VEZESDE:$x:$y};
			valor=${ITVACREDITOMENSALOCORRE14VEZESDE:$x+17:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99  ]; then
				strinCreditoMensal=${strinCreditoMensal}"-"${ITVACREDITOMENSALOCORRE14VEZESDE:$x:16}"."${ITVACREDITOMENSALOCORRE14VEZESDE:$x+16:1}$operadorValor'\t';
			else
				strinCreditoMensal=${strinCreditoMensal}${ITVACREDITOMENSALOCORRE14VEZESDE:$x:16}"."${ITVACREDITOMENSALOCORRE14VEZESDE:$x+16:2}'\t';
			fi
			let n++;
			x=$x+$y;
		done
	fi

ITDATRANSACAO=${LINHA:605:8};
	if [ -z $ITDATRANSACAO ]; then
	ITDATRANSACAO=${NULO};
	else
	ITDATRANSACAO=${LINHA:605:2}"/"${LINHA:607:2}"/"${LINHA:609:4};
	fi

ITHOTRANSACAO=${LINHA:613:6};
	if [ -z $ITHOTRANSACAO ]; then
	ITHOTRANSACAO=${NULO};
	else
	ITHOTRANSACAO=${LINHA:613:2}":"${LINHA:615:2}":"${LINHA:617:2};
	fi

ITVADEBITOINICIALCAMBIO=${LINHA:619:18};
	if [ -z $ITVADEBITOINICIALCAMBIO ]; then
	ITVADEBITOINICIALCAMBIO=${NULO};
	else
		valor=${LINHA:636:1};
		contasNum $valor;
		if  [ $operadorValor -ne 99  ]; then
			ITVADEBITOINICIALCAMBIO="-"${LINHA:619:16}"."${LINHA:635:1}$operadorValor
		else
			ITVADEBITOINICIALCAMBIO=${LINHA:619:16}"."${LINHA:635:2}
		fi
	fi

ITVACREDITOINICIALCAMBIO=${LINHA:637:18};
	if [ -z $ITVACREDITOINICIALCAMBIO ]; then
	ITVACREDITOINICIALCAMBIO=${NULO};
	else
		valor=${LINHA:654:1};
		contasNum $valor;
		if  [ $operadorValor -ne 99  ]; then
			ITVACREDITOINICIALCAMBIO="-"${LINHA:637:16}"."${LINHA:653:1}$operadorValor
		else
			ITVACREDITOINICIALCAMBIO=${LINHA:637:16}"."${LINHA:653:2}
		fi
	fi

ITVADEBITOMENSALCAMBIO14VEZESDE=${LINHA:655:252};
	if [ -z $ITVADEBITOMENSALCAMBIO14VEZESDE ]; then
	strinDebitoMensalCambio=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=18;
		strinDebitoMensalCambio='';
		while [ $n -le 13 ]; do
			campo=${ITVADEBITOMENSALCAMBIO14VEZESDE:$x:$y};
			valor=${ITVADEBITOMENSALCAMBIO14VEZESDE:$x+17:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99  ]; then
				strinDebitoMensalCambio=${strinDebitoMensalCambio}"-"${ITVADEBITOMENSALCAMBIO14VEZESDE:$x:16}"."${ITVADEBITOMENSALCAMBIO14VEZESDE:$x+16:1}$operadorValor'\t';
			else
				strinDebitoMensalCambio=${strinDebitoMensalCambio}${ITVADEBITOMENSALCAMBIO14VEZESDE:$x:16}"."${ITVADEBITOMENSALCAMBIO14VEZESDE:$x+16:2}'\t';
			fi
			let n++;
			x=$x+$y;
		done
	fi

ITVACREDITOMENSALCAMBIO14VEZESDE=${LINHA:907:252};
	if [ -z $ITVACREDITOMENSALCAMBIO14VEZESDE ]; then
	strinCreditoMensalCambio=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=18;
		strinCreditoMensalCambio='';
		while [ $n -le 13 ]; do
			campo=${ITVACREDITOMENSALCAMBIO14VEZESDE:$x:$y};
			valor=${ITVACREDITOMENSALCAMBIO14VEZESDE:$x+17:1};
			contasNum $valor;
			if  [ $operadorValor -ne 99  ]; then
				strinCreditoMensalCambio=${strinCreditoMensalCambio}"-"${ITVACREDITOMENSALCAMBIO14VEZESDE:$x:16}"."${ITVACREDITOMENSALCAMBIO14VEZESDE:$x+16:1}$operadorValor;
			else
				strinCreditoMensalCambio=${strinCreditoMensalCambio}${ITVACREDITOMENSALCAMBIO14VEZESDE:$x:16}"."${ITVACREDITOMENSALCAMBIO14VEZESDE:$x+16:2};
			fi
			let n++;
			x=$x+$y;
			if [ $n -ne 14 ];then
				strinCreditoMensalCambio=$strinCreditoMensalCambio'\t'
			fi 
		done
	fi


echo -e $ITANQDD${TAB}$ITCOUNIDADEGESTORA${TAB}$ITCOGESTAO${TAB}$GRCODIGOCONTA${TAB}$ITCOCONTACORRENTECONTABIL${TAB}$ITVADEBITOINICIAL${TAB}$ITVACREDITOINICIAL${TAB}$ITDATRANSACAO${TAB}$ITHOTRANSACAO${TAB}$ITVADEBITOINICIALCAMBIO${TAB}$ITVACREDITOINICIALCAMBIO${TAB}${strinDebitoMensal}${strinCreditoMensal}${strinDebitoMensalCambio}${strinCreditoMensalCambio} >> ${FILE}.sql;
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