<?php


// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
include APPRAIZ . "includes/workflow.php";
//include APPRAIZ . "www/reuni/constantesPerfil.php";

$db = new cls_banco();

if(isset($_REQUEST['cod'])){
	
	$cod = base64_decode($_REQUEST['cod']);
	
	$sql = "update public.arquivo set arqstatus = 0 where arqid = $cod";
	
	if($db->executar($sql))
	{
		$db->commit();
		die("Arquivo excluído com sucesso!");
	}
	else
	{ 
		die("Não foi possivel remover o arquivo.");
	}
	
}


?>