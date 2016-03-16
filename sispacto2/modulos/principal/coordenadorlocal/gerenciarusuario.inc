<?
$sql_equipe = sqlEquipeCoordenadorLocal(array("picid"=>$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']));

$pflcod_gerenciador = PFL_COORDENADORLOCAL;
$mensagemalert	= "Dados do Coordenador Local salvos com sucesso";
$sis 	  		= "coordenadorlocal";

include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>