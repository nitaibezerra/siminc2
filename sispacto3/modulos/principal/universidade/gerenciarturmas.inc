<?

$pflcod_gerenciador = PFL_COORDENADORIES;
$sis 	  		= "universidade";

$turcoordenadoradjuntoies = true;
$turformadores 			= true;
$turorientadores 		= true;
$tursupervisores 		= true;
$turcoordenadoreslocais = true;

$filtro_esfera = " AND i.uncid='".$_SESSION['sispacto3'][$sis]['uncid']."'";


include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>