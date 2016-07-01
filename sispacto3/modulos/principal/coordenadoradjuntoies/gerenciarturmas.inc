<?

$pflcod_gerenciador = PFL_COORDENADORADJUNTOIES;
$sis 	  		= "coordenadoradjuntoies";

$turformadores = true;
$turorientadores = true;

$filtro_esfera = " AND i.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."'";


include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>