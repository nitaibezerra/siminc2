<?php
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '';
$_SESSION['usucpf'] = '';

$db = new cls_banco();

$sql = "select * from emenda.v_recursoempenho ptr
		INNER JOIN workflow.documento doc ON ptr.docid = doc.docid
		INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
		where empenhado='t' and esd.esdid=57";
$lista = $db->carregar($sql);
if($lista[0]) {
	foreach($lista as $l) {
		$docid = $l['docid'];
		$aedid = 252;
		$dados = array('ptrid' => $l['ptrid']);
		$result = wf_alterarEstado( $docid, $aedid, $cmddsc = '', $dados);
		echo "Plano de Trabalho PTRID:".$l['ptrid']." | Result.".$result."<br>";		
	}
}
?>