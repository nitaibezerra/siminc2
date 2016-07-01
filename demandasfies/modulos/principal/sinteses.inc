<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaObservacao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelDemandaObservacao = new DemandaObservacao($_REQUEST['dmoid']);
$modelDemandaObservacaoAutor = $modelDemandaObservacaoDeterminacao = $modelDemandaObservacaoQuestionamentos = new DemandaObservacao();

$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
	$modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

$perfis = pegaPerfilGeral();
$perfis = $perfis ? $perfis : array();

$podeEditar = $modelDemanda->verificarPermissaoEdicao($estadoAtual['esdid'], $perfis);

$complementoDisabled = '';

// -- Mensagens para a interface do usuário
$fm = new Simec_Helper_FlashMessage('demandasfies/demandas');
switch ($_REQUEST['action']) {
	case 'salvar_observacao':
		$modelDemandaObservacao->popularDadosObjeto();

		if ($modelDemandaObservacao->dmotexto) {
			$modelDemandaObservacao->dmotexto = $modelDemandaObservacao->dmotexto;
		}

		if ($modelDemandaObservacao->dmoid) {
			$modelDemandaObservacao->dmodtalteracao = date('Y-m-d H:i:s');
			$modelDemandaObservacao->usucpfalteracao = $_SESSION['usucpforigem'];
		} else {
			$modelDemandaObservacao->dmodtinclusao = date('Y-m-d H:i:s');
			$modelDemandaObservacao->usucpfinclusao = $_SESSION['usucpforigem'];
		}

		$modelDemandaObservacao->esdid = $modelDemandaObservacao->dmotipo == DemandaObservacao::K_TIPO_QUESTIONAMENTOS_AREAS ? $estadoAtual['esdid'] : null;

		$sucesso = $modelDemandaObservacao->salvar();

		$modelDemandaObservacao->commit();
		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=sinteses';

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");

		die;
	case 'inativar_observacao':
		$modelDemandaObservacao->dmostatus = 'I';
		$modelDemandaObservacao->dmodtinativacao = date('Y-m-d H:i:s');
		$modelDemandaObservacao->usucpfinativacao = $_SESSION['usucpforigem'];
		$sucesso = $modelDemandaObservacao->salvar();
		$modelDemandaObservacao->commit();

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&dmdid=' . $modelDemanda->dmdid . '&aba=sinteses';

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
}

include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');
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
		<div class="row col-md-11">

			<div id="div_listagem_sintese_autor">
				<?php if ($modelDemanda->dmdid) {
					if ($modelDemandaObservacao->dmotipo == DemandaObservacao::K_TIPO_SINTESE_AUTOR) {
						$modelDemandaObservacaoAutor = $modelDemandaObservacao;
					}
					$modelDemandaObservacaoAutor->dmdid = $modelDemanda->dmdid;
					$modelDemandaObservacaoAutor->formularioSinteses(DemandaObservacao::K_TIPO_SINTESE_AUTOR, 'Síntese das alegações do autor');
				} ?>
			</div>

			<div id="div_listagem_sintese_autor">
				<?php if ($modelDemanda->dmdid) {
					if ($modelDemandaObservacao->dmotipo == DemandaObservacao::K_TIPO_DETERMINACAO_JUDICIAL) {
						$modelDemandaObservacaoDeterminacao = $modelDemandaObservacao;
					}
					$modelDemandaObservacaoDeterminacao->dmdid = $modelDemanda->dmdid;
					$modelDemandaObservacaoDeterminacao->formularioSinteses(DemandaObservacao::K_TIPO_DETERMINACAO_JUDICIAL, 'Síntese da determinação judicial');
				} ?>
			</div>

			<div id="div_listagem_sintese_autor">
				<?php if ($modelDemanda->dmdid) {
					if ($modelDemandaObservacao->dmotipo == DemandaObservacao::K_TIPO_QUESTIONAMENTOS_DIRIMIDOS) {
						$modelDemandaObservacaoQuestionamentos = $modelDemandaObservacao;
					}
					$modelDemandaObservacaoQuestionamentos->dmdid = $modelDemanda->dmdid;
					$modelDemandaObservacaoQuestionamentos->formularioSinteses(DemandaObservacao::K_TIPO_QUESTIONAMENTOS_DIRIMIDOS, 'Questionamentos a serem dirimidos');
				} ?>
			</div>

			<div id="div_listagem_sintese_autor">
				<?php if ($modelDemanda->dmdid) {
					if ($modelDemandaObservacao->dmotipo == DemandaObservacao::K_TIPO_QUESTIONAMENTOS_AREAS) {
						$modelDemandaObservacaoQuestionamentos = $modelDemandaObservacao;
					}
					$modelDemandaObservacaoQuestionamentos->dmdid = $modelDemanda->dmdid;
					$modelDemandaObservacaoQuestionamentos->formularioSinteses(DemandaObservacao::K_TIPO_QUESTIONAMENTOS_AREAS, 'Questionamentos das Áreas');
				} ?>
			</div>

		</div>
		<div class="row col-md-1">
			<?php wf_desenhaBarraNavegacao_demandasFIES($modelDemanda->dmdid, $modelDemanda->docid, array('dmdid' => $modelDemanda->dmdid), $estadoAtual['esdid']); ?>
		</div>
		<div class="clearfix"></div>

	</div>


	<script src="/library/jquery/jquery.form.min.js" type="text/javascript"></script>

	<script type="text/javascript">
		$(document).ready(function () {
			$('.cnpj').mask('99.999.999/9999-99');
			$('.moeda').mask('000.000.000.000.000,00', {reverse: true});
			$('.data').mask('99/99/9999');
			$('.data').datepicker();
		});
	</script>