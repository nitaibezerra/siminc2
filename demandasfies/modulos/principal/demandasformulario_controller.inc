<?php
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/ResponsavelArea.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaObservacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/HistoricoIntervencao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
include_once APPRAIZ . "spo/autoload.php";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelResponsavelArea = new ResponsavelArea($_REQUEST['reaid']);
$modelDemandaObservacao = new DemandaObservacao($_REQUEST['dmoid']);
$areaSelecionada = utf8_decode($_REQUEST['area']);

$perfis = pegaPerfilGeral();
$perfis = $perfis ? $perfis : array();

$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
	$modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

$fm = new Simec_Helper_FlashMessage('demandasfies/demandas');

switch ($_REQUEST['action']) {
	case 'modal_observacao':
		$modelDemandaObservacao->popularDadosObjeto();

		$sql = "select dmotexto, usunome, to_char(dmo.dmodtinclusao, 'DD/MM/YYYY HH:24:mi:ss') as data, esddsc
                from demandasfies.demandaobservacao dmo
                    INNER join seguranca.usuario u on dmo.usucpfinclusao = u.usucpf
                    LEFT  join workflow.estadodocumento ed on ed.esdid = dmo.esdid
                where dmoid = {$modelDemandaObservacao->dmoid}";

		$dados = $modelDemandaObservacao->pegaLinha($sql);
		$dados = $dados ? $dados : array();

		?>
		<p style="color: red;"><?php echo $titulo; ?><?php echo $dados['esddsc'] ? " <strong>({$dados['esddsc']})</strong>" : '' ?></p>
		<p>Por: <strong><?php echo $dados['usunome']; ?></strong></p>
		<p>Em: <strong><?php echo $dados['data']; ?></strong></p>

		<p style="margin-top: 10px; margin-bottom: 50px;">
			<?php echo nl2br($dados['dmotexto']); ?>
		</p>

		<?php
		die;
	case 'modal_alteracao_prazo':
		$modelResponsavelArea->popularDadosObjeto();
		?>

		<form id="form-save-prazo" method="post" class="form-horizontal">
			<input name="action" type="hidden" value="salvar-prazo"> <input name="reaid" id="reaid" type="hidden" value="<?php echo $modelResponsavelArea->reaid; ?>">
			<input name="area" id="area_selecionada" type="hidden" value="<?php echo $areaSelecionada; ?>">

			<div class="form-group">
				<label for="reaprazo" class="col-lg-4 col-md-4 control-label">Novo Prazo:</label>

				<div class="col-lg-8 col-md-8 ">
					<input id="reaprazo_alteracao" name="reaprazo" type="text" class="form-control data">
				</div>
			</div>

			<div class="form-group">
				<label for="cmddsc" class="col-lg-4 col-md-4 control-label">Justificativa:</label>

				<div class="col-lg-8 col-md-8">
					<textarea class="form-control" rows="5" name="reajustificativa" id="reajustificativa_alteracao"></textarea>
				</div>
			</div>

		</form>

		<script type="text/javascript">
			$(function () {
				$('.data').mask('99/99/9999');


				$('.data').datepicker({
					onSelect: function (dateText) {
						var dtInicio = $("#reaprazo_cadastramento_prof").val().split("/");
						var dtFim = dateText.split("/");
						data1 = new Date(dtInicio[2] + "/" + dtInicio[1] + "/" + dtInicio[0]);
						data2 = new Date(dtFim[2] + "/" + dtFim[1] + "/" + dtFim[0]);
						var total = dateDiferencaEmDias(data1, data2);

						<?php if($areaSelecionada == 'Procuradoria Federal' AND ( in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_PROCURADOR_FEDERAL, $perfis) ) ){ ?>
						<?php }elseif ($areaSelecionada == 'Analista DTI/MEC' AND (in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_DTI_MEC, $perfis)) ){ ?>
//						CONFORME E-MAIL DO GESTOR RAFAEL TAVARES
						<?php }else{ ?>
						if (total > 0) {
							alert('Data inserida é maior que a data da Procuradoria Federal!');
							$(this).val('');
						}
						<?php } ?>
					}
				});
			});


		</script>

		<?php
		die;
	case 'salvar-prazo':

		// Inativando registro
		$modelResponsavelArea->reastatus = 'I';
		$modelResponsavelArea->readtinativacao = date('Y-m-d H:i:s');
		$modelResponsavelArea->usucpfinativacao = $_SESSION['usucpforigem'];
		$modelResponsavelArea->reajustificativa = $_REQUEST['reajustificativa'];

		$dataAntiga = formata_data($modelResponsavelArea->reaprazo);
		$sucesso = $modelResponsavelArea->salvar();
		$modelResponsavelArea->commit();

		// Criando novo registro
		$modelResponsavelArea->reaid = $modelResponsavelArea->reastatus = $modelResponsavelArea->usucpfinativacao = $modelResponsavelArea->readtinativacao = $modelResponsavelArea->reajustificativa = null;

		$modelResponsavelArea->reaprazo = $_REQUEST['reaprazo'];
		$modelResponsavelArea->readtinclusao = date('Y-m-d H:i:s');
		$modelResponsavelArea->usucpfinclusao = $_SESSION['usucpforigem'];

		$sucesso = $modelResponsavelArea->salvar();
		$modelResponsavelArea->commit();

		// Envia email para o setor ou responsavel
		$paransEmail = $modelDemanda->getParansEmailAlteracaoDataArea($dataAntiga, $_REQUEST['reaprazo'], $_REQUEST['reajustificativa'], $_REQUEST['area']);
		enviarEmailDemandasFies($modelResponsavelArea->esdid, $_REQUEST['dmdid'], $paransEmail);

		if ($modelResponsavelArea->esdid == ESD_DEMANDA_EM_CADASTRAMENTO) {
			enviarEmailDemandasFies(ESD_DEMANDA_NUCLEO_JURIDICO, $_REQUEST['dmdid'], $paransEmail);
			enviarEmailDemandasFies(ESD_DEMANDA_ADVOGADO, $_REQUEST['dmdid'], $paransEmail);

			$cpfAdvogado = $modelResponsavelArea->getCpfAdvogadoDemanda( $modelResponsavelArea->dmdid );

			$paransAviso = array(
				'sisid' => SIS_DEMANDASFIES,
				'usucpf' => $cpfAdvogado,
				'mensagem' => "A data da Procuradoria Federal foi alterada para a demanda {$modelResponsavelArea->dmdid} de {$dataAntiga} para {$modelResponsavelArea->reaprazo} " ,
				'url' => "/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid={$modelResponsavelArea->dmdid}&aba=responsaveis",
			);
			cadastrarAvisoUsuario($paransAviso);
		}

		$msg = $sucesso ? 'Operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&aba=responsaveis&dmdid=' . $modelDemanda->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");

		die;
	case 'salvar-intervencao':

		$msg = $tipo = '.';
		if ($estadoAtual['esdid'] && $modelDemanda->docid) {

			$sql = "SELECT aedid FROM workflow.acaoestadodoc
                    WHERE esdiddestino = " . ESD_DEMANDA_EM_INTERVENCAO . "
                    AND esdidorigem = {$estadoAtual['esdid']}";
			$aedid = $modelDemanda->pegaUm($sql);

			$sql = "select pfldsc
                    from seguranca.perfil
                    where sisid = " . SIS_DEMANDASFIES . "
                    and pflcod = {$_REQUEST['pflcod']}
                    order by pfldsc;";

			$pfldsc = $modelDemanda->pegaUm($sql);

			$cmddsc = "Solicitação de intervenção da área ({$pfldsc}): " . $_REQUEST['cmddsc'];
			$a = wf_alterarEstado($modelDemanda->docid, $aedid, $cmddsc, array('dmdid' => $modelDemanda->dmdid, 'esdid' => ESD_DEMANDA_EM_INTERVENCAO));

			$sql = "SELECT * FROM workflow.documento
                    WHERE docid = {$modelDemanda->docid}";
			$dados = $modelDemanda->pegaLinha($sql);

			$modeloHistorico = new HistoricoIntervencao();

			$sql = "update demandasfies.historicointervencao set hiistatus = 'I' where dmdid = {$modelDemanda->dmdid}";
			$modeloHistorico->executar($sql);

			$modeloHistorico->dmdid = $modelDemanda->dmdid;
			$modeloHistorico->docid = $modelDemanda->docid;
			$modeloHistorico->hstid = $dados['hstid'];
			$modeloHistorico->pflcod = $_REQUEST['pflcod'];
			$modeloHistorico->hiistatus = 'A';
			$modeloHistorico->esdidorigem = $estadoAtual['esdid'];
			$modeloHistorico->salvar();

			$modeloHistorico->commit();
			$msg = 'Operação realizada com sucesso!';
			$tipo = Simec_Helper_FlashMessage::SUCESSO;
		}


		$url .= '&dmdid=' . $modelDemanda->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
	case 'salvar-retorno-intervencao':

		$msg = $tipo = '';
		if ($estadoAtual['esdid'] && $modelDemanda->docid) {

			$sql = "select * from demandasfies.historicointervencao h where dmdid = $modelDemanda->dmdid and hiistatus = 'A'";
			$dados = $db->pegaLinha($sql);

			$sql = "SELECT aedid FROM workflow.acaoestadodoc
                    WHERE esdiddestino = {$dados['esdidorigem']}
                    AND esdidorigem = " . ESD_DEMANDA_EM_INTERVENCAO;
			$aedid = $modelDemanda->pegaUm($sql);

			$cmddsc = "Demanda retornada à área demandante: " . $_REQUEST['cmddsc'];
			wf_alterarEstado($modelDemanda->docid, $aedid, $cmddsc, array('dmdid' => $modelDemanda->dmdid, 'esdid' => ESD_DEMANDA_EM_INTERVENCAO));

			$sql = "SELECT * FROM workflow.documento
                    WHERE docid = {$modelDemanda->docid}";
			$dados = $modelDemanda->pegaLinha($sql);

			$modeloHistorico = new HistoricoIntervencao();

			$sql = "update demandasfies.historicointervencao set hiistatus = 'I' where dmdid = {$modelDemanda->dmdid}";
			$modeloHistorico->executar($sql);
			$modeloHistorico->commit();
			$msg = 'Operação realizada com sucesso!';
			$tipo = Simec_Helper_FlashMessage::SUCESSO;
		}

		$url .= '&dmdid=' . $modelDemanda->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");
		die;
}

$abaAtiva = (isset($_REQUEST['aba']) ? $_REQUEST['aba'] : 'geral');
// -- URL base das abas
$urlBaseDasAbas = "/demandasfies/demandasfies.php?modulo=principal/demandasformulario&acao=A&dmdid={$_REQUEST['dmdid']}&aba=";

$listaAbas = array();
$listaAbas[] = array("id" => 10, "descricao" => "Geral", "link" => "{$urlBaseDasAbas}geral");

if (!empty($_REQUEST['dmdid'])) {

	if ($db->testa_superuser() || in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_PROCURADOR_FEDERAL, $perfis)) {
		$listaAbas[] = array("id" => 15, "descricao" => "Dados do Procurador", "link" => "{$urlBaseDasAbas}procuradores");
	}

	$listaAbas[] = array("id" => 20, "descricao" => "Sínteses", "link" => "{$urlBaseDasAbas}sinteses");
	$listaAbas[] = array("id" => 30, "descricao" => "Entregas", "link" => "{$urlBaseDasAbas}entregas");
	$listaAbas[] = array("id" => 40, "descricao" => "Responsáveis", "link" => "{$urlBaseDasAbas}responsaveis");

	$perfis = pegaPerfilGeral();

	if (in_array(PFL_DISTRIBUIDOR, $perfis) && !in_array(PFL_NUCLEO_JURIDICO, $perfis)) {
		$abaAtiva = 'responsaveis';
		$listaAbas = array();
		$listaAbas[] = array("id" => 40, "descricao" => "Responsáveis", "link" => "{$urlBaseDasAbas}responsaveis");
	}
	if (in_array(PFL_ADVOGADO, $perfis) || in_array(PFL_SUPER_USUARIO, $perfis)) {
		$listaAbas[] = array("id" => 40, "descricao" => "Em Atendimento", "link" => "{$urlBaseDasAbas}em_atendimento");
	}

}

/**
 * Incluíndo o arquivo de acordo com a aba selecionada.
 * Para cada aba, é chamado o arquivo início dentro do diretório
 * com o mesmo nome da aba selecionada.
 */
require_once(dirname(__FILE__) . "/{$abaAtiva}.inc");