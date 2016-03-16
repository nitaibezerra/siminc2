<?
$sql_equipe = sqlEquipeCoordenadorAdjunto(array("picid"=>$_SESSION['sisindigena2']['coordenadoradjuntoies']['picid']));

$pflcod_gerenciador = PFL_COORDENADORADJUNTOIES;
$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISINDIGENA."gerenciarequipe.inc";

?>