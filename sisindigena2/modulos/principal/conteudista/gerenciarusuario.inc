<?
$sql_equipe = sqlEquipeSupervisor(array("uncid"=>$_SESSION['sisindigena2']['supervisories']['uncid']));

$pflcod_gerenciador = PFL_SUPERVISORIES;
$mensagemalert	= "Dados do Supervisor IES salvos com sucesso";
$sis 	  		= "supervisories";
$goto_ant 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sisindigena2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISINDIGENA."gerenciarequipe.inc";

?>