<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Tribunal.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);

$modelTribunal = new Tribunal($_REQUEST['trbid']);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('tribunaisfies/tribunais');
switch ($_REQUEST['action']) {
    case 'salvar':
        $modelTribunal->popularDadosObjeto();

        $sucesso = $modelTribunal->salvar();

        $modelTribunal->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&trbid=' . $modelTribunal->trbid;

        ob_clean();
        $fm->addMensagem($msg, $tipo);
        header("Location: {$url}");

        die;
    case 'deletar':
        $modelTribunal->popularDadosObjeto();
        $modelTribunal->trbstatus = 'I';
        $sucesso = $modelTribunal->salvar();

        $modelTribunal->commit();
        $msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
        $tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

//         $url .= '&trbid=' . $modelTribunal->trbid;

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
				<input name="trbid" id="trbid" type="hidden" value="<?php echo $modelTribunal->trbid; ?>">
			
				<div class="well">
					<fieldset>
						<legend>Tribunal Origem</legend>
						
						<div class="form-group tipo-judicial">
    						<label for="dmdcompetencia" class="col-lg-4 col-md-4 control-label">Competência:</label>

						    <div class="col-lg-8 col-md-8 ">
						        <select class="form-control" id="trbcompetencia" name="trbcompetencia">
						            <?php echo $modelDemanda->getCompetencias(); ?>
						        </select>
						    </div>
						</div>
						<div class="form-group">
						    <label for="trbnome" class="col-lg-4 col-md-4 control-label">Nome:</label>
						
						    <div class="col-lg-8 col-md-8 ">
						        <input id="trbnome" name="trbnome" type="text" class="form-control" value="<?php echo $modelTribunal->trbnome; ?>" <?php echo $complementoDisabled; ?>>
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
			
				$sql = "select trbid,trbnome,
						case 
							when trbcompetencia = 1 then 'Juizado Federal'
							when trbcompetencia = 2 then 'Juizado Estadual'
							else 'Juizado Trabalhista'
						end as competencia 
						from demandasfies.tribunal 
						where trbstatus = 'A'
						order by trbnome";
				
		        $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
		        $listagem->addCallbackDeCampo(array('trbnome','competencia'), 'alinhaParaCentro');
		        $listagem->setCabecalho(array('Nome','Competência'));
		        $listagem->addAcao('edit', 'editarTribunal');
		        $listagem->addAcao('delete', 'deletarTribunal');
		        $listagem->setQuery($sql);
		        $listagem->render();
			?>
			
		</div>
	</div>
</div>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script type="text/javascript">
	$(function(){
	    $('#form-save').submit(function(){
	        if(!$('#trbnome').val()){
	            alert('O campo "Nome" é obrigatório.');
	            return false;
	        }
	    });
	});     
			
	function editarTribunal(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/tribunal&acao=A&trbid=' + id;
	}
	
	function deletarTribunal(id)
	{
	    window.location = 'demandasfies.php?modulo=principal/apoio/tribunal&acao=A&action=deletar&trbid=' + id;
	}
</script>