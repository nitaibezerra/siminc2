<?php
/**
 * Controlador de requisões de definição de limites da PLOA.
 *
 * $Id: limites.inc 101153 2015-08-12 12:59:53Z maykelbraz $
 * @filesource
 */

/**
 * Funções da configuração da PLOA.
 * @uses _funcoesconfiguracaoploa.php
 */
require_once APPRAIZ . 'www/proporc/_funcoesconfiguracaoploa.php';

$fm = new Simec_Helper_FlashMessage('proporc/gestao-limites');

if (isset($_REQUEST['requisicao']) && !empty($_REQUEST['requisicao'])) {
    $requisicao = $_REQUEST['requisicao'];
    $service = new Proporc_Service_Limites();
    $urlRedirecionamento = $_SERVER['REQUEST_URI'];

    try {
        switch ($requisicao) {
            case 'exportarXLS':
                $service->prfid = $_REQUEST['prfid'];
                $service->exportarXLSResumoLimites();
                die();
            case 'detalharCategoriasDoGrupo':
                $service->gdpid = current($_REQUEST['dados']);
                $service->detalharCategoriasDoGrupo();
                die();
            case 'detalharLimitesCategoria':
                $service->dspid = $_GET['detalharlimitescategoria']['dspid'];
                $service->detalharLimitesCategoria();
                die();
            case 'salvarLimitesCategoria':
                $dados = $_POST['limitecategoria'];

                $service->dspid = $dados['dspid'];
                $service->salvarLimitesCategoria($dados);
                echo simec_json_encode($service->carregarLimites());
                die();
            case 'carregarOpcoesCategoria':
                $service->gdpid = $_REQUEST['opcoescategorias']['gdpid'];
                $service->prfid = $_REQUEST['opcoescategorias']['prfid'];

                echo simec_json_encode($service->carregarOpcoesCategoria());
                die();
            case 'importarLimites':
                $dados = $_REQUEST['importarlimites'];

                $service->dspid = $dados['dspid'];
                $service->limites = trim($dados['limites']);
                $service->setFlashmessage($fm);
                $redirecionarConfirmacao = $service->importarLimites();

                // -- Se houver confirmações a serem feitas, redirecionar para a aba de confirmação
                if ($redirecionarConfirmacao) {
                    $urlRedirecionamento = str_replace(
                        'importar',
                        'confirmar',
                        $urlRedirecionamento
                    ) . "&dspid={$dados['dspid']}";
                }

                break;
            case 'confirmarSubstituicao':
                $dados = $_REQUEST['confirmacoes'];
                $service->dspid = $dados['dspid'];
                $service->limites = $dados['limites'];
                $service->setFlashMessage($fm);
                $service->substituirLimites();
                $urlRedirecionamento = '/proporc/proporc.php?modulo=principal/limites/limites&acao=A&aba=visualizar';
                break;
            default:
                ver($requisicao, d);
        }
    } catch (Exception $e) {
        $fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
    }

    // -- redirecionamento
    header("Location: {$urlRedirecionamento}");
    die();
}

// -- Período atual
$prfref = new Proporc_Model_Periodoreferencia();
$prfref->carregarAtual();

if (empty($prfref->prfid)) {
    // -- @todo: Criar um aviso qdo o período foi carregado desta forma avisando o usuário que não pode editar
    $prfref->carregarPorAno($_SESSION['exercicio']);
}

$abas = new Simec_Abas('proporc.php?modulo=principal/limites/limites&acao=A');
$abas->adicionarAba('visualizar', 'Visualização', dirname(__FILE__) . '/limitesVisualizar.inc', 'eye-open')
    ->adicionarAba('importar', 'Importar limites', dirname(__FILE__) . '/limitesImportar.inc', 'upload');
switch ($abas->getAbaAtiva()) {
    case 'confirmar':
        $abas->adicionarAba('confirmar', 'Confirmar substituições', dirname(__FILE__) . '/limitesConfirmar.inc', 'check');
        break;
}
$abas->definirAbaDefault('visualizar');

/**
 * Cabeçalho padrão do SIMEC.
 * @uses cabecalho.inc
 */
require_once APPRAIZ . 'includes/cabecalho.inc';
?>
<link rel="stylesheet" type="text/css" href="/includes/spo.css" />
<style type="text/css">
.label-td{text-align:right;font-weight:bold;width:25%}
.modal .chosen-container{width:100%!important}
</style>
<div class="col-md-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Definição de Limites')
        ->render();
    painelInformacoesPLOA($prfref, array('preenchimentoLimites' => true));
    echo $fm;
    require $abas->render(true);
    ?>
</div>
<script type="text/javascript">
$(function() {
    $('[data-toggle="tooltip"]').tooltip();
    $('#exportar').click(function() {
        window.open('proporc.php?modulo=principal/limites/limites&acao=A&requisicao=exportarXLS&prfid=2');
    });
});
</script>