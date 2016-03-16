<?php
header("Content-Type: text/html; charset=ISO-8859-1",true);

function listaItens(){
	
	global $db;
	
	$cabecalho = array("Código do Item","Nome do Item","Data de Inclusão do Item","Código da Categoria", "Nome da Categoria", "Data de Inclusão da Categoria");

	$sql = "SELECT 
				sit.sitcodigo as codigo, 
				sit.sitdsc as descricao, 
				TO_CHAR(sit.sitdtinclusao, 'dd/mm/YYYY') as data,
				sct.sctcodigo as codigo_categoria,
				sct.sctdsc as descricao_categoria,
				TO_CHAR(sct.sctdtinclusao, 'dd/mm/YYYY') as data_categoria
			FROM 
				par.sigarpitem sit
			INNER JOIN par.sigarpcategoria sct ON sct.sctid = sit.sctid
			ORDER BY
				sit.sitcodigo, sit.sitdsc";
	
	return $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formprocesso');
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']();
	exit;
}

require_once APPRAIZ . 'includes/cabecalho.inc';

echo '<br/>';
monta_titulo( $titulo_modulo, '' );
echo '<br/>';
$db->cria_aba($abacod_tela,$url,$parametros);
?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function listaItens(){
	$.ajax({
		type: "POST",
		url: window.location,
	   	data: "req=listaItens",
	   	dataType: 'html',
	   	success: function(msg){
		   	$('#lista').html(msg);
		}
	 });
}

$(document).ready(function(){
	
	listaItens();
	
});


</script>
<form method="post" name="formGrupo" id="formGrupo">
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="gicid" name="gicid" value="" />
</form>
<div id="lista">
</div>