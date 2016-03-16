<?
$sql_equipe = sqlEquipeOrientador(array("uncid"=>$_SESSION['sispacto']['orientadorestudo']['uncid'],"iusd"=>$_SESSION['sispacto']['orientadorestudo']['iusd']));

$pflcod_gerenciador = PFL_ORIENTADORESTUDO;
$mensagemalert	= "Dados do Orientador de Estudo salvos com sucesso";
$sis 	  		= "orientadorestudo";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=gerenciarusuario";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>