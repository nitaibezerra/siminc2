<?
$sql_equipe = sqlEquipeProfessor(array("uncid"=>$_SESSION['sispacto']['professoralfabetizador']['uncid']));

$pflcod_gerenciador = PFL_PROFESSORALFABETIZADOR;
$mensagemalert	= "Dados do Professor Alfabetizador salvos com sucesso";
$sis 	  		= "professoralfabetizador";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=principal";
$goto_sal 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=dados";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarusuario";


include_once APPRAIZ_SISPACTO."gerenciarequipe.inc";

?>