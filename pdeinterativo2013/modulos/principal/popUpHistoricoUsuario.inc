<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
	</head>
	<body>
		<?php listaHistoricoUsuario($_GET['usucpf']) ?>
		<table class="listagem" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center" width="95%">
		<tr>
			<td class="SubTituloCentro">
				<input type="button" name="btn_fechar" value="Fechar" onclick="window.close()" >
			</td>
		</tr>
	</body>
</html>