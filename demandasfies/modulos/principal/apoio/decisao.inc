<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Decisao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$modelDecisao = new Decisao($_REQUEST['decid']);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasfies/decisao');
switch ($_REQUEST['action']) {
    case 'salvar':
        $modelDecisao->popularDadosObjeto();

        $sucesso = $modelDecisao->salvar();

        $modelDecisao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&decid=' . $modelDecisao->decid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'deletar':
        $modelDecisao->popularDadosObjeto();
        $modelDecisao->decstatus = 'I';
        $sucesso = $modelDecisao->salvar();

        $modelDecisao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&decid=' . $modelDecisao->decid;

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
				<input name="decid" id="decid" type="hidden" value="<?php echo $modelDecisao->decid; ?>">
			
				<div class="well">
					<fieldset>
						<legend>Decisão</legend>
						
						<div class="form-group">
						    <label for="decnome" class="col-lg-4 col-md-4 control-label">Nome:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="decnome" name="decnome" type="text" class="form-control" value="<?php echo $modelDecisao->decnome; ?>" <?php echo $complementoDisabled; ?>>
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
			
				$sql = "select decid,decnome 
						from demandasfies.decisao 
						where decstatus = 'A'
						order by decnome";
			
		        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		        $listagem->setCabecalho(array('Nome'));
		        $listagem->addAcao('edit', 'editarDecisao');
		        $listagem->addAcao('delete', 'deletarDecisao');
                $listagem->addCallbackDeCampo(array('decnome'), 'alinhaParaEsquerda' );
		        $listagem->setQuery($sql);
		        $listagem->render();
			?>
			
		</div>
	</div>
</div>

<script type="text/javascript">
	function editarDecisao(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/decisao&acao=A&decid=' + id;
	}
	
	function deletarDecisao(id)
	{
        if (confirm('Deseja realmente excluir o registro?')) {
	        window.location = 'demandasfies.php?modulo=principal/apoio/decisao&acao=A&action=deletar&decid=' + id;
        }

	}
</script>