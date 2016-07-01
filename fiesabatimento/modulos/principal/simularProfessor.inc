<?

if( !in_array($_SESSION['usucpf'], $cpf_liberados) ){
	echo '<script>
 			location.href = "fiesabatimento.php?modulo=inicio&acao=C";
 		  </script>';
	die;
}


if($_POST['idocpf']){
	$cpf = str_replace("-", "",str_replace(".", "", $_POST['idocpf']));
	$_SESSION['fiesabatimento_var']['cpfusuario'] = $cpf;
	echo '<script>
			location.href = "fiesabatimento.php?modulo=principal/identificacaosolicitante&acao=A";
		  </script>';
	die;
}


//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";
// $db->cria_aba($abacod_tela,$url,$parametros);
monta_titulo('Simular Professor', $linha2);
?>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>

$(document).ready(function(){

	$('.simular').click(function(){
		$('#formListaSolicitacao').submit();
	});
	
});

</script>
<form method="post" name="formListaSolicitacao" id="formListaSolicitacao">
<input type="hidden" name="req" id="req" value="" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"	align="center">
	<tr>
		<td class="SubTituloDireita" width="25%"><b>Orientações :</b></td>
		<td>
		<p>
			Simular Professor.
		</p>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita">CPF :</td>
		<td><? echo campo_texto("idocpf", "N", "S", '', 14, 15, "###.###.###-##", "", "", "", 0, ""); ?></td>
	</tr>
	
	<tr>
		<td class="SubTituloDireita">&nbsp;</td>
		<td><input type="button" class="simular" value="Simular"></td>
	</tr>
</table>
</form>
