<?php

$cAcompanhamentoMedida = new Acompanhamento_Controller_AcompanhamentoMedida();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cAcompanhamentoMedida->salvar($_REQUEST);
		die;
	case 'excluir':
        $cAcompanhamentoMedida->excluir($_REQUEST['acmid']);
		die;
}

$mAcompanhamentoMedida = new Acompanhamento_Model_AcompanhamentoMedida($_REQUEST['acmid']);

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-12">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="acmid" id="acmid" type="hidden" value="<?php echo $mAcompanhamentoMedida->acmid; ?>">
                        
                        <?php                         
                        echo $simec->input('acoid', 'acoid', $mAcompanhamentoMedida->acoid);                    
                        echo $simec->input('medid', 'medid', $mAcompanhamentoMedida->medid);                        
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/acompanhamentomedida&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mAcompanhamentoMedida->acmid){ ?>
                                    <a href="?modulo=apoio/acompanhamentomedida_form&acao=A&req=excluir&acmid=<?php echo $mAcompanhamentoMedida->acmid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>