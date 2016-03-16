<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/AcaoJudicial.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$modelAcaojudicial = new Acaojudicial($_REQUEST['acjid']);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasfies/acaojudicial');
switch ($_REQUEST['action']) {
    case 'salvar':
        $modelAcaojudicial->popularDadosObjeto();

        $sucesso = $modelAcaojudicial->salvar();

        $modelAcaojudicial->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&acjid=' . $modelAcaojudicial->acjid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'deletar':
        $modelAcaojudicial->popularDadosObjeto();
        $modelAcaojudicial->acjstatus = 'I';
        $sucesso = $modelAcaojudicial->salvar();

        $modelAcaojudicial->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&acjid=' . $modelAcaojudicial->acjid;

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
				<input name="acjid" id="acjid" type="hidden" value="<?php echo $modelAcaojudicial->acjid; ?>">
			
				<div class="well">
					<fieldset>
						<legend>Tipo de Ação</legend>
						
						<div class="form-group">
						    <label for="acjnome" class="col-lg-4 col-md-4 control-label">Nome:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="acjnome" name="acjnome" type="text" class="form-control" value="<?php echo $modelAcaojudicial->acjnome; ?>" <?php echo $complementoDisabled; ?>>
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
			
				$sql = "select acjid,acjnome 
						from demandasfies.acaojudicial 
						where acjstatus = 'A'
						order by acjnome";
			
		        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		        $listagem->setCabecalho(array('Nome'));
		        $listagem->addAcao('edit', 'editarAcaojudicial');
		        $listagem->addAcao('delete', 'deletarAcaojudicial');
                $listagem->addCallbackDeCampo(array('acjnome'), 'alinhaParaEsquerda' );
		        $listagem->setQuery($sql);
		        $listagem->render();
			?>
			
		</div>
	</div>
</div>

<script type="text/javascript">
	function editarAcaojudicial(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/tipoacao&acao=A&acjid=' + id;
	}
	
	function deletarAcaojudicial(id)
	{
        if (confirm('Deseja realmente excluir o registro?')) {
	        window.location = 'demandasfies.php?modulo=principal/apoio/tipoacao&acao=A&action=deletar&acjid=' + id;
        }
	}
</script>