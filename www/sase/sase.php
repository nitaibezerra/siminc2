<?php

//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// $_SESSION['sisid'] = '';

include_once APPRAIZ . 'includes/workflow.php';
// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';
include_once APPRAIZ . "www/autoload.php";

if( strpos($_SERVER['REQUEST_URI'],'sistema') || strpos($_SERVER['REQUEST_URI'],'inicio') )
echo '<script language="JavaScript" src="../includes/funcoes.js"></script>';


simec_magic_quotes();
/**
 * @TODO Tratamento para colocar o layout antigo nas telas de sistemas que não tem o jquery compativel ainda com o layout novo
 */
$arrModulo = explode( '/', $_GET['modulo']);
$modulo = reset($arrModulo);
if(!empty($modulo) && $modulo == 'sistema'){
   $_SESSION['sislayoutbootstrap'] = false; 
} else {
   $_SESSION['sislayoutbootstrap'] = true; 
}




//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";

