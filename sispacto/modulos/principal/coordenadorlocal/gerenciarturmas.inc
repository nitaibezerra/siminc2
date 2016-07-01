<?

$pflcod_gerenciador = PFL_COORDENADORLOCAL;
$sis 	  		= "coordenadorlocal";
$goto_ant 		= "sispacto.php?modulo=principal/{$sis}/coordenadorlocalexecucao&acao=A&aba=acompanhamentoavaliacaobolsas";
$goto_pro 		= false;

$turformadores = false;
$turorientadores = true;
$filtro_esfera = " AND i.picid='".$_SESSION['sispacto']['coordenadorlocal'][$_SESSION['sispacto']['esfera']]['picid']."'";

include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>