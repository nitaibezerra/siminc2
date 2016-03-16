<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Solicitacao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$modelSolicitacao = new Solicitacao($_REQUEST['solid']);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasfies/solicitacao');
switch ($_REQUEST['action']) {
    case 'salvar':
        $modelSolicitacao->popularDadosObjeto();

        $sucesso = $modelSolicitacao->salvar();

        $modelSolicitacao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&solid=' . $modelSolicitacao->solid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'deletar':
        $modelSolicitacao->popularDadosObjeto();
        $modelSolicitacao->solstatus = 'I';
        $sucesso = $modelSolicitacao->salvar();

        $modelSolicitacao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&solid=' . $modelSolicitacao->solid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

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
				<input name="solid" id="solid" type="hidden" value="<?php echo $modelSolicitacao->solid; ?>">
			
				<div class="well">
					<fieldset>
						<legend>Solicitação</legend>
						
						<div class="form-group">
						    <label for="solnome" class="col-lg-4 col-md-4 control-label">Nome:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="solnome" name="solnome" type="text" class="form-control" value="<?php echo $modelSolicitacao->solnome; ?>" <?php echo $complementoDisabled; ?>>
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
			
				$sql = "select solid,solnome 
						from demandasfies.solicitacao 
						where solstatus = 'A'
						order by solnome";
			
		        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		        $listagem->setCabecalho(array('Nome'));
		        $listagem->addAcao('edit', 'editarSolicitacao');
		        $listagem->addAcao('delete', 'deletarSolicitacao');
                $listagem->addCallbackDeCampo(array('solnome'), 'alinhaParaEsquerda' );
		        $listagem->setQuery($sql);
		        $listagem->render();
			?>
			
		</div>
	</div>
</div>

<script type="text/javascript">
	function editarSolicitacao(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/solicitacao&acao=A&solid=' + id;
	}
	
	function deletarSolicitacao(id)
	{
        if (confirm('Deseja realmente excluir o registro?')) {
	        window.location = 'demandasfies.php?modulo=principal/apoio/solicitacao&acao=A&action=deletar&solid=' + id;
        }
	}
</script>