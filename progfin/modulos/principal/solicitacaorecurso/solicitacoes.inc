<?php
/**
 * Arquivo principal da solicitação de recursos financeiros.
 * $Id: solicitacoes.inc 102309 2015-09-10 14:29:34Z maykelbraz $
 */

/**
 * Funções de apoio às solicitações recurso.
 * @see _funcoessolicitacoesrecursos.php
 */
require(APPRAIZ . 'www/progfin/_funcoessolicitacoesrecursos.php');

/**
 * Helper de exibição de alertas entre requisições.
 * @see Simec_Helper_FlashMessage
 */
require APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
$fm = new Simec_Helper_FlashMessage('progfin/solicitacoes');

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'criarNovaSolicitacao':
            dadosNovaSolicitacaoRecurso($_REQUEST['dados']);
            // -- Redirecionando para a criação da nova solicitação de recurso financeiro
            $novaURL = preg_replace(
                '|modulo=[a-z/]+|',
                'modulo=principal/solicitacaorecurso/novasolicitacao',
                $_SERVER['REQUEST_URI']
            );
            header("Location: {$novaURL}");
            die();
        case 'limparFiltros':
            unset($_SESSION['progfin']['solicitacao']['dados']);
            header("Location: progfin.php?modulo=principal/solicitacaorecurso/solicitacoes&acao=A");
            die();
        case 'deletaPedido':
            echo deletePedido($_POST['id']);
            die();
        case 'busca':
        default:
    }

    // -- Redirecionando conforme resultado da operação
    $novaURL = $_SERVER['REQUEST_URI'];
    header('Location: ' . $novaURL);
    die();
}

// -- Carregando dados das solicitações
$dados = array();
if (dadosSolicitacaoRecursoNaSessao()) {
    // -- Copia os dados da sessão para a variável
    $dados = &$_SESSION['progfin']['solicitacao']['dados'];
}
if (!$dados) {
    $dados['unicod'] = $_REQUEST['unicod'];
    $dados['ungcod'] = $_REQUEST['ungcod'];
    $dados['trccod'] = $_REQUEST['trccod'];
    $dados['fdsid'] = $_REQUEST['fdsid'];
    $dados['ctgcod'] = $_REQUEST['ctgcod'];
    $dados['elmcod'] = $_REQUEST['elmcod'];
    $dados['clpid'] = $_REQUEST['clpid'];
}

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<style type="text/css">
label{margin-top:10px;cursor:pointer}
</style>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="progfin.php?modulo=inicio&acao=C">Programação Financeira</a></li>
        <li class="active">Liberações Financeiras</li>
    </ol>
    <div class="well">
        <?php require dirname(__FILE__) . '/solicitacoes/formSolicitacoes.inc'; ?>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <div>
        <?php require dirname(__FILE__) . '/solicitacoes/listarSolicitacoes.inc'; ?>
    </div>
</div>