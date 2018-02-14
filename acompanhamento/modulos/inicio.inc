<?php

//simec_redirecionar('emendas.php?modulo=principal/emenda&acao=A');

switch ($_REQUEST['req']) {
    case 'espelho-pi':
        include APPRAIZ. 'planacomorc/modulos/principal/unidade/espelho-pi.inc';
        die;
}

$mEmendas = new Emendas_Model_Emenda();
$mAcompanhamento = new Acompanhamento_Model_Acompanhamento($_REQUEST['acoid']);

include_once APPRAIZ . "includes/library/simec/Grafico.php";
include APPRAIZ . "includes/cabecalho.inc";
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
    <div class="col-lg-4">
        <div class="form-horizontal" style="padding-top: 10px;">
            <?php
            $aUnidades = (new Public_Model_SubUnidadeOrcamentaria())->recuperarPorUsuario();
            $suoid = $_REQUEST['suoid'] ? $_REQUEST['suoid'] : key($aUnidades);
            echo $simec->select('suoid', 'Unidade', $suoid, $aUnidades, null, ['label-size'=>2]);
            ?>
        </div>
    </div>
    <div class="col-lg-4">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-danger" href="?modulo=principal/monitorar-ppa&acao=A&suoid=<?php echo $suoid; ?>">
                <i class="fa fa-list"></i> Monitorar PPA
            </a>
            <a class="btn btn-success" href="?modulo=principal/monitorar-pnc&acao=A&suoid=<?php echo $suoid; ?>">
                <i class="fa fa-list"></i> Monitorar PNC
            </a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <?php $aPis = (new Pi_PlanoInterno())->recuperarPorUnidade($suoid); ?>
                    <h5>Meus Projetos <span style="color: red;">(<?php echo count($aPis); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    $aObjetivos = [];
                    if(count($aPis)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                <tr class="text-center">
                                    <th>Status</th>
                                    <th>PI</th>
                                    <th>Título</th>
                                    <th>Vl. Total (R$)</th>
                                    <th>Físico</th>
                                    <th>Unidade</th>
                                    <th>Produto</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                $total = 0;

                                foreach($aPis as $dados){
                                    $status = $dados['acoid'] ?
                                        '<a href="?modulo=principal/monitorar-pi&acao=A&pliid=' . $dados['pliid'] . '"><span class="label label-primary">Preenchida</span></a>' :
                                        '<a href="?modulo=principal/monitorar-pi&acao=A&pliid=' . $dados['pliid'] . '&suoid=' . $dados['suoid'] . '"><span class="label label-danger">Pendente</span></a>';
                                    if($dados['oppcod']){
                                        $aObjetivos[$dados['oppcod']] = $dados;
                                    }
                                    $total += $dados['valor'];
                                    ?>
                                    <tr>
                                        <td><?php echo $status; ?></td>
                                        <td>
                                            <a href="javascript: exibirEspelhoPi('<?php echo $dados['pliid']; ?>')"><?php echo $dados['plicod'] ?></a>
                                        </td>
                                        <td><?php echo $dados['plititulo'] ?></td>
                                        <td class="text-right"><?php echo number_format($dados['valor'], 2, ',', '.'); ?></td>
                                        <td class="text-right"><?php echo number_format($dados['picquantidade'], 2, ',', '.'); ?></td>
                                        <td><?php echo $dados['pumnome'] ?></td>
                                        <td><?php echo $dados['pprnome'] ?></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                                <tfoot>
                                <tr>
                                    <th colspan="3">TOTAL</th>
                                    <th class="text-right"><?php echo number_format($total, 2, ',', '.'); ?></th>
                                    <th colspan="3"></th>
                                </tr>
                                </tfoot>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
        </div>
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Objetivos PPA que Contribuo <span style="color: red;">(<?php echo count($aObjetivos); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    if(count($aObjetivos)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                <tr class="text-center">
                                    <th>Objetivo</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php
                                $total = 0;
                                foreach($aObjetivos as $dados){ ?>
                                    <tr>
                                        <td><?php echo $dados['oppcod'] . ' - ' . $dados['oppnome'] ?></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <?php $aPos = (new Spo_Model_PtresSubunidade())->recuperarPorUnidade($suoid); ?>
                    <h5>Meus Planos Orçamentários <span style="color: red;">(<?php echo count($aPos); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    if(count($aPis)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                <tr class="text-center">
                                    <th>Funcional</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aPos as $dados){ ?>
                                    <tr>
                                        <td><?php echo $dados['funcional'] . ' - ' . $dados['plodsc'] ?></td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
        </div>
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <?php $aMetasPpa = $mAcompanhamento->getMetasPpaComPi($suoid); ?>
                    <h5>Minhas Metas PPA <span style="color: red;">(<?php echo count($aMetasPpa); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    if(count($aMetasPpa)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                <tr class="text-center">
                                    <th>Status</th>
                                    <th>Meta</th>
                                    <th>Qtd. Prevista</th>
                                    <th>Qtd. Alcançada</th>
                                    <th>PIs</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aMetasPpa as $dados){
                                    $status = $dados['acoid'] ?
                                        '<a href="?modulo=principal/monitorar-ppa&acao=A&acoid=' . $dados['acoid'] . '"><span class="label label-primary">Preenchida</span></a>' :
                                        '<a href="?modulo=principal/monitorar-ppa&acao=A&mppid=' . $dados['mppid'] . '&suoid=' . $dados['suoid'] . '"><span class="label label-danger">Pendente</span></a>';
                                    ?>
                                    <tr>
                                        <td class="text-center"><?php echo $status; ?></td>
                                        <td>
                                            <?php
                                            echo $dados['mppcod'] . " - " . $dados['mppdsc'];
                                            if($dados['compartilhada'] == 't'){
                                                echo ' <i style="font-size: 15px; color: #6892ff;" class="fa fa-handshake-o link" title="Meta Compartilhada com outras Subunidades."></i>';
                                            }
                                            ?>
                                        </td>
                                        <td class="text-right"><?php echo number_format($dados['mppquantidade'], 2, ',', '.'); ?></td>
                                        <td class="text-right"><?php echo number_format($dados['acoquantidade'], 2, ',', '.'); ?></td>
                                        <td class="text-center">
                                            <a class="detalhe-pi" href="?modulo=principal/monitorar-ppa&acao=A&req=detalhe-pi&mppid=<?php echo $dados['mppid']; ?>&suoid=<?php echo $dados['suoid']; ?>"><?php echo $dados['pis']; ?></a>
                                        </td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
        </div>
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <?php $aIndicadoresPnc = $mAcompanhamento->getIndicadoresPncComPi($suoid); ?>
                    <h5>Meus Indicadores PNC <span style="color: red;">(<?php echo count($aIndicadoresPnc); ?>)</span></h5>
                </div>
                <div class="ibox-content">
                    <?php
                    $aIndicadoresPnc = $mAcompanhamento->getIndicadoresPncComPi($suoid);
                    if(count($aIndicadoresPnc)){ ?>
                        <div class="table-responsive">
                            <table class="table table-striped table-bordered table-hover table-condensed" >
                                <thead>
                                <tr class="text-center">
                                    <th>Status</th>
                                    <th>Indicador</th>
                                    <th>Qtd. Prevista</th>
                                    <th>Qtd. Alcançada</th>
                                    <th>PIs</th>
                                </tr>
                                </thead>
                                <tbody>
                                <?php foreach($aIndicadoresPnc as $dados){
                                    $status = $dados['acoid'] ?
                                        '<a href="?modulo=principal/monitorar-pnc&acao=A&acoid=' . $dados['acoid'] . '"><span class="label label-primary">Preenchida</span></a>' :
                                        '<a href="?modulo=principal/monitorar-pnc&acao=A&ipnid=' . $dados['ipnid'] . '&suoid=' . $dados['suoid'] . '"><span class="label label-danger">Pendente</span></a>';
                                    ?>

                                    <tr>
                                        <td class="text-center"><?php echo $status; ?></td>
                                        <td title="Meta <?php echo $dados['mpncod'] . " - " . $dados['mpndsc']; ?>"><?php echo $dados['ipncod'] . " - " . $dados['ipndsc']; ?></td>
                                        <td class="text-right"><?php echo number_format($dados['ipnquantidade'], 2, ',', '.'); ?></td>
                                        <td class="text-right"><?php echo number_format($dados['acoquantidade'], 2, ',', '.'); ?></td>
                                        <td class="text-center">
                                            <a class="detalhe-pi" href="?modulo=principal/monitorar-pnc&acao=A&req=detalhe-pi&ipnid=<?php echo $dados['ipnid']; ?>&suoid=<?php echo $dados['suoid']; ?>"><?php echo $dados['pis']; ?></a>
                                        </td>
                                    </tr>
                                <?php } ?>
                                </tbody>
                            </table>
                        </div>
                    <?php } else {
                        echo '<div class="alert alert-danger">Nenhum registro cadastrado</div>';
                    } ?>
                </div>
            </div>
        </div>
    </div>
</div>

<!-- Modal -->
<div class="modal fade" id="detalhe-pi" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Pi's Vinculados</h4>
            </div>
            <div class="modal-body">

            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $('#suoid').change(function(){
            window.location.href = '?modulo=inicio&acao=C&suoid=' + $(this).val();
        });

        $('.detalhe-pi').click(function(){
            var url = $(this).attr('href');
            $('#detalhe-pi .modal-body').load(url);
            $('#detalhe-pi').modal();
            return false;
        });
    })

    /**
     * Exibe popup com Detalhes do pi. Tela de Espelho de PI.
     *
     * @returns VOID
     */
    function exibirEspelhoPi(pliid){
        window.open(
            '?modulo=inicio&acao=C&req=espelho-pi&pliid='+ pliid,
            'popup_espelho_pi',
            'width=780,height=1000,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    }
</script>