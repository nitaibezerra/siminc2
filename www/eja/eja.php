<?php
//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_componentes.php';

/**
 * @TODO Tratamento para colocar o layout antigo nas telas de sistemas que não tem o jquery compativel ainda com o layout novo
 */
#BLOCO DE CÓDIGO USADO APENAS PARA O RELATÓRIO. MDIDA EMERGENCIAL PARA SER A ESTRUTURA ENTIGA NO RELATÓRIO.
$arrModulo = explode( '/', $_GET['modulo']);
$modulo = reset($arrModulo);

if(!empty($modulo) && ( $modulo == 'sistema' || $modulo == 'relatorio') ){
    $_SESSION['sislayoutbootstrap'] = false; 
} else {
    $_SESSION['sislayoutbootstrap'] = true; 
}
#FIM DE BLOCO.

//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";

?>