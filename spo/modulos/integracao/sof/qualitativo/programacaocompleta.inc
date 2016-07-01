<?php
$servicos = array();

$map = array
(
	'Orgãos' => array('ws' => 'retornarOrgaos', 'dto' => 'orgaosDTO', 'tbl' => 'wssof.ws_orgaosdto'),
	'Programas' => array('ws' => 'retornarProgramas', 'dto' => 'programasDTO', 'tbl' => 'wssof.ws_programasdto'),
	'Indicadores' => array('ws' => 'retornarIndicadores', 'dto' => 'indicadoresDTO', 'tbl' => 'wssof.ws_indicadoresdto'),
	'Objetivos' => array('ws' => 'retornarObjetivos', 'dto' => 'objetivosDTO', 'tbl' => 'wssof.ws_objetivosdto'),
	'Iniciativas' => array('ws' => 'retornarIniciativas', 'dto' => 'iniciativasDTO', 'tbl' => 'wssof.ws_iniciativasdto'),
	'Ações' => array('ws' => 'retornarAcoes', 'dto' => 'acoesDTO', 'tbl' => 'wssof.ws_acoesdto'),
	'Localizadores' => array('ws' => 'retornarLocalizadores', 'dto' => 'localizadoresDTO', 'tbl' => 'wssof.ws_localizadoresdto'),
	'Metas' => array('ws' => 'retornarMetas', 'dto' => 'metasDTO', 'tbl' => 'wssof.ws_metasdto'),
	'Regionilizações' => array('ws' => 'retornarRegionalizacoes', 'dto' => 'regionalizacoesDTO', 'tbl' => 'wssof.ws_regionalizacoesdto'),
	'Planos Orçamentarios' => array('ws' => 'retornarPlanosOrcamentarios', 'dto' => 'planosOrcamentariosDTO', 'tbl' => 'wssof.ws_planosorcamentariosdto'),
	'Agendas Sam' => array('ws' => 'retornarAgendaSam', 'dto' => 'agendasSamDTO', 'tbl' => 'wssof.ws_agendasamdto'),
	'Medidas Institucionais Normativas' => array('ws' => 'retornarMedidasInstitucionaisNormativas', 'dto' => 'medidasInstitucionaisNormativasDTO', 'tbl' => 'wssof.ws_medidasinstitucionaisnormativasdto'),
);

ksort($map);

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/qualitativo/progamacaoCompleta');

// -- Processamento de requisições
if (chaveTemValor($_REQUEST, 'requisicao'))
{
	switch ($_REQUEST['requisicao'])
	{
		case 'carregarProgramacaoCompleta':
			$servicos = carregarProgramacaoCompleta($_POST, $map, $fm);
			break;
	}
}

$servicos = array_merge($servicos, ($_REQUEST['frmVinculacao']['servico']?$_REQUEST['frmVinculacao']['servico']:array()));

require_once APPRAIZ . 'includes/cabecalho.inc';
?>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<script type="text/javascript">$(document).ready(function(){ $('[data-toggle="tooltip"]').tooltip(); });</script>
<style>.red {color: red; font-weight:bold;}.bold {font-weight:bold; }</style>
<div class="col-lg-12">
    <?php
    $bc = new Simec_View_Breadcrumb();
    $bc->add('Integração')
        ->add('SOF')
        ->add('Qualitativo')
        ->add('Programação Completa')
        ->render();

    echo $fm->getMensagens();
    ?>
	<div class="well">
		<fieldset>
			<form id="frmVinculacao" name="frmVinculacao" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
				<input type="hidden" name="requisicao" value="carregarProgramacaoCompleta" />
				<div class="form-group">
					<div class="col-lg-10">
						<input type="hidden" name="execucao" id="execucao" value="cadastrar">
						<input type="hidden" name="vacid" id="vacid" value="<?= isset($resultSet['vacid']) ? $resultSet['vacid'] : null; ?>" />
						<input type="hidden" name="vaeid" id="vaeid" value="<?= isset($resultSet['vaeid']) ? $resultSet['vaeid'] : null; ?>" />
					</div>
				</div>
				<div class="form-group">
					<label for="servicos[]" class="col-lg-2 control-label">
						Programação completa:
					</label>
					<div class="col-lg-10">
						<?php foreach ($map as $name => $info) : ?>
						<input type="checkbox" value="<?php echo $name; ?>" <?php echo in_array($name, $servicos) ? 'checked' : null; ?> name="servicos[]" data-toggle="toggle" data-on="<span class='glyphicon glyphicon-ok'></span>" data-off="&nbsp;" data-size="mini"> <?php echo $name; ?><br>
						<?php endforeach; ?>
					</div>
				</div>
				<div class="form-group">
					<label for="momento" class="col-lg-2 control-label">
						Momento
					</label>
					<div class="col-lg-10">
					<?php
						$sql = "SELECT distinct
								codigomomento as codigo,
								codigomomento || ' - ' || descricao as descricao
							FROM wssof.ws_momentosdto m
							WHERE m.snativo = '1'";

						$options = array(
							'titulo' => 'Selecione ao menos um momento',
							'obrig' => 'S'
						);

						inputCombo('momento', $sql, $_REQUEST['frmVinculacao']['momento'], 'cols-qualit', $options);
					?>
					</div>
				</div>
				<div class="form-group">
					<label for="log" class="col-lg-2 control-label"></label>
					<div class="col-lg-10">
						<input type="checkbox" value="1" name="log" <?php echo $_REQUEST['log'] ? 'checked' : null; ?> data-toggle="toggle" data-on="<span class='glyphicon glyphicon-ok'></span>" data-off="&nbsp;" data-size="mini"> Registrar logs dos serviços?<br>
					</div>
				</div>
				<div class="form-group">
					<div class="col-lg-10 col-lg-offset-2">
						<button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
						<button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
						<button class="btn btn-success" id="inserir" type="submit"><i class="glyphicon glyphicon-upload"></i> Importar</button>
					</div>
				</div>
			</form>
		</fieldset>
	</div>
</div>