<?php
set_time_limit(0);
ini_set("memory_limit", "4000M");

function gerar_relatorio(){
	
	echo "	<link rel=stylesheet type=text/css href=../includes/Estilo.css />
		  	<link rel=stylesheet type=text/css href=../includes/listagem.css />
			<script type=text/javascript src=../par/js/jquery-1.11.1.min.js > </script>
			<script type=text/javascript src=../includes/funcoes.js ></script>
			<script>jQuery.noConflict();</script>
			<table width=100% cellspacing=1 cellpadding=5 border=0 align=center class=tabela >
			<tr><td colspan=100 > ".monta_cabecalho_relatorio('100')." </td> </tr></table>";
	
	include APPRAIZ.'includes/library/simec/Lista_jQuery_DataTables.php';
	
	$cabecalho = montaCabecalho();
	
	$numColunas = '0';
	for( $x=1; $x <= count($cabecalho); $x++ ){
		$numColunas .= ','.$x;
	}
	
	$param['nome'] 				= 'lista';
	$param['titulo'] 			= 'Relatórios de Mobiliário e Equipamentos - Proinfância';
	$param['descricaoBusca']	= 'filtro';
	$param['instrucaoBusca']	= '(Digite o texto a ser buscado)';
	$param['nomeXLS']			= "Relatórios de Mobiliário e Equipamentos - Proinfância";
	$param['numeroColunasXLS']	= $numColunas;
	$param['arrDados']			= buscaDados();
	$param['arrCabecalho']		= $cabecalho;
// 	$param['arrSemSpan']		= Array( 'N', 'N', '', '', '', '', '', '', 'N' );
// 	$param['arrCampoNumerico']	= Array();
	
	$lista = new listaDT();
	echo $lista->lista($param);
}

function gerar_relatorio_xls(){
	
	global $db;
	
	$cabecalho 	= montaCabecalho();
	$arrDados	= buscaDados();
	
	ob_clean();
	header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
	header("Pragma: no-cache");
	header("Content-type: application/xls; name=SIMEC_RelatDocente" . date("Ymdhis") . ".xls");
	header("Content-Disposition: attachment; filename=SIMEC_RelatDocente" . date("Ymdhis") . ".xls");
	$db->monta_lista_tabulado($arrDados, $cabecalho, 1000000, 5, 'N', '100%');
	exit;
}

function montaCabecalho(){
	
	$cabecalho = Array();
	
	/* Cabeçalho */
	if( $_REQUEST['coluna'][0] != '' ){
		foreach( $_REQUEST['coluna'] as $coluna ){
			switch ($coluna){
				case 'perc_exec_inst':
					$cabecalho[] = '% Executado Instituição';
				break;
				case 'perc_exec_emp':
					$cabecalho[] = '% Executado Empresa';
				break;
				case 'obranoconvenio':
					$cabecalho[] = 'Ano Convênio da obra';
				break;
				case 'sbadsc':
					$cabecalho[] = 'Descrição da Subação';
				break;
				case 'sbdano':
					$cabecalho[] = 'Ano da Subação';
				break;
				case 'dimcod':
					$cabecalho[] = 'Dimensão';
				break;
				case 'boo_documento':
					$cabecalho[] = 'Documento';
				break;
				case 'boo_empenho':
					$cabecalho[] = 'Empenho';
				break;
				case 'esfera':
					$cabecalho[] = 'Esfera';
				break;
				case 'toodescricao':
					$cabecalho[] = 'Fonte';
				break;
				case 'frmdsc':
					$cabecalho[] = 'Forma de Execução';
				break;
				case 'obrid':
					$cabecalho[] = 'ID Obra';
				break;
				case 'preid':
					$cabecalho[] = 'ID Pré-Obra';
				break;
				case 'qtditemcomposicao':
					$cabecalho[] = 'Item de Composição';
				break;
				case 'estuf':
					$cabecalho[] = 'UF';
				break;
				case 'mundescricao':
					$cabecalho[] = 'Município';
				break;
				case 'obrnumprocessoconv':
					$cabecalho[] = 'N° Processo da Obra';
				break;
				case 'numconvenio':
					$cabecalho[] = 'Nº Convênio da obra';
				break;
				case 'processo':
					$cabecalho[] = 'Número do Processo';
				break;
				case 'dopnumerodocumento':
					$cabecalho[] = 'Número do Termo';
				break;
				case 'predescricao':
					$cabecalho[] = 'Nome da Obra';
				break;
				case 'boo_pagamento':
					$cabecalho[] = 'Pagamento';
				break;
				case 'sbdplanointerno':
					$cabecalho[] = 'Plano Interno';
				break;
				case 'sbdptres':
					$cabecalho[] = 'PTRES';
				break;
				case 'programa':
					$cabecalho[] = 'Programa';
				break;
				case 'prfdesc':
					$cabecalho[] = 'Programa Obras 2.0';
				break;
				case 'sub_valor':
					$cabecalho[] = 'Valor da Subação';
				break;
				case 'qtditemcomposicao_aprovado':
					$cabecalho[] = 'Quantidade Aprovada de Itens de Composição';
				break;
				case 'esddsc_par':
					$cabecalho[] = 'Situação da Pre-Obra';
				break;
				case 'esddsc_obras':
					$cabecalho[] = 'Situação no Obras 2.0';
				break;
				case 'ssudescricao':
					$cabecalho[] = 'Status da Subação';
				break;
				case 'codigo':
					$cabecalho[] = 'Subação';
				break;
				case 'tiposubacao':
					$cabecalho[] = 'Tipo da Subação';
				break;
				case 'tpodsc':
					$cabecalho[] = 'Tipologia';
				break;
			}
		}
	}
	if( $_REQUEST['req'] == 'gerar_relatorio_xls' ){
		$cabecalho[] 	= 'Situação do Pagamento';
		$cabecalho[] 	= 'N° de VInculação';
	}
	/* FIM - Cabeçalho */
	return $cabecalho;
}

function buscaDados(){
	
	global $db;
	
	$where 	= Array('sba.ppsid IN (924, 906, 913, 925, 914, 904)');
	/* Mapa de Join
	 * 	chave - alias - valor
	 * 	1 	- obr - LEFT  JOIN obras2.obras					obr ON obr.obrid = sov.obrid
		2 	- emp - LEFT  JOIN obras2.empreendimento		emp ON emp.empid = obr.empid --obr
		3 	- pto - LEFT  JOIN obras2.tipologiaobra			tpo ON tpo.tpoid = obr.tpoid --obr
		4 	- prf - LEFT  JOIN obras2.programafonte 		prf ON prf.prfid = emp.prfid --obr, emp
		5 	- too - LEFT  JOIN obras2.tipoorigemobra 		too ON too.tooid = obr.tooid --obr
		6 	- do1 - LEFT  JOIN workflow.documento			do1 ON do1.docid = obr.docid --obr
		7 	- es1 - LEFT  JOIN workflow.estadodocumento 	es1 ON es1.esdid = do1.esdid --obr, do1
		8 	- pre - LEFT  JOIN obras.preobra				pre ON pre.preid = sov.preid
		9 	- doc - LEFT  JOIN workflow.documento			doc ON doc.docid = pre.docid --pre
		10	- esd - LEFT  JOIN workflow.estadodocumento 	esd ON esd.esdid = doc.esdid --pre, doc
		11	- em1 - INNER JOIN par.empenhosubacao			em1 ON em1.sbaid = sbd.sbaid AND em1.eobano = sbd.sbdano AND em1.eobstatus = 'A'
		12	- tc1 - INNER JOIN par.termocomposicao			tc1 ON tc1.sbdid = sbd.sbdid
		13	- tc1 - INNER JOIN par.termocomposicao			tc1 ON tc1.sbdid = sbd.sbdid
		14  - tec - LEFT  JOIN par.termocomposicao 			tec ON tec.sbdid = sbd.sbdid
		15	- dop - LEFT  JOIN par.documentopar				dop ON dop.dopid = tec.dopid
		16	- emp - LEFT  JOIN par.empenho					em ON emp.empid = em1.empid
		17	- vps - LEFT  JOIN par.vinculacaoptressigef		vps ON vps.vpsptres = e.empcodigoptres
	 * */
	$join 		= Array();
	$colunas 	= Array();
	
	/* Filtros */
	if( $_REQUEST['numeroprocesso'] != '' ){
		$where[] = " prp.prpnumeroprocesso = '".str_replace(Array('.','-','/'), '', $_REQUEST['numeroprocesso'])."'";		
	}
	
	if( $_REQUEST['obrid'] != '' ){
		$where[] = "sov.obrid = {$_REQUEST['obrid']}";		
	}
	
	if( $_REQUEST['preid'] != '' ){
		$where[] = "sov.preid = {$_REQUEST['preid']}";		
	}
	
	if( $_REQUEST['esdid'][0] && ( $_REQUEST['esdid_campo_flag'] || $_REQUEST['esdid_campo_flag'] == '1' )){
		$where[] = "do1.esdid " . (( $_REQUEST['esdid_campo_excludente'] == null || $_REQUEST['esdid_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " 
					('" . implode( "','", $_REQUEST['esdid'] ) . "')";	
		$join[1] = "LEFT  JOIN obras2.obras					obr ON obr.obrid = sov.obrid";
		$join[6] = "LEFT  JOIN workflow.documento			do1 ON do1.docid = obr.docid";
	}
	
	if( $_REQUEST['esfera'] == 'E' ){
		$where[] = "inu.estuf IS NOT NULL";		
	}
	
	if( $_REQUEST['esfera'] == 'M' ){
		$where[] = "inu.muncod IS NOT NULL";		
	}
	
	if( $_REQUEST['estuf'][0] && ( $_REQUEST['estuf_campo_flag'] || $_REQUEST['estuf_campo_flag'] == '1' )){
		$where[] = "( inu.estuf " . (( $_REQUEST['estuf_campo_excludente'] == null || $_REQUEST['estuf_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_REQUEST['estuf'] ) . "') OR 
					mun.estuf " . (( $_REQUEST['estuf_campo_excludente'] == null || $_REQUEST['estuf_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_REQUEST['estuf'] ) . "') )";	
	}
	
	if( $_REQUEST['mundescricao'] ){
		$_REQUEST['mundescricao'] = removeAcentos(utf8_decode($_REQUEST['mundescricao']));
		$where[] = "public.removeacento(mun.mundescricao) ILIKE '%".$_REQUEST['mundescricao']."%'";		
	}
	
	if( $_REQUEST['sbdano'][0] && ( $_REQUEST['sbdano_campo_flag'] || $_REQUEST['sbdano_campo_flag'] == '1' )){
		$where[] = "sbd.sbdano " . (( $_REQUEST['sbdano_campo_excludente'] == null || $_REQUEST['sbdano_campo_excludente'] == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $_REQUEST['sbdano'] ) . "') ";	
	}
	
	if( $_REQUEST['empenhado'] == 'S' ){
		$where[] = "( ( sbd.sbaid, sbd.sbdano )  IN ( SELECT DISTINCT sbaid, eobano FROM par.empenhosubacao WHERE eobstatus = 'A' ) )";
	}
	
	if( $_REQUEST['empenhado'] == 'N' ){
		$where[] = "( ( sbd.sbaid, sbd.sbdano ) NOT IN ( SELECT DISTINCT sbaid, eobano FROM par.empenhosubacao WHERE eobstatus = 'A' ) )";		
	}
	
	if( $_REQUEST['termogerado'] == 'S' ){
		$where[] = "sbd.sbdid IN (SELECT DISTINCT sbdid FROM par.termocomposicao)";
	}
	
	if( $_REQUEST['termogerado'] == 'N' ){
		$where[] = "sbd.sbdid NOT IN (SELECT DISTINCT sbdid FROM par.termocomposicao)";		
	}
	
	if( $_REQUEST['termovalidado'] == 'S' ){
		$where[] = "dop.dopdatavalidacaogestor IS NOT NULL";
		$join[14] = "LEFT  JOIN par.termocomposicao 			tec ON tec.sbdid = sbd.sbdid";
		$join[15] = "LEFT  JOIN par.documentopar				dop ON dop.dopid = tec.dopid";
	}
	
	if( $_REQUEST['termovalidado'] == 'N' ){
		$where[] = "dop.dopdatavalidacaogestor IS NULL";	
		$join[14] = "LEFT  JOIN par.termocomposicao 			tec ON tec.sbdid = sbd.sbdid";
		$join[15] = "LEFT  JOIN par.documentopar				dop ON dop.dopid = tec.dopid";	
	}
	/*O que é isso?*/
	if( $_REQUEST['publicacao_anexada'] == 'S' ){
		
	}
	/*O que é isso?*/
	if( $_REQUEST['publicacao_anexada'] == 'N' ){
		
	}
	
	if( $_REQUEST['pagamento_solicitado'] == 'S' ){
		$where[] = "(sbd.sbaid, sbd.sbdano) IN (SELECT DISTINCT sbaid, pobano FROM par.pagamentosubacao WHERE pobstatus = 'A')";
	}
	
	if( $_REQUEST['pagamento_solicitado'] == 'N' ){
		$where[] 	= "(sbd.sbaid, sbd.sbdano) NOT IN (SELECT DISTINCT sbaid, pobano FROM par.pagamentosubacao WHERE pobstatus = 'A')";	
	}
	
	/* FIM - Filtros */
	
	/* Colunas */
	if( $_REQUEST['coluna'][0] != '' ){
		foreach( $_REQUEST['coluna'] as $coluna ){
			switch ($coluna){
				case 'perc_exec_inst':
					$colunas[] 	= "obr.obrpercentultvistoria as perc_exec_inst";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
				break;
				case 'perc_exec_emp':
					$colunas[] 	= "emp.emppercentultvistoriaempresa as perc_exec_emp";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
					$join[2] 	= "LEFT JOIN obras2.empreendimento		emp ON emp.empid = obr.empid";
				break;
				case 'obranoconvenio':
					$colunas[] 	= "obr.obranoconvenio";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
				break;
				case 'sbadsc':
					$colunas[] = "sba.sbadsc";
				break;
				case 'sbdano':
					$colunas[] = "sbd.sbdano";
				break;
				case 'dimcod':
					$colunas[] = "dim.dimcod";
				break;
				case 'boo_documento':
					$colunas[] = "CASE WHEN (SELECT DISTINCT true FROM par.termocomposicao tec WHERE tec.sbdid = sbd.sbdid)
									THEN 'Sim'
									ELSE 'Não'
								END as boo_documento";
				break;
				case 'boo_empenho':
					$colunas[] = "CASE WHEN (SELECT DISTINCT true FROM par.empenhosubacao ems WHERE ems.sbaid = sbd.sbaid AND ems.eobano = sbd.sbdano AND eobstatus = 'A')
									THEN 'Sim'
									ELSE 'Não'
								END as boo_empenho";
				break;
				case 'esfera':
					$colunas[] 	= "CASE 
										WHEN emp.empesfera = 'M' THEN 'Municipal'
										WHEN emp.empesfera = 'E' THEN 'Estadual'
									END as esfera";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
					$join[2] 	= "LEFT JOIN obras2.empreendimento		emp ON emp.empid = obr.empid";
				break;
				case 'toodescricao':
					$colunas[] 	= "too.toodescricao";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
					$join[5] 	= "LEFT JOIN obras2.tipoorigemobra 		too ON too.tooid = obr.tooid";
				break;
				case 'frmdsc':
					$colunas[] = "frm.frmdsc";
				break;
				case 'obrid':
					$colunas[] = "sov.obrid";
				break;
				case 'preid':
					$colunas[] = "sov.preid";
				break;
				case 'qtditemcomposicao':
					$colunas[] = "par.recuperaquantidadeitensplanejadossubacaoporano(sbd.sbaid, sbd.sbdano) as qtditemcomposicao";
				break;
				case 'estuf':
					$colunas[] = "coalesce(inu.estuf, mun.estuf) as estuf";
				break;
				case 'mundescricao':
					$colunas[] = "coalesce(mun.mundescricao, '-') as mundescricao";
				break;
				case 'obrnumprocessoconv':
					$colunas[] 	= "obr.obrnumprocessoconv ||'&nbsp;' as obrnumprocessoconv";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
				break;
				case 'numconvenio':
					$colunas[] 	= "obr.numconvenio";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
				break;
				case 'processo':
					$colunas[] = "prp.prpnumeroprocesso||'&nbsp;' as processo";
				break;
				case 'dopnumerodocumento':
					$colunas[] = "dop.dopnumerodocumento";
					$join[14] = "LEFT  JOIN par.termocomposicao 			tec ON tec.sbdid = sbd.sbdid";
					$join[15] = "LEFT  JOIN par.documentopar				dop ON dop.dopid = tec.dopid";
				break;
				case 'predescricao':
					$colunas[] 	= "pre.predescricao";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
				break;
				case 'boo_pagamento':
					$colunas[] = "CASE WHEN (SELECT DISTINCT true 
											FROM par.pagamentosubacao pas 
											INNER JOIN par.pagamento pag2 ON pag2.pagid = pas.pagid
											WHERE pas.sbaid = sbd.sbaid AND pas.pobano = sbd.sbdano AND pobstatus = 'A' AND pag2.pagsituacaopagamento <> '%CANCELADO%' AND pag2.pagstatus = 'A')
									THEN 'Sim'
									ELSE 'Não'
								END as boo_pagamento";
				break;
				case 'sbdplanointerno':
					$colunas[] = "sbd.sbdplanointerno";
				break;
				case 'sbdptres':
					$colunas[] = "sbd.sbdptres";
				break;
				case 'programa':
					$colunas[] = "CASE 
									WHEN sba.ppsid IN (924, 906, 913) THEN 'FNDE - Proinfância - Mobiliário'
									WHEN sba.ppsid IN (925, 914, 904) THEN 'FNDE - Proinfância - Equipamentos'
								END as programa";
				break;
				case 'prfdesc':
					$colunas[] 	= "prf.prfdesc";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
					$join[2] 	= "LEFT JOIN obras2.empreendimento		emp ON emp.empid = obr.empid";
					$join[4] 	= "LEFT JOIN obras2.programafonte 		prf ON prf.prfid = emp.prfid";
				break;
				case 'sub_valor':
					$colunas[] = 'par.recuperavalorvalidadossubacaoporano(sbd.sbaid, sbd.sbdano) as sub_valor';
				break;
				case 'qtditemcomposicao_aprovado':
					$colunas[] = "par.recuperaquantidadeitensvalidadossubacaoporano(sbd.sbaid, sbd.sbdano) as qtditemcomposicao_aprovado";
				break;
				case 'esddsc_par':
					$colunas[] 	= "esd.esddsc as esddsc_par";
					$join[8] 	= "LEFT JOIN obras.preobra				pre ON pre.preid = sov.preid";
					$join[9] 	= "LEFT JOIN workflow.documento			doc ON doc.docid = pre.docid";
					$join[10]	= "LEFT JOIN workflow.estadodocumento 	esd ON esd.esdid = doc.esdid";
				break;
				case 'esddsc_obras':
					$colunas[] 	= "es1.esddsc as esddsc_obras";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
					$join[6] 	= "LEFT JOIN workflow.documento			do1 ON do1.docid = obr.docid";
					$join[7] 	= "LEFT JOIN workflow.estadodocumento 	es1 ON es1.esdid = do1.esdid";
				break;
				case 'ssudescricao':
					$colunas[] = "ssu.ssudescricao";
				break;
				case 'codigo':
					$colunas[] = "dimcod || '.' || arecod || '.' || indcod || '.' || sbaordem as codigo";
				break;
				case 'tiposubacao':
					$colunas[] = "CASE 
									WHEN sba.ppsid IN (924, 906, 913) THEN 'Mobiliário'
									WHEN sba.ppsid IN (925, 914, 904) THEN 'Equipamentos'
								END as tiposubacao";
				break;
				case 'tpodsc':
					$colunas[] 	= "tpo.tpodsc";
					$join[1] 	= "LEFT JOIN obras2.obras				obr ON obr.obrid = sov.obrid";
					$join[3] 	= "LEFT JOIN obras2.tipologiaobra		tpo ON tpo.tpoid = obr.tpoid";
				break;
			}
		}
	}
	/* FIM - Colunas */
	
	if( $_REQUEST['req'] == 'gerar_relatorio_xls' ){
		$colunas[] 	= 'pag.pagsituacaopagamento';
		$colunas[] 	= 'vps.vpsvinculacao';
		$join[11] 	= "INNER JOIN par.empenhosubacao		em1 ON em1.sbaid = sbd.sbaid AND em1.eobano = sbd.sbdano AND em1.eobstatus = 'A'";
		$join[16] 	= "LEFT  JOIN par.empenho				em  ON em.empid = em1.empid";
		$join[17] 	= "LEFT  JOIN par.vinculacaoptressigef	vps ON vps.vpsptres = em.empcodigoptres";
	}
	
	$sql = "SELECT
				".implode(', ',$colunas)."
			FROM par.subacao sba
			INNER JOIN par.acao 	 aca ON aca.aciid = sba.aciid
			INNER JOIN par.pontuacao pto ON pto.ptoid = aca.ptoid
			INNER JOIN par.criterio  crt ON crt.crtid = pto.crtid
			INNER JOIN par.indicador ind ON ind.indid = crt.indid AND ind.indcod = 11
			INNER JOIN par.area 	 are ON are.areid = ind.areid AND are.arecod = 2
			INNER JOIN par.dimensao  dim ON dim.dimid = are.dimid AND dim.dimcod = 4
			
			INNER JOIN par.formaexecucao			frm ON frm.frmid = sba.frmid
			
			INNER JOIN par.subacaodetalhe 			sbd ON sbd.sbaid = sba.sbaid
			INNER JOIN par.processoparcomposicao  	ppc ON ppc.sbdid = sbd.sbdid
			INNER JOIN par.processopar 				prp ON prp.prpid = ppc.prpid and prp.prpstatus = 'A'
			
			INNER JOIN par.statussubacao			ssu ON ssu.ssuid = sbd.ssuid
			
			INNER JOIN par.subacaoobravinculacao 	sov ON sov.sbaid = sba.sbaid
						
			LEFT  JOIN par.pagamentosubacao			pgs ON pgs.sbaid = sbd.sbaid AND pgs.pobano = sbd.sbdano AND pgs.pobstatus = 'A' 
			LEFT  JOIN par.pagamento				pag ON pag.pagid = pgs.pagid AND pag.pagstatus = 'A'
			
			".implode(' ',$join)."
			
			INNER JOIN par.instrumentounidade 		inu ON inu.inuid  = pto.inuid
			LEFT  JOIN territorios.municipio 		mun ON mun.muncod = inu.muncod
			
			WHERE
				".implode(' AND ', $where)." --LIMIT 10";
//  	ver($sql,d);

	$arDados = $db->carregar( $sql );
	$arDados = is_array($arDados) ? $arDados : Array();

	return $arDados;
}
?>