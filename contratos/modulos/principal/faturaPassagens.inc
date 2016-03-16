<?php
include  APPRAIZ."www/evento/geral/_funcoes_fatura_passagem.php";
//ver('foi',d);
if($_REQUEST['req']){
	$_REQUEST['req']($_REQUEST);
	die();
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( 'Lista de Faturamentos de Passagens', '' );

?>
<script	type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script	type="text/javascript" src="geral/funcoes_curso.js"></script>

<script	language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script	language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css"	type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

	jQuery.noConflict();
	
	jQuery(document).ready(function() {
	
		jQuery('#pesquisar').click(function(){
		
			jQuery.ajax({
				type: "POST",
				url: window.location,
				data: "req=listaFaturas&"+jQuery('#formListaFat').serialize(),
				async: false,
				success: function(msg){
					jQuery('#listaFaturamento').html(msg);
					jQuery('#pesquisar').attr('disabled',false);
				}
			});
		});
		
	
		jQuery('#limpar').click(function(){
			$('#fatnufatura').val('');
			$('#inicio').val('');
			$('#fim').val('');
		});
		
	
		jQuery('#importarFat').click(function(){
			window.open('evento.php?modulo=principal/popupImportaFatura&acao=A',
	                	'', 'height=150,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=no');
		});
	});

</script>

<form method="post" name="formListaFat" id="formListaFat" action="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		
		<tr>
			<td class="SubTituloDireita" width="30%">Código da Fatura</td>
			<td><?php 
			$fatnufatura = $_POST['fatnufatura'];
			echo campo_texto( 'fatnufatura', 'N', 'S', '', 16, 15, '[#]', '','','','','id="fatnufatura"' );
			?></td>
		</tr>
		
		<tr>
			<td class="SubTituloDireita" width="30%">Periodo de Cadastro</td>
			<td>De <?=campo_data2('inicio','S','S','','DD/MM/YYYY','','', $curinicio, '', '', 'inicio' ) ?>
			&nbsp;Até&nbsp; <?=campo_data2('fim','N','S','','DD/MM/YYYY','','', $curfim, '', '', 'fim' ) ?>
			</td>
		</tr>
		
		<tr>
			<td class="SubTituloCentro" colspan="2">
				<div style="float: left">
					<input type="button" id="importarFat" value="Importar Faturamento." />
				</div>
				<input type="button" id="pesquisar" value="Pesquisar" />
				<div style="float: right">
					<input type="button" id="limpar" value="Limpar campos da pesquisa." />
				</div>
			</td>
		</tr>
		
	</table>
</form>
<div id="listaFaturamento"></div>
