<?php
set_time_limit(0);
ini_set("memory_limit", "12000M");

global $db;

$sql = "SELECT 
			d.dopid, d.mdoid, d.dopvalortermo, d.prpid
		FROM 
			par.vm_documentopar_ativos d 
		INNER JOIN par.modelosdocumentos md ON md.mdoid = d.mdoid 
		WHERE 
			d.dopid NOT IN ( SELECT dopid FROM par.documentoparexgerado )
			AND d.dopnumerodocumento IN (2854,2865,3608,3684,3774,3817,3819,3821,3824,3827,3835,3836,3838,3848,3853,3857,3863,3867,3872,3877,3878,3882,3890,3891,3893,3901,3907,3921,3927,3931,3933,4006,4011,4186,4226,4244,4246,4247,4250,4252,4254,4333,4335,4348,4380,4385,4397,4403,4431,4440,4447,4453,4472,4474,4541,4558,4560,4593,4595,4601,4636,4641,4659,4684,4685,4694,4717,4719,4720,4727,4737,4744,4752,4756,4833,4845,4880)
		ORDER BY
			d.mdoid, d.dopid
		LIMIT 1";
$dados = $db->carregar($sql);
//ver($dados, d);
$sqlPerdido = "";
$sqlGerado = "";

if( $dados[0] ){
	foreach( $dados as $dado ){
	
		if( $dado['mdoid'] == 19 || $dado['mdoid'] == 39 ){	// "PAR_Termo de Compromisso_Estados"
			
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
			
			$sql = "SELECT mdoconteudo, tpdcod FROM par.modelosdocumentos WHERE mdostatus = 'A' AND mdoid = 42";
			$arrModelo = $db->pegaLinha($sql);
			
			$mdoconteudo = $arrModelo['mdoconteudo'];
			$tpdcod = 102; //$arrModelo['tpdcod'];
			
			$sql = "SELECT sbdid as chk FROM par.termocomposicao WHERE dopid = ".$dado['dopid'];
			$subacoes = $db->carregarColuna($sql);
			
			$post = array('mdoid' => 42, 'dopid' => $dados['dopid'], 'tpdcod' => $tpdcod, 'chk' => array());
			
			$_SESSION['par']['cronogramaFinal'] = '07/2013';
			$_SESSION['par']['cronogramainicial'] = $arrDadosDoc['dopdatainiciovigencia'];
			
			$doptexto = alteraMacrosMinuta($mdoconteudo, $dado['prpid'], $post);
			
			/*if( number_format($dado['dopvalortermo'], 2, ',', '.') != number_format($_SESSION['par']['totalVLR'], 2, ',', '.') ){
				$sqlPerdido = "INSERT INTO par.documentoparvalordivergente (dopid, dopvalortermoantigo, dopvalortermonovo) VALUES (".$dado['dopid'].",".$dado['dopvalortermo'].",".$_SESSION['par']['totalVLR']."); ";
				$db->executar($sqlPerdido);
			} else { */
				
				$sqlGerado = "INSERT INTO par.documentoparexgerado (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				$db->executar($sqlGerado);
				$arrDadosDoc['mdoid'] = 42;
				$arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
				$arrDadosDoc['dopdatainiciovigencia'] = $arrDadosDoc['dopdatainiciovigencia'];
				$arrDadosDoc['dopdatafimvigencia'] = '07/2013';
				
				$dopid = salvarDadosMinuta($arrDadosDoc, $doptexto, $dado['dopid']);
				
				$db->executar("UPDATE par.documentopar SET dopstatus = 'I', dopidpai = ".$arrDadosDoc['dopidpai'].", dopnumerodocumento = ".$arrDadosDoc['dopidpai']." WHERE dopid = ".$dopid);
				$db->executar("UPDATE par.documentopar SET dopstatus = 'A', dopidpai = ".$dopid." WHERE dopid = ".$dado['dopid']);
				
			//}
			
		}elseif( $dado['mdoid'] == 20 || $dado['mdoid'] == 23 ){ // "PAR_Termo de Compromisso_Municipios"
			
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
			
			$post = array('mdoid' => 41, 'tpdcod' => $tpdcod, 'dopid' => $dado['dopid'], 'chk' => array());
			
			$_SESSION['par']['cronogramaFinal'] = '07/2013';
			$_SESSION['par']['cronogramainicial'] = $arrDadosDoc['dopdatainiciovigencia'];
			
			$doptexto = alteraMacrosMinuta($mdoconteudo, $dado['prpid'], $post);
			
	/*		if( number_format($dado['dopvalortermo'], 2, ',', '.') != number_format($_SESSION['par']['totalVLR'], 2, ',', '.') ){
				$sqlPerdido = "INSERT INTO par.documentoparvalordivergente (dopid, dopvalortermoantigo, dopvalortermonovo) VALUES (".$dado['dopid'].",".$dado['dopvalortermo'].",".$_SESSION['par']['totalVLR']."); ";
				$db->executar($sqlPerdido);
			} else { */
				
				$sqlGerado = "INSERT INTO par.documentoparexgerado (dopid, dopdata) VALUES (".$dado['dopid'].", 'NOW()'); ";
				$db->executar($sqlGerado);
				$arrDadosDoc['mdoid'] = 41;
				$arrDadosDoc['dopvalor'] = $_SESSION['par']['totalVLR'];
				$arrDadosDoc['dopdatainiciovigencia'] = $arrDadosDoc['dopdatainiciovigencia'];
				$arrDadosDoc['dopdatafimvigencia'] = '07/2013';
				
				$dopid = salvarDadosMinuta($arrDadosDoc, $doptexto, $dado['dopid']);
				
				$db->executar("UPDATE par.documentopar SET dopstatus = 'I', dopidpai = ".$arrDadosDoc['dopidpai'].", dopnumerodocumento = ".$arrDadosDoc['dopidpai']." WHERE dopid = ".$dopid);
				$db->executar("UPDATE par.documentopar SET dopstatus = 'A', dopidpai = ".$dopid." WHERE dopid = ".$dado['dopid']);
	//		}
			
		}elseif( $dado['mdoid'] == 21 ){	// "PAR_Termo de Compromisso_Estados_Emendas"
			ver($dado);
		}
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