<?
$sql_equipe = sqlEquipeFormador(array("uncid"=>$_SESSION['sispacto3']['formadories']['uncid'],"iusd"=>$_SESSION['sispacto3']['formadories']['iusd']));

$pflcod_gerenciador = PFL_FORMADORIES;
$mensagemalert	= "Dados do Formador IES salvos com sucesso";
$sis 	  		= "formadories";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>