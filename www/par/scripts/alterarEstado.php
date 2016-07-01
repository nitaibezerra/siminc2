<?php
set_time_limit(30000);
$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";

$_SESSION['usucpf'] = '';
$_SESSION['usucpforigem'] = '';

$db = new cls_banco();



$sql = "SELECT preid, docid FROM obras.preobra WHERE muncod = '3304557' AND prestatus = 'A'";

$arDados = $db->carregar($sql);

foreach($arDados as $dados){

	$preid = $dados['preid'];
	$docid = $dados['docid'];
	
	wf_alterarEstado($docid, 420, $cmddsc = '', array( 'preid' => $preid ));
	
}

?>