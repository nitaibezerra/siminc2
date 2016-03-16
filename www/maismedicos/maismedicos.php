<?php
function __autoload($class_name) {
	$arCaminho = array(
			APPRAIZ . "maismedicos/classes/",
			APPRAIZ . "includes/classes/",
	);

	foreach($arCaminho as $caminho){
		$arquivo = $caminho . $class_name . '.class.inc';
		if ( file_exists( $arquivo ) ){
			require_once( $arquivo );
			break;
		}
	}
}

//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

$xperfil = arrayPerfil();
$entrou = false;

if( in_array(PERFIL_SUPER_USUARIO ,$xperfil) || in_array(PERFIL_ADMINISTRADOR,$xperfil) ){

	$link  = $_SERVER['REQUEST_URI'];
	$pagina = explode("&", $link);

	if( $pagina[0] == '/maismedicos/maismedicos.php?modulo=sistema/usuario/consusuario' ||
		$pagina[0] == '/maismedicos/maismedicos.php?modulo=sistema/usuario/cadusuario' ) {
		$entrou = true;
	}
}

if($entrou == false) {
	if ($_SERVER['REQUEST_URI'] != '/maismedicos/maismedicos.php?modulo=inicio&acao=C' && empty($_GET['forca_pagina'])) {
		header("Location: /maismedicos/maismedicos.php?modulo=inicio&acao=C");
	}
}

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>