<?php
//Declaração de objetos
$gerenciadorAba = new Ted_Model_GerenciadorDeAbas();

//Verifica se existe tcpid setado, senão, efetua redirecionamento
Ted_Utils_Model::capturaTcpid(true);

Ted_Utils_Model::userIsAllowed();

$tabVersao = $tabHistorico = '';
$contentVersao = $contentHistorico = '';
if (isset($_GET['versao'])) {
    $tabVersao = 'class="active"';
    $contentVersao = 'active';
} else {
    $tabHistorico = 'class="active"';
    $contentHistorico = 'active';
}

//Cabeçalho do sistema
include APPRAIZ . 'includes/cabecalho.inc';
?>
<section class="col-md-12">
    <ol class="breadcrumb">
        <li><a href="ted.php?modulo=inicio&acao=C"><?= simec_htmlentities($_SESSION['sisdsc']); ?></a></li>
        <li class=""><a href="ted.php?modulo=principal/termoexecucaodescentralizada/listarTermos&acao=A">Listar Termos</a></li>
        <li class="active">Histórico</li>
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
            <h4 class="text-center">Gerenciamento de Históricos</h4>
        </section>

        <div role="tabpanel">
            <ul class="nav nav-tabs" role="tablist">
                <li role="presentation" <?=$tabHistorico;?>>
                    <a href="#historicowf" aria-controls="historicowf" role="tab" data-toggle="tab">
                        Histórico Workflow
                    </a>
                </li>
                <li role="presentation" <?=$tabVersao;?>>
                    <a href="#versao" aria-controls="versao" role="tab" data-toggle="tab">
                        Versionamentos do Termo
                    </a>
                </li>
            </ul>

            <div class="tab-content">
                <div role="tabpanel" class="tab-pane <?=$contentHistorico;?>" id="historicowf">
                    <?php include_once dirname(__FILE__) . '/historico/historico_wf.inc'; ?>
                </div>
                <div role="tabpanel" class="tab-pane <?=$contentVersao;?>" id="versao">
                    <?php include_once dirname(__FILE__) . '/historico/versionamento.inc'; ?>
                </div>
            </div>
        </div>

    </section>
</section>