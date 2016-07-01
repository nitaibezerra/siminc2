<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaObservacao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";
include APPRAIZ . "includes/cabecalho.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);

$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
	$modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

$perfis = pegaPerfilGeral();
$perfis = $perfis ? $perfis : array();

$podeEditar = $modelDemanda->verificarPermissaoEdicao($estadoAtual['esdid'], $perfis);
$reaprazo = $modelDemanda->getDataRespProfeByDmdid();

$fm = new Simec_Helper_FlashMessage('demandasfies/demandas');
switch ($_POST['action']) {
	case 'salvar-data-atendimento':

		$modelDemanda = new Demanda($_POST['dmdid']);
		$modelDemanda->popularDadosObjeto();
		$sucesso = $modelDemanda->salvar();
		$modelResponsavelArea->commit();

//		// Envia email para o setor ou responsavel
//		$paransEmail = $modelDemanda->getParansEmailAlteracaoDataArea($dataAntiga, $_REQUEST['reaprazo'], $_REQUEST['reajustificativa'], $_REQUEST['area']);
//		enviarEmailDemandasFies($modelResponsavelArea->esdid, $_REQUEST['dmdid'], $paransEmail);
//
//		if ($modelResponsavelArea->esdid == ESD_DEMANDA_EM_CADASTRAMENTO) {
//			enviarEmailDemandasFies(ESD_DEMANDA_NUCLEO_JURIDICO, $_REQUEST['dmdid'], $paransEmail);
//			enviarEmailDemandasFies(ESD_DEMANDA_ADVOGADO, $_REQUEST['dmdid'], $paransEmail);
//
//			$cpfAdvogado = $modelResponsavelArea->getCpfAdvogadoDemanda($modelResponsavelArea->dmdid);
//
//			$paransAviso = array('sisid' => SIS_DEMANDASFIES, 'usucpf' => $cpfAdvogado, 'mensagem' => "A data da Procuradoria Federal foi alterada para a demanda {$modelResponsavelArea->dmdid} de {$dataAntiga} para {$modelResponsavelArea->reaprazo} ", 'url' => "/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid={$modelResponsavelArea->dmdid}&aba=responsaveis",);
//			cadastrarAvisoUsuario($paransAviso);
//		}

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;
		$url .= '&aba=em_atendimento&dmdid=' . $modelDemanda->dmdid;
		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
}

?>
<div class="col-lg-12">
	<div class="page-header">
		<h3 id="forms"><?php echo $modelDemanda->dmdid ? 'Demanda ' . $modelDemanda->dmdid : 'Nova Demanda'; ?></h3>
	</div>

	<?php
	echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");
	echo $fm->getMensagens();
	?>

	<div class="row">
		<div class="row col-md-12">
			<h3>Data de Atendimento <small> <?php echo ($modelDemanda->dmdstatusatendimento == 'A' ? '- <b style="color: #1b903d">ATIVADO</b>' : '- <b style="color: #843534">DESATIVADO</b>'); ?></small></h3>

			<form id="form-save-prazo" method="post" class="form-horizontal">
				<input name="action" type="hidden" value="salvar-data-atendimento">
				<input name="dmdid" id="dmdid" type="hidden" value="<?php echo $modelDemanda->dmdid; ?>">
				<input name="reaprazo" id="reaprazo" type="hidden" value="<?php echo $reaprazo; ?>">

				<div class="form-group">
					<label for="dmddiasatendimento" class="col-lg-4 col-md-4 control-label">Informe a quantidade dias:</label>

					<div class="col-lg-2 col-md-2">
						<input id="dmddiasatendimento" name="dmddiasatendimento" type="text" class="form-control somente-numero" value="<?php echo $modelDemanda->dmddiasatendimento ?>">
					</div>

					<div class="col-lg-4 col-md-4">
						<p class="help-block" style="display: none" id="data_prevista"></p>
					</div>
				</div>

				<div class="form-group tipo-judicial">
					<label for="dmdstatusatendimento" class="col-lg-4 col-md-4 control-label"><span style="color: red;">*</span> Ativar data:</label>

					<div class="col-lg-8 col-md-8 ">
						<div class="btn-group" data-toggle="buttons">
							<label class="btn btn-default <?= ($modelDemanda->dmdstatusatendimento == 'A' ? 'active' : ''); ?> ">
								<input class="campo-decisao" type="radio" name="dmdstatusatendimento" id="dmdstatusatendimento1" value="A" <?= ($modelDemanda->dmdstatusatendimento == 'A' ? 'checked="checked"' : ''); ?>> Sim
							</label>
							<label class="btn btn-default <?= ($modelDemanda->dmdstatusatendimento == 'I' ? 'active' : ''); ?>">
								<input class="campo-decisao" type="radio" name="dmdstatusatendimento" id="dmdstatusatendimento2" value="I" <?= ($modelDemanda->dmdstatusatendimento == 'I' ? 'checked="checked"' : ''); ?>> Não
							</label>
						</div>
					</div>
				</div>

				<div class="form-group">
					<div class="col-lg-8 col-md-8 text-right">
						<button title="Salvar" class="btn btn-success" type="submit">
							<span class="glyphicon glyphicon-thumbs-up"></span> Salvar
						</button>
					</div>
				</div>

			</form>
			<hr>
		</div>
		<div class="clearfix"></div>
	</div>


	<script src="/library/jquery/jquery.form.min.js" type="text/javascript"></script>

	<script type="text/javascript">
		$(function () {
			calculaDataProfe();

			$(".somente-numero").bind("keyup blur focus", function(e) {
				e.preventDefault();
				var expre = /[^0-9]/g;
				if ($(this).val().match(expre))
					$(this).val($(this).val().replace(expre,''));
			});

			$("#dmddiasatendimento").on("blur", function(e) {
				calculaDataProfe();
			});

		});

		function calculaDataProfe(){
			var dias = parseInt( $("#dmddiasatendimento").val() );
			var prazo_profe = $("#reaprazo").val().split("/");
			if(prazo_profe){
				data_profe = new Date(prazo_profe[2] + "/" + prazo_profe[1] + "/" + prazo_profe[0]);
				if(dias){
					data_profe.setDate(data_profe.getDate() + dias);
					$("#data_prevista").html(' (Data da PROFE + dias) = '+ data_profe.toLocaleDateString()  ).show();
				}
			}
		}
	</script>