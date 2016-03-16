<?php
set_time_limit(0);
ini_set("memory_limit", "12000M");

global $db;

$sql = "SELECT 
			d.dopid, d.mdoid, d.dopvalortermo, d.prpid --, d.tpdcod
		FROM 
			par.vm_documentopar_ativos d 
		INNER JOIN par.modelosdocumentos md ON md.mdoid = d.mdoid 
		WHERE 
			d.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado2 )
			AND d.mdoid = 41
			AND dopdatafimvigencia = '07/2013'
		ORDER BY
			d.mdoid, d.dopid
		LIMIT 1";
$dados = $db->carregar($sql);
//ver($dados, d);
$sqlPerdido = "";
$sqlGerado = "";

if( $dados[0] ){
	foreach( $dados as $dado ){
	
			
		$sql = "SELECT
			  prpid, dopstatus, dopdiasvigencia, dopdatainicio, dopdatafim,
			  mdoid as modelo, dopdatainclusao, usucpfinclusao, dopdataalteracao, usucpfalteracao, dopjustificativa,
			  dopdatavalidacaofnde, dopusucpfvalidacaofnde, dopdatavalidacaogestor, dopusucpfvalidacaogestor,
			  dopusucpfstatus, dopdatastatus, dopdatapublicacao, doppaginadou, dopnumportaria, proid,
			  dopreformulacao, dopidpai, dopdatainiciovigencia
			FROM 
			  par.documentopar WHERE dopid = {$dado['dopid']} and dopstatus = 'A'";
			  
		$arrDadosDoc = $db->pegaLinha( $sql );
		$arrDadosDoc = $arrDadosDoc ? $arrDadosDoc : array();
		
		$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = 41";
		$arrModelo = $db->pegaLinha($sql);
		
		$mdoconteudo = $arrModelo['mdoconteudo'];
		$tpdcod = 102; //$arrModelo['tpdcod'];
		
		$sql = "SELECT sbdid as chk FROM par.termocomposicao WHERE dopid = ".$dado['dopid'];
		$subacoes = $db->carregarColuna($sql);
		
		$post = array('mdoid' => 41, 'tpdcod' => $tpdcod, 'dopid' => $dado['dopid'], 'chk' => $subacoes);
		
		$_SESSION['par']['cronogramaFinal'] = '09/2013';
		$_SESSION['par']['cronogramainicial'] = $arrDadosDoc['dopdatainiciovigencia'];
		
		$doptexto = alteraMacrosMinuta($mdoconteudo, $dado['prpid'], $post);
		
		$sqlGerado = "INSERT INTO par.documentoparexgerado2 (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
		$db->executar($sqlGerado);
		$arrDadosDoc['mdoid'] = 41;
		$arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
		$arrDadosDoc['dopdatainiciovigencia'] = $arrDadosDoc['dopdatainiciovigencia'];
		$arrDadosDoc['dopdatafimvigencia'] = '09/2013';
		
		$dopid = salvarDadosMinuta($arrDadosDoc, $doptexto, $dado['dopid']);

		unset($_SESSION['par']['totalVLR']);
	}
	
	$db->commit();
	
} else {
	echo "ACABOU";
	return false;
}

echo "FIM:".date("d/m/Y h:i:s");
echo "<script>window.location=window.location;</script>";
?>