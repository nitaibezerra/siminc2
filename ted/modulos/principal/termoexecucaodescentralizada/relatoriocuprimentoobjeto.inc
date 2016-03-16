<?php

/**
 * Componente para disparar mensagens de alerta
 * @see FlashMessage.php
 */
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';

//Declaração de objetos
$formRelatorio = new Ted_Form_RelatorioCumprimentoObjeto();
$formParecerRco = new Ted_Form_ParecerRCO();
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$modelRCO = new Ted_Model_RelatorioCumprimento();
$arquivo = new Ted_Model_Arquivo();
$fm = new Simec_Helper_FlashMessage('ted/rco');
$situacao = Ted_Utils_Model::pegaSituacaoTed();
$business = new Ted_Model_RelatorioCumprimento_Business();

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

Ted_Utils_Model::userIsAllowed();

if (isset($_GET['download']) && $_REQUEST['download'] == 's' && is_numeric($_REQUEST['arqid'])) {
    require_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
    unset($_REQUEST['download']);
    $arqid = (int) $_REQUEST['arqid'];
    ob_get_clean();
    $arquivo->getDownload($arqid);
    echo "<script> window.close(); </script>";
    die;
}

if (isset($_GET['removerAnexo'])) {
    echo $arquivo->desativarAnexo($_GET['removerAnexo']);
    die();
}

/**
 * Recebe a requisição que popula o combo com os municipios
 */
if (isset($_GET['estuf']) && Ted_Utils_Model::isAjax()) {
    $muncod = new Zend_Form_Element_Select('muncod');
    $muncod->setRequired(true);
    $muncod->setAttrib('id', 'muncod');
    $muncod->setAttrib('class', 'form-control chosen-select');
    $muncod->setAttrib('style', 'width:100%;');
    $muncod->setAttrib('required', 'true');
    $muncod->addMultiOption('', 'Selecione');
    $muncod->addMultiOptions(Ted_Utils_Model::pegaMunicipio($_GET['estuf']));
    $muncod->setValue($_GET['muncod']);
    $muncod->addErrorMessage('Valor é necessário e não pode ser vazio');
    $muncod->removeDecorator('Label');
    $muncod->removeDecorator('DtDdWrapper');
    $muncod->removeDecorator('Description');
    $muncod->removeDecorator('HtmlTag');
    $formRelatorio->addElement($muncod);
    $view = new Zend_View();
    $view->setEncoding('ISO-8859-1');
    $muncod->setView($view);
    echo $muncod->__toString();
    die();
}

//Salva o parecer do RCO pela coordenação
$modelTed = new Ted_Model_TermoExecucaoDescentralizada();
if (isset($_POST['tcpobsrelatorio']) && $formParecerRco->isValid($_POST) && ($situacao['esdid'] == RELATORIO_OBJ_AGUARDANDO_ANALISE_COORD)) {
    $dados = array_merge($_POST, $_FILES);

    /*$arquivo = new Ted_Model_Arquivo();
    $f = $arquivo->inserirAnexo($dados, Ted_Model_Arquivo::ABA_TRAMITE);*/
    $u = $modelTed->updateRelatorioRCO($dados['tcpobsrelatorio']);

    if ($u) {
        $fm->addMensagem('Parecer do RCO salvo com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Falha ao tentar salvar o parecer do RCO!', Simec_Helper_FlashMessage::ERRO);
    }

    $_POST = array();
}

if (isset($_POST['muncod']) && !empty($_POST['muncod'])) {
    $formRelatorio->getElement('muncod')->addMultiOptions(Ted_Utils_Model::pegaMunicipio($_POST['estuf']));
    $formRelatorio->getElement('muncod')->setValue($_POST['muncod']);
}

/**
 * Recebe a requisição para salvar ou alterar o RCO
 */
if (isset($_POST['recid'])
    && (($modelRCO->isValid($formRelatorio, $_POST) && in_array($situacao['esdid'], array(EM_EXECUCAO, TERMO_EM_DILIGENCIA_RELATORIO)))
    || $business->termoVencido(Ted_Utils_Model::capturaTcpid())))
{

    if ($recid = $modelRCO->save($_POST)) {
        $arquivo = $_FILES['arquivo'];

        if ($_FILES['arquivo'] && $arquivo['name'] && $arquivo['type'] && $arquivo['size']) {
            include_once APPRAIZ . 'includes/classes/fileSimec.class.inc';
            $file = new FilesSimec('arquivo', null, 'public');
            $file->setPasta('ted');
            $file->setUpload(null, 'arquivo', false);

            $recid = (is_bool($recid)) ? $_POST['recid'] : $recid;
            //ver($recid, $file->getIdArquivo(), d);

            $strInsert = sprintf("
                INSERT INTO ted.relatoriocumprimentoanexo(arqid, recid, usucpf) VALUES (%d, %d, '%s')
            ", $file->getIdArquivo(), $recid, $_SESSION['usucpf']);
            $db->executar($strInsert);
            $db->commit();
        }

        $fm->addMensagem('Registro salvo com sucesso!', Simec_Helper_FlashMessage::SUCESSO);
    } else {
        $fm->addMensagem('Falha ao tentar salvar o registro!', Simec_Helper_FlashMessage::ERRO);
    }
} else {
    if (count($_POST) && !$modelRCO->isValid($formRelatorio, $_POST)) {
        $fm->addMensagem('Existem campos obrigatórios que não foram preenchidos, favor verificar o formulário!', Simec_Helper_FlashMessage::ERRO);
        $formRelatorio->populate($_POST);
    }
}

/**
 * Busca os dados para o formulário de RCO
 * Dados de preenchimento padrão ou dados de preenchimento do usuário
 */
if (!$modelRCO->capturaRelatorioCumprimento()) {
    $dados = $modelRCO->preenchimentoPadraoDoObjeto($_GET['ted']);
    $formRelatorio->getElement('tcpid')->setValue($_GET['ted']);
} else {
    $dados = $modelRCO->capturaRelatorioCumprimento();
}

/**
 * Popula o formulário usando os dados encontrados
 */
if (is_array($dados) && !isset($_POST['recid'])) {
    unset($dados['recstatus'], $dados['recnumnotacredito']);
    $formRelatorio->populate($dados);
}

//Popula o campo de parecer do RCO pela coordenação
if (strlen($modelTed->tcpobsrelatorio)) {
    $formParecerRco->getElement('tcpobsrelatorio')->setValue($modelTed->tcpobsrelatorio);
}

//Cabeçalho do sistema
include APPRAIZ.'includes/cabecalho.inc';
?>
<link rel="stylesheet" href="/ted/css/ted-geral.css" type="text/css"/>
<script src="/includes/funcoes.js" type="text/javascript"></script>
<script src="/ted/js/jquery.livequery.js" type="text/javascript"></script>
<script src="/ted/js/jquery.textarea.limiter.js" type="text/javascript"></script>
<script src="/ted/js/bootbox.js" type="text/javascript"></script>
<script type="text/javascript">
var rcoInit = window.rcoInit = {
    muncod:<?= empty($dados['muncod'])? 'null' : $dados['muncod']; ?>
  , ted:<?=Ted_Utils_Model::capturaTcpid();?>
  , gestor:<?=(int)Ted_Utils_Model::possuiPerfilGestor();?>
};
</script>
<link rel="stylesheet" href="/ted/css/bootstrap-datepicker3.min.css" type="text/css"/>
<script src="/ted/js/bootstrap-datepicker.min.js" type="text/javascript"></script>
<script src="/ted/js/bootstrap-datepicker.pt-BR.min.js" type="text/javascript"></script>
<script src="/ted/js/relatorio-cumprimento-objeto.js" type="text/javascript"></script>

<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Prestação de Contas do Objeto</li>
    </ol>
    <nav class="col-md-2">
        <?php echo $gerenciadorAba->apresentaAbas(); ?>
    </nav>

    <div class="col-md-10">
        <div class="col-md-2"></div>
        <div class="col-md-8">
            <?php echo Ted_Utils_Model::montaInformacaoTermo();?>
        </div>
        <div class="col-md-2"></div>
    </div>

    <section class="col-md-10">
        <?php echo $fm->getMensagens(); ?>

        <div class="panel-group" id="accordion">
            <div class="panel panel-default" style="overflow-y: auto;">
                <div class="panel-heading" role="tab" id="headingOne">
                    <h4 class="panel-title">
                        <a data-toggle="collapse" data-parent="#accordion" href="#collapseOne">
                            Prestação de Contas do Objeto
                        </a>
                    </h4>
                </div>
                <div id="collapseOne" class="panel-collapse collapse">
                    <div class="panel-body">
                        <?php echo $formRelatorio->showForm(); ?>

                        <section>
                            <?php if (isset($dados['recid'])) $modelRCO->listarAnexos($dados['recid']); ?>
                        </section>
                    </div>
                </div>

                <?php
                $podeAlterar = array(639, 660);
                $nãoPodeAlterar = array(652, 655, 656, 1492, 1493, 1495, 1494);
                ?>

                <?php if (in_array($situacao['esdid'], $podeAlterar) && Ted_Utils_Model::uoEquipeTecnicaConcedente()) : ?>

                <?php else: ?>
                    <?php if (in_array($situacao['esdid'], $nãoPodeAlterar) && Ted_Utils_Model::uoEquipeTecnicaConcedente()) : ?>
                        <script type="text/javascript">
                            $(function(){
                                $("#enviar").parent().parent().remove();
                            });
                        </script>
                    <?php endif; ?>
                <?php endif; ?>

            </div>

            <?php if ($modelRCO->momentoRCOemAnaliseCoordenacao()) : ?>

                    <script type="text/javascript">
                        setTimeout(function(){
                            $("#collapseOne").collapse("hide");
                            $("#collapseTwo").collapse("show");
                        }, 600);
                    </script>

                    <div class="panel panel-default">
                        <div class="panel-heading" role="tab" id="headingTwo">
                            <h4 class="panel-title">
                                <a data-toggle="collapse" data-parent="#accordion" href="#collapseTwo">
                                    Parecer Tecnico da Prestação de Contas do Objeto
                                </a>
                            </h4>
                        </div>
                        <div id="collapseTwo" class="panel-collapse collapse">
                            <div class="panel-body" >
                                <?php echo $formParecerRco->showForm(); ?>
                                <?php $arquivo->listarAnexos(Ted_Model_Arquivo::ABA_TRAMITE); ?>
                            </div>
                        </div>
                    </div>

                    <?php if (!$modelRCO->momentoRCOemAnaliseCoordenacao()) : ?>
                        <script type="text/javascript">
                            setTimeout(function(){
                                $("#collapseOne").collapse("show");
                                $("#collapseTwo").collapse("show");
                                $("#enviar_rco").parent().parent().remove();
                            }, 800);
                        </script>
                    <?php endif; ?>

            <?php endif; ?>
        </div>

    </section>
</section>

<script type="text/javascript">
$(function(){
    setTimeout(function() {
        $("#muncod_chosen").attr("style", "width: 100%;");
    }, 1500);
});
</script>
