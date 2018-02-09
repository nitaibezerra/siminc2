<?php
set_time_limit(0);
ini_set("memory_limit", "1024M");
montaBotaoForm();
//include APPRAIZ. 'includes/classes/relatorio.class.inc';

echo '<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	 <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>';

if($_POST['limpaSession']){
	unset($_SESSION['emenda']['post']);
	$_POST['limpaSession'] = "";
}

if( empty($_SESSION['emenda']['post']) ){
	$_SESSION['emenda']['post'] = $_POST;
}
$col = monta_coluna();
$coluna = array();
$cabecalho = array();
$id = array();

foreach ($col as $c) {
	if( $c['campo'] ){
		$coluna[] = $c['campo'];
	}
	$cabecalho[] = $c['label'];
	
	if( strstr( strtoupper($c['label']), strtoupper('valor') ) ) {
		$formato[] = "n";
	} else {
		$formato[] = "";
	}
	$id[] = $c['id'];
}

$arSql   = monta_sql($coluna, $id);
sort($arSql['sql']);

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	sort($arSql['excel']);
	$db->sql_to_excel($arSql['excel'], 'relEmendasPTA', $cabecalho, $formato);
	exit;
	$exporta = "false";
}

$cabecalhoBrasao .= "<table width=\"100%\" cellspacing=\"1\" cellpadding=\"5\" border=\"0\" align=\"center\" class=\"tabela\">";
$cabecalhoBrasao .= "<tr>" .
				"<td colspan=\"100\">" .			
					monta_cabecalho_relatorio('100') .
				"</td>" .
			  "</tr>
			  </table>";

echo $cabecalhoBrasao;
$db->monta_lista_array( $arSql['sql'], $cabecalho, 100000, 1, 'N', 'center', '');

montaBotaoForm();

echo '<script>
		$(\'loader-container\').hide();
	  </script>';

function montaJoinColunas( $arColunas, $arFiltros ){
	$arJoin = array();
	$arCols = array();
	
	//foreach( $arColunas as $stColuna ){
		/*if( in_array( 'responsavel', $arColunas ) || in_array( 'responsavel', $arFiltros ) ){
			(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
			$join = "left join emenda.responsavel res on res.resid = eme.resid";
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
		}*/
		/*if( in_array( 'tpeid', $arColunas ) || in_array( 'tpeid', $arFiltros ) ){
			(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
			$join = "left join emenda.responsavel res on res.resid = eme.resid";
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			$join = "left join emenda.tipoensino tpe on tpe.resid = res.resid and tpe.tpestatus = 'A'";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			
		} */
		/*if( in_array( 'entidade', $arColunas) || in_array( 'valorentidade', $arColunas) || in_array( 'usucpf', $arColunas) || in_array( 'entidade', $arFiltros) || 
				in_array( 'estufentidade', $arFiltros) || in_array( 'mundescricao', $arColunas) ){
			( in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			$join = "left join emenda.entidadebeneficiada ent on (ede.enbid = ent.enbid)
					 left join territorios.municipio mun on mun.muncod = ent.muncod";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			
		}*/
	/*ver($arColunas,d);
		if( in_array( 'emeano', $arColunas ) || in_array( 'autnome', $arColunas ) ){
			//(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
			$join = "inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
				    left join emenda.autor aut 
				    	inner join emenda.tipoautor tpa on tpa.tpaid = aut.tpaid
				    on aut.autid = vede.autid";
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
		}*/
		$arJoinPTA = array();		
		if ( in_array( 'ptrcod', $arColunas) || in_array( 'ptivalortotal', $arColunas) || in_array( 'pmcnumconveniosiafi', $arColunas) || in_array( 'ptrnumconvenio', $arColunas) /*|| 
					in_array( 'esddsc', $arColunas)  || in_array( 'exfvalor', $arColunas) || 
					in_array( 'pmcnumconveniosiafi', $arFiltros) || in_array( 'esddsc', $arFiltros) || in_array( 'ininome', $arColunas) ||
					in_array( 'espid', $arColunas) || in_array( 'espid', $arFiltros) || in_array( 'ptrnumconvenio', $arFiltros) ||
					in_array( 'vigdatainicio', $arColunas) || in_array( 'vigdatafim', $arColunas ) || in_array( 'iniid', $arFiltros ) || 
					in_array( 'unisigla', $arColunas ) || in_array( 'ptrnumprocessoempenho', $arFiltros ) || in_array( 'ptrnumprocessoempenho', $arColunas ) 
					|| in_array( 'pedvalor', $arFiltros )*/ ){
			
			(in_array( "(ptr.ptrcod ||'/'|| ptr.ptrexercicio) as ptrcodexer", $arCols ) ? '' : $arCols[] = "(ptr.ptrcod ||'/'|| ptr.ptrexercicio) as ptrcodexer");
			//(in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			//(in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
			//(in_array( 'ptr.ptrid', $arCols ) ? '' : $arCols[] = 'ptr.ptrid');
			
			/*if( in_array( 'ininome', $arColunas) || in_array( 'iniid', $arFiltros ) ){
				$join = "inner join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.iniciativa ini on ini.iniid = pti.iniid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);				
			}*/
			
			/*if( in_array( 'espid', $arColunas) || in_array( 'espid', $arFiltros) ){				
				$join = "inner join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.ptiniciativaespecificacao ptie on ptie.ptiid = pti.ptiid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.iniciativaespecificacao ine on ine.iceid = ptie.iceid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.especificacao esp on esp.espid = ine.espid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}*/
			
			/*if( in_array( 'pmcnumconveniosiafi', $arColunas) || in_array( 'pmcnumconveniosiafi', $arFiltros) || in_array( 'datareferencia', $arColunas ) ){
				$join = "left join emenda.ptminutaconvenio ptmi on ptmi.ptrid = ptr.ptrid and ptmi.pmcstatus = 'A'
						 left join emenda.ptpublicacao pt on pt.pmcid = ptmi.pmcid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}
			
			if( in_array( 'esddsc', $arColunas) || in_array( 'esddsc', $arFiltros) ){
				$join = "left join workflow.documento doc on ptr.docid = doc.docid 
						  left join workflow.estadodocumento esd on esd.esdid = doc.esdid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}*/
			
			/*
			if( in_array( 'pmcnumconveniosiafi', $arColunas) ){
				$join = "left join emenda.dadosconveniosiafi dcs on substring(dcs.it_nu_original from 1 for 6) = ptr.ptrnumconvenio";
				(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			}
			*/
			
			if( in_array( 'empenhado', $arFiltros) || in_array( 'pagamento', $arFiltros) ){
				$join = "left join emenda.execucaofinanceira ef on ptr.ptrid = ef.ptrid and pte.pedid = ef.pedid and ef.exfstatus = 'A'
						left join emenda.ordembancaria ord on ord.exfid = ef.exfid ";
				(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			}
			if( in_array( 'ptivalortotal', $arColunas) ){
				$join = "left join
							(SELECT ptrid, sum(ptivalortotal) as ptivalortotal
							 FROM emenda.v_ptiniciativa 
							 GROUP BY ptrid) vpti ON vpti.ptrid = ptr.ptrid";
				(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			}
			/*if( in_array( 'vigdatainicio', $arColunas) || in_array( 'vigdatafim', $arColunas) ){
				$join = "left join emenda.ptvigencia ptv ON ptv.ptrid = ptr.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}*/
			
			/*$join = "left join emenda.ptemendadetalheentidade pede
					  	inner join emenda.planotrabalho ptr on ptr.ptrid = pede.ptrid and ptr.ptrstatus = 'A' and ptr.ptrexercicio = ".$_SESSION['exercicio']."
					  	".implode(' ', $arJoinPTA)."
					  on ede.edeid = pede.edeid";
			
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			$join = "left join emenda.entidadebeneficiada ent on (ede.enbid = ent.enbid)
					 left join territorios.municipio mun on mun.muncod = ent.muncod";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);*/
			
			//ver($arColunas,$arJoin,d);
		}
		/*if( in_array( 'obcdsc', $arColunas) ){
			(in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			(in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
		}
		if( in_array( 'iniid', $arColunas) || in_array( 'iniid', $arFiltros) ){
			(in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			(in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
			
			if( in_array( 'iniid', $arColunas) ){
				$join = "left join emenda.iniciativadetalheentidade ide
			            	inner join emenda.iniciativa ini1 on ini1.iniid = ide.iniid
			            on ide.edeid = ede.edeid";
				(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			}
			
		}*/
	//}
	return array("colunas" => $arCols,
				 "joins" => $arJoin);
}

function montaBotaoForm(){
	echo '	<html>
			<head>
				<style>
				
				#loader-container,
				#LOADER-CONTAINER{
				    background: transparent;
				    position: absolute;
				    width: 100%;
				    text-align: center;
				    z-index: 8000;
				    height: 100%;
				}
				
				
				#loader {
				    background-color: #fff;
				    color: #000033;
				    width: 300px;
				    border: 2px solid #cccccc;
				    font-size: 12px;
				    padding: 25px;
				    font-weight: bold;
				    margin: 150px auto;
				}
				</style>
				</head>
			<script type="text/javascript" src="/includes/prototype.js"></script>
			<body>
			<style type="">
				@media print {.notprint { display: none } .div_rolagem{display: none} }	
				@media screen {.notscreen { display: none; }
				
				.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
				
			</style>
			<div id="loader-container" style="display: none">
		   		<div id="loader"><img src="../imagens/wait.gif" border="0" align="middle"><span>Aguarde! Carregando Dados...</span></div>
			</div>
			<form action="" method="post" name="formulario" id="formulario">
			<input type="hidden" id="exporta" name="exporta" value="<?=$exporta; ?>">
			<table  align="center" cellspacing="1" cellpadding="4">
				<tr>
					<td style="height: 20px;"></td>
				</tr>
				<tr>
					<td style="text-align: center;" class="div_rolagem">
						<input type="button" name="fechar" value="Fechar" onclick="javascript: window.close();">
						<input type="button" name="excel" value="Gerar Excel" onclick="exportarExcel();">
					</td>
				</tr>
			</table>
			</form>
			</body>
			<script type="text/javascript">				
				function exportarExcel(){
					document.getElementById(\'exporta\').value = "true";
					document.getElementById(\'formulario\').submit();
				}
				$(\'loader-container\').show();
			</script>
			</html>';
}

function monta_sql($cols, $id){
	global $db;
	$arFiltros = array();
	$carregaCPFresp = false;
	extract($_SESSION['emenda']['post']);
	
	$autid 		 = !is_array($autid) ? explode(',', $autid) : $autid;
	$responsavel = !is_array($responsavel) ? explode(',', $responsavel) : $responsavel;
	$parid 		 = !is_array($parid) ? explode(',', $parid) : $parid;
	$tpaid		 = !is_array($tpaid) ? explode(',', $tpaid) : $tpaid;
	$estuf		 = !is_array($estuf) ? explode(',', $estuf) : $estuf;
	$enbid 		 = !is_array($enbid) ? explode(',', $enbid) : $enbid;
	$unicod 	 = !is_array($unicod) ? explode(',', $unicod) : $unicod;
	$gndcod 	 = !is_array($gndcod) ? explode(',', $gndcod) : $gndcod;
	$acacod 	 = !is_array($acacod) ? explode(',', $acacod) : $acacod;
	$espid   	 = !is_array($espid) ? explode(',', $espid) : $espid;
	$iniid   	 = !is_array($iniid) ? explode(',', $iniid) : $iniid;
	$tpeid   	 = !is_array($tpeid) ? explode(',', $tpeid) : $tpeid;
	
	if( $autid[0] && ( $autid_campo_flag || $autid_campo_flag == '1' )){
		$where[] = " vede.autid " . (( $autid_campo_excludente == null || $autid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $autid ) . "') ";		
	}
	if( $responsavel[0] && ( $responsavel_campo_flag || $responsavel_campo_flag == '1' )){
		$where[] = " res.resid " . (( $responsavel_campo_excludente == null || $responsavel_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $responsavel ) . "') ";
		$arFiltros[] = 'responsavel';		
	}
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){
		$where[] = " enb.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	if( $enbid[0] && ( $enbid_campo_flag || $enbid_campo_flag == '1' )){
		$where[] = " enb.enbid " . (( $entid_campo_excludente == null || $entid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $enbid ) . "') ";
		$arFiltros[] = 'entidade';		
	}
	if( $espid[0] && ( $espid_campo_flag || $espid_campo_flag == '1' )){
		$where[] = " esp.espid " . (( $espid_campo_excludente == null || $espid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $espid ) . "') ";
		$whereEspid[]= " esp.espid " . (( $espid_campo_excludente == null || $espid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $espid ) . "') ";
		$arFiltros[] = 'espid';
	}
	if( $iniid[0] && ( $iniid_campo_flag || $iniid_campo_flag == '1' )){
		$where[] = " pti.iniid " . (( $iniid_campo_excludente == null || $iniid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $iniid ) . "') ";
		$whereIniid[]= " pti.iniid " . (( $iniid_campo_excludente == null || $iniid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $iniid ) . "') ";
		$arFiltros[] = 'iniid';
	}
		
	if( $tpeid[0] && ( $tpeid_campo_flag || $tpeid_campo_flag == '1' )){
		$where[] = " pti.tpeid " . (( $tpeid_campo_excludente == null || $tpeid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpeid ) . "') ";
		$arFiltros[] = 'tpeid';		
	}
	
	
	if($emecod){
		$where[] = "vede.emecod = '".$emecod."'";
	}
	if($ptrcod){
		$where[] = "ptr.ptrcod = '".$ptrcod."'";
	}
	if($pedvalor){
		$where[] = "(select sum(p.pedvalor) from emenda.ptemendadetalheentidade p where p.ptrid = ptr.ptrid) = '".retiraPontos($pedvalor)."'";
		$arFiltros[] = 'pedvalor';
	}
	if( $esdid ){
		$where[] = "esd.esdid = ".$esdid;
		$arFiltros[] = 'esddsc';	
	}
	
	if( $liberadocasacivil ) $where[] = "ede.emdliberado = '".$liberadocasacivil."'";
	
	/*
	if($pmcnumconveniosiafi){
		$where[] = "dcs.it_nu_convenio = '".$pmcnumconveniosiafi."'";
		$arFiltros[] = 'pmcnumconveniosiafi';
	}	
	*/
	
	if($pmcnumconveniosiafi){
		$where[] = "ptmi.pmcnumconveniosiafi = '".$pmcnumconveniosiafi."'";
		$arFiltros[] = 'pmcnumconveniosiafi';
	}	
	

	if($ptrnumconvenio){
		$where[] = "ptr.ptrnumconvenio = '".$ptrnumconvenio."'";
		$arFiltros[] = 'ptrnumconvenio';
	}	
	
	//$where[] = "eme.emeano = ".$_SESSION['exercicio'];
	
	$order = array();
	if($agrupador){
		foreach ($agrupador as $val){
			switch ($val) {
				case 'ptrcod':
					$order[] = 'ptr.ptrcod';
				break;
				/*case 'emecod':
					$order[] = 'eme.emecod';
				break;
				case 'pmcnumconveniosiafi':
					$order[] = 'ptmi.pmcnumconveniosiafi';
					$id[] = 'pmcnumconveniosiafi';
				break;				
				case 'enbid':
					$order[] = 'ent.enbnome';
				break;
				case 'acacod':
					$order[] = 'vfun.acadsc';
				break;
				case 'autid':
					$order[] = 'aut.autnome';
				break;
				case 'estuf':
					$order[] = 'aut.estuf';
				break;
				case 'parid':
					$order[] = 'par.parsigla';
				break;
				case 'gndcod':
					$order[] = 'emd.gndcod';
				break;
				case 'resid':
					$order[] = 'res.resdsc';
				break;
				case 'tpaid':
					$order[] = 'tpa.tpanome';
				break;
				case 'unicod':
					$order[] = 'vfun.unicod';
				break;*/
			}
		}
	}
	#verifica a diferença entre o array de ordenação com o de colunas
	#caso exista diferença, deleta os valores de ordenação que estão diferente do de colunas
	$arDif = array_diff( $order, $cols );
	$arDif = ( $arDif ? $arDif : array() );
	$arDifKey = array_keys( $arDif );

	if( $arDifKey ){
		foreach ($arDifKey as $v) {
			unset( $order[$v] );
		}		
	}
		
	if( !empty($geradoconvenio) ){
		if($geradoconvenio == 'S'){
			$where[] = "ptr.ptrnumconvenio is not null";
		}else{
			$where[] = "ptr.ptrnumconvenio is null";
		}
		$arFiltros[] = 'ptrnumconvenio';
	}
	if( !empty($empenhado) ){
		if($empenhado == 'S'){
			$where[] = " ef.semid = 4 ";
		}else{
			$where[] = " ef.semid <> 4 ";
		}
		$arFiltros[] = 'empenhado';
	}
	if( !empty($pagamento) ){
		if($pagamento == 'S'){
			$where[] = " ord.spgcodigo = '2' ";
		}else{
			$where[] = " ord.spgcodigo <> '2' ";
		}
		$arFiltros[] = 'pagamento';
	}
	if(  in_array( 'exfvalor', $id ) ){
		$where[] = "ef.exfvalor is not null and ef.semid = 4";
	}

	$arColunas = array_merge( $cols, $id );
	$arJoinColunas = montaJoinColunas( $arColunas, $arFiltros );
	$arJoinColunas['colunas'] = array_merge( $arJoinColunas['colunas'], $cols );
	$stColunas = (is_array($arJoinColunas['colunas']) && $arJoinColunas['colunas'][0]) ? ', '.implode(', ', $arJoinColunas['colunas']) : "*";

	if($stColunas) {
		$sql = "SELECT DISTINCT ptr.ptrid, ptr.resid, ptr.enbid 
					$stColunas
				FROM 
   					emenda.planotrabalho ptr
   					inner join emenda.ptemendadetalheentidade pte on pte.ptrid = ptr.ptrid
   					inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid and vede.edestatus = 'A'
   					inner join emenda.entidadebeneficiada enb on enb.enbid = ptr.enbid and enb.enbstatus = 'A'
   					left join emenda.ptminutaconvenio ptmi on ptmi.ptrid = ptr.ptrid and ptmi.pmcstatus = 'A'
   					left join workflow.documento doc on doc.docid = ptr.docid
                    left join workflow.estadodocumento esd on esd.esdid = doc.esdid
                    left join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
                    left join emenda.ptiniciativaespecificacao ptie on ptie.ptiid = pti.ptiid
                    left join emenda.iniciativaespecificacao ine on ine.iceid = ptie.iceid
                    left join emenda.especificacao esp on esp.espid = ine.espid
                    left join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
   					".( $arJoinColunas ? implode(' ', $arJoinColunas['joins']) : "" )."
				WHERE
   					ptr.ptrstatus = 'A'
   					and ptr.ptrexercicio = '".$_SESSION['exercicio']."'
   					".(($where)? ' and '.implode(" AND ", $where):"")."
   				".($order ? "ORDER BY " . implode(',', $order) : '');
		//dbg($sql,1);	    
		$arDados = $db->carregar( $sql );
	} else {
		die("<script>alert('Colunas não selecionadas');</script>");
	}	
	$arRegistro = array();
	$arExcel	= array();
	if( $arDados ){
		$arDadosArray = array();
		foreach ($arDados as $key => $value) {
			// Carrega coluna de CPF Autorizado
			if( in_array('usucpf', $id) ){
				$sql = "SELECT 
							substr(usucpf,1,3)||'.'||substr(usucpf,4,3)||'.'||substr(usucpf,7,3)||'-'||substr(usucpf,10,2) as usucpf
						FROM emenda.usuarioresponsabilidade 
						WHERE enbid = ".$value['enbid'] . "
						AND rpustatus = 'A'";
				
				$arUserAut = $db->carregarColuna($sql);
				$userAut = ($arUserAut ? implode(',<br/>', $arUserAut) : '');
			}
			//Carrega objeto de convenio do PTA
			if( in_array('obcdsc', $id) ){
				$sql = "SELECT DISTINCT oc.obcdsc
						FROM emenda.ptemendadetalheentidade ped
						    inner join emenda.ptminutaconvenio pt on pt.ptrid = ped.ptrid
						    inner join emenda.objetominutaconvenio om on om.pmcid = pt.pmcid
						    inner join emenda.objetoconvenio oc on oc.obcid = om.obcid and oc.obcstatus = 'A'
						WHERE
							ped.ptrid = {$value['ptrid']}";				
				$arObjeto = $db->carregarColuna($sql);
				$objeto = ($arObjeto ? implode(',<br/>', $arObjeto) : '');
			}			
			// Carrega a coluna de Iniciativa do PTA
			if( in_array('iniid', $id) || in_array('ininome', $id) ) {
				
				$sqlIninome = "SELECT ini.ininome FROM emenda.ptiniciativa pti
							    inner join emenda.iniciativa ini on ini.iniid = pti.iniid
							WHERE pti.ptrid = {$value['ptrid']}";				
				$arIniNome = $db->carregarColuna($sqlIninome);
				$IniNome = ($arIniNome ? implode(',<br/>', $arIniNome) : '');
			}
			// Carrega coluna da iniciativa do Recurso
			if( in_array('iniidemenda', $id) ) {
				$sqlIniEmenda = "SELECT DISTINCT ini.ininome FROM
									emenda.ptemendadetalheentidade pte
								    inner join emenda.v_emendadetalheentidade vede on vede.edeid = pte.edeid
								    inner join emenda.iniciativaemendadetalhe ied on ied.emdid = vede.emdid
								    inner join emenda.iniciativa ini on ini.iniid = ied.iniid and iedstatus = 'A'
								WHERE
									pte.ptrid = {$value['ptrid']}";							
				$arIniID = $db->carregarColuna($sqlIniEmenda);
				$IniID = ($arIniID ? implode(',<br/>', $arIniID) : '');
			}
			
			// Carrega coluna de Tipo de Ensino
			if( in_array('tpeid', $id) && !empty($value['resid']) ){
				$sqlTE = "SELECT DISTINCT
							  	tpedsc
							FROM emenda.tipoensino
							WHERE
								resid = {$value['resid']}
								and tpestatus = 'A'";
				
				$arTipoEnsino = $db->carregarColuna($sqlTE);
				$tipoensino = ($arTipoEnsino ? implode(',<br/>', $arTipoEnsino) : '');
			}			
			// Carrega coluna de Especificação do PTA
			if( in_array('espid', $id) ){				
				$sqlEspf = "SELECT DISTINCT esp.espnome FROM
								emenda.v_ptiniciativaespecificacao ptie
							    inner join emenda.ptiniciativa pti on pti.ptiid = ptie.ptiid and ptie.ptestatus = 'A'
							    inner join emenda.iniciativaespecificacao ice on ice.iceid = ptie.iceid
							    inner join emenda.especificacao esp on esp.espid = ice.espid
							    inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
							WHERE
								pti.ptrid = {$value['ptrid']}";				
				$arEspecif = $db->carregarColuna($sqlEspf);			
				$especificacao = ($arEspecif ? implode(',<br/>', $arEspecif) : '');
			}
			
			if( in_array( 'unisigla', $id ) ){
				$sql = "SELECT DISTINCT uni.unisigla FROM emenda.analise ana 
							inner join emenda.unidades uni on uni.uniid = ana.uniid and uni.unistatus = 'A'
						WHERE ptrid = ".$value['ptrid']." 
							and ana.anatipo = 'T' 
							and ana.analote = (SELECT max(analote) 
													FROM emenda.analise 
												WHERE anatipo = 'T'
													and anastatus = 'A' 
													and ptrid = ana.ptrid)";
				
				$arUnidadeGestora = $db->carregarColuna($sql);			
				$unidadeGestora = ($arUnidadeGestora ? implode(',<br/>', $arUnidadeGestora) : '');
			}
			if( in_array( 'emdliberado', $id ) ){
				$html = '';
				$sql = "select DISTINCT ed.emecod, (CASE WHEN ed.emdliberado = 'N' THEN 'Não' ELSE 'Sim' END) as liberado
			        	from emenda.v_emendadetalheentidade ed 
			        	inner join emenda.ptemendadetalheentidade pt on pt.edeid = ed.edeid and pt.ptrid = ".$value['ptrid'];
				$arEmenda = $db->carregar( $sql );
				$arEmenda = $arEmenda ? $arEmenda : array();								            
				$html = '<table id="table" class="listagem" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
							<tr>
								<th>Código</th>
								<th>Limite Autorizado</th>
							</tr>';
				if( $arEmenda ){
					foreach ($arEmenda as $emenda) {
						$html .= '<tr>
									<td>'.$emenda['emecod'].'</td>
									<td style="text-align: center;">'.$emenda['liberado'].'</td>
								</tr>';
					}
					$html .= '</table>';
					
					$htmlEmenda = $html;
					$ExcelEmenda = 'Sim';
				} else {
					$htmlEmenda = 'Não';					
					$ExcelEmenda = 'Não';
				}
			}
			if( in_array( 'situacaoempenho', $id ) ){
				$html = '';
				$sql = "select DISTINCT exfnumempenhooriginal, case when ex.semid is not null then se.semdesc else 'Não' end as situacao 
						from emenda.execucaofinanceira ex 
							inner join emenda.situacaoempenho se on se.semid = ex.semid and se.semstatus = 'A'
						where ex.ptrid = ".$value['ptrid'];
				$arEmpenho = $db->carregar( $sql );
				$arEmpenho = $arEmpenho ? $arEmpenho : array();								            
				$html = '<table id="table" class="listagem" bgcolor="#f5f5f5" cellSpacing="0" cellPadding="0" align="center">
							<tr>
								<th>Nº Empenho</th>
								<th>Situação</th>
							</tr>';
				if( $arEmpenho ){
					foreach ($arEmpenho as $empenho) {
						$html .= '<tr>
									<td>'.$empenho['exfnumempenhooriginal'].'</td>
									<td>'.$empenho['situacao'].'</td>
								</tr>';
					}
					$html .= '</table>';
					
					$htmlEmpenho = $html;
					$empenhoExcel = 'Sim';
				} else {
					$htmlEmpenho = 'Não';					
					$empenhoExcel = 'Não';
				}
			}
			
			$xx += isset($xx) ? 1 : 0;
			$boRegistro = false;
			
			foreach ($id as $key => $c) {
				if( $c == 'cnpj' ){ 
					$arRegistro[$xx][$c] = formatar_cpf_cnpj( $value[$c] );
					$arExcel[$xx][$c] = formatar_cpf_cnpj( $value[$c] );
				} elseif( $c == 'ptrnumprocessoempenho' ) {
					$arRegistro[$xx][$c] = formataProcesso( $value[$c] );
					$arExcel[$xx][$c] = formataProcesso( $value[$c] );
				} else {
					$arRegistro[$xx][$c] = $value[$c];
					$arExcel[$xx][$c] = $value[$c];
				}
				if( !empty($value[$c]) ){
					$boRegistro = true;
				}
			}
			if( in_array('usucpf', $id) && !empty($userAut) ){
				$boRegistro = true;
				$arRegistro[$xx]['usucpf'] = $userAut;
				$arExcel[$xx]['usucpf'] = $userAut;
				$userAut = '';
			}	
			if( in_array('iniidemenda', $id) && !empty($IniID) ) {
				$boRegistro = true;
				$arRegistro[$xx]['iniidemenda'] = $IniID;
				$arExcel[$xx]['iniidemenda'] = $IniID;
				$IniID = '';
			}
			if( in_array('ininome', $id) && !empty($IniNome) ) {
				$boRegistro = true;
				$arRegistro[$xx]['ininome'] = $IniNome;
				$arExcel[$xx]['ininome'] = $IniNome;
				$IniNome = '';
			}
			if( in_array('espid', $id) && !empty($especificacao) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['espid'] = $especificacao;
				$arExcel[$xx]['espid'] = $especificacao;
				$especificacao = '';
			}	
			if( in_array('tpeid', $id) && !empty($tipoensino) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['tpeid'] = $tipoensino;
				$arExcel[$xx]['tpeid'] = $tipoensino;
				$tipoensino = '';
			}
			if( in_array('unisigla', $id) && !empty($unidadeGestora) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['unisigla'] = $unidadeGestora;
				$arExcel[$xx]['unisigla'] = $unidadeGestora;
				$unidadeGestora = '';
			}
			if( in_array('obcdsc', $id) && !empty($objeto) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['obcdsc'] = $objeto;
				$arExcel[$xx]['obcdsc'] = $objeto;
				$objeto = '';
			}
			if( in_array('situacaoempenho', $id) && !empty($htmlEmpenho) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['situacaoempenho'] = $htmlEmpenho;
				$arExcel[$xx]['situacaoempenho'] = $empenhoExcel;
				$htmlEmpenho = '';
			}
			if( in_array( 'emdliberado', $id ) ){
				$boRegistro = true;	
				$arRegistro[$xx]['emdliberado'] = $htmlEmenda;
				$arExcel[$xx]['emdliberado'] = $ExcelEmenda;
				$htmlEmpenho = '';
			}
			if( !$boRegistro ){
				unset( $arRegistro[$xx] );
				sort($arRegistro);
			}
		}// fim do foreach
	}
	return array("sql" => $arRegistro, 
				 "excel" => $arExcel);
}

function formataProcesso( $numero ){
	if( !empty( $numero ) ) $processo = substr($numero,0,5) . ".".substr($numero,5,6)."/".substr($numero,11,4) . "-".substr($numero,15,2);
	return $processo;
}

function monta_coluna(){
	
	$cols = $_SESSION['emenda']['post']['coluna'] ? $_SESSION['emenda']['post']['coluna'] : array();
	
	$coluna = array("coluna" => array() );
	
	if( is_array($cols) ){
		for($i=0; $i<sizeof($cols); $i++){
			
//		}	
//		foreach ($cols as $val) {
			switch ($cols[$i]) {
				case 'autnome':
					array_push($coluna['coluna'], array(
														"campo" => "(select distinct case when v.emerelator = 'S' then a.autnome||' - Relator Geral' else a.autnome end from emenda.autor a 
												                    	inner join emenda.v_emendadetalheentidade v on v.autid = a.autid and a.autstatus = 'A'
												                        inner join emenda.ptemendadetalheentidade p on p.edeid = v.edeid
												                     where p.ptrid = pte.ptrid limit 1) as autnome",
														"id" => "autnome",
												  		"label" => "Nome do Parlamentar")									
										   				);
				break;
				case 'parsigla':
					array_push($coluna['coluna'], array(
														"campo" => "par.parsigla",
														"id" => "parsigla",
												  		"label" => "Partido")
										   				);
				break;
				case 'estufator':
					array_push($coluna['coluna'], array(
														"campo" => "aut.estuf",
														"id" => "estuf",
												  		"label" => "UF do Autor")										
										   				);
				break;
				case 'estuf':
					array_push($coluna['coluna'], array(
														"campo" => "enb.estuf as estufentidade",
														"id" => "estufentidade",
												  		"label" => "UF da Entidade")										
										   				);
				break;
				case 'responsavel':
					array_push($coluna['coluna'], array(
														"campo" => "(CASE WHEN res.resid is not null THEN res.resdsc
																	        ELSE 'Não Informado' 
																	    END) as responsavel",
														"id" => "responsavel",
												  		"label" => "Responsável")										
										   				);
				break;
				case 'emecod':
					array_push($coluna['coluna'], array(
														"campo" => "eme.emecod",
														"id" => "emecod",
												  		"label" => "Emenda")										
										   				);
				break;
				case 'pmcnumconveniosiafi':
					array_push($coluna['coluna'], array(
														"campo" => "ptmi.pmcnumconveniosiafi",
														"id" => "pmcnumconveniosiafi",
												  		"label" => "Nº do Convênio SIAFI")										
										   				);
				break;				
				case 'fupfuncionalprogramatica':
					array_push($coluna['coluna'], array(
														"campo" => "vfun.fupfuncionalprogramatica",
														"id" => "fupfuncionalprogramatica",
												  		"label" => "Funcional Programática")										
										   				);
				break;
				case 'fupdsc':
					array_push($coluna['coluna'], array(
														"campo" => "vfun.fupdsc",
														"id" => "fupdsc",
												  		"label" => "SubTitulo")
										   				);
				break;
				case 'gndcod':
					array_push($coluna['coluna'], array(
														"campo" => "emd.gndcod",
														"id" => "gndcod",
												  		"label" => "GND")
										   				);
				break;
				case 'foncod':
					array_push($coluna['coluna'], array(
														"campo" => "emd.foncod",
														"id" => "foncod",
												  		"label" => "Fonte")
										   				);
				break;
				case 'mapcod':
					array_push($coluna['coluna'], array(
														"campo" => "emd.mapcod",
														"id" => "mapcod",
												  		"label" => "Mod")
										   				);
				break;
				case 'valoremenda':
					array_push($coluna['coluna'], array(
														"campo" => "emd.emdvalor as valoremenda",
														"id" => "valoremenda",
												  		"label" => "Valor da Emenda")
										   				);
				break;
				case 'entidade':
					array_push($coluna['coluna'], array(
														"campo" => "enb.enbnome as entidade",
														"id" => "entidade",
												  		"label" => "Entidade")
										   				);
				break;
				case 'enbcnpj':
					array_push($coluna['coluna'], array(
														"campo" => "ent.enbcnpj",
														"id" => "enbcnpj",
												  		"label" => "CNPJ")
										   				);
				break;
				case 'valorentidade':
					array_push($coluna['coluna'], array(
														"campo" => "ede.edevalor as valorentidade",
														"id" => "valorentidade",
												  		"label" => "Valor da Entidade")
										   				);
				break;

				case 'esddsc':
					array_push($coluna['coluna'], array(
														"campo" => "esd.esddsc",
														"id" => "esddsc",
												  		"label" => "Situação")
										   				);
										   				
				break;
				case 'unicod':
					array_push($coluna['coluna'], array(
														"campo" => "vfun.unicod",
														"id" => "unicod",
												  		"label" => "Unidade")
										   				);
				break;
				case 'acacod':
					array_push($coluna['coluna'], array(
														"campo" => "(vfun.acacod ||' - '|| vfun.acadsc) as acaoorcamentaria",
														"id" => "acaoorcamentaria",
												  		"label" => "Ação Orçamentária")
										   				);
				break;
				case 'tpanome':
					array_push($coluna['coluna'], array(
														"campo" => "tpa.tpanome",
														"id" => "tpanome",
												  		"label" => "Tipo de Autor")
										   				);
				break;
				case 'tpaid':
					array_push($coluna['coluna'], array(
														"campo" => "tpa.tpanome",
														"id" => "tpanome",
												  		"label" => "Tipo de Autor")
										   				);
				break;
				case 'usucpf':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "usucpf",
												  		"label" => "CPF Autorizado")
										   				);
				break;
				case 'iniidemenda':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "iniidemenda",
												  		"label" => "Iniciativa do Recurso")
										   				);
				break;
				case 'emdliberado':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "emdliberado",
												  		"label" => "Limite Autorizado")
										   				);
				break;
				case 'ininome':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "ininome",
												  		"label" => "Iniciativa do PTA")
										   				);
				break;
				case 'ptrcod':
					array_push($coluna['coluna'], array(
														"campo" => "ptr.ptrcod",
														"id" => "ptrcod",
												  		"label" => "PTA")
										   				);
				break;
				case 'espid':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "espid",
												  		"label" => "Especificação do PTA")
										   				);
				break;
				case 'tpeid':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "tpeid",
												  		"label" => "Tipo de Ensino")
										   				);
				break;
				case 'ptivalortotal':
					array_push($coluna['coluna'], array(
														"campo" => "vpti.ptivalortotal",
														"id" => "ptivalortotal",
												  		"label" => "Valor do PTA")
										   				);
				break;
				case 'ptrnumconvenio':
					array_push($coluna['coluna'], array(
														"campo" => "ptr.ptrnumconvenio",
														"id" => "ptrnumconvenio",
												  		"label" => "Nº do convênio FNDE")
										   				);
				break;
				case 'exfvalor':
					array_push($coluna['coluna'], array(
														"campo" => "ef.exfvalor",
														"id" => "exfvalor",
												  		"label" => "Valor Empenhado")
										   				);
				break;
				case 'vigdatainicio':
					array_push($coluna['coluna'], array(
														"campo" => "to_char(ptv.vigdatainicio, 'DD/MM/YYYY') as vigdatainicio",
														"id" => "vigdatainicio",
												  		"label" => "Data inicio de vigência")
										   				);
				break;
				case 'vigdatafim':
					array_push($coluna['coluna'], array(
														"campo" => "to_char(ptv.vigdatafim, 'DD/MM/YYYY') as vigdatafim",
														"id" => "vigdatafim",
												  		"label" => "Data fim de vigência")
										   				);
				break;
				case 'unisigla':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "unisigla",
												  		"label" => "Área Gestora")
										   				);
				break;
				case 'ptrnumprocessoempenho':
					array_push($coluna['coluna'], array(
														"campo" => "ptr.ptrnumprocessoempenho",
														"id" => "ptrnumprocessoempenho",
												  		"label" => "Nº do Processo Empenho")
										   				);
				break;
				case 'objeto':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "obcdsc",
												  		"label" => "Objeto da Emenda")
										   				);
				break;
				case 'muncod':
					array_push($coluna['coluna'], array(
														"campo" => "(select mun.mundescricao || ' / ' || mun.estuf from territorios.municipio mun WHERE mun.muncod = enb.muncod) as mundescricao",
														"id" => "mundescricao",
												  		"label" => "Município")
										   				);
				break;
				case 'orbvalorparcela':
					array_push($coluna['coluna'], array(
														"campo" => "(SELECT sum(orbvalorpagamento) FROM emenda.ordembancaria  WHERE exfid = ef.exfid) as valorpago",
														"id" => "valorpago",
												  		"label" => "Valor Pago")
										   				);
				break;
				case 'datareferencia':
					array_push($coluna['coluna'], array(
														"campo" => "to_char(pt.pubdatapublicacao, 'DD/MM/YYYY') as datareferencia",
														"id" => "datareferencia",
												  		"label" => "Data de Referência")
										   				);
				break;
				case 'emeano':
					array_push($coluna['coluna'], array(
														"campo" => "eme.emeano",
														"id" => "emeano",
												  		"label" => "Ano Orçamento")
										   				);
				break;
				case 'situacaoempenho':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "situacaoempenho",
												  		"label" => "Empenho")
										   				);
					/*(select case when ex.semid is not null then se.semdesc else 'Não' end from emenda.execucaofinanceira ex 
															        	inner join emenda.situacaoempenho se on se.semid = ex.semid and se.semstatus = 'A'
															            where ex.ptrid = ptr.ptrid order by ex.exfnumempenhooriginal is not null limit 1) as situacaoempenho*/
				break;
				case 'anadataconclusao':
					array_push($coluna['coluna'], array(
														"campo" => "(select to_char(max(an.anadataconclusao), 'DD/MM/YYYY HH24:MI:SS') 
																	 from emenda.analise an where an.analote = (SELECT max(analote) 
													    					from emenda.analise 
													                        where anatipo = 'T' 
													                        	and anastatus = 'A'
													                        	and ptrid = an.ptrid and anadataconclusao is not null) 
													                  and an.ptrid = ptr.ptrid and an.anastatus = 'A' and an.anatipo = 'T') as anadataconclusao",
														"id" => "anadataconclusao",
												  		"label" => "Data da Análise Técnica")
										   				);
				break;
				case 'anasituacao':
					array_push($coluna['coluna'], array(
														"campo" => "(select CASE WHEN anasituacaoparecer = 'F' THEN 'Aprovado'
																			 WHEN anasituacaoparecer = 'D' THEN 'Indeferido'
																			 WHEN anasituacaoparecer = 'E' THEN 'Em diligência'
																			 ELSE 'Aguardando' END as analise 
																	 from emenda.analise an where an.analote = (SELECT max(analote) 
													    					from emenda.analise 
													                        where anatipo = 'T' 
													                        	and anastatus = 'A'
													                        	and ptrid = an.ptrid and anadataconclusao is not null) 
													                  and an.ptrid = ptr.ptrid and an.anastatus = 'A' and an.anatipo = 'T'
                                                                      and an.anadataconclusao = (SELECT max(anadataconclusao) 
													    					from emenda.analise 
													                        where anatipo = 'T' 
													                        	and anastatus = 'A'
													                        	and ptrid = an.ptrid and anadataconclusao is not null) limit 1) as anasituacao",
														"id" => "anasituacao",
												  		"label" => "Situação Análise Técnica")
										   				);
				break;
			}		
		}
	}
	return $coluna['coluna'];			  	
}
?>