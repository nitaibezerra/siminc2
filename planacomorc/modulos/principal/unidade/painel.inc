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
        $oPlanoInterno->recuperarPiPorSubunidade($_REQUEST['suoid']);
        die;
    case 'detalhe-funcional':
        $oPlanoInterno->recuperarPiPorFuncional($_REQUEST['ptrid']);
        die;
}

$filtros = array(
    "suo.prsano = '". $_SESSION['exercicio']. "'",
    'suo.unofundo = FALSE',
);
# Filtro por Subunidades do perfil do usuário logado.
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}
$aPropostas = $oPlanoInterno->recuperarExecucaoOrcamentaria($filtros);
//var_dump($aPropostas);
//die('painel');

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";


?>

<style>
    .detalhe-subunidade, .detalhe-funcional{
        cursor: pointer;
    }

    .title-disponivel{
        cursor: help;
    }
    
    .linha_subunidade {
        background-color: rgba(241, 246, 215, 0.3) !important;
        font-weight: bold;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-4">
        <h2>Painel de Acompanhamento</h2>
    </div>
    <div class="col-lg-8">
        <div style="padding-top: 10px;" class="text-right">
<!--            <a class="btn btn-success" href="planacomorc.php?modulo=principal/unidade/painel_fnc&acao=A"><i class="fa fa-area-chart"></i> Painel FNC</a>-->
            <a class="btn btn-danger" href="planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A"><i class="fa fa-list"></i> Lista de PI</a>
            <a class="btn btn-success" href=""planacomorc.php?modulo=principal/unidade/cadastro_pi&acao=A"><i class="fa fa-plus-circle"></i> Criar PI</a>
<!--            <a class="btn btn-primary" href="planacomorc.php?modulo=relatorio/geral&acao=A"><i class="fa fa-file-excel-o"></i> Relatório Geral</a>-->
        </div>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <?php if (!$listaSubUnidadeUsuario): ?>
            <div class="col-md-12">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Valores por Unidades</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        $estatistica = $oPlanoInterno->recuperarEstatisticaPagamento($_SESSION['exercicio']);
                        $grafico = new Grafico(Grafico::K_TIPO_COLUNA, false);
                        $grafico->setFormatoTooltip(Grafico::K_TOOLTIP_DECIMAL_0)->gerarGrafico($estatistica);
                        ?>
                    </div>
                </div>
            </div>
        <?php endif; ?>

        <div class="col-md-12">

            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Acompanhamento de Plano Interno</h5>
                </div>
                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th rowspan="2" style="border-right: 1px black solid;">
                                    <i class="fa fa-plus detalhar-todos link"></i>&nbsp;&nbsp;Unidade
                                </th>
                                <th colspan="2" style="border-right: 1px black solid;">Custeio</th>
                                <th colspan="2" style="border-right: 1px black solid;">Capital</th>
                                <th colspan="2" style="border-right: 1px black solid;">Total</th>
                                <th rowspan="2" style="border-right: 1px black solid;">Limite</th>
                            </tr>
                            <tr class="text-center">
                                <th>Dotação</th>
                                <th style="border-right: 1px black solid;">Planejado</th>
                                <th>Dotação</th>
                                <th style="border-right: 1px black solid;">Planejado</th>
                                <th>Dotação</th>
                                <th style="border-right: 1px black solid;">Planejado</th>
                            </tr>
                            </thead>
                            <tbody>
                            <?php foreach($aPropostas as $dados){
                                $dado = $dados['detalhe'][0];
                                ?>
                                <tr class="linha_subunidade">
                                    <td style="border-right: 1px black solid;">
                                        <i class="fa fa-plus detalhar-linha link" data-suoid="<?php echo $dado['suoid']; ?>"></i>&nbsp;&nbsp;
                                        <span class="detalhe-subunidade" data-suoid="<?php echo $dado['suoid']; ?>">
                                            <?php echo $dado['unosigla'] . ' - ' . $dado['suonome'] . ' (' . $dado['suocod'] . ')'; ?>
                                        </span>
                                    </td>

                                    <td class="text-right"><?php echo number_format($dados['dotacaoCusteio'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dados['dotacaoCusteio'] - $dados['planejadoCusteio']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['planejadoCusteio'], 0, ',', '.'); ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format($dados['dotacaoCapital'], 0, ',', '.'); ?></td>
                                    <?php $disponivel = $dados['dotacaoCapital'] - $dados['planejadoCapital']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['planejadoCapital'], 0, ',', '.'); ?>
                                    </td>

                                    <td class="text-right"><?php echo number_format(($dados['dotacaoCusteio'] + $dados['dotacaoCapital']), 0, ',', '.'); ?></td>
                                    <?php $disponivel = ($dados['dotacaoCusteio'] + $dados['dotacaoCapital']) - $dados['totalPlanejado']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dados['totalPlanejado'], 0, ',', '.'); ?>
                                    </td>

                                    <?php $disponivel = $dado['lmuvlr'] - $dados['totalPlanejado']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <?php echo number_format($dado['lmuvlr'], 0, ',', '.'); ?>
                                    </td>
                                </tr>
                                <?php foreach($dados['detalhe'] as $dado){ ?>
                                    <tr class="tr_<?php echo $dado['suoid']; ?> tr_detalhe" style="display: none;">
                                        <td style="padding-left: 30px; border-right: 1px black solid;" >
                                            <span class="detalhe-funcional" data-ptrid="<?php echo $dado['ptrid']; ?>">
                                                <?php echo $dado['funcional'] . ' - ' . $dado['acatitulo']; ?>
                                            </span>
                                        </td>

                                        <td class="text-right"><?php echo number_format($dado['ptrdotacao_custeio'], 0, ',', '.'); ?></td>
                                        <?php $disponivel = $dado['ptrdotacao_custeio'] - $dado['picvalorcusteio']; ?>
                                        <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format($dado['picvalorcusteio'], 0, ',', '.'); ?>
                                        </td>

                                        <td class="text-right"><?php echo number_format($dado['ptrdotacao_capital'], 0, ',', '.'); ?></td>
                                        <?php $disponivel = $dado['ptrdotacao_capital'] - $dado['picvalorcapital']; ?>
                                        <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format($dado['picvalorcapital'], 0, ',', '.'); ?>
                                        </td>

                                        <td class="text-right"><?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 0, ',', '.'); ?></td>
                                        <?php $disponivel = ($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']) - ($dado['picvalorcusteio'] + $dado['picvalorcapital']); ?>
                                        <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right title-disponivel" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                            <?php echo number_format(($dado['picvalorcusteio'] + $dado['picvalorcapital']), 0, ',', '.'); ?>
                                        </td>
                                        <td class="text-center" style="border-right: 1px black solid;"> - </td>
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
bootstrapPopup("PI's da Unidade", 'detalhe-subunidade', '', array('fechar'), array('tamanho' => 'lg'));
bootstrapPopup("PI's da Funcional", 'detalhe-funcional', '', array('fechar'), array('tamanho' => 'lg'));
?>

<script>
    $(document).ready(function(){

        $('.detalhe-subunidade').click(function(){
            $('#detalhe-subunidade .modal-body').load('planacomorc.php?modulo=principal/unidade/painel&acao=A&req=detalhe-subunidade&suoid=' + $(this).data('suoid'));
            $('#detalhe-subunidade').modal();
        });

        $('.detalhe-funcional').click(function(){
            $('#detalhe-funcional .modal-body').load('planacomorc.php?modulo=principal/unidade/painel&acao=A&req=detalhe-funcional&ptrid=' + $(this).data('ptrid'));
            $('#detalhe-funcional').modal();
        });

        $('.detalhar-linha').click(function(){

            var suoid = $(this).data('suoid');

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.tr_' + suoid).hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.tr_' + suoid).show('slow');
            }
        });

        $('.detalhar-todos').click(function(){

            if($(this).hasClass('fa-minus')){
                $(this).removeClass('fa-minus').addClass('fa-plus');
                $('.detalhar-linha').removeClass('fa-minus').addClass('fa-plus');
                $('.tr_detalhe').hide();
            } else {
                $(this).removeClass('fa-plus').addClass('fa-minus');
                $('.detalhar-linha').removeClass('fa-plus').addClass('fa-minus');
                $('.tr_detalhe').show('slow');
            }
        });
    });
</script>
