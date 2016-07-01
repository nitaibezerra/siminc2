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
echo  "COPY siafi.convenioexecucao( it_co_usuario, it_co_terminal_usuario, it_da_transacao, it_ho_transacao, it_co_ug_operador, it_in_operacao, it_nu_convenio, it_nu_parcela, it_in_execucao, it_co_motivo, it_da_execucao, it_va_execucao )   FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^EC*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do

    # 00001 00011  NUM     IT-CO-USUARIO
    CAMPO01=${LINHA:00:11}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi
																																			    
    # 00012  00008  ALFANUM IT-CO-TERMINAL-USUARIO
    CAMPO02=${LINHA:11:8}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
																																				    
    # 00020  00027  00008  NUM     IT-DA-TRANSACAO (DDMMAAAA)  
    CAMPO03=${LINHA:25:4}${LINHA:  23: 2}${LINHA:  21: 2}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
																																					 
    # 00028  00004  NUM     IT-HO-TRANSACAO
    CAMPO04=${LINHA:27:4}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
																																						 
    # 00032  00006  NUM     IT-CO-UG-OPERADOR
    CAMPO05=${LINHA:31:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
																																							 
    # 00038  00001  ALFANUM IT-IN-OPERACAO
    CAMPO06=${LINHA:37:1}; if [ -z "$CAMPO06" ]; then CAMPO06=${NULO}; fi
																																								    
    # 00039  00006  NUM     IT-NU-CONVENIO
    CAMPO07=${LINHA:38:6}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
																																									    
    # 00045  00003  NUM     IT-NU-PARCELA
    CAMPO08=${LINHA:44:3}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
																																										    
    # 00048  00002  NUM     IT-IN-EXECUCAO
    CAMPO09=${LINHA:47:2}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi 
																																											 
    # 00050  00003  NUM     IT-CO-MOTIVO
    CAMPO10=${LINHA:49:3}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi 
																																												    
    # 00053  00008  NUM     IT-DA-EXECUCAO (DDMMAAAA)  
    CAMPO11=${LINHA:58:4}${LINHA:  56: 2}${LINHA:  54: 2}; if [ -z "$CAMPO11" ]; then CAMPO11=${NULO}; fi
																																													 
    # 00061  00017  NUM     IT-VA-EXECUCAO (N15,2)
    CAMPO12=${LINHA:60:17};
    if [ -z $CAMPO12 ]; then
	CAMPO12=${NULO};
    else
	valor=${LINHA:76:1};
	contasNum $valor;
	if  [ $operadorValor -ne 99  ]; then
	    CAMPO12="-"${LINHA:60:15}"."${LINHA:76:1}$operadorValor
	else
	    CAMPO12=${LINHA:60:15}"."${LINHA:76:2}
	fi
    fi 
																																																								 
    echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO06${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10${TAB}$CAMPO11${TAB}$CAMPO12 >> ${FILE}.sql; 
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