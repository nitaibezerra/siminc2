<script>
	//$('.ajaxCarregando').hide(); 
</script>

<?php

header('Content-type: text/html; charset=ISO-8859-1');

//Chamada de programa
require_once APPRAIZ . 'includes/workflow.php';
include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
include_once APPRAIZ . "demandasfies/classes/AcaoJudicial.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaObservacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaMateria.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaClassificacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/DemandaPartesAcao.inc.php";
include_once APPRAIZ . "demandasfies/classes/DemandaArquivo.class.inc";
include_once APPRAIZ . "demandasfies/classes/Objeto.class.inc";
include_once APPRAIZ . "demandasfies/classes/Tribunal.class.inc";
include_once APPRAIZ . "demandasfies/classes/Classificacao.class.inc";
include_once APPRAIZ . "demandasfies/classes/HistoricoIntervencao.class.inc";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
include_once APPRAIZ . "includes/classes/fileSimec.class.inc";

$modelDemanda = new Demanda($_REQUEST['dmdid']);
$modelDemandaPartesAcao = new DemandaPartesAcao($_REQUEST['dpaid']);
$modelDemandaObservacao = new DemandaObservacao($_REQUEST['dmoid']);
$modelDemandaArquivo = new DemandaArquivo($_REQUEST['dmaid']);
$modelDemanda->dmdcumprimento = trim($modelDemanda->dmdcumprimento);


$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
if ($modelDemanda->dmdid && !$modelDemanda->docid) {
	$modelDemanda->docid = pegarDocidDemanda($modelDemanda->dmdid);
	$estadoAtual = wf_pegarEstadoAtual($modelDemanda->docid);
}

$perfis = pegaPerfilGeral();
$perfis = $perfis ? $perfis : array();

$complementoDisabled = '';

?>
<div class="col-lg-12">
<div class="page-header">
	<h3 id="forms"><?php echo $modelDemanda->dmdid ? 'Demanda ' . $modelDemanda->dmdid : 'Nova Demanda'; ?></h3>
</div>

<?php if ($modelDemanda->acjid == AcaoJudicial::K_MANDADO_SEGURANCA || $modelDemanda->acjid == AcaoJudicial::K_ACAO_CIVIL_PUBLICA) { ?>
	<div class="row col-md-12">
		<div style="background-color: yellow; text-align: center; color: red;" class="alert alert-warning" role="alert">
			<strong>ATENÇÃO</strong>: <?php echo $modelDemanda->acjid == AcaoJudicial::K_MANDADO_SEGURANCA ? 'MANDADO DE SEGURANÇA' : 'AÇÃO CIVIL PÚBLICA (ACP)'; ?>
		</div>
	</div>
<?php } ?>


<div class="row">
<div class="row col-md-12">

	<div class="row">
		<div class="row col-md-12">


			<div class="well">
				<fieldset class="form-horizontal">
					<legend>Dados Gerais</legend>

					<?php if ($modelDemanda->dmdid) { ?>
						<div class="form-group">
							<label for="dmdid" class="col-lg-4 col-md-4 control-label">Código:</label>

							<div class="col-lg-8 col-md-8 " style="padding-top: 5px;">
								<span style="color: red;"><?php echo $modelDemanda->dmdid; ?></span>
							</div>
						</div>
					<?php } ?>

					<div class="form-group">
						<label for="dmdtipo" class="col-lg-4 col-md-4 control-label">Tipo:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control chosen" id="dmdtipo" name="dmdtipo" <?php echo complementoDisabled('dmdtipo', $podeEditar, $perfis); ?>>
								<?php echo $modelDemanda->getTipos(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="matid" class="col-lg-4 col-md-4 control-label">Matéria:</label>

						<div class="col-lg-8 col-md-8 ">
							<select data-placeholder="Selecione" class="form-control chosen campo-judicial" id="matid" name="matid[]" multiple <?php echo complementoDisabled('matid', $podeEditar, $perfis); ?>>
								<?php echo $modelDemanda->getMaterias(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="acjid" class="col-lg-4 col-md-4 control-label">Tipo de Ação:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control chosen campo-judicial" id="acjid" name="acjid" <?php echo complementoDisabled('acjid', $podeEditar, $perfis); ?>>
								<?php echo $modelDemanda->getTipoAcao(); ?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="objid" class="col-lg-4 col-md-4 control-label">Objeto:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control chosen campo-judicial" id="objid" name="objid" <?php echo complementoDisabled('objid', $podeEditar, $perfis); ?>>
								<?php
								echo $modelDemanda->getClassificacao();?>
							</select>
						</div>
					</div>

					<div class="form-group">
						<label for="claid" class="col-lg-4 col-md-4 control-label">Classificação:</label>

						<div class="col-lg-8 col-md-8 ">
							<select data-placeholder="Selecione" class="form-control chosen campo-judicial" id="claid" name="claid[]" multiple <?php echo complementoDisabled('claid', $podeEditar, $perfis); ?>>
								<?php
								echo $modelDemanda->getClassificacao('D');?>
							</select>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="dmdliminar" class="col-lg-4 col-md-4 control-label">Decisão:</label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($modelDemanda->dmdliminar == 't' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdliminar', $podeEditar, $perfis); ?>">
									<input class="campo-decisao" type="radio" name="dmdliminar" id="dmdliminar1" value="t" <?= ($modelDemanda->dmdliminar == 't' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdliminar', $podeEditar, $perfis); ?>> Sim
								</label> <label class="btn btn-default <?= ($modelDemanda->dmdliminar == 'f' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdliminar', $podeEditar, $perfis); ?>">
									<input class="campo-decisao" type="radio" name="dmdliminar" id="dmdliminar2" value="f" <?= ($modelDemanda->dmdliminar == 'f' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdliminar', $podeEditar, $perfis); ?>> Não
								</label>
							</div>
						</div>
					</div>

					<div class="form-group tipo-judicial tipo-decisao">
						<label for="claid" class="col-lg-4 col-md-4 control-label">Tipo Decisão:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control" id="decid" name="decid" <?php echo complementoDisabled('decid', $podeEditar, $perfis); ?>>
								<?php
								echo $modelDemanda->getDecisao();?>
							</select>
						</div>
					</div>

					<div class="form-group dmdstatusdecisao">
						<label for="dmdstatusdecisao" class="col-lg-4 col-md-4 control-label">Status da Decisão:</label>

						<div class="col-lg-8 col-md-8">
							<div class="btn-group " data-toggle="buttons">
								<label class="btn btn-default <?= ($modelDemanda->dmdstatusdecisao == 'DF' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao1" value="DF" <?= ($modelDemanda->dmdstatusdecisao == 'DF' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>> Deferida
								</label> <label class="btn btn-default <?= ($modelDemanda->dmdstatusdecisao == 'ID' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao2" value="ID" <?= ($modelDemanda->dmdstatusdecisao == 'ID' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>> Indeferida
								</label> <label class="btn btn-default <?= ($modelDemanda->dmdstatusdecisao == 'PD' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdstatusdecisao" id="dmdstatusdecisao3" value="PD" <?= ($modelDemanda->dmdstatusdecisao == 'PD' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdstatusdecisao', $podeEditar, $perfis); ?>> Parcialmente Deferida
								</label>
							</div>
						</div>
					</div>

					<div class="form-group tipo-judicial tipo-decisao">
						<label for="dmddecisaorecorrivel" class="col-lg-4 col-md-4 control-label">Recorrível:</label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($modelDemanda->dmddecisaorecorrivel == 't' ? 'active' : ''); ?> <?php echo complementoDisabled('dmddecisaorecorrivel', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmddecisaorecorrivel" id="dmddecisaorecorrivel1" value="t" <?= ($modelDemanda->dmddecisaorecorrivel == 't' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmddecisaorecorrivel', $podeEditar, $perfis); ?>> Sim
								</label> <label class="btn btn-default <?= ($modelDemanda->dmddecisaorecorrivel == 'f' ? 'active' : ''); ?> <?php echo complementoDisabled('dmddecisaorecorrivel', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmddecisaorecorrivel" id="dmddecisaorecorrivel2" value="f" <?= ($modelDemanda->dmddecisaorecorrivel == 'f' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmddecisaorecorrivel', $podeEditar, $perfis); ?>> Não
								</label>
							</div>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="dmdmulta" class="col-lg-4 col-md-4 control-label">Multa:</label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($modelDemanda->dmdmulta == 't' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdmulta', $podeEditar, $perfis); ?>">
									<input class="campo-multa" type="radio" name="dmdmulta" id="dmdmulta1" value="t" <?= ($modelDemanda->dmdmulta == 't' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdmulta', $podeEditar, $perfis); ?>> Sim
								</label> <label class="btn btn-default <?= ($modelDemanda->dmdmulta == 'f' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdmulta', $podeEditar, $perfis); ?>">
									<input class="campo-multa" type="radio" name="dmdmulta" id="dmdmulta2" value="f" <?= ($modelDemanda->dmdmulta == 'f' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdmulta', $podeEditar, $perfis); ?>> Não
								</label>
							</div>
						</div>
					</div>

					<div class="form-group tipo-judicial div-multa tipo-multa">
						<label for="dmdmultadsc" class="col-lg-4 col-md-4 control-label">Descrição Multa:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdmultadsc" name="dmdmultadsc" type="text"
								   class="form-control" placeholder=""
								   value="<?php echo $modelDemanda->dmdmultadsc; ?>" <?php echo $complementoDisabled; ?>  <?php echo complementoDisabled('dmdmultadsc', $podeEditar, $perfis); ?>>
						</div>
					</div>

					<?php if ($modelDemanda->testa_superuser() || in_array(PFL_ADMINISTRADOR, $perfis) || in_array(PFL_PROCURADOR_FEDERAL, $perfis)) { ?>
						<div class="form-group">
							<label for="dmdprazojudicial" class="col-lg-4 col-md-4 control-label">Prazo Judicial (em dias):</label>

							<div class="col-lg-8 col-md-8 ">
								<input id="dmdprazojudicial" name="dmdprazojudicial" type="text" class="form-control campo-judicial inteiro"
									   value="<?php echo $modelDemanda->dmdprazojudicial; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdprazojudicial', $podeEditar, $perfis); ?>>
							</div>
						</div>
					<?php } ?>

					<?php if ($modelDemanda->dmdprazojudicial && $modelDemanda->dmdprazojudicial <= 2) { ?>
						<div class="form-group">
							<label for="dmdprazotecnico" class="col-lg-4 col-md-4 control-label">Prazo Técnico:</label>

							<div class="col-lg-8 col-md-8 ">
								<?php $dmdprazotecnico = $modelDemanda->dmdprazotecnico ? substr($modelDemanda->dmdprazotecnico, 8, 2) . '/' . substr($modelDemanda->dmdprazotecnico, 5, 2) . '/' . substr($modelDemanda->dmdprazotecnico, 0, 4) : ''; ?>
								<input id="dmdprazotecnico" name="dmdprazotecnico" type="text" class="data form-control"
									   placeholder=""
									   value="<?php echo $dmdprazotecnico; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdprazotecnico', $podeEditar, $perfis); ?>>
							</div>
						</div>
					<?php } ?>

					<div class="form-group">
						<label for="dmdvalorcausa" class="col-lg-4 col-md-4 control-label">Valor da Causa:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdvalorcausa" name="dmdvalorcausa" type="text" class="form-control moeda  campo-judicial"
								   placeholder=""
								   value="<?php echo $modelDemanda->dmdvalorcausa; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdvalorcausa', $podeEditar, $perfis); ?>>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="solid" class="col-lg-4 col-md-4 control-label">Solicitação:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control chosen campo-judicial" id="solid" name="solid" <?php echo complementoDisabled('solid', $podeEditar, $perfis); ?>>
								<?php
								echo $modelDemanda->getSolicitacao();?>
							</select>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="dmdcompetencia" class="col-lg-4 col-md-4 control-label">Competência:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control chosen campo-judicial" id="dmdcompetencia" name="dmdcompetencia" <?php echo complementoDisabled('dmdcompetencia', $podeEditar, $perfis); ?>>
								<option value="">Selecione</option>
								<?php echo $modelDemanda->getCompetencias(); ?>
							</select>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="dmdjuizadoespecial" class="col-lg-4 col-md-4 control-label">Juizado Especial:</label>

						<div class="col-lg-8 col-md-8 ">
							<div class="btn-group" data-toggle="buttons">
								<label class="btn btn-default <?= ($modelDemanda->dmdjuizadoespecial == 't' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdjuizadoespecial', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdjuizadoespecial" id="dmdjuizadoespecial1" value="t" <?= ($modelDemanda->dmdjuizadoespecial == 't' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdjuizadoespecial', $podeEditar, $perfis); ?>> Sim
								</label> <label class="btn btn-default <?= ($modelDemanda->dmdjuizadoespecial == 'f' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdjuizadoespecial', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdjuizadoespecial" id="dmdjuizadoespecial2" value="f" <?= ($modelDemanda->dmdjuizadoespecial == 'f' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdjuizadoespecial', $podeEditar, $perfis); ?>> Não
								</label>
							</div>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="trbid" class="col-lg-4 col-md-4 control-label">Tribunal de Origem:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control campo-judicial" id="trbid" name="trbid" <?php echo complementoDisabled('trbid', $podeEditar, $perfis); ?>>
								<?php echo $modelDemanda->getTribunais($modelDemanda->dmdcompetencia); ?>
							</select>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="dmdvara" class="col-lg-4 col-md-4 control-label">Vara:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdvara" name="dmdvara" type="text"
								   class="form-control" placeholder=""
								   value="<?php echo $modelDemanda->dmdvara; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdvara', $podeEditar, $perfis); ?>>
						</div>
					</div>

					<div class="form-group">
						<label for="dmduf" class="col-lg-4 col-md-4 control-label">UF:</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control chosen" id="dmduf" name="dmduf" <?php echo complementoDisabled('dmduf', $podeEditar, $perfis); ?>>
								<?php echo $modelDemanda->getComboUfs(); ?>
							</select>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="dmdprocesso" class="col-lg-4 col-md-4 control-label">Nº Processo:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdprocesso" name="dmdprocesso" type="text" class="form-control campo-judicial"
								   placeholder=""
								   value="<?php echo $modelDemanda->dmdprocesso; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdprocesso', $podeEditar, $perfis); ?>>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdnumeroprocessoadm" class="col-lg-4 col-md-4 control-label">Nº Processo Administrativo:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdnumeroprocessoadm" name="dmdnumeroprocessoadm" type="text"
								   class="form-control" placeholder=""
								   value="<?php echo $modelDemanda->dmdnumeroprocessoadm; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdnumeroprocessoadm', $podeEditar, $perfis); ?>>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="dmdtransitojulgado" class="col-lg-4 col-md-4 control-label">Trânsito em Julgado:</label>

						<div class="col-lg-8 col-md-8">
							<div class="btn-group " data-toggle="buttons">
								<label class="btn btn-default <?= ($modelDemanda->dmdtransitojulgado == 't' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdtransitojulgado', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdtransitojulgado" id="dmdtransitojulgado1" value="t" <?= ($modelDemanda->dmdtransitojulgado == 't' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdtransitojulgado', $podeEditar, $perfis); ?>> Sim
								</label> <label class="btn btn-default <?= ($modelDemanda->dmdtransitojulgado == 'f' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdtransitojulgado', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdtransitojulgado" id="dmdtransitojulgado2" value="f" <?= ($modelDemanda->dmdtransitojulgado == 'f' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdtransitojulgado', $podeEditar, $perfis); ?>> Não
								</label>
							</div>
						</div>
					</div>

					<div class="form-group tipo-judicial">
						<label for="dmdfavoravel" class="col-lg-4 col-md-4 control-label">Status da Sentença:</label>

						<div class="col-lg-8 col-md-8">
							<div class="btn-group " data-toggle="buttons">
								<label class="btn btn-default <?= ($modelDemanda->dmdfavoravel == 'PR' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdfavoravel" id="dmdfavoravel1" value="PR" <?= ($modelDemanda->dmdfavoravel == 'PR' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>> Procedente
								</label> <label class="btn btn-default <?= ($modelDemanda->dmdfavoravel == 'IP' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdfavoravel" id="dmdfavoravel2" value="IP" <?= ($modelDemanda->dmdfavoravel == 'IP' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>> Improcedente
								</label> <label class="btn btn-default <?= ($modelDemanda->dmdfavoravel == 'PP' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdfavoravel" id="dmdfavoravel3" value="PP" <?= ($modelDemanda->dmdfavoravel == 'PP' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdfavoravel', $podeEditar, $perfis); ?>> Parcialmente Procedente
								</label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdcumprimento" class="col-lg-4 col-md-4 control-label">Cumprimento:</label>

						<div class="col-lg-8 col-md-8">
							<div class="btn-group " data-toggle="buttons">
								<label class="btn btn-default btn-sm <?= ($modelDemanda->dmdcumprimento == 'DC' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdcumprimento" id="dmdcumprimento1" value="DC" <?= ($modelDemanda->dmdcumprimento == 'DC' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar,
										$perfis); ?>> Decisão Cumprida
								</label> <label class="btn btn-default btn-sm <?= ($modelDemanda->dmdcumprimento == 'DCP' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdcumprimento" id="dmdcumprimento2" value="DCP" <?= ($modelDemanda->dmdcumprimento == 'DCP' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdcumprimento',
										$podeEditar, $perfis); ?>> Decisão Cumprida Parcialmente
								</label> <label class="btn btn-default btn-sm <?= ($modelDemanda->dmdcumprimento == 'DNC' ? 'active' : ''); ?> <?php echo complementoDisabled('dmdcumprimento', $podeEditar, $perfis); ?>">
									<input type="radio" name="dmdcumprimento" id="dmdcumprimento3" value="DNC" <?= ($modelDemanda->dmdcumprimento == 'DNC' ? 'checked="checked"' : ''); ?> <?php echo complementoDisabled('dmdcumprimento',
										$podeEditar, $perfis); ?>> Decisão Não Cumprida
								</label>
							</div>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdchaveacesso" class="col-lg-4 col-md-4 control-label">Chave de Acesso:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdchaveacesso" name="dmdchaveacesso" type="text"
								   class="form-control" placeholder=""
								   value="<?php echo $modelDemanda->dmdchaveacesso; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdchaveacesso', $podeEditar, $perfis); ?>>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdcnpjies" class="col-lg-4 col-md-4 control-label">CNPJ Instituição Ensino:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdcnpjies" name="dmdcnpjies" type="text"
								   class="form-control cnpj cnpj_buscar" placeholder=""
								   value="<?php echo $modelDemanda->dmdcnpjies; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdcnpjies', $podeEditar, $perfis); ?>> <span id="span_dmdcnpjies" class="help-block"></span>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdcnpjmantedora" class="col-lg-4 col-md-4 control-label">CNPJ Mantenedora:</label>

						<div class="col-lg-8 col-md-8 ">
							<input id="dmdcnpjmantedora" name="dmdcnpjmantedora" type="text"
								   class="form-control cnpj cnpj_buscar" placeholder=""
								   value="<?php echo $modelDemanda->dmdcnpjmantedora; ?>" <?php echo $complementoDisabled; ?> <?php echo complementoDisabled('dmdcnpjmantedora', $podeEditar, $perfis); ?>>
							<span id="span_dmdcnpjmantedora" class="help-block"></span>
						</div>
					</div>

					<div class="form-group">
						<label for="dmdtipo" class="col-lg-4 col-md-4 control-label">Classificação (DTI/MEC):</label>

						<div class="col-lg-8 col-md-8 ">
							<select class="form-control chosen" id="claiddti" name="claiddti" <?php echo complementoDisabled('claiddti', $podeEditar, $perfis); ?>>
								<?php echo $modelDemanda->getClassificacao('T'); ?>
							</select>
						</div>
					</div>

				</fieldset>
				<div>

				</div>
			</div>
		</div>

		<div class="row col-md-12">

			<div id="div_listagem_autor" class="tipo-judicial">
				<?php if ($modelDemanda->dmdid) {
					//$modelDemandaPartesAcao->dmdid = $modelDemanda->dmdid;
					//$modelDemandaPartesAcao->formularioAutor($podeEditar);

					?>
					<div class="well">

						<fieldset class="form-horizontal">
							<legend>Autor(es)</legend>
						</fieldset>

						<div style="margin-top: 5px; background: #ffffff !important;">
							<?php
							$listagem = new Simec_Listagem();
							$listagem->setCabecalho(array('Autor', 'CPF / CNPJ'));
							$listagem->esconderColunas(array('usucpfinclusao', 'dpaid'));


							$listagem->setQuery($modelDemandaPartesAcao->getSqlPessoa(DemandaPartesAcao::TIPO_PESSOA_AUTOR, $modelDemanda->dmdid));
							$listagem->addCallbackDeCampo(array('dpacpf'), 'adicionarMascaraCpfCnpj');
							$listagem->render();
							?>
						</div>
					</div>
				<? } ?>
			</div>

			<div id="div_listagem_reu" class="tipo-judicial">
				<?php if ($modelDemanda->dmdid) {
					//$modelDemandaPartesAcao->dmdid = $modelDemanda->dmdid;
					//$modelDemandaPartesAcao->formularioReu($podeEditar);
					?>
					<div class="well">

						<fieldset class="form-horizontal">
							<legend>Réu(s)</legend>
						</fieldset>

						<div style="margin-top: 5px; background: #ffffff !important;">
							<?php
							$listagem = new Simec_Listagem();
							$listagem->setCabecalho(array('Réu', 'CPF / CNPJ'));
							$listagem->esconderColunas(array('usucpfinclusao', 'dpaid'));
							$listagem->setQuery($modelDemandaPartesAcao->getSqlPessoa(DemandaPartesAcao::TIPO_PESSOA_REU, $modelDemanda->dmdid));
							$listagem->addCallbackDeCampo(array('dpacpf'), 'adicionarMascaraCpfCnpj');
							$listagem->render();
							?>
						</div>
					</div>
				<? } ?>
			</div>

			<div id="div_listagem_observacao">
				<?php if ($modelDemanda->dmdid) {
					//$modelDemandaObservacao->dmdid = $modelDemanda->dmdid;
					//$modelDemandaObservacao->formularioObservacoes($complementoDisabled);
					?>
					<div class="well">

						<fieldset class="form-horizontal">
							<legend>Observação(ções)</legend>
						</fieldset>

						<div style="margin-top: 5px; background: #ffffff !important;">
							<?php

							$sqlob = "select dmotexto, usunome
			                from demandasfies.demandaobservacao dob
			                    INNER join seguranca.usuario u on dob.usucpfinclusao = u.usucpf
			                where dmdid = {$modelDemanda->dmdid}
			                and dmostatus = 'A'
			                and dmotipo = 'GE'
			                order by dmoid desc";

							$listagem = new Simec_Listagem();
							$listagem->setCabecalho(array('Observação', 'Responsável'));
							$listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');
							$listagem->setQuery($sqlob);
							$listagem->render();
							?>
						</div>
					</div>
				<? } ?>
			</div>

			<div id="div_listagem_arquivo">
				<?php if ($modelDemanda->dmdid) {
					//$modelDemandaArquivo->dmdid = $modelDemanda->dmdid;
					//$modelDemandaArquivo->formularioArquivos($complementoDisabled);
					?>
					<div class="well">

						<fieldset class="form-horizontal">
							<legend>Arquivo(s)</legend>
						</fieldset>

						<div style="margin-top: 5px; background: #ffffff !important;">
							<?php

							$sqlaq = "select dmadsc from demandasfies.demandaarquivo where dmdid = {$modelDemanda->dmdid} and dmastatus = 'A' and dmatipo = 'GE'  order by dmaid desc";

							$listagem = new Simec_Listagem();
							$listagem->setCabecalho(array('Arquivo'));
							$listagem->setQuery($sqlaq);
							$listagem->render();
							?>
						</div>
					</div>
				<? } ?>
			</div>

		</div>
	</div>
</div>


<?php if ($modelDemanda->dmdid) { ?>

	<div class="col-md-12">
		<div class="row">
			<div class="col-md-12">
				<div class="well">
					<fieldset>
						<legend>Sínteses</legend>
						<?php //$modelDemandaObservacao->ultimaSintese(DemandaObservacao::K_TIPO_SINTESE_AUTOR, 'Síntese das alegações do autor'); ?>
						<?php //$modelDemandaObservacao->ultimaSintese(DemandaObservacao::K_TIPO_DETERMINACAO_JUDICIAL, 'Síntese da determinação judicial'); ?>
						<?php //$modelDemandaObservacao->ultimaSintese(DemandaObservacao::K_TIPO_QUESTIONAMENTOS_DIRIMIDOS, 'Questionamentos a serem dirimidos'); ?>
						<?php //$modelDemandaObservacao->ultimaSintese(DemandaObservacao::K_TIPO_QUESTIONAMENTOS_AREAS, 'Questionamentos das Áreas'); ?>

						<?
						$sql = "select dmotexto, to_char(dmo.dmodtinclusao, 'DD/MM/YYYY HH:24:mi:ss') as dtini,  usunome
				                from demandasfies.demandaobservacao dmo
				                    INNER join seguranca.usuario u on dmo.usucpfinclusao = u.usucpf
				                    LEFT  join workflow.estadodocumento ed on ed.esdid = dmo.esdid
				                where dmostatus = 'A'
				                and dmdid = {$modelDemanda->dmdid}
				                and dmotipo = 'SA'
				                order by dmo.dmodtinclusao desc";
						?>
						<p style="color: red;"><b><?php echo strtoupper('Síntese das alegações do autor'); ?></b></p>
						<?
						$listagem = new Simec_Listagem();
						$listagem->setCabecalho(array('Texto', 'Dt. Inclusão', 'Responsável'));
						$listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');
						$listagem->setQuery($sql);
						$listagem->render();
						?>

						<br>

						<?
						$sql = "select dmotexto, to_char(dmo.dmodtinclusao, 'DD/MM/YYYY HH:24:mi:ss') as dataini, usunome
				                from demandasfies.demandaobservacao dmo
				                    INNER join seguranca.usuario u on dmo.usucpfinclusao = u.usucpf
				                    LEFT  join workflow.estadodocumento ed on ed.esdid = dmo.esdid
				                where dmostatus = 'A'
				                and dmdid = {$modelDemanda->dmdid}
				                and dmotipo = 'DJ'
				                order by dmo.dmodtinclusao desc";
						?>
						<p style="color: red;"><b><?php echo strtoupper('Síntese da determinação judicial'); ?></b></p>
						<?
						$listagem = new Simec_Listagem();
						$listagem->setCabecalho(array('Texto', 'Dt. Inclusão', 'Responsável'));
						$listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');
						$listagem->setQuery($sql);
						$listagem->render();
						?>

						<br>

						<?
						$sql = "select dmotexto, to_char(dmo.dmodtinclusao, 'DD/MM/YYYY HH:24:mi:ss') as dataini, usunome
				                from demandasfies.demandaobservacao dmo
				                    INNER join seguranca.usuario u on dmo.usucpfinclusao = u.usucpf
				                    LEFT  join workflow.estadodocumento ed on ed.esdid = dmo.esdid
				                where dmostatus = 'A'
				                and dmdid = {$modelDemanda->dmdid}
				                and dmotipo = 'QD'
				                order by dmo.dmodtinclusao desc";
						?>
						<p style="color: red;"><b><?php echo strtoupper('Questionamentos a serem dirimidos'); ?></b></p>
						<?
						$listagem = new Simec_Listagem();
						$listagem->setCabecalho(array('Texto', 'Dt. Inclusão', 'Responsável'));
						$listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');
						$listagem->setQuery($sql);
						$listagem->render();
						?>

						<br>

						<?
						$sql = "select dmotexto, to_char(dmo.dmodtinclusao, 'DD/MM/YYYY HH:24:mi:ss') as dataini, usunome
				                from demandasfies.demandaobservacao dmo
				                    INNER join seguranca.usuario u on dmo.usucpfinclusao = u.usucpf
				                    LEFT  join workflow.estadodocumento ed on ed.esdid = dmo.esdid
				                where dmostatus = 'A'
				                and dmdid = {$modelDemanda->dmdid}
				                and dmotipo = 'AR'
				                order by dmo.dmodtinclusao";
						?>
						<p style="color: red;"><b><?php echo strtoupper('Questionamentos das Áreas'); ?></b></p>
						<?
						$listagem = new Simec_Listagem();
						$listagem->setCabecalho(array('Texto', 'Dt. Inclusão', 'Responsável'));
						$listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');
						$listagem->setQuery($sql);
						$listagem->render();
						?>
					</fieldset>
				</div>
			</div>
		</div>
	</div>

	<div class="clearfix"></div>

	<div class="col-md-12">
		<div class="row">
			<div class="col-md-12">
				<div class="well">
					<fieldset>
						<legend>Entregas</legend>
						<?
						$esdidAprovada = ESD_ENTREGA_APROVADA;
						$esdidEmElaboracao = ESD_ENTREGA_EM_ELABORACAO;
						$esdidAguardando = ESD_ENTREGA_AGUARDANDO_APROVACAO;
						$esdidNucleoJuridico = ESD_DEMANDA_NUCLEO_JURIDICO;
						$esdidNucleoJuridico = ESD_DEMANDA_NUCLEO_JURIDICO;
						$usucpf = $_SESSION['usucpf'];
						$sisId = SIS_DEMANDASFIES;

						$perfis = pegaPerfilGeral();
						$perfisSql = implode(',', $perfis);
						$sql = "SELECT dmeid, dmedsc, to_char(de.dmedtinclusao, 'DD/MM/YYYY HH24:mi:ss') as dtini, usunome, ed.esddsc,  est_entrega.esddsc as estado
				                        FROM demandasfies.demandaentrega de
				                                INNER JOIN seguranca.usuario u on de.usucpfinclusao = u.usucpf
				                                INNER JOIN workflow.estadodocumento ed on ed.esdid = de.esdid
				                                INNER JOIN workflow.documento doc ON doc.docid = de.docid
				                                INNER JOIN workflow.estadodocumento est_entrega on est_entrega.esdid = doc.esdid
				                                INNER JOIN seguranca.perfilusuario pu on pu.usucpf = u.usucpf
				                                INNER JOIN seguranca.perfil p on p.pflcod = pu.pflcod  AND p.sisid =  {$sisId}
				                        WHERE  1 = 1
				                          AND (
				                                  est_entrega.esdid = {$esdidAprovada} OR -- Todas as aprovadas
				                                  ( est_entrega.esdid = {$esdidEmElaboracao} AND pu.pflcod in ($perfisSql) ) OR -- Em elaboração: somente para os mesmos perfis de quem incluiu
				                                  ( est_entrega.esdid = {$esdidAguardando} AND de.esdid = $esdidNucleoJuridico) -- Aguardando aprovação: somente Núcleo Jurídico
				                              )
				                          AND dmdid = {$modelDemanda->dmdid}
				                          AND dmestatus = 'A'
				                        ORDER BY de.dmedtinclusao desc";

						$dadosEntregas = $db->carregar($sql);
						//$listagem->setCabecalho(array('Descrição', 'Dt. Inclusão', 'Responsável', 'Área', 'Estado'));

						if ($dadosEntregas) {


							foreach ($dadosEntregas as $etr) {

								echo '<p style="color: blue;"><b>' . strtoupper($etr['dmedsc'] . ' - ' . $etr['dtini'] . ' - ' . $etr['usunome'] . ' - ' . $etr['esddsc'] . ' - ' . $etr['estado']) . '</b></p>';

								$sqlEtr = "select dmotexto, to_char(dmo.dmodtinclusao, 'DD/MM/YYYY HH:24:mi:ss') as dtini,  usunome
				                from demandasfies.demandaobservacao dmo
				                    INNER join seguranca.usuario u on dmo.usucpfinclusao = u.usucpf
				                    LEFT  join workflow.estadodocumento ed on ed.esdid = dmo.esdid
				                where dmostatus = 'A'
				                and dmdid = {$modelDemanda->dmdid}
				                and dmotipo = 'ET'
				                and dmeid = {$etr['dmeid']}
				                order by dmoid desc";

								$listagem = new Simec_Listagem();
								$listagem->setCabecalho(array('Texto', 'Dt. Inclusão', 'Responsável'));
								$listagem->addCallbackDeCampo(array('dmotexto', 'usunome'), 'alinhaParaEsquerda');
								$listagem->setQuery($sqlEtr);
								$listagem->render(true);

							}

						}


						?>
					</fieldset>
				</div>
			</div>
		</div>
	</div>


	<div class="clearfix"></div>

	<div class="col-md-12">
		<div class="row">
			<div class="col-md-12">
				<div class="well">
					<fieldset>
						<legend>Responsáveis</legend>
						<?

						$listaEstados = array(ESD_DEMANDA_EM_CADASTRAMENTO => array('pflcod' => PFL_PROCURADOR_FEDERAL, 'area' => 'Procuradoria Federal'), ESD_DEMANDA_NUCLEO_JURIDICO => array('pflcod' => PFL_NUCLEO_JURIDICO, 'area' => 'Núcleo Jurídico'), ESD_DEMANDA_ADVOGADO => array('pflcod' => PFL_ADVOGADO, 'area' => 'Advogado'), ESD_DEMANDA_DTI_MEC => array('pflcod' => PFL_DTI_MEC, 'area' => 'DTI/MEC'), ESD_DEMANDA_ANALISTA_DTI_MEC => array('pflcod' => PFL_ANALISTA_DTI_MEC, 'area' => 'Analista DTI/MEC'), ESD_DEMANDA_EXECUCAO_DTI_MEC => array('pflcod' => PFL_ANALISTA_DTI_MEC, 'area' => 'Execução DTI/MEC'), ESD_DEMANDA_4_NIVEL => array('pflcod' => PFL_4_NIVEL, 'area' => '4º Nível'), ESD_DEMANDA_GESTOR_FIES => array('pflcod' => PFL_GESTOR_FIES, 'area' => 'Gestor FIES'),);
						//ver($listaEstados, d);

						$sql = "select p.pflcod, p.pfldsc, u.usucpf, u.usunome, u.usuemail, '(' || u.usufoneddd || ') ' || u.usufonenum as telefone
							        from seguranca.perfilusuario pu
							                inner join seguranca.perfil p on p.pflcod = pu.pflcod
							                inner join seguranca.usuario u on u.usucpf = pu.usucpf
							        where sisid = 198";

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


						?>

						<table class="table table-bordered table-hover table-striped table-condensed">
							<thead>
							<tr>
								<th>Área</th>
								<th>Responsável</th>
								<th>Telefone</th>
								<th>Email</th>
							</tr>
							</thead>
							<tbody>
							<?php foreach ($listaEstados as $esdid => $dados) {
								?>
								<tr>
									<td style="width: 20%;"><?php echo $dados['area']; ?></td>
									<?php
									$usucpf = !empty($responsaveis[$esdid]['usucpf']) ? $responsaveis[$esdid]['usucpf'] : '';
									$usunome = !empty($responsaveis[$esdid]['usunome']) ? $responsaveis[$esdid]['usunome'] : '';
									$usuemail = !empty($responsaveis[$esdid]['usuemail']) ? $responsaveis[$esdid]['usuemail'] : '';
									$telefone = !empty($responsaveis[$esdid]['telefone']) ? $responsaveis[$esdid]['telefone'] : '';
									?>
									<td style="width: 30%;"><?php echo $usunome; ?></td>
									<td style="width: 10%;"><?php echo $telefone; ?></td>
									<td style="width: 20%;"><?php echo $usuemail; ?></td>
								</tr>
							<?php } ?>
							</tbody>
						</table>


					</fieldset>
				</div>
			</div>
		</div>
	</div>

	<div class="clearfix"></div>
<?php } ?>

</div>

</div>

<?
die;
?>


