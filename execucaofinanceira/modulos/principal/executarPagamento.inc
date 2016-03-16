<?php

ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

// ------------- DECLARAÇÃO DE VARIAVEIS ------------- //
$empid	 	= $_REQUEST['empid'] 	? $_REQUEST['empid'] 	: 0;
if( $_REQUEST['proid'] ){
	$processo 	= $_REQUEST['proid'];
	$_SESSION['execucaofinanceira']['proid'] = $_REQUEST['proid']; 
} else {
	$processo = $_SESSION['execucaofinanceira']['proid'];
}

if( empty($processo) ) die("<script> alert('Erro ao tentar identificar o processo!'); window.history.back(-1);</script>");

$obProcesso = new Processo();
$obProcesso->carregarPorId($processo);

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

$obPagamento = new Pagamento();

/*if($_REQUEST['prpid']) {
	$_SESSION['par_var']['prpid'] = $_REQUEST['prpid']; 
}
if($_REQUEST['empnumeroprocesso']) {
	$_SESSION['par_var']['empnumeroprocesso'] = $_REQUEST['empnumeroprocesso']; 
}
$processo = $_REQUEST['processo'];

if(!$_SESSION['par_var']['prpid']) die("<script>
											alert('Processo não encontrado');
											window.close();
										</script>");*/

?>
<html>
<head>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body>
<?php
monta_titulo( $titulo_modulo, '<p align=center><b>SELECIONE UM EMPENHO</b> para visualizar os dados do pagamento</p>' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<? $obProcesso->montaCabecalhoEmpenho(); ?>
<form id="formPagamento">
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td id="listapagamento">Carregando...</td>
	</tr>
	<tr>
		<td id="dadospagamento"></td>
	</tr>
</table>
</form>
<script type="text/javascript">

function carregarListaEmpenhoPagamento( empid ) {
	$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
			data: "requisicao=listaEmpenho&empid="+empid,
			async: false,
			success: function(msg){
				document.getElementById('listapagamento').innerHTML = msg;
			}
		}
	);
}

function verDadosPagamento(empid) {
	divCarregando();
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
		data: "requisicao=dadosPagamento&empid="+empid,
		async: false,
		success: function(msg){
		document.getElementById('dadospagamento').innerHTML = msg;
		divCarregado();
	}
	});
}

$(document).ready(function() {
	carregarListaEmpenhoPagamento();
});

messageObj = new DHTML_modalMessage();	// We only create one object of this class
messageObj.setShadowOffset(5);	// Large shadow
	
function displayMessage(url) {
	var today = new Date();
	messageObj.setSource(url+'&hash='+today);
	messageObj.setCssClassMessageBox(false);
	messageObj.setSize(690,400);
	messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
	messageObj.display();
}

function closeMessage() {
	messageObj.close();	
}

function verObrasPagamento(pagid)
{
	var today = new Date();
	displayMessage('par.php?modulo=principal/solicitacaoPagamento&acao=A&requisicao=obrasPagamento&pagid='+pagid+'&hash='+today);
}

function marcarSubacao(obj) {

	if(obj.checked) {
		obj.parentNode.parentNode.cells[6].childNodes[0].className='normal';
		obj.parentNode.parentNode.cells[6].childNodes[0].disabled=false;
		obj.parentNode.parentNode.cells[7].childNodes[0].className='normal';
		obj.parentNode.parentNode.cells[7].childNodes[0].disabled=false;
		
	} else {
		obj.parentNode.parentNode.cells[6].childNodes[0].className='disabled';
		obj.parentNode.parentNode.cells[6].childNodes[0].disabled=true;
		obj.parentNode.parentNode.cells[6].childNodes[0].value='';
		obj.parentNode.parentNode.cells[7].childNodes[0].className='disabled';
		obj.parentNode.parentNode.cells[7].childNodes[0].disabled=true;
		obj.parentNode.parentNode.cells[7].childNodes[0].value='';
	}
}

function cacularValorPagamento(obj) {
	var tabela = obj.parentNode.parentNode.parentNode;
	if(obj.id == 'valorpagamentoobra'){
		var valorDigitado = replaceAll(replaceAll(obj.value, ".", ""),",", ".");
		var valorPago     = parseFloat(replaceAll(tabela.rows[(tabela.rows.length)-2].cells[0].childNodes[0].value,"<br>",""));
		var totalObra	  = replaceAll(replaceAll(replaceAll(obj.parentNode.parentNode.cells[3].innerHTML,".",""),",","."),"<br>","");

		if( isNaN(parseFloat(valorPago)) ){
			var total 		  = valorDigitado;
		} else {
			var total 		  = valorDigitado+valorPago;
		}

		if(Number(total) > Number(totalObra)) {
			alert('"Valor pagamento" + "Valor pago" não pode ser maior que o "Total"');
			obj.value='';
			return false;
		}
		
		var porcentPagamento = (valorDigitado*100)/totalObra;
		obj.parentNode.parentNode.cells[6].childNodes[0].value = mascaraglobal('[.###],##',porcentPagamento.toFixed(2));
		
	} else {
		var porcentDigitado = parseFloat(replaceAll(obj.value, ",", "."));
		var porcentPago     = parseFloat(replaceAll(tabela.rows[(tabela.rows.length)-2].cells[0].childNodes[0].value,"<br>",""));
		var totalObra		= parseFloat(replaceAll(replaceAll(replaceAll(obj.parentNode.parentNode.cells[3].innerHTML,".",""),",","."),"<br>",""));
	
		if(porcentDigitado > (100-porcentPago)) {
			alert('"Valor pagamento" + "Valor pago" não pode ser maior que o "Total"');
			obj.value='';
			return false;
		}
		
		var valorPagamento = (porcentDigitado/100)*totalObra;
		obj.parentNode.parentNode.cells[7].childNodes[0].value = mascaraglobal('[.###],##',valorPagamento.toFixed(2));
	
	}
	
	var totalPagamento=0;
	var falta;
	var valorempenho = tabela.rows[(tabela.rows.length)-2].cells[2].childNodes[0].value; //quanto falta pra pagar.
	
	for(var i=0;i<(tabela.rows.length)-3;i++) {
		if(tabela.rows[i].cells[7].childNodes[0].value) {
			totalPagamento += parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[7].childNodes[0].value,".",""),",","."));
		}
	}

	if(totalPagamento > 0) {
		document.getElementById('solicitar').disabled=false;
		document.getElementById('valorpagamento').value = mascaraglobal('[.###],##',totalPagamento.toFixed(2));
		falta = valorempenho - ( replaceAll(replaceAll(document.getElementById('valorpagamento').value,".",""),",",".") );
		if( mascaraglobal('[###].##',parseFloat(valorempenho).toFixed(2)) < ( replaceAll(replaceAll(document.getElementById('valorpagamento').value,".",""),",",".") ) ){
			tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = '<span style="color:red;">valor empenhado<br>ultrapassado</span>';
			document.getElementById('solicitar').disabled=true;
		} else {
			tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = mascaraglobal('[.###],##',falta.toFixed(2));
		}
	} else {
		document.getElementById('solicitar').disabled=true;
		document.getElementById('valorpagamento').value = '';
		tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = mascaraglobal('[.###],##',parseFloat(valorempenho).toFixed(2));
	}

}

function solPag() {
	document.getElementById('div_auth').style.display='block';
}

function visPag(){
	var dados = $('#formPagamento').serialize();

	var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
	empid = empid.replace('=','');

	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
		data: "requisicao=solicitarPagamento&tipo=visualiza&"+dados,
		async: false,
		success: function(msg){
			document.getElementById('visXML').style.display='block';
			document.getElementById('visXMLDadosPagamento').innerHTML=msg;
	}
	});
}

function solicitarPagamento() {
	var form = document.getElementById('formPagamento');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha   = document.getElementById('wssenha').value;
	
	if(!usuario) {
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	
	divCarregando();

	var dados = $('#formPagamento').serialize();

	var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
	empid = empid.replace('=','');

	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarPagamento&"+dados,
		async: false,
		success: function(msg){
		alert(msg);
	}
	});
	
	if(empid) {
		carregarListaEmpenhoPagamento(empid);
	}
	carregarListaPagamentoEmpenho();
	
	document.getElementById('div_auth').style.display='none';
	document.getElementById('solicitar').disabled = false;
	divCarregado();
	
	
}


</script>