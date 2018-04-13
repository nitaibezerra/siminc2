<?php

$cProposta = new Proposta_Controller_Proposta();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cProposta->salvar($_REQUEST);
		die;
	case 'excluir':
        $cProposta->excluir($_REQUEST['proid']);
		die;
    case 'carregar-funcional':
        echo $simec->select('ptrid', 'Funcional', null, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($_REQUEST['suoid'], $_REQUEST['eqdid']), ['required']);
        die;
}

$mProposta = new Proposta_Model_Proposta($_REQUEST['proid']);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-6">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="proid" id="proid" type="hidden" value="<?php echo $mProposta->proid; ?>">
                        
                        <?php
                        echo $simec->select('suoid', 'SubUnidade', $mProposta->suoid, (new Public_Model_SubUnidadeOrcamentaria())->recuperarSqlCombo(), ['required']);
                        echo $simec->select('eqdid', 'Enquadramento da Despesa', $mProposta->eqdid, (new Monitora_Model_PiEnquadramentoDespesa())->recuperarSqlCombo(), ['required']);
                        echo '<span id="span-funcional">' . $simec->select('ptrid', 'Funcional', $mProposta->ptrid, (new Monitora_Model_Ptres())->recuperarSqlPorSubunidadeEEnquadramento($mProposta->suoid, $mProposta->eqdid), ['required']) . '</span>';
                        echo $simec->input('proquantidade', 'Quantidade', $mProposta->proquantidade, ['required']);
                        echo $simec->input('proquantidadeexpansao', 'Quantidade Expansão', $mProposta->proquantidadeexpansao);
                        echo $simec->textarea('projustificativa', 'Justificativa', $mProposta->projustificativa, ['cols'=>60, 'rows'=>6]);
                        echo $simec->textarea('projustificativaexpansao', 'Justificativa Expansão', $mProposta->projustificativaexpansao, ['cols'=>60, 'rows'=>6]);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=principal/proposta&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mProposta->proid){ ?>
                                    <a href="?modulo=principal/proposta_form&acao=A&req=excluir&proid=<?php echo $mProposta->proid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $('#eqdid, #suoid').change(function(){
            $('#span-funcional').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-funcional&eqdid=' + $('#eqdid').val() + '&suoid=' + $('#suoid').val());
        });
    })
</script>