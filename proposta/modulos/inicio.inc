<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

$mProposta = new Proposta_Model_Proposta();
$mPtres = new Monitora_Model_Ptres();
$mPrePlanoInterno = new Proposta_Model_PrePlanoInterno();

$momento = 1000;

switch ($_REQUEST['req']) {
    case 'importar-siop':
        $mPtres->importarSiop($_SESSION['exercicio'], $momento);
        simec_redirecionar('?modulo=inicio&acao=C', 'success');
        die;
    case 'gerar-planilha':
        $mProposta->gerarPlanilhaImportacao();
        die;
    case 'carregarGraficoUnidade':
        $mPrePlanoInterno->carregarGrafico();
        die;
    case 'carregarGraficoDireta':
        $mPrePlanoInterno->carregarGrafico(['unosigla' => 'MINC', 'unidades'=>"suocod not in ('420009', '420008')"]);
        die;
    case 'carregarGraficoCgconCogep':
        $mPrePlanoInterno->carregarGrafico(['unosigla' => 'MINC', 'unidades'=>"suocod in ('420009', '420008')"]);
        die;
}

$quantidadeSiop = $mPtres->recuperarQuantidadesSiop($momento);
$funcionaisSiop = $mPtres->recuperarFuncionaisSiop($momento);
$comparacaoSiop = $mPtres->recuperarComparacaoSiop($momento);

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-danger" href="?modulo=inicio&acao=C&req=gerar-planilha">Gerar Planilha</a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="col-md-3">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Funcionais SIOP</h5>
                </div>
                <div class="ibox-content">

                    <a class="btn btn-warning" id="btn-funcionais-siop"><i class="fa fa-search"></i>&nbsp;Ver Funcionais</a>
                    <a href="?modulo=inicio&acao=C&req=importar-siop" class="btn btn-danger" id="btn-salvar"><i class="fa fa-download"></i>&nbsp;Importar</a>

                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th>Carga</th>
                                <th>Quantidade</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($quantidadeSiop as $dado){ ?>
                                <tr>
                                    <td><?php echo $dado['descricao']; ?></td>
                                    <td align="right"><?php echo $dado['qtd']; ?></td>
                                </tr>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-3">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Comparativo SIOP/Propostas SIMINC</h5>
                </div>
                <div class="ibox-content">

                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <?php foreach($comparacaoSiop as $dado){ ?>
                                    <th><?php echo $dado['descricao']; ?></th>
                                <?php } ?>
                            </tr>
                            </thead>
                            <tbody>
                            <tr>
                                <?php foreach($comparacaoSiop as $dado){ ?>
                                    <td  align="right"><?php echo $dado['qtd']; ?></td>
                                <?php } ?>
                            </tr>
                            </tbody>
                        </table>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por Unidades</h5>
                </div>
                <div class="ibox-content">
                    <div class="tabs-container tabs">
                        <ul class="nav nav-tabs prodTabs">
                            <li class="active"><a data-toggle="tab" href="#tab-1" data-url="?modulo=inicio&acao=C&req=carregarGraficoUnidade"> Gráficos das Unidades</a></li>
                            <li class=""><a data-toggle="tab" href="#tab-2" data-url="?modulo=inicio&acao=C&req=carregarGraficoDireta"> Gráficos da Direta</a></li>
                            <li class=""><a data-toggle="tab" href="#tab-3" data-url="?modulo=inicio&acao=C&req=carregarGraficoCgconCogep"> Gráficos da CGCON/COGEP</a></li>
                        </ul>
                        <div class="tab-content">
                            <div id="tab-1" class="tab-pane active">
                                <?php $mPrePlanoInterno->carregarGrafico(); ?>
                            </div>
                            <div id="tab-2" class="tab-pane">
                                <!--O grafico 2 será carregado aqui via ajax-->
                            </div>
                            <div id="tab-3" class="tab-pane">
                                <!--O grafico 3 será carregado aqui via ajax-->
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>

    </div>
</div>

<!-- Modal -->
<div class="modal fade" id="modalFuncionaisSiop" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
    <div class="modal-dialog modal-lg" role="document">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
                <h4 class="modal-title" id="myModalLabel">Funcionais do SIOP</h4>
            </div>
            <div class="modal-body">
                <div class="table-responsive">
                    <table class="table table-bordered table-hover" >
                        <thead>
                        <tr class="text-center">
                            <th>Unidade</th>
                            <th>Ação</th>
                            <th>Programa</th>
                            <th>Localizador</th>
                            <th>Objetivo</th>
                            <th>PO</th>
                            <th>Título Ação</th>
                            <th>Título PO</th>
                        </tr>
                        </thead>
                        <tbody>
                        <?php foreach($funcionaisSiop as $dado){ ?>
                            <tr>
                                <td><?php echo $dado['codigoorgao']; ?></td>
                                <td><?php echo $dado['codigoacao']; ?></td>
                                <td><?php echo $dado['codigoprograma']; ?></td>
                                <td><?php echo $dado['codigolocalizador']; ?></td>
                                <td><?php echo $dado['codigoobjetivo']; ?></td>
                                <td><?php echo $dado['planoorcamentario']; ?></td>
                                <td><?php echo $dado['acatitulo']; ?></td>
                                <td><?php echo $dado['plotitulo']; ?></td>
                            </tr>
                        <?php } ?>
                        </tbody>
                    </table>
                </div>
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>

<script>
    $(document).ready(function(){

        $('.tabs').on('click', '.tablink, .prodTabs a',function (e) {
            e.preventDefault();
            var url = $(this).attr("data-url");

            if (typeof url !== "undefined") {
                var pane = $(this), href = this.hash;

                // ajax load from data-url
                $(href).empty().load(url, function(result){
                    pane.tab('show');
                });
            } else {
                $(this).tab('show');
            }
        });

        $('#btn-funcionais-siop').click(function(){
            $('#modalFuncionaisSiop').modal();
        });
    });

</script>

