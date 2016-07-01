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
monta_titulo( 'Fatura de Passagens', '' );

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript" src="geral/funcoes_curso.js"></script>

<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script type="text/javascript">

$(document).ready(function() {

});

</script>
<form method="post" name="formListaFat" id="formListaFat" action="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td>
			</td>
		</tr>
	</table>
</form>
<div id="listaPassagemSCDP" >
<?php if( $_REQUEST['fatid'] ){ listaPassagensFaturadas( $_REQUEST['fatid'] ); } ?>
</div>