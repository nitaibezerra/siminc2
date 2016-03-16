<?php
header("Content-Type: text/html; charset=ISO-8859-1",true);

function listaCategorias(){
	
	global $db;
	
	$cabecalho = array("Código da Categoria", "Nome da Categoria", "Data de Inclusão da Categoria");

	$sql = "SELECT 
				sct.sctcodigo as codigo_categoria,
				sct.sctdsc as descricao_categoria,
				TO_CHAR(sct.sctdtinclusao, 'dd/mm/YYYY') as data_categoria
			FROM 
				par.sigarpcategoria sct
			ORDER BY
				sct.sctcodigo, sct.sctdsc";
	
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

function listaCategorias(){
	$.ajax({
		type: "POST",
		url: window.location,
	   	data: "req=listaCategorias",
	   	dataType: 'html',
	   	success: function(msg){
		   	$('#lista').html(msg);
		}
	 });
}

$(document).ready(function(){
	
	listaCategorias();
	
});


</script>
<form method="post" name="formGrupo" id="formGrupo">
	<input type="hidden" id="req" name="req" value="" />
	<input type="hidden" id="gicid" name="gicid" value="" />
</form>
<div id="lista">
</div>