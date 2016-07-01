<?php
/**
 * Criação/Edição de novo pedido de alteração orçamentária.
 * $Id: inicio.inc 96859 2015-05-04 20:39:09Z maykelbraz $
 */

/**
 * Funções de apoio ao pedido de alteração orçamentária.
 * @see _funcoesalteracaoorcamentaria.php
 */
require APPRAIZ . "www/altorc/_funcoesalteracaoorcamentaria.php";

// -- Validação da sessão, que as vezes está quebrando
if (!session_id()) {
    header('Loaction: altorc.php?modulo=inicio&acao=C');
    die();
}

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('altorc/pedido');
// -- Momento de crédito atual - para limitação de permissões
list($mcridAtual, $mcrtipocancelamento) = momentoDeCreditoAtual($_SESSION['exercicio'], array(), true);

// -- Carregando informações do pedido
$dadospao = array();
if (dadosAlteracaoOrcamentariaNaSessao() // -- 1: Dados já armazenados na sessão
    // -- 2: Pedido já criado, mas dados não armazenados na sessão OU novo pedido
    || dadosDoPedidoAlteracaoOrcamentaria($_REQUEST['dados'])) {
    // -- Copia os dados da sessão para a variável
    $dadospao = &$_SESSION['altorc']['pedido']['dados'];
    // -- Verificando se um novo pedido pode ser criado
    if (('novoPedido' == $_REQUEST['requisicao']) && ($_REQUEST['dados']['mcrid'] != $mcridAtual)) {
        $fm->addMensagem(
            'Não é possível criar novos pedidos para o momento de crédito escolhido, pois o este momento não está aberto.',
            Simec_Helper_FlashMessage::ERRO
        );
        header('Location: altorc.php?modulo=principal/pedido&acao=A');
        die();
    }
} else {
    $fm->addMensagem(
        'Os dados do pedido não foram encontrados.',
        Simec_Helper_FlashMessage::ERRO
    );
    header('Location: altorc.php?modulo=principal/pedido&acao=A');
    die();
}

// -- Processamento de requisições
// -- todo: fazer redirecionamento pra tirar mensagem de reenviar formulário
if (chaveTemValor($_POST, 'requisicao')) {
    switch ($_POST['requisicao']) {
        case 'programas':
            echo carregarProgramas($_POST['dados']['acacod'], $_SESSION['exercicio'], true, $_POST['dados']['unicod']);
            die();
        case 'salvarFisico':
            // -- Tentando salvar as alterações físicas do localizador
            if (salvarFisicoDoLocalizador($dadospao, $_POST['valacrescimo'], $_POST['valreducao'])) {
                $fm->addMensagem('Alteração do físico realizada com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível executar a alteração do físico.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'alterarPO':
            // -- Tentando salvar as alterações do PO
            if (salvarFinanceiroDoPO($_POST['dados'], $dadospao)) {
                $fm->addMensagem('Alteração do financeiro realizada com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível executar a alteração do financeiro.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'criarPO';
            // -- Tentando salvar as alterações do PO
            if (salvarFinanceiroDoPO($_POST['dados'], $dadospao, 'N')) {
                $fm->addMensagem('Nova entrada financeira criada com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível criar a nova entrada financeira.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'salvarJustificativas':
            if (salvarJustificativas($_POST['just'], $dadospao['paoid'], $_SESSION['usucpf'], $_SESSION['usunome'])) {
                $fm->addMensagem('As justificativas foram salvas com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível salvar as justificativas.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'excluirCredito':
            if (excluirCredito($_POST['dados'])) {
                $fm->addMensagem('O crédito selecionado foi excluído com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível excluir o crédito selecionado.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'limparCredito':
            if (limparCredito($_POST['dados'])) {
                $fm->addMensagem('O crédito selecionado foi limpo com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível limpar o crédito selecionado.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'limparFuncional':
            if (limparFuncional($_POST['dados'])) {
                $fm->addMensagem('O localizador selecionado foi limpo com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível limpar o localizador selecionado.', Simec_Helper_FlashMessage::ERRO);
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'detalharLocalizador':
            $dadospao['snaid'] = $_POST['dados'][0];
            $dadospao['paoid'] = $_POST['dados'][1];
            $detalharPO = true;
            require(dirname(__FILE__) . '/detalhamento/listarAlteracoesPO.inc');
            die();
        case 'alterarTipoCredito':
            if(alterarTipoCredito($_REQUEST['dados']['paoid'], $_REQUEST['dados']['tcrid'])){
                $fm->addMensagem ('Tipo de crédito alterado com sucesso.');
                dadosDoPedidoAlteracaoOrcamentaria($dadospao);
            }else
                $fm->addMensagem ('Não foi possível alterar o tipo de crédito.',  Simec_Helper_FlashMessage::ERRO);

            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
    }
// -- Requisição de edição de um localizador
} elseif (chaveTemValor($_REQUEST, 'dados') && chaveTemValor($_REQUEST['dados'], 'snaid')) {
    $_SESSION['altorc']['pedido']['dados']['snaid'] = $_REQUEST['dados']['snaid'];
    header('Location: ' . substr($_SERVER['REQUEST_URI'], 0, strrpos($_SERVER['REQUEST_URI'], '&')));
    die();
} elseif (chaveTemValor($_REQUEST, 'target')) {
    header('Location: ' . substr($_SERVER['REQUEST_URI'], 0, strrpos($_SERVER['REQUEST_URI'], '?'))
        . '?modulo=principal/pedido/inicio&acao=A&aba=resumo#collapseSIOP');
    die();
}

// -- Calculo de suplementações:
$dadosfinanceiro = suplementacoes($dadospao['paoid']);
$podeSalvar = podeAlterarPedido($dadospao['paoid']);
/**
 * Cabeçalho simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<link rel="stylesheet" href="/proporc/css/print.css" media="print" />
<style type="text/css">
label{margin-top:10px}
.page-header{margin-bottom:3px;padding-bottom:2px;font-weight:bold}
</style>
<script type="text/javascript" language="javascript">
$(document).ready(function(){
    $('#formularioAlterarTipoCredito').css('display','none');
    $('#tcrid_chosen').css('width','100%');
    $('#btnVoltar').click(function(){
        window.location = 'altorc.php?modulo=principal/pedido&acao=A';
    });
    $('#modal-alert .modal-dialog').css('width', '80%');
});
function detalharRetornoSIOP(paoid)
{
    $.post('altorc.php?modulo=principal/pedido&acao=A', {'dados[paoid]':paoid,'requisicao':'retornoSIOP'}, function(data){
        $('#modal-alert .modal-title').text('Retorno SIOP');
        $('#modal-alert .modal-body').html(data);
        $('#modal-alert').modal();
    });
}
function alterarTipoCredito()
{
    $('#modal-confirm .modal-body p').html($('#formularioAlterarTipoCredito').css('display','inherit'));
    $('.modal-dialog').css('width','70%');
    $('#modal-confirm .modal-title').html('Alterar Tipo de Crédito - Pedido n° <?php echo $dadospao['paoid'] .' - '. $dadospao['paodsc']; ?>');
    $('#modal-confirm .btn-primary').html('Salvar');
    $('#modal-confirm .btn-primary').attr('onclick','atualizarTipoCredito();');
    $('#modal-confirm .btn-primary').attr('type','button');
    $('#modal-confirm .btn-default').html('Fechar');
    $('.modal-dialog').show();
    $('#modal-confirm').modal();
}

function atualizarTipoCredito()
{
    if($('#tcrid').val() == ""){
        alert('Você deve selecionar um Tipo de Crédito para prosseguir.');
    }else{
        $('#formularioAlteraTipoCredito').val('alterarTipoCredito');
        $('#formularioAlteraTC').submit();
    }
}
</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo $_SESSION['sisdiretorio']; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <li><a id="btnVoltar" href="#">Pedidos</a></li>
        <li class="active">Visualização de pedido</li>
    </ol>
    <!-- INÍCIO FORMULÁRIO MODAL ALTERAR TIPO DE CRÉDITO -->
    <div id="formularioAlterarTipoCredito">
        <form class="form-horizontal" method="post" id="formularioAlteraTC" action="">
            <input type="hidden" name="requisicao" id="formularioAlteraTipoCredito" value="" />
            <input type="hidden" name="dados[paoid]" value="<?=$dadospao['paoid']  ?>" />
            <section class="form-group">
                <label class="control-label col-md-2">Tipo de Crédito</label>
                <section class="col-md-10">
                    <?php
                    $opcoesTcrid = array();
                    $opcoesTcrid = carregarTipoCredito($_SESSION['exercicio'], $dadospao['mcrid'], $asJSON = false);
                    $db->monta_combo('dados[tcrid]', $opcoesTcrid, 'S', 'Selecione um Tipo de crédito', null, null, null, null, 'N', 'tcrid', null, $dadospao['tcrid'], null, 'class="form-control chosen-select"');
                    ?>
                </section>
            </section>
        </form>
    </div>
    <!-- FIM -- FORMULÁRIO MODAL ALTERAR TIPO DE CRÉDITO -->
    <div class="col-md-10 col-md-offset-1">
        <div class="panel panel-primary">
            <div class="panel-heading">
                <strong>
                <?php if (!isset($dadospao['paoid'])) {
                    echo 'Informações do novo pedido';
                } else {
                    echo "Informações do pedido nº {$dadospao['paoid']} - {$dadospao['paodsc']}";
                }
                ?>
                </strong>
            </div>
            <table class="table">
                <tr>
                    <td><strong>Momento de crédito:</strong></td>
                    <td style="width:60%"><?php echo $dadospao['mcrdsc']; ?></td>
                    <td><strong>IUSIOP:</strong></td>
                    <td><?php echo $dadospao['siopid']; ?></td>
                </tr>
                <tr>
                    <td><strong>Unidade Orçamentária:</strong></td>
                    <td><?php echo $dadospao['unidsc']; ?></td>
                    <td><strong>Status:</strong></td>
                    <td><?php echo statusNoSIOP($dadospao['paostatus'], array('paoid2' => $dadospao['paoid'])); ?></td>
                </tr>
                <tr>
                    <td><strong>Tipo de crédito:</strong></td>
                    <td><?php echo $dadospao['tcrcod'] . ' - '. $dadospao['tcrdsc']; ?></td>
                    <td colspan="2" style="text-align:center">
                    <? if (isset($_REQUEST['dados']['paoid'])): ?>
                        <button type="button" class="btn btn-info" onclick="alterarTipoCredito();">
                            <span class="glyphicon glyphicon-edit"></span> Alterar tipo de Crédito
                        </button>
                    <? endif; ?>
                    <? if (isset($_REQUEST['aba']) && 'resumo' == $_REQUEST['aba']): ?>
                        <button type="button" class="btn btn-success notprint" id="print-resumo">
                            <span class="glyphicon glyphicon-print"></span> Imprimir
                        </button>
                    <?php endif; ?>
                    </td>
                </tr>
            </table>
            <div class="panel-heading">
                <table style="color:whitesmoke;width:100%">
                    <tr>
                        <td style="width:16%"><strong>Sup. Cancelamento:</strong></td>
                        <td style="width:16%"><?php echo $dadosfinanceiro['supcancelamento']; ?></td>
                        <td style="width:16%"><strong>Cancelamento:</strong></td>
                        <td style="width:16%"><?php echo $dadosfinanceiro['cancelamento']; ?></td>
                        <td style="width:16%"><strong>Diferença:</strong></td>
                        <td style="width:16%"><?php echo $dadosfinanceiro['diferenca']; ?></td>
                    </tr>
                    <tr>
                        <td><strong>Sup. Excesso:</strong></td>
                        <td><?php echo $dadosfinanceiro['supexcesso']; ?></td>
                        <td><strong>Sup. Superavit:</strong></td>
                        <td><?php echo $dadosfinanceiro['supsuperavit']; ?></td>
                        <td><strong>Total Suplementado:</strong></td>
                        <td><?php echo $dadosfinanceiro['suptotal']; ?></td>
                    </tr>
                    <tr>
                        <td><strong>Sup. Operação Crédito:</strong></td>
                        <td><?php echo $dadosfinanceiro['supopcredito']; ?></td>
                        <td>&nbsp;</td>
                        <td>&nbsp;</td>
                        <td>&nbsp;</td>
                        <td>&nbsp;</td>
                    </tr>
                </table>
            </div>
        </div>
    </div>
    <br style="clear:both" class="noprint" />
<?php
// -- Identificando a aba ativa
$abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'funcionais');
// -- Limpando snaid, caso a aba funcionais seja acessada (para esconder a aba detalhamento)
if ('detalhamento' != $abaAtiva) {
    unset($dadospao['snaid']);
}

// -- URL base das abas
$urlBaseDasAbas = '/altorc/altorc.php?modulo=principal/pedido/inicio&acao=A&aba=';

$listaAbas = array();
// -- @todo: Exibir a aba de detalhamento apenas quando um localizador for selecionado
// -- abafuncionais, sempre visivel
$listaAbas[] = array("id" => 1, "descricao" => "Funcionais", "link" => "{$urlBaseDasAbas}funcionais");

// -- Exibe a aba de detalhamento, apenas se um pedido de localizador estiver sendo editado ou criado
if (isset($dadospao['snaid'])) {
    $listaAbas[] = array("id" => 2, "descricao" => "Detalhamento", "link" => "{$urlBaseDasAbas}detalhamento");
}
// -- Se já existir um pedido (ou seja, ao menos um localizador alterado) exibe as abas de justificativa e resumo
if (isset($dadospao['paoid'])) {
    $listaAbas[] = array("id" => 3, "descricao" => "Justificativas", "link" => "{$urlBaseDasAbas}justificativa");
    $listaAbas[] = array("id" => 4, "descricao" => "Resumo / Trâmite", "link" => "{$urlBaseDasAbas}resumo");
}

// -- HTML das abas
echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");

/**
 * Incluíndo o arquivo de acordo com a aba selecionada.
 * Para cada aba, é chamado o arquivo início dentro do diretório
 * com o mesmo nome da aba selecionada.
 */
require_once(dirname(__FILE__) . "/{$abaAtiva}/inicio.inc");
?>
</div>