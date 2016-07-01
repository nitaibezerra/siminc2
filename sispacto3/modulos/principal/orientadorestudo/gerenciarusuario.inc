<?
$sql_equipe = sqlEquipeOrientador(array("uncid"=>$_SESSION['sispacto3']['orientadorestudo']['uncid'],"iusd"=>$_SESSION['sispacto3']['orientadorestudo']['iusd']));

$pflcod_gerenciador = PFL_ORIENTADORESTUDO;
$mensagemalert	= "Dados do Orientador de Estudo salvos com sucesso";
$sis 	  		= "orientadorestudo";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>