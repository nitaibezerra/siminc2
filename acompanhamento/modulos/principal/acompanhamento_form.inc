<?php

$cAcompanhamento = new Acompanhamento_Controller_Acompanhamento();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cAcompanhamento->salvar($_REQUEST);
		die;
	case 'excluir':
        $cAcompanhamento->excluir($_REQUEST['acoid']);
		die;
}

$mAcompanhamento = new Acompanhamento_Model_Acompanhamento($_REQUEST['acoid']);

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
                        <input name="acoid" id="acoid" type="hidden" value="<?php echo $mAcompanhamento->acoid; ?>">
                        
                        <?php                         
                        echo $simec->input('acoquantidade', 'acoquantidade', $mAcompanhamento->acoquantidade, ['required']);                    
                        echo $simec->input('acodata', 'acodata', $mAcompanhamento->acodata, ['required']);                    
                        echo $simec->input('acoanalise', 'acoanalise', $mAcompanhamento->acoanalise, ['required']);                    
                        echo $simec->input('claid', 'claid', $mAcompanhamento->claid);                    
                        echo $simec->input('acoprovidencias', 'acoprovidencias', $mAcompanhamento->acoprovidencias, ['required']);                    
                        echo $simec->input('mppid', 'mppid', $mAcompanhamento->mppid, ['required']);                    
                        echo $simec->input('ipnid', 'ipnid', $mAcompanhamento->ipnid, ['required']);                    
                        echo $simec->input('pliid', 'pliid', $mAcompanhamento->pliid, ['required']);                    
                        echo $simec->input('janid', 'janid', $mAcompanhamento->janid);                    
                        echo $simec->input('prsano', 'prsano', $mAcompanhamento->prsano, ['maxlength' => 4]);                        
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/acompanhamento&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mAcompanhamento->acoid){ ?>
                                    <a href="?modulo=apoio/acompanhamento_form&acao=A&req=excluir&acoid=<?php echo $mAcompanhamento->acoid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>