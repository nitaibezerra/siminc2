<?php

$cPiJanela = new Planacomorc_Controller_PiJanela();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cPiJanela->salvar($_REQUEST);
		die;
	case 'excluir':
        $cPiJanela->excluir($_REQUEST['pijid']);
		die;
}

$mPiJanela = new Planacomorc_Model_PiJanela($_REQUEST['pijid']);

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
                        <input name="pijid" id="pijid" type="hidden" value="<?php echo $mPiJanela->pijid; ?>">
                        
                        <?php                         
                        echo $simec->input('pijdsc', 'pijdsc', $mPiJanela->pijdsc, ['maxlength' => 500]);                    
                        echo $simec->input('pijinicio', 'pijinicio', $mPiJanela->pijinicio);                    
                        echo $simec->data('pijfim', 'pijfim', $mPiJanela->pijfim);
                        echo $simec->data('pijcadastro', 'pijcadastro', $mPiJanela->pijcadastro);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/pi_janela&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mPiJanela->pijid){ ?>
                                    <a href="?modulo=apoio/pi_janela_form&acao=A&req=excluir&pijid=<?php echo $mPiJanela->pijid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>