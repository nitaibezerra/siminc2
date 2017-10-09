<?php
/**
 * Gerenciamento dos grupos de despesas da PLOA e layout geral.
 *
 * $Id: despesas.inc 101146 2015-08-11 21:22:58Z maykelbraz $
 */

/**
 * Funções da configuração da PLOA.
 * @uses _funcoesconfiguracaoploa.php
 */
require_once APPRAIZ . 'www/proporc/_funcoesconfiguracaoploa.php';

$fm = new Simec_Helper_FlashMessage('proporc/salvar-grupo-despesa');
if (isset($_REQUEST['requisicao'])) {
    try {
        $despesas = new Proporc_Service_Despesas();
        $urlRedirecionamento = $_SERVER['REQUEST_URI'];

        switch ($_REQUEST['requisicao']) {
            case 'consultarGrupo':
                echo $despesas->setDados($_GET['grupodespesa'])
                    ->consultarGrupo($retornarJSON = true);
                die();
            case 'salvarGrupodespesa':
                $despesas->setDados($_POST['grupodespesa'])
                    ->salvarGrupoDespesas();
                $fm->addMensagem('Grupo de despesas criado ou alterado com sucesso.');
                break;
            case 'apagarGrupodespesa':
                $despesas->setDados($_POST['grupodespesa'])
                    ->apagarGrupoDespesas();
                $fm->addMensagem('Grupo de despesas apagado com sucesso.');
                break;
            case 'filtrarGrupos':
                $uri = parse_url($urlRedirecionamento);
                parse_str($uri['query'], $uri['query']);
                $uri['query']['filtrogrupos'] = $_REQUEST['filtrogrupos'];
                $urlRedirecionamento = "{$uri['path']}?" . urldecode(http_build_query($uri['query'], '', '&'));
                break;
            case 'consultarCategoria':
                echo $despesas->setDados($_GET['categoriadespesa'])
                    ->consultarCategoria($retornarJSON = true);
                die();
            case 'salvarCategoriadespesa':
                $despesas->setDados($_POST['categoriadespesa'])
                    ->salvarCategoriaDespesas();
                $fm->addMensagem('Coluna de despesas criada ou alterada com sucesso.');
                break;
            case 'apagarCategoriadespesa':
                $despesas->setDados($_POST['categoriadespesa'])
                    ->apagarCategoriaDespesas();
                $fm->addMensagem('Coluna de despesas apagada com sucesso.');
                break;
            case 'filtrarCategorias':
                $uri = parse_url($urlRedirecionamento);
                parse_str($uri['query'], $uri['query']);
                $uri['query']['filtrocategorias'] = array_filter($_REQUEST['filtrocategorias'], 'strlen');
                $urlRedirecionamento = "{$uri['path']}?" . urldecode(http_build_query($uri['query'], '', '&'));
                break;
            case 'salvarDespesaFGU':
                $despesas->setDados($_POST['fgu'])
                    ->salvarDespesasFGU();
                $fm->addMensagem('Configurações salvas com sucesso.');
                break;
            case 'salvarDespesaAdicionais':
                $despesas->setDados($_POST['adicionais'])
                    ->salvarDespesasAdicionais();
                $fm->addMensagem('Configurações salvas com sucesso.');
                break;
            case 'filtrarPlanoorcamentario':
                $despesas->setDados($_GET['planoorcamentario']);
                $despesas->exercicio = $_SESSION['exercicio'];
                $despesas->imprimirListaPlanosorcamentarios();
                die();
            case 'contarDetalhamentoCategoria':
                $despesas->setDados($_GET['categoriadespesa']);
                echo simec_json_encode($despesas->contarDetalhamentoCategoria());
                die();
            case 'contarDetalhamentoGrupo':
                $despesas->setDados($_GET['grupodespesa']);
                echo simec_json_encode($despesas->contarDetalhamentoGrupo());
                die();
            default:
                ver($_REQUEST['requisicao'], d);
        }
    } catch (Exception $e) {
        $fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
    }

    // -- redirecionamento
    header("Location: {$urlRedirecionamento}");
    die();
}

$abas = new Simec_Abas('proporc.php?modulo=principal/despesas/despesas&acao=A');
$abas->adicionarAba('grupo', 'Grupos de despesas', dirname(__FILE__) . '/grupo/grupo.inc', 'th')
    ->adicionarAba('categoria', 'Colunas de despesas', dirname(__FILE__) . '/categoria/categoria.inc', 'th-list');

switch ($abas->getAbaAtiva()) {
    case 'fgu':
    case 'adicionais':
        $params = array('dspid' => $_REQUEST['dspid']);
        $abas->adicionarAba('fgu', 'Fontes, GND & UO', dirname(__FILE__) . '/fgu/fgu.inc', 'check', $params);
        $abas->adicionarAba('adicionais', 'Configurações adicionais', dirname(__FILE__) . '/adicionais/adicionais.inc', 'check', $params);
        break;
}
$abas->definirAbaDefault('grupo');

// -- Período atual
$prfref = new Proporc_Model_Periodoreferencia();
$prfref->carregarAtual();

if (empty($prfref->prfid)) {
    // -- @todo: Criar um aviso qdo o período foi carregado desta forma avisando o usuário que não pode editar
    $prfref->carregarPorAno($_SESSION['exercicio']);
}

/**
 * Cabecalho do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<style type="text/css">
.label-td{text-align:right;font-weight:bold}
.modal .chosen-container{width:100%!important}
</style>
<div class="col-md-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Configuração de despesas')
        ->render();

    painelInformacoesPLOA($prfref);
    echo $fm;
    require $abas->render(true);
    ?>
</div>