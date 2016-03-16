<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
require_once APPRAIZ . 'includes/classes/Modelo.class.inc';
require_once APPRAIZ . 'fabrica/classes/autoload.inc';
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
 

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>