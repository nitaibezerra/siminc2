<?php 
$empid = ($_GET['empid'] ? $_GET['empid'] : $_SESSION['obras2']['empid']);
$_SESSION['obras2']['empid'] = $empid;

if ( $_GET['empid'] ){
	$_SESSION['obras2']['obrid'] = '';	
}

$obras = new Obras();
$arDadosObra = $obras->listaCombo( $empid );

if ( $_POST['obrid'] || count($arDadosObra) == 1 ){
	$_SESSION['obras2']['obrid'] = $_POST['obrid'] ? $_POST['obrid'] : $arDadosObra[0]['codigo'];
	die("<script>
			location.href = '?modulo=principal/cadObra&acao=V';
		 </script>");
}
$obrid = $_SESSION['obras2']['obrid'];
?>
<html>
<head>
	<title>Selecionar Objeto</title>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
	<link rel="stylesheet" type="text/css" href="../includes/listagem.css">
	<script type="text/javascript">
	<!--
	function selecionaObra( obrid ){
		if ( obrid ){
			$('#formulario').submit();
		}	
	}
	//-->
	</script>
</head>
<body topmargin="0" leftmargin="0">
<?php
if ( $obrid ){
	$db->cria_aba($abacod_tela,$url,$parametros);	
}
monta_titulo($titulo_modulo, '');
?>
<form method="post" id="formulario" name="formulario">
<table class="tabela" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">
	<tr>
		<td width="265" class="subtitulodireita">Objeto:</td>
		<td>
			<?php 
				$db->monta_combo('obrid', $arDadosObra, 'S', "Selecione...", 'selecionaObra', '', '', '', 'S', 'obrid'); 
			?>
		</td>
	</tr>
</table>
</form>
</body>
</html>