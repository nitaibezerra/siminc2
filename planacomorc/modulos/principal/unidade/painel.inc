<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

$oPlanoInterno = new Pi_PlanoInterno();
/**
 * Cabeçalho padrão do simec.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";

// @TODO - Retirar ano fico de 2017 e colocar $_SESSION['exercicio'];

$aPropostas = $oPlanoInterno->recuperarExecucaoOrcamentaria(2017, $aFiltro);
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-danger" href="planacomorc.php?modulo=principal/unidade/listapimanter&acao=A"><i class="fa fa-list"></i> Lista de PI</a>
            <a class="btn btn-warning" href="planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A"><i class="fa fa-plus-circle"></i> Novo PI</a>
            <a class="btn btn-primary" href="planacomorc.php?modulo=relatorio/geral&acao=A"><i class="fa fa-file-excel-o"></i> Relatório Geral</a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Valores por Unidades</h5>
                </div>
                <div class="ibox-content">
                    <?php
                    $estatistica = $oPlanoInterno->recuperarEstatisticaPagamento(2017);
                    $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
                    $grafico->setColors("'#FFD700', '#55BF3B', '#FF6A6A'")->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($estatistica);
                    ?>
                </div>
            </div>
        </div>

        <div class="col-md-6">
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
            		<h5>Valores Autorizados por Unidade</h5>
            	</div>
            	<div class="ibox-content">
                    <?php
                    $estatistica = $oPlanoInterno->recuperarEstatisticaUnidade(2017);
                    $grafico->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($estatistica);
                    ?>
            	</div>
            </div>
        </div>

        <div class="col-md-6">
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
            		<h5>PI's por Unidade</h5>
            	</div>
            	<div class="ibox-content">
                    <?php
                    $estatistica = $oPlanoInterno->recuperarEstatisticaPiUnidade(2017);
                    $grafico->setTipo(Grafico::K_TIPO_PIZZA)->gerarGrafico($estatistica);
                    ?>
            	</div>
            </div>
        </div>

        <div class="col-md-12">

            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Acompanhamento de Plano Interno</h5>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover " >
                            <thead>
                            <tr class="text-center">
                                <th rowspan="2">Subunidade</th>
                                <th colspan="2">Custeio</th>
                                <th colspan="2">Capital</th>
                                <th colspan="2">Total</th>
                            </tr>
                            <tr class="text-center">
                                <th>Dotação</th>
                                <th>Planejado</th>
                                <th>Dotação</th>
                                <th>Planejado</th>
                                <th>Dotação</th>
                                <th>Planejado</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aPropostas as $dados){
                                $dado = $dados['detalhe'][0];
                                ?>
                                <tr class="info">
                                    <td><?php echo $dado['suonome'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')'; ?></td>

                                    <td class="text-right"><?php echo number_format($dados['dotacaoCusteio'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dados['dotacaoCusteio'] - $dados['planejadoCusteio']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>" class="text-right" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['planejadoCusteio'], 0, ',', '.'); ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format($dados['dotacaoCapital'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dados['dotacaoCapital'] - $dados['planejadoCapital']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>" class="text-right" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['planejadoCapital'], 0, ',', '.'); ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format($dados['totalDotacao'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dados['totalDotacao'] - $dados['totalPlanejado']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>" class="text-right" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['totalPlanejado'], 0, ',', '.'); ?>
                                    </td>
                                </tr>
                                <?php foreach($dados['detalhe'] as $dado){ ?>
                                    <tr class="tr_<?php echo $dado['suocod']; ?>">
                                        <td><?php echo $dado['funcional'] . ' - ' . $dado['acatitulo']; ?></td>

                                        <td class="text-right"><?php echo number_format($dado['ptrdotacao_custeio'], 0, ',', '.'); ?></td>
                                        <?php $disponivel = $dado['ptrdotacao_custeio'] - $dado['picvalorcusteio']; ?>
                                        <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>" class="text-right" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format($dado['picvalorcusteio'], 0, ',', '.'); ?>
                                        </td>

                                        <td class="text-right"><?php echo number_format($dado['ptrdotacao_capital'], 0, ',', '.'); ?></td>
                                        <?php $disponivel = $dado['ptrdotacao_capital'] - $dado['picvalorcapital']; ?>
                                        <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>" class="text-right" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format($dado['picvalorcapital'], 0, ',', '.'); ?>
                                        </td>

                                        <td class="text-right"><?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 0, ',', '.'); ?></td>
                                        <?php $disponivel = ($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']) - ($dado['picvalorcusteio'] + $dado['picvalorcapital']); ?>
                                        <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>" class="text-right" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format(($dado['picvalorcusteio'] + $dado['picvalorcapital']), 0, ',', '.'); ?>
                                        </td>
                                    </tr>
                                <?php } ?>
                            <?php } ?>
                            </tbody>
                        </table>
                    </div>

                </div>
            </div>
        </div>
    </div>
</div>

<?php
// -- popup de detalhamento do PI
bootstrapPopup('Dados do Plano Interno', 'detalhepi', '', array('fechar'), array('tamanho' => 'lg'));
?>

<script>
    $(document).ready(function(){

        function detalhePI(pliid) {
            $('#detalhepi .modal-body').empty();
            $.post("planacomorc.php?modulo=principal/unidade/listapimanter&acao=A&requisicao=detalharPi&pliid=" + pliid, function(html) {
                $('#detalhepi .modal-body').html(html);
                $('#detalhepi').modal();
            });
        }
    });

</script>
