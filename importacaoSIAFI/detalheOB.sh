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
echo  "COPY siafi.ob(cpf_usuario, terminal_usuario, data_transacao, hora_transacao, codigo_ug_operador, numero_ob, data_emissao, banco_origem, agencia_origem, conta_corrente_origem, tipo_favorecido, codigo_favorecido, banco_destino, agencia_destino, conta_corrente_destino, codigo_evento_sistema, valor_evento_sistema, numero_processo, numero_ano_relatorio, tipo_ob, pagamento, pessoal, numeroisn, controleimpressao, numero_ns_conciliacao, numero_remessa, operacao_cambial, inversao_saldo_doc, observacao, cancelamento_ob, numero_ob_cancelamento, restabelecimento_ob, numero_ob_restabelecimento, codigo_contrato_repasse, mes_lancamento, codigo_evento_01, codigo_evento_02, codigo_evento_03, codigo_evento_04, codigo_evento_05, codigo_evento_06, codigo_evento_07, codigo_evento_08, codigo_evento_09, codigo_evento_10, codigo_evento_11, codigo_evento_12, codigo_inscricao1_01, codigo_inscricao1_02, codigo_inscricao1_03, codigo_inscricao1_04, codigo_inscricao1_05, codigo_inscricao1_06, codigo_inscricao1_07, codigo_inscricao1_08, codigo_inscricao1_09, codigo_inscricao1_10, codigo_inscricao1_11, codigo_inscricao1_12, codigo_inscricao2_01, codigo_inscricao2_02, codigo_inscricao2_03, codigo_inscricao2_04, codigo_inscricao2_05, codigo_inscricao2_06, codigo_inscricao2_07, codigo_inscricao2_08, codigo_inscricao2_09, codigo_inscricao2_10, codigo_inscricao2_11, codigo_inscricao2_12, classificacao1_01, classificacao1_02, classificacao1_03, classificacao1_04, classificacao1_05, classificacao1_06, classificacao1_07, classificacao1_08, classificacao1_09, classificacao1_10, classificacao1_11, classificacao1_12, classificacao2_01, classificacao2_02, classificacao2_03, classificacao2_04, classificacao2_05, classificacao2_06, classificacao2_07, classificacao2_08, classificacao2_09, classificacao2_10, classificacao2_11, classificacao2_12, valor_transacao_01, valor_transacao_02, valor_transacao_03, valor_transacao_04, valor_transacao_05, valor_transacao_06, valor_transacao_07, valor_transacao_08, valor_transacao_09, valor_transacao_10, valor_transacao_11, valor_transacao_12, sequencial_favorecido_lista, numero_lista, codigo_sistema_origem, envio_fita_online, hora_envio_fita_online, ug_gestao_cancelamento, codigo_evento_bacen, hora_liberacao_ordenador_despesa, hora_liberacao_gestor_financeiro, cpf_liberacao_cofin, hora_liberacao_cofin, it_in_operador_favorecido, it_in_ob_dar, it_co_msg_documento, it_co_finalidade_spb, it_co_ident_transferencia, it_nu_cpf_ordenador_ass, it_nu_cpf_gestor_financeiro, it_co_msg_conformidade, data_saque_bacen, it_va_cancelamento, it_co_controle_stn_original, it_nu_operacao_spb, data_leit_aud_spb, it_co_ug_doc_referencia, it_co_gestao_doc_referencia, gr_an_nu_documento_referencia, it_in_controle_ob_ra, it_in_scdp, it_in_leitura_scdp, it_co_controle_interno, it_in_inadimplencia_cauc, it_tx_justificativa_cauc, it_nu_cartao, it_nu_remessa_bb)    FROM stdin;" >> ${FILE}.sql;
cat ${FILE} | grep ^[^OB*] | sed 's/\\/ /g' | sed 's/*/ /g' | while read LINHA;
do
CPFDOUSUARIO=${LINHA:0:11};
	if [ -z $CPFDOUSUARIO ]; then
	CPFDOUSUARIO=${NULO};
	fi

TERMINALDOUSUARIO=${LINHA:11:8};
	if [ -z $TERMINALDOUSUARIO ]; then
	TERMINALDOUSUARIO=${NULO};
	fi

DATADETRANSACAO=${LINHA:19:8};
	if [ -z $DATADETRANSACAO ]; then
	DATADETRANSACAO=${NULO};
	else
	DATADETRANSACAO=${LINHA:19:2}"/"${LINHA:21:2}"/"${LINHA:23:4};
	fi

HORATRANSACAO=${LINHA:27:4};
	if [ -z $HORATRANSACAO ]; then
	HORATRANSACAO=${NULO};
	else
	HORATRANSACAO=${LINHA:27:2}":"${LINHA:29:2};
	fi

CODIGODAUGDOOPERADOR=${LINHA:31:6};
	if [ -z $CODIGODAUGDOOPERADOR ]; then
	CODIGODAUGDOOPERADOR=${NULO};
	fi

NUMERODAOB=${LINHA:37:23};
	if [ -z $NUMERODAOB ]; then
	NUMERODAOB=${NULO};
	fi

DATADEEMISSAO=${LINHA:60:8};
	if [ -z $DATADEEMISSAO ]; then
	DATADEEMISSAO=${NULO};
	else
	DATADEEMISSAO=${LINHA:60:2}"/"${LINHA:62:2}"/"${LINHA:64:4};
	fi

BANCODEORIGEM=${LINHA:68:3};
	if [ -z $BANCODEORIGEM ]; then
	BANCODEORIGEM=${NULO};
	fi

AGENCIADEORIGEM=${LINHA:71:4};
	if [ -z $AGENCIADEORIGEM ]; then
	AGENCIADEORIGEM=${NULO};
	fi

CONTACORRENTEDEORIGEM=${LINHA:75:10};
	if [ -z $CONTACORRENTEDEORIGEM ]; then
	CONTACORRENTEDEORIGEM=${NULO};
	fi

TIPODEFAVORECIDO=${LINHA:85:1};
	if [ -z $TIPODEFAVORECIDO ]; then
	TIPODEFAVORECIDO=${NULO};
	fi

CODIGODOFAVORECIDO=${LINHA:86:14};
	if [ -z $CODIGODOFAVORECIDO ]; then
	CODIGODOFAVORECIDO=${NULO};
	fi

BANCODEDESTINO=${LINHA:100:3};
	if [ -z $BANCODEDESTINO ]; then
	BANCODEDESTINO=${NULO};
	fi

AGENCIADEDESTINO=${LINHA:103:4};
	if [ -z $AGENCIADEDESTINO ]; then
	AGENCIADEDESTINO=${NULO};
	fi

CONTACORRENTEDEDESTINO=${LINHA:107:10};
	if [ -z $CONTACORRENTEDEDESTINO ]; then
	CONTACORRENTEDEDESTINO=${NULO};
	fi

CODIGODOEVENTODOSISTEMA=${LINHA:117:6};
	if [ -z $CODIGODOEVENTODOSISTEMA ]; then
	CODIGODOEVENTODOSISTEMA=${NULO};
	fi

VALORDOEVENTODOSISTEMA=${LINHA:123:17};
	if [ -z $VALORDOEVENTODOSISTEMA ]; then
	VALORDOEVENTODOSISTEMA=${NULO};
	else
	VALORDOEVENTODOSISTEMA=${LINHA:123:15}"."${LINHA:138:2};
	fi

NUMERODOPROCESSO=${LINHA:140:20};
	if [ -z "$NUMERODOPROCESSO" ]; then
	NUMERODOPROCESSO=${NULO};
	fi

NUMERODOANODORELATORIO=${LINHA:160:12};
	if [ -z $NUMERODOANODORELATORIO ]; then
	NUMERODOANODORELATORIO=${NULO};
	fi

TIPODEOB=${LINHA:172:2};
	if [ -z $TIPODEOB ]; then
	TIPODEOB=${NULO};
	fi

PAGAMENTO=${LINHA:174:1};
	if [ -z $PAGAMENTO ]; then
	PAGAMENTO=${NULO};
	fi

PESSOAL=${LINHA:175:1};
	if [ -z $PESSOAL ]; then
	PESSOAL=${NULO};
	fi

NUMERODOISN=${LINHA:176:8};
	if [ -z $NUMERODOISN ]; then
	NUMERODOISN=${NULO};
	fi

CONTROLEDEIMPRESSAO=${LINHA:184:1};
	if [ -z $CONTROLEDEIMPRESSAO ]; then
	CONTROLEDEIMPRESSAO=${NULO};
	fi

NUMERODANSDECONCILIACAO=${LINHA:185:12};
	if [ -z $NUMERODANSDECONCILIACAO ]; then
	NUMERODANSDECONCILIACAO=${NULO};
	fi

NUMERODAREMESSA=${LINHA:197:5};
	if [ -z $NUMERODAREMESSA ]; then
	NUMERODAREMESSA=${NULO};
	fi

OPERACAOCAMBIAL=${LINHA:202:10};
	if [ -z $OPERACAOCAMBIAL ]; then
	OPERACAOCAMBIAL=${NULO};
	fi

INVERSAODOSALDODODOC=${LINHA:212:1};
	if [ -z $INVERSAODOSALDODODOC ]; then
	INVERSAODOSALDODODOC=${NULO};
	fi

OBSERVACAO=${LINHA:213:234};
	if [ -z "$OBSERVACAO" ]; then
	OBSERVACAO=${NULO};
	fi

CANCELAMENTODAOB=${LINHA:447:1};
	if [ -z $CANCELAMENTODAOB ]; then
	CANCELAMENTODAOB=${NULO};
	fi

NUMERODAOBDECANCELAMENTO=${LINHA:448:12};
	if [ -z $NUMERODAOBDECANCELAMENTO ]; then
	NUMERODAOBDECANCELAMENTO=${NULO};
	fi

RESTABELECIMENTODAOB=${LINHA:460:1};
	if [ -z $RESTABELECIMENTODAOB ]; then
	RESTABELECIMENTODAOB=${NULO};
	fi

NUMERODAOBDERESTABELECIMENTO=${LINHA:461:12};
	if [ -z $NUMERODAOBDERESTABELECIMENTO ]; then
	NUMERODAOBDERESTABELECIMENTO=${NULO};
	fi

CODIGODOCONTRATODEREPASSE=${LINHA:473:3};
	if [ -z $CODIGODOCONTRATODEREPASSE ]; then
	CODIGODOCONTRATODEREPASSE=${NULO};
	fi

MESDOLANCAMENTO=${LINHA:476:2};
	if [ -z $MESDOLANCAMENTO ]; then
	MESDOLANCAMENTO=${NULO};
	fi

CODIGODOEVENTOOBS=${LINHA:478:72};
	if [ -z $CODIGODOEVENTOOBS ]; then
	stringCodigoEvento=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=6;
		stringCodigoEvento='';
		while [ $n -le 11 ]; do
			campo=${CODIGODOEVENTOOBS:$x:$y};
			if [ -z "$campo" ]; then
				stringCodigoEvento=${stringCodigoEvento}${NULO}'\t'
			else
				stringCodigoEvento=${stringCodigoEvento}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done

	fi

CODIGODAINSCRICAO1OBS=${LINHA:550:168};
	if [ -z "$CODIGODAINSCRICAO1OBS" ]; then
	stringInscricao=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=14;
		stringInscricao='';
		while [ $n -le 11 ]; do
			campo=${CODIGODAINSCRICAO1OBS:$x:$y};
			if [ -z "$campo" ]; then
				stringInscricao=${stringInscricao}${NULO}'\t'
			else
				stringInscricao=${stringInscricao}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CODIGODAINSCRICAO2OBS=${LINHA:718:168};
	if [ -z "$CODIGODAINSCRICAO2OBS" ]; then
	stringInscricao2=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=14;
		stringInscricao2='';
		while [ $n -le 11 ]; do
			campo=${CODIGODAINSCRICAO2OBS:$x:$y};
			if [ -z "$campo" ]; then
				stringInscricao2=${stringInscricao2}${NULO}'\t'
			else
				stringInscricao2=${stringInscricao2}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CLASSIFICACAO1___OBS=${LINHA:886:108};
	if [ -z $CLASSIFICACAO1___OBS ]; then
	stringClassificacao1=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=9;
		stringClassificacao1='';
		while [ $n -le 11 ]; do
			campo=${CLASSIFICACAO1___OBS:$x:$y};
			if [ -z "$campo" ]; then
				stringClassificacao1=${stringClassificacao1}${NULO}'\t'
			else
				stringClassificacao1=${stringClassificacao1}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

CLASSIFICACAO2___OBS=${LINHA:994:108};
	if [ -z $CLASSIFICACAO2___OBS ]; then
	stringClassificacao2=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=9;
		stringClassificacao2='';
		while [ $n -le 11 ]; do
			campo=${CLASSIFICACAO2___OBS:$x:$y};
			if [ -z "$campo" ]; then
				stringClassificacao2=${stringClassificacao2}${NULO}'\t'
			else
				stringClassificacao2=${stringClassificacao2}${campo}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

VALORDATRANSACAO_OBS=${LINHA:1102:204};
	if [ -z $VALORDATRANSACAO_OBS ]; then
	strinValorTransacao=${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t'${NULO}'\t';
	else
		n=0;
		x=0;
		y=17;
		strinValorTransacao='';
		while [ $n -le 11 ]; do
			campo=${VALORDATRANSACAO_OBS:$x:$y};
			if [ -z "$campo" ]; then
				strinValorTransacao=${strinValorTransacao}${NULO}'\t'
			else
				strinValorTransacao=${strinValorTransacao}${VALORDATRANSACAO_OBS:$x:15}"."${VALORDATRANSACAO_OBS:$x+15:2}'\t'
			fi

		let n++;
		x=$x+$y;
		done
	fi

SEQUENCIALDOFAVORECIDONALISTA=${LINHA:1306:6};
	if [ -z $SEQUENCIALDOFAVORECIDONALISTA ]; then
	SEQUENCIALDOFAVORECIDONALISTA=${NULO};
	fi

NUMERODALISTALISTA=${LINHA:1312:6};
	if [ -z $NUMERODALISTALISTA ]; then
	NUMERODALISTALISTA=${NULO};
	fi

CODIGODOSISTEMADEORIGEM=${LINHA:1318:10};
	if [ -z $CODIGODOSISTEMADEORIGEM ]; then
	CODIGODOSISTEMADEORIGEM=${NULO};
	fi

ENVIODEFITAONLINE=${LINHA:1328:1};
	if [ -z $ENVIODEFITAONLINE ]; then
	ENVIODEFITAONLINE=${NULO};
	fi

HORADEENVIODEFITAONLINE=${LINHA:1329:4};
	if [ -z $HORADEENVIODEFITAONLINE ]; then
	HORADEENVIODEFITAONLINE=${NULO};
	else
	HORADEENVIODEFITAONLINE=${LINHA:1329:2}":"${LINHA:1331:2};
	fi

UGGESTAOCANCELAMENTO=${LINHA:1333:11};
	if [ -z $UGGESTAOCANCELAMENTO ]; then
	UGGESTAOCANCELAMENTO=${NULO};
	fi

CODIGODOEVENTOBACEN=${LINHA:1344:9};
	if [ -z $CODIGODOEVENTOBACEN ]; then
	CODIGODOEVENTOBACEN=${NULO};
	fi

HORADALIBERACAODOORDENADORDEDESPESA=${LINHA:1353:4};
	if [ -z $HORADALIBERACAODOORDENADORDEDESPESA ]; then
	HORADALIBERACAODOORDENADORDEDESPESA=${NULO};
	else
	HORADALIBERACAODOORDENADORDEDESPESA=${LINHA:1353:2}":"${LINHA:1355:2};
	fi

HORADALIBERACAODOGESTORFINANCEIRO=${LINHA:1357:4};
	if [ -z $HORADALIBERACAODOGESTORFINANCEIRO ]; then
	HORADALIBERACAODOGESTORFINANCEIRO=${NULO};
	else
	HORADALIBERACAODOGESTORFINANCEIRO=${LINHA:1357:2}":"${LINHA:1359:2};
	fi

CPFDALIBERACAOCOFIN=${LINHA:1361:11};
	if [ -z $CPFDALIBERACAOCOFIN ]; then
	CPFDALIBERACAOCOFIN=${NULO};
	fi

HORADALIBERACAOCOFIN=${LINHA:1372:4};
	if [ -z $HORADALIBERACAOCOFIN ]; then
	HORADALIBERACAOCOFIN=${NULO};
	else
	HORADALIBERACAOCOFIN=${LINHA:1372:2}":"${LINHA:1374:2};
	fi

ITINOPERADORFAVORECIDO=${LINHA:1376:1};
	if [ -z $ITINOPERADORFAVORECIDO ]; then
	ITINOPERADORFAVORECIDO=${NULO};
	fi

ITINOBDAR=${LINHA:1377:1};
	if [ -z $ITINOBDAR ]; then
	ITINOBDAR=${NULO};
	fi

ITCOMSGDOCUMENTO=${LINHA:1378:3};
	if [ -z $ITCOMSGDOCUMENTO ]; then
	ITCOMSGDOCUMENTO=${NULO};
	fi

ITCOFINALIDADESPB=${LINHA:1381:3};
	if [ -z $ITCOFINALIDADESPB ]; then
	ITCOFINALIDADESPB=${NULO};
	fi

ITCOIDENTTRANSFERENCIA=${LINHA:1384:25};
	if [ -z "$ITCOIDENTTRANSFERENCIA" ]; then
	ITCOIDENTTRANSFERENCIA=${NULO};
	fi

ITNUCPFORDENADORASS=${LINHA:1409:11};
	if [ -z $ITNUCPFORDENADORASS ]; then
	ITNUCPFORDENADORASS=${NULO};
	fi

ITNUCPFGESTORFINANCEIRO=${LINHA:1420:11};
	if [ -z $ITNUCPFGESTORFINANCEIRO ]; then
	ITNUCPFGESTORFINANCEIRO=${NULO};
	fi

ITCOMSGCONFORMIDADE=${LINHA:1431:30};
	if [ -z $ITCOMSGCONFORMIDADE ]; then
	ITCOMSGCONFORMIDADE=${NULO};
	fi

DATASAQUEBACEN=${LINHA:1461:8};
	if [ -z $DATASAQUEBACEN ]; then
	DATASAQUEBACEN=${NULO};
	else
	DATASAQUEBACEN=${LINHA:1461:2}"/"${LINHA:1463:2}"/"${LINHA:1465:4};
	fi

ITVACANCELAMENTO=${LINHA:1469:17};
	if [ -z $ITVACANCELAMENTO ]; then
	ITVACANCELAMENTO=${NULO};
	else
	ITVACANCELAMENTO=${LINHA:1469:15}"."${LINHA:1484:2};
	fi

ITCOCONTROLESTNORIGINAL=${LINHA:1486:20};
	if [ -z $ITCOCONTROLESTNORIGINAL ]; then
	ITCOCONTROLESTNORIGINAL=${NULO};
	fi

ITNUOPERACAOSPB=${LINHA:1506:23};
	if [ -z $ITNUOPERACAOSPB ]; then
	ITNUOPERACAOSPB=${NULO};
	fi

DATALEITAUDSPB=${LINHA:1529:8};
	if [ -z $DATALEITAUDSPB ]; then
	DATALEITAUDSPB=${NULO};
	else
	DATALEITAUDSPB=${LINHA:1529:2}"/"${LINHA:1531:2}"/"${LINHA:1533:4};
	fi

ITCOUGDOCREFERENCIA=${LINHA:1537:6};
	if [ -z $ITCOUGDOCREFERENCIA ]; then
	ITCOUGDOCREFERENCIA=${NULO};
	fi

ITCOGESTAODOCREFERENCIA=${LINHA:1543:5};
	if [ -z $ITCOGESTAODOCREFERENCIA ]; then
	ITCOGESTAODOCREFERENCIA=${NULO};
	fi

GRANNUDOCUMENTOREFERENCIA=${LINHA:1548:12};
	if [ -z $GRANNUDOCUMENTOREFERENCIA ]; then
	GRANNUDOCUMENTOREFERENCIA=${NULO};
	fi

ITINCONTROLEOBRA=${LINHA:1560:1};
	if [ -z $ITINCONTROLEOBRA ]; then
	ITINCONTROLEOBRA=${NULO};
	fi

ITINSCDP=${LINHA:1561:1};
	if [ -z $ITINSCDP ]; then
	ITINSCDP=${NULO};
	fi

ITINLEITURASCDP=${LINHA:1562:1};
	if [ -z $ITINLEITURASCDP ]; then
	ITINLEITURASCDP=${NULO};
	fi

ITCOCONTROLEINTERNO=${LINHA:1563:20};
	if [ -z $ITCOCONTROLEINTERNO ]; then
	ITCOCONTROLEINTERNO=${NULO};
	fi

ITININADIMPLENCIACAUC=${LINHA:1583:1};
	if [ -z $ITININADIMPLENCIACAUC ]; then
	ITININADIMPLENCIACAUC=${NULO};
	fi

ITTXJUSTIFICATIVACAUC=${LINHA:1584:100};
	if [ -z "$ITTXJUSTIFICATIVACAUC" ]; then
	ITTXJUSTIFICATIVACAUC=${NULO};
	fi

ITNUCARTAO=${LINHA:1684:16};
	if [ -z $ITNUCARTAO ]; then
	ITNUCARTAO=${NULO};
	fi

ITNUREMESSABB=${LINHA:1700:5};
	if [ -z $ITNUREMESSABB ]; then
	ITNUREMESSABB=${NULO};
	fi

echo -e $CPFDOUSUARIO${TAB}$TERMINALDOUSUARIO${TAB}$DATADETRANSACAO${TAB}$HORATRANSACAO${TAB}$CODIGODAUGDOOPERADOR${TAB}$NUMERODAOB${TAB}$DATADEEMISSAO${TAB}$BANCODEORIGEM${TAB}$AGENCIADEORIGEM${TAB}$CONTACORRENTEDEORIGEM${TAB}$TIPODEFAVORECIDO${TAB}$CODIGODOFAVORECIDO${TAB}$BANCODEDESTINO${TAB}$AGENCIADEDESTINO${TAB}$CONTACORRENTEDEDESTINO${TAB}$CODIGODOEVENTODOSISTEMA${TAB}$VALORDOEVENTODOSISTEMA${TAB}$NUMERODOPROCESSO${TAB}$NUMERODOANODORELATORIO${TAB}$TIPODEOB${TAB}$PAGAMENTO${TAB}$PESSOAL${TAB}$NUMERODOISN${TAB}$CONTROLEDEIMPRESSAO${TAB}$NUMERODANSDECONCILIACAO${TAB}$NUMERODAREMESSA${TAB}$OPERACAOCAMBIAL${TAB}$INVERSAODOSALDODODOC${TAB}$OBSERVACAO${TAB}$CANCELAMENTODAOB${TAB}$NUMERODAOBDECANCELAMENTO${TAB}$RESTABELECIMENTODAOB${TAB}$NUMERODAOBDERESTABELECIMENTO${TAB}$CODIGODOCONTRATODEREPASSE${TAB}$MESDOLANCAMENTO${TAB}${stringCodigoEvento}${stringInscricao}${stringInscricao2}${stringClassificacao1}${stringClassificacao1}${strinValorTransacao}$SEQUENCIALDOFAVORECIDONALISTA${TAB}$NUMERODALISTALISTA${TAB}$CODIGODOSISTEMADEORIGEM${TAB}$ENVIODEFITAONLINE${TAB}$HORADEENVIODEFITAONLINE${TAB}$UGGESTAOCANCELAMENTO${TAB}$CODIGODOEVENTOBACEN${TAB}$HORADALIBERACAODOORDENADORDEDESPESA${TAB}$HORADALIBERACAODOGESTORFINANCEIRO${TAB}$CPFDALIBERACAOCOFIN${TAB}$HORADALIBERACAOCOFIN${TAB}$ITINOPERADORFAVORECIDO${TAB}$ITINOBDAR${TAB}$ITCOMSGDOCUMENTO${TAB}$ITCOFINALIDADESPB${TAB}$ITCOIDENTTRANSFERENCIA${TAB}$ITNUCPFORDENADORASS${TAB}$ITNUCPFGESTORFINANCEIRO${TAB}$ITCOMSGCONFORMIDADE${TAB}$DATASAQUEBACEN${TAB}$ITVACANCELAMENTO${TAB}$ITCOCONTROLESTNORIGINAL${TAB}$ITNUOPERACAOSPB${TAB}$DATALEITAUDSPB${TAB}$ITCOUGDOCREFERENCIA${TAB}$ITCOGESTAODOCREFERENCIA${TAB}$GRANNUDOCUMENTOREFERENCIA${TAB}$ITINCONTROLEOBRA${TAB}$ITINSCDP${TAB}$ITINLEITURASCDP${TAB}$ITCOCONTROLEINTERNO${TAB}$ITININADIMPLENCIACAUC${TAB}$ITTXJUSTIFICATIVACAUC${TAB}$ITNUCARTAO${TAB}$ITNUREMESSABB >> ${FILE}.sql;
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