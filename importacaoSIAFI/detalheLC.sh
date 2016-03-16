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
echo  "COPY siafi.lancamentocontabil( data_lancamento,it_co_unidade_gestora,it_co_gestao,it_va_lancamento,gr_codigo_evento,gr_dc_conta_contabil_01,gr_dc_conta_contabil_02,gr_dc_conta_contabil_03,gr_dc_conta_contabil_04,gr_dc_conta_contabil_05,gr_dc_conta_contabil_06,gr_dc_conta_contabil_07,gr_dc_conta_contabil_08,gr_dc_conta_contabil_09,gr_dc_conta_contabil_10,gr_dc_conta_contabil_11,gr_dc_conta_contabil_12,gr_dc_conta_contabil_13,gr_dc_conta_contabil_14,gr_dc_conta_contabil_15,gr_dc_conta_contabil_16,gr_dc_conta_contabil_17,gr_dc_conta_contabil_18,gr_dc_conta_contabil_19,gr_dc_conta_contabil_20,it_co_unidade_gestora_ident,it_co_gestao_ident,gr_an_nu_documento_ident, it_da_emissao ) FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^LC*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do

	# 00001 00008  NUM     DATA-LANCAMENTO (DDMMAAAA)
	CAMPO01=${LINHA: 4: 4}${LINHA: 2: 2}${LINHA: 0: 2}; if [ -z "$CAMPO01" ]; then CAMPO01=${NULO}; fi

	# 00009 00006  NUM     IT-CO-UNIDADE-GESTORA
	CAMPO02=${LINHA:8:6}; if [ -z "$CAMPO02" ]; then CAMPO02=${NULO}; fi
	
	# 00015 00005  NUM     IT-CO-GESTAO
	CAMPO03=${LINHA:14:5}; if [ -z "$CAMPO03" ]; then CAMPO03=${NULO}; fi
	
	# 00020 00017  NUM     IT-VA-LANCAMENTO (N15,2)
	CAMPO04=${LINHA:19:17}; if [ -z "$CAMPO04" ]; then CAMPO04=${NULO}; fi
	
	# 00037 00006  NUM     GR-CODIGO-EVENTO
	CAMPO05=${LINHA:36:6}; if [ -z "$CAMPO05" ]; then CAMPO05=${NULO}; fi
	
	# 00043  01102  01060  ALFANUM GR-DC-CONTA-CONTABIL CAMPO DE 53 BYTES QUE OCORRE 20 VEZES
		CAMPO06_1=${LINHA:43:53}; if [ -z "$CAMPO06_1" ]; then CAMPO06_1=${NULO}; fi
		CAMPO06_2=${LINHA:95:53}; if [ -z "$CAMPO06_2" ]; then CAMPO06_2=${NULO}; fi
		CAMPO06_3=${LINHA:147:53}; if [ -z "$CAMPO06_3" ]; then CAMPO06_3=${NULO}; fi
		CAMPO06_4=${LINHA:199:53}; if [ -z "$CAMPO06_4" ]; then CAMPO06_4=${NULO}; fi
		CAMPO06_5=${LINHA:251:53}; if [ -z "$CAMPO06_5" ]; then CAMPO06_5=${NULO}; fi
		CAMPO06_6=${LINHA:303:53}; if [ -z "$CAMPO06_6" ]; then CAMPO06_6=${NULO}; fi
		CAMPO06_7=${LINHA:355:53}; if [ -z "$CAMPO06_7" ]; then CAMPO06_7=${NULO}; fi
		CAMPO06_8=${LINHA:459:53}; if [ -z "$CAMPO06_8" ]; then CAMPO06_8=${NULO}; fi
		CAMPO06_9=${LINHA:511:53}; if [ -z "$CAMPO06_9" ]; then CAMPO06_9=${NULO}; fi
		CAMPO06_10=${LINHA:563:53}; if [ -z "$CAMPO06_10" ]; then CAMPO06_10=${NULO}; fi
		CAMPO06_11=${LINHA:615:53}; if [ -z "$CAMPO06_11" ]; then CAMPO06_11=${NULO}; fi
		CAMPO06_12=${LINHA:667:53}; if [ -z "$CAMPO06_12" ]; then CAMPO06_12=${NULO}; fi
		CAMPO06_13=${LINHA:719:53}; if [ -z "$CAMPO06_13" ]; then CAMPO06_13=${NULO}; fi
		CAMPO06_14=${LINHA:771:53}; if [ -z "$CAMPO06_14" ]; then CAMPO06_14=${NULO}; fi
		CAMPO06_15=${LINHA:823:53}; if [ -z "$CAMPO06_15" ]; then CAMPO06_15=${NULO}; fi
		CAMPO06_16=${LINHA:875:53}; if [ -z "$CAMPO06_16" ]; then CAMPO06_16=${NULO}; fi
		CAMPO06_17=${LINHA:927:53}; if [ -z "$CAMPO06_17" ]; then CAMPO06_17=${NULO}; fi
		CAMPO06_18=${LINHA:979:53}; if [ -z "$CAMPO06_18" ]; then CAMPO06_18=${NULO}; fi
		CAMPO06_19=${LINHA:1031:53}; if [ -z "$CAMPO06_19" ]; then CAMPO06_19=${NULO}; fi
		CAMPO06_20=${LINHA:1083:53}; if [ -z "$CAMPO06_20" ]; then CAMPO06_20=${NULO}; fi
	
	# 01103 00006  NUM     IT-CO-UNIDADE-GESTORA-IDENT
	CAMPO07=${LINHA:1102:6}; if [ -z "$CAMPO07" ]; then CAMPO07=${NULO}; fi
	
	# 01109 00005  NUM     IT-CO-GESTAO-IDENT
	CAMPO08=${LINHA:1108:5}; if [ -z "$CAMPO08" ]; then CAMPO08=${NULO}; fi
	
	
	# 01114 00012  ALFANUM GR-AN-NU-DOCUMENTO-IDENT
	CAMPO09=${LINHA:1113:12}; if [ -z "$CAMPO09" ]; then CAMPO09=${NULO}; fi
	
	# 01126 00008  NUM     IT-DA-EMISSAO (DDMMAAAA)
	CAMPO10=${LINHA:1125:8}; if [ -z "$CAMPO10" ]; then CAMPO10=${NULO}; fi
	
		
	echo -e $CAMPO01${TAB}$CAMPO02${TAB}$CAMPO03${TAB}$CAMPO04${TAB}$CAMPO05${TAB}$CAMPO006_1${TAB}$CAMPO006_2${TAB}$CAMPO006_3${TAB}$CAMPO006_4${TAB}$CAMPO006_5${TAB}$CAMPO006_6${TAB}$CAMPO006_7${TAB}$CAMPO006_8${TAB}$CAMPO006_9${TAB}$CAMPO006_10${TAB}$CAMPO006_11${TAB}$CAMPO006_12${TAB}$CAMPO006_13${TAB}$CAMPO006_14${TAB}$CAMPO006_15${TAB}$CAMPO006_16${TAB}$CAMPO006_17${TAB}$CAMPO006_18${TAB}$CAMPO006_19${TAB}$CAMPO006_20${TAB}$CAMPO07${TAB}$CAMPO08${TAB}$CAMPO09${TAB}$CAMPO10 >> ${FILE}.sql;

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