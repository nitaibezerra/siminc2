<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

function __autoload( $classe ){
	if( file_exists( APPRAIZ . "seed/classes/{$classe}.class.inc" ) ){		
		include_once( APPRAIZ . "seed/classes/{$classe}.class.inc");
	} else {
		include_once( APPRAIZ . "includes/classes/{$classe}.class.inc");
	}
}

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>