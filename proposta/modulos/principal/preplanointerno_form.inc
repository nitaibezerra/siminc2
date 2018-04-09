<?php

$cPrePlanoInterno = new Proposta_Controller_PrePlanoInterno();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cPrePlanoInterno->salvar($_REQUEST);
		die;
	case 'excluir':
        $cPrePlanoInterno->excluir($_REQUEST['pliid']);
		die;
	case 'carregar-item':
        echo $simec->select('maiid', 'Item', null, (new Planacomorc_Model_ManutencaoItem())->recuperarSqlCombo(null, ["eqdid = " . (int) $_REQUEST['eqdid']]));
		die;
	case 'carregar-subitem':
        echo $simec->select('masid', 'SubItem', $mPrePlanoInterno->masid, (new Planacomorc_Model_ManutencaoSubItem())->recuperarSqlCombo(null, ["maiid = " . (int) $_REQUEST['maiid'] ]));
		die;
	case 'carregar-funcional':
        echo $simec->select('ptrid', 'Funcional', $mPrePlanoInterno->ptrid, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($_REQUEST['suoid'], $_REQUEST['eqdid']), ['required']);
		die;
}

$mPrePlanoInterno = new Proposta_Model_PrePlanoInterno($_REQUEST['pliid']);

$perfis = pegaPerfilGeral();


include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">

    <form id="formulario" name="formulario" method="post" class="form-horizontal">
        <input type="hidden" name="req" id="req" value="salvar" />
        <input name="pliid" id="pliid" type="hidden" value="<?php echo $mPrePlanoInterno->pliid; ?>">

        <div class="row">
            <div class="col-md-6">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Dados do Pré-Plano Interno</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        echo $simec->input('plititulo', 'Título', $mPrePlanoInterno->plititulo, ['required', 'maxlength' => 250]);
                        echo $simec->textarea('plidsc', 'Descrição / Finalidade', $mPrePlanoInterno->plidsc, ['required', 'maxlength' => 1000, 'cols'=>60, 'rows'=>6]);
                        ?>
                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Metas</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        echo $simec->select('oppid', 'Objetivo PPA', $mPrePlanoInterno->oppid, (new Public_Model_ObjetivoPpa())->recuperarSqlCombo(), ['required']);
                        echo $simec->select('mppid', 'Metas PPA', $mPrePlanoInterno->mppid, (new Public_Model_MetaPpa())->recuperarSqlCombo(), ['required']);
                        echo $simec->select('ippid', 'Iniciativa PPA', $mPrePlanoInterno->ippid, (new Public_Model_IniciativaPpa())->recuperarSqlCombo(), ['required']);
                        echo $simec->select('mpnid', 'Meta PNC', $mPrePlanoInterno->mpnid, (new Public_Model_MetaPnc())->recuperarSqlCombo(), ['required']);
                        echo $simec->select('ipnid', 'Indicador PNC', $mPrePlanoInterno->ipnid, (new Public_Model_IndicadorPnc())->recuperarSqlCombo(), ['required']);
                        ?>
                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Produto PI</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        echo $simec->select('pprid', 'Produto', $mPrePlanoInterno->pprid, ['required']);
                        echo $simec->select('pumid', 'Unidade de Medida', $mPrePlanoInterno->pumid, ['required']);
                        echo $simec->valor('pliquantidade', 'Quantidade', $mPrePlanoInterno->pliquantidade, ['required']);
                        ?>
                    </div>
                </div>

            </div>
            <div class="col-md-5">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Identificação</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        echo $simec->select('suoid', 'SubUnidade', $mPrePlanoInterno->suoid, (new Public_Model_SubUnidadeOrcamentaria())->recuperarSqlCombo(), ['required']);
                        echo $simec->select('eqdid', 'Enquadramento da Despesa', $mPrePlanoInterno->eqdid, (new Monitora_Model_PiEnquadramentoDespesa())->recuperarSqlCombo(), ['required']);
                        echo '<span id="span-item">' . $simec->select('maiid', 'Item', $mPrePlanoInterno->maiid, (new Planacomorc_Model_ManutencaoItem())->recuperarSqlCombo(null, ["eqdid = " . (int) $mPrePlanoInterno->eqdid ])) . '</span>';
                        echo '<span id="span-subitem">' . $simec->select('masid', 'SubItem', $mPrePlanoInterno->masid, (new Planacomorc_Model_ManutencaoSubItem())->recuperarSqlCombo(null, ["maiid = " . (int) $mPrePlanoInterno->maiid ])) . '</span>';
                        echo '<span id="span-funcional">' . $simec->select('ptrid', 'Funcional', $mPrePlanoInterno->ptrid, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($mPrePlanoInterno->suoid, $mPrePlanoInterno->eqdid)) . '</span>';
                        ?>
                    </div>
                </div>
            </div>

            <div class="col-md-6">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Informações Complementares</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        echo $simec->select('mdeid', 'Área Cultural', $mPrePlanoInterno->mdeid, (new Monitora_Model_PiModalidadeEnsino())->recuperarSqlCombo(), ['required']);
                        echo '<span id="span-segmento">' . $simec->select('neeid', 'Segmento Cultural', $mPrePlanoInterno->neeid, (new Monitora_Model_PiNivelEtapaEnsino())->recuperarSqlCombo(), ['required']) . '</span>';
                        ?>
                    </div>
                </div>
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Custeio e Capital</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        echo $simec->valor('plivalorcusteio', 'plivalorcusteio', $mPrePlanoInterno->plivalorcusteio, ['required']);
                        echo $simec->valor('plivalorcapital', 'plivalorcapital', $mPrePlanoInterno->plivalorcapital, ['required']);
                        ?>
                    </div>
                </div>
            </div>
        </div>

        <div class="form-group">
            <div class="text-center">
                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                <a href="?modulo=principal/preplanointerno&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                <?php if($mPrePlanoInterno->pliid){ ?>
                    <a href="?modulo=principal/preplanointerno_form&acao=A&req=excluir&pliid=<?php echo $mPrePlanoInterno->pliid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                <?php } ?>
            </div>
        </div>
    </form>
</div>

<script>
    $(function(){
        toggleItem();
        $('#eqdid').change(function(){
            $('#span-item').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-item&eqdid=' + $(this).val(), function(){
                toggleItem();
            });
        });

        $('#eqdid, #suoid').change(function(){
            $('#span-funcional').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-funcional&eqdid=' + $('#eqdid').val() + '&suoid=' + $('#suoid').val());
        });

        $('body').on('change', '#maiid', function(){
            $('#span-subitem').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-subitem&maiid=' + $(this).val());
        });

        function toggleItem(){
            if($('#maiid option').size() > 1){
                $('#span-item, #span-subitem').show();
            } else {
                $('#span-item, #span-subitem').hide();
            }
        }
    })
</script>