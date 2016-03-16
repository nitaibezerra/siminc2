<?php
//Carrega parametros iniciais do simec
include_once 'controleInicio.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

// carrega libs necessarias
include_once APPPUB . 'libs/Math.class.inc';

// carregando classes automaticamente
function __autoload($classe) {
	require_once(APPPUB . 'classes/' . $classe . '.class.inc');
}

//Carrega as funções de controle de acesso
include_once 'controleAcesso.inc';
?>