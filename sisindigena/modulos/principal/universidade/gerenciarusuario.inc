<?
$sql_equipe = sqlEquipeCoordenadorIES(array("uncid"=>$_SESSION['sisindigena']['universidade']['uncid']));

$pflcod_gerenciador = PFL_COORDENADORIES;
$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "universidade";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=dados";
$goto_sal 		= "sisindigena.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=gerenciarusuario";
$goto_pro 		= false;


include_once APPRAIZ_SISINDIGENA."gerenciarequipe.inc";

?>