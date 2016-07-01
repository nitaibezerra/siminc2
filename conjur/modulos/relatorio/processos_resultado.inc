<?php
//dbg($_REQUEST,1);
ini_set("memory_limit", "2048M");

include APPRAIZ. 'includes/classes/relatorio.class.inc';
$agrup = monta_agp();
$col   = monta_coluna();
$sql   = monta_sql();
$dados = $db->carregar($sql);

if( $_POST['req'] == 'geraxls' ){
	$arCabecalho = array();
	$colXls = array();
	foreach ($_POST['agrupador'] as $agrup){
		if($agrup == 'cadastrador'){
			array_push($arCabecalho, 'Cadastrador');
			array_push($colXls, 'cadastrador');
		}
		if($agrup == 'nuprocesso'){
			array_push($arCabecalho, 'N° do Processo SIDOC');
			array_push($colXls, 'nuprocesso');
		}
		if($agrup == 'coordenacao'){
			array_push($arCabecalho, 'Coordenação');
			array_push($colXls, 'coordenacao');
		}
		if($agrup == 'advogado'){
			array_push($arCabecalho, 'Advogado');
			array_push($colXls, 'advogado');
		}
		if($agrup == 'tema'){
			array_push($arCabecalho, 'Tema');
			array_push($colXls, 'tema');
		}
		if($agrup == 'procedencia'){
			array_push($arCabecalho, 'Procedência');
			array_push($colXls, 'procedencia');
		}
		if($agrup == 'tipoprocesso'){
			array_push($arCabecalho, 'Tipo de Processo');
			array_push($colXls, 'tipoprocesso');
		}
		if($agrup == 'prioridade'){
			array_push($arCabecalho, 'Prioridade');
			array_push($colXls, 'prioridade');
		}
	}
	foreach($col as $cabecalho){
		array_push($arCabecalho, $cabecalho['label']);
		array_push($colXls, $cabecalho['campo']);
	}
	
	$arDados = Array();
	foreach( $dados as $k => $registro ){
		foreach( $colXls as $campo ){
			$arDados[$k][$campo] = $registro[$campo];
		}
	}
//	ver($dados,d);
//	$arDados = ($dados) ? $dados : array();
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatorioIndicadores".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatDocente".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$arCabecalho,100000,5,'N','100%',$par2);
	die;
}


?>
<html>
	<head>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
	</head>
<body marginheight="0" marginwidth="0" leftmargin="0" topmargin="0">	

<?php
//dbg($sql,1);
$r = new montaRelatorio();
$r->setAgrupador($agrup, $dados); 
$r->setColuna($col);
$r->setTotNivel(true);
$r->setMonstrarTolizadorNivel(true);
$r->setBrasao(true);
$r->setEspandir( $_REQUEST['expandir']);
echo $r->getRelatorio();

function monta_sql(){
	extract($_REQUEST);
	$where = array();
	
	// coordenação
	if( $coordenacao[0] && $coordenacao_campo_flag ){
		array_push($where, " coo.coonid " . (!$coordenacao_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $coordenacao ) . "') ");
	}
	
	// advogado
	if( $advogado[0] && $advogado_campo_flag ){
		array_push($where, " adv.advid " . (!$advogado_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $advogado ) . "') ");
	}
	
	// tema
	if( $tema[0] && $tema_campo_flag ){
		array_push($where, " tia.tasid " . (!$tema_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tema ) . "') ");
	}
	
	// procedência
	if( $procedencia[0]  && $procedencia_campo_flag ){
		array_push($where, " pcd.proid " . (!$procedencia_campo_excludente ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $procedencia ) . "') ");
	}
	
	// tipoprocesso
	if( $tipoprocesso[0] && $tipoprocesso_campo_flag ){
		array_push($where, " tip.tprid " . (!$tipoprocesso_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $tipoprocesso ) . ") ");
	}
	
	// prioridade
	if( $prioridade[0] && $prioridade_campo_flag ){
		array_push($where, " tid.tipid " . (!$prioridade_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $prioridade ) . ") ");
	}
	
	// situacao
	if( $situacao[0] && $situacao_campo_flag ){
		// período
		if( $dtinicio && $dtfim ){
			$dtinicio = formata_data_sql($dtinicio);
			$dtfim = formata_data_sql($dtfim);
			array_push($where, " aed.esdidorigem " . (!$situacao_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $situacao ) . ") AND
								 hdoc.htddata BETWEEN '{$dtinicio}' AND '{$dtfim}' ");
		}else{
			array_push($where, " esd.esdid " . (!$situacao_campo_excludente ? ' IN ' : ' NOT IN ') . " (" . implode( ',', $situacao ) . ") ");
		}
	}else{
		// período
		if( $dtinicio && $dtfim ){
			$dtinicio = formata_data_sql($dtinicio);
			$dtfim = formata_data_sql($dtfim);
			array_push($where, " prc.prcdtinclusao BETWEEN '{$dtinicio}' AND '{$dtfim}' ");
		}
	}	
	
	

	
	
	$agrupadorsql = $_REQUEST['agrupador'];
	$whereaux = "";
//	if($agrupadorsql[0]=='coordenacao') $whereaux = " AND coo.coodsc <> '' ";
//	if($agrupadorsql[0]=='advogado') $whereaux = " AND ent.entnome <> '' ";
	if(count($agrupadorsql)==2){
		if($agrupadorsql[0]=='coordenacao' && $agrupadorsql[1]=='advogado') $whereaux = " AND coo.coodsc <> '' AND ent.entnome <> '' ";
		if($agrupadorsql[0]=='advogado' && $agrupadorsql[1]=='coordenacao') $whereaux = " AND ent.entnome <> '' AND coo.coodsc <> '' ";
	}
	
	//if (in_array("coordenacao", $agrupadorsql)) $whereaux = " AND coo.coodsc <> '' ";
	//if (in_array("advogado", $agrupadorsql)) $whereaux = " AND ent.entnome <> '' ";
		
	//conjur.php?modulo=principal/editarprocesso&acao=A&prcid=137
	//'<a style=\"cursor:pointer;\" onclick=\"parent.opener.window.location.href=\'/obras/obras.php?modulo=principal/cadastro&acao=A&obrid=' || oi.obrid || '\'; parent.opener.window.focus();\">' || oi.obrdesc || '</a>' as nomedaobra,
	
	// monta o sql 
	if( $_POST['req'] == 'geraxls' ){
		$sql = "SELECT DISTINCT 
					cad.usunome as cadastrador,
					prc.prcnumsidoc as nuprocesso,
					to_char(prc.prcdtentrada, 'dd/mm/YYYY') as dataentrada,
	            	CASE 
						WHEN data is not null THEN 
							to_char(data	, 'DD/MM/YYYY')
						ELSE '-' 
					END as data,
					prc.prcnomeinteressado as interessado,
					esd.esddsc as situacao,
					prc.prcdesc,
					prc.prcdtentrada,
					tip.tprdsc as tipoprocesso,
					tia.tasdsc as tema,
					tid.tipdsc as prioridade,
					pcd.prodsc as procedencia,
					CASE WHEN coo.coodsc<>'' THEN coo.coodsc
						 ELSE 'Não Informado'
					END as coordenacao,
					CASE WHEN ent.entnome<>'' THEN ent.entnome
						 ELSE 'Não Informado'
					END as advogado
				FROM 
					conjur.processoconjur prc
				INNER JOIN 
					conjur.tipoprocesso tip ON tip.tprid = prc.tprid AND tip.tprstatus='A'
				INNER JOIN 
					conjur.tipoassunto tia ON tia.tasid = prc.tasid AND tia.tasstatus='A'
				INNER JOIN 
					conjur.tipoprioridade tid ON tid.tipid = prc.tipid
				INNER JOIN 
					conjur.procedencia pcd ON pcd.proid = prc.proid AND pcd.prodtstatus='A'
				LEFT JOIN 
					conjur.coordenacao coo ON coo.coonid = prc.cooid AND coo.coostatus='A' 
				LEFT JOIN 
					conjur.advogados adv ON adv.advid = prc.advid
				LEFT JOIN 
					entidade.entidade ent ON ent.entid = adv.entid AND ent.entstatus='A'
				LEFT JOIN 
					conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
				LEFT JOIN 
					workflow.documento doc ON doc.docid = esp.docid 
				LEFT JOIN 
					(SELECT max(htddata) as data, docid FROM workflow.historicodocumento GROUP BY docid ) wd ON wd.docid = doc.docid
				LEFT JOIN 
					workflow.estadodocumento esd ON esd.esdid = doc.esdid
				LEFT JOIN
					workflow.historicodocumento hdoc ON hdoc.docid = esp.docid
				LEFT JOIN
					workflow.acaoestadodoc       aed ON aed.aedid  = hdoc.aedid
				LEFT JOIN 
					workflow.estadodocumento    esd2 ON esd2.esdid = aed.esdidorigem
				WHERE
					prc.prcstatus='A' " . $whereaux . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "   
				ORDER BY
					prc.prcdesc";
	}else{
		$sql = "SELECT DISTINCT 
					cad.usunome as cadastrador,
					CASE WHEN (esd.esdid <> 80) THEN
	                    CASE WHEN (current_date - INTERVAL '15 days' > prc.prcdtentrada AND tid.tipid = 2 ) 
	                        THEN '<a style=\"cursor:pointer; color:red;\" onclick=\"chamaproc(' || prc.prcid || ');\">' || prc.prcnumsidoc || '</a>' 
	                    ELSE 
	                        CASE WHEN (current_date - INTERVAL '5 days' > prc.prcdtentrada AND tid.tipid = 1 ) 
	                            THEN '<a style=\"cursor:pointer; color:red;\" onclick=\"chamaproc(' || prc.prcid || ');\">' || prc.prcnumsidoc || '</a>' 
	                        ELSE '<a style=\"cursor:pointer; color:black;\" onclick=\"chamaproc(' || prc.prcid || ');\">' || prc.prcnumsidoc || '</a>' 
	                        END 
	                    END 
	            	ELSE '<a style=\"cursor:pointer; color:black;\" onclick=\"chamaproc(' || prc.prcid || ');\">' || prc.prcnumsidoc || '</a>' 
	            	END as nuprocesso,
					--prc.prcnumsidoc as nuprocesso,
					
					CASE WHEN (esd.esdid <> 80) THEN
	                    CASE WHEN (current_date - INTERVAL '15 days' > prc.prcdtentrada AND tid.tipid = 2 ) 
	                        THEN '<font color=red>' || to_char(prc.prcdtentrada, 'dd/mm/YYYY') || '</font>' 
	                    ELSE 
	                        CASE WHEN (current_date - INTERVAL '5 days' > prc.prcdtentrada AND tid.tipid = 1 ) 
	                            THEN '<font color=red>' || to_char(prc.prcdtentrada, 'dd/mm/YYYY') || '</font>' 
	                        ELSE to_char(prc.prcdtentrada, 'dd/mm/YYYY') END 
	                    END 
	            	ELSE to_char(prc.prcdtentrada, 'dd/mm/YYYY') END as dataentrada,
	            	CASE 
						WHEN data is not null THEN 
							to_char(data	, 'DD/MM/YYYY')
						ELSE '-' 
					END as data,
	            	
	            	--to_char(prc.prcdtentrada, 'dd/mm/YYYY') as dataentrada,
					
					prc.prcnomeinteressado as interessado,
					esd.esddsc as situacao,
					prc.prcdesc,
					prc.prcdtentrada,
					tip.tprdsc as tipoprocesso,
					tia.tasdsc as tema,
					tid.tipdsc as prioridade,
					pcd.prodsc as procedencia,
					CASE WHEN coo.coodsc<>'' THEN coo.coodsc
						 ELSE 'Não Informado'
					END as coordenacao,
					CASE WHEN ent.entnome<>'' THEN ent.entnome
						 ELSE 'Não Informado'
					END as advogado
					--, 1 AS quant
				FROM 
					conjur.processoconjur prc
				INNER JOIN 
					conjur.tipoprocesso tip ON tip.tprid = prc.tprid AND tip.tprstatus='A'
				INNER JOIN 
					conjur.tipoassunto tia ON tia.tasid = prc.tasid AND tia.tasstatus='A'
				INNER JOIN 
					conjur.tipoprioridade tid ON tid.tipid = prc.tipid
				INNER JOIN 
					conjur.procedencia pcd ON pcd.proid = prc.proid AND pcd.prodtstatus='A'
				LEFT JOIN 
					conjur.coordenacao coo ON coo.coonid = prc.cooid AND coo.coostatus='A' 
				LEFT JOIN 
					conjur.advogados adv ON adv.advid = prc.advid
				LEFT JOIN 
					entidade.entidade ent ON ent.entid = adv.entid AND ent.entstatus='A'
				LEFT JOIN 
					conjur.estruturaprocesso esp ON prc.prcid = esp.prcid 
				LEFT JOIN 
					workflow.documento doc ON doc.docid = esp.docid 
				LEFT JOIN 
					(SELECT max(htddata) as data, docid FROM workflow.historicodocumento GROUP BY docid ) wd ON wd.docid = doc.docid
				LEFT JOIN 
					workflow.estadodocumento esd ON esd.esdid = doc.esdid
				LEFT JOIN
					workflow.historicodocumento hdoc ON hdoc.docid = esp.docid
				LEFT JOIN
					workflow.acaoestadodoc       aed ON aed.aedid  = hdoc.aedid
				LEFT JOIN 
					workflow.estadodocumento    esd2 ON esd2.esdid = aed.esdidorigem
				WHERE
					prc.prcstatus='A' " . $whereaux . ( $where[0] ? ' AND' . implode(' AND ', $where) : '' ) . "   
				ORDER BY
					prc.prcdesc";
	}
	
	
	return $sql;
}

function monta_agp(){
	$agrupador = $_REQUEST['agrupador'];
	//dbg($agrupador,1);
	
	if (in_array("nuprocesso", $agrupador)){
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array(
												/*"quant",
										   		"nome", 
	 									   		"estado",
								   				"municipio",*/ 
												"interessado",
												"prioridade",
												"dataentrada",
												"data",
												"coordenacao",
												"situacao"			   		
											  )	  
					);
	}
	else{
		$agp = array(
					"agrupador" => array(),
					"agrupadoColuna" => array("")	  
					);
	}
	
	foreach ($agrupador as $val): 
		switch ($val) {
		    case 'cadastrador':
				array_push($agp['agrupador'], array(
													"campo" => "cadastrador",
											  		"label" => "Cadastrador")										
									   				);				
		    	continue;
		        break;
		    case 'nuprocesso':
				array_push($agp['agrupador'], array(
													"campo" => "nuprocesso",
											  		"label" => "Nº do Processo SIDOC")										
									   				);				
		    	continue;
		        break;
		    /*
		    case 'prcdesc':
				array_push($agp['agrupador'], array(
													"campo" => "prcdesc",
											  		"label" => "Descrição")										
									   				);					
		    	continue;
		        break;
			*/		    	
		    case 'tipoprocesso':
				array_push($agp['agrupador'], array(
													"campo" => "tipoprocesso",
											 		"label" => "Tipo do Processo")										
									   				);					
		    	continue;			
		        break;	
		    case 'tema':
				array_push($agp['agrupador'], array(
												"campo" => "tema",
												"label" => "Tema")										
										   		);	
				continue;
				break;
					    	
		    case 'prioridade':
				array_push($agp['agrupador'], array(
												"campo" => "prioridade",
												"label" => "Prioridade")										
										   		);	
				continue;
				break;	 

					    	
		    case 'procedencia':
				array_push($agp['agrupador'], array(
												"campo" => "procedencia",
												"label" => "Procedência")										
										   		);	
				continue;
				break;	 
				
					    	
		    case 'coordenacao':
				array_push($agp['agrupador'], array(
												"campo" => "coordenacao",
												"label" => "Coordenação")										
										   		);	
				continue;
				break;	 

		    case 'advogado':
				array_push($agp['agrupador'], array(
												"campo" => "advogado",
												"label" => "Advogado")										
										   		);	
				continue;
				break;	 
			/*
		    case 'prcdtentrada':
				array_push($agp['agrupador'], array(
												"campo" => "prcdtentrada",
												"label" => "Data de Entrada")										
										   		);	
				continue;
				break;	 
			*/
		}
	endforeach;
	
	return $agp;
}

function monta_coluna(){
	$agrupador = $_REQUEST['agrupador'];
	//dbg($agrupador,1);
	
	if (in_array("nuprocesso", $agrupador)){	
		$coluna    = array(
						/*array(
							  "campo" 	 => "quant",
					   		  "label" 	 => "Total",
							  "type" 	 => "numeric"
						),*/
						array(
							  "campo" => "interessado",
					   		  "label" => "Interessado"
						),	
						array(
							  "campo" => "prioridade",
					   		  "label" => "Prioridade"
						),	
						array(
							  "campo" => "dataentrada",
					   		  "label" => "Data de Entrada"	
						),
						array(
							  "campo" => "data",
					   		  "label" => "Data da Última Tramitação"	
						),
						array(
							  "campo" => "coordenacao",
					   		  "label" => "Coordenação"
						),	
						array(
							  "campo" => "situacao",
					   		  "label" => "Situação"	
						)/*,
						array(
							  "campo" 	 => "quant",
					   		  "label" 	 => "Quantidade de Escolas",
					   		  "blockAgp" => "nome",
					   		  "type"	 => "numeric"
						),	
						array(
							  "campo" => "valor",
					   		  "label" => "Valor Paf"	
						)
						*/				
		
				);
	}
					  	
	return $coluna;			  	
}
?>
</body>
</html>

<script>
	function chamaproc(prcid)
	{
		window.opener.location="conjur.php?modulo=principal/editarprocesso&acao=A&prcid="+prcid;
		window.opener.focus();
		
	}
</script>