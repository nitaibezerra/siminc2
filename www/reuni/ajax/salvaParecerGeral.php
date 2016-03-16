<?php


// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
//include APPRAIZ . "includes/workflow.php";
//include APPRAIZ . "www/reuni/constantesPerfil.php";

$db = new cls_banco();

$codUnidade = trim(base64_decode($_REQUEST['codUnidade']));
$tipoParecer = trim($_REQUEST['tipoParecer']);
$parecer	 = trim($_REQUEST['parecer']);
$situacao	 = trim($_REQUEST['situacao']);
$usucpf 	 = $_SESSION['usucpf'];


//die(var_dump($_REQUEST));

$sql = "select count(*) as qtd from reuni.parecer where unpid = $codUnidade and tpacod = $tipoParecer";
if($db->pegaUm($sql) > 0){
	$sql = '';
	$sql = "update reuni.parecer set sitcod = $situacao , usucpf = '$usucpf' , pardtatual = now() , pardsc = '$parecer' where tpacod = $tipoParecer and unpid = $codUnidade ";
	$mes = "Parecer atualizado com sucesso!";
}
else
{
	$sql = '';
	$sql = "insert into reuni.parecer (sitcod,pflcod,usucpf,pardtatual,tpacod,pardsc,rspcod,unpid) ".
			"values " .
			" ($situacao,null,'$usucpf',now(),$tipoParecer,'$parecer',null,$codUnidade)";
	$mes = "Parecer salvo com sucesso!";		
}


//die($sql);

$db->executar($sql);
$db->commit();
die($mes);





?>