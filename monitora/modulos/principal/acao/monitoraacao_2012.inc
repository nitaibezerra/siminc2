<?php 
$acaidrap = $_REQUEST['acaorap'];

$perfis = pegaPerfilGeral();

$permPreenchimento = false;
if( $db->testa_superuser() || ( in_array(PFL_COORDENADOR_ACAO,$perfis) || in_array(PERFIL_MONIT_CPMO, $perfis) || in_array(PERFIL_MONIT_VALIDADOR, $perfis) ) ) {
	$permPreenchimento = true;
}

if ( $acaidrap ){
	$_SESSION['acaorap'] = $acaidrap;
	include (APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/acao/monitoraacao_2012_RAP.inc");
}else{
	include (APPRAIZ.$_SESSION['sisdiretorio']."/modulos/principal/acao/monitoraacao_2012_LOA.inc");
}
?>