<?php

include_once APPRAIZ . "includes/library/simec/Grafico.php";

include_once '_funcoespi.php';

require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$oPlanoInterno = new Pi_PlanoInterno();

switch ($_REQUEST['req']) {
    case 'detalhe-subunidade':
        $oPlanoInterno->recuperarPiPorSubunidade($_REQUEST['suoid'], 'fnc');
        die;
    case 'detalhe-funcional':
        $oPlanoInterno->recuperarPiPorFuncional($_REQUEST['ptrid'], 'fnc');
        die;
}

$filtros = array(
    "suo.prsano = '". $_SESSION['exercicio']. "'",
    'suo.unofundo = TRUE',
);
# Filtro por Subunidades do perfil do usuário logado.
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}
$aDadosPorAcao = $oPlanoInterno->recuperarDadosFncPorAcao($_SESSION['exercicio']);
$aValoresEstado = $oPlanoInterno->recuperarValoresPorEstado($_SESSION['exercicio']);
$aValoresExecucao = $oPlanoInterno->recuperarValoresPorExecucao($_SESSION['exercicio']);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2 style="color: red;">Painel de Acompanhamento FNC</h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
            <a class="btn btn-success" href="planacomorc.php?modulo=principal/unidade/painel&acao=A"><i class="fa fa-area-chart"></i> Painel</a>
            <a class="btn btn-danger" href="planacomorc.php?modulo=principal/unidade/listapimanter_fnc&acao=A"><i class="fa fa-list"></i> Lista de PI - FNC</a>
            <a class="btn btn-warning" href="planacomorc.php?modulo=principal/unidade/cadastro_pi_fnc&acao=A"><i class="fa fa-plus-circle"></i> Criar PI - FNC</a>
            <a class="btn btn-primary" href="planacomorc.php?modulo=relatorio/geral&acao=A&fundo=t"><i class="fa fa-file-excel-o"></i> Relatório Geral</a>
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="col-md-12">

            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Acompanhamento Geral FNC - Por Ação</h5>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th>Ação</th>
                                <th>Dotação</th>
                                <th>Apresentado</th>
                                <th>Selecionado</th>
                                <th>Aprovado</th>
                                <th>Liberado</th>
                                <th>Limite</th>
                                <th>Empenhado</th>
                                <th>Pago</th>
                                <th>Liquidado</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php
                            $funcionalAtual = '';
                            foreach($aDadosPorAcao as $dados){

                                $total['dotacao'][$dados['tipo']] += $dados['dotacao'];

                                $style = '';
                                if($funcionalAtual != $dados['funcional']){
                                    $style = 'style="border-top: 2px #acacac solid"';
                                    $funcionalAtual = $dados['funcional'];
                                }
                                ?>
                                <tr class="text-right" <?php echo $style; ?>>
                                    <td style="text-align: left"><?php echo $dados['funcional'] . ' (' . $dados['tipo'] . ') - ' . $dados['acatitulo'] . ': ' . $dados['plodsc'] . ' (RP ' . $dados['irpcod'] . ')'; ?></td>

                                    <td><?php echo number_format($dados['dotacao'], 2, ',', '.'); ?></td>
                                    <?php
                                    // VALOR APRESENTADO
                                    $valor = $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_EM_ANALISE][$dados['tipo']] +
                                             $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_DELIBERACAO_CFNC][$dados['tipo']] +
                                             $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_SELECIONADO_CFNC][$dados['tipo']] +
                                             $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_APROVADO][$dados['tipo']];
                                    $total['APRESENTADO'][$dados['tipo']] += $valor;
                                    $disponivel = $dados['dotacao'] - $valor;
                                    ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>;">
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                    <?php
                                    // VALOR SELECIONADO
                                    $valor = !empty($aValoresEstado[$dados['ptrid']][ESD_FNC_PI_SELECIONADO_CFNC][$dados['tipo']]) ? $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_SELECIONADO_CFNC][$dados['tipo']] : '0';
                                    $total[ESD_FNC_PI_SELECIONADO_CFNC][$dados['tipo']] += $valor;
                                    $disponivel = $dados['dotacao'] - $valor;
                                    ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>;">
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                    <?php
                                    // VALOR APRESENTADO
                                    $valor = !empty($aValoresEstado[$dados['ptrid']][ESD_FNC_PI_APROVADO][$dados['tipo']]) ? $aValoresEstado[$dados['ptrid']][ESD_FNC_PI_APROVADO][$dados['tipo']] : '0';
                                    $total[ESD_FNC_PI_APROVADO][$dados['tipo']] += $valor;
                                    $disponivel = $dados['dotacao'] - $valor;
                                    ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>;">
                                        <?php echo $valor ? number_format($valor, 2, ',', '.') : '-'; ?>
                                    </td>
                                    <td> - </td>
                                    <td> - </td>

                                    <td>
                                        <?php
                                        // VALOR EMPENHADO
                                        $valor = !empty($aValoresExecucao[$dados['ptrid']][$dados['tipo']]['vlrempenhado']) ? $aValoresEstado[$dados['ptrid']][$dados['tipo']]['vlrempenhado'] : '0';
                                        $total['vlrempenhado'][$dados['tipo']] += $valor;
                                        echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                    <td>
                                        <?php
                                        // VALOR PAGO
                                        $valor = !empty($aValoresExecucao[$dados['ptrid']][$dados['tipo']]['vlrpago']) ? $aValoresEstado[$dados['ptrid']][$dados['tipo']]['vlrpago'] : '0';
                                        $total['vlrpago'][$dados['tipo']] += $valor;
                                        echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                    <td>
                                        <?php
                                        // VALOR LIQUIDADO
                                        $valor = !empty($aValoresExecucao[$dados['ptrid']][$dados['tipo']]['vlrliquidado']) ? $aValoresEstado[$dados['ptrid']][$dados['tipo']]['vlrliquidado'] : '0';
                                        $total['vlrliquidado'][$dados['tipo']] += $valor;
                                        echo $valor ? number_format($valor, 2, ',', '.') : '-';
                                        ?>
                                    </td>
                                </tr>
                            <?php } ?>
                            </tbody>
                            <tfoot>
                                <tr class="text-right" style="border-top: 2px #acacac solid">
                                    <td style="text-align: left">TOTAL CUSTEIO</td>
                                    <td><?php echo number_format($total['dotacao']['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['APRESENTADO']['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total[ESD_FNC_PI_SELECIONADO_CFNC]['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total[ESD_FNC_PI_APROVADO]['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td> - </td>
                                    <td> - </td>
                                    <td><?php echo number_format($total['vlrempenhado']['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['vlrpago']['CUSTEIO'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['vlrliquidado']['CUSTEIO'], 2, ',', '.'); ?></td>
                                </tr>
                                <tr class="text-right">
                                    <td style="text-align: left">TOTAL CAPITAL</td>
                                    <td><?php echo number_format($total['dotacao']['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['APRESENTADO']['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total[ESD_FNC_PI_SELECIONADO_CFNC]['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total[ESD_FNC_PI_APROVADO]['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td> - </td>
                                    <td> - </td>
                                    <td><?php echo number_format($total['vlrempenhado']['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['vlrpago']['CAPITAL'], 2, ',', '.'); ?></td>
                                    <td><?php echo number_format($total['vlrliquidado']['CAPITAL'], 2, ',', '.'); ?></td>
                                </tr>
                                <tr class="text-right">
                                    <th style="text-align: left">TOTAL</th>
                                    <th><?php echo number_format(array_sum($total['dotacao']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total['APRESENTADO']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total[ESD_FNC_PI_SELECIONADO_CFNC]), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total[ESD_FNC_PI_APROVADO]), 2, ',', '.'); ?></th>
                                    <th> - </th>
                                    <th> - </th>
                                    <th><?php echo number_format(array_sum($total['vlrempenhado']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total['vlrpago']), 2, ',', '.'); ?></th>
                                    <th><?php echo number_format(array_sum($total['vlrliquidado']), 2, ',', '.'); ?></th>
                                </tr>
                            </tfoot>
                        </table>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>