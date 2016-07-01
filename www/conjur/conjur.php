<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

function __autoload($class_name) {
	$arCaminho = array(
						APPRAIZ . "includes/classes/modelo/public/",
						APPRAIZ . "includes/classes/modelo/territorios/",
						APPRAIZ . "includes/classes/modelo/entidade/",
						APPRAIZ . "includes/classes/controller/",
						APPRAIZ . "includes/classes/view/",
						APPRAIZ . "includes/classes/html/",
						APPRAIZ . "obras/classe/controller/",
						APPRAIZ . "obras/classe/modelo/" );
					  
	foreach($arCaminho as $caminho){
		$arquivo = $caminho . $class_name . '.class.inc';
		if ( file_exists( $arquivo ) ){
			require_once( $arquivo );
			break;	
		}
	}				  
}

// Painel do Administrador e Super Usuário
if ($db->testa_superuser()) {
    $painelCabecalho = array(
        array('titulo' => 'WorkFlow', 'funcao' => 'montarPainelWorkflow', 'icon' => 'tasks'),
    );
}

// carrega as funções específicas do módulo
include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once '_constantes.php';
include_once '_funcoes.php';

// Classes
include_once APPRAIZ . "conjur/classes/modelo/ProcessoConjur.class.inc"; 

// Carrega as funções de controle de acesso
include_once "controleAcesso.inc";

?>