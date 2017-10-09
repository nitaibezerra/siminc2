<?php
/**
 * Gestão de metas físicas da ação.
 * $Id: metas.inc 96617 2015-04-23 20:53:16Z maykelbraz $
 */

/**
 * Funções de apoio à gestão da ploa.
 * @see _funcoesgestaoploa.php
 */
require(APPRAIZ . "www/{$_SESSION['sisdiretorio']}/_funcoesgestaoploa.php");

$fm = new Simec_Helper_FlashMessage("{$_SESSION['sisdiretorio']}/gestaoploa");

// -- Carregando dados da gestão
$dados = &dadosDaSessao('gestaoploa');

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];
    $params = array();
    switch ($requisicao) {
        case 'salvarFisico':
        case 'salvarMeta':
            $params = array($_POST['dados']);
            break;
        case 'salvarJustificativa':
            $params = array($_POST['dados'], $_SESSION['usucpf'], $_SESSION['usunome']);
            break;
        default: ver($_POST, d);
    }

    // -- Processamento da requisição
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

// -- Identificando a aba ativa
$abaAtiva = (isset($_REQUEST['aba'])?$_REQUEST['aba']:'fisico');

// -- URL base das abas
$urlBaseDasAbas = '/proporc/proporc.php?modulo=principal/gestaoploa/metas&acao=A&aba=';

$listaAbas = array();
$listaAbas[] = array("descricao" => "Meta física", "link" => "{$urlBaseDasAbas}fisico");
$listaAbas[] = array("descricao" => "Metas PO", "link" => "{$urlBaseDasAbas}metas");
$listaAbas[] = array("descricao" => "Justificativa", "link" => "{$urlBaseDasAbas}justificativa");

// -- Indica se os dados dos formulários da tela podem ser editados
$podeEditarDados = podeEditar($dados['acao']['esdid']);

/**
 * Cabecalho padrão do Sistema.
 * @see cabecalho.inc
 */
require(APPRAIZ . 'includes/cabecalho.inc');
?>
<div class="row col-md-12">
    <ol class="breadcrumb">
        <li><a href="<?php echo MODULO; ?>.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
        <?php if (exibirSeletorDeUO()): ?>
        <li><a href="<?php echo MODULO; ?>.php?modulo=principal/gestaoploa/inicio&acao=A">Seleção de UO</a></li>
        <?php endif; ?>
        <li><a href="<?php echo MODULO; ?>.php?modulo=principal/gestaoploa/limites&acao=A&aba=metas">Limites Orçamentários</a></li>
        <li class="active">Metas e Justificativa</li>
    </ol>
    <div class="col-md-offset-2 col-md-8">
        <div class="panel panel-primary">
            <div class="panel-heading"><strong>PLOA - Limites Orçamentários</strong></div>
            <table class="table">
                <tbody>
                    <tr>
                        <td><strong>Unidade Orçamentária:</strong></td>
                        <td style="text-align:left;">
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
                            <?php echo "{$dados['acao']['acacod']} - {$dados['acao']['acadsc']}"; ?>
                        </td>
                        <td><strong>Localizador:</strong></td>
                        <td style="text-align:left">
                            <?php echo "{$dados['acao']['loccod']} - {$dados['acao']['locdsc']}"; ?>
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
    <?php echo $fm->getMensagens(); ?>
    <br style="clear:both" />
    <div class="col-md-12 row">
    <?php
    // -- HTML das abas
    echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}", false, false);
    switch ($abaAtiva) {
        case 'fisico':
        case 'justificativa':
            $abaAtiva = 'form' . ucfirst($abaAtiva);
            break;
        default:
            $abaAtiva = 'listar' . ucfirst($abaAtiva);
    }
    require dirname(__FILE__) . "/metas/{$abaAtiva}.inc";
    ?>
    </div>
</div>
<br />