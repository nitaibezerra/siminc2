<?

$pflcod_gerenciador = PFL_COORDENADORADJUNTOIES;
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=acompanhamentoavaliacaobolsas";
$goto_pro 		= false;

$turformadores = true;
$turorientadores = true;

$filtro_esfera = " AND i.uncid='".$_SESSION['sispacto'][$sis]['uncid']."'";


include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>