<?
$sql_equipe = sqlEquipeSupervisor(array("uncid"=>$_SESSION['sismedio']['supervisories']['uncid']));

$pflcod_gerenciador = PFL_SUPERVISORIES;
$mensagemalert	= "Dados do Supervisor IES salvos com sucesso";
$sis 	  		= "supervisories";
$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISMEDIO."gerenciarequipe.inc";

?>