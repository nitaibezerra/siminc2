<?php

ob_start();

global $db;

// --------------- Dependências
include_once '../../sase/classes/IndAvaliadorEducacional.class.inc';
// --
include APPRAIZ . 'includes/cabecalho.inc';

$iaediretoria = $_GET['iaediretoria'] != '' ? $_GET['iaediretoria'] : 1;

$indavaliadoreducacional = new IndAvaliadorEducacional();

switch ($_REQUEST['requisicao']){
	case 'salvar_indicacao':
		
		foreach ($_POST['estuf'] as $campo => $valor){
			$indavaliadoreducacional = new IndAvaliadorEducacional();
			$arAtributos = array(
					'iaeid' => $_POST['iaeid'][$campo] != '' ? $_POST['iaeid'][$campo] : null,
					'estuf' => $valor,
					'iaemunicipioatendido' =>   $_POST['iaemunicipioatendido'][$campo]   != '' ? $_POST['iaemunicipioatendido'][$campo] : NULL,
					'iaetecnicoprevisto' =>     $_POST['iaetecnicoprevisto'][$campo]     != '' ? $_POST['iaetecnicoprevisto'][$campo] : NULL,
					'iaetecnicoindicado' =>     $_POST['iaetecnicoindicado'][$campo]     != '' ? $_POST['iaetecnicoindicado'][$campo] : NULL,
                    'iaesupervisorprevisto' =>  $_POST['iaesupervisorprevisto'][$campo]  != '' ? $_POST['iaesupervisorprevisto'][$campo] : NULL,
					'iaesupervisorindicado' =>  $_POST['iaesupervisorindicado'][$campo]  != '' ? $_POST['iaesupervisorindicado'][$campo] : NULL,
					'iaeexecutivoprevisto' =>   $_POST['iaeexecutivoprevisto'][$campo]   != '' ? $_POST['iaeexecutivoprevisto'][$campo] : NULL,
					'iaeexecutivoindicado' =>   $_POST['iaeexecutivoindicado'][$campo]   != '' ? $_POST['iaeexecutivoindicado'][$campo] : NULL,
					'iaesupervgeralprevisto' => $_POST['iaesupervgeralprevisto'][$campo] != '' ? $_POST['iaesupervgeralprevisto'][$campo] : NULL,
					'iaesupervgeralindicado' => $_POST['iaesupervgeralindicado'][$campo] != '' ? $_POST['iaesupervgeralindicado'][$campo] : NULL,
					'iaestatus' => 'A',
                    'iaeetapa' => $_POST['eieid'],
                    'iaediretoria' => $_POST['iaediretoria']
			);
			$indavaliadoreducacional->iaeid = $_POST['iaeid'][$campo] != '' ? $_POST['iaeid'][$campo] : null;
			if ($_POST['eieid']){
				$indavaliadoreducacional->popularDadosObjeto($arAtributos);
				$indavaliadoreducacional->salvar(null, null, array());
				$indavaliadoreducacional->commit();
			}
		}
		
		break;
		
	case 'exportar_xls':
		ob_clean();
		header("Expires: Mon, 1 Apr 1974 05:00:00 GMT");
		header("Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT");
		header("Pragma: no-cache");
		header("Content-type: application/xls; name=simec_sase_indavaliadoreducacional_" . date("Ymdhis") . ".xls");
		header("Content-Disposition: attachment; filename=simec_sase_indavaliadoreducacional_" . date("Ymdhis") . ".xls");
		header("Content-Description: MID Gera excel");
		
		echo $indavaliadoreducacional->listar();
		exit();
		break;
}

// --------------- Cabeçalho
//$cabecalhoSistema[] = "Secretaria de Articulação com os Sistemas de Ensino";
//$cabecalhoSistema[] = "Diretoria de Cooperação e Planos de Educação";

$titulo = 'Indicação Avaliador Educacional';


?>
<link rel='StyleSheet' href="/sase/css/estilo.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="/includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" media='screen'/>
<link rel='StyleSheet' href="../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script src="/includes/funcoes.js"></script>
<script src="/includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>

<style type="text/css">
	.form-control {
		margin: auto !important;
	}
</style>

<script>
	function salvar(){
		jQuery('[name=requisicao]').val('salvar_indicacao');
		jQuery('[name=formCadastroLista]').submit();
	}

	function exportarXls(){
		jQuery('[name=requisicao]').val('exportar_xls');
		jQuery('[name=formCadastroLista]').submit();
	}

    function filtraTabela(){
        jQuery('[name=requisicao]').val('');
        jQuery('[name=formCadastroLista]').submit();
    }
</script>

<div id="container">
	<div class="col-lg-12">
		<div class="row">
			<div class="col-md-12">
			
				<form id="formCadastroLista" name="formCadastroLista" method="post" class="form-horizontal">
					<input type="hidden" id="requisicao" name="requisicao" value="" />
                    <input type="hidden" id="hideieid" name="hideieid" value=""/>
                    <input type="hidden" id="iaediretoria" name="iaediretoria" value="<?=$iaediretoria?>"/>
                    <div class="well" id="divEtapa">
                        <fieldset>
                            <legend>Etapa</legend>
                            <div class="text-left">
                                <?php echo $indavaliadoreducacional->getEtapas('filtraTabela()'); ?>
                            </div>
                        </fieldset>
                    </div>
					<div class="well" id="divLista">
						<fieldset>
							<legend><?=$titulo?></legend>
							<?php echo $indavaliadoreducacional->listar(true); ?>
							<div class="text-left">
								<button title="Salvar" class="btn btn-success" type="button" id="btnSalvar" onclick="salvar()"><span
									class="glyphicon glyphicon-thumbs-up"></span> Salvar
								</button>
								<button title="Exportar XLS" class="btn btn-primary" type="button" id="btnxls" onclick="exportarXls()">
									Exportar XLS
								</button>
							</div>
						</fieldset>
					</div>
				</form>
					
			</div>
		</div>
	</div>
</div>
