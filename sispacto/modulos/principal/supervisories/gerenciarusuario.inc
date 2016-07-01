<?
$sql_equipe = sqlEquipeSupervisor(array("uncid"=>$_SESSION['sispacto']['supervisories']['uncid']));

$pflcod_gerenciador = PFL_SUPERVISORIES;
$mensagemalert	= "Dados do Supervisor IES salvos com sucesso";
$sis 	  		= "supervisories";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>