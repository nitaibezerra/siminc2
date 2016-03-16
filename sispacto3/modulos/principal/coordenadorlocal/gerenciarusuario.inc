<?
$sql_equipe = sqlEquipeCoordenadorLocal(array("picid"=>$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid']));

$pflcod_gerenciador = PFL_COORDENADORLOCAL;
$mensagemalert	= "Dados do Coordenador Local salvos com sucesso";
$sis 	  		= "coordenadorlocal";

include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>