<?
$sql_equipe = sqlEquipeFormadorRegional(array("uncid"=>$_SESSION['sismedio']['formadorregional']['uncid'],"iusd"=>$_SESSION['sismedio']['formadorregional']['iusd']));

$pflcod_gerenciador = PFL_FORMADORREGIONAL;
$mensagemalert	= "Dados do Formador IES salvos com sucesso";
$sis 	  		= "formadorregional";
$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISMEDIO."gerenciarequipe.inc";

?>