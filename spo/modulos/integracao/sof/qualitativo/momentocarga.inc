<?php

require_once APPRAIZ . "includes/cabecalho.inc";
require_once APPRAIZ . 'includes/funcoesspo.php';
require_once APPRAIZ . 'includes/library/simec/Listagem.php';
require_once APPRAIZ . "includes/library/simec/Helper/FlashMessage.php";
require_once APPRAIZ . "spo/ws/sof/Qualitativo.php";

$fm = new Simec_Helper_FlashMessage('spo/integracao/sof/qualitativo/momentoCarga');

if ($_REQUEST['importar']) 
{
	header("Keep-Alive: timeout=9999, max=9999");
	set_time_limit(0);
	
	try
	{
		$ws = new Spo_Ws_Sof_Qualitativo(null);
		
		$mensagem = 'Registros importados com sucesso.';
		
		$count = 0;
		$informacoes = $map[$servico];
		$obterMomentoCarga = new ObterMomentoCarga();
		$obterMomentoCarga->exercicio = $_SESSION['exercicio'];
		$return = $ws->obterMomentoCarga($obterMomentoCarga)->return;
	
		if ($return->mensagensErro)
			throw new Exception($return->mensagensErro);
		
		$resultados = $return->momento;
		
		if ($resultados) 
		{
			$values = null;
			$dados = get_object_vars($resultados);
			$campos = strtolower(implode(', ', array_keys($dados)));
			$valores = array();
			
			foreach ($dados as $index => $value)
				$valores[] = addslashes($value);
			
			$insert = 'insert into wssof.ws_momentocargadto (' . $campos . ') values (\'' . implode("', '", $valores) . '\')';
			$delete = "delete from wssof.ws_momentocargadto";
			
			$db->executar($delete);
			$db->executar($insert);
		} 
		
		$db->commit();
		
		$fm->addMensagem($mensagem, Simec_Helper_FlashMessage::SUCESSO);
	}
	catch (Exception $e)
	{
		$mensagem = "Ocorreu um problema ao importar os registros<br>{$e->getMessage()}";
		
		$fm->addMensagem($mensagem, Simec_Helper_FlashMessage::ERRO);
	}
}
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
            <li class="active">Momento Carga</li>
        </ol>
    </div>
    <div class="well">
    	<fieldset>
            <form id="frmImportar" name="frmImportar" method="POST" class="form-horizontal" enctype="multipart/form-data" onsubmit="return true;">
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
    <?php echo $fm->getMensagens(); ?>
    <?php
    	$sql = "SELECT *, 
				to_char(dataultimaatualizacao,'DD/MM/YYYY HH24:MI:SS') as dataultimaatualizacao 
				FROM wssof.ws_momentocargadto ORDER BY 1";
            
		$listagem = new Simec_Listagem();
        $listagem->setQuery($sql);
        $listagem->turnOnPesquisator();
        $listagem->setCabecalho('auto');
        $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
	?>
</div>