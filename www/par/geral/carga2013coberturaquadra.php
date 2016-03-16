<?php
ini_set("memory_limit","2048M");
set_time_limit(0);

include_once 'config.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';
include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
require_once APPRAIZ . 'includes/workflow.php';
require_once '../_funcoes.php';
require_once '../_funcoesPar.php';
require_once '../_constantes.php';

$db = new cls_banco();

/* ------------------------------------------------------------------ */
// INSERE CONSTRUÇÃO DE QUADRAS

$sql = "select * from carga.tascaconstrucao -- where \"Cod_IBGE\" in ('1501907', '1200203' )";
$dados = $db->carregar($sql);
$total = 0;

if( is_array($dados) ){
	//dbg($dados,1);
	foreach($dados as $dado){
		$quantidades = $dado['quantidade'];
		$muncod = $dado['Cod_IBGE'];
		$uf = $dado['UF'];
		$sql = "SELECT (COUNT(preid) + 1) FROM obras.preobra WHERE muncod = '".$muncod."' and ptoid IN (27) AND tooid = 1";
		$qtdDeObras = $db->pegaUm($sql);
		
		//dbg($quantidades,1);
		for($x=1; $x <= $quantidades; $x++ ){
			$sql = "INSERT INTO obras.preobra(
						presistema, preidsistema, ptoid, muncod, estuf, muncodpar,
	           			predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	           			preprioridade, preanoselecao, preesfera)
					VALUES (
						23, 23, 5, '".$muncod."', '".$uf."', '".$muncod."', now(), 
				    	'2013', 'PAC 2 - Construção de Quadra Escolar Coberta '||to_char($qtdDeObras, '000')||'/2013', 'A', false, true, 1,
				    	(SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.muncodpar = '".$muncod."' AND pt.ptoclassificacaoobra = 'Q' )
				    	, '2013', 'M'
				    	)
				    RETURNING
				    	preid;";
			$preid = $db->pegaUm($sql);
			preCriarDocumento($preid, WF_FLUXO_PRO_INFANCIA);
			$total++;
			echo "<br>Inserido Construção de Quadras: ".$total;
			$qtdDeObras++;
			
		}
		$qtdDeObras = 0;
	}
	//$db->commit();
}

echo "<br>Total de Construção de Quadras inseridas: ".$total;

/* ------------------------------------------------------------------ */
// INSERE CONSTRUÇÃO DE COBERTURA DE QUADRAS

$sql = "select * from carga.tascacobertura -- where \"Cod_IBGE\" in ( '1501907', '1200203')";
$dadoscobertura = $db->carregar($sql);
$total = 0;

if( is_array($dadoscobertura) ){
	//dbg($dados,1);
	foreach($dadoscobertura as $dado2){
		$quantidades = $dado2['quantidade'];
		$muncod = $dado2['Cod_IBGE'];
		$uf = $dado2['UF'];
		$sql = "SELECT (COUNT(preid) + 1) FROM obras.preobra WHERE muncod = '".$muncod."' and ptoid IN (23) AND tooid = 1";
		$qtdDeCobertura = $db->pegaUm($sql);
		
		//dbg($quantidades,1);
		for($x=1; $x <= $quantidades; $x++ ){
			$sql = "INSERT INTO obras.preobra(
						presistema, preidsistema, ptoid, muncod, estuf, muncodpar,
	           			predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	           			preprioridade, preanoselecao, preesfera)
					VALUES (
						23, 23, 23, '".$muncod."', '".$uf."', '".$muncod."', now(), 
				    	'2013', 'PAC 2 - Cobertura de Quadra Escolar '||to_char($qtdDeCobertura, '000')||'/2013', 'A', false, true, 1,
				    	coalesce((SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.muncodpar = '".$muncod."' AND pt.ptoclassificacaoobra = 'C' ),1)
				    	,'2013', 'M' )
				    RETURNING
				    	preid;";
			$preid = $db->pegaUm($sql);
			preCriarDocumento($preid, WF_FLUXO_PRO_INFANCIA);
			$total++;
			echo "<br>Inseridno Cobertura de Quadras: ".$total;
			$qtdDeCobertura++;
		}
		$qtdDeObras = 0;
	}
	
}
$db->commit();
echo "<br>Total de Cobertura de Quadras inseridas: ".$total;

/*
			$sql = "INSERT INTO obras.preobra(
						presistema, preidsistema, ptoid, muncod, estuf, muncodpar,
	           			predtinclusao, preano, predescricao, prestatus, premcmv, precarga, tooid,
	           			preprioridade, preanoselecao, preesfera)
					VALUES (
						23, 23, 23, '".$muncod."', '".$uf."', '".$muncod."', now(), 
				    	'2013', 'PAC 2 - Cobertura de Quadra Escolar '||to_char($qtdDeObras, '000'), 'A', false, true, 1,
				    	coalesce((SELECT max(preprioridade)+1 as prioridade FROM obras.preobra p2 INNER JOIN obras.pretipoobra pt ON pt.ptoid = p2.ptoid WHERE p2.muncodpar = '".$muncod."' AND pt.ptoclassificacaoobra = 'C' ),1)
				    	,'2013', 'M' )
				    RETURNING
				    	preid;";
*/
?>