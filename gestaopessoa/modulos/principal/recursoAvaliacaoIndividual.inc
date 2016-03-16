<?php
$fdpcpf = $_SESSION['cpfavaliado'];

if( $_POST['requisicao'] == 'salvar' ){
	extract($_POST);
	
	$ftrperiodoavalini = $ftrperiodoavalini ? "'".formata_data_sql($ftrperiodoavalini)."'" : 'null';
	$ftrperiodoavalfim = $ftrperiodoavalfim ? "'".formata_data_sql($ftrperiodoavalfim)."'" : 'null';
	
	if( $ftrid ){
		$sql = "UPDATE gestaopessoa.ftrecavaliacaoindividual SET 
				  fdpcpf = '$fdpcpf',
				  anoreferencia = '{$_SESSION['exercicio']}',
				  ftrargfundamentacao = '$ftrargfundamentacao',
				  ftrperiodoavalini = $ftrperiodoavalini,
				  ftrperiodoavalfim = $ftrperiodoavalfim
				WHERE 
					ftrid = $ftrid";
	} else {
		$sql = "INSERT INTO gestaopessoa.ftrecavaliacaoindividual(fdpcpf, anoreferencia, ftrargfundamentacao, ftrperiodoavalini, ftrperiodoavalfim) 
				VALUES ('$fdpcpf', '{$_SESSION['exercicio']}', '$ftrargfundamentacao', $ftrperiodoavalini, $ftrperiodoavalfim)";
	}
	$db->executar( $sql );
	
	if( $db->commit() ){
		$db->sucesso( 'principal/recursoAvaliacaoIndividual' );
	} else {
		echo "<script>
				alert('Falha na operação');
				window.location.href = window.location; 
			</script>";
	}
	exit();
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br>'; 
$db->cria_aba( $abacod_tela, $url, '' );

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
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form id="formulario" name="formulario" method="post" action="">
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="ftrid" id="ftrid" value="<?=$ftrid; ?>">
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
		<td><? echo campo_data2('ftrperiodoavalini', 'N', 'S','Data Inicio de Avaliação', '', '', '', $ftrperiodoavalini, '', '', 'ftrperiodoavalini'); ?> a
			<? echo campo_data2('ftrperiodoavalfim', 'N', 'S','Data Fim de Avaliação', '', '', '', $ftrperiodoavalfim, '', '', 'ftrperiodoavalfim'); ?></td>
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
		<td><? echo campo_textarea('ftrargfundamentacao', 'N', 'S', 'Argumentação/Fundamentação', 200, 10, 4000); ?></td>
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
<table align="center" border="0" width="95%" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td colspan="2" class="subtituloCentro">
			<input type="button" name="bt_gravar" value="Gravar Recurso" onclick="gravarRecurso();">
			<input type="button" name="bt_imprimir" value="Imprimir Recurso" onclick="imprimir();">
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function imprimir(){
	window.open("gestaopessoa.php?modulo=principal/imprimirRecursoAvaliacao&acao=A","Recurso Avaliação",'scrollbars=yes,height=500,width=900,status=no,toolbar=no,menubar=no,location=no');
}
function gravarRecurso(){

	if(!validaData( document.getElementById('ftrperiodoavalini') ) ) {
		alert('Data início está no formato incorreto.');
		document.getElementById('ftrperiodoavalini').focus();
		return false;
	}else if(!validaData( document.getElementById('ftrperiodoavalfim') ) ) {
		alert('Data fim está no formato incorreto.');
		document.getElementById('ftrperiodoavalfim').focus();
		return false;
	}else if( !validaDataMaior( document.getElementById('ftrperiodoavalini'), document.getElementById('ftrperiodoavalfim') ) ){
		alert("A data inicial não pode ser maior que data final.");
			document.getElementById('ftrperiodoavalini').focus();
		return false;
	}
	
	if( $('#ftrargfundamentacao').val() == '' ){
		alert('O campo "Argumentação/Fundamentação" é de preenchimento obrigatório!');
		$('#ftrargfundamentacao').focus();
		return false;
	}
	
	$('#requisicao').val('salvar');
	$('#formulario').submit();
}
</script>