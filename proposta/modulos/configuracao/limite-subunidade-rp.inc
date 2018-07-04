<?php

/**
 * @todo retirar include e dependencia
 */
include_once APPRAIZ . "monitora/classes/Pi_PlanoInterno.class.inc";

# Verifica se o usuário tem vinculo de Sub-Unidades no seu Perfil
$listaSubUnidadeUsuario = buscarSubUnidadeUsuario((object) array('usucpf' => $_SESSION['usucpf']));

$oPlanoInterno = new Pi_PlanoInterno();

$subunidade = new Public_Model_SubUnidadeOrcamentaria();

switch($_REQUEST['req']){
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

$listaSubunidadeTotais = $subunidade->buscarPropostaTotaisLimites((object)array(
    'exercicio' => $_SESSION['exercicio']
));
//$aFnc = $oPlanoInterno->recuperarExecucaoOrcamentariaFnc();

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
                                <th width="10%" style="border-right: 1px black solid;">Total Pré-PI</th>
                                <th width="10%" style="border-right: 1px black solid;">Limite RP</th>
                            </tr>
                            </thead>
                            <?php if($listaSubunidadeTotais): ?>
                                <tbody>
                                    <?php foreach($listaSubunidadeTotais as $subunidade): ?>
                                        <?php $disponivel = $dado['total_limite_rp'] - $subunidade['total_pre_pi']; ?>
                                        <tr class="linha_subunidade">
                                            <td style="border-right: 1px black solid;">
                                                <i class="fa fa-plus detalhar-linha link" data-suoid="<?php echo $subunidade['suoid']; ?>"></i>&nbsp;&nbsp;
                                                <span class="detalhe-subunidade" data-suoid="<?php echo $subunidade['suoid']; ?>">
                                                    <?php echo $subunidade['unosigla']. ' - '. $subunidade['suonome']. ' (' . $subunidade['suocod']. ')'; ?>
                                                </span>
                                            </td>
                                            <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right link" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                                <?php echo number_format($subunidade['total_pre_pi'], 0, ',', '.'); ?>
                                            </td>
                                            <td style="color: <?php echo controlarCorPorValorDisponivel($disponivel); ?>; border-right: 1px black solid;" class="text-right link" title="Disponível: <?php echo number_format($disponivel, 0, ',', '.'); ?>">
                                                <?php echo number_format($subunidade['total_limite_rp'], 0, ',', '.'); ?>
                                            </td>
                                        </tr>

                                    <?php endforeach; ?>
                                </tbody>
                            <?php endif; ?>
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
