<?php

$cPropostaDetalhe = new Proposta_Controller_PropostaDetalhe();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cPropostaDetalhe->salvar($_REQUEST);
		die;
	case 'excluir':
        $cPropostaDetalhe->excluir($_REQUEST['prdid']);
		die;
}

$mPropostaDetalhe = new Proposta_Model_PropostaDetalhe($_REQUEST['prdid']);

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
                        <input name="prdid" id="prdid" type="hidden" value="<?php echo $mPropostaDetalhe->prdid; ?>">
                        
                        <?php                         
                        echo $simec->input('ndpid', 'ndpid', $mPropostaDetalhe->ndpid);                    
                        echo $simec->input('iducod', 'iducod', $mPropostaDetalhe->iducod, ['maxlength' => 1]);                    
                        echo $simec->input('foncod', 'foncod', $mPropostaDetalhe->foncod, ['maxlength' => 3]);                    
                        echo $simec->input('idoid', 'idoid', $mPropostaDetalhe->idoid);                    
                        echo $simec->input('prdvalor', 'prdvalor', $mPropostaDetalhe->prdvalor);                    
                        echo $simec->input('prdvalorexpansao', 'prdvalorexpansao', $mPropostaDetalhe->prdvalorexpansao, ['required']);                    
                        echo $simec->input('proid', 'proid', $mPropostaDetalhe->proid);                        
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=proposta/propostadetalhe&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mPropostaDetalhe->prdid){ ?>
                                    <a href="?modulo=proposta/propostadetalhe_form&acao=A&req=excluir&prdid=<?php echo $mPropostaDetalhe->prdid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
    </div>
</div>