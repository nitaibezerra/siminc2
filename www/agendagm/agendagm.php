<?php
//Carrega parametros iniciais do simec
    include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

initAutoload();

//Carrega as funções de controle de acesso
$_SESSION['sislayoutbootstrap'] = 'zimec';

require_once APPRAIZ . 'includes/library/simec/view/Helper.php';
$simec = new Simec_View_Helper();

include_once "controleAcesso.inc";
?>