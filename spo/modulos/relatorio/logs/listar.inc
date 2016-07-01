<?php
if ($_REQUEST['download']) {
	$file = md5(time()) . '.xml';
	header('Content-type: text/xml');
	header('Content-Disposition: attachment; filename="' . $file . '"');
	echo stripslashes($_REQUEST['xml']);
	die;
}

$fm = new Simec_Helper_FlashMessage('spo/relatorio/logs/listar');
require_once APPRAIZ . 'includes/cabecalho.inc';
?>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script>
$(document).ready(function() {
	 $('#servico1,#servico2').on("change", function() {
    	$('#nomeMetodo1,#nomeMetodo2').val('').trigger('chosen:updated');
        $(this).val() == 1 ?
        ($('#servico-1').show(), $('#servico-2').hide()) :
        ($('#servico-2').show(), $('#servico-1').hide());
    });
	$('#codeModal').on('show.bs.modal', function (event) {
	  var button = $(event.relatedTarget);
	  var code = button.data('code');
	  var xml = button.data('xml');
	  var modal = $(this);
	  modal.find('.modal-body').find('.code').text(formatXml(code)).prettify();
	  modal.find('.modal-body').find('.xml').val(formatXml(xml));
	});
	$('#servico<?php echo (isset($_REQUEST["servico"])?$_REQUEST['servico']:"1")?>').change();
});
</script>
<div class="col-lg-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('SIOP (SOF)')
        ->add('Log de requisições')
        ->render();
    echo $fm->getMensagens();
    ?>
    <div class="well">
        <fieldset>
            <form id="frmConsulta" name="frmConsulta" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
            	<div class="form-group">
                    <label for="servico" class="col-lg-2 control-label">
                        Serviço:
                    </label>
                    <div class="col-lg-2">
                    <?php
			            $opcoes = array('Qualitativo' => '1', 'Quantitativo' => '2');
			            inputChoices('servico', $opcoes, isset($_REQUEST['servico']) ? $_REQUEST['servico'] : '1', 'servico');
		            ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="nomeMetodo" class="col-lg-2 control-label">
                        Nome do método:
                    </label>
                    <div class="col-lg-6">
	                    <span id="servico-1">
	                    <?php
	                    	$servicos = array();
	                    	$qualitativo = new ReflectionClass('Spo_Ws_Sof_Qualitativo');
	                    	$qualitativoServices = $qualitativo->getMethods(ReflectionMethod::IS_PUBLIC);

	                    	foreach ($qualitativoServices as $service)
	                    	{
	                    		$metodo = new ReflectionMethod($service->class, $service->name);
	                    		$docs = $metodo->getDocComment();
	                    		$logger = getDocComment($docs, 'logger');
	                    		$name = getDocComment($docs, 'name');

	                    		if ($logger)
	                    			$servicos[] = array('codigo' => $service->name, 'descricao' => $name . ' (' . $service->name . ')');
	                    	}

	                    	sort($servicos);

	                        $options = array(
	                            'titulo' => 'Selecione ao menos um serviço'
	                        );
	                        inputCombo('nomeMetodo1', $servicos, $_REQUEST['nomemetodo1'], 'cols-qualit', $options);
	                    ?>
	                    </span>
	                    <span id="servico-2">
	                    <?php
	                    	$servicos = array();
	                    	$quantitativo = new ReflectionClass('Spo_Ws_Sof_Quantitativo');
	                    	$quantitativoServices = $quantitativo->getMethods(ReflectionMethod::IS_PUBLIC);

	                    	foreach ($quantitativoServices as $service)
	                    	{
	                    		$metodo = new ReflectionMethod($service->class, $service->name);
	                    		$docs = $metodo->getDocComment();
	                    		$logger = getDocComment($docs, 'logger');
	                    		$name = getDocComment($docs, 'name');

	                    		if ($logger)
	                    			$servicos[] = array('codigo' => $service->name, 'descricao' => $name . ' (' . $service->name . ')');
	                    	}

	                    	sort($servicos);

	                        $options = array(
	                            'titulo' => 'Selecione ao menos um serviço'
	                        );

	                        inputCombo('nomeMetodo2', $servicos, $_REQUEST['nomemetodo2'], 'cols-quantit', $options);
	                    ?>
	                    </span>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-0">
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="btnListar" type="submit"><i class="glyphicon glyphicon-search"></i> Exibir dados da tabela</button>
                    </div>
                </div>
            </form>

        </fieldset>
    </div>
     <?php
		if ($_REQUEST['nomemetodo1'] || $_REQUEST['nomemetodo2'])
		{
			$metodo = $_REQUEST['nomemetodo' . $_REQUEST['servico']];

	    	$sql = "SELECT
	    			lwsid as \"ID\",
	    			lwsrequestheader as \"Requisição Header\",
	    			lwsresponseheader as \"Resposta Header\",
	    			lwsrequestcontent as \"Requisição XML\",
	    			lwsresponsecontent as \"Resposta XML\",
	    			to_char(lwsrequesttimestamp,'DD/MM/YYYY HH24:MI:SS') as \"Requisição Data\",
	    			to_char(lwsresponsetimestamp,'DD/MM/YYYY HH24:MI:SS') as \"Resposta Data\",
	    			lwsobservacao as \"Observação\"
					FROM spo.logws WHERE lwsmetodo = '{$metodo}'
	    			ORDER BY lwsrequesttimestamp DESC";

	    	$listagem = new Simec_Listagem();
	        $listagem->setQuery($sql);
	        $listagem->turnOnPesquisator();
	        $listagem->setCabecalho('auto');
	        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS)
                ->setFormFiltros('frmConsulta')
                ->setTamanhoPagina(30);
	        $listagem->addCallbackDeCampo('Requisição Header', 'mascaraHeader');
	        $listagem->addCallbackDeCampo('Resposta Header', 'mascaraHeader');
	        $listagem->addCallbackDeCampo('Requisição XML', 'mascaraCode');
	        $listagem->addCallbackDeCampo('Resposta XML', 'mascaraCode');
	        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
		}
	?>
</div>
<div class="modal fade" id="codeModal" tabindex="-1" role="dialog" aria-hidden="true">
  <div class="modal-dialog" style="width: 70%">
    <div class="modal-content">
    	<form id="frmDownload" name="frmDownload" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
	      <div class="modal-header">
	        <button type="button" class="close" data-dismiss="modal"><span aria-hidden="true">&times;</span><span class="sr-only">Close</span></button>
	        <h4 class="modal-title" id="label">XML</h4>
	      </div>
	      <div class="modal-body">
	     	<input type="hidden" name="download" value="true" />
    		<input type="hidden" name="xml" class="xml" />
	        <pre class="prettyprint linenums code"></pre>
	      </div>
	      <div class="modal-footer">
	        <button type="submit" class="btn btn-success">Download</button>
	      	<button type="button" class="btn btn-danger" data-dismiss="modal">Ok</button>
	      </div>
	    </form>
    </div>
  </div>
</div>