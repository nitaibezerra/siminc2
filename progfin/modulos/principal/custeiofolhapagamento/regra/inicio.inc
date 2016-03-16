<?php
/**
 * Faz o gerenciamento dos momentos de crédito.
 * $Id: inicio.inc 102309 2015-09-10 14:29:34Z maykelbraz $
 */

/**
 * Funções de apoio ao gerenciamento de momentos de crédito.
 * @see _funcoesregrascargacusteiofolhapagamento.php
 */
require APPRAIZ . "www/progfin/_funcoesregrascargacusteiofolhapagamento.php";


$fm = new Simec_Helper_FlashMessage('progfin/custeiofolhapagamento/regra');

if (chaveTemValor($_POST, 'requisicao')) {

    switch ($_POST['requisicao']) {
        case 'apagarRegra':
            if (apagarRegra($_POST['dados'])) {
                $fm->addMensagem('A regra selecionado foi removida com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível remover a regra.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'salvarRegra':
            if (salvarRegra($_POST['dados'])) {
                $fm->addMensagem('A regra selecionada foi alterado com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível alterar a regra selecionada.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'editarRegra':
            $formData = carregarDadosRegras($_POST['dados']);
            // -- Dados do pedido não encontrados
            if (empty($formData)) {
                $fm->addMensagem('Não foi possível carregar os da regra selecionada.', Simec_Helper_FlashMessage::ERRO);
                header('Location: ' . $_SERVER['REQUEST_URI']);
                die();
            }
            break;
        case 'novoRegra':
            if (salvarRegra($_POST['dados'], $_SESSION['exercicio'])) {
                $fm->addMensagem('Nova regra inserida com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível inserir a nova regra.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'buscarRegra':
            $filtrar = false;
            foreach ($_POST['dados'] as $dado) {
                if ('' != $dado) {
                    $filtrar = true;
                    break;
                }
            }
            if ($filtrar) {
                $_SESSION['progfin']['regrascargacusteiofolhapagamento']['busca'] = $_POST['dados'];
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'limparBusca':
            unset($_SESSION['progfin']['regrascargacusteiofolhapagamento']['busca']);
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
    }
}

if (isset($_SESSION['progfin']['regrascargacusteiofolhapagamento']['busca'])) {
    $formData = $_SESSION['progfin']['regrascargacusteiofolhapagamento']['busca'];
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
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="active">Regras para processamento</li>
    </ol>
    <div class="well col-md-12">
        <?php require dirname(__FILE__) . '/formRegra.inc'; ?>
    </div>
    <?php
    echo $fm->getMensagens();
    ?>
    <div class="col-md-12">
        <?php
        if (!(isset($_POST['requisicao']) && ('editarRegra' == $_POST['requisicao']))) {
            require dirname(__FILE__) . '/listarRegra.inc';
        }
        ?>
    </div>
</div>

