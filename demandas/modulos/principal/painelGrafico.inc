<?php
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Painel de Gráficos", '&nbsp;');
?>

<script language="javascript" type="text/javascript" src="/includes/open_flash_chart/swfobject.js"></script>
<form id="form_grafico" name="form_grafico" method="post" >
	<?php 
		$_POST['funcao'] = !$_POST['funcao'] ? criaTabelaGraficos : $_POST['funcao'];
		call_user_func_array($_POST['funcao'],$_POST['parametros']);
	?>
</form>