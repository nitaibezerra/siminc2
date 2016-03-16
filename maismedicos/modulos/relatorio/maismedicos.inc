<?php 
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br>";

if ($_SESSION['baselogin'] == 'simec_desenvolvimento') {
    define('PERFIL_CONSULTA_EXTERNA', 1087); #CONSULTA EXTERNA. - DEV
    define('PERFIL_CONSULTA_COCKPIT', 1088); #CONSULTA COCKPIT. - DEV
} else {
    define('PERFIL_CONSULTA_EXTERNA', 1089); #CONSULTA EXTERNA. - ESP
    define('PERFIL_CONSULTA_COCKPIT', 1091); #CONSULTA COCKPIT. - ESP
}

if(!$_REQUEST['requisicaoAjax']){
	$_REQUEST['requisicaoAjax'] = "popUpMaisMedicos";
	$_REQUEST['visao'] = "brasil";
	$_GET['visao'] = "brasil";
	$_GET['active'] = "0";
}
$nao_valida_acesso = true;
include APPRAIZ."pde/modulos/principal/cockpit_mais_medicos.inc";

?>