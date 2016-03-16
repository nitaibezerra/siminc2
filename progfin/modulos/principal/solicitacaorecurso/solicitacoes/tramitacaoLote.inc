<?php
/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
/**
 * Funções de apoio às solicitações recurso.
 * @see _funcoessolicitacoesrecursos.php
 */
require(APPRAIZ . 'www/progfin/_funcoessolicitacoesrecursos.php');

//Declaração de objetos
$fm = new Simec_Helper_FlashMessage('progfin/tramitacaoLote');

if ($_GET['exportar'] == 'xls') {
    tramitacaoLoteListagem();
    exit;
}

if (count($_POST['dados']['plfid'])) {
    require_once APPRAIZ . 'includes/workflow.php';
    global $db;

    $qtd = 0;
    $_SESSION['_progfin_']['_destinatarios_'][] = array();
    /* Enviando os PEDIDOS */
    foreach ($_POST['dados']['plfid'] as $plfid) {

        $pedido = $db->pegaLinha("select docid, usucpf from progfin.pedidoliberacaofinanceira where plfid = {$plfid}");
        $docid = $pedido['docid'];
        $aedid = TRANS_PEDIDO_ANALISE_SPO_AJUSTE_UO;
        $paramsWf = array('docid' => $docid, 'plfid' => $plfid);
        $RetornaPedido = wf_alterarEstado($docid, $aedid, $_POST['dados']['cmddsc'], $paramsWf);
        if ($RetornaPedido) {
            /* Retorna as Liberações do Pedido */
            $liberacoes = $db->carregar("select docid from progfin.liberacoesfinanceiras where plfid = {$plfid}");
            foreach ($liberacoes as $docidLiberacao) {
                $aedid = TRANS_LIBERACAO_ANALISE_SPO_CORRECOES_UO;
                $paramsWf = array('docid' => $docidLiberacao['docid'], 'plfid' => $plfid);
                $RetornaLiberacao = wf_alterarEstado($docid, $aedid, $_POST['dados']['cmddsc'], $paramsWf);
            }
            $qtd++;
            /*
             * Troca a data do pedido para a data de hoje
             */
            $sql = "UPDATE progfin.pedidoliberacaofinanceira SET dataultimaatualizacao = CURRENT_TIMESTAMP WHERE plfid = {$plfid}";
            $db->executar($sql);
            $db->commit();
            /*
             * Grava a notificação de aviso para o usuário
             */
            $params['sisid'] = $_SESSION['sisid'];
            $params['usucpf'] = $pedido['usucpf'];
            $params['mensagem'] = "A Solicitação de Programação Financeira {$plfid} retornou para ajustes.";
            $params['url'] = '/progfin/progfin.php?modulo=principal/solicitacaorecurso/novasolicitacao&acao=A&id=' . $plfid;
            cadastrarAvisoUsuario($params);

            tramitarLiberacoesParaAjustesUO($plfid);
        }
    }

    if ($qtd > 0) {
        enviaEmailConfirmacaoTramite();
        $fm->addMensagem($qtd . ' Pedidos foram tramitados com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Erro ao tentar tramitar os pedidos selecionados!', Simec_Helper_FlashMessage::ERRO);
    }
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script src="/ted/js/jquery.livequery.js"></script>
<script src="/ted/js/checkbox-checker.js"></script>
<script>
    $(document).ready(function () {
        var uriBase = "progfin.php?modulo=principal/solicitacaorecurso/solicitacoes/tramitacaoLote&acao=A";

        $('#search').click(function () {
            var value;
            uriBase += (value = $('input[type="radio"][name="esdid"]:checked').val()) ? '&esdid=' + value : '';
            window.location = uriBase;
        });

        $("#exportXls").on("click", function () {
            var esdid = (value = $('input[type="radio"][name="esdid"]:checked').val()) ? '&esdid=' + value : '';
            window.location = uriBase + "&exportar=xls&esdid=" + esdid;
        });
    });
</script>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="progfin.php?modulo=inicio&acao=C">Programação Financeira</a></li>
        <li>Liberações Financeiras</li>
        <li class="active">Tramitação em Lote</li>
    </ol>

    <?php
    if (is_object($fm)) {
        echo $fm->getMensagens();
    }
    ?>
    <form name="filtrosolicitacoes" id="filtrosolicitacoes" method="POST" role="form" class="form-horizontal">
        <div class="form-group row">
            <label class="control-label col-md-2">Situação:</label>
            <div class="col-md-10">
                <?php
                $opcaoMarcada = isset($_GET['esdid']) ? $_GET['esdid'] : '';
                if ($_REQUEST['tipotabela'] == 'detalhada') {
                    inputChoices('esdid', situacaoDocumentosLiberacao(), $opcaoMarcada, 'esdid');
                } else {
                    inputChoices('esdid', situacaoDocumentosPedido(), $opcaoMarcada, 'esdid');
                }
                ?>
            </div>
        </div>
        <!--<div class="form-group row">
            <label class="control-label col-md-2">Vencimento:</label>
            <div class="col-md-10">
        <?php
        $opcaoMarcada = isset($_GET['esdid']) ? $_GET['esdid'] : '';
        if ($_REQUEST['tipotabela'] == 'detalhada') {
            inputChoices('esdid', situacaoDocumentosLiberacao(), $opcaoMarcada, 'esdid');
        } else {
            inputChoices('esdid', situacaoDocumentosPedido(), $opcaoMarcada, 'esdid');
        }
        ?>
            </div>
        </div>-->
        <div class="form-group row">
            <div class="col-md-offset-3">
                <button type="button" class="btn btn-primary" id="search">Filtrar</button>
                <button type="button" class="btn btn-primary" id="exportXls">Exportar XLS</button>
            </div>
        </div>
    </form>
    <div class="row col-md-12">
        <?php tramitacaoLoteListagem(); ?>
        <div class="form-group comment">
            <label class="control-label col-md-2" for="cmddsc">Comentário:</label>
            <div class="col-md-10">
                <textarea name="dados[cmddsc]" id="cmddsc" class="form-control" rows="5" cols="25"></textarea>
            </div>
        </div>

        <div class="form-group">
            <div class="col-md-offset-1">
                <button type="button" class="btn btn-primary" name="enviar" id="btn-enviar">
                    Enviar
                </button>
            </div>
        </div>

    </div>
</div>
<?php
/**
 *
 */
function tramitacaoLoteListagem() {
    $estados = $_GET['esdid'] ? $_GET['esdid'] : ESDID_PEDIDO_ANALISE_SPO;

    $strSQL = sprintf("
                    SELECT DISTINCT
                        plf.plfid,
                        lpad(plf.plfid::text, 7,'0') AS numpedido,
                        to_char(dataultimaatualizacao,'dd/mm/yyyy') as datapedido,
                        uni.unicod || ' - ' || uni.unidsc AS unidade,
                        COUNT( 0 )              AS liberacoes,
                        SUM(lfnvalorsolicitado) AS solicitado,
                        SUM(lfnvalorautorizado) AS autorizado,
                        SUM(lfnvaloratendido)   AS atendido,
                        esd.esddsc              AS situacao,
                        esd.esdid
                    FROM progfin.pedidoliberacaofinanceira plf
                    JOIN public.unidade uni ON uni.unicod = plf.unicod
                    JOIN public.unidadegestora ung ON ung.unicod = uni.unicod
                    LEFT JOIN progfin.liberacoesfinanceiras lfn ON lfn.plfid = plf.plfid and lfn.ungcodfavorecida = ung.ungcod
                    JOIN workflow.documento doc ON plf.docid = doc.docid
                    LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                      WHERE esd.esdid IN(%d)
                    AND lfnvalorsolicitado is not null
                    AND plf.unicod NOT IN ('26101', '26298', '26291', '26290', '26443')
                    GROUP BY 1,2,3,4,9,10
                    ORDER BY 1 DESC
                ", $estados);

    require_once APPRAIZ . 'includes/library/simec/Listagem.php';

    if ($_GET['exportar'] == 'xls') {
        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_XLS);
    } else {
        $list = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
    }

    $list->setCabecalho(array(
                '<input type="checkbox" name="controlePai" class="controlePai" />',
                'Pedido',
                'Data',
                'Unidade Orçamentária (UO)',
                'Liberações',
                'Liberações de Recursos (R$)' => array(
                    'Solicitado',
                    'Aprovado',
                    'Atendido'
                ),
                'Situação'
            ))
            ->addCallbackDeCampo('unidade', 'alinhaParaEsquerda')
            ->addCallbackDeCampo('solicitado', 'mascaraMoeda')
            ->addCallbackDeCampo('autorizado', 'mascaraMoeda')
            ->addCallbackDeCampo('atendido', 'mascaraMoeda')
            ->addCallbackDeCampo('plfid', 'retornaCheckbox')
            ->esconderColunas(array('esdid'))
            ->setQuery($strSQL);

    $list->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    if ($_GET['exportar'] != 'xls') {
        $list->turnOnPesquisator();
    }
    $list->render(SIMEC_LISTAGEM::SEM_REGISTROS_MENSAGEM);
}
?>

<script type="text/javascript">
    $(function () {
        new ControleCheckbox("#tb_render", ".controlePai", ".controleFilho");

        //Adicionando o campo de comentário dentro do fmrulário de listagem
        var $commentElement = $(".comment").clone();
        $(".comment").remove();
        $(".form-listagem").append($commentElement);

        $("#btn-enviar").livequery("click", function (e) {
            if (!$(".controleFilho").is(":checked")) {
                e.preventDefault();
                bootbox.alert("Voce precisa marcar pelo menos um pedido");
                return false;
            }

            if (!$("#cmddsc").val()) {
                e.preventDefault();
                bootbox.alert("Voce precisa preenchar o comentário");
                return false;
            }

            $(".form-listagem").submit();
        });
    });
</script>
