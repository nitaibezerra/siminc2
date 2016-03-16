<?
$sql_equipe = sqlEquipeCoordenadorIES(array("uncid"=>$_SESSION['sispacto2']['universidade']['uncid']));

$pflcod_gerenciador = PFL_COORDENADORIES;
$mensagemalert	= "Dados do Coordenador da IES salvos com sucesso";
$sis 	  		= "universidade";
$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=dados";
$goto_sal 		= "sispacto2.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=gerenciarusuario";
$goto_pro 		= false;


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>