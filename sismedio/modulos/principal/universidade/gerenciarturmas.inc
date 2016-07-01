<?

$pflcod_gerenciador = PFL_COORDENADORIES;

$turformadores = true;
$turorientadores = true;
$sis = 'universidade';

$filtro_esfera = " AND i.uncid='".$_SESSION['sismedio'][$sis]['uncid']."'";


include_once APPRAIZ_SISMEDIO."gerenciarturmas.inc";

?>