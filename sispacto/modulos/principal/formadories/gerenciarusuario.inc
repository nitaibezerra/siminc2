<?
$sql_equipe = sqlEquipeFormador(array("uncid"=>$_SESSION['sispacto']['formadories']['uncid'],"iusd"=>$_SESSION['sispacto']['formadories']['iusd']));

$pflcod_gerenciador = PFL_FORMADORIES;
$mensagemalert	= "Dados do Formador IES salvos com sucesso";
$sis 	  		= "formadories";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>