<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

include_once APPRAIZ . "includes/classes/Modelo.class.inc";
include_once APPRAIZ . "includes/funcoesspo.php";
require_once APPRAIZ . 'www/includes/webservice/PessoaJuridicaClient.php';

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

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
?>

<script type="application/javascript" src="js/funcoes.js"></script>
<script type="text/javascript" src="/estrutura/js/funcoes.js"></script>