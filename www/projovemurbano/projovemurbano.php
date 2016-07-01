<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

simec_magic_quotes();

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>