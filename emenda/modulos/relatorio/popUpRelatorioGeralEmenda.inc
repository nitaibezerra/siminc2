<?php
set_time_limit(0);
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
sort($arSql);

//gera relatorio XLS
if($_POST['exporta'] == "true"){
	global $db;
	ob_clean();
	header('content-type: text/html; charset=ISO-8859-1');
	
	$db->sql_to_excel($arSql, 'relEmendasPTA', $cabecalho, $formato);
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
$db->monta_lista_array( $arSql, $cabecalho, 100000, 1, 'N', 'center', '');

montaBotaoForm();

echo '<script>
		$(\'loader-container\').hide();
	  </script>';

function montaJoinColunas( $arColunas, $arFiltros ){
	$arJoin = array();
	$arCols = array();
	//ver($arColunas, $arFiltros);
	//foreach( $arColunas as $stColuna ){
		if( in_array( 'responsavel', $arColunas ) || in_array( 'responsavel', $arFiltros ) ){
			(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
			$join = "left join emenda.responsavel res on res.resid = eme.resid";
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
		}
		if( in_array( 'tpeid', $arColunas ) || in_array( 'tpeid', $arFiltros ) ){
			(in_array( 'res.resid', $arCols ) ? '' : $arCols[] = 'res.resid');
			$join = "left join emenda.responsavel res on res.resid = eme.resid";
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			$join = "left join emenda.tipoensino tpe on tpe.resid = res.resid and tpe.tpestatus = 'A'";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			
		} 
		if( in_array( 'entidade', $arColunas) || in_array( 'valorentidade', $arColunas) || in_array( 'usucpf', $arColunas) || in_array( 'entidade', $arFiltros) || 
				in_array( 'estufentidade', $arFiltros) || in_array( 'mundescricao', $arColunas) ){
			( in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			$join = "left join emenda.entidadebeneficiada ent on (ede.enbid = ent.enbid)
					 left join territorios.municipio mun on mun.muncod = ent.muncod";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			
		}
		$arJoinPTA = array();
		if ( in_array( 'ptrid', $arColunas) || in_array( 'pmcnumconveniosiafi', $arColunas) || in_array( 'ptrnumconvenio', $arColunas) || 
					in_array( 'esddsc', $arColunas) || in_array( 'ptivalortotal', $arColunas) || in_array( 'exfvalor', $arColunas) || 
					in_array( 'pmcnumconveniosiafi', $arFiltros) || in_array( 'esddsc', $arFiltros) || in_array( 'ininome', $arColunas) ||
					in_array( 'espid', $arColunas) || in_array( 'espid', $arFiltros) || in_array( 'ptrnumconvenio', $arFiltros) ||
					in_array( 'vigdatainicio', $arColunas) || in_array( 'vigdatafim', $arColunas ) || in_array( 'iniid', $arFiltros ) || 
					in_array( 'unisigla', $arColunas ) || in_array( 'ptrnumprocessoempenho', $arFiltros ) || in_array( 'ptrnumprocessoempenho', $arColunas ) || 
					in_array( 'ptridentificacaofederal', $arColunas ) || in_array( 'ptrobjetivofederal', $arFiltros ) || in_array( 'ptrjustificativafederal', $arColunas ) 
					|| in_array( 'pedvalor', $arFiltros ) ){
			
			(in_array( "(ptr.ptrcod ||'/'|| ptr.ptrexercicio) as ptrcod", $arCols ) ? '' : $arCols[] = "(ptr.ptrcod ||'/'|| ptr.ptrexercicio) as ptrcod");
			(in_array( 'ent.enbid', $arCols ) ? '' : $arCols[] = 'ent.enbid');
			(in_array( 'eme.emeid', $arCols ) ? '' : $arCols[] = 'eme.emeid');
			(in_array( 'ptr.ptrid', $arCols ) ? '' : $arCols[] = 'ptr.ptrid');
			(in_array( 'ptr.ptridentificacaofederal', $arCols ) ? '' : $arCols[] = 'ptr.ptridentificacaofederal');
			(in_array( 'ptr.ptrobjetivofederal', $arCols ) ? '' : $arCols[] = 'ptr.ptrobjetivofederal');
			(in_array( 'ptr.ptrjustificativafederal', $arCols ) ? '' : $arCols[] = 'ptr.ptrjustificativafederal');
			
			if( in_array( 'ininome', $arColunas) || in_array( 'iniid', $arFiltros ) ){
				$join = "inner join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.iniciativa ini on ini.iniid = pti.iniid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);				
			}
			
			if( in_array( 'espid', $arColunas) || in_array( 'espid', $arFiltros) ){				
				$join = "inner join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.ptiniciativaespecificacao ptie on ptie.ptiid = pti.ptiid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.iniciativaespecificacao ine on ine.iceid = ptie.iceid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
				
				$join = "inner join emenda.especificacao esp on esp.espid = ine.espid
						inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}
			
			if( in_array( 'pmcnumconveniosiafi', $arColunas) || in_array( 'pmcnumconveniosiafi', $arFiltros) || in_array( 'datareferencia', $arColunas ) ){
				$join = "left join emenda.ptminutaconvenio ptmi on ptmi.ptrid = ptr.ptrid and ptmi.pmcstatus = 'A'
						 left join emenda.ptpublicacao pt on pt.pmcid = ptmi.pmcid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}
			if( in_array( 'exfvalor', $arColunas) || in_array( 'valorpago', $arColunas) ){
				$join = "left join emenda.execucaofinanceira ef on ptr.ptrid = ef.ptrid and pede.pedid = ef.pedid and ef.exfstatus = 'A'";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}
			if( in_array( 'esddsc', $arColunas) || in_array( 'esddsc', $arFiltros) ){
				$join = "left join workflow.documento doc on ptr.docid = doc.docid 
						  left join workflow.estadodocumento esd on esd.esdid = doc.esdid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}
			if( in_array( 'ptivalortotal', $arColunas) ){
				$join = "left join
							(SELECT ptrid, sum(ptivalortotal) as ptivalortotal
							 FROM emenda.v_ptiniciativa 
							 GROUP BY ptrid) pti1 ON pti1.ptrid = ptr.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}
			if( in_array( 'vigdatainicio', $arColunas) || in_array( 'vigdatafim', $arColunas) ){
				$join = "left join emenda.ptvigencia ptv ON ptv.ptrid = ptr.ptrid";
				(in_array( $join, $arJoinPTA ) ? '' : $arJoinPTA[] = $join);
			}
			
			$join = "left join emenda.ptemendadetalheentidade pede
					  	inner join emenda.planotrabalho ptr on ptr.ptrid = pede.ptrid and ptr.ptrstatus = 'A' and ptr.ptrexercicio = ".$_SESSION['exercicio']."
					  	".implode(' ', $arJoinPTA)."
					  on ede.edeid = pede.edeid";
			
			(in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			$join = "left join emenda.entidadebeneficiada ent on (ede.enbid = ent.enbid)
					 left join territorios.municipio mun on mun.muncod = ent.muncod";
			( in_array( $join, $arJoin ) ? '' : $arJoin[] = $join);
			
			//ver($arColunas,$arJoin,d);
		}
		if( in_array( 'obcdsc', $arColunas) ){
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
			
		}
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
		$where[] = " aut.autid " . (( $autid_campo_excludente == null || $autid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $autid ) . "') ";		
	}
	if( $responsavel[0] && ( $responsavel_campo_flag || $responsavel_campo_flag == '1' )){
		$where[] = " res.resid " . (( $responsavel_campo_excludente == null || $responsavel_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $responsavel ) . "') ";
		$arFiltros[] = 'responsavel';		
	}
	if( $parid[0] && ( $parid_campo_flag || $parid_campo_flag == '1' )){
		$where[] = " par.parid " . (( $parid_campo_excludente == null || $parid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $parid ) . "') ";		
	}
	if( $tpaid[0] && ( $tpaid_campo_flag || $tpaid_campo_flag == '1' )){
		$where[] = " tpa.tpaid" . (( $tpaid_campo_excludente == null || $tpaid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpaid ) . "') ";		
	}
	if( $estuf[0] && ( $estuf_campo_flag || $estuf_campo_flag == '1' )){
		$where[] = " aut.estuf " . (( $estuf_campo_excludente == null || $estuf_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $estuf ) . "') ";		
	}
	if( $enbid[0] && ( $enbid_campo_flag || $enbid_campo_flag == '1' )){
		$where[] = " ent.enbid " . (( $entid_campo_excludente == null || $entid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $enbid ) . "') ";
		$arFiltros[] = 'entidade';		
	}
	if( $unicod[0] && ( $unicod_campo_flag || $unicod_campo_flag == '1' )){
		$where[] = " vfun.unicod " . (( $unicod_campo_excludente == null || $unicod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $unicod ) . "') ";		
	}
	if( $gndcod[0] && ( $gndcod_campo_flag || $gndcod_campo_flag == '1' )){
		$where[] = " emd.gndcod " . (( $gndcod_campo_excludente == null || $gndcod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $gndcod ) . "') ";		
	}
	if( $acacod[0] && ( $acacod_campo_flag || $acacod_campo_flag == '1' )){
		$where[] = " vfun.acacod " . (( $acacod_campo_excludente == null || $acacod_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $acacod ) . "') ";		
	}
	if( $espid[0] && ( $espid_campo_flag || $espid_campo_flag == '1' )){
		$where[] = " esp.espid " . (( $espid_campo_excludente == null || $espid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $espid ) . "') ";
		$whereEspid[]= " esp.espid " . (( $espid_campo_excludente == null || $espid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $espid ) . "') ";
		$arFiltros[] = 'espid';
	}
	if( $iniid[0] && ( $iniid_campo_flag || $iniid_campo_flag == '1' )){
		$where[] = " ini.iniid " . (( $iniid_campo_excludente == null || $iniid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $iniid ) . "') ";
		$whereIniid[]= " ini.iniid " . (( $iniid_campo_excludente == null || $iniid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $iniid ) . "') ";
		$arFiltros[] = 'iniid';
	}
		
	if( $tpeid[0] && ( $tpeid_campo_flag || $tpeid_campo_flag == '1' )){
		$where[] = " tpe.tpeid " . (( $tpeid_campo_excludente == null || $tpeid_campo_excludente == '0') ? ' IN ' : ' NOT IN ') . " ('" . implode( "','", $tpeid ) . "') ";
		$arFiltros[] = 'tpeid';		
	}
	
	
	if($emecod){
		$where[] = "eme.emecod = '".$emecod."'";
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
	
	if($pmcnumconveniosiafi){
		$where[] = "ptmi.pmcnumconveniosiafi = '".$pmcnumconveniosiafi."'";
		$arFiltros[] = 'pmcnumconveniosiafi';
	}	
	
	$where[] = "eme.emeano = ".$_SESSION['exercicio'];
	
	$order = array();
	if($agrupador){
		foreach ($agrupador as $val){
			switch ($val) {
				case 'emecod':
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
					$order[] = 'autnome';
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
					$order[] = 'unicod';
				break;
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
	
	if(!empty($emenda_relator)){
		if($emenda_relator == 'S'){
			$where[] = " eme.emerelator = 'S' ";
		}else{
			$where[] = " eme.emerelator = 'N' ";
		}
	}
	
	if(!empty($comresponsavel)){
		if($comresponsavel == 'S'){
			$where[] = 'res.resid is not null';
		}else{
			$where[] = 'res.resid is null';
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
	if(  in_array( 'exfvalor', $id ) ){
		$where[] = "ef.exfvalor is not null and ef.semid = 4";
	}
	
	//$where[] = "ptr.ptrcod = 6122 ";
	$arColunas = array_merge( $cols, $id );
	$arJoinColunas = montaJoinColunas( $arColunas, $arFiltros );
	$arJoinColunas['colunas'] = array_merge( $arJoinColunas['colunas'], $cols );
	$stColunas = is_array($arJoinColunas) ? implode(',', $arJoinColunas['colunas']) : "*";
	
	if($stColunas) {
		/*$sql = "SELECT DISTINCT 
					$stColunas
				FROM emenda.emenda eme 
				inner join emenda.emendadetalhe emd ON eme.emeid = emd.emeid
				inner join emenda.autor aut on eme.autid = aut.autid
				inner join emenda.tipoautor tpa on tpa.tpaid = aut.tpaid
				inner join emenda.partido par ON aut.parid = par.parid
				inner join emenda.v_funcionalprogramatica vfun on vfun.acaid = eme.acaid
				left join emenda.emendadetalheentidade ede on emd.emdid = ede.emdid
				left join emenda.responsavel res on res.resid = eme.resid 
                left join emenda.tipoensino tpe on tpe.resid = res.resid and tpe.tpestatus = 'A' 
                left join emenda.entidadebeneficiada ent on ede.enbid = ent.enbid
				left join territorios.municipio mun on mun.muncod = ent.muncod 
                left join emenda.ptemendadetalheentidade pede on ede.edeid = pede.edeid
                left join emenda.planotrabalho ptr on ptr.ptrid = pede.ptrid and ptr.ptrstatus = 'A' and ptr.ptrexercicio = 2011
                left join emenda.ptiniciativa pti on pti.ptrid = pede.ptrid 
                left join emenda.iniciativa ini on ini.iniid = pti.iniid 
                left join emenda.ptiniciativaespecificacao ptie on ptie.ptiid = pti.ptiid 
                left join emenda.iniciativaespecificacao ine on ine.iceid = ptie.iceid 
                left join emenda.especificacao esp on esp.espid = ine.espid 
                left join emenda.ptminutaconvenio ptmi on ptmi.ptrid = ptr.ptrid and ptmi.pmcstatus = 'A'
                left join emenda.ptpublicacao pt on pt.pmcid = ptmi.pmcid 
                left join emenda.execucaofinanceira ef on ptr.ptrid = ef.ptrid and pede.pedid = ef.pedid and ef.exfstatus = 'A' 
                left join workflow.documento doc on ptr.docid = doc.docid 
                left join workflow.estadodocumento esd on esd.esdid = doc.esdid 
                left join
                    (SELECT ptrid, sum(ptivalortotal) as ptivalortotal
                     FROM emenda.v_ptiniciativa 
                     GROUP BY ptrid) pti1 ON pti1.ptrid = ptr.ptrid 
                left join emenda.ptvigencia ptv ON ptv.ptrid = ptr.ptrid 
                left join emenda.iniciativadetalheentidade ide
			    	inner join emenda.iniciativa ini1 on ini1.iniid = ide.iniid
			    on ide.edeid = ede.edeid 
			    
				".(($where)?" WHERE ".implode(" AND ", $where):"")."
				".($order ? "ORDER BY " . implode(',', $order) : '');*/
		$sql = "SELECT DISTINCT 
					$stColunas
				FROM emenda.emenda eme 
				inner join emenda.emendadetalhe emd ON eme.emeid = emd.emeid
				inner join emenda.autor aut on eme.autid = aut.autid
				inner join emenda.tipoautor tpa on tpa.tpaid = aut.tpaid
				inner join emenda.partido par ON aut.parid = par.parid
				inner join emenda.v_funcionalprogramatica vfun on vfun.acaid = eme.acaid
				inner join public.unidade un on un.unicod = vfun.unicod
				left join emenda.emendadetalheentidade ede on emd.emdid = ede.emdid
				".( $arJoinColunas ? implode(' ', $arJoinColunas['joins']) : "" )."
			    
				".(($where)?" WHERE ".implode(" AND ", $where):"")."
				".($order ? "ORDER BY " . implode(',', $order) : '');
				 //".( $arJoinColunas ? implode(' ', $arJoinColunas['joins']) : "" )."
		//ver( $sql );	
		$arDados = $db->carregar( $sql );
	} else {
		die("<script>alert('Colunas não selecionadas');</script>");
	}
	//ver( $sql );
	//$arDados = array();
	
	$arRegistro = array();
	if( $arDados ){
		$arDadosArray = array();
		foreach ($arDados as $key => $value) {
			// Carrega coluna de CPF Autorizado
			if( in_array('usucpf', $id) && !empty($value['enbid']) ){
				$sql = "SELECT 
							substr(usucpf,1,3)||'.'||substr(usucpf,4,3)||'.'||substr(usucpf,7,3)||'-'||substr(usucpf,10,2) as usucpf
						FROM emenda.usuarioresponsabilidade 
						WHERE enbid = ".$value['enbid'] . "
						AND rpustatus = 'A'";
				
				$arUserAut = $db->carregarColuna($sql);
				$userAut = ($arUserAut ? implode(',<br/>', $arUserAut) : '');
			}
			if( in_array('obcdsc', $id) && !empty($value['emeid']) && !empty($value['enbid']) ){
				$sql = "SELECT oc.obcdsc FROM emenda.emendadetalhe emd 
					        inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid
					            and ede.edestatus = 'A'
					        inner join emenda.ptemendadetalheentidade ped on ede.edeid = ped.edeid
					        inner join emenda.ptminutaconvenio pt on pt.ptrid = ped.ptrid
					        inner join emenda.objetominutaconvenio om on om.pmcid = pt.pmcid
						    inner join emenda.objetoconvenio oc on oc.obcid = om.obcid and oc.obcstatus = 'A'
					    WHERE emd.emeid = {$value['emeid']}
					        and ede.enbid = {$value['enbid']}";				
				$arObjeto = $db->carregarColuna($sql);
				$objeto = ($arObjeto ? implode(',<br/>', $arObjeto) : '');
			}
			// Carrega coluna da iniciativa do Recurso
			if( (in_array('iniid', $id) || in_array('ininome', $id)) && !empty($value['emeid']) && !empty($value['enbid']) ) {
				
				$sqlIniid = " Select DISTINCT
								  ini.ininome
							FROM emenda.emendadetalhe emd 
							inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid
								and ede.edestatus = 'A'
							inner join emenda.iniciativadetalheentidade ide on ede.edeid = ide.edeid
								and ide.idestatus = 'A'
							inner join emenda.iniciativa ini on ini.iniid = ide.iniid
						  	WHERE emd.emeid = {$value['emeid']}
							AND ede.enbid = {$value['enbid']}".((!empty( $whereIniid ))?" and ".implode(" AND ", $whereIniid):"");				
				
				$arIniID = $db->carregarColuna($sqlIniid);
				$IniID = ($arIniID ? implode(',<br/>', $arIniID) : '');
			}
			// Carrega a coluna de Iniciativa do PTA
			if( in_array('ininome', $id) && !empty($value['emeid']) && !empty($value['enbid'])) {
				$sqlEmeid = "SELECT DISTINCT
								  ini.ininome
							FROM emenda.emendadetalhe emd 
							inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid
								and ede.edestatus = 'A'
							inner join emenda.ptemendadetalheentidade ped on ede.edeid = ped.edeid
							inner join emenda.planotrabalho ptr on ptr.ptrid = ped.ptrid
							    and ptr.ptrstatus = 'A'
							inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
							inner join emenda.iniciativa ini on ini.iniid = pti.iniid
							WHERE emd.emeid = {$value['emeid']}
							AND ede.enbid = {$value['enbid']}
							and ptr.ptrexercicio = ".$_SESSION['exercicio'].
							((!empty( $whereIniid ))?" and ".implode(" AND ", $whereIniid):"");
							
				$arIniNome = $db->carregarColuna($sqlEmeid);
				$IniNome = ($arIniNome ? implode(',<br/>', $arIniNome) : '');
			}
			
			// Carrega coluna de Tipo de Ensino
			if( in_array('tpeid', $id) && !empty($value['resid']) ){
				$sqlTE = "SELECT
							  	tpedsc
							FROM emenda.tipoensino
							WHERE
								resid = {$value['resid']}
								and tpestatus = 'A'";
				
				$arTipoEnsino = $db->carregarColuna($sqlTE);
				$tipoensino = ($arTipoEnsino ? implode(',<br/>', $arTipoEnsino) : '');
			}
			
			// Carrega coluna de Especificação do PTA
			if( in_array('espid', $id) && !empty($value['emeid']) && !empty($value['enbid']) ){
				
				$sqlEspf = "SELECT DISTINCT
								  esp.espnome
							FROM emenda.emendadetalhe emd 
							inner join emenda.emendadetalheentidade ede on ede.emdid = emd.emdid
								and ede.edestatus = 'A'
							inner join emenda.ptemendadetalheentidade ped on ede.edeid = ped.edeid
							inner join emenda.planotrabalho ptr on ptr.ptrid = ped.ptrid
							    and ptr.ptrstatus = 'A'
							inner join emenda.ptiniciativa pti on pti.ptrid = ptr.ptrid
							inner join emenda.ptiniciativaespecificacao pte on pte.ptiid = pti.ptiid
							    and pte.ptestatus = 'A'
							inner join emenda.iniciativaespecificacao ice on ice.iceid = pte.iceid
							inner join emenda.especificacao esp on esp.espid = ice.espid
							inner join emenda.especificacao_programacaoexercicio epe on epe.espid = esp.espid and epe.prsano = '".$_SESSION['exercicio']."'
				            WHERE emd.emeid = {$value['emeid']}
							AND ede.enbid = {$value['enbid']}
							and ptr.ptrexercicio = ".$_SESSION['exercicio'].((!empty($whereEspid))?" and ".implode(" and ", $whereEspid):"");
				
				$arEspecif = $db->carregarColuna($sqlEspf);			
				$especificacao = ($arEspecif ? implode(',<br/>', $arEspecif) : '');
			}
			
			if( in_array( 'unisigla', $id ) && !empty($value['ptrid']) ){
				$sql = "SELECT uni.unisigla FROM emenda.analise ana 
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
			
			$xx += isset($xx) ? 1 : 0;
			$boRegistro = false;
			foreach ($id as $key => $c) {
				if( $c == 'enbcnpj' ){ 
					$arRegistro[$xx][$c] = formatar_cpf_cnpj( $value[$c] );
				} else {
					$arRegistro[$xx][$c] = $value[$c];
				}
				if( !empty($value[$c]) ){
					$boRegistro = true;
				}
			}
			if( in_array('usucpf', $id) && !empty($userAut) ){
				$boRegistro = true;
				$arRegistro[$xx]['usucpf'] = $userAut;
				$userAut = '';
			}	
			if( in_array('iniid', $id) && !empty($IniID) ) {
				$boRegistro = true;
				$arRegistro[$xx]['iniid'] = $IniID;
				$IniID = '';
			}
			if( in_array('ininome', $id) && !empty($IniNome) ) {
				$boRegistro = true;
				$arRegistro[$xx]['ininome'] = $IniNome;
				$IniNome = '';
			}
			if( in_array('espid', $id) && !empty($especificacao) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['espid'] = $especificacao;
				$especificacao = '';
			}	
			if( in_array('tpeid', $id) && !empty($tipoensino) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['tpeid'] = $tipoensino;
				$tipoensino = '';
			}
			if( in_array('unisigla', $id) && !empty($unidadeGestora) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['unisigla'] = $unidadeGestora;
				$unidadeGestora = '';
			}
			if( in_array('obcdsc', $id) && !empty($objeto) ) {
				$boRegistro = true;	
				$arRegistro[$xx]['obcdsc'] = $objeto;
				$objeto = '';
			}
			if( !$boRegistro ){
				unset( $arRegistro[$xx] );
				sort($arRegistro);
			}
		}// fim do foreach
	}
	return $arRegistro;
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
														"campo" => "case when eme.emerelator = 'S' then aut.autnome||' - Relator Geral' else aut.autnome end as autnome",
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
														/*"campo" => "(SELECT e.estuf FROM entidade.endereco e
																		INNER JOIN territorios.municipio mun ON (mun.muncod = e.muncod)
																	WHERE e.entid = ent.entid and endstatus = 'A' limit 1) as estufentidade",*/
														"campo" => "ent.estuf as estufentidade",
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
												  		"label" => "Convênio")										
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
														"campo" => "ent.enbnome as entidade",
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
														"campo" => "vfun.unicod||' - '||un.unidsc as unicod",
														"id" => "unicod",
												  		"label" => "Unidade Orçamentária")
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
				case 'iniid':
					array_push($coluna['coluna'], array(
														"campo" => "",
														"id" => "iniid",
												  		"label" => "Iniciativa do Recurso")
										   				);
				break;
				case 'emdliberado':
					array_push($coluna['coluna'], array(
														"campo" => "(CASE WHEN ede.emdliberado = 'N' THEN 'Não'
																	        ELSE 'Sim' 
																	    END) as emdliberado",
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
				case 'ptrid':
					array_push($coluna['coluna'], array(
														"campo" => "",
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
														"campo" => "pti1.ptivalortotal",
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
														"campo" => "mun.mundescricao",
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
				 case 'ptridentificacaofederal':
					array_push($coluna['coluna'], array(
														"campo" => "ptr.ptridentificacaofederal",
														"id" => "ptridentificacaofederal",
												  		"label" => "Identificação (Título / Objeto da despesa)")
										   				);
				break;
				case 'ptrobjetivofederal':
					array_push($coluna['coluna'], array(
														"campo" => "ptr.ptrobjetivofederal",
														"id" => "ptrobjetivofederal",
												  		"label" => "Objetivo PTA")
										   				);
				break;
				case 'ptrjustificativafederal':
					array_push($coluna['coluna'], array(
														"campo" => "ptr.ptrjustificativafederal",
														"id" => "ptrjustificativafederal",
												  		"label" => "Justificativa (Motivação / Clientela / Cronograma físico)")
										   				);
				break;
			}		
		}
	}
	
	return $coluna['coluna'];			  	
}
?>