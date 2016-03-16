<?

$pflcod_gerenciador = PFL_COORDENADORLOCAL;
$sis 	  		= "coordenadorlocal";

$turformadores = false;
$turorientadores = true;
$filtro_esfera = " AND i.picid='".$_SESSION['sispacto2']['coordenadorlocal'][$_SESSION['sispacto2']['esfera']]['picid']."'";

include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>