<?php
ini_set("memory_limit","25000M");
set_time_limit(0);

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/workflow.php";
include_once APPRAIZ . "www/par/autoload.php";

$db = new cls_banco();


$arrCpfs = Array('', '', '', '', '', '', '', '', '');

if( !in_array($_SESSION['usucpf'], $arrCpfs) ){
	echo "
		<script>
			alert('Acesso negado.');
			window.history.back();
		</script>";
}

function validaStrPreids( $preids ){
	$valida = str_replace('"', '', $preids);
	$valida = explode(',', $preids);
	$valida = $valida[0] != '' ? $valida : Array();
	$preids = Array();
	foreach( $valida as $val ){
		if( is_numeric(trim($val)) ){
			$preids[] = trim($val);
		}
	}
	return $preids;
}

function importaObras(){
	global $db;
	//$preids = validaStrPreids( $_REQUEST['preids'] );
	$preids = explode(',', $_REQUEST['preids']);
	echo "Início do Script<br>";
	if(is_array($preids)){
		foreach($preids as $preid){
			if( is_numeric( (int) $preid  )){	
				$sql = "SELECT TRUE FROM obras.preobra WHERE preid = $preid";
				$existe = $db->pegaUm($sql);
				if( $existe != 't' ){
					echo "<label style=\"color:red\">PRÉ-OBRA $preid NÃO EXISTE;</label> <br>";
					continue;
				}
				$sql = "SELECT TRUE FROM obras2.obras WHERE preid =".$preid." and obrstatus = 'A' ";
				$existe = $db->pegaUm($sql);
				if( $existe == 't' ){
					echo "<label style=\"color:red\">PRÉ-OBRA $preid JÁ EXISTE OBRA NO OBRAS 2;</label><br>";
					continue;
				}
		
				$preObra = new PreObra( $preid );
				$obrid = $preObra->importarPreobraParaObras2( $preid );
				$programa = $preObra->recuperaProgramaObra( $preid );
				if($obrid){
					echo "FOI INSERIDO NO MONITORAMENTO DE OBRAS A OBRA:$obrid DO TIPO $programa <br>";
				}else{
					$sqlErro = "INSERT INTO par.obrascomproblema (preid) VALUES ( ".$preid." )";
					$db->executar($sqlErro);
					echo "ERRO AO INSERIR A OBRA: $preid DO TIPO $programa <br>";
				}
			}else{
				echo "O PREID não é um número: {$preid}, <br>";
			}
		}
		$db->commit();
		echo "<br> Fim do Script";
		
	}else{
		echo "Não existe obras para ser importado.";
	}
	
}

if( $_REQUEST['req'] != '' ){
	ob_clean();
	$_REQUEST['req']();
	die();
}

?>
<link href="../../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">
<link rel="stylesheet" type="text/css" href="../../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../../includes/listagem.css'/>
		
<link rel="stylesheet" type="text/css" href="../../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
<link href="../../library/bootstrap-3.0.0/css/bootstrap.min-simec.css" rel="stylesheet" media="screen">

<script language="JavaScript" src="../../includes/funcoes.js"></script>
<script type="text/javascript" src="../../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<script type="text/javascript">
$(document).ready(function(){

	$('.importar').click(function(){
		$('#aguardando').show();
		var strPreids = $('#preids').val();
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: '&req=importaObras&preids='+strPreids+'',
	   		async: false,
	   		success: function(resp){
	   			$('#td_log_importacao').html(resp);
				$('#aguardando').hide();
	   		}
	 	});
	});
});
</script>
<center>
	<div id="aguardando" style="display:none; position: absolute; background-color: white; height:300%; width:100%; opacity:0.4; filter:alpha(opacity=40); " >
		<div style="margin-top:250px; align:center;">
			<img border="0" title="Aguardando" src="../../imagens/carregando.gif">
			Carregando...
		</div>
	</div>
</center>
<form name="form_importa_obra" id="form_importa_obra" method="post" action="" >
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" colspan="2"><b><center>Importar Pre-Obras para Obras e Obras2</center></b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita" width="20%"><b>Obras a serem importadas. (separadas por vírgulas):</b></td>
			<td>
				<?=campo_textarea('preids', 'N', 'S', 'Query', 200, 10, 5000)?><br><br>
				<label style="color:red"><b>Colocar os id's das obras separados por virgula para importa-las. (Ex.: 1234, 2345, 3456)</b></label>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" colspan="2"><b><center><input type="button" class="importar" value="Importar Obras"/></center></b></td>
		</tr>
		<tr>
			<td class="SubTituloDireita"><b>Log de importação</b></td>
			<td id="td_log_importacao"></td>
		</tr>
	</table>
</form>