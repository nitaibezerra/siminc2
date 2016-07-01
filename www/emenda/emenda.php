<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

function __autoload( $classe ){
	if( file_exists( APPRAIZ . "emenda/classes/{$classe}.class.inc" ) ){		
		include_once( APPRAIZ . "emenda/classes/{$classe}.class.inc");
	} elseif( file_exists( APPRAIZ . "includes/classes/{$classe}.class.inc" ) ){
		include_once( APPRAIZ . "includes/classes/{$classe}.class.inc");
	}
}

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
include_once 'fndeWebservice.php';
include_once '_funcoesAnalise.php';
include_once '_funcoesWorkflow.php';

simec_magic_quotes();

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>

<script type="text/javascript" src="js/pta.js"></script>