<?php
require_once APPRAIZ . "www/spo/_funcoes.php";
require_once APPRAIZ . 'includes/funcoesspo.php';
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . "spo/ws/sof/Quantitativo.php";

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/quantitativo/obterAcoesDisponiveisParaAcompanhamentoOrcamentario');

// -- Processamento de requisições
if (chaveTemValor($_REQUEST, 'requisicao'))
{
    switch ($_REQUEST['requisicao'])
    {
        case 'obterAcoesAcompanhamentoOrcamentario':
          	obterAcoesAcompanhamentoOrcamentario($_POST, $fm);
          	break;
    }
}

require_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
?>
<link rel="stylesheet" href="/library/bootstrap-toggle/css/bootstrap-toggle.min.css">
<script src="/library/bootstrap-toggle/js/bootstrap-toggle.min.js"></script>
<div class="col-lg-12">
    <div class="page-header">
        <ol class="breadcrumb">
            <li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
            <li>Integração</li>
            <li>Sof Qualitativo</li>
            <li class="active">Obter Ações para Acompanhamento Orçamentario</li>
        </ol>
    </div>
    <?php echo $fm->getMensagens(); ?>
    <div class="well">
    	<fieldset>
            <form id="frmConsultar" name="frmConsultar" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
            	<input type="hidden" name="requisicao" value="obterAcoesAcompanhamentoOrcamentario" />
				<div class="form-group">
					<label for="periodo" class="col-lg-2 control-label">
						Período
					</label>
					<div class="col-lg-10">
					<?php
						$dados = array(
							array('codigo' => 1, 'descricao' => 'Janeiro - Junho'),
							array('codigo' => 2, 'descricao' => 'Julho - Dezembro'),
						);

						$options = array(
							'titulo' => 'Selecione ao menos um período',
							'obrig' => 'S'
						);

						inputCombo('periodo', $dados, $_REQUEST['periodo'], 'periodo', $options);
					?>
					</div>
				</div>
				<div class="form-group">
                    <label for="log" class="col-lg-2 control-label"></label>
                    <div class="col-lg-10">
                    	<input type="checkbox" value="1" name="log" <?php echo $_REQUEST['log'] ? 'checked' : null; ?> data-toggle="toggle" data-on="<span class='glyphicon glyphicon-ok'></span>" data-off="&nbsp;" data-size="mini"> Registrar log do serviço?<br>
                    </div>
                </div>
                <div class="form-group">
                    <div class="col-lg-10 col-lg-offset-0">
                    	<input type="hidden" name="importar" value="1" />
                        <button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
                        <button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
                        <button class="btn btn-success" id="btnImportar" type="submit"><i class="glyphicon glyphicon-upload"></i> Importar</button>
                    </div>
                </div>
            </form>
        </fieldset>
    </div>
</div>