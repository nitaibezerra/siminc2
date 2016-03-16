<?php
/**
 * Arquivo principal de controle de limites e metas da PLOA.
 * $Id: limites.inc 99906 2015-07-09 13:29:47Z maykelbraz $
 */

/**
 * Funções do workflow
 * @see workflow.php
 */
include_once APPRAIZ . 'includes/workflow.php';

/**
 * Funções de apoio à gestão da PLOA.
 * @see _funcoesgestaoploa.php
 */
require(APPRAIZ . "www/{$_SESSION['sisdiretorio']}/_funcoesgestaoploa.php");

$fm = new Simec_Helper_FlashMessage("{$_SESSION['sisdiretorio']}/gestao");

// -- Carregando dados da gestão
$dados = &dadosDaSessao('gestaoploa');

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    switch ($requisicao) {
        case 'consultarAcoes':
            imprimirAcoes($_POST['dados'], $dados['ppoid'], $_SESSION['exercicio']);
            die();
        case 'detalharGrupo':
            imprimirColunasDoGrupo($_POST['dados'], $dados['ppoid'], $dados['unicod']);
            die();
        case 'detalharColuna':
            $dadosrequisicao = array(
                'mtrid' => current($_POST['dados']),
                'gpmid' => next($_POST['dados']),
                'unicod' => $dados['unicod']
            );
            imprimirAcoes($dadosrequisicao, $dados['ppoid'], $_SESSION['exercicio'], 'despesas');
            die();
        case 'showQDDPO':
            // -- Construíndo o relatório
            ob_start();
            require_once(APPRAIZ . 'elabrev/modulos/relatorio/orcamento/relatorioQuadroCreditosPo.inc');
            //$qddHTML = ob_get_flush();
            $qddHTML = ob_get_contents();
            ob_end_clean();
            if ($_POST['exportar']) { // -- Exportar relatório XLS
                echo removeLinks($qddHTML);
                die();
            }
            echo <<<HTML
<div class="btn-group col-lg-5 col-lg-offset-7">
HTML;
            echo exerciciosElabrev($_POST['dados']['exercicio'], 'relatorioElabrevQddPO', $dados['unicod'] .' '. $dados['unidsc']);

            echo <<<HTML
    <button type="button" class="btn btn-default btn-success" onclick="abrirFecharTodos()"
            id="detalhar-qdd"><span class="glyphicon glyphicon-plus"></span> Detalhar</button>
    <button type="button" class="btn btn-default btn-danger" onclick="imprimirRelatorioQDD()">
        <span class="glyphicon glyphicon glyphicon-print"></span> Imprimir</button>
    <button type="button" class="btn btn-default btn-warning" onclick="exportarRelatorioQDD('showQDDPO')">
        <span class="glyphicon glyphicon-save"></span> Exportar</button>
</div>
<br style="clear:both" />
<br />
<div id="relatorio-qdd">
HTML;
            echo removeLinks($qddHTML);
            echo <<<HTML
</div>
HTML;
            die();
        case 'showLimites':
            ob_start();
            require_once(APPRAIZ . 'elabrev/modulos/principal/propostaorcamentaria/consultalimites.inc');
            //$qddHTML = ob_get_flush();
            $qddHTML = ob_get_contents();
            ob_end_clean();
            if ($_POST['exportar']) { // -- Exportar relatório XLS
                echo removeLinks($qddHTML);
                die();
            }
            echo <<<HTML
<div class="btn-group col-lg-4 col-lg-offset-8">
HTML;
            echo exerciciosElabrev($_POST['dados']['exercicio'], 'relatorioElabrevLimites',$dados['unicod'] .' '. $dados['unidsc']);
            echo <<<HTML
    <button type="button" class="btn btn-default btn-danger" onclick="imprimirRelatorioQDD()">
        <span class="glyphicon glyphicon glyphicon-print"></span> Imprimir</button>
    <button type="button" class="btn btn-default btn-warning" onclick="exportarRelatorioQDD('showLimites')">
        <span class="glyphicon glyphicon-save"></span> Exportar</button>
</div>
<br style="clear:both" />
<br />
<div id="relatorio-qdd">
HTML;
            echo removeLinks($qddHTML);
            echo <<<HTML

</div>
HTML;

            die();
        case 'showQDDSubacao':
            ob_start();
            require_once(APPRAIZ . 'elabrev/modulos/relatorio/orcamento/relatorioQuadroCreditosSubacao.inc');
            //$qddHTML = ob_get_flush();
            $qddHTML = ob_get_contents();
            ob_end_clean();
            if ($_POST['exportar']) { // -- Exportar relatório XLS
                echo removeLinks($qddHTML);
                die();
            }
            echo <<<HTML
<div class="btn-group col-lg-4 col-lg-offset-8">
HTML;
            echo exerciciosElabrev($_POST['dados']['exercicio'], 'relatorioElabrevQddSubacao',$dados['unicod'] .' '. $dados['unidsc']);
            echo <<<HTML
    <button type="button" class="btn btn-default btn-danger" onclick="imprimirRelatorioQDD()">
        <span class="glyphicon glyphicon glyphicon-print"></span> Imprimir</button>
    <button type="button" class="btn btn-default btn-warning" onclick="exportarRelatorioQDD('showQDDSubacao')">
        <span class="glyphicon glyphicon-save"></span> Exportar</button>
</div>
<br style="clear:both" />
<br />
<div id="relatorio-qdd">
HTML;
            echo removeLinks($qddHTML);
            echo <<<HTML
</div>
HTML;
            die();
        case 'showSinteseDespesas':
            if ($_POST['exportar']) { // -- Exportar relatório XLS
                require_once(APPRAIZ . 'elabrev/modulos/relatorio/orcamento/relatorioSintese.inc');
                die();
            }
            echo <<<HTML
<div class="btn-group col-lg-4 col-lg-offset-8">
HTML;
            echo exerciciosElabrev($_POST['dados']['exercicio'], 'relatorioElabrevSintese',$dados['unicod'] .' '. $dados['unidsc']);
            echo <<<HTML
    <button type="button" class="btn btn-default btn-danger" onclick="imprimirRelatorioQDD()">
        <span class="glyphicon glyphicon glyphicon-print"></span> Imprimir</button>
    <button type="button" class="btn btn-default btn-warning" onclick="exportarRelatorioQDD('showSinteseDespesas')">
        <span class="glyphicon glyphicon-save"></span> Exportar</button>
</div>
<br style="clear:both" />
<br />
<div id="relatorio-qdd">
HTML;
            require_once(APPRAIZ . 'elabrev/modulos/relatorio/orcamento/relatorioSintese.inc');
            echo <<<HTML
</div>
HTML;
            die();
        case 'consultarMotivoFalhaSIOP':
            imprimirMotivoFalhaSIOP($_POST['dados']);
            die();
        case 'editarAcao':
            $requisicao = 'carregarDadosAcao';
            $params = array($_POST['dados'], $_SESSION['exercicio'], false);
            $novaURL = 'proporc.php?modulo=principal/gestaoploa/metas&acao=A';
            break;
        case 'detalharDespesas':
            $requisicao = 'carregarDadosAcao';
            $params = $_POST['dados'];
            $params['unicod'] = $dados['unicod'];
            $params['ppoid'] = $dados['ppoid'];
            $params = array($params, $_SESSION['exercicio'], true);
            $novaURL = 'proporc.php?modulo=principal/gestaoploa/detalhamento&acao=A';
            break;
        case 'enviarProposta':
            if (is_array($_POST['dados']['acaid'])) {
                $listaAcoes = $_POST['dados']['acaid'];
                $sucessoNoEnvio = true;
                foreach ($listaAcoes as $acaid => $_) {
                    $retorno = enviarProposta(array('acaid' => $acaid, 'tramitar' => true));
                    if (!$retorno[sucesso]) {
                        $sucessoNoEnvio = false;
                    }
                }
                // -- Todos os registros enviados com sucesso
                if ($sucessoNoEnvio) {
                    $fm->addMensagem('Registros selecionados enviados com sucesso.');
                } else {
                    $fm->addMensagem('Alguns registros selecionados não foram enviados. Veja o log de erros para maiores detalhes.');
                }
            } else {
                $dadosEnvio = $_POST['dados'];
                $dadosEnvio['tramitar'] = true;
                $returno = enviarProposta($dadosEnvio);
                if ($retorno['sucesso']) {
                    $fm->addMensagem('Registros selecionados enviados com sucesso.');
                } else {
                    $fm->addMensagem('Alguns registros selecionados não foram enviados. Veja o log de erros para maiores detalhes.');
                }
            }
            header('Location: ' . $_SERVER['REQUEST_URI']);
            die();
        case 'drawWorkflow':
            list($acaid, $docid) = $_POST['params'];
            wf_desenhaBarraNavegacao(
                $docid,
                array(
                    'dados' => array(
                        'acaid' => $acaid,
                        'tramitar' => false
                    )
                )
            );
            die();
        default:
            die($requisicao);
    }

    // -- Processamento genérico da requisição
    $result = call_user_func_array($requisicao, $params);
    if ($result['sucesso'] && !empty($result['mensagem'])) {
        $fm->addMensagem($result['mensagem']);
    } elseif (!$result['sucesso']) {
        unset($novaURL);
        $fm->addMensagem($result['mensagem'], Simec_Helper_FlashMessage::ERRO);
    }

    // -- Redirecionamento padrão
    if (!isset($novaURL)) {
        $novaURL = $_SERVER['REQUEST_URI'];
    }
    header('Location: ' . $novaURL);
    die();
}
// -- Carregando e verificando se existe uma proposta
if (!$existeProposta = verificarPropostaOrcamentaria($_SESSION['exercicio'])) {
    $exercicioatual = $_SESSION['exercicio'];
    $exercicioseguinte = $exercicioatual + 1;
    $fm->addMensagem(
        <<<HTML
<strong><span class="glyphicon glyphicon-warning-sign"></span> Atenção!</strong>
Não foi localizada uma proposta orçamentária no exercício atual ({$exercicioatual})
referente ao exercício seguinte ({$exercicioseguinte}).
HTML
        , Simec_Helper_FlashMessage::ERRO);
}

$limiteresumo = consultarLimites($dados, 'resumo');

if ($existeProposta) {
    // -- Identificando a aba ativa
    $abaAtiva = (isset($_REQUEST['aba']) ? $_REQUEST['aba'] : 'limites');

    // -- URL base das abas
    $urlBaseDasAbas = '/proporc/proporc.php?modulo=principal/gestaoploa/limites&acao=A&aba=';

    $listaAbas = array();
    $listaAbas[] = array("descricao" => "Gestão financeira", "link" => "{$urlBaseDasAbas}limites");
    $listaAbas[] = array("descricao" => "Gestão de metas", "link" => "{$urlBaseDasAbas}metas");
    $listaAbas[] = array("descricao" => "Tramitação", "link" => "{$urlBaseDasAbas}tramitacoes");
}

/**
 * Cabecalho padrão do SIMEC.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<style type="text/css">
label{margin-top:10px;cursor:pointer}
.panel-collapse table{margin-bottom:0}
.glyphicon-chevron-right,.glyphicon-info-sign{font-size:.8em;margin-right:5px}
.modal-body{text-align:left}
@media(min-width:992px){.modal-lg{width:100%}}
</style>
<script type="text/javascript" src="js/gestaoploa.js"></script>
<script type="text/javascript" lang="javascript">
$(document).ready(function(){
    var ppoid  = $('#ppoid').val();
    if ('' == ppoid) { // -- Bloqueando relatorios qdd quando não há ppoid
        $("#qdd-po").attr( "disabled", "disabled" );
        $("#qdd-subacao").attr( "disabled", "disabled" );
        $("#sintese-despesas").attr( "disabled", "disabled" );
    }
    $('#modal-alert .modal-dialog').addClass('modal-lg');
    $('.info-elabrev').click(function(){
        var requisicao = $(this).attr('data-requisicao');
        var titulo = $(this).attr('data-titulo');
        var exercicio = $('#exercicio_spo').val();
        var ppoid = $('#ppoid').val();
        relatorioElabrev(requisicao, titulo, exercicio, ppoid);
    });
});
</script>
<div class="row col-md-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    if (exibirSeletorDeUO()) {
        $bc->add('Seleção de UO', 'proporc.php?modulo=principal/gestaoploa/inicio&acao=A');
    }
    $bc->add('Limites Orçamentários')
        ->render();
    ?>
    <div class="col-md-offset-2 col-md-8">
        <div class="panel panel-primary">
            <div class="panel-heading"><strong>PLOA - Limites Orçamentários</strong></div>
            <table class="table">
                <tbody>
                    <tr>
                        <td><strong>Exercicio:</strong></td>
                        <td style="text-align:left">
                            <?php echo $_SESSION['exercicio'] + 1; ?>
                            <input type="hidden" id="ppoid" value="<?php echo $dados['ppoid']; ?>" />
                            <input type="hidden" id="exercicio_spo" value="<?php echo $_SESSION['exercicio']; ?>" />
                        </td>
                        <td><strong>Unidade Orçamentária:</strong></td>
                        <td><?php echo "{$dados['unicod']} - {$dados['unidsc']}"; ?></td>
                    </tr>
                </tbody>
            </table>
            <hr style="margin:5px 10px" />
            <table class="table">
                <thead>
                    <tr>
                        <th colspan="6">LIMITES (R$)</th>
                    </tr>
                </thead>
                <tbody>
                    <tr>
                        <td style="width:16%"><strong>Tesouro (a):</strong></td>
                        <td style="width:18%"><?php echo mascaraNumero($limiteresumo['valortesouro']); ?></td>
                        <td style="width:16%"><strong>Próprios (b):</strong></td>
                        <td style="width:17%"><?php echo mascaraNumero($limiteresumo['valoroutros']); ?></td>
                        <td style="width:16%"><strong>Total (a + b):</strong></td>
                        <td style="width:17%"><?php echo mascaraNumero($limiteresumo['valortotal']); ?></td>
                    </tr>
                    <tr>
                        <td colspan="6" style="text-align: right" >
                            <button type="button" class="btn btn-success info-elabrev" id="qdd-po"
                                    data-requisicao="showQDDPO"
                                    data-titulo="Quadro de Detalhamento das Despesas - PO - <?= $dados['unicod'] .' '. $dados['unidsc'] ?>">QDD por PO </button>
                            <button type="button" class="btn btn-success info-elabrev" id="qdd-subacao"
                                    data-requisicao="showQDDSubacao"
                                    data-titulo="Quadro de Detalhamento das Despesas - Subação - <?= $dados['unicod'] .' '. $dados['unidsc'] ?>">QDD por Subação </button>
                            <button type="button" class="btn btn-success info-elabrev" id="sintese-despesas"
                                    data-requisicao="showSinteseDespesas"
                                    data-titulo="Síntese de Despesas da UO - <?= $dados['unicod'] .' '. $dados['unidsc'] ?>">Síntese das Despesas</button>
                            <button type="button" class="btn btn-success info-elabrev" id="sintese-despesas"
                                    data-requisicao="showLimites"
                                    data-titulo="Limites Orçamentários da UO por Grupo de Coluna - <?= $dados['unicod'] .' '. $dados['unidsc'] ?>">UO e Grupo de Coluna</button>
                        </td>
                    </tr>
                </tbody>

            </table>
        </div>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <br style="clear:both" />
    <?php if ('limites' == $abaAtiva): ?>
        <div class="col-md-10 col-md-offset-1">
            <div class="panel-group" id="accordion_fontes">
                <?php require(dirname(__FILE__) . '/limites/listarFontes.inc'); ?>
            </div>
        </div>
        <br style="clear:both" />
        <br />
    <?php endif; ?>
    <div class="col-md-12 row">
        <?php
        if ($existeProposta) {
            // -- HTML das abas
            echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}", false, false);
            switch ($abaAtiva) {
                default:
                    $abaAtiva = 'listar' . ucfirst($abaAtiva);
            }
            require dirname(__FILE__) . "/limites/{$abaAtiva}.inc";
        }
        ?>
    </div>
</div>
<br />