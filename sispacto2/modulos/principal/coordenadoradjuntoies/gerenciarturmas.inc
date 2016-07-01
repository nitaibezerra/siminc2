<?

$pflcod_gerenciador = PFL_COORDENADORADJUNTOIES;
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sispacto2.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=acompanhamentoavaliacaobolsas";
$goto_pro 		= false;

$turformadores = true;
$turorientadores = true;

$filtro_esfera = " AND i.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."'";


include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>