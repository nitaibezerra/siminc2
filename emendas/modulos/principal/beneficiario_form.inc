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
                        $sql = (new Public_Model_SubUnidadeOrcamentaria())->recuperarSqlCombo(['suocod', 'suonome'], ["unoid = {$mEmenda->unoid}"], 'suonome');
                        $options = simec_preparar_array($db->carregar($sql));

                        $mBeneficiario->suoid = (empty($mBeneficiario->suoid) && count($options) == 1) ? key($options) : $mBeneficiario->suoid;

                        echo $simec->select('suoid', 'SubUnidade', $mBeneficiario->suoid, $options, ['required']);
                        echo $simec->select('proid', 'Proponente', $mBeneficiario->proid, (new Emendas_Model_Proponente())->recuperarSqlCombo(['procnpj', 'pronome'], null,'pronome'), ['required']);
                        echo $simec->input('bennumeroprocesso', 'Número do Processo', $mBeneficiario->bennumeroprocesso, ['maxlength' => 50]);
                        echo $simec->data('beninicio', 'Data de Início da Vigência', $mBeneficiario->beninicio);
                        echo $simec->radio('benformaexecucao', 'Forma de Execução', $mBeneficiario->benformaexecucao, ['S'=>'SICONV', 'D'=>'Direta', 'T'=>'TED'], ['required']);
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
            <div class="col-md-4">

                <div class="ibox float-e-margins">
                    <div class="ibox-title">
                        <h5>Localização do Projeto <span title="Campo obrigatório" class="link campo-obrigatorio">*</span></h5>
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
        verificarDadosSiconv();
        $('[id^="benformaexecucao"]').change(function(){
            verificarDadosSiconv();
        });

        $('#esfid').change(function(){
            $('.select-localizacao').hide('slow');
            $('#div-localizacao_' + $('#esfid').val()).show('slow');
        }).change();

        $('#bennumeroprocesso').mask('99999.999999/9999-99');

        function verificarDadosSiconv(){
            if($('[id^="benformaexecucao"]:checked').val() == 'S'){
                $('.dados-siconv').show('slow');
            } else {
                $('.dados-siconv').hide('slow');
            }
        }
    })
</script>