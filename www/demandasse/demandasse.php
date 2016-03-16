<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

include_once APPRAIZ . "includes/workflow.php";

include_once APPRAIZ . "includes/classes/Modelo.class.inc";

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

//$abasArray = array(
$_SESSION['demandasse']['abas_array'] = array(
        array('link' => 'demandasse.php?modulo=principal/procedencia&acao=A', 'descricao' => 'Procedência') ,
        array('link' => 'demandasse.php?modulo=principal/documento&acao=A', 'descricao' => 'Documento')
    );

$_SESSION['demandasse']['url'] = $url;

include_once APPRAIZ . "includes/classes/file.class.inc";
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

/**
 * Classe que gera graficos.
 */
include_once APPRAIZ . "includes/library/simec/Grafico.php";

/**
 * Classe de listagem.
 */
include_once APPRAIZ . "includes/library/simec/Crud/Listing.php";

/**
 * Classe para carregar as classes em mvc.
 */
include_once '_autoload.php';

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";
?>