<?php
/**
 * Arquivo de controle a funcionalidade de detalhamento da gestão da PLOA.
 * $Id: detalhamento.inc 101175 2015-08-12 15:00:57Z maykelbraz $
 */

/**
 * Funções de apoio à gestão da ploa.
 * @see _funcoesgestaoploa.php
 */
require(APPRAIZ . "www/{$_SESSION['sisdiretorio']}/_funcoesgestaoploa.php");

// -- Carregando dados da gestão
$dados = &dadosDaSessao('gestaoploa');
// -- Gerenciador de mensagens

$fm = new Simec_Helper_FlashMessage("{$_SESSION['sisdiretorio']}/gestaoploa");

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $tipoRequisicao = 'normal';
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'alterarFinanceiro':
            $params = array($_POST['dados'], $_SESSION['usucpf'], $dados);
            break;
        case 'novoFinanceiro':
            $params = array($_POST['dados'], $_SESSION['usucpf'], $dados);
            break;
        case 'detalharFinanceiro':
            $params = array($_POST['dados'], true);
            $tipoRequisicao = 'ajax-html';
            break;
        case 'excluirFinanceiro':
            $params = array($_POST['dados'], $dados);
            break;
        default: ver($_POST, d);
    }

    // -- Processamento da requisição
    $result = call_user_func_array($requisicao, $params);

    // -- Tipo de processamento
    switch ($tipoRequisicao) {
        case 'normal':
            if (!$result['sucesso']) {
                $fm->addMensagem($result['mensagem'], Simec_Helper_FlashMessage::ERRO);
            } else {
                $fm->addMensagem('Sua solicitação foi executada com sucesso.');
            }

            // -- Redirecionamento padrão
            $novaURL = $_SERVER['REQUEST_URI'];
            header('Location: ' . $novaURL);
            break;
        case 'ajax-html':
            echo $result;
            break;
        case 'ajax-json':
            echo simec_json_encode($result);
            break;
    }
    die();
}

// -- Indica se os dados dos formulários da tela podem ser editados
$podeEditarDados = podeEditar($dados['acao']['esdid']);
/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<style type="text/css">
label{margin-top:10px;cursor:pointer}
</style>
<script type="text/javascript">
$(function(){
    $('[data-toggle="popover"]').popover({
        trigger: 'hover',
        placement: 'top'
    });
    $('#mostrar-acoes').click(function(){
        var url = '/proporc/proporc.php?modulo=principal/dadosacoes/detalhardadosacoes&acao=A&unicod='
                + $(this).attr('data-uo') + '&acacod=' + $(this).attr('data-acao');

        window.open(url, '_blank');
    });
});
</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <?php if (exibirSeletorDeUO()): ?>
        <li><a href="<?php echo MODULO; ?>.php?modulo=principal/gestaoploa/inicio&acao=A">Seleção de UO</a></li>
        <?php endif; ?>
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=principal/gestaoploa/limites&acao=A">Limites Orçamentários</a></li>
        <li class="active">Detalhamento de despesas</li>
    </ol>
    <div class="col-md-offset-2 col-md-8">
        <div class="panel panel-primary">
            <div class="panel-heading"><strong>PLOA - Limites Orçamentários</strong></div>
            <table class="table">
                <tbody>
                    <tr>
                        <td><strong>Unidade Orçamentária:</strong></td>
                        <td>
                            <?php echo "{$dados['unicod']} - {$dados['unidsc']}"; ?>
                        </td>
                        <td><strong>Programa:</strong></td>
                        <td style="text-align:left">
                            <?php echo "{$dados['acao']['prgcod']} - {$dados['acao']['prgdsc']}"; ?>
                        </td>
                    </tr>
                    <tr>
                        <td><strong>Ação:</strong></td>
                        <td style="text-align:left">
                            <button class="btn btn-xs btn-info" data-toggle="popover"
                                    title="Detalhamento de ações" id="mostrar-acoes"
                                    data-content="Verifique aqui mais informações sobre esta Ação em <?php echo $_SESSION['exercicio']; ?>"
                                    data-uo="<?php echo $dados['unicod']; ?>"
                                    data-acao="<?php echo $dados['acao']['acacod']; ?>">
                                <span class="glyphicon glyphicon-eye-open"></span>
                            </button>
                        <?php echo "{$dados['acao']['acacod']} - {$dados['acao']['acadsc']}"; ?>
                        </td>
                        <td><strong>Localizador:</strong></td>
                        <td style="text-align:left">
                            <?php echo "{$dados['acao']['loccod']} - {$dados['acao']['locdsc']}"; ?>
                        </td>
                    </tr>
                    <tr>
                        <td><strong>Grupo:</strong></td>
                        <td style="text-align:left"><?php echo $dados['gpmdsc']; ?></td>
                        <td><strong>Coluna:</strong></td>
                        <td style="text-align:left"><?php echo $dados['mtrdsc']; ?></td>
                    </tr>
                    <tr>
                        <td nowrap><strong>Limite (R$):</strong></td>
                        <td style="text-align:left">
                            <input type="hidden" id="limitecoluna" value="<?php echo $dados['mtmvlrlimite']; ?>" />
                            <?php echo mascaraNumero($dados['mtmvlrlimite']); ?>
                        </td>
                        <td nowrap><strong>Saldo (R$):</strong></td>
                        <td style="text-align:left">
                            <input type="hidden" id="saldocoluna" value="<?php echo $dados['saldocoluna']; ?>" />
                            <?php echo mascaraNumero($dados['saldocoluna']); ?>
                        </td>
                    </tr>
                    <tr>
                        <td><strong>Estado da proposta:</strong></td>
                        <td style="text-align:left">
                            <?php echo "{$dados['acao']['esddsc']}"; ?>
                        </td>
                        <td>&nbsp;</td>
                        <td>&nbsp;</td>
                    </tr>
                </tbody>
            </table>
        </div>
    </div>
    <hr style="clear:both" />
    <?php echo $fm->getMensagens(); ?>
    <div class="col-md-12 row">
    <?php require dirname(__FILE__) . "/detalhamento/listarFinanceiro.inc"; ?>
    </div>
</div>