<?php

//Declaração de Objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();
$modelOrcamento = new Ted_Model_Orcamento();
$modelMonitora = new Ted_Model_Monitora();
$fm = new Simec_Helper_FlashMessage('ted/ted');

//Url base
$urlBase = 'ted.php?modulo=principal/termoexecucaodescentralizada';

if (Ted_Utils_Model::isAjax() && isset($_GET['saveorcamento'])) {
    header("Content-Type: application/json");
    $lote = $modelOrcamento->prepareData($_POST);
    foreach ($lote as $item) {
        $modelOrcamento->salvarDados($item);
    }
    echo simec_json_encode(array('success' => true));
    die;
}

if (Ted_Utils_Model::isAjax() && isset($_GET['deleteid'])) {
    header("Content-Type: application/json");
    if ($modelOrcamento->delete($_GET['deleteid'])) {
        echo simec_json_encode(array('success' => true));
    } else {
        echo simec_json_encode(array('fail' => true));
    }
    die;
}

//urlPage
$urlPage = $urlBase.'/programacao&acao=A';

if (isset($_GET['action']) && Ted_Utils_Model::isAjax()) {
    switch ($_GET['action']) {
        case 'getMesesExecucao':
            $dados = $modelOrcamento->getIntervaloMeses();
            echo $dados;
            die();
        case 'getNaturezaDespesa':
            $dados = $modelMonitora->getNaturezaDespesa();
            echo $dados;
            die();
        case 'getPtres':
            $currentYear = (isset($_GET['onlypopulate']))?false:true;
            $dados = $modelMonitora->getPtres($currentYear);
            echo $dados;
            die();
        case 'getPtresJSON':
            echo $modelMonitora->getPtres((int)$_GET['orcamentaria']['proanoreferencia'], 'json');
            die();
            // -- no break
        case 'getPlanoInterno':
            $dados = $modelMonitora->getPlanoInterno((int) $_GET['ptrid']);
            echo $dados;
            die();
        case 'getPlanoInternoJSON':
            $dados = $modelMonitora->getPlanoInterno((int)$_GET['orcamentaria']['ptrid'], 'json');
            echo $dados?$dados:'{}';
            die();
        case 'getAcaoPtrid':
            $dados = $modelMonitora->getAcaoPtrid((int) $_GET['ptrid']);
            echo $dados;
            die();
        case 'getDescricaoAcao':
            $dados = $modelMonitora->getDescricaoAcao((int) $_GET['ptrid']);
            echo $dados;
            die();
    }
}

if (isset($_POST['action'])) {
    $modelOrcamento->popularDadosObjeto($_POST['orcamentaria']);
    switch ($_POST['action']) {
        case 'salvarOrcamentaria':
            if ($modelOrcamento->salvar()) {
                $modelOrcamento->commit();
                $fm->addMensagem('Item orçamentário criado ou alterado com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível alterar ou criar o item orçamentário.', Simec_Helper_FlashMessage::ERRO);
            }
            break;
        case 'apagarOrcamentaria':
            if ($modelOrcamento->excluir()) {
                $modelOrcamento->commit();
                $fm->addMensagem('Item orçamentário removido com sucesso.');
            } else {
                $fm->addMensagem('Não foi possível remover o item orçamentário.', Simec_Helper_FlashMessage::ERRO);
            }
            break;
        default:
            ver($_REQUEST, d);
    }
    header('Location: ' . $_SERVER['REQUEST_URI']);
    die();
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<script src="/ted/js/jquery.livequery.js" type="text/javascript"></script>
<script src="/ted/js/handlebars-v2.0.0.js" type="text/javascript"></script>
<script type="text/javascript">
var settings = {
    endpoint:"<?=$urlPage;?>",
    ted:<?=$_GET['ted']?>,
    year:"<?=date("Y");?>",
};

var panelControl = function() {
    var $btnControl = $btnControl || $(".panel-control");
    $btnControl.on("click", function(){
        $(this).parent().parent().parent().find(".panel-body").slideToggle();
    });
};

window.onload = function() {
    if (document.readyState == "complete") {
        panelControl();
    }
};
$(function(){
  $('[data-toggle="tooltip"]').tooltip();
});
</script>
<style type="text/css">
.panel-default>.panel-heading{
color:#d9edf7;
background-color:#337ab7;
border-color:#ddd
}
.tb_render{margin-top:20px}
.remove-nc{cursor:pointer}
.btn-float-right{float:right}
</style>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Programação Orçamentária</li>
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
        <section class="well">
            <h4 class="text-center">Programação Orçamentária</h4>
        </section>
        <section class="">
            <?php echo $fm->getMensagens(); ?>
        </section>

        <div class="panel panel-primary">
            <div class="panel-heading">
                <h3 class="panel-title">Orçamentário <span class="glyphicon glyphicon-minus panel-control" style="float:right"></span></h3>
            </div>
            <div class="panel-body">
                <?php require_once dirname(__FILE__) . '/programacao/orcamentaria.inc'; ?>
            </div>
        </div>

<!--        <div class="panel panel-primary">
            <div class="panel-heading">
                <h3 class="panel-title">Cronograma Físico/Financeiro <span class="glyphicon glyphicon-minus panel-control" style="float:right"></span></h3>
            </div>
            <div class="panel-body">
                <?php require_once dirname(__FILE__) . '/programacao/financeira.inc'; ?>
            </div>
        </div>

        <div class="panel panel-primary">
            <div class="panel-heading">
                <h3 class="panel-title">Nota de Crédito <span class="glyphicon glyphicon-minus panel-control" style="float:right"></span></h3>
            </div>
            <div class="panel-body">
                <?php require_once dirname(__FILE__) . '/programacao/nota_credito.inc'; ?>
            </div>
        </div>-->

    </section>
</section>
