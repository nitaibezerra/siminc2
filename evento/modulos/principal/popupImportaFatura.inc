<?php 
include  APPRAIZ."www/evento/geral/_funcoes_fatura_passagem.php";

if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

monta_titulo( 'Lista de Faturamentos de Passagens', '' );

?>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'>

<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_curso.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

$(document).ready(function() {

	$('#importar').click(function(){
		if( $('#codigo').val() != '' ){
			window.opener.location = 'evento.php?modulo=principal/cadFatura&acao=A&req=importarFatura&codigo='+$('#codigo').val();
			window.close();
		}
	});
});

</script>
<form method="post" name="formImportaFat" id="formImportaFat" action="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="30%">Código Fatura</td>
			<td>
				<?php 
					$codigo = $_POST['codigo']; 
					echo campo_texto( 'codigo', 'N', 'S', '', 16, 15, '', '','','','','id="codigo"' ); 
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloCentro" colspan="2" >
				<input type="button" id="importar" value="Importar" />
			</td>
		</tr>
	</table>
</form>
<div id="listaFaturamento" >
</div>