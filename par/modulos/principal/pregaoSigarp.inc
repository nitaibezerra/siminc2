<?php
header("Content-Type: text/html; charset=ISO-8859-1",true);

function listaItens(){
	
	global $db;
	
	$cabecalho = array("Ação", "Número Sequencial do Pregão","Número do Pregão","Data de Início do Pregão","Data Final do Pregão", "Data de Inclusão do Pregão");

	$sql = "SELECT
				'<img src=\"../imagens/mais.gif\"title=\"mais\" style=\"cursor:pointer;\" onclick=\"carregarDadosPregao(\''||sprid||'\',this);\">' as acao,
				sprnuseqpregao as sequencial,
				sprnupregao as pregao,
				TO_CHAR(sprdtinicio, 'dd/mm/YYYY') as dtinicio,
				TO_CHAR(sprdtfim, 'dd/mm/YYYY') as dtfim,
				TO_CHAR(sprdtinclusao, 'dd/mm/YYYY') as dtinclusao
			FROM
				par.sigarppregao
			ORDER BY
				sprnuseqpregao";
	
	return $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formpregao');
}

function carregarDadosPregao($dados){
	
	global $db;
	
	$cabecalho = array("Descrição do Item","UF","Valor");

	$sql = "SELECT
				sit.sitdsc as item,
				spd.spduf as uf,
				spd.spdvalor
			FROM
				par.sigarppregaodados spd
			INNER JOIN par.sigarpitem sit ON sit.sitid = spd.sitid
			WHERE
				spd.sprid = ".$dados['pregao'];
	
	return $db->monta_lista($sql,$cabecalho,100,5,'N','center','N','formdadospregao');
	
}

if( $_REQUEST['req'] ){
	$_REQUEST['req']($_REQUEST);
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

function carregarDadosPregao(pregao, obj) {

	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;

	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=7;
		ncol.innerHTML="Carregando...";
		$.ajax({
			type: "POST",
			url: window.location,
			data: "req=carregarDadosPregao&pregao="+pregao,
			async: false,
			success: function(msg){
			ncol.innerHTML="<div id='listatermoprocesso_" + pregao + "' >" + msg + "</div>";
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}

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