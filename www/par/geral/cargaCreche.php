<?php 

ini_set("memory_limit","2048M");

include_once 'config.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
require_once APPRAIZ . 'includes/workflow.php';
require_once '../_funcoes.php';
require_once '../_funcoesPar.php';
require_once '../_constantes.php';

$db = new cls_banco();

// Creches
$sql = "SELECT 	
			t.muncod, 
			t.estuf,
			qtdcreche as total, 
			qtdcreche-qtdmcmv as creche,
			qtdmcmv as mcmv
		FROM 
			carga.\"CargaCreches\" c
		INNER JOIN territorios.municipio t ON substr(t.muncod,1,6) = trim(c.muncod)
		WHERE
			qtdcreche > 0
			--AND t.estuf = 'RJ'
		--LIMIT 0";

$dados = $db->carregar($sql);
$total = 0;
if( is_array($dados) ){
	foreach($dados as $dado){
		$num = 1;
		$num2 = 1;
		for($x=1; $x <= $dado['total']; $x++ ){
			if($dado['creche'] >0){
				$sql = "INSERT INTO obras.preobra(
							presistema, preidsistema, ptoid, estuf, 
	          	 			muncod, muncodpar, predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	          	 			preprioridade)
						VALUES (
							23, 23, 2, '".$dado['estuf']."', '".$dado['muncod']."', '".$dado['muncod']."', now(), 
					    	'2011', 'PAC 2 - CRECHE/PRÉ-ESCOLA '||to_char($num, '000'), 'A', false, true, 1,
				    	(SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.muncodpar = '".$dado['muncod']."' AND pt.ptoclassificacaoobra = 'P' ))
					    RETURNING
					    	preid;";
//				echo "<br>".$sql;
				$dado['creche']--;
			}elseif($dado['mcmv'] >0){
				$sql = "INSERT INTO obras.preobra(
							presistema, preidsistema, ptoid, estuf, 
	          	 			muncod, muncodpar, predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	          	 			preprioridade)
						VALUES (
							23, 23, 2, '".$dado['estuf']."', '".$dado['muncod']."', '".$dado['muncod']."', now(), 
					    	'2011', 'PAC 2 - CRECHE/PRÉ-ESCOLA MCMV '||to_char($num2, '000'), 'A', true, true, 1,
				    	(SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.muncodpar = '".$dado['muncod']."' AND pt.ptoclassificacaoobra = 'P' ))
					    RETURNING
					    	preid;";
//				echo "<br><br>".$sql;
				$dado['mcmv']--;
				$num2++;
			}
			$preid = $db->pegaUm($sql);
			preCriarDocumento($preid, WF_FLUXO_PRO_INFANCIA);
			$num++;
			$total++;
		}
	}
}
//Cobertura Estadual
//Não tem ptoid
$sql = "SELECT 
			\"UF\" as estuf, 
			\"QTDCOBERTURA\" as qtd 
		FROM 
			carga.\"CargaCOBERTURAestadual\" 
		WHERE 
			\"QTDCOBERTURA\" > 0
			--AND \"UF\" = 'RJ'
		--LIMIT 0";

$dados1 = $db->carregar($sql);
if( is_array($dados1) ){
	foreach($dados1 as $dado){
		$num = 1;
		for($x=1; $x <= $dado['qtd']; $x++ ){
			$sql = "INSERT INTO obras.preobra(
						presistema, preidsistema, ptoid, estuf, estufpar,
	           			predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	           			preprioridade)
					VALUES (
						23, 23, 23, '".$dado['estuf']."', '".$dado['estuf']."', now(), 
				    	'2011', 'PAC 2 - Cobertura de Quadra Escolar '||to_char($num, '000'), 'A', false, true, 1,
				    	coalesce((SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.estufpar = '".$dado['estuf']."' AND pt.ptoclassificacaoobra = 'C' ),1))
				    RETURNING
				    	preid;";
//			echo "<br><br>".$sql;
			$preid = $db->pegaUm($sql);
			preCriarDocumento($preid, WF_FLUXO_PRO_INFANCIA);
			$num++;
			$total++;
		}
	}
}
//Cobertura Municipal
//Não tem ptoid
$sql = "SELECT 
			\"MUNCOD7\" as muncod, 
			\"UF\" as estuf, 
			\"QTDCOBERTURA\" as qtd 
		FROM 
			carga.\"CargaCoberturaMunicipal\" 
		WHERE 
			\"QTDCOBERTURA\" > 0
			--AND \"UF\" = 'RJ'
		--LIMIT 1";

$dados2 = $db->carregar($sql);
if( is_array($dados2) ){
	foreach($dados2 as $dado){
		$num = 1;
		for($x=1; $x <= $dado['qtd']; $x++ ){
			$sql = "INSERT INTO obras.preobra(
						presistema, preidsistema, ptoid, muncod, estuf, muncodpar,
	           			predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	           			preprioridade)
					VALUES (
						23, 23, 23, '".$dado['muncod']."', '".$dado['estuf']."', '".$dado['muncod']."', now(), 
				    	'2011', 'PAC 2 - Cobertura de Quadra Escolar '||to_char($num, '000'), 'A', false, true, 1,
				    	coalesce((SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.muncodpar = '".$dado['muncod']."' AND pt.ptoclassificacaoobra = 'C' ),1))
				    RETURNING
				    	preid;";
//			echo "<br><br>".$sql;
			$preid = $db->pegaUm($sql);
			preCriarDocumento($preid, WF_FLUXO_PRO_INFANCIA);
			$num++;
			$total++;
		}
	}
}
//Quadras Estaduais
//Não tem ptoid
$sql = "SELECT 
			\"UF\" as estuf, 
			\"QTDQUADRA\" as qtd 
		FROM 
			carga.\"CargaConstrucaoEstadual\" 
		WHERE 
			\"QTDQUADRA\" > 0
			--AND \"UF\" = 'RJ'
		--LIMIT 0";

$dados3 = $db->carregar($sql);
if( is_array($dados3) ){
	foreach($dados3 as $dado){
		$num = 1;
		for($x=1; $x <= $dado['qtd']; $x++ ){
			$sql = "INSERT INTO obras.preobra(
						presistema, preidsistema, ptoid, estuf, estufpar,
	           			predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	           			preprioridade)
					VALUES (
						23, 23, 5, '".$dado['estuf']."', '".$dado['estuf']."', now(), 
				    	'2011', 'PAC 2 - Construção de Quadra Escolar Coberta '||to_char($num, '000'), 'A', false, true, 1,
				    	(SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.estufpar = '".$dado['estuf']."' AND pt.ptoclassificacaoobra = 'Q' ))
				    RETURNING
				    	preid;";
//			echo "<br><br>".$sql;
			$preid = $db->pegaUm($sql);
			preCriarDocumento($preid, WF_FLUXO_PRO_INFANCIA);
			$num++;
			$total++;
		}
	}
}
//Quadras Munucipais
//Não tem ptoid
$sql = "SELECT 
			\"MUNCOD7\" as muncod, 
			\"UF\" as estuf, 
			\"QTDQUADRA\" as qtd 
		FROM 
			carga.\"CargaConstrucaoMunicipal\" 
		WHERE 
			\"QTDQUADRA\" > 0
			--AND \"UF\" = 'RJ'
		--LIMIT 0";

$dados4 = $db->carregar($sql);
if( is_array($dados4) ){
	foreach($dados4 as $dado){
		$num = 1;
		for($x=1; $x <= $dado['qtd']; $x++ ){
			$sql = "INSERT INTO obras.preobra(
						presistema, preidsistema, ptoid, muncod, estuf, muncodpar,
	           			predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	           			preprioridade)
					VALUES (
						23, 23, 5, '".$dado['muncod']."', '".$dado['estuf']."', '".$dado['muncod']."', now(), 
				    	'2011', 'PAC 2 - Construção de Quadra Escolar Coberta '||to_char($num, '000'), 'A', false, true, 1,
				    	(SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.muncodpar = '".$dado['muncod']."' AND pt.ptoclassificacaoobra = 'Q' ))
				    RETURNING
				    	preid;";
//			echo "<br><br>".$sql;
			$preid = $db->pegaUm($sql);
			preCriarDocumento($preid, WF_FLUXO_PRO_INFANCIA);
			$num++;
			$total++;
		}
	}
}
$db->commit();
echo "<br>Total de obras inseridas: ".$total;
?>