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
echo  "COPY siafi.cronograma( it_co_usuario, it_co_terminal_usuario, it_da_transacao, it_ho_transacao, it_co_ug_operador, it_in_operacao, it_nu_convenio, it_co_concedente, it_co_gestao_concedente, it_nu_original, it_nu_parcela, it_nu_prazo, it_va_realizado, it_pe_realizado, it_co_motivo_inadiplencia, it_da_inadiplencia, it_in_cadastro)   FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^CC*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
    # 00001  00011  NUM     IT-CO-USUARIO
    CAMPO01=${LINHA:0:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
    
    # 00012  00008  ALFANUM IT-CO-TERMINAL-USUARIO
    CAMPO02=${LINHA:11:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
    
    # 00020  00027  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)
    CAMPO03=${LINHA:  25: 4}${LINHA:  23: 2}${LINHA:  21: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
    
    # 00028  00004  NUM     IT-HO-TRANSACAO
    CAMPO04=${LINHA:27:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
    
    # 00032  00006  NUM     IT-CO-UG-OPERADOR
    CAMPO05=${LINHA:31:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
    
    # 00038  00001  ALFANUM IT-IN-OPERACAO
    CAMPO06=${LINHA:37:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
    
    # 00039  00006  NUM     IT-NU-CONVENIO
    CAMPO07=${LINHA:38:6}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
																																								
    # 00045  00014  ALFANUM IT-CO-CONCEDENTE
    CAMPO08=${LINHA:44:14}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
																								
    # 00059  00005  NUM     IT-CO-GESTAO-CONCEDENTE
    CAMPO09=${LINHA:58:5}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
																																											 
    # 00064  00020  ALFANUM IT-NU-ORIGINAL
    CAMPO10=${LINHA:63:20}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
																																												 
    # 00084  00003  NUM     IT-NU-PARCELA
    CAMPO11=${LINHA:83:3}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
																																												 
    # 00087  00003  NUM     IT-NU-PRAZO
    CAMPO12=${LINHA:86:3}; if [ -z "$CAMPO12" ]; then CAMPO12=${NULO}; fi
																																														
    # 00090  00017  NUM     IT-VA-REALIZADO (N15,2)
    CAMPO13=${LINHA:89:17}; 
    if [ -z $CAMPO13 ]; then 
	CAMPO13=${NULO}; 
    else valor=${LINHA:106:1}; 
	contasNum $valor;
	if [ $operadorValor -ne 99  ]; then
	    CAMPO13="-"${LINHA:89:15}"."${LINHA:106:1}$operadorValor
	else
	    CAMPO13=${LINHA:89:15}"."${LINHA:106:2}
	fi
    fi 
    
    # 00107  00005  NUM     IT-PE-REALIZADO (N3,2)
    CAMPO14=${LINHA:106:5};
    if [ -z $CAMPO14 ]; then
	CAMPO14=${NULO};
    else
	valor=${LINHA:111:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99  ]; then
	    CAMPO14="-"${LINHA:106:3}"."${LINHA:111:1}$operadorValor
	else
	    CAMPO14=${LINHA:106:3}"."${LINHA:111:2}
	fi
    fi 
    
    # 00112  00003  NUM     IT-CO-MOTIVO-INADIMPLENCIA
    CAMPO15=${LINHA:111:3}; if [ -z "$CAMPO15" ]; then CAMPO15=${NULO}; fi
    
    # 00115  00122  00008  NUM     IT-DA-INADIMPLENCIA (DDMMAAAA)
    CAMPO16=${LINHA:  120: 4}${LINHA:  118: 2}${LINHA:  116: 2}; if [ -z "$CAMPO16" ]; then CAMPO16=${NULO}; fi
																																																																						 
    # 00123  00001  NUM     IT-IN-CADASTRO
    CAMPO17=${LINHA:122:1}; if [ -z "$CAMPO17" ]; then CAMPO17=${NULO}; fi
																																																																							 
    echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12${TAB}$CAMPO13${TAB}$CAMPO14${TAB}$CAMPO15${TAB}$CAMPO16${TAB}$CAMPO17 >> ${FILE}.sql; 
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