<?php

require_once APPRAIZ . 'www/spo/_funcoes.php';

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/qualitativo/tabelasApoio');

$servicos = array();

$map = array(
    'Esferas' => array('ws' => 'retornarEsferas', 'dto' => 'esferasDTO', 'tbl' => 'wssof.ws_esferasdto'),
    'Funções' => array('ws' => 'retonarFuncoes', 'dto' => 'funcoesDTO', 'tbl' => 'wssof.ws_funcoesdto'),
    'Momentos' => array('ws' => 'retornarMomentos', 'dto' => 'momentosDTO', 'tbl' => 'wssof.ws_momentosdto'),
    'Perfis' => array('ws' => 'retornarPerfis', 'dto' => 'perfisDTO', 'tbl' => 'wssof.ws_perfisdto'),
    'Produtos' => array('ws' => 'retornarProdutos', 'dto' => 'produtosDTO', 'tbl' => 'wssof.ws_produtosdto'),
    'Subfunções' => array('ws' => 'retornarSubFuncoes', 'dto' => 'subFuncoesDTO', 'tbl' => 'wssof.ws_subfuncoesdto'),
    'Regiões' => array('ws' => 'retornarRegioes', 'dto' => 'regioesDTO', 'tbl' => 'wssof.ws_regioesdto'),
    'Unidades de medida' => array('ws' => 'retornarUnidadesMedida', 'dto' => 'unidadesMedidaDTO', 'tbl' => 'wssof.ws_unidadesmedidadto'),
    'Tipos de ações' => array('ws' => 'retornarTiposAcao', 'dto' => 'tiposAcaoDTO', 'tbl' => 'wssof.ws_tiposacaodto'),
    'Tipos de inclusão' => array('ws' => 'retornarTiposInclusao', 'dto' => 'tiposInclusaoDTO', 'tbl' => 'wssof.ws_tiposinclusaodto'),
    'Tipos de programa' => array('ws' => 'retornarTiposPrograma', 'dto' => 'tiposProgramaDTO', 'tbl' => 'wssof.ws_tiposprogramadto'),
);

ksort($map);

// -- Processamento de requisições
if (chaveTemValor($_REQUEST, 'requisicao'))
{
	switch ($_REQUEST['requisicao'])
	{
		case 'carregarTabelasApoio':
			$servicos = carregarTabelasApoio($_POST, $map, $fm);
			break;
	}
}

require_once APPRAIZ . 'includes/cabecalho.inc';
?>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<script type="text/javascript">$(document).ready(function(){ $('[data-toggle="tooltip"]').tooltip(); });</script>
<style>.red {color: red; font-weight:bold;}.bold {font-weight:bold; }</style>
<div class="col-lg-12">
    <div class="page-header">
        <ol class="breadcrumb">
            <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li>Integração</li>
            <li>Sof Qualitativo</li>
            <li class="active">Tabelas de Apoio</li>
        </ol>
    </div>
    <?php echo $fm->getMensagens();
    $servicos += $_REQUEST['frmVinculacao']['servico']?$_REQUEST['frmVinculacao']['servico']:array();
    ?>
    <div class="well">
    	<fieldset>
            <form id="frmVinculacao" name="frmVinculacao" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
            	<input type="hidden" name="requisicao" value="carregarTabelasApoio" />
                <div class="form-group">
                    <div class="col-lg-10">
                        <input type="hidden" name="execucao" id="execucao" value="cadastrar">
                        <input type="hidden" name="vacid" id="vacid" value="<?= isset($resultSet['vacid']) ? $resultSet['vacid'] : null; ?>" />
                        <input type="hidden" name="vaeid" id="vaeid" value="<?= isset($resultSet['vaeid']) ? $resultSet['vaeid'] : null; ?>" />
                    </div>
                </div>
                <div class="form-group">
                    <label for="servicos[]" class="col-lg-2 control-label">
                        Tabelas de apoio:
                    </label>
                    <div class="col-lg-10">
                    	<?php foreach ($map as $name => $info) : ?>
                    	<input type="checkbox" value="<?php echo $name; ?>"
                               <?php echo in_array($name, $servicos) ? 'checked' : null; ?>
                               name="servicos[]" data-toggle="toggle" data-on="<span class='glyphicon glyphicon-ok'></span>"
                               data-off="&nbsp;" data-size="mini"> <?php echo $name; ?><br />
                    	<?php endforeach; ?>
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