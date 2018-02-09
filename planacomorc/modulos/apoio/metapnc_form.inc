<?php

$cMetapnc = new Public_Controller_Metapnc();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cMetapnc->salvar($_REQUEST);
		die;
	case 'excluir':
        $cMetapnc->excluir($_REQUEST['mpnid']);
		die;
}

$mMetapnc = new Public_Model_MetaPnc($_REQUEST['mpnid']);

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
                        <input name="mpnid" id="mpnid" type="hidden" value="<?php echo $mMetapnc->mpnid; ?>">
                        
                        <?php                         
                        echo $simec->input('mpndsc', 'mpndsc', $mMetapnc->mpndsc, ['required', 'maxlength' => 1000]);                    
                        echo $simec->input('mpncod', 'mpncod', $mMetapnc->mpncod, ['maxlength' => 4]);                    
                        echo $simec->input('mpnnome', 'mpnnome', $mMetapnc->mpnnome, ['maxlength' => 1000]);                    
                        echo $simec->input('prsano', 'prsano', $mMetapnc->prsano, ['required', 'maxlength' => 4]);                        
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/metapnc&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mMetapnc->mpnid){ ?>
                                    <a href="?modulo=apoio/metapnc_form&acao=A&req=excluir&mpnid=<?php echo $mMetapnc->mpnid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>