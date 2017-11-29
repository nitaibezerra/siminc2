<?php

$cPartido = new Emendas_Controller_Partido();
switch ($_REQUEST['req']) {
    case 'salvar':
        $cPartido->salvar($_REQUEST);
        die;
    case 'excluir':
        $cPartido->salvar(array('parid' => $_REQUEST['parid'], 'parstatus' => 'I'));
        die;
}

$mPartido = new Emendas_Model_Partido($_REQUEST['parid']);

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
                        <input name="parid" id="parid" type="hidden" value="<?php echo $mPartido->parid; ?>">
                        
                        <?php
                            echo $simec->input('parcod', 'Código', $mPartido->parcod);                        
                            echo $simec->input('pardsc', 'Nome', $mPartido->pardsc, ['maxlength' => 200]);                    
                            echo $simec->input('parsigla', 'Sigla', $mPartido->parsigla, ['maxlength' => 30]);                    
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/partido&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mPartido->parid){ ?>
                                    <a href="?modulo=apoio/partido_form&acao=A&req=excluir&parid=<?php echo $mPartido->parid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>