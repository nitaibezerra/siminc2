<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";
//echo '<pre>';
//var_dump(defined('APPRAIZ_ZEND'));
//var_dump(APPRAIZ_ZEND);
//var_dump(array_shift(debug_backtrace()));
//exit;
// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

include_once APPRAIZ . 'includes/library/simec/Crud/Listing.php';
include_once '_autoload.php';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>