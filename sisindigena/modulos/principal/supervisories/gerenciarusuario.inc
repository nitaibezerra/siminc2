<?
$sql_equipe = sqlEquipeSupervisor(array("iusd"=>$_SESSION['sisindigena']['supervisories']['iusd']));

$pflcod_gerenciador = PFL_SUPERVISORIES;
$mensagemalert	= "Dados do Supervisor IES salvos com sucesso";
$sis 	  		= "supervisories";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISINDIGENA."gerenciarequipe.inc";

?>