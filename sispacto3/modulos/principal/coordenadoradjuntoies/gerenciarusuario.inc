<?

$pflcod_gerenciador = PFL_COORDENADORADJUNTOIES;
$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "coordenadoradjuntoies";

$sql_equipe = sqlEquipeCoordenadorAdjunto(array("uncid"=>$_SESSION['sispacto3']['coordenadoradjuntoies']['uncid'],"pflcod_gerenciador"=>$pflcod_gerenciador));

include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>