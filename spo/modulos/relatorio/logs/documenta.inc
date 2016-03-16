<?php
if ($_REQUEST['download']) {
	$file = md5(time()) . '.xml';
	header('Content-type: text/xml');
	header('Content-Disposition: attachment; filename="' . $file . '"');
	echo stripslashes($_REQUEST['xml']);
	die;
}

require_once APPRAIZ . 'www/spo/_funcoes.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
require_once APPRAIZ . 'includes/funcoesspo.php';
require_once APPRAIZ . 'includes/library/simec/Helper/FlashMessage.php';
require_once APPRAIZ . 'spo/ws/sof/Qualitativo.php';

$mapServiçosTED1 = array
(
	'Solicitar processo' => array('ws' => 'solicitarProcesso', 'tbl' => 'seguranca.historicowsprocessofnde', 'param' => 'sisid = 2'),
);

ksort($mapServiçosTED1);

$mapServiçosTED2 = array
(
	'Solicitar NC' => array('ws' => 'solicitarNC', 'tbl' => 'ted.log', 'param' => 'logtipo = 3'),
	'Consultar NC' => array('ws' => 'consultarNC', 'tbl' => 'ted.log', 'param' => 'logtipo = 2'),
	'Solicitar PF' => array('ws' => 'solicitarPF', 'tbl' => 'ted.log', 'param' => 'logtipo = 4'),
	'Solicitar processo' => array('ws' => 'solicitarProcesso', 'tbl' => 'ted.log', 'param' => 'logtipo = 1'),
);

ksort($mapServiçosTED2);

$fm = new Simec_Helper_FlashMessage('spo/relatorio/logs/listar');
?>
<script type="text/javascript" src="/includes/funcoesspo.js"></script>
<script>
$(document).ready(function() {
    $('#ted1,#ted2').on("change", function() {
    	$('#servico1,#servico2').val('').trigger('chosen:updated');
        $(this).val() == 1 ? 
        ($('#ted-1').show(), $('#ted-2').hide()) : 
        ($('#ted-2').show(), $('#ted-1').hide());
    });
	$('#codeModal').on('show.bs.modal', function (event) {
	  var button = $(event.relatedTarget);
	  var code = button.data('code');
	  var xml = button.data('xml');
	  var modal = $(this);
	  modal.find('.modal-body').find('.code').text(formatXml(code)).prettify();
	  modal.find('.modal-body').find('.xml').val(formatXml(xml));
	});
	$('#ted1').change();
});
</script>
<div class="col-lg-12">
    <div class="page-header">
        <ol class="breadcrumb">
            <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li>SIOP (SOF)</li>
            <li>Relatório</li>
            <li class="active">Listar logs das transações do DOCUMENTA</li>
        </ol>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <div class="well">
        <fieldset>
            <form id="frmConsulta" name="frmConsulta" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
                <div class="form-group">
                    <label for="ted" class="col-lg-2 control-label">
                        TED:
                    </label>
                    <div class="col-lg-2">
                    <?php
			            $opcoes = array('TED 1.0' => '1', 'TED 2.0' => '2');
			            inputChoices('ted', $opcoes, isset($_REQUEST['ted']) ? $_REQUEST['ted'] : '1', 'ted');
		            ?>	
                    </div>
                </div>
				<div class="form-group">
                    <label for="ted" class="col-lg-2 control-label">
                       Serviço:
                    </label>
                    <div class="col-lg-6">
	                    <span id="ted-1">
	                    <?php 
	                    	$dados = array();
	                    	
	                    	foreach ($mapServiçosTED1 as $name => $info) {
	                    		$dados[] = array('codigo' => $name, 'descricao' => $name);
	                    	}

	                        $options = array(
	                            'titulo' => 'Selecione ao menos um serviço'
	                        );
	                        
	                        inputCombo('servico1', $dados, $_REQUEST['servico1'], 'servico1', $options);
	                    ?>
	                    </span>
	                    <span id="ted-2">
	                    <?php 
	                    	$dados = array();
	                    	
	                    	foreach ($mapServiçosTED2 as $name => $info) {
	                    		$dados[] = array('codigo' => $name, 'descricao' => $name);
	                    	}

	                        $options = array(
	                            'titulo' => 'Selecione ao menos um serviço'
	                        );
		                        
	                        inputCombo('servico2', $dados, $_REQUEST['servico2'], 'servico2', $options);
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
    	if ($_REQUEST['ted']) 
    	{
	    	$servico = $_REQUEST['ted'] == '1' ? $mapServiçosTED1[$_REQUEST['servico1']] : $mapServiçosTED2[$_REQUEST['servico2']];
	    	
	    	$sql = "SELECT 
	    			logid as \"ID\",
	    			logmsg as \"Mensagem\",
	    			logxmlenvio as \"Requisição XML\",
	    			logxmlretorno as \"Resposta XML\",
	    			to_char(logdata,'DD/MM/YYYY HH24:MI:SS') as \"Requisição Data\", 
	    			to_char(logdtretorno,'DD/MM/YYYY HH24:MI:SS') as \"Resposta Data\"
					FROM {$servico['tbl']} 
					WHERE {$servico['param']}
					ORDER BY logdata DESC";
	    	
			$listagem = new Simec_Listagem();
	        $listagem->setQuery($sql);
	        $listagem->turnOnPesquisator();
	        $listagem->setCabecalho('auto');
	        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
	        $listagem->addCallbackDeCampo('Mensagem', 'mascaraHeader');
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