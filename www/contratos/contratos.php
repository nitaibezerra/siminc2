<?php

function __autoload($class_name) {
	$arCaminho = array(
			APPRAIZ . "contratos/classes/",
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

//Carrega par�metros iniciais do SIG
include_once "controleInicio.inc";

// carrega as fun��es espec�ficas do m�dulo
// include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

//Carrega workflow
include_once APPRAIZ . 'includes/workflow.php';

//Carrega as fun��es de controle de acesso
include_once "controleAcesso.inc";
?>