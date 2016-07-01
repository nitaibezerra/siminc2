<?

$pflcod_gerenciador = PFL_COORDENADORIES;
$sis 	  		= "universidade";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarequipe";
$goto_pro 		= "sispacto.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=planoatividade";

$turformadores = true;
$turorientadores = true;

$filtro_esfera = " AND i.uncid='".$_SESSION['sispacto'][$sis]['uncid']."'";


include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>