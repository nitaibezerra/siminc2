<?
$sql_equipe = sqlEquipeOrientador(array("uncid"=>$_SESSION['sismedio']['orientadorestudo']['uncid'],"iusd"=>$_SESSION['sismedio']['orientadorestudo']['iusd']));

$pflcod_gerenciador = PFL_ORIENTADORESTUDO;
$mensagemalert	= "Dados do Orientador de Estudo salvos com sucesso";
$sis 	  		= "orientadorestudo";
$goto_ant 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_sal 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";
$goto_pro 		= "sismedio.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISMEDIO."gerenciarequipe.inc";

?>