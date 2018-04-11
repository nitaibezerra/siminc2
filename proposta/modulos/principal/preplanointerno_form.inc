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
        echo $simec->select('masid', 'SubItem', null, (new Planacomorc_Model_ManutencaoSubItem())->recuperarSqlCombo(null, ["maiid = " . (int) $_REQUEST['maiid'] ]));
		die;
	case 'carregar-funcional':
        echo $simec->select('ptrid', 'Funcional', null, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($_REQUEST['suoid'], $_REQUEST['eqdid']), ['required']);
		die;
	case 'carregar-segmento':
        echo $simec->select('neeid', 'Segmento Cultural', null, (new Monitora_Model_PiNivelEtapaEnsino())->recuperarSqlCombo(null, ['mdeid = ' . (int)$_REQUEST['mdeid']]), ['required']);
		die;
	case 'carregar-metapnc':
        echo $simec->select('mpnid', 'Meta PNC', null, (new Public_Model_MetaPnc())->recuperarSqlCombo(null, ['suoid'=>$_REQUEST['suoid']]), ['required']);
		die;
	case 'carregar-indicadorpnc':
        echo $simec->select('ipnid', 'Indicador PNC', null, (new Public_Model_IndicadorPnc())->recuperarSqlCombo(null, ['mpnid'=>(int)$_REQUEST['mpnid']]), ['required']);
		die;
	case 'recuperar-objetivoppa':
	    echo (new Monitora_Model_Ptres($_REQUEST['ptrid']))->recuperarObjetivoPorPtres();
		die;
	case 'carregar-metappa':
        echo $simec->select('mppid', 'Metas PPA', null, (new Public_Model_MetaPpa())->recuperarSqlCombo(null, ['suoid'=>(int)$_REQUEST['suoid'], 'oppid'=>(int)$_REQUEST['oppid']]), ['required']);
		die;
	case 'carregar-iniciativappa':
        echo $simec->select('ippid', 'Iniciativa PPA', null, (new Public_Model_IniciativaPpa())->recuperarSqlCombo(null, ['oppid'=>(int)$_REQUEST['oppid']]), ['required']);
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
                        echo '<span id="span-metappa">' . $simec->select('mppid', 'Metas PPA', $mPrePlanoInterno->mppid, (new Public_Model_MetaPpa())->recuperarSqlCombo(), ['required']) . '</span>';
                        echo '<span id="span-iniciativappa">' . $simec->select('ippid', 'Iniciativa PPA', $mPrePlanoInterno->ippid, (new Public_Model_IniciativaPpa())->recuperarSqlCombo(), ['required']) . '</span>';
                        echo '<span id="span-metapnc">' . $simec->select('mpnid', 'Meta PNC', $mPrePlanoInterno->mpnid, (new Public_Model_MetaPnc())->recuperarSqlCombo(null, ['suoid'=>$mPrePlanoInterno->suoid]), ['required']) . '</span>';
                        echo '<span id="span-indicadorpnc">' . $simec->select('ipnid', 'Indicador PNC', $mPrePlanoInterno->ipnid, (new Public_Model_IndicadorPnc())->recuperarSqlCombo(), ['required']) . '</span>';
                        ?>
                    </div>
                </div>

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Produto PI</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        echo $simec->select('pprid', 'Produto', $mPrePlanoInterno->pprid, (new Monitora_Model_PiProduto())->recuperarSqlCombo(), ['required']);
                        echo $simec->select('pumid', 'Unidade de Medida', $mPrePlanoInterno->pumid, (new Monitora_Model_PiUnidadeMedida())->recuperarSqlCombo(), ['required']);
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

                        <table cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                            <tr>
                                <td style="text-align: left;"></td>
                                <td colspan="3" style="text-align: center;">
                                    <b>Total (Custeio e Capital)</b>
                                </td>
                            </tr>
                            <tr style="height: 30px;" class="tr_limite_autorizado_unidade">
                                <td style="text-align: left;">Limite Autorizado na Unidade: </td>
                                <td colspan="3" id="td_autorizado_sub_unidade" style="text-align: center;" >0,00</td>
                            </tr>
                            <tr style="height: 30px;" class="tr_limite_disponivel_unidade">
                                <td style="text-align: left;">
                                    Limite Disponível na Unidade:
                                    <input id="VlrSUDisponivel" type="hidden" value="" />
                                </td>
                                <td colspan="3" id="td_disponivel_sub_unidade" style="text-align: center;" >0,00</td>
                            </tr>
                            <tr>
                                <td style="text-align: left;"></td>
                                <td style="text-align: center;"><b>Custeio </b></td>
                                <td style="text-align: center;"><b>Capital</b></td>
                            </tr>
                            <tr>
                                <td style="text-align: left;">
                                    Total Cadastrado na Funcional:
                                    <input id="VlrFuncionalDisponivelCusteio" type="hidden" value="" />
                                    <input id="VlrFuncionalDisponivelCapital" type="hidden" value="" />
                                </td>
                                <td style="text-align: left;" id="td_disponivel_funcional_custeio" >0,00</td>
                                <td style="text-align: center;" id="td_disponivel_funcional_capital" >0,00</td>
                            </tr>
                            <tr>
                                <td id="" style="text-align: left;">Valor do Projeto: <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></td>
                                <td style="text-align: left;">
                                    <?php echo $simec->valor('plivalorcusteio', '', $mPrePlanoInterno->plivalorcusteio, ['required']); ?>
                                </td>
                                <td style="text-align: center;">
                                    <?php echo $simec->valor('plivalorcapital', '', $mPrePlanoInterno->plivalorcapital, ['required']); ?>
                                </td>
                            </tr>
                        </table>
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

        $('#suoid').change(function(){
            $('#span-metapnc').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-metapnc&suoid=' + $('#suoid').val());
        });

        $('#mdeid').change(function(){
            $('#span-segmento').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-segmento&mdeid=' + $(this).val());
        });

        $('body').on('change', '#maiid', function(){
            $('#span-subitem').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-subitem&maiid=' + $(this).val());
        });

        $('body').on('change', '#mpnid', function(){
            $('#span-indicadorpnc').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-indicadorpnc&mpnid=' + $(this).val());
        });

        $('#oppid').change(function(){
            $('#span-metappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-metappa&oppid=' + $('#oppid').val() + '&suoid=' + $('#suoid').val());
            $('#span-iniciativappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-iniciativappa&oppid=' + $('#oppid').val());
        });

        $('body').on('change', '#ptrid', function(){
            $.ajax({
                url: '?modulo=principal/preplanointerno_form&acao=A&req=recuperar-objetivoppa&ptrid=' + $(this).val(),
                success: function(oppid){
                    if(oppid){
                        $('#oppid').val(oppid).prop('disabled', 'disabled').trigger("chosen:updated");
                    } else {
                        $('#oppid').val('').prop('disabled', false).trigger("chosen:updated");
                    }
                }
            });
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