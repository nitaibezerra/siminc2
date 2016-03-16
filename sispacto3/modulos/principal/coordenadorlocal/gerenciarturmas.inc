<?

$pflcod_gerenciador = PFL_COORDENADORLOCAL;
$sis 	  		= "coordenadorlocal";

$turcoordenadoreslocais = true;
$turorientadores = true;
$filtro_esfera = " AND i.picid='".$_SESSION['sispacto3']['coordenadorlocal'][$_SESSION['sispacto3']['esfera']]['picid']."'";

include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>