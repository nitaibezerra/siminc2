<?php

$cImpedimento = new Emendas_Controller_Impedimento();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cImpedimento->salvar($_REQUEST);
    die;
    case 'excluir':
        $cImpedimento->excluir($_REQUEST['impid']);
    die;
}

$mImpedimento = new Emendas_Model_Impedimento($_REQUEST['impid']);

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
                        <input name="impid" id="impid" type="hidden" value="<?php echo $mImpedimento->impid; ?>">
                        
                        <?php
                            echo $simec->input('impcod', 'Código', $mImpedimento->impcod, ['class'=>'inteiro']);
                            echo $simec->input('impdsc', 'Nome', $mImpedimento->impdsc, ['maxlength' => 100]);                        
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/impedimento&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mImpedimento->impid){ ?>
                                    <a href="?modulo=apoio/impedimento_form&acao=A&req=excluir&impid=<?php echo $mImpedimento->impid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>