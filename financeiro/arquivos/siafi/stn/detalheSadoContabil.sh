#!/bin/bash

contasNum() {
	contas['112160400']=45;
	contas['112161400']=12;
	contas['191140000']=72;
	contas['192110101']=16;
	contas['192110201']=16;
	contas['192110209']=16;
	contas['192110301']=16;
	contas['192110303']=16;
	contas['192120100']=16;
	contas['192120200']=16;
	contas['192120300']=16;
	contas['192130100']=16;
	contas['192130101']=16;
	contas['192130102']=16;
	contas['192130200']=16;
	contas['192130201']=16;
	contas['192140100']=16;
	contas['192140200']=16;
	contas['192190101']=16;
	contas['192190109']=16;
	contas['192190201']=16;
	contas['192190209']=16;
	contas['192190301']=16;
	contas['192190302']=16;
	contas['192210101']=16;
	contas['192210102']=16;
	contas['192210201']=16;
	contas['192210202']=16;
	contas['192210901']=16;
	contas['192210909']=16;
	contas['192220100']=16;
	contas['192220200']=16;
	contas['192220300']=16;
	contas['192220901']=16;
	contas['192220909']=16;
	contas['192230000']=16;
	contas['192240000']=16;
	contas['192290100']=16;
	contas['192290200']=16;
	contas['192410101']=26;
	contas['192410102']=26;
	contas['192410103']=26;
	contas['192410106']=16;
	contas['192410109']=26;
	contas['193110303']=50;
	contas['193110304']=50;
	contas['193110306']=50;
	contas['193110307']=50;
	contas['193110308']=50;
	contas['193110316']=50;
	contas['193110317']=50;
	contas['193110318']=50;
	contas['193210701']=52;
	contas['195100000']=26;
	contas['195300000']=26;
	contas['195400000']=26;
	contas['195910000']=26;
	contas['195920000']=26;
	contas['292110000']=16;
	contas['292120101']=16;
	contas['292120102']=16;
	contas['292120103']=16;
	contas['292120104']=16;
	contas['292120201']=16;
	contas['292120202']=16;
	contas['292120500']=16;
	contas['292129900']=16;
	contas['292130100']=16;
	contas['292130201']=31;
	contas['292130209']=31;
	contas['292140200']=16;
	contas['292140300']=16;
	contas['292210101']=16;
	contas['292210201']=16;
	contas['292210901']=16;
	contas['292210909']=16;
	contas['292220100']=16;
	contas['292220200']=16;
	contas['292220300']=16;
	contas['292220901']=16;
	contas['292220909']=16;
	contas['292230000']=16;
	contas['292240000']=16;
	contas['292290100']=16;
	contas['292290200']=16;
	contas['292410101']=26;
	contas['292410102']=26;
	contas['292410402']=26;
	contas['292410403']=26;
	contas['292410508']=26;
	contas['292410510']=26;
	contas['292410590']=26;
	contas['292410591']=26;
	contas['292440100']=40;
	contas['292440200']=40;
	contas['293110104']=43;
	contas['293110117']=43;
	contas['293110118']=43;
	contas['293110119']=43;
	contas['293110303']=50;
	contas['293110304']=50;
	contas['293110306']=50;
	contas['293110307']=50;
	contas['293110308']=50;
	contas['293110309']=50;
	contas['293110601']=17;
	contas['293110699']=17;
	contas['295100000']=26;
	contas['295200000']=26;
	contas['295300000']=26;
	contas['295400000']=26;
	contas['295600000']=26;
	contas['512120000']=12;
	contas['612120000']=12;
	contas['293110317']=50;
	contas['293110603']=17;
	contas['293110602']=17;
	contas['293110216']=43;
	contas['293110316']=50;
	contas['293110214']=43;
	contas['293110204']=43;
	contas['293110207']=43;
	contas['292120105']=16;
	contas['191210100']=28;
	contas['293110604']=17;
	contas['293110605']=17;
	contas['293110606']=17;
	contas['293110607']=17;
	contas['293110608']=17;
	contas['293110609']=17;
	codConta=${contas["$CONTA"]};
}

case12() {
	echo '1';
}
TAB='\t';
NULO='\\N';
LIDOS='lidos/';
SQLCOPY='sqlCopy/';

FILE=$1;
echo -e "SET client_encoding TO 'LATIN5'; \n" >> ../${FILE}.sql;
echo  "COPY financeiro.saldocontabil(slddatatransacao,sldmes,sldano,orgcod,ungcod,gstcod,sldcccnum,esfcod,unicod,ptrcod,iducod,grfcod,frscod,frsgrcod,ndpcod,ungcodresponsavel,noenumoriginal,npenumoriginal,vincod,irpcod,tarcod,concod,tcccod,slddebitovlr,sldcreditovlr) FROM stdin WITH NULL AS '${NULO}';" >> ${FILE}.sql;

#file_length=`wc -l '${FILE}' | cut -c1-5`;


cat ${FILE} | grep ^[^SC*] | while read LINHA;
do 

    CONTA=${LINHA:13:9};
    
    SLDDATATRANSACAO=${LINHA:609:4}${LINHA:607:2}${LINHA:605:2};
    echo $SLDDATATRANSACAO;
#    if [ "${SLDDATATRANSACAO:0:1}" -eq ' '  ];
#    then
#	SLDDATATRANSACAO='\\N';
#	echo;
#    fi
    
    SLDMES=${LINHA:607:2};
#    if [ "${SLDMES}" -eq '  '  ];
#    then
#	SLDMES='\\N';
#	echo;
#    fi
    
    SLDANO=${LINHA:609:4};
#    if [ ${SLDANO:0:1}==' '  ];
#    then
#	SLDANO='\\N';
#	echo;
#    fi
    
    UNGCOD=${LINHA:2:6};
#    if [ ${UNGCOD:0:1}==' '  ];
#    then
#	UNGCOD='\\N';
#	echo;
#    fi
    
    GSTCOD=${LINHA:8:5};
#    if [ ${GSTCOD:0:1}==' '  ];
#    then
#	GSTCOD='\\N';
#	echo;
#    fi
    
    CONCOD=${LINHA:13:7};
#    if [ ${CONCOD:0:1}==' '  ];
#    then
#	CONCOD='\\N';
#	echo;
#    fi
    
    TCCCOD=${LINHA:20:2};
#    if [ ${TCCCOD:0:1}==' '  ];
#    then
#	TCCCOD='\\N';
#	echo 1;
#    fi
    
    SLDCCCNUM=${LINHA:22:43};
#    if [ "${SLDCCCNUM:0:1}"==' '  ];
#    then
#	SLDCCCNUM='\\N';
#	echo;
#    fi
    SLDDEBITOVLR=${LINHA:100:18};
    SLDCREDITOVLR=${LINHA:352:18};

	ORGCOD=${NULO};
	ESFCOD=${NULO};
	UNICOD=${NULO};
	FUNCOD=${NULO};
	SFUCOD=${NULO};
	PRGCOD=${NULO};
	ACACOD=${NULO};
	LOCCOD=${NULO};
	PTRCOD=${NULO};
	IDUCOD=${NULO};
	GRFCOD=${NULO};
	FRSCOD=${NULO};
	FRSGRCOD=${NULO};
	NDPCOD=${NULO};
	UNGCODRESPONSAVEL=${NULO};
	PLICOD=${NULO};
	NOENUMORIGINAL=${NULO};
	NPENUMORIGINAL=${NULO};
	VINCOD=${NULO};
	IRPCOD=${NULO};
	TARCOD=${NULO};
	contasNum;
	case $codConta in
       		"12")
		FRSCOD=$SLDCCCNUM;
        ;;
		"16")
		ESFCOD=${SLDCCCNUM:0:1};
		PTRCOD=${SLDCCCNUM:1:6};
		FRSCOD=${SLDCCCNUM:7:10};
		NDPCOD=${SLDCCCNUM:17:6};
		UNGCODRESPONSAVEL=${SLDCCCNUM:23:6};
		PLICOD=${SLDCCCNUM:29:11};
	;;
		"17")

	;;
		"26")
		NOENUMORIGINAL=$SLDCCCNUM;
	;;
		"28")
		IDUCOD=${SLDCCCNUM:0:1};
		FRSCOD=${SLDCCCNUM:2:10};
	;;
		"31")
		ESFCOD=${SLDCCCNUM:0:1};
		PTRCOD=${SLDCCCNUM:1:6}; 
		FRSCOD=${SLDCCCNUM:7:10};
		NDPCOD=${SLDCCCNUM:17:8};
		UNGCODRESPONSAVEL=${SLDCCCNUM:25:6};
		PLICOD=${SLDCCCNUM:31:11};
	;;
		"40")
		NPENUMORIGINAL=${SLDCCCNUM:9:10}; 
	;;
		"43")
		ORGCOD=${SLDCCCNUM:0:5}; 
	;;
		"45")
		FRSCOD=${SLDCCCNUM:0:10};
	;;
		"50")
		FRSCOD=${SLDCCCNUM:6:10};
		VINCOD=${SLDCCCNUM:18:3};
	;;
		"52")
		FRSCOD=${SLDCCCNUM:0:10};
		VINCOD=${SLDCCCNUM:10:3};
	;;
		"72")
		ESFCOD=${SLDCCCNUM:18:1};
		UNICOD=${SLDCCCNUM:19:5};
		FRSCOD=${SLDCCCNUM:8:10};
		NDPCOD=${SLDCCCNUM:0:8};
		IRPCOD=${SLDCCCNUM:24:1};
		TARCOD=${SLDCCCNUM:25:1};
	;;
		*)

	;;
	esac
    echo -e $SLDDATATRANSACAO${TAB}$SLDMES${TAB}$SLDANO${TAB}$ORGCOD${TAB}${UNGCOD}${TAB}$GSTCOD${TAB}${SLDCCCNUM}${TAB}${ESFCOD}${TAB}${UNICOD}${TAB}${PTRCOD}${TAB}${IDUCOD}${TAB}${GRFCOD}${TAB}${FRSCOD}${TAB}${FRSGRCOD}${TAB}${NDPCOD}${TAB}${UNGCODRESPONSAVEL}${TAB}${NOENUMORIGINAL}${TAB}${NPENUMORIGINAL}${TAB}${VINCOD}${TAB}${IRPCOD}${TAB}${TARCOD}${TAB}${CONCOD}${TAB}${TCCCOD}${TAB}${SLDDEBITOVLR}${TAB}${SLDCREDITOVLR} >> ${FILE}.sql;

done
mv ${FILE} ${LIDOS};
tar -cf ${LIDOS}${FILE}".tar.gz" ${LIDOS}${FILE}
rm ${LIDOS}${FILE}
mv ${FILE}.sql ${SQLCOPY}/${FILE}.sql;
psql -h $PGHOST -U $PGUSER -d $PGDB -f ${SQLCOPY}/${FILE}.sql
