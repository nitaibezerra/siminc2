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
echo  "COPY siafi.creditodescentralizado(gr_orgao_superior_emitente, gr_orgao_executor_emitente, it_co_unidade_gestora_emitente, it_co_gestao, it_in_esfera_orcamentaria, gr_unidade_orcamentaria, gr_programa_trabalho, it_co_fonte_recurso, gr_natureza_despesa, it_co_unidade_gestora_responsavel, it_co_plano_interno, gr_codigo_conta, it_va_saldo_inicial, it_va_saldo_jan, it_va_saldo_fev, it_va_saldo_mar, it_va_saldo_abr, it_va_saldo_mai, it_va_saldo_jun, it_va_saldo_jul, it_va_saldo_ago, it_va_saldo_set, it_va_saldo_out, it_va_saldo_nov, it_va_saldo_dez)   FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^CD*] | grep ^[^00*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
#echo ${#LINHA}
ORGAOSUPERIOR=${LINHA:0:5};
	if [ -z $ORGAOSUPERIOR ]; then
	ORGAOSUPERIOR=${NULO};
	fi

ORGAOEXECUTOR=${LINHA:5:5};
	if [ -z $ORGAOEXECUTOR ]; then
	ORGAOEXECUTOR=${NULO};
	fi

UNIDADEGESTORA=${LINHA:10:6};
	if [ -z $UNIDADEGESTORA ]; then
	UNIDADEGESTORA=${NULO};
	fi

GESTAO=${LINHA:16:5};
	if [ -z $GESTAO ]; then
	GESTAO=${NULO};
	fi

ESFERA=${LINHA:21:1};
	if [ -z $ESFERA ]; then
	ESFERA=${NULO};
	fi

UNIDADEORCAMENTARIA=${LINHA:22:5};
	if [ -z $UNIDADEORCAMENTARIA ]; then
	UNIDADEORCAMENTARIA=${NULO};
	fi

PROGRAMATRABALHO=${LINHA:27:17};
	if [ -z $PROGRAMATRABALHO ]; then
	PROGRAMATRABALHO=${NULO};
	fi

FONTE=${LINHA:44:10};
	if [ -z $FONTE ]; then
	FONTE=${NULO};
	fi

NATUREZADESPESA=${LINHA:54:8};
	if [ -z $NATUREZADESPESA ]; then
	NATUREZADESPESA=${NULO};
	fi

UNIDADEGESTORARESPONSAVEL=${LINHA:62:6};
	if [ -z $UNIDADEGESTORARESPONSAVEL ]; then
	UNIDADEGESTORARESPONSAVEL=${NULO};
	fi

PLANOINTERNO=${LINHA:68:11};
	if [ -z $PLANOINTERNO ]; then
	PLANOINTERNO=${NULO};
	fi

CODIGOCONTA=${LINHA:79:9};
	if [ -z "$CODIGOCONTA" ]; then
	CODIGOCONTA=${NULO};
	fi

SALDOINICIAL=${LINHA:88:18};
if [ -z "$SALDOINICIAL" ]; then
	SALDOINICIAL=${NULO};
else
	stValor=SALDOINICIAL;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOINICIAL="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOINICIAL=${stValor:0:15}"."${stValor:16:2}
	fi
fi


SALDOJANEIRO=${LINHA:106:18};
if [ -z "$SALDOJANEIRO" ]; then
	SALDOJANEIRO=${NULO};
else
	stValor=SALDOJANEIRO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOJANEIRO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOJANEIRO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOFEVEREIRO=${LINHA:124:18};
if [ -z "$SALDOFEVEREIRO" ]; then
	SALDOFEVEREIRO=${NULO};
else
	stValor=SALDOFEVEREIRO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOFEVEREIRO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOFEVEREIRO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOMARCO=${LINHA:142:18};
if [ -z "$SALDOMARCO" ]; then
	SALDOMARCO=${NULO};
else
	stValor=SALDOMARCO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOMARCO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOMARCO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOABRIL=${LINHA:160:18};
if [ -z "$SALDOABRIL" ]; then
	SALDOABRIL=${NULO};
else
	stValor=SALDOABRIL;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOABRIL="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOABRIL=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOMAIO=${LINHA:178:18};
if [ -z "$SALDOMAIO" ]; then
	SALDOMAIO=${NULO};
else
	stValor=SALDOMAIO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOMAIO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOMAIO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOJUNHO=${LINHA:196:18};
if [ -z "$SALDOJUNHO" ]; then
	SALDOJUNHO=${NULO};
else
	stValor=SALDOJUNHO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOJUNHO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOJUNHO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOJULHO=${LINHA:214:18};
if [ -z "$SALDOJULHO" ]; then
	SALDOJULHO=${NULO};
else
	stValor=SALDOJULHO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOJULHO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOJULHO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOAGOSTO=${LINHA:232:18};
if [ -z "$SALDOAGOSTO" ]; then
	SALDOAGOSTO=${NULO};
else
	stValor=$SALDOAGOSTO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOAGOSTO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOAGOSTO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOSETEMBRO=${LINHA:250:18};
if [ -z "$SALDOSETEMBRO" ]; then
	SALDOSETEMBRO=${NULO};
else
	stValor=$SALDOSETEMBRO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOSETEMBRO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOSETEMBRO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDOOUTUBRO=${LINHA:268:18};
if [ -z "$SALDOOUTUBRO" ]; then
	SALDOOUTUBRO=${NULO};
else
	stValor=$SALDOOUTUBRO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDOOUTUBRO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDOOUTUBRO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDONOVEMBRO=${LINHA:286:18};
if [ -z "$SALDONOVEMBRO" ]; then
	SALDONOVEMBRO=${NULO};
else
	stValor=$SALDONOVEMBRO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDONOVEMBRO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDONOVEMBRO=${stValor:0:15}"."${stValor:16:2}
	fi
fi

SALDODEZEMBRO=${LINHA:304:18};
if [ -z "$SALDODEZEMBRO" ]; then
	SALDODEZEMBRO=${NULO};
else
	stValor=$SALDODEZEMBRO;
	valor=${stValor:16:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99 ]; then
		SALDODEZEMBRO="-"${stValor:0:15}"."${stValor:16:1}$operadorValor
	else
		SALDODEZEMBRO=${stValor:0:15}"."${stValor:16:2}
	fi
fi


echo -e $ORGAOSUPERIOR${TAB}$ORGAOEXECUTOR${TAB}$UNIDADEGESTORA${TAB}$GESTAO${TAB}$ESFERA${TAB}$UNIDADEORCAMENTARIA${TAB}$PROGRAMATRABALHO${TAB}$FONTE${TAB}$NATUREZADESPESA${TAB}$UNIDADEGESTORARESPONSAVEL${TAB}$PLANOINTERNO${TAB}$CODIGOCONTA${TAB}$SALDOINICIAL${TAB}$SALDOJANEIRO${TAB}$SALDOFEVEREIRO${TAB}$SALDOMARCO${TAB}$SALDOABRIL${TAB}$SALDOMAIO${TAB}$SALDOJUNHO${TAB}$SALDOJULHO${TAB}$SALDOAGOSTO${TAB}$SALDOSETEMBRO${TAB}$SALDOOUTUBRO${TAB}$SALDONOVEMBRO${TAB}$SALDODEZEMBRO >> ${FILE}.sql;

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