<?php
$mEmenda = new Emendas_Model_Emenda($_REQUEST['emeid']);

$cEmenda = new Emendas_Controller_Emenda();
switch ($_REQUEST['req']) {
	case 'salvar':
        $cEmenda->salvar($_REQUEST);
		die;
	case 'excluir':
        $cEmenda->excluir($_REQUEST['emeid']);
		die;
	case 'carregarAcaoPorUo':
        echo $simec->select('acaid', 'Funcional', null, (new Monitora_Model_Acao())->recuperarSqlCombo(['prgcod', 'acacod', 'acaobjetivocod', 'loccod', 'acatitulo'], ["unoid = '{$_REQUEST['unoid']}'"]));
		die;
    case 'formulario-detalhe':
        $cEmenda->montarFormularioDetalhes($_REQUEST['emeid'], $_REQUEST['emdid']);
        die;
    case 'listar-detalhe':
        $cEmenda->montarListagemDetalhes($mEmenda->emeid);
        die;
    case 'salvar-detalhe':
        $cEmendaDetalhe = new Emendas_Controller_EmendaDetalhe();
        $cEmendaDetalhe->salvar($_POST);
        die;
    case 'excluir-detalhe':
        $cEmendaDetalhe = new Emendas_Controller_EmendaDetalhe();
        echo $cEmendaDetalhe->excluir($_REQUEST['emdid']);
        die;
}

include APPRAIZ . "includes/cabecalho.inc";
?>

<div class="row wrapper border-bottom white-bg page-heading">
    <div class="col-lg-10">
        <h2><?php echo $titulo_modulo; ?></h2>
    </div>
</div>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="row">
        <div class="col-md-8">
            <div class="ibox float-e-margins">
                <div class="ibox-title">
                    <h5>Dados Gerais</h5>
                </div>
                <div class="ibox-content">
                    <form id="formulario" name="formulario" method="post" class="form-horizontal">
                        <input type="hidden" name="req" id="req" value="salvar" />
                        <input name="emeid" id="emeid" type="hidden" value="<?php echo $mEmenda->emeid; ?>">
                        
                        <?php                         
                        echo $simec->input('emenumero', 'Número', $mEmenda->emenumero, ['class'=>'inteiro']);
                        echo $simec->select('autid', 'Autor', $mEmenda->autid, (new Emendas_Model_Autor())->recuperarSqlCombo(['autcod', 'autnome'], null, 'autnome'));
                        echo $simec->select('unoid', 'Unidade', $mEmenda->unoid, (new Public_Model_UnidadeOrcamentaria())->recuperarSqlCombo(['unocod', 'unonome'], null, 'unonome'));
                        echo '<span id="span-funcional">' . $simec->select('acaid', 'Funcional', $mEmenda->acaid, (new Monitora_Model_Acao())->recuperarSqlCombo(['prgcod', 'acacod', 'acaobjetivocod', 'loccod', 'acatitulo'])) . '</span>';
                        echo $simec->radio('emeimpositiva', 'Impositiva', $mEmenda->emeimpositiva, ['t'=>'Sim', 'f'=>'Não'], ['required']);
                        echo $simec->textarea('emejustificativa', 'Justificativa', $mEmenda->emejustificativa, ['maxlength' => 2000]);
                        ?>
                        
                        <div class="form-group">
                            <div class="text-center">
                                <button class="btn btn-primary" type="submit" id="btn-salvar"><i class="fa fa-check"></i>&nbsp;Salvar</button>
                                <a href="?modulo=principal/emenda&acao=A" class="btn btn-warning" id="btn-voltar" type="button"><i class="fa fa-arrow-left"></i>&nbsp;Voltar</a>
                                <?php if($mEmenda->emeid){ ?>
                                    <a href="?modulo=principal/emenda_form&acao=A&req=excluir&emeid=<?php echo $mEmenda->emeid; ?>" class="btn btn-danger link-excluir" id="btn-excluir" type="button"><i class="fa fa-close"></i>&nbsp;Excluir</a>
                                <?php } ?>                                
                            </div>
                        </div>                        
                    </form>
                </div>
            </div>
        </div>
        <div class="col-md-4">
            <div class="ibox float-e-margins">
            	<div class="ibox-title">
            		<h5>Dados Financeiros</h5>
            	</div>
            	<div class="ibox-content" id="">
                    <div id="div_formulario_detalhe">
                        <?php $cEmenda->montarFormularioDetalhes($mEmenda->emeid); ?>
                    </div>
                    <div id="div_listagem_detalhe">
                        <?php $cEmenda->montarListagemDetalhes($mEmenda->emeid); ?>
                    </div>
            	</div>
            </div>
        </div>
    </div>
</div>

<script>
    $(function(){
        $('#unoid').change(function(){
            $('#span-funcional').load('?modulo=principal/emenda_form&acao=A&req=carregarAcaoPorUo&unoid=' + $(this).val());
        });
    })
</script>