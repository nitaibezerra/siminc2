<?php
/**
 * Arquivo de controle da carga de despesas.
 *
 * $Id: carga.inc 100665 2015-07-31 18:36:43Z maykelbraz $
 * @filesource
 */

/**
 * Funções da configuração da PLOA.
 * @uses _funcoesconfiguracaoploa.php
 */
require_once APPRAIZ . 'www/proporc/_funcoesconfiguracaoploa.php';

$fm = new Simec_Helper_FlashMessage('proporc/carga-despesas');

if (isset($_POST['requisicao']) && !empty($_POST['requisicao'])) {
    $requisicao = $_POST['requisicao'];

    try {
        switch ($requisicao) {
            case 'carregar-despesas':
                $service = new Proporc_Service_Despesas();
                $service->setDados($_POST['cargadespesas']);
                $service->setFlashMessage($fm);
                $resultado = $service->importarValorDasDespesas();

                break;
            default:
                ver($requisicao, d);
        }

    } catch (Exception $e) {
        $fm->addMensagem($e->getMessage(), Simec_Helper_FlashMessage::ERRO);
    }

    header('Location: ' . $_SERVER['REQUEST_URI']);
    die();
}

// -- Período atual
$prfref = new Proporc_Model_Periodoreferencia();
$prfref->carregarAtual();

if (empty($prfref->prfid)) {
    // -- @todo: Criar um aviso qdo o período foi carregado desta forma avisando o usuário que não pode editar
    $prfref->carregarPorAno($_SESSION['exercicio']);
}

/**
 * Cabeçalho padrão do Simec.
 */
include APPRAIZ . "includes/cabecalho.inc";
?>
<style type="text/css">
.label-td{text-align:right;font-weight:bold;width:25%}
.modal .chosen-container{width:100%!important}
</style>
<script type="text/javascript">
function onCargadespesasSubmit(e)
{
    var mensagem = '';
    if (!$('#dspid').val()) {
        mensagem += 'O campo "Categoria" não pode ser deixado em branco.<br />';
    }
    if (!$('#justificativa').val()) {
        mensagem += 'O campo "Justificativa" não pode ser deixado em branco.<br />';
    }
    if (!$('#despesas').val()) {
        mensagem += 'O campo "Despesas" não pode ser deixado em branco.<br />';
    }

    if (!mensagem) {
        return true;
    }

    bootbox.alert(mensagem);
    return false;
}
</script>
<div class="row col-md-12">
<?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Carga de Despesas')
        ->render();

    painelInformacoesPLOA($prfref);
    echo $fm;

    $form = new Simec_View_Form('cargadespesas');

    // -- Grupo de despesa
    $mdl = new Proporc_Model_Grupodespesa();
    $where = array(
        "EXISTS (SELECT 1 FROM proporc.despesa dsp WHERE dsp.gdpid = t1.gdpid)",
        "prfid = {$prfref->prfid}"
    );
    $sql = $mdl->recuperarTodosFormatoInput('gdpnome', $where, true);
    $opcoes = array('flabel' => 'Grupo', 'titulo' => 'Selecione um grupo');
    $form->addInputCombo('gdpid', $sql, null, 'gdpid', $opcoes);
    unset($mdl, $sql, $opcoes);

    // -- Categoria de despesa
    $mdl = new Proporc_Model_Despesa();
    $where = "EXISTS (SELECT 1 FROM proporc.grupodespesa gdp WHERE gdp.prfid = {$prfref->prfid} AND gdp.gdpid = t1.gdpid)";
    $sql = $mdl->recuperarTodosFormatoInput('dspnome', array($where));
    $opcoes = array('flabel' => 'Categoria', 'titulo' => 'Selecione uma categoria');
    $form->addInputCombo('dspid', $sql, null, 'dspid', $opcoes);
    unset($mdl, $sql, $opcoes, $where);

    // -- Textarea de justificativa das despesas
    $form->addInputTextarea('justificativa', null, 'justificativa', 3000, array('flabel' => 'Justificativa'));

    // -- Textarea de carga dos dados
    $info = <<<HTML
<div class="alert alert-info" role="alert" style="margin-bottom:2px;padding:8px">
    <span class="glyphicon glyphicon-info-sign"></span> <strong>Importante:</strong> Preencha apenas um registro
    por linha, separando os campos por "<u>&lt;tab&gt;</u>" (basta copiar no Excel e colar na caixa abaixo). A estrutura do registro
    deve ser a seguinte:
    <tt>
        <ul style="font-weight:bold;list-style:square">
            <li>COD_UNIDADE_ORCAMENTARIA</li>
            <li>COD_UNIDADE_GESTORA</li>
            <li>COD_ACAO</li>
            <li>COD_LOCALIZADOR</li>
            <li>COD_PLANO_ORCAMENTARIO</li>
            <li>COD_SUBACAO</li>
            <li>COD_NATUREZA_DESPESA</li>
            <li>COD_FONTE</li>
            <li>META_LOCALIZADOR</li>
            <li>META_PLANO_ORCAMENTARIO</li>
            <li>VALOR_DESPESA</li>
        </lu>
    </tt>
</div>
HTML;
    $opcoes = array('flabel' => 'Despesas', 'info' => $info);
    $form->addInputTextarea('despesas', null, 'despesas', null, $opcoes);
    unset($opcoes, $info);

    $form->addBotoes(array('importar'))
        ->addHidden('prfid', $prfref->prfid)
        ->setRequisicao('carregar-despesas')
        ->render();
?>
</div>