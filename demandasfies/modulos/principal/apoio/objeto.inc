<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Objeto.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$modelObjeto = new Objeto($_REQUEST['objid']);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('objetosfies/objetos');
switch ($_REQUEST['action']) {
    case 'salvar':
        $modelObjeto->popularDadosObjeto();

        $sucesso = $modelObjeto->salvar();

        $modelObjeto->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&objid=' . $modelObjeto->objid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'deletar':
        $modelObjeto->popularDadosObjeto();
        $modelObjeto->objstatus = 'I';
        $sucesso = $modelObjeto->salvar();

        $modelObjeto->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&objid=' . $modelObjeto->objid;

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
				<input name="objid" id="objid" type="hidden" value="<?php echo $modelObjeto->objid; ?>">
			
				<div class="well">
					<fieldset>
						<legend>Objetos</legend>
						
						<div class="form-group">
						    <label for="objnome" class="col-lg-4 col-md-4 control-label">Nome:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="objnome" name="objnome" type="text" class="form-control" value="<?php echo $modelObjeto->objnome; ?>" <?php echo $complementoDisabled; ?>>
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
			
				$sql = "select objid,objnome 
						from demandasfies.objeto 
						where objstatus = 'A'
						order by objnome";
			
		        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		        $listagem->setCabecalho(array('Nome'));
		        $listagem->addAcao('edit', 'editarObjeto');
		        $listagem->addAcao('delete', 'deletarObjeto');
                $listagem->addCallbackDeCampo(array('objnome'), 'alinhaParaEsquerda' );
		        $listagem->setQuery($sql);
		        $listagem->render();
			?>
			
		</div>
	</div>
</div>

<script type="text/javascript">
	function editarObjeto(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/objeto&acao=A&objid=' + id;
	}
	
	function deletarObjeto(id)
	{
        if (confirm('Deseja realmente excluir o registro?')) {
	        window.location = 'demandasfies.php?modulo=principal/apoio/objeto&acao=A&action=deletar&objid=' + id;
        }
	}
</script>