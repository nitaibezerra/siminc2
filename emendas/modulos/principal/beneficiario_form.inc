<?php

$cBeneficiario = new Emendas_Controller_Beneficiario();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cBeneficiario->salvar($_REQUEST);
		die;
	case 'excluir':
        $cBeneficiario->excluir($_REQUEST['benid']);
		die;
}

$mBeneficiario = new Emendas_Model_Beneficiario($_REQUEST['benid']);
if(!$mBeneficiario->emeid){ $mBeneficiario->emeid = $_REQUEST['emeid']; }

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <form id="formulario" name="formulario" method="post" class="form-horizontal">
            <div class="col-md-8">
                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Dados Gerais</h5>
                    </div>
                    <div class="ibox-content">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="benid" id="benid" type="hidden" value="<?php echo $mBeneficiario->benid; ?>">
                        <input name="emeid" id="emeid" type="hidden" value="<?php echo $mBeneficiario->emeid; ?>">

                        <?php
                        echo $simec->input('suoid', 'SubUnidade', $mBeneficiario->suoid, ['required']);
                        echo $simec->input('proid', 'Proponente', $mBeneficiario->proid, ['required']);
                        echo $simec->input('bennumeroprocesso', 'Número do Processo', $mBeneficiario->bennumeroprocesso, ['required', 'maxlength' => 50]);
                        echo $simec->data('beninicio', 'Data de Início da Vigência', $mBeneficiario->beninicio, ['required']);
                        echo $simec->input('benformaexecucao', 'Forma de Execução', $mBeneficiario->benformaexecucao, ['required', 'maxlength' => 1]);
                        ?>

                        <div class="col-md-6">
                            <h5 class="text-center" style="color: #55b3a3; border-bottom: 1px #55b3a3 solid; padding-bottom: 5px;">Dados da Programa</h5>
                            <?php
                            echo $simec->input('benpprogramanumero', 'Número', $mBeneficiario->benpprogramanumero, ['class'=>'inteiro']);
                            echo $simec->input('benprogramatitulo', 'Título', $mBeneficiario->benprogramatitulo, ['maxlength' => 1000]);
                            echo $simec->textarea('benprogramaobjeto', 'Objeto', $mBeneficiario->benprogramaobjeto);
                            ?>
                        </div>
                        <div class="col-md-6">
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
            <div class="col-md-4">

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Localização do Projeto <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></h5>
                    </div>
                    <div class="ibox-content">
                        <div class="form-group">
                            <div class="col-md-12">
                                <label for="esfid" class="control-label">Tipo de localização:</label>
                            </div>
                            <div class="col-sm-12">
                                <?php
                                $sql = "SELECT esfid AS codigo, esfdsc AS descricao FROM territorios.esfera ORDER BY descricao";
                                $esfid = $dadosPI['esfid'];
                                $db->monta_combo('esfid', $sql, 'S', 'Selecione', null, null, null, null, 'N', 'esfid', null, '', null, 'class="form-control" style="width=100%;"', null, null);
                                ?>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table id="table_localizacao" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr class="tr_head">
                                        <td style="text-align: left;"><b>UF</b></td>
                                        <td style="text-align: left;"><b>Município</b></td>
                                        <td style="text-align: center;"></td>
                                    </tr>
                                    <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_MUNICIPAL){
                                        foreach($aLocalizacao as $localizacao){ ?>
                                            <tr style="height: 30px;" class="tr_localizacao_<?php echo $localizacao['muncod']; ?>" >
                                                <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                <td style="text-align: left;"><?php echo $localizacao['mundescricao']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" value="<?php echo $localizacao['muncod']; ?>" name="listaLocalizacao[]">
                                                    <span class="glyphicon glyphicon-trash btnRemoverLocalizacao link" data-localizacao="<?php echo $localizacao['muncod']; ?>" />
                                                </td>
                                            </tr>
                                        <?php }
                                    } ?>

                                </table>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table id="table_localizacao_estadual" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr class="tr_head">
                                        <td style="text-align: left;"><b>UF</b></td>
                                        <td style="text-align: left;"><b>Nome</b></td>
                                        <td style="text-align: center;"></td>
                                    </tr>
                                    <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_ESTADUAL){
                                        foreach($aLocalizacao as $localizacao){ ?>
                                            <tr style="height: 30px;" class="tr_localizacao_estadual_<?php echo $localizacao['estuf']; ?>" >
                                                <td style="text-align: left;"><?php echo $localizacao['estuf']; ?></td>
                                                <td style="text-align: left;"><?php echo $localizacao['estdescricao']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" value="<?php echo $localizacao['estuf']; ?>" name="listaLocalizacaoEstadual[]">
                                                    <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoEstadual link" data-localizacao-estadual="<?php echo $localizacao['estuf']; ?>" />
                                                </td>
                                            </tr>
                                        <?php }
                                    } ?>
                                </table>
                            </div>
                        </div>
                        <div class="form-group">
                            <div class="col-lg-12">
                                <table id="table_localizacao_exterior" cellpadding="5" border="0" width="98%" align="center" class="table table-striped table-hover table-bordered table-hover">
                                    <tr class="tr_head">
                                        <td style="text-align: left;"><b>País</b></td>
                                        <td style="text-align: center;"></td>
                                    </tr>
                                    <?php if($dadosPI['esfid'] == Territorios_Model_Esfera::K_EXTERIOR){
                                        foreach($aLocalizacao as $localizacao){ ?>
                                            <tr style="height: 30px;" class="tr_localizacao_exterior_<?php echo $localizacao['paiid']; ?>" >
                                                <td style="text-align: left;"><?php echo $localizacao['paidescricao']; ?></td>
                                                <td style="text-align: center;">
                                                    <input type="hidden" value="<?php echo $localizacao['paiid']; ?>" name="listaLocalizacaoExterior[]">
                                                    <span class="glyphicon glyphicon-trash btnRemoverLocalizacaoExterior link" data-localizacao-exterior="<?php echo $localizacao['paiid']; ?>" />
                                                </td>
                                            </tr>
                                        <?php }
                                    } ?>
                                </table>
                            </div>
                        </div>
                        <div class="form-group text-right">
                            <div class="col-lg-12">
                                <div class="text-right">
                                    <input type="button" id="btn_selecionar_localizacao_estadual" title="Inserir Localização Estadual/Distrito Federal" value="Inserir Localização Estadual/Distrito Federal" class="btn btn-info" />
                                    <input type="button" id="btn_selecionar_localizacao_exterior" title="Inserir Localização no Exterior" value="Inserir Localização no Exterior" class="btn btn-info" />
                                    <input type="button" id="btn_selecionar_localizacao" title="Inserir Localização municipal" value="Inserir Localização municipal" class="btn btn-info" />
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>

            <div class="clearfix"></div>

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
        </form>
        <div class="col-md-12">
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
            		<h5>Dados Financeiros</h5>
            	</div>
            	<div class="ibox-content">
            
            		
            	</div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        // Localizações
        intEsfidEstadualDF = "<?php echo ESFERA_ESTADUAL_DISTRITO_FEDERAL; ?>";
        intEsfidExterior = "<?php echo ESFERA_EXTERIOR; ?>";
        intEsfidFederalBrasil = "<?php echo ESFERA_FEDERAL_BRASIL; ?>";
        intEsfidMunicipal = "<?php echo ESFERA_MUNICIPAL; ?>";

        $('#esfid').change(function(){
            controlarTipoLocalizacao($(this).val());
        });

        controlarTipoLocalizacao($('#esfid').val());

        $('#table_localizacao').on('click', '.btnRemoverLocalizacao', function(){
            var id = $(this).attr('data-localizacao');
            $('.tr_localizacao_'+ id).remove();
        });

        $('#table_localizacao_estadual').on('click', '.btnRemoverLocalizacaoEstadual', function(){
            var id = $(this).attr('data-localizacao-estadual');
            $('.tr_localizacao_estadual_'+ id).remove();
        });

        $('#table_localizacao_exterior').on('click', '.btnRemoverLocalizacaoExterior', function(){
            var id = $(this).attr('data-localizacao-exterior');
            $('.tr_localizacao_exterior_'+ id).remove();
        });

        /**
         * Controla as opções do formulario conforme a opção de tipo de localização selecionada.
         *
         * @param int esfid Código da esfera preenchido pelo usuário
         * @return VOID
         */
        function controlarTipoLocalizacao(esfid){

            alert(esfid)

            $('#btn_selecionar_localizacao').hide();
            $('#btn_selecionar_localizacao_estadual').hide();
            $('#btn_selecionar_localizacao_exterior').hide();
            $('#table_localizacao tr').hide();
            $('#table_localizacao_estadual tr').hide();
            $('#table_localizacao_exterior tr').hide();

            switch (esfid) {
                // Verifica se a esfera é Estadual/DF.
                case intEsfidEstadualDF:
                    $('#table_localizacao_estadual tr').show('slow');
                    $('#btn_selecionar_localizacao_estadual').show('slow');

                    $('#table_localizacao tr').not('tr.tr_head').remove();
                    $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
                    break;
                // Verifica se a esfera é Exterior.
                case intEsfidExterior:
                    $('#table_localizacao_exterior tr').show('slow');
                    $('#btn_selecionar_localizacao_exterior').show('slow');
                    $('#table_localizacao tr').not('tr.tr_head').remove();
                    $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                    break;
                // Verifica se a esfera é Federal.
                case intEsfidFederalBrasil:
                    $('#btn_selecionar_localizacao').hide('slow');
                    $('#table_localizacao tr').hide('slow');
                    $('#table_localizacao tr').not('tr.tr_head').remove();
                    $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                    $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
                    break;
                // Verifica se a esfera é Municipal.
                case intEsfidMunicipal:
                    $('#table_localizacao tr').show('slow');
                    $('#btn_selecionar_localizacao').show('slow');
                    $('#table_localizacao_estadual tr').not('tr.tr_head').remove();
                    $('#table_localizacao_exterior tr').not('tr.tr_head').remove();
                    break;
            }
        }
    })
</script>