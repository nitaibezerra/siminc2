<?php

//Carrega parametros iniciais do simec
include_once "controleInicio.inc";

include_once APPRAIZ . 'includes/workflow.php';
// carrega as funções específicas do módulo
include_once '_constantes.php';
include_once '_funcoes.php';
include_once '_funcoes_obras_par.php';
include_once '_componentes.php';
include_once APPRAIZ . "www/autoload.php";

simec_magic_quotes();

if ($db->testa_superuser()) {
    $painelCabecalho = array(
        array('titulo' => 'WorkFlow', 'funcao' => 'montarPainelWorkflow', 'icon' => 'tasks'),
    );
}


//Carrega as funções de controle de acesso
include_once "controleAcesso.inc";

$aPaginas = array('principal/listaObras', 'principal/inicioLista');
if(in_array($_REQUEST['modulo'], $aPaginas)){
    exibirAvisoPendencias($_SESSION['usucpf']);
}

prepararDetalheProcesso();
prepararDetalhePendenciasObras();
?>

