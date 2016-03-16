<?
//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/ResponsavelArea.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelResponsavelArea = new ResponsavelArea();
$complementoDisabled = '';

$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
	$modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

// -- Mensagens para a interface do usuario
$fm = new Simec_Helper_FlashMessage('demandasfies/demandas');
switch ($_REQUEST['action']) {
	case 'listarAlteracoes':
		$dmdid = (int)$_GET['dmdid'];
		$esdid = (int)$_GET['esdid'];

		if (!empty($dmdid) && !empty($esdid)) {
			$dados = $modelResponsavelArea->lista(null, array("dmdid = {$dmdid}", "esdid = {$esdid}"));
			$html = $modelResponsavelArea->getHtmlTabelaHistoricoData($dados);
			echo $html;
		}
		die();

	case 'salvar':
		if ($_REQUEST['responsavel'] && is_array($_REQUEST['responsavel'])) {
			foreach ($_REQUEST['responsavel'] as $esdid => $dados) {
				$modelResponsavelArea = new ResponsavelArea($dados['reaid']);
				$modelResponsavelArea->popularDadosObjeto($dados);

				$modelResponsavelArea->esdid = $esdid;
				$modelResponsavelArea->dmdid = $modelDemanda->dmdid;
				$modelResponsavelArea->reaprazo = $dados['reaprazo'] ? formata_data_sql($dados['reaprazo']) : $modelResponsavelArea->reaprazo;
				$modelResponsavelArea->readtsubsidio = $dados['readtsubsidio'] ? formata_data_sql($dados['readtsubsidio']) : $modelResponsavelArea->readtsubsidio;
				$modelResponsavelArea->usucpf = $modelResponsavelArea->usucpf ? $modelResponsavelArea->usucpf : null;

				$modelResponsavelArea->readtinclusao = date('Y-m-d H:i:s');
				$modelResponsavelArea->usucpfinclusao = $_SESSION['usucpforigem'];

				$sucesso = $modelResponsavelArea->salvar(true, true, array('usucpf', 'esdid', 'reaprazo', 'readtsubsidio'));
				$modelResponsavelArea->commit();
				unset($modelResponsavelArea);
			}
		}

		$msg = $sucesso ? 'operação realizada com sucesso!' : 'Ocorreu um erro ao processar operação.';
		$tipo = $sucesso ? Simec_Helper_FlashMessage::SUCESSO : Simec_Helper_FlashMessage::ERRO;

		$url .= '&aba=responsaveis&dmdid=' . $modelDemanda->dmdid;

		ob_clean();
		$fm->addMensagem($msg, $tipo);
		header("Location: {$url}");

		die;
}

include APPRAIZ . "includes/cabecalho.inc";
$db->cria_aba($abacod_tela, $url, '');
?>

<style type="text/css">
	.chosen {
		width: 300px;
	}
</style>

<div class="col-lg-12">
	<div class="page-header">
		<h3 id="forms"><?php echo $modelDemanda->dmdid ? 'Demanda ' . $modelDemanda->dmdid : 'Nova Demanda'; ?></h3>
	</div>

	<?php

	// -- HTML das abas
	echo montarAbasArray($listaAbas, "{$urlBaseDasAbas}{$abaAtiva}");

	echo $fm->getMensagens();

	$listaEstados = ResponsavelArea::getArrayPerfilResponsaveis();
	//ver($listaEstados, d);
	$sisid = SIS_DEMANDASFIES;
	$sql = "select p.pflcod, p.pfldsc, u.usucpf, u.usunome, u.usuemail, '(' || u.usufoneddd || ') ' || u.usufonenum as telefone
        from seguranca.perfilusuario pu
                inner join seguranca.perfil p on p.pflcod = pu.pflcod
                inner join seguranca.usuario u on u.usucpf = pu.usucpf
        where sisid = {$sisid}";

	$dados = $modelDemanda->carregar($sql);
	$dados = $dados ? $dados : array();
	$usuarios = array();
	foreach ($dados as $dado) {
		$usuarios[$dado['pflcod']][] = array('codigo' => $dado['usucpf'], 'descricao' => $dado['usunome']);
	}

	$sql = "select ra.*, u.usucpf, u.usunome, u.usuemail, '(' || u.usufoneddd || ') ' || u.usufonenum as telefone
        from demandasfies.responsavelarea ra
                left join seguranca.usuario u on u.usucpf = ra.usucpf
        where dmdid = {$modelDemanda->dmdid}
        and reastatus = 'A'";

	$dados = $modelDemanda->carregar($sql);
	$dados = $dados ? $dados : array();

	$responsaveis = array();
	foreach ($dados as $dado) {
		$responsaveis[$dado['esdid']] = $dado;
	}
	$perfis = pegaPerfilGeral();
	$perfis = $perfis ? $perfis : array();

	?>

	<div class="col-lg-12">

		<?php echo $fm->getMensagens(); ?>

		<div class="row">
			<div class="row col-md-11">

				<form id="form-save-responsavel" method="post" class="form-horizontal">
					<input name="action" type="hidden" value="salvar"> <input name="dmdid" type="hidden" value="<?php echo $modelDemanda->dmdid; ?>">

					<table class="table table-bordered table-hover table-striped table-condensed">
						<thead>
						<tr>
							<th></th>
							<th>Área</th>
							<th>Responsável</th>
							<th>Telefone</th>
							<th>Email</th>
							<th>Data de Cumprimento</th>
							<th>Data de Subsídios</th>
						</tr>
						</thead>
						<tbody>
						<?php

						foreach ($listaEstados as $esdid => $dados) {
							?>
							<tr>
								<td style="width:33px" class="text-center bt_historico_alteracao" data-esdid="<?php echo $responsaveis[$esdid]['esdid']; ?>">
									<a title="Mais informações" data-toggle="modal" data-target="#visualizar_historico_justificativa">
										<span class="btn btn-info btn-sm glyphicon glyphicon-info-sign"></span> </a>
								</td>
								<td style="width: 20%;"><?php echo $dados['area']; ?></td>

								<?php

								$podeEditarData = ResponsavelArea::podeEditarData($perfis, $esdid);
								$podeEditarResponsavel = ResponsavelArea::podeEditarResponsavel($perfis, $esdid);

								$reaid = !empty($responsaveis[$esdid]['reaid']) ? $responsaveis[$esdid]['reaid'] : '';
								$usucpf = !empty($responsaveis[$esdid]['usucpf']) ? $responsaveis[$esdid]['usucpf'] : '';
								$usunome = !empty($responsaveis[$esdid]['usunome']) ? $responsaveis[$esdid]['usunome'] : '';
								$usuemail = !empty($responsaveis[$esdid]['usuemail']) ? $responsaveis[$esdid]['usuemail'] : '';
								$telefone = !empty($responsaveis[$esdid]['telefone']) ? $responsaveis[$esdid]['telefone'] : '';
								$dmdprazo = $responsaveis[$esdid]['reaprazo'] ? substr($responsaveis[$esdid]['reaprazo'], 8, 2) . '/' . substr($responsaveis[$esdid]['reaprazo'], 5, 2) . '/' . substr($responsaveis[$esdid]['reaprazo'], 0, 4) : '';
								$readtsubsidio = $responsaveis[$esdid]['readtsubsidio'] ? substr($responsaveis[$esdid]['readtsubsidio'], 8, 2) . '/' . substr($responsaveis[$esdid]['readtsubsidio'], 5, 2) . '/' . substr($responsaveis[$esdid]['readtsubsidio'], 0, 4) : '';
								?>
								<td style="width: 30%;">
									<input name="responsavel[<?php echo $esdid ?>][reaid]" type="hidden" value="<?= $reaid; ?>">
									<?php
									if ($podeEditarResponsavel) {
										$opcoes = !empty($usuarios[$dados['pflcod']]) ? $usuarios[$dados['pflcod']] : array();
										echo $db->monta_combo("responsavel[{$esdid}][usucpf]", $opcoes, 'S', "Selecione", "", "", "", " ", "N", "", "", $usucpf, '', '', 'form-control chosen');
									} else {
										echo $usunome;
									}
									?>
								</td>
								<td style="width: 10%;"><?php echo $telefone; ?></td>
								<td style="width: 20%;"><?php echo $usuemail; ?></td>
								<td style="width: 10%; text-align: center;">
									<?php
									if (isset($responsaveis[ESD_DEMANDA_EM_CADASTRAMENTO]['reaprazo'])):
										?>
										<input id="reaprazo_cadastramento_prof" type="hidden" value="<?= formata_data($responsaveis[ESD_DEMANDA_EM_CADASTRAMENTO]['reaprazo']); ?>">
									<?php endif; ?>
									<?php
									if ($podeEditarData) {

										if ($dmdprazo) {
											$area = $dados['area'];
											echo '<a style="cursor: pointer" data-area="' . $area . '" data-reaid="' . $reaid . '" class="alterar-prazo-responsavel">
												<span style="margin-right: 10px;" class="glyphicon glyphicon-pencil" aria-hidden="true"></span></a>' . $dmdprazo;
										} else {
											echo '<input name="responsavel[' . $esdid . '][reaprazo]" type="text" class="form-control data" data-esdid="' . $esdid . '" value="' . $dmdprazo . '">';
										}
									} else {
										if (
											(in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)) ||
											(in_array(PFL_PROCURADOR_FEDERAL, $perfis)) ||
											(in_array(PFL_NUCLEO_JURIDICO, $perfis) && !in_array($esdid, array(ESD_DEMANDA_EM_CADASTRAMENTO))) ||
											(in_array(PFL_ADVOGADO, $perfis) && in_array($esdid, array(ESD_DEMANDA_ADVOGADO, ESD_DEMANDA_DTI_MEC, ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC, ESD_DEMANDA_4_NIVEL, ESD_DEMANDA_GESTOR_FIES))) ||
											(in_array(PFL_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_DTI_MEC, ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC, ESD_DEMANDA_4_NIVEL))) ||
											(in_array(PFL_ANALISTA_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC, ESD_DEMANDA_4_NIVEL))) ||
											(in_array(PFL_4_NIVEL, $perfis) && in_array($esdid, array(ESD_DEMANDA_4_NIVEL))) ||
											(in_array(PFL_GESTOR_FIES, $perfis))
										) {
											echo $dmdprazo;
										}
									}
									?>

								</td>

								<td style="width: 10%; text-align: center;">
									<?php if ($podeEditarData): ?>
										<input name="responsavel[<?php echo $esdid ?>][readtsubsidio]" type="text" class="form-control dataSub" value="<?php echo $readtsubsidio;
										?>">
									<?php else:
										if (
											(in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_ADMINISTRADOR, $perfis)) ||
											(in_array(PFL_PROCURADOR_FEDERAL, $perfis)) ||
											(in_array(PFL_NUCLEO_JURIDICO, $perfis) && !in_array($esdid, array(ESD_DEMANDA_EM_CADASTRAMENTO))) ||
											(in_array(PFL_ADVOGADO, $perfis) && in_array($esdid, array(ESD_DEMANDA_ADVOGADO, ESD_DEMANDA_DTI_MEC, ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC, ESD_DEMANDA_4_NIVEL, ESD_DEMANDA_GESTOR_FIES))) ||
											(in_array(PFL_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_DTI_MEC, ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC, ESD_DEMANDA_4_NIVEL))) ||
											(in_array(PFL_ANALISTA_DTI_MEC, $perfis) && in_array($esdid, array(ESD_DEMANDA_ANALISTA_DTI_MEC, ESD_DEMANDA_EXECUCAO_DTI_MEC, ESD_DEMANDA_4_NIVEL))) ||
											(in_array(PFL_4_NIVEL, $perfis) && in_array($esdid, array(ESD_DEMANDA_4_NIVEL))) ||
											(in_array(PFL_GESTOR_FIES, $perfis))
										):
											echo $dmdprazo;
										endif;
									endif;
									?>
								</td>
							</tr>
						<?php } ?>
						</tbody>
					</table>

					<div>
						<button title="Salvar" class="btn btn-success" type="submit"><span class="glyphicon glyphicon-thumbs-up"></span> Salvar</button>
					</div>
				</form>

			</div>

			<div class="row col-md-1">
				<?php wf_desenhaBarraNavegacao_demandasFIES($modelDemanda->dmdid, $modelDemanda->docid, array('dmdid' => $modelDemanda->dmdid), $estadoAtual['esdid']); ?>
			</div>
		</div>
	</div>

	<!-- Modal -->
	<div class="modal fade" id="visualizar_historico_justificativa" tabindex="-1" role="dialog" aria-labelledby="myModalLabel">
		<div class="modal-dialog modal-lg" role="document">
			<div class="modal-content">
				<div class="modal-header">
					<button type="button" class="close" data-dismiss="modal" aria-label="Close"><span aria-hidden="true">&times;</span></button>
					<h4 class="modal-title" id="myModalLabel">Lista de histórico de alteração de datas</h4>
				</div>
				<div class="modal-body div_conteudo_lista"></div>
			</div>
		</div>
	</div>
	<script type="text/javascript">

		$(document).ready(function () {

			$('.data, .dataSub').mask('99/99/9999');
			$('.dataSub').datepicker();

			$('.data').datepicker({
				onSelect: function (dateText, obj) {
					var dtInicio = $("#reaprazo_cadastramento_prof").val().split("/");
					var dtFim = dateText.split("/");
					data1 = new Date(dtInicio[2] + "/" + dtInicio[1] + "/" + dtInicio[0]);
					data2 = new Date(dtFim[2] + "/" + dtFim[1] + "/" + dtFim[0]);

					var total = dateDiferencaEmDias(data1, data2);
					var area_selecionada = $(this).data('esdid');

					<?php if(  in_array(PFL_SUPER_USUARIO, $perfis) || in_array(PFL_PROCURADOR_FEDERAL, $perfis) ){ ?>

					if (total > 0 && area_selecionada != <?= ESD_DEMANDA_EM_CADASTRAMENTO ?>) {
						alert('Data inserida é menor que a data anterior da Procuradoria Federal!');
						$(this).val('');
					}
					<?php }elseif ( in_array(PFL_DTI_MEC, $perfis) || in_array(PFL_ANALISTA_DTI_MEC, $perfis) ){ ?>
//					CONFORME E-MAIL DO GESTOR RAFAEL TAVARES
					<?php }else{ ?>
					if (total > 0) {
						alert('Data inserida é maior que a data da Procuradoria Federal!');
						$(this).val('');
					}
					<?php } ?>
				}
			});

			$('.alterar-prazo-responsavel').click(function () {
				var area = encodeURIComponent($(this).data('area'));
				var url = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&action=modal_alteracao_prazo&reaid=' + $(this).attr('data-reaid') + '&area=' + area;
				$('#conteudo_modal_prazo').load(url, function () {
					$('#modal-alterar-prazo').modal();
				});
			});

			$('.bt_historico_alteracao').click(function () {
				var dmdid = $("input[name='dmdid']").val();
				var esdid = $(this).data('esdid');

				var url = 'demandasfies.php?modulo=principal/demandasformulario&acao=A&action=listarAlteracoes&aba=responsaveis&dmdid=' + dmdid + '&esdid=' + esdid;
				$('.div_conteudo_lista').load(url, function () {
				});
			});
		});

		function dateDiferencaEmDias(a, b) {
			// Descartando timezone e horario de verao
			var utc1 = Date.UTC(a.getFullYear(), a.getMonth(), a.getDate());
			var utc2 = Date.UTC(b.getFullYear(), b.getMonth(), b.getDate());

			return Math.floor((utc2 - utc1) / ( 1000 * 60 * 60 * 24));
		}
	</script>