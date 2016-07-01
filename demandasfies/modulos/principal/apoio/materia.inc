<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Materia.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$modelMateria = new Materia($_REQUEST['matid']);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasfies/materia');
switch ($_REQUEST['action']) {
    case 'salvar':
        $modelMateria->popularDadosObjeto();

        $sucesso = $modelMateria->salvar();

        $modelMateria->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&matid=' . $modelMateria->matid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'deletar':
        $modelMateria->popularDadosObjeto();
        $modelMateria->matstatus = 'I';
        $sucesso = $modelMateria->salvar();

        $modelMateria->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&matid=' . $modelMateria->matid;

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
				<input name="matid" id="matid" type="hidden" value="<?php echo $modelMateria->matid; ?>">
			
				<div class="well">
					<fieldset>
						<legend>Matéria</legend>
						
						<div class="form-group">
						    <label for="matnome" class="col-lg-4 col-md-4 control-label">Nome:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="matnome" name="matnome" type="text" class="form-control" value="<?php echo $modelMateria->matnome; ?>" <?php echo $complementoDisabled; ?>>
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
			
				$sql = "select matid,matnome
						from demandasfies.materia 
						where matstatus = 'A'
						order by matnome";
			
		        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		        $listagem->setCabecalho(array('Nome'));
		        $listagem->addAcao('edit', 'editarMateria');
		        $listagem->addAcao('delete', 'deletarMateria');
                $listagem->addCallbackDeCampo(array('matnome'), 'alinhaParaEsquerda' );
		        $listagem->setQuery($sql);
		        $listagem->render();
			?>
			
		</div>
	</div>
</div>

<script type="text/javascript">
	function editarMateria(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/materia&acao=A&matid=' + id;
	}
	
	function deletarMateria(id)
	{
        if (confirm('Deseja realmente excluir o registro?')) {
	        window.location = 'demandasfies.php?modulo=principal/apoio/materia&acao=A&action=deletar&matid=' + id;
        }
	}
</script>