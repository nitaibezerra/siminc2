<?php
/**
 * Arquivo principal de criação de um novo pedido de recurso financeiro.
 * $Id: novasolicitacao.inc 98794 2015-06-18 14:17:47Z LucasGomes $
 */

/**
 * Funções de apoio às solicitações recurso.
 * @see _funcoessolicitacoesrecursos.php
 */
require(APPRAIZ . 'www/progfin/_funcoessolicitacoesrecursos.php');

/**
 * Abstração de abas do simec.
 * @see Simec_Abas
 */
require(APPRAIZ . 'includes/library/simec/Abas.php');

/**
 * Helper de exibição de alertas entre requisições.
 * @see Simec_Helper_FlashMessage
 */
require APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
$fm = new Simec_Helper_FlashMessage('progfin/novasolicitacao');

if ($_POST['somaLiberacoesAnalise']) {
    $totalAprovados = explode('|', $_POST['totalAprovados']);
    $total=0;
    foreach ($totalAprovados as $valor) {
        $tmpValue = str_replace('.', '', $valor);
        $tmpValue = str_replace(',', '.', $tmpValue);
        $total+=$tmpValue;
    }

    echo 'R$ ' . number_format($total, 2, ',', '.');
    die;
}

if (isset($_GET['comboDD']) && is_numeric($_GET['comboDD'])) {
    inputComboNatDespesaDetalhada((int) $_GET['comboDD'], isset($_GET['nddcod']) ? $_GET['nddcod'] : null);
    echo "<script> $('#nddcod').trigger('chosen:updated').chosen(); </script>";
    die;
}

if ($_GET['lfnid'] && $_GET['id'] && $_GET['action'] == 'delete') {
    deleteItemPedido($_GET['lfnid']);
    $fm->addMensagem('Registro apagado com sucesso!');

    header("Location: progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A&id={$_GET['id']}");
    die;
}

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];

    switch ($requisicao) {
        case 'salvarValoresFinanceiros':
            salvarValoresFinanceiros($_POST);
            break;
        case 'voltarParaListagem':
            $novaURL = preg_replace(
                    '|modulo=[a-z/]+|', 'modulo=principal/solicitacaorecurso/solicitacoes', $_SERVER['REQUEST_URI']
            );
            unset($_SESSION['progfin']['solicitacao']['dados']);
            header("Location: progfin.php?modulo=principal/solicitacaorecurso/solicitacoes&acao=A");
            die();
        case 'salvarPedidoRecursoFinanceiro':
            if (!$_POST['dados']['plfid']) {
                $id = $requisicao($_POST);
            } else {
                $id = $_POST['dados']['plfid'];
            }

            if (validaPedido($_POST)) {
                if ($id && !$_POST['dados']['lfnid']) {
                    $_POST['dados']['plfid'] = $id;
                    salvarItemPedidoRecursoFinanceiro($_POST['dados']);
                    $fm->addMensagem("Solicitação criada com sucesso!");
                } else {
                    alterarItemPedidoRecursoFinanceiro($_POST['dados']);
                    $fm->addMensagem("Solicitação alterada com sucesso!");
                }
            } else {
                $fm->addMensagem("Já existe pedido de recurso financeiro com os seguintes dados iguais: Situação contábil, Cat. Gasto e Vinculação!", Simec_Helper_FlashMessage::ERRO);
            }

            $novaURL = "progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A&id={$id}";
            break;
        case 'alterarPedidoRecursoFinanceiro':
            alterarPedidoRecursoFinanceiro($_POST);
            $fm->addMensagem("Solicitação alterada com sucesso!");
            break;
        case 'salvaAnaliseSolicitacaoRecurso':
            $requisicao($_POST);
            $fm->addMensagem("Análise de Solicitação salva com sucesso!");
            break;
        case 'enviar-liberacoes':
            // -- Atualiza os novos valores de aprovado e atendido
            salvarValoresFinanceiros(
                array('aprovado' => $_POST['aprovado'], 'atendido' => ''),
                $parse = false
            );

            // -- Tramitando as liberações
            tramitarLiberacoes(
                $_POST['lfnid'], $_POST['siafi_usuario'], $_POST['siafi_password'], $_POST['siafi_ug'], $_POST['siafi_comment'], true
            );
            $totalLiberacoes = count($_POST['lfnid']);
            $fm->addMensagem("{$totalLiberacoes} liberação(ões) foi(ram) encaminhada(s) para envio e estão aguardando comunicação.");
            header("Location: {$_SESSION['HTTP_REFERER']}");
            die();
            break;
        case 'drawWorkflow':
            list(, $docid, $plfid) = $_POST['params'];
            wf_desenhaBarraNavegacao($docid, array('docid' => $docid, 'plfid' => $plfid));
            die();
        default:
    }

    // -- Redirecionamento padrão
    if (!isset($novaURL)) {
        $novaURL = $_SERVER['REQUEST_URI'];
    }
    header('Location: ' . $novaURL);
    die();
}

// -- id do pedido
$id = (int)$_GET['id'];

$dados = array();
if ($_GET['lfnid'] && $id && $_GET['action'] == 'edit') {
    $dados = getDadosFormItemPedido($_GET['lfnid']);
    $dados['lfnvalor'] = $dados['lfnvalorsolicitado'];
} else {
    // -- Carregando dados das solicitações
    if (dadosSolicitacaoRecursoNaSessao()) {
        // -- Copia os dados da sessão para a variável
        $dados = &$_SESSION['progfin']['solicitacao']['dados'];
    }
}

// -- carregando dados do pedido
$result = array();
if ($id && is_numeric($id)) {
    $result = getPedidoRecursoFinanceiro($id);
    $dados['unicod'] = $result['unicod'];
}

// -- criação de abas
$baseUrl = 'progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A' . ($id?"&id={$id}":'');
$abas = new Simec_Abas($baseUrl);
$formSolicitacao = ($dados['unicod'] == UNIDADE_FNDE)?'formSolicitacaoFNDE':'formSolicitacao';
$abas->adicionarAba('solicitacao', 'Solicação', dirname(__FILE__) . "/novasolicitacao/{$formSolicitacao}.inc", 'edit')
    ->adicionarAba('analise', 'Análise', dirname(__FILE__) . "/novasolicitacao/formAnalise.inc", 'check')
    ->adicionarCondicaoAba('analise', array(
        array('op1' => $result['esdid'], 'op2' => ESDID_PEDIDO_EM_PREENCHIMENTO, 'op' => 'diferente'),
        array('op1' => $result['esdid'], 'op2' => '', 'op' => 'diferente')
    ))->definirAbaDefault('solicitacao');

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<style type="text/css">
    .control-label,.form-control-static{padding-top:7px;margin-bottom:0}
</style>
<script type="text/javascript" lang="JavaScript">
    $(document).ready(function () {
        $('#lfnvalor').blur();
        $('#lfnvalorautorizado').blur();
        $('#lfnvaloratendido').blur();

        $("#ndscod").on("change", function () {
            if ($(this).val()) {
                var loc = "progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A"
                  , $request = $.ajax({url: loc + "&comboDD=" + $(this).val()});

                $request.done(function (data) {
                    $(".comboDD").html(data);
                });
            }
        });

        if ($("#ndscod").val()) {
            var loc = "progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A"
              , $request = $.ajax({url: loc + "&comboDD=" + $("#ndscod").val() + "&nddcod=<?= $dados['nddcod']; ?>"});

            $request.done(function (data) {
                $(".comboDD").html(data);
            });
        }
    });

    var editarPedido = function (lfnid, id) {
        var endpoint = "progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A";
        location.href = endpoint + "&id=" + id + "&lfnid=" + lfnid + "&action=edit";
    };

    var delPedido = function (lfnid, id) {
        var endpoint = "progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A";
        if (confirm('Deseja realmente apagar esta linha?')) {
            location.href = endpoint + "&id=" + id + "&lfnid=" + lfnid + "&action=delete";
        }
    };

</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="progfin.php?modulo=inicio&acao=C">Programação Financeira</a></li>
        <li><a href="progfin.php?modulo=principal/solicitacaorecurso/solicitacoes&acao=A">Liberações Financeiras</a></li>
        <li class="active"><?php echo (isset($id)?'Recurso Financeiro':'Novo Recurso Financeiro') ?></li>
    </ol>
    <?php echo apresentaNumeroPedido($id); ?>
    <div class="row col-md-12">
    <?php echo $fm->getMensagens(); ?>
        <div class="col-md-11">
        <?php $abas->render(); ?>
        </div>
        <div class="col-md-1">
            <!-- workflow -->
            <?php
            if ($result != null || $result != false) {
                if (array_key_exists('docid', $result) && is_numeric($result['docid'])) {
                    $docid = $result['docid'];
                    $plfid = $result['plfid'];
                    echo wf_desenhaBarraNavegacao($docid, array('docid' => $docid, 'plfid' => $plfid), array('historico' => false));
                }
            }
            ?>
        </div>
    </div>
</div>