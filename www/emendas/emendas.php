<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
require_once APPRAIZ . 'includes/funcoesspo.php';

$simec = new Simec_View_Helper();
$_SESSION['sislayoutbootstrap'] = 'zimec';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>
