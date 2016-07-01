<?php
/**
 * Arquivo inicial da programação orçamentária - listagem de pedidos de alteração orçamentária.
 * $Id: pedido.inc 102362 2015-09-11 19:01:00Z maykelbraz $
 */

/**
 * Funções de apoio ao pedido de alteração orçamentária.
 * @see _funcoesalteracaoorcamentaria.php
 */
require APPRAIZ . "www/altorc/_funcoesalteracaoorcamentaria.php";

// -- Removendo os dados do pedido da sessão
limpaDadosDoPedido();

// -- Removendo os dados de formulário/listagem da sessão
if(isset($_GET['clean'])){
    unset($_SESSION['altorc']['filtro_params']);
    unset($_SESSION['altorc']['filtro_pedido']);
}


// -- Processamento de requisições
if (chaveTemValor($_REQUEST, 'requisicao')) {
    switch ($_REQUEST['requisicao']) {
        case 'tipoCredito':
            echo carregarTipoCredito($_SESSION['exercicio'], $_POST['dados']['mcrid']);
            break;
        case 'retornoSIOP':
            $dadospao['paoid'] = $_REQUEST['dados']['paoid'];
            require_once(dirname(__FILE__) . '/pedido/resumo/listarErros.inc');
            /*echo detalharSIOP($_POST['dados']);*/
            break;
        case 'enviarPedido':
            enviarPedido($_REQUEST['dados']['paoid'], $_SESSION['exercicio']);
            header('Location: ' . substr($_SERVER['REQUEST_URI'], 0, strrpos($_SERVER['REQUEST_URI'], '&requisicao')));
            die();
        case 'apagarPedido':
            echo apagarPedido($_REQUEST['dados'], $_SESSION['exercicio'], $_SESSION['usucpf']);
            header('Location: ' . substr($_SERVER['REQUEST_URI'], 0, strrpos($_SERVER['REQUEST_URI'], '&requisicao')));
            die();
    }
    // -- todo: fazer redirecionamento pra tirar mensagem de reenviar formulário
    die();
}

// -- Consultando perfis do usuários
$perfis = pegaPerfilGeral();

// -- Momento de crédito atual - para limitação de permissões
list($mcridAtual, $mcrtipocancelamento) = momentoDeCreditoAtual($_SESSION['exercicio'], array(), true);

/**
 * Cabeçalho simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<style type="text/css">
label{margin-top:10px;cursor:pointer}
</style>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li class="">Alteração de Crédito</li>
        <li class="active">Alteração Orçamentária - Pedidos</li>
    </ol>
    <div class="well col-md-12">
        <?php require dirname(__FILE__) . '/formPedidos.inc'; ?>
    </div>
    <?php
    $fm = new Simec_Helper_FlashMessage('altorc/pedido');
    echo $fm->getMensagens();
    ?>
    <div class="col-md-12">
        <?php require dirname(__FILE__) . '/listarPedidos.inc'; ?>
    </div>
</div>