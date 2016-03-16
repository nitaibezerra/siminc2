<?

$pflcod_gerenciador = PFL_COORDENADORIES;
$sis 	  		= "universidade";

$turformadores 			= true;
$turorientadores 		= true;
$tursupervisores 		= true;
$turcoordenadoreslocais = true;

$filtro_esfera = " AND i.uncid='".$_SESSION['sispacto2'][$sis]['uncid']."'";


include_once APPRAIZ_SISPACTO."gerenciarturmas.inc";

?>