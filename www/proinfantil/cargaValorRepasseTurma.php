<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "proinfantil/classes/NovasTurmas.class.inc";
include_once "_funcoes_novasturmas.php";
include_once "_constantes.php";
 
session_start();

//$_SESSION['usucpf'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$sql = "select distinct
			t.turid,
			t.turano as exercicio,
			(t.turano - 1) as anoCenso,
			t.turmes as ntmmmes,
		    t.muncod
		from
			proinfantil.turma t
            inner join proinfantil.novasturmasworkflowturma nt on nt.turid = t.turid and nt.muncod = t.muncod
            inner join proinfantil.analisenovasturmasaprovacao a on a.turid = t.turid
		where
            t.turano > 2012";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();

foreach ($arrDados as $tur) {
	
	$obNovasTurmas = new NovasTurmas( $tur );
	$arrRepasse = $obNovasTurmas->carregaRepassePorTurma();
	
	$arrRegistro = array();
	$vrlTotal = 0;
	foreach ($arrRepasse as $key => $v) {
		/*$arrRegistro[$key] = array(
									'nome' => $v['nome'],
									'total_alunos' => '&nbsp;'.$v['total_alunos'],
									'vaavalor' => $v['vaavalor'],
									'periodo' => $v['periodo'].'&nbsp;',
									'valor_total' => $v['valor_total']
								);*/
		
		$valor = str_replace(".","", $v['valor_total']);
		$valor = str_replace(",",".", $valor);
		$vrlTotal += $valor;
	}
	
	$sql = "UPDATE proinfantil.turma SET turvalorrepasse = $vrlTotal WHERE  turid = {$tur['turid']}";
	$db->executar($sql);
	$db->commit();
	//ver($arrRegistro, $tur['turid'], $vrlTotal,d);
}
echo 'ok';

?>