<?php

$cAreaCultural = new Public_Controller_AreaCultural();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cAreaCultural->salvar($_REQUEST);
		die;
	case 'excluir':
        $cAreaCultural->excluir($_REQUEST['mdeid']);
		die;
}

$mAreaCultural = new Monitora_Model_PiModalidadeEnsino($_REQUEST['mdeid']);

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
                        <input name="mdeid" id="mdeid" type="hidden" value="<?php echo $mAreaCultural->mdeid; ?>">
                        
                        <?php
                        echo $simec->input('mdecod', 'Código PI', $mAreaCultural->mdecod, ['required','maxlength' => 1]);
                        echo $simec->input('mdedsc', 'Descrição', $mAreaCultural->mdedsc, ['required', 'maxlength' => 250]);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/area_cultural&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mAreaCultural->mdeid){ ?>
                                    <a href="?modulo=apoio/area_cultural_form&acao=A&req=excluir&mdeid=<?php echo $mAreaCultural->mdeid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>