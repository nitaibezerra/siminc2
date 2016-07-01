<?

$pflcod_gerenciador = PFL_COORDENADORADJUNTOIES;
$sis 	  		= "coordenadoradjuntoies";
$goto_ant 		= "sisindigena.php?modulo=principal/{$sis}/{$sis}&acao=A&aba=avaliarequipe";
$goto_pro 		= "sisindigena.php?modulo=principal/{$sis}/universidadeexecucao&acao=A&aba=planoatividade";

$turformadores = true;
$turorientadores = true;

$filtro_esfera = " AND i.picid='".$_SESSION['sisindigena'][$sis]['picid']."'";


include_once APPRAIZ_SISINDIGENA."gerenciarturmas.inc";

?>