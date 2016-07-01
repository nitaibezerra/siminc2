<?php
$fdpcpf = $_SESSION['cpfavaliado'];

monta_titulo( 'Formulário de Recurso de Avaliação Individual', '' );

$sql = "SELECT ftrid, fdpcpf, anoreferencia, ftrargfundamentacao, to_char(ftrperiodoavalini, 'DD/MM/YYYY') as ftrperiodoavalini, to_char(ftrperiodoavalfim, 'DD/MM/YYYY') as ftrperiodoavalfim 
FROM gestaopessoa.ftrecavaliacaoindividual WHERE fdpcpf = '$fdpcpf' AND anoreferencia = {$_SESSION['exercicio']}";
$arrDados = $db->pegaLinha( $sql );
$arrDados = $arrDados ? $arrDados : array();
extract($arrDados);

$sql = "SELECT ser.sercpf, ser.sernome, ser.sersiape, ser.sercargo, tls.tlssigla, ser.sercpfchefe
			FROM gestaopessoa.servidor ser
				left join gestaopessoa.tipolotacaoservidor tls on tls.tlsid = ser.tlsid and tls.tlsstatus = 'A'
		WHERE
			ser.sercpf = '$fdpcpf'
			AND ser.seranoreferencia = {$_SESSION['exercicio']}";
$arrServidor = $db->pegaLinha( $sql );
	
?>
<html>
	<head>
		<meta http-equiv="Cache-Control" content="no-cache">
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	    <link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	    <script type="text/javascript" src="../includes/funcoes.js"></script>
	    <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	</head>
	<style type="">
		@media print {.notprint { display: none } .div_rolagem{display: none} .div_rol{display: 'none'} }        
        
		@media screen {.notscreen { display: none; }.div_rol{display: none;}}
                       
		.div_rolagem{ overflow-x: auto; overflow-y: auto; height: 50px;}
		.div_rol{ overflow-x: auto; overflow-y: auto; height: 50px;}
                       
	</style>
	<body>
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<tbody>
	<tr>
		<td style="text-align: center;">
			<span style="font: bold; color: blue"><b>Ciclo Avaliativo: 01/11/2011 a 31/10/2012</b></span><br><br>
			Gratificação de Desempenho do Plano Geral de Cargos do Poder Executivo – GDPGPE<br>
			Decreto nº 7.133, de 19 de março de 2010<br><br><br>			
		</td>
	</tr>
	</tbody>
</table>

<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
	<tr>
		<td class="subtituloesquerda" width="30%">Matrícula SIAPE</td>
		<td class="subtituloesquerda" width="70%">Nome do(a) Servidor(a)</td>
	</tr>
	</thead>
	<tbody>
	<tr>
		<td width="30%"><?=$arrServidor['sersiape']; ?></td>
		<td width="70%"><?=$arrServidor['sernome']; ?></td>
	</tr>
	</tbody>
</table>

<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
	<tr>
		<td class="subtituloesquerda">Cargo Efetivo</td>
	</tr>
	</thead>
	<tbody>
	<tr>
		<td><?=$arrServidor['sercargo']; ?></td>
	</tr>
	</tbody>
</table>

<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
	<tr>
		<td class="subtituloesquerda">Unidade de Exercício</td>
	</tr>
	</thead>
	<tbody>
	<tr>
		<td><?=$arrServidor['tlssigla']; ?></td>
	</tr>
	</tbody>
</table>
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
	<tr>
		<td class="subtituloesquerda">Período de Avaliação</td>
	</tr>
	</thead>
	<tbody>
	<tr>
		<td><? echo $ftrperiodoavalini; ?> a
			<? echo $ftrperiodoavalfim; ?></td>
	</tr>
	</tbody>
</table>
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
	<tr>
		<td class="subtituloesquerda">Argumentação/Fundamentação</td>
	</tr>
	</thead>
	<tbody>
	<tr>
		<td valign="top"><? echo $ftrargfundamentacao; ?></td>
	</tr>
	</tbody>
</table>
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<thead>
	<tr>
		<td class="subtitulocentro" width="50%">Local e Data</td>
		<td class="subtitulocentro" width="50%">Assinatura do(a) Servidor(a)</td>
	</tr>
	</thead>
	<tbody>
	<tr style="text-align: center;">
		<td width="50%">_______________, ____ de ____________ de <?=date('Y'); ?>.</td>
		<td width="50%">_____________________________________________</td>
	</tr>
	</tbody>
</table>
<div id = "div_rolagem" class="notprint">
<table style="padding:15px; background-color:#e9e9e9; color:#404040;" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td colspan="2" class="subtituloCentro">
			<input type="button" name="bt_imprimir" value="Imprimir" onclick="javascript: window.print();">
			<input type="button" name="bt_fechar" value="Fechar" onclick="javascript: window.close();">
			</td>
	</tr>
</table>	
</div>
</body>
</html>