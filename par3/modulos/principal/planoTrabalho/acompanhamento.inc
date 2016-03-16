<?php

$inuid = $_REQUEST['inuid'];

$controleUnidade = new Par3_Controller_InstrumentoUnidade();

switch ($_REQUEST['req']) {
	case 'baixarArquivoDopid':
	    $controllerDocumento = new Par3_Controller_DocumentoLegado();
	    $controllerDocumento->baixarArquivoDopid($_REQUEST);
	    break;
	case 'vizualizaDocumento':
	    $documento = new Par3_Model_DocumentoparLegado();
	    $documento->formVizualizaDocumento($_REQUEST);
        die();
	    break;
}

require APPRAIZ.'includes/cabecalho.inc';
?>
<style>
.esconde{
    cursor:pointer;
}
.esconde:hover{
    background-color: #F5F5DC;
}
.processo_detalhe{
    color:blue;
    cursor:pointer;
}
.modal-dialog{
    margin-left: 0px;
    margin-right: 0px;
}
</style>
<script language="javascript" src="js/documentoLegado.js"></script>
<div class="wrapper wrapper-content animated fadeIn">
	<div class="row">
        <div class="ibox float-e-margins">
            <div class="ibox-content">
                <input type="hidden" name="menu" id="menu" value="<?php echo $_REQUEST['menu']?>"/>
                <?php echo $simec->title($controleUnidade->pegarNomeEntidade($_REQUEST['inuid'])); ?>
                <br>
                <?php $controleUnidade->cabecalhoUnidade(); ?>
                <div class="ibox-content-round-gray ibox-content">
                    <div class="ibox">
                    	<div class="ibox-title esconde" tipo="PAR">
                    	    <h3>Documentos PAR</h3>
                    	</div>
                        <div class="ibox-content" id="PAR">
                            <?php echo Par3_Controller_DocumentoLegado::listaPAR(Array('dopstatus' => 'A', 'inuid' => $inuid)); ?>
                        </div>
                    	<div class="ibox-title esconde" tipo="ObrPAR">
                    	    <h3>Documentos de Obras do PAR</h3>
                    	</div>
                        <div class="ibox-content" id="ObrPAR">
                            <?php echo Par3_Controller_DocumentoLegado::listaObrasPAR(Array('dopstatus' => 'A', 'inuid' => $inuid)); ?>
                        </div>
                    	<div class="ibox-title esconde" tipo="ObrPAC">
                    	    <h3>Documentos de Obras do PAC</h3>
                    	</div>
                        <div class="ibox-content" id="ObrPAC">
                            <?php echo Par3_Controller_DocumentoLegado::listaObras(array('terstatus'=>'A', 'inuid'=>$inuid)); ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>
</div>
<div id="modal-form" class="modal fade" aria-hidden="true" style="width:100%">
    <center>
    	<div class="modal-dialog">
    		<div id="html_modal-form" class="ibox-content"></div>
    	</div>
    </center>
</div>
<script>
$(document).ready(function()
{

	$('.esconde').click(function()
	{
	    var id = $(this).attr('tipo');

	    if ($('#'+id).css('display') == 'none') {
	    	$('#'+id).show();
		} else {
	    	$('#'+id).hide();
		}

	});
});
</script>