<?php
require_once APPRAIZ . "www/spo/_funcoes.php";
require_once APPRAIZ . "includes/funcoesspo.php";
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/acompanhamentosubacoes');

if (chaveTemValor($_REQUEST, 'requisicao'))
{
    switch ($_REQUEST['requisicao'])
    {
        case 'processarSnapshotSubacoes':
            processarSnapshotSubacoes($_REQUEST, $fm);
	break;
    }
}

require_once APPRAIZ . "includes/cabecalho.inc";
?>

<div class="col-lg-12">
	<div class="page-header">
		<ol class="breadcrumb">
			<li><a href="spo.php?modulo=inicio&acao=C"><?php echo $_SESSION['sisabrev']; ?></a></li>
			<li>Integração</li>
			<li>Processamento</li>
			<li class="active">Processar Dados Carga da LOA</li>
		</ol>
	</div>
	<?php echo $fm->getMensagens(); ?>
	<div class="well">
		<fieldset>
                    <div> Baixe com a data de <b>HOJE</b> os dados abaixo, para o exercício desejado.</div>
			<form id="frmProcessamento" name="frmProcessamento" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
				<input type="hidden" name="requisicao" value="processarSnapshotSubacoes"/>
				<div class="form-group">
					<label for="momento" class="col-lg-2 control-label">
						Período:
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
						
						inputCombo('momento', $sql, $_REQUEST['momento'], 'cols-qualit', $options);
					?>
					</div>
				</div>						
				<div class="form-group">
					<div class="col-lg-10 col-lg-offset-2">
						<button class="btn btn-warning" type="reset" onclick="window.location = window.location;">Limpar</button>
						<button class="btn btn-primary" id="btnVoltar" type="button" onclick="voltar();">Voltar</button>
						<button class="btn btn-success" id="inserir" type="submit"><i class="glyphicon glyphicon-upload"></i> Executar</button>
					</div>
				</div>
			</form>
		</fieldset>
	</div>
</div>