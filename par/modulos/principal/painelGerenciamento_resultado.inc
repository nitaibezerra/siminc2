<?php

function obras_monta_agp_painel_gerencial(){

	if( $_POST['tipo'] == 2 ){ //excel
		$agrupador = $_POST['colunas'];
	} else {
		$colunas = str_replace("}{",",",$_REQUEST["colunas"]);
		$colunas = str_replace("}","",$colunas);
		$colunas = str_replace("{","",$colunas);
		$colunas = str_replace("\'","'",$colunas);
		$colunas = explode(",",$colunas);
		$agrupador = $colunas;//array('nomedaobra');
	}
	
	$agp = array(
				"agrupador" => array(),
				"agrupadoColuna" => array( "qtd", "vlr", "vlrempenho", "esddsc", "muncod", "municipio", "uf", "nomedaobra", "resolucao", "entcodent", "entnome", "preanometa", "preanoselecao", "preesfera" )
				);
	
	foreach ( $agrupador as $val ){
		switch( $val ){
			case "nomedaobra":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobra",
											  		"label" => "Nome da Obra")										
									   				);
			break;
			case "uf":
				array_push($agp['agrupador'], array(
													"campo" => "uf",
											  		"label" => "Estado")										
									   				);
			break;
			case "municipio":
				array_push($agp['agrupador'], array(
													"campo" => "municipio",
											  		"label" => "Municipio")										
									   				);
			break;
			case "esddsc_":
				array_push($agp['agrupador'], array(
													"campo" => "esddsc_",
											  		"label" => "Situação FNDE")										
									   				);
			break;
			case "esddsc":
				array_push($agp['agrupador'], array(
													"campo" => "esddsc",
											  		"label" => "Situação da Obra")										
									   				);
			break;
			case "ptodescricao":
				array_push($agp['agrupador'], array(
													"campo" => "ptodescricao",
											  		"label" => "Tipo da Obra")										
									   				);
			break;
			case "obra":
				array_push($agp['agrupador'], array(
													"campo" => "nomedaobra",
											  		"label" => "Obra")										
									   				);
			break;
			case "tpmdsc":
				array_push($agp['agrupador'], array(
													"campo" => "tpmdsc",
											  		"label" => "Grupo de Municipios")										
									   				);
			break;
			case "regdescricao":
				array_push($agp['agrupador'], array(
													"campo" => "regdescricao",
											  		"label" => "Região")										
									   				);
			break;
			case "tipoobra":
				array_push($agp['agrupador'], array(
													"campo" => "tipoobra",
											  		"label" => "Pro-Infância/Quadra")										
									   				);
			break;
			case "analista":
				array_push($agp['agrupador'], array(
													"campo" => "analista",
											  		"label" => "Analista")										
									   				);
			break;
			case "data":
				array_push($agp['agrupador'], array(
													"campo" => "data",
											  		"label" => "Data")										
									   				);
			break;
			case "resolucao":
				array_push($agp['agrupador'], array(
													"campo" => "resolucao",
											  		"label" => "Resolução")										
									   				);
			break;
			case "empenhado":
				array_push($agp['agrupador'], array(
													"campo" => "empenhado",
											  		"label" => "Obras Empenhadas")										
									   				);
			break;
			case "esfera":
				array_push($agp['agrupador'], array(
													"campo" => "preesfera",
											  		"label" => "Esfera")										
									   				);
			break;
		}	
	}
	
	return $agp;
	
}

/**
 * Funçao que monta o sql para trazer o relatório geral de obras
 *
 * @author Fernando A. Bagno da Silva
 * @since 20/02/2009
 * @return string
 */
function obras_monta_sql_painel_gerencial(){
	
	global $db;
	
	$where = array();
	
	$filtro="";
	
	$colunas = str_replace("}{",",",$_REQUEST["colunas"]);
	$colunas = str_replace("}","",$colunas);
	$colunas = str_replace("{","",$colunas);
	$colunas = str_replace("\'","'",$colunas);
	$colunas = explode(",",$colunas);
	
	if( in_array('tpmdsc', $colunas)){
		$select = "tpmdsc, ";
	}
	
	// Filtros
	if ( $_REQUEST["regcod"] && $_REQUEST["regcod"] != '' ){
		$filtro .= " AND reg.regcod = '".$_REQUEST["regcod"]."'";
	}
	
	if( is_array( $_REQUEST["tpmid"] ) ){
		$_REQUEST["tpmid"] = implode('}{', $_REQUEST["tpmid"]);
	}
	if ( $_REQUEST["tpmid"] ){
		$select = "tpmdsc, ";
		$nottpmid = $_REQUEST['nottpmid'] == 'true' ? ' NOT ' : '';
		$tpmid = str_replace("}{",",",$_REQUEST["tpmid"]);
		$tpmid = str_replace("}","",$tpmid);
		$tpmid = str_replace("{","",$tpmid);
		$tpmid = str_replace("\'","'",$tpmid);
		$filtro .= " AND tpm.tpmid ".$nottpmid."IN (".$tpmid.") ";
	}
	
	if( is_array( $_REQUEST["esdid"] ) ){
		$_REQUEST["esdid"] = implode('}{', $_REQUEST["esdid"]);
	}
	if ( $_REQUEST["esdid"] ){
		$notesdid = $_REQUEST['notesdid'] == 'true' ? ' NOT ' : '';
		$esdid = str_replace("}{",",",$_REQUEST["esdid"]);
		$esdid = str_replace("}","",$esdid);
		$esdid = str_replace("{","",$esdid);
		$esdid = str_replace("\'","'",$esdid);
		$filtro .= " AND ed.esdid ".$notesdid."IN (".$esdid.") ";
	}
	if( is_array( $_REQUEST["esdid_"] ) ){
		$_REQUEST["esdid_"] = implode('}{', $_REQUEST["esdid_"]);
	}
	if ( $_REQUEST["esdid_"] ){
		$notesdid_ = $_REQUEST['notesdid_'] == 'true' ? ' NOT ' : '';
		$esdid_ = str_replace("}{",",",$_REQUEST["esdid_"]);
		$esdid_ = str_replace("}","",$esdid_);
		$esdid_ = str_replace("{","",$esdid_);
		$esdid_ = str_replace("\'","'",$esdid_);
		$filtro .= " AND ed.esdid ".$notesdid_."IN (".$esdid_.") ";
	}
	
	
	if( is_array( $_REQUEST["ptoid"] ) ){
		$_REQUEST["ptoid"] = implode('}{', $_REQUEST["ptoid"]);
	}
	if ( $_REQUEST["ptoid"] ){
		$notptoid = $_REQUEST['notptoid'] == 'true' ? ' NOT ' : '';
		$ptoid = str_replace("}{",",",$_REQUEST["ptoid"]);
		$ptoid = str_replace("}","",$ptoid);
		$ptoid = str_replace("{","",$ptoid);
		$ptoid = str_replace("\'","'",$ptoid);
		$filtro .= " AND tpo.ptoid ".$notptoid."IN (".$ptoid.") ";
	}
	
	if( $_POST['tipo'] == 2 ){
		if( is_array( $_REQUEST["estuf"][0] ) ){
			$_REQUEST["estuf"] = "{'".implode('}{', $_REQUEST["estuf"])."'";
			$notestuf = $_REQUEST['notestuf'] == 'true' ? ' NOT ' : '';
			$estuf = str_replace("}{",",",$_REQUEST["estuf"]);
			$estuf = str_replace("}","",$estuf);
			$estuf = str_replace("{","",$estuf);
			$estuf = str_replace("\'","'",$estuf);
			$filtro .= " AND mun.estuf ".$notestuf."IN (".$estuf.") ";
		}
	} else {
		if ($_REQUEST["estuf"]){
			$notestuf = $_REQUEST['notestuf'] == 'true' ? ' NOT ' : '';
			$estuf = str_replace("}{",",",$_REQUEST["estuf"]);
			$estuf = str_replace("}","",$estuf);
			$estuf = str_replace("{","",$estuf);
			$estuf = str_replace("\'","'",$estuf);
			$filtro .= " AND mun.estuf ".$notestuf."IN (".$estuf.") ";
		}
	}
	
	if( $_POST['tipo'] == 2 ){
		if( $_REQUEST["muncod"][0] ){
			$_REQUEST["muncod"] = "{'".implode("'}{'", $_REQUEST["muncod"])."'";
			$notmuncod = $_REQUEST['notmuncod'] == 'true' ? ' NOT ' : '';
			$muncod = str_replace("}{",",",$_REQUEST["muncod"])	;
			$muncod = str_replace("}","",$muncod);
			$muncod = str_replace("{","",$muncod);
			$muncod = str_replace("\'","'",$muncod);
			$filtro .= " AND mun.muncod ".$notmuncod."IN (".$muncod.") ";
		}
	} else {
		if ( $_REQUEST["muncod"] ){
			$notmuncod = $_REQUEST['notmuncod'] == 'true' ? ' NOT ' : '';
			$muncod = str_replace("}{",",",$_REQUEST["muncod"])	;
			$muncod = str_replace("}","",$muncod);
			$muncod = str_replace("{","",$muncod);
			$muncod = str_replace("\'","'",$muncod);
			$filtro .= " AND mun.muncod ".$notmuncod."IN (".$muncod.") ";
		}
	}
	
	if( is_array( $_REQUEST["usucpfanalista"] ) ){
		$_REQUEST["usucpfanalista"] = implode('}{', $_REQUEST["usucpfanalista"]);
	}  
	if ( $_REQUEST["usucpfanalista"] ){
		$notusucpf = $_REQUEST["notusucpfanalista"] == 'true' ? ' NOT ' : '';
		$usucpfanalista = str_replace("}{",",",$_REQUEST["usucpfanalista"])	;
		$usucpfanalista = str_replace("}","",$usucpfanalista);
		$usucpfanalista = str_replace("{","",$usucpfanalista);
		$usucpfanalista = str_replace("\'","'",$usucpfanalista);
		
		$filtro_cpf .= " AND hd.usucpf ".$notusucpf."IN (".$usucpfanalista.") ";		
		
//		if( !$esdid_ && !$esdid ){
//			$filtro .= " AND ac.esdiddestino in (".WF_TIPO_VALIDACAO_DILIGENCIA.",".WF_TIPO_VALIDACAO_INDEFERIMENTO.",".WF_TIPO_VALIDACAO_DEFERIMENTO.") ";
//		}
		
	}
	
	if( is_array( $_REQUEST["resid"] ) ){
		$_REQUEST["resid"] = implode('}{', $_REQUEST["resid"]);
	} 
	if ( $_REQUEST["resid"] ){
		$notresid = $_REQUEST["notresid"] == 'true' ? ' NOT ' : '';
		$resid = str_replace("}{",",",$_REQUEST["resid"])	;
		$resid = str_replace("}","",$resid);
		$resid = str_replace("{","",$resid);
		$resid = str_replace("\'","'",$resid);
		
		$filtro_cpf .= " AND pre.resid ".$notusucpf."IN (".$resid.") ";		
	}

	if( $_REQUEST["anoselecao"] ){
		$filtro .= " AND po.preanoselecao = {$_REQUEST["anoselecao"]} ";
	}

	if( $_REQUEST["preanometa"] ){
		$filtro .= " AND po.preanometa = {$_REQUEST["preanometa"]} ";
	}
	
	if( $_REQUEST['datatramite_inicio'] && $_REQUEST['datatramite_fim'] ){
		
		$datai = formata_data_sql($_REQUEST['datatramite_inicio']);
		$dataf = formata_data_sql($_REQUEST['datatramite_fim']);
		
		$filtro .= " AND hd1.htddata between '{$datai}' and '{$dataf}'";
		
	}elseif( $_REQUEST['datatramite_inicio'] && !$_REQUEST['datatramite_fim'] ){
				
		$arData = explode("/",$_REQUEST['datatramite_inicio']);
		
		$filtro .= " AND date_part('year', hd1.htddata) = '{$arData[2]}' AND date_part('month', hd1.htddata) = '{$arData[1]}' AND date_part('day', hd1.htddata) = '{$arData[0]}'";
		
	}elseif( !$_REQUEST['datatramite_inicio'] && $_REQUEST['datatramite_fim'] ){
		
		$arData = explode("/",$_REQUEST['datatramite_fim']);
		
		$filtro .= " AND date_part('year', hd1.htddata) = '{$arData[2]}' AND date_part('month', hd1.htddata) = '{$arData[1]}' AND date_part('day', hd1.htddata) = '{$arData[0]}'";
	}
	
	if( $_REQUEST['datatramite_inicio'] || $_REQUEST['datatramite_fim'] ){
		$stOrder .= " usu.usunome, data, ";		
	}
	
	if($_REQUEST['pagamento'] == "1") {
		$filtro .= " AND ( pag.pagsituacaopagamento ilike '%SOLICITA%' OR pag.pagsituacaopagamento ilike '%AUTORIZADO%' )";
	}
	if($_REQUEST['pagamento'] == "2") {
		$filtro .= " AND pag.pagsituacaopagamento is NULL ";
	}
	if($_REQUEST['pagamento'] == "3") {
		$filtro .= " AND pag.pagsituacaopagamento ilike '%EFETIVADO%' ";
	}
	
	if($_REQUEST['selecao'] == 'pac'){
		if($_REQUEST['assinado'] == "1") {
			$filtro .= " AND ter.terassinado = TRUE ";
		}
		if($_REQUEST['assinado'] == "2") {
			$filtro .= " AND ( ter.terassinado = FALSE OR ter.terassinado IS NULL ) ";
		}
	}
	
	if($_REQUEST['mcmv'] == "t") {
		$filtro .= " AND po.premcmv = 't' ";
	}elseif($_REQUEST['mcmv'] == "f"){
		$filtro .= " AND po.premcmv = 'f' ";
	}
	if($_REQUEST['carga'] == "t") {
		$filtro .= " AND po.precarga = 't' ";
	}elseif($_REQUEST['carga'] == "f"){
		$filtro .= " AND po.precarga = 'f' ";
	}

	if($_REQUEST['esferafiltro'] == "e") {
		$filtro .= " AND po.preesfera = 'E' ";
	} elseif($_REQUEST['esferafiltro'] == "m") {
		$filtro .= " AND po.preesfera = 'M' ";
	}
	
	if($_REQUEST['selecao'] == 'par'){
		$leftSelecao = "LEFT JOIN par.processopar 				pro ON pro.muncod = po.muncod";
		$sel = "'Não Informado' as resolucao,
				'' as vlrempenho,";
		$gr = "";
	} else {
		$leftSelecao = "LEFT JOIN par.pagamentoobra				pgo ON pgo.preid  = po.preid
						LEFT JOIN par.pagamento					pag ON pag.pagid  = pgo.pagid AND pag.pagstatus = 'A'
						LEFT JOIN par.processoobra 			pro ON pro.muncod = po.muncod and pro.prostatus = 'A'
						LEFT JOIN par.termocompromissopac       ter ON ter.proid  = pro.proid AND ter.terstatus='A'
						LEFT JOIN par.resolucao 				res ON res.resid  = po.resid
						LEFT JOIN (SELECT DISTINCT sum(eobvalorempenho) as vlr, preid FROM par.empenhoobra where eobstatus = 'A' GROUP BY preid) emp ON emp.preid = po.preid";
		$sel = "CASE WHEN resnumero IS NULL
					THEN 'Não Informado'
					ELSE res.resdescricao
				END as resolucao,
				emp.vlr as vlrempenho,
				CASE WHEN emp.preid IS NOT NULL
					THEN 'Com Empenho'
					ELSE 'Sem Empenho'
				END as empenhado,";
		$gr = "res.resnumero,
				res.resdescricao,
				emp.vlr,
				emp.preid,";
	}
	
	// monta o sql 
	if( $_REQUEST['selecao'] == 'pac' ){
	$sql = "SELECT DISTINCT ";
			if( $_POST['tipo'] == 2 ){ //excel
				$sql .= " po.preid || ' - ' || po.predescricao as nomedaobra, ";
			} else {
				$sql .= " '<a href=\"javascript:abreObrasPainel(' || po.preid || ',\'' || COALESCE(mun.muncod,'') || '\')\" >' || po.preid || ' - ' || po.predescricao || '</a>' as nomedaobra, ";
			}
			$sql .= "ed.esddsc,
				CASE WHEN ed.esdid IN(194,215,218,212,217,211,210) THEN 'Em análise' ELSE ed.esddsc END as esddsc_,
				tpo.ptodescricao,
				po.estuf as uf,
				mun.muncod,
				CASE WHEN mun.mundescricao IS NOT NULL 
					THEN   mun.estuf || '-' || mun.mundescricao 
					ELSE  est.estdescricao 
				END as municipio,
				{$select}
				regdescricao,
				CASE 
					WHEN tpo.ptoclassificacaoobra = 'Q' THEN 'Quadra'
					WHEN tpo.ptoclassificacaoobra = 'C' THEN 'Cobertura'
					WHEN tpo.ptoclassificacaoobra = 'P' THEN 'Pro-Infância'
					WHEN tpo.ptoclassificacaoobra = 'E' THEN 'Escola'
					ELSE ' - '
				END as tipoobra,
				1 as qtd,
				--sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as vlr,
				po.prevalorobra as vlr,
				docs.data,
				ent.entcodent,
				ent.entnome,
				usu.usunome as analista,
				preanometa, preanoselecao,
				{$sel}
				CASE WHEN po.preesfera = 'M' THEN 'Municipal' WHEN po.preesfera = 'E' THEN 'Estadual' END as preesfera
			FROM
				obras.preobra po
			{$leftSelecao}
			LEFT JOIN territorios.municipio         mun ON mun.muncod = po.muncod
			LEFT JOIN obras.pretipoobra             tpo ON tpo.ptoid  = po.ptoid
			LEFT JOIN entidade.entidade 			ent ON ent.entcodent = po.entcodent
			LEFT JOIN workflow.documento            doc ON doc.docid  = po.docid
			LEFT JOIN workflow.estadodocumento       ed ON ed.esdid   = doc.esdid
			
			".($filtro_cpf ? 'INNER' : 'LEFT')." JOIN (select distinct
															max(hd.hstid) as hstid,
															max(to_char(hd.htddata, 'DD/MM/YYYY')) as data,					
															pre.docid
														from obras.preobra pre
														inner join obras.preobraanalise poa on poa.preid = pre.preid
														inner join workflow.historicodocumento hd on hd.docid = pre.docid
														inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
														where ac.esdiddestino in (210,211,212)
														{$filtro_cpf}
														group by pre.docid					
													) AS docs ON po.docid = docs.docid
								
			LEFT JOIN workflow.historicodocumento hd1 ON hd1.hstid = docs.hstid
			LEFT JOIN seguranca.usuario usu ON usu.usucpf = hd1.usucpf	
			LEFT JOIN territorios.muntipomunicipio mtpm ON mtpm.muncod = mun.muncod
			LEFT JOIN territorios.tipomunicipio     tpm ON tpm.tpmid   = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
			LEFT JOIN territorios.estado            est ON est.estuf   = po.estuf
			LEFT JOIN territorios.regiao 			  reg ON reg.regcod  = est.regcod
			WHERE
				po.prestatus = 'A' {$filtro} AND po.tooid = 1 AND po.docid IS NOT NULL
			GROUP BY
				po.preid,po.predescricao,po.muncod,po.estuf,
				mun.muncod,mun.mundescricao, mun.estuf,
				ed.esddsc,ed.esdid,
				tpo.ptodescricao,tpo.ptoid,tpo.ptoclassificacaoobra,
				tpm.tpmdsc,reg.regdescricao,
				docs.data,usu.usunome,
				po.prevalorobra,
				mun.muncod,
				{$gr}
				po.preesfera,
				est.estdescricao, ent.entcodent, ent.entnome,
				preanometa, preanoselecao
			ORDER BY
				{$stOrder} uf, municipio, nomedaobra, ed.esddsc, tpo.ptodescricao " ;
	} else{
		$sql = "SELECT DISTINCT	
					po.preid || ' - ' || po.predescricao as nomedaobra,
					ed.esddsc,
					CASE WHEN ed.esdid IN(194,215,218,212,217,211,210) THEN 'Em análise' ELSE ed.esddsc END as esddsc_,
					tpo.ptodescricao,
					po.estuf as uf,
					mun.muncod,
					CASE WHEN mun.mundescricao IS NOT NULL 
						THEN mun.estuf || '-' || mun.mundescricao 
						ELSE est.estdescricao 
					END as municipio,
					{$select}
					regdescricao,
					CASE 
						WHEN tpo.ptoclassificacaoobra = 'Q' THEN 'Quadra'
						WHEN tpo.ptoclassificacaoobra = 'C' THEN 'Cobertura'
						WHEN tpo.ptoclassificacaoobra = 'P' THEN 'Pro-Infância'
						WHEN tpo.ptoclassificacaoobra = 'E' THEN 'Escola'
						ELSE ' - '
					END as tipoobra,
					1 as qtd,
					--sum(coalesce(ppo.ppovalorunitario, 0)*itc.itcquantidade) as vlr,
					po.prevalorobra as vlr,
					docs.data,
					ent.entcodent,
					ent.entnome,
					usu.usunome as analista,
					preanometa, preanoselecao,
					{$sel}
					CASE WHEN po.preesfera = 'M' THEN 'Municipal' WHEN po.preesfera = 'E' THEN 'Estadual' END as preesfera
				FROM 
					par.subacaoobra sbo
				INNER JOIN par.subacao 					s ON s.sbaid = sbo.sbaid AND s.sbastatus = 'A'
				INNER JOIN obras.preobra 				po ON po.preid = sbo.preid AND po.prestatus = 'A'
				INNER JOIN workflow.documento 			doc ON doc.docid = po.docid
				INNER JOIN workflow.estadodocumento 	ed ON ed.esdid = doc.esdid
				".($filtro_cpf ? 'INNER' : 'LEFT')." JOIN (select distinct
															max(hd.hstid) as hstid,
															max(to_char(hd.htddata, 'DD/MM/YYYY')) as data,					
															pre.docid
														from obras.preobra pre
														inner join obras.preobraanalise poa on poa.preid = pre.preid
														inner join workflow.historicodocumento hd on hd.docid = pre.docid
														inner join workflow.acaoestadodoc ac on ac.aedid = hd.aedid
														where ac.esdiddestino in (210,211,212)
														{$filtro_cpf}
														group by pre.docid					
													) AS docs ON po.docid = docs.docid
								
				LEFT JOIN workflow.historicodocumento 	hd1 ON hd1.hstid = docs.hstid
				LEFT JOIN seguranca.usuario 			usu ON usu.usucpf = hd1.usucpf	
				LEFT JOIN entidade.entidade ent ON ent.entcodent = po.entcodent
				INNER JOIN obras.pretipoobra 			tpo ON tpo.ptoid = po.ptoid AND tpo.ptostatus = 'A'
				INNER JOIN par.acao 					a ON a.aciid = s.aciid AND a.acistatus = 'A'
				INNER JOIN par.pontuacao 				p ON p.ptoid = a.ptoid AND p.ptostatus = 'A'
				INNER JOIN par.instrumentounidade 		iu ON iu.inuid = p.inuid
				LEFT JOIN territorios.municipio 		mun ON mun.muncod = po.muncod
				LEFT JOIN territorios.muntipomunicipio mtpm ON mtpm.muncod = mun.muncod
				LEFT JOIN territorios.tipomunicipio     tpm ON tpm.tpmid   = mtpm.tpmid AND tpmstatus = 'A' AND gtmid = 7
				LEFT JOIN territorios.estado            est ON est.estuf   = po.estuf
				LEFT JOIN territorios.regiao 			reg ON reg.regcod  = est.regcod
				WHERE
					po.preidpai IS NULL
					{$filtro}
					AND po.tooid = 2 
				
				GROUP BY
					po.preid,po.predescricao,po.muncod,po.estuf,
					mun.muncod,mun.mundescricao, mun.estuf,
					ed.esddsc,ed.esdid,
					tpo.ptodescricao,tpo.ptoid,tpo.ptoclassificacaoobra,
					tpm.tpmdsc,reg.regdescricao,
					docs.data,usu.usunome,
					po.prevalorobra,
					{$gr}
					po.preesfera,
					est.estdescricao, ent.entcodent, ent.entnome,
					preanometa, preanoselecao
				ORDER BY
					{$stOrder} uf, municipio, nomedaobra, ed.esddsc, tpo.ptodescricao " ;
	}
	//dbg($sql,1);
	return $sql;
	
}

function obras_monta_coluna_painel_gerencial(){
	
	$coluna = array();
	
	if( $_POST['tipo'] == 2 ){ //excel
		$agrupador = $_POST['colunas2'];
	} else {
		$colunas = str_replace("}{",",",$_POST["colunas2"]);
		$colunas = str_replace("}","",$colunas);
		$colunas = str_replace("{","",$colunas);
		$colunas = str_replace("\'","'",$colunas);
		$colunas = explode(",",$colunas);
		$agrupador = $colunas;//array('nomedaobra');
	}

	foreach ($agrupador as $val){
		switch ($val) {
				case 'qtd':
		
					array_push( $coluna, array("campo" 	  => "qtd",
									   		   "label" 	  => "Quantidade de Obras",
									   		   "blockAgp" => "nomedaobra",
									   		   "type"	  => "numeric") );
				continue;
			break;
				case 'vlr':
					array_push( $coluna, array("campo" 	  => "vlr",
									   		   "label" 	  => "Valor das Obras",
									   		   "blockAgp" => "",
									   		   "type"	  => "numeric") );
				continue;
			break;
				case 'vlrempenho':
					array_push( $coluna, array("campo" 	  => "vlrempenho",
									   		   "label" 	  => "Valores Empenhados das Obras",
									   		   "blockAgp" => "",
									   		   "type"	  => "numeric") );
				continue;
			break;
				case 'esddsc':
					array_push( $coluna, array("campo" 	  => "esddsc",
									   		   "label" 	  => "Situação da Obra",
									   		   "type"	  => "string") );
				continue;
			break;
				case 'muncod':
					array_push( $coluna, array("campo" 	  => "muncod",
									   		   "label" 	  => "Código do Município",
									   		   "type"	  => "string") );
				continue;
			break;
				case 'municipio':
					array_push( $coluna, array("campo" 	  => "municipio",
									   		   "label" 	  => "Município",
									   		   "type"	  => "string") );
				continue;
			break;
				case 'muncod':
					array_push( $coluna, array("campo" 	  => "muncod",
									   		   "label" 	  => "Código IBGE",
									   		   "type"	  => "string") );
				continue;
			break;
				case 'uf':
					array_push( $coluna, array("campo" 	  => "uf",
									   		   "label" 	  => "Estado",
									   		   "type"	  => "string") );
				continue;
			break;
				case 'nomedaobra':
					array_push( $coluna, array("campo" 	  => "nomedaobra",
									   		   "label" 	  => "Nome da Obra",
									   		   "type"	  => "string") );
				continue;
			break;
				case 'resolucao':
					array_push( $coluna, array("campo" 	  => "resolucao",
									   		   "label" 	  => "Resolução",
									   		   "type"	  => "string") );
				continue;
			break;
				case 'entcodent':
					array_push( $coluna, array("campo" 	  => "entcodent",
									   		   "label" 	  => "Código da Escola",
									   		   "type"	  => "string") );
				continue;
			break;
				case 'entnome':
					array_push( $coluna, array("campo" 	  => "entnome",
									   		   "label" 	  => "Nome da Escola",
									   		   "type"	  => "string") );
				continue;
			break;
			case 'preesfera':
					array_push( $coluna, array("campo" 	  => "preesfera",
									   		   "label" 	  => "Esfera",
									   		   "type"	  => "string") );
				continue;
			break;
			case 'preanometa':
					array_push( $coluna, array("campo" 	  => "preanometa",
									   		   "label" 	  => "Ano da Meta",
									   		   "type"	  => "string") );
				continue;
			break;
			case 'preanoselecao':
					array_push( $coluna, array("campo" 	  => "preanoselecao",
									   		   "label" 	  => "Ano da Seleção",
									   		   "type"	  => "string") );
				continue;
			break;	
		}
	}
	
	return $coluna;
	
}


/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['espandir']) $expandir = true;
else $expandir = false;

// Inclui componente de relatórios
include APPRAIZ. 'includes/classes/relatorio.class.inc';

// instancia a classe de relatório
$rel = new montaRelatorio();

// monta o sql, agrupador e coluna do relatório
$sql       = obras_monta_sql_painel_gerencial();
$agrupador = obras_monta_agp_painel_gerencial();
$coluna    = obras_monta_coluna_painel_gerencial();
$dados 	   = $db->carregar( $sql );

$rel->setAgrupador($agrupador, $dados);
$rel->setColuna($coluna);
$rel->setTolizadorLinha(true);
$rel->setEspandir($expandir);
$rel->setTotNivel(true);

if( $_POST['tipo'] == 2 ){
	ob_clean();
	$nomeDoArquivoXls="Relatório_Painel_Gerenciamento_".date('d-m-Y_H_i');
	echo $rel->getRelatorioXls();
} else {
	echo $rel->getRelatorio();
} 
?>
