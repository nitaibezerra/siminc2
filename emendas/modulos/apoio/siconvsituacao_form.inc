<?php

$cSiconvSituacao = new Emendas_Controller_SiconvSituacao();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cSiconvSituacao->salvar($_REQUEST);
		die;
	case 'excluir':
        $cSiconvSituacao->excluir($_REQUEST['sitid']);
		die;
}

$mSiconvSituacao = new Emendas_Model_SiconvSituacao($_REQUEST['sitid']);

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
                        <input name="sitid" id="sitid" type="hidden" value="<?php echo $mSiconvSituacao->sitid; ?>">
                        
                        <?php                         
                        echo $simec->input('sitdsc', 'sitdsc', $mSiconvSituacao->sitdsc, ['maxlength' => 200]);                    
                        echo $simec->input('sitcodigo', 'sitcodigo', $mSiconvSituacao->sitcodigo, ['required', 'maxlength' => 300]);                        
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/siconvsituacao&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mSiconvSituacao->sitid){ ?>
                                    <a href="?modulo=apoio/siconvsituacao_form&acao=A&req=excluir&sitid=<?php echo $mSiconvSituacao->sitid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>