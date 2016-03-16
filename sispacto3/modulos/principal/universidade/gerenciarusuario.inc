<?
$sql_equipe = sqlEquipeCoordenadorIES(array("uncid"=>$_SESSION['sispacto3']['universidade']['uncid']));

$pflcod_gerenciador = PFL_COORDENADORIES;
$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "universidade";

include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>