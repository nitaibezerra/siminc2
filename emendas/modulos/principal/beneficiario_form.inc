<?php

include_once '../planacomorc/_funcoespi.php';

$mBeneficiario = new Emendas_Model_Beneficiario($_REQUEST['benid']);

$cBeneficiario = new Emendas_Controller_Beneficiario();

switch ($_REQUEST['req']) {
	case 'salvar':
        $cBeneficiario->salvar($_REQUEST);
		die;
	case 'excluir':
        $cBeneficiario->excluir($_REQUEST['benid']);
		die;
    case 'formulario-historico':
        $cBeneficiario->montarFormularioHistorico($_REQUEST['benid'], $_REQUEST['behid']);
        die;
    case 'listar-historico':
        $cBeneficiario->montarListagemHistorico($mBeneficiario->benid);
        die;
    case 'salvar-historico':
        $cBeneficiarioHistorico = new Emendas_Controller_BeneficiarioHistorico();
        $cBeneficiarioHistorico->salvar($_POST);
        die;
    case 'excluir-historico':
        $cBeneficiarioHistorico = new Emendas_Controller_BeneficiarioHistorico();
        echo $cBeneficiarioHistorico->excluir($_REQUEST['behid']);
        die;
    case 'formulario-detalhe':
        $cBeneficiario->montarFormularioDetalhe($_REQUEST['benid'], $_REQUEST['bedid']);
        die;
    case 'listar-detalhe':
        $cBeneficiario->montarListagemDetalhe($mBeneficiario->benid);
        die;
    case 'salvar-detalhe':
        $cBeneficiarioDetalhe = new Emendas_Controller_BeneficiarioDetalhe();
        $cBeneficiarioDetalhe->salvar($_POST);
        die;
    case 'excluir-detalhe':
        $cBeneficiarioDetalhe = new Emendas_Controller_BeneficiarioDetalhe();
        echo $cBeneficiarioDetalhe->excluir($_REQUEST['bedid']);
        die;
    case 'carregarSegmento':
        echo $simec->select('neeid', 'Segmento Cultural', $mBeneficiario->neeid, (new Monitora_Model_PiNivelEtapaEnsino())->recuperarSqlCombo(null, ["neestatus = 'A'", "mdeid = " . (int)$_REQUEST['mdeid']]));
        die;
    case 'verificarPactuacaoConvenio':
        echo verificarPactuacaoConvenio($_REQUEST['capid']);
        die();
}


if(!$mBeneficiario->emeid){ $mBeneficiario->emeid = $_REQUEST['emeid']; }

$mEmenda = new Emendas_Model_Emenda($mBeneficiario->emeid);

include APPRAIZ . "includes/cabecalho.inc";
?>

<style>
    .select-localizacao{
        display: none;
    }
</style>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <form id="formulario-beneficiario" name="formulario-beneficiario" method="post" class="form-horizontal">
            <div class="col-md-6">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Dados Gerais</h5>
                    </div>
                    <div class="ibox-content">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="benid" id="benid" type="hidden" value="<?php echo $mBeneficiario->benid; ?>">
                        <input name="emeid" id="emeid" type="hidden" value="<?php echo $mBeneficiario->emeid; ?>">

                        <?php
                        $sql = (new Public_Model_SubUnidadeOrcamentaria())->recuperarSqlCombo(['suocod', 'suonome'], ["unoid = {$mEmenda->unoid}"], 'suonome');
                        $options = simec_preparar_array($db->carregar($sql));

                        $mBeneficiario->suoid = (empty($mBeneficiario->suoid) && count($options) == 1) ? key($options) : $mBeneficiario->suoid;

                        echo $simec->select('suoid', 'SubUnidade', $mBeneficiario->suoid, $options, ['required']);
                        echo $simec->select('proid', 'Proponente', $mBeneficiario->proid, (new Emendas_Model_Proponente())->recuperarSqlCombo(["replace(to_char(procnpj::numeric, '00:000:000/0000-00'), ':', '.')", 'pronome'], null,'pronome'), ['required']);
                        echo $simec->input('bennumeroprocesso', 'Número do Processo', $mBeneficiario->bennumeroprocesso, ['maxlength' => 50]);
                        echo $simec->data('beninicio', 'Data de Início da Vigência', $mBeneficiario->beninicio);
                        echo $simec->select('capid', 'Modalidade de Pactuação', $mBeneficiario->capid, (new Monitora_Model_CategoriaApropriacao())->recuperarSqlCombo(null, ["capstatus = 'A'"]));
                        ?>

                        <div class="col-md-6 dados-siconv">
                            <h5 class="text-center" style="color: #55b3a3; border-bottom: 1px #55b3a3 solid; padding-bottom: 5px;">Dados da Programa</h5>
                            <?php
                            echo $simec->input('benpprogramanumero', 'Número', $mBeneficiario->benpprogramanumero, ['class'=>'inteiro']);
                            echo $simec->input('benprogramatitulo', 'Título', $mBeneficiario->benprogramatitulo, ['maxlength' => 1000]);
                            echo $simec->textarea('benprogramaobjeto', 'Objeto', $mBeneficiario->benprogramaobjeto);
                            ?>
                        </div>
                        <div class="col-md-6 dados-siconv">
                            <h5 class="text-center" style="color: #55b3a3; border-bottom: 1px #55b3a3 solid; padding-bottom: 5px;">Dados da Proposta</h5>
                            <?php
                            echo $simec->input('benpropostanumero', 'Número', $mBeneficiario->benpropostanumero, ['class'=>'inteiro']);
                            echo $simec->input('benpropostatitulo', 'Título', $mBeneficiario->benpropostatitulo, ['maxlength' => 1000]);
                            echo $simec->textarea('benpropostaobjeto', 'Objeto', $mBeneficiario->benpropostaobjeto);
                            ?>
                        </div>
                        <div class="clearfix"></div>
                    </div>
                </div>
            </div>
            <div class="col-md-5">

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Informações Complementares</h5>
                    </div>
                    <div class="ibox-content">
                        <?php
                        $aLocalizacoes = (new Emendas_Model_BeneficiarioLocalizacao())->recuperarPorBeneficiario($mBeneficiario->benid);
                        echo $simec->select('esfid', 'Localização', $mBeneficiario->esfid, (new Territorios_Model_Esfera())->recuperarSqlCombo());
                        ?>
                        <div class="select-localizacao"  id="div-localizacao_<?php echo Territorios_Model_Esfera::K_EXTERIOR ?>">
                            <?php echo $simec->select('paiid[]', 'País', $aLocalizacoes['paiid'], (new Territorios_Model_Pais())->recuperarSqlCombo()); ?>
                        </div>
                        <div class="select-localizacao" id="div-localizacao_<?php echo Territorios_Model_Esfera::K_ESTADUAL?>">
                            <?php echo $simec->select('estuf[]', 'UF', $aLocalizacoes['estuf'], (new Territorios_Model_Estado())->recuperarSqlCombo(['estuf', 'estdescricao'])); ?>
                        </div>
                        <div class="select-localizacao"  id="div-localizacao_<?php echo Territorios_Model_Esfera::K_MUNICIPAL ?>">
                            <?php echo $simec->select('muncod[]', 'Município', $aLocalizacoes['muncod'], (new Territorios_Model_Municipio())->recuperarSqlCombo(['estuf', 'mundescricao'])); ?>
                        </div>
                        
                        <?php
                        echo $simec->select('mdeid', 'Área Cultural', $mBeneficiario->mdeid, (new Monitora_Model_PiModalidadeEnsino())->recuperarSqlCombo(null, ["mdestatus = 'A'"]));
                        echo '<span id="span-segmento">' . $simec->select('neeid', 'Segmento Cultural', $mBeneficiario->neeid, (new Monitora_Model_PiNivelEtapaEnsino())->recuperarSqlCombo(null, ["neestatus = 'A'", "mdeid = " . (int)$mBeneficiario->mdeid])) . '</span>';
                        ?>
                        <div class="form-group ">
                            <label for="benempenhado" class="col-sm-3 control-label">Empenhado: </label>
                            <div class="col-md-2 ">
                                <input type="checkbox" name="benempenhado" id="benempenhado" value="t" class="js-switch" <?php echo 't' == $mBeneficiario->benempenhado ? 'checked="checked"' : ''; ?> />
                            </div>
                            <label for="benconveniado" class="col-sm-2 control-label">Conveniado: </label>
                            <div class="col-md-1 ">
                                <input type="checkbox" name="benconveniado" id="benconveniado" value="t" class="js-switch" <?php echo 't' == $mBeneficiario->benconveniado ? 'checked="checked"' : ''; ?> />
                            </div>
                            <label for="benpago" class="col-sm-2 control-label">Pago: </label>
                            <div class="col-md-1 ">
                                <input type="checkbox" name="benpago" id="benpago" value="t" class="js-switch" <?php echo 't' == $mBeneficiario->benpago ? 'checked="checked"' : ''; ?> />
                            </div>
                        </div>

                        <div class="form-group ">
                            <label for="benimpedimento" class="col-sm-3 control-label">Impedimento: </label>
                            <div class="col-md-1 ">
                                <input type="checkbox" name="benimpedimento" id="benimpedimento" value="t" class="js-switch" <?php echo $mBeneficiario->impid ? 'checked="checked"' : ''; ?> />
                            </div>
                            <label for="benimpedimento" class="col-sm-2 control-label div_motivo">Motivo: </label>
                            <div class="col-md-6 div_motivo">
                                <?php echo $simec->select('impid', null, $mBeneficiario->impid, (new Emendas_Model_Impedimento())->recuperarSqlCombo(['impcod', 'impdsc'], ["impstatus = 'A'"], 'impcod'));; ?>
                            </div>
                        </div>

                        <div class="div_motivo">
                            <?php echo $simec->textarea('benmotivoimpedimento', 'Justivicativa', $mBeneficiario->benmotivoimpedimento); ?>
                        </div>
                    </div>
                </div>
            </div>
        </form>

        <div class="col-md-12">
            <div class="form-group">
                <div class="text-center">
                    <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                    <a href="?modulo=principal/emenda_form&acao=A&emeid=<?php echo $mBeneficiario->emeid; ?>" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                    <?php if($mBeneficiario->benid){ ?>
                        <a href="?modulo=principal/beneficiario_form&acao=A&req=excluir&benid=<?php echo $mBeneficiario->benid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                    <?php } ?>
                </div>
            </div>
        </div>

        <div class="col-md-4">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Histórico/Observações</h5>
                </div>
                <div class="ibox-content">
                    <div id="div_formulario_historico">
                        <?php if($mBeneficiario->benid){
                            $cBeneficiario->montarFormularioHistorico($mBeneficiario->benid);
                        } ?>
                    </div>
                    <div id="div_listagem_historico">
                        <?php $cBeneficiario->montarListagemHistorico($mBeneficiario->benid); ?>
                    </div>
                </div>
            </div>
        </div>

        <div class="col-md-8">
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
            		<h5>Dados Financeiros</h5>
            	</div>
                <div class="ibox-content">
                    <div id="div_formulario_detalhe">
                        <?php if($mBeneficiario->benid){
                            $cBeneficiario->montarFormularioDetalhe($mBeneficiario->benid);
                        } ?>
                    </div>
                    <div id="div_listagem_detalhe">
                        <?php $cBeneficiario->montarListagemDetalhe($mBeneficiario->benid); ?>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){

        toggleMotivo();
        $('#benimpedimento').change(function(){
            toggleMotivo();
        });

        $('#btn-salvar').click(function(){
            $('#formulario-beneficiario').submit();
        });

        $('#esfid').change(function(){
            $('.select-localizacao').hide('slow');
            $('#div-localizacao_' + $('#esfid').val()).show('slow');
        }).change();

        $('#bennumeroprocesso').mask('99999.999999/9999-99');

        $('#mdeid').change(function(){
            $('#span-segmento').load('?modulo=principal/beneficiario_form&acao=A&req=carregarSegmento&mdeid=' + $(this).val());
        });

        $('#capid').change(function(){

            $.ajax({
                url: 'emendas.php?modulo=principal/beneficiario_form&acao=A&req=verificarPactuacaoConvenio&capid='+$('#capid').val(),
                success: function($retorno){
                    if($retorno){
                        $('.dados-siconv').show('slow');
                    } else {
                        $('.dados-siconv').hide('slow');
                    }
                }
            });

        }).change();

        function toggleMotivo(){
            if($('#benimpedimento').is(':checked')){
                $('.div_motivo').show('slow');
            } else {
                $('#impid').val('').trigger("chosen:updated");
                $('.div_motivo').hide('slow');
            }
        }
    })
</script>