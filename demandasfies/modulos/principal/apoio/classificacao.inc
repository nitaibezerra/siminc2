<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Classificacao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$modelClassificacao = new Classificacao($_REQUEST['claid']);

$clatipo = $_REQUEST['clatipo'] ? $_REQUEST['clatipo'] : 'G';
$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('classificacaofies/classificacao');
switch ($_REQUEST['action']) {
    case 'salvar':
        $modelClassificacao->popularDadosObjeto();
        $modelClassificacao->clatipo = $clatipo;

        $sucesso = $modelClassificacao->salvar();

        $modelClassificacao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&claid=' . $modelClassificacao->claid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'deletar':
        $modelClassificacao->popularDadosObjeto();
        $modelClassificacao->clastatus = 'I';
        $sucesso = $modelClassificacao->salvar();

        $modelClassificacao->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&claid=' . $modelClassificacao->claid;

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
				<input name="claid" id="claid" type="hidden" value="<?php echo $modelClassificacao->claid; ?>">
			
				<div class="well">
					<fieldset>
						<legend>Classificacão</legend>
						
						<div class="form-group">
						    <label for="clanome" class="col-lg-4 col-md-4 control-label">Nome:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="clanome" name="clanome" type="text" class="form-control" value="<?php echo $modelClassificacao->clanome; ?>" <?php echo $complementoDisabled; ?>>
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
			
				$sql = "select claid,clanome 
						from demandasfies.classificacao 
						where clastatus = 'A'
						and clatipo = '{$clatipo}'
						order by clanome";
			
		        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		        $listagem->setCabecalho(array('Nome'));
		        $listagem->addAcao('edit', 'editarClassificacao');
		        $listagem->addAcao('delete', 'deletarClassificacao');
		        $listagem->setQuery($sql);
		        $listagem->render();
			?>
			
		</div>
	</div>
</div>

<script type="text/javascript">
	function editarClassificacao(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/classificacao&acao=A&claid=' + id;
	}
	
	function deletarClassificacao(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/classificacao&acao=A&action=deletar&claid=' + id;
	}
</script>