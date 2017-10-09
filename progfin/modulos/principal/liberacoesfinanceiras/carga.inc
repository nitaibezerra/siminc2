<?php
/**
 * Arquivo de carga de dados da programação financeira.
 * $Id: carga.inc 102309 2015-09-10 14:29:34Z maykelbraz $
 */

/**
 * Funções de apoio para a liberação financeira.
 * @see _funcoesliberacoes.php
 */
require(APPRAIZ . "www/progfin/_funcoesliberacoes.php");

$fm = new Simec_Helper_FlashMessage('progfin/liberacoes');

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'processarArquivo':
            $resultado = processarArquivoLiberacoes(
                $_FILES['liberacoes'],
                $_POST['dados'],
                $_SESSION['usucpf'],
                $_SESSION['exercicio']
            );
            break;
        case 'carregarErrosDaLiberacao':
        case 'mostraErrosDoItem':
            $lfnid = current($_POST['dados']);
            die(carregarErrosDaLiberacao(array('lfnid' => $lfnid)));
    }

    $fm->addMensagem(
        $resultado['msg'],
        $resultado['sucesso']
            ?Simec_Helper_FlashMessage::SUCESSO
            :Simec_Helper_FlashMessage::ERRO
    );

    // -- Redirecionando conforme resultado da operação
    $novaURL = $_SERVER['REQUEST_URI'];
    if ($resultado['sucesso']) {
        $novaURL = str_replace('aba=arquivo', '', $novaURL);
        $novaURL = "{$novaURL}&aba=detalhes&lote={$resultado['llfid']}";
    }
    header('Location: ' . $novaURL);
    die();
}

// -- Gerenciamento de abas
// -- Identificando a aba ativa
$abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'arquivo');

// -- URL base das abas
$urlBaseDasAbas = 'progfin.php?modulo=principal/liberacoesfinanceiras/carga&acao=A&aba=';

$listaAbas = array();
$listaAbas[] = array("id" => 1, "descricao" => '<span class="glyphicon glyphicon-upload"></span> Enviar arquivo', "link" => "{$urlBaseDasAbas}arquivo");
$listaAbas[] = array('id' => 2, 'descricao' => '<span class="glyphicon glyphicon-time"></span> Importações', 'link' => "{$urlBaseDasAbas}importacoes");
// -- Exibição dos detalhes de uma carga anterior
if ('detalhes' == $abaAtiva) {
    $listaAbas[] = array('id' => 3, 'descricao' => '<span class=" glyphicon glyphicon-eye-open"></span> Detalhes do lote', 'link' => "{$urlBaseDasAbas}detalhes");
}

/**
 * Cabecalho padrão do Sistema.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="progfin.php?modulo=inicio&acao=C">Programação Financeira</a></li>
        <li>Liberações Financeiras</li>
        <li class="active">Gestão de lotes</li>
    </ol>
    <div class="row col-md-12">
    <?php
    echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");
    $arquivo = '';
    switch ($abaAtiva) {
        case 'arquivo':
            $arquivo = 'formCarga';
            break;
        case 'dados':
            $arquivo = 'listarCarga';
            break;
        case 'importacoes':
            $arquivo = 'listarImportacoes';
            break;
        case 'detalhes':
            $arquivo = 'listarDetalhes';
            break;
    }
    // -- Exibindo mensagens do sistema
    echo $fm->getMensagens();
    require(dirname(__FILE__) . "/carga/{$arquivo}.inc");
    ?>
    </div>
</div>