<?php

// inicializa sistema
require_once "config.inc";
include APPRAIZ . "includes/classes_simec.inc";
include APPRAIZ . "includes/funcoes.inc";
require_once('../webservice/painel/nusoap.php');

if($_REQUEST['enviarsms']) {
	
	$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);
	$err = $client->getError();
	if ($err) die('<h2>Constructor error</h2><pre>' . $err . '</pre>');
	
	if(strlen($_REQUEST['celular']) == 10) {
		$envio = $client->call('sendMessage', array('user' => 'inep', 'password' => 'tmmjee', 'testMode' => false, 'sender' => $_REQUEST['nome'], 'target' => '55'.$_REQUEST['celular'], 'body' => $_REQUEST['mensagem'], 'ID' => date("Ymdhis")));
	}
	
	die("<script>
			alert('Enviado com sucesso');
			window.location='sms.php';
	     </script>");
	
}
?>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

	
<html>
	<head>
		<meta name="description" content="SIMEC - Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação, Permite o Monitoramento Físico e Financeiro e a Avaliação das Ações e Programas do Ministério dentre outras atividades estratégicas">
		<meta name="keywords" content="SIMEC, MEC, PDE, Ministério da Educação, Analistas: ,Cristiano Cabral, Adonias Malosso, Gilberto Xavier">
		<META NAME="Author" CONTENT="Cristiano Cabral, cristiano.cabral@gmail.com">
		<meta name="audience" content="all">
		<meta http-equiv="Cache-Control" content="no-cache">
		<meta http-equiv="Pragma" content="no-cache">

		<meta http-equiv="Expires" content="-1">

	</head>
	
	<body>
	<script>
	function enviarSMS() {
	
		if(document.getElementById('nome').value == "") {
			alert('Nome obrigatório');
			return false;
		}
	
		if(document.getElementById('celular').value.length != 10) {
			alert('Celular obrigatório');
			return false;
		}
		
		if(document.getElementById('mensagem').value == "") {
			alert('Mensagem obrigatório');
			return false;
		}
		
		document.getElementById('formulario').submit();

	}
	</script>
	<form method="post" id="formulario">
	<input type="hidden" name="enviarsms" value="1">
	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2">Envio de SMS</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Nome:</td>
			<td><? echo campo_texto('nome', 'S', 'S', 'Nome', 13, 12, "", "", '', '', 0, 'id="nome"' ); ?></td>
		</tr>	
		<tr>
			<td class="SubTituloDireita">Celular:</td>
			<td><? echo campo_texto('celular', 'S', 'S', 'Celular', 13, 10, "##########", "", '', '', 0, 'id="celular"' ); ?> <font size=1>formato: ddnnnnnnnn | exemplo: 6155556666</font></td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Mensagem:</td>
			<td><? echo campo_textarea( 'mensagem', 'S', 'S', '', '70', '4', '150'); ?></td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2"><input type="button" name="enviarsms" value="Enviar SMS" onclick="enviarSMS();"></td>
		</tr>
	</table>
	</form>
	
	</body>
	
	
</html>
	