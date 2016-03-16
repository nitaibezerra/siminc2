<?php
/**
 * Faz o gerenciamento dos momentos de crédito.
 * $Id: inicio.inc 102362 2015-09-11 19:01:00Z maykelbraz $
 */

/**
 * Funções de apoio ao gerenciamento de momentos de crédito.
 * @see _funcoesmomentocredito.php
 */
require APPRAIZ . "www/altorc/_funcoesmomentocredito.php";

if (chaveTemValor($_POST, 'requisicao')) {
    $fm = new Simec_Helper_FlashMessage('altorc/momentocredito');

    switch ($_POST['requisicao']) {
        case 'apagarMomentoCredito':
            if (apagarMomentoCredito($_POST['dados'])) {
                $fm->addMensagem('O momento selecionado foi removido com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível remover o momento selecionado.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'salvarMomentoCredito':
            if (salvarMomentoCredito($_POST['dados'], $_SESSION['exercicio'])) {
                $fm->addMensagem('O momento selecionado foi alterado com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível alterar o momento selecionado.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'editarMomentoCredito':
            $formData = carregarDadosDoMomentoDeCredito($_POST['dados']);
            // -- Dados do pedido não encontrados
            if (empty($formData)) {
                $fm->addMensagem('Não foi possível carregar os dados do momento selecionado.', Simec_Helper_FlashMessage::ERRO);
                header('Location: ' . $_SERVER['REQUEST_URI']);
                die();
            }
            break;
        case 'novoMomentoCredito':
            if (salvarMomentoCredito($_POST['dados'], $_SESSION['exercicio'])) {
                $fm->addMensagem('O momento selecionado foi alterado com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível alterar o momento selecionado.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'buscarMomentoCredito':
            $filtrar = false;
            foreach ($_POST['dados'] as $dado) {
                if ('' != $dado) {
                    $filtrar = true;
                    break;
                }
            }
            if ($filtrar) {
                $_SESSION['altorc']['momentocredito']['busca'] = $_POST['dados'];
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'limparBusca':
            unset($_SESSION['altorc']['momentocredito']['busca']);
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
    }
}

if (isset($_SESSION['altorc']['momentocredito']['busca'])) {
    $formData = $_SESSION['altorc']['momentocredito']['busca'];
}

/**
 * Cabeçalho simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<style type="text/css"></style>
<script language="javascript" type="text/javascript"></script>
<div class="row col-md-12">
<!--    <div class="page-header">
        <h4>Momentos de crédito</h4>
    </div>-->
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Momentos de crédito</li>
    </ol>
    <div class="well col-md-12">
        <?php require dirname(__FILE__) . '/formMomentoCredito.inc'; ?>
    </div>
    <?php
    $fm = new Simec_Helper_FlashMessage('altorc/momentocredito');
    echo $fm->getMensagens();
    ?>
    <div class="col-md-12">
        <?php
        if (!(isset($_POST['requisicao']) && ('editarMomentoCredito' == $_POST['requisicao']))) {
            require dirname(__FILE__) . '/listarMomentoCredito.inc';
        }
        ?>
    </div>
</div>

