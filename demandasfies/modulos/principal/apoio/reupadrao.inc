<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/ReuPadrao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);

$modelReuPadrao = new ReuPadrao($_REQUEST['repid']);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('reupadraofies/reupadrao');
switch ($_REQUEST['action']) {
    case 'salvar':
        $modelReuPadrao->popularDadosObjeto();

        $sucesso = $modelReuPadrao->salvar();

        $modelReuPadrao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&repid=' . $modelReuPadrao->repid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'deletar':
        $modelReuPadrao->popularDadosObjeto();
        $modelReuPadrao->repstatus = 'I';
        $sucesso = $modelReuPadrao->salvar();

        $modelReuPadrao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&repid=' . $modelReuPadrao->repid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
	case 'getPessoaJuridica':
		require_once APPRAIZ . 'www/includes/webservice/PessoaJuridicaClient.php';
		$pessoaJuridica = new PessoaJuridicaClient("http://ws.mec.gov.br/PessoaJuridica/wsdl");
        
		$pj = str_replace(array('/', '.', '-'), '', $_POST['cnpj']);
		$xml = $pessoaJuridica->solicitarDadosPessoaJuridicaPorCnpj($pj);
		$obj = (object)simplexml_load_string($xml);
		if (!$obj->PESSOA) {
			echo "CNPJ inexistente na base da Receita Federal.";
		}
		echo (string)$obj->PESSOA->no_empresarial_rf;
		die;
}

include APPRAIZ . "includes/cabecalho.inc";
?>
<div class="col-lg-12">

	<?php echo $fm->getMensagens(); ?>
	
	<div class="row">
		<div class="row col-md-12">
	
			<?php if (!$complementoDisabled){ ?>
				<form id="form-save" method="post" class="form-horizontal">
			<?php } ?>
				<input name="controller" type="hidden" value="instituicao">
				<input name="action" type="hidden" value="salvar">
				<input name="repid" id="repid" type="hidden" value="<?php echo $modelReuPadrao->repid; ?>">
			
				<div class="well">
					<fieldset>
						<legend>CNPJ Réu Padrão</legend>
						
						<div class="form-group">
						    <label for="repcnpj" class="col-lg-4 col-md-4 control-label">CNPJ:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="repcnpj" name="repcnpj" type="text" class="form-control" value="<?php echo $modelReuPadrao->repcnpj; ?>" <?php echo $complementoDisabled; ?>>
						    <span id="cnpj_autor" class="help-block"></span>
						    </div>
						</div>
						<div class="form-group">
						    <label for="repnome" class="col-lg-4 col-md-4 control-label">Nome:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="repnome" name="repnome" type="text" class="form-control" value="<?php echo $modelReuPadrao->repnome; ?>" <?php echo $complementoDisabled; ?>>
						    </div>
						</div>
				    </fieldset>
					<div>
					    <button title="Salvar" class="btn btn-success" type="submit"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar </button>
					</div>	
			    </div>	
			
			<?php if (!$complementoDisabled){ ?>
				</form>
			<?php } 
			
				$sql = "select repid,repnome,repcnpj
						from demandasfies.reupadrao 
						where repstatus = 'A'
						order by repnome";
				
		        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		        $listagem->addCallbackDeCampo(array('repnome','repcnpj'), 'alinhaParaCentro');
		        $listagem->addCallbackDeCampo('repcnpj', 'formatar_cnpj');
		        $listagem->setCabecalho(array('Nome','CNPJ'));
		        $listagem->addAcao('edit', 'editarReuPadrao');
		        $listagem->addAcao('delete', 'deletarReuPadrao');
		        $listagem->setQuery($sql);
		        $listagem->render();
			?>
			
		</div>
	</div>
</div>
<script type="text/javascript">
	$(function(){
	    $('#form-save').submit(function(){
	        if(!$('#repcnpj').val()){
	            alert('O campo "CNPJ" é obrigatório.');
	            return false;
	        }
	        if(!$('#repnome').val()){
	            alert('O campo "Nome" é obrigatório.');
	            return false;
	        }
	    });

	    $('#repcnpj').on('blur', function () {
            $.post(window.location.href, {action: 'getPessoaJuridica', cnpj: $('#repcnpj').val() }, function (data) {
                $('#cnpj_autor').html(data);
                $('#repnome').val(data);
            });
        });
	    
	    $('.cnpj').mask('99.999.999/9999-99');
	    
	});     
			
	function editarReuPadrao(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/reupadrao&acao=A&repid=' + id;
	}
	
	function deletarReuPadrao(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/reupadrao&acao=A&action=deletar&repid=' + id;
	}
</script>