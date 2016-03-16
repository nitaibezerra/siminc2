<?
$sql_equipe = sqlEquipeSupervisor(array("uncid"=>$_SESSION['sispacto2']['supervisories']['uncid']));

$pflcod_gerenciador = PFL_SUPERVISORIES;
$mensagemalert	= "Dados do Supervisor IES salvos com sucesso";
$sis 	  		= "supervisories";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>