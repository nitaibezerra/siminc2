<?
$sql_equipe = sqlEquipeCoordenadorAdjunto(array("picid"=>$_SESSION['sisindigena']['coordenadoradjuntoies']['picid']));

$pflcod_gerenciador = PFL_COORDENADORADJUNTOIES;
$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISINDIGENA."gerenciarequipe.inc";

?>