<?php

//ver('OK', d);

//include_once APPRAIZ . "includes/library/simec/Grafico.php";

//include_once APPRAIZ. 'www/planacomorc/_funcoespi.php';
//
//require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$oPlanoInterno = new Pi_PlanoInterno();

$subunidade = new Public_Model_SubUnidadeOrcamentaria();

switch ($_REQUEST['req']) {
    case 'salvar':
        $cUnidadegestoraLimite = new Planacomorc_Controller_Unidadegestoralimite();
        $cUnidadegestoraLimite->salvar($_REQUEST);
        die;
    case 'salvar-dotacao':
        $mPtresSubunidade = new Spo_Model_PtresSubunidade($_GET['psuid']);
        $mPtresSubunidade->popularDadosObjeto($_GET);
        $mPtresSubunidade->salvar(null, null, ['ptrdotacaocusteio', 'ptrdotacaocapital']);
        $mPtresSubunidade->commit();
        die;
}

$filtros = array(
    "suo.prsano = '". (int)$_SESSION['exercicio']. "'",
    'suo.unofundo = FALSE',
);

# Filtro por Subunidades do perfil do usuário logado.
if($listaSubUnidadeUsuario){
    $filtros[] = "suo.suocod IN('". join("','", $listaSubUnidadeUsuario). "')";
}
//$aPropostas = $oPlanoInterno->recuperarExecucaoOrcamentaria($filtros);
//$listaRp = $subunidade->executar();
$aFnc = $oPlanoInterno->recuperarExecucaoOrcamentariaFnc();

/**
 * Cabeçalho padrão do sistema.
 * @see cabecalho.inc
 */
include APPRAIZ . "includes/cabecalho.inc";
?>

<style>
    .title-disponivel{
        cursor: help;
    }

    .linha_subunidade {
        background-color: rgba(241, 246, 215, 0.3) !important;
        font-weight: bold;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">

        <div class="col-md-12">

            <div class="ibox float-e-margins">

                <div class="ibox-content">
                    <div class="table-responsive">
                        <table class="table table-bordered table-hover" >
                            <thead>
                            <tr class="text-center">
                                <th style="border-right: 1px black solid;">
                                    <i class="fa fa-plus detalhar-todos link"></i>&nbsp;&nbsp;Unidade
                                </th>
                                <th width="10%" style="border-right: 1px black solid;">Custeio</th>
                                <th width="10%" style="border-right: 1px black solid;">Capital</th>
                                <th width="10%" style="border-right: 1px black solid;">Total</th>
                                <th width="10%" style="border-right: 1px black solid;">Limite</th>
                            </tr>
                            </thead>
                            <tbody>

                            <?php foreach($aFnc as $dados){
                                $dado = $dados['detalhe'][0];
                                ?>
                                <tr class="linha_subunidade" style="background-color: #ffe6e1 !important;">
                                    <td style="border-right: 1px black solid;">
                                        <i class="fa fa-plus detalhar-linha link" data-suoid="<?php echo $dado['unoid']; ?>"></i>&nbsp;&nbsp;
                                        <span class="detalhe-subunidade" data-suoid="<?php echo $dado['unoid']; ?>">
                                            <?php echo $dado['unosigla'] . ' - ' . $dado['unonome'] . ' (' . $dado['unocod'] . ')'; ?>
                                        </span>
                                    </td>

                                    <td class="text-right" style="border-right: 1px black solid;"><?php echo number_format($dados['dotacaoCusteio'], 0, ',', '.'); ?></td>

                                    <td class="text-right" style="border-right: 1px black solid;"><?php echo number_format($dados['dotacaoCapital'], 0, ',', '.'); ?></td>

                                    <td class="text-right" style="border-right: 1px black solid;"><?php echo number_format(($dados['dotacaoCusteio'] + $dados['dotacaoCapital']), 0, ',', '.'); ?></td>

                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right">
                                        <input name="limites" type="text" class="input_valor salvar-limite-fnc" value="<?php echo $dado['lmuvlr']; ?>" data-unoid="<?php echo $dado['unoid']; ?>" data-lmuid="<?php echo $dado['lmuid']; ?>" >
                                    </td>
                                </tr>
                                <?php foreach($dados['detalhe'] as $dado){ ?>
                                    <tr class="tr_<?php echo $dado['unoid']; ?> tr_detalhe" style="display: none; background-color: #fff0e6 !important;">
                                        <td style="padding-left: 30px; border-right: 1px black solid;" >
                                            <span class="detalhe-funcional" data-ptrid="<?php echo $dado['ptrid']; ?>">
                                                <?php echo $dado['funcional'] . ' - ' . $dado['acatitulo'] . ' (RP ' . $dado['irpcod'] . ')'; ?>
                                            </span>
                                            <?php if($dado['compartilhada'] > 1): ?>
                                                &nbsp;<i class="fa fa-handshake-o link" title="Funcional Compartilhada com outras Subunidades."></i>
                                            <?php endif; ?>
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;">
                                            <input name="limites[]" type="text" class="input_valor salvar-dotacao-fnc" value="<?php echo number_format($dado['ptrdotacao_custeio'], 2, '.', ''); ?>" data-ptrid="<?php echo $dado['ptrid']; ?>" data-campo="ptrdotacaocusteio" >
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;">
                                            <input name="limites[]" type="text" class="input_valor salvar-dotacao-fnc" value="<?php echo number_format($dado['ptrdotacao_capital'], 2, '.', ''); ?>" data-ptrid="<?php echo $dado['ptrid']; ?>" data-campo="ptrdotacaocapital" >
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;"><?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 2, ',', '.'); ?></td>
                                        <td class="text-center" style="border-right: 1px black solid;"> - </td>
                                    </tr>
                                <?php } ?>
                            <?php } ?>
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

                                    <td class="text-right" style="border-right: 1px black solid;"><?php echo number_format($dados['dotacaoCusteio'], 0, ',', '.'); ?></td>

                                    <td class="text-right" style="border-right: 1px black solid;"><?php echo number_format($dados['dotacaoCapital'], 0, ',', '.'); ?></td>

                                    <td class="text-right" style="border-right: 1px black solid;"><?php echo number_format(($dados['dotacaoCusteio'] + $dados['dotacaoCapital']), 0, ',', '.'); ?></td>

                                    <?php $disponivel = $dado['lmuvlr'] - $dados['totalPlanejado']; ?>
                                    <td style="color: <?php echo $oPlanoInterno->getCorPainelPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                        <input name="limites[]" type="text" class="input_valor salvar-limite" value="<?php echo $dado['lmuvlr']; ?>" data-ungcod="<?php echo $dado['suocod']; ?>" data-lmuid="<?php echo $dado['lmuid']; ?>" >
                                    </td>
                                </tr>
                                <?php foreach($dados['detalhe'] as $dado){ ?>
                                    <tr class="tr_<?php echo $dado['suoid']; ?> tr_detalhe" style="display: none;">
                                        <td style="padding-left: 30px; border-right: 1px black solid;" >
                                            <span class="detalhe-funcional" data-ptrid="<?php echo $dado['ptrid']; ?>">
                                                <?php echo $dado['funcional'] . ' - ' . $dado['acatitulo'] . ' (RP ' . $dado['irpcod'] . ')'; ?>
                                            </span>
                                            <?php if($dado['compartilhada'] > 1): ?>
                                                &nbsp;<i class="fa fa-handshake-o link" title="Funcional Compartilhada com outras Subunidades."></i>
                                            <?php endif; ?>
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;">
                                            <input name="limites[]" type="text" class="input_valor salvar-dotacao" value="<?php echo number_format($dado['ptrdotacao_custeio'], 2, '.', ''); ?>" data-psuid="<?php echo $dado['psuid']; ?>" data-campo="ptrdotacaocusteio" >
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;">
                                            <input name="limites[]" type="text" class="input_valor salvar-dotacao" value="<?php echo number_format($dado['ptrdotacao_capital'], 2, '.', ''); ?>" data-psuid="<?php echo $dado['psuid']; ?>" data-campo="ptrdotacaocapital" >
                                        </td>

                                        <td class="text-right" style="border-right: 1px black solid;"><?php echo number_format(($dado['ptrdotacao_custeio'] + $dado['ptrdotacao_capital']), 2, ',', '.'); ?></td>
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

<script>
    $(document).ready(function(){

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

        $('.input_valor').keyup(function(){
            $(this).val(mascaraglobal('###.###.###.###,##', $(this).val()));
        }).keyup();

        $('.salvar-limite').change(function(){
            var lmuvlr = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var ungcod = $(this).data('ungcod');
            var lmuid = $(this).data('lmuid');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar&lmuvlr=' + lmuvlr + '&ungcod=' + ungcod + '&lmuid=' + lmuid
            });
        });

        $('.salvar-limite-fnc').change(function(){
            var lmuvlr = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var unoid = $(this).data('unoid');
            var lmuid = $(this).data('lmuid');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar&lmuvlr=' + lmuvlr + '&unoid=' + unoid + '&lmuid=' + lmuid
            });
        });

        $('.salvar-dotacao').change(function(){
            var valor = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var campo = $(this).data('campo');
            var psuid = $(this).data('psuid');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar-dotacao&psuid=' + psuid + '&' + campo + '=' + valor
            });
        });

        $('.salvar-dotacao-fnc').change(function(){
            var valor = $(this).val() ? str_replace(['.', ','], ['', '.'], $(this).val()) : 0;
            var campo = $(this).data('campo');
            var ptrid = $(this).data('ptrid');
            $.ajax({
                url: 'planacomorc.php?modulo=apoio/unidadegestora-limite&acao=A&req=salvar-dotacao-fnc&ptrid=' + ptrid + '&' + campo + '=' + valor
            });
        });

    });

</script>
