<?php
pegaSessoes();
$id_obs_nefetivado = $_REQUEST['id_obs_nefetivado'];
$titulo = "Cadastrar Observação";
monta_titulo( $titulo, '' );
?>
<html>
	<head>
		<title>Cadastrar Observação</title>
		<script language="JavaScript" src="./academico/geral/js/academico.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
		<body>
	<form method="POST"  name="form_obs">
		<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">			
			<tr>
			    <td align='right' class="SubTituloDireita">Descrição:</td>
			    <td><?=campo_textarea( 'lepobs','N', 'S', '', 60, 7, 500 );?></td>
			</tr>
			<tr bgcolor="#C0C0C0">
			<td align="left" colspan="2">
				<input type="button" id="salvar" value="Salvar" onclick="salvarobs();">
			</td>
			</tr>			
		</table>
	</form>
	<script language="JavaScript" src="../includes/wz_tooltip.js"></script>  
	</body>
</html>

<script type="text/javascript"><!--
				
	function salvarobs() {	
				
		var obs_cargo = window.opener.document.getElementById('<?=$id_obs_nefetivado?>');
		obs_cargo.value = document.getElementById("lepobs").value;
		obs_cargo.focus();
		window.close();		
	}
	
	document.getElementById("lepobs").value = window.opener.document.getElementById('<?=$id_obs_nefetivado?>').value;
	--></script>
