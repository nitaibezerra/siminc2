<?
$sql_equipe = sqlEquipeProfessor(array("uncid"=>$_SESSION['sispacto2']['professoralfabetizador']['uncid']));

$pflcod_gerenciador = PFL_PROFESSORALFABETIZADOR;
$mensagemalert	= "Dados do Professor Alfabetizador salvos com sucesso";
$sis 	  		= "professoralfabetizador";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>