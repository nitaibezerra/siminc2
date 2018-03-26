<?php

$cPtres = new Monitora_Controller_Ptres();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cPtres->salvar($_REQUEST);
		die;
	case 'excluir':
        $cPtres->excluir($_REQUEST['ptrid']);
		die;
}

$mPtres = new Monitora_Model_Ptres($_REQUEST['ptrid']);

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
                        <input name="ptrid" id="ptrid" type="hidden" value="<?php echo $mPtres->ptrid; ?>">
                        
                        <?php                         
                        echo $simec->input('ptres', 'PTRES', $mPtres->ptres, ['required','maxlength' => 50]);
                        echo $simec->input('acaid', 'Ação', $mPtres->acaid, ['required']);
                        echo $simec->input('irpcod', 'RP', $mPtres->irpcod, ['maxlength' => 1]);
                        echo $simec->input('ptrdotacao', 'Dotação Total', $mPtres->ptrdotacao);
                        echo $simec->input('plocod', 'Código PO', $mPtres->plocod, ['maxlength' => 4]);
                        echo $simec->input('plodsc', 'Descrição PO', $mPtres->plodsc, ['maxlength' => 500]);
                        echo $simec->input('ptrdotacaocapital', 'Dotação Capital', $mPtres->ptrdotacaocapital);
                        echo $simec->input('ptrdotacaocusteio', 'Dotação Custeio', $mPtres->ptrdotacaocusteio);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=apoio/ptres&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mPtres->ptrid){ ?>
                                    <a href="?modulo=apoio/ptres_form&acao=A&req=excluir&ptrid=<?php echo $mPtres->ptrid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>