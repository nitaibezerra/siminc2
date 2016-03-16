<?php 
include_once "_funcoes_formacao.php";
?>
<html>
	<head>
		<title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form method="post" name="frmCursos" id="frmCursos">
		<?php detalheDocente( $_REQUEST ); ?>
		</form>
	</body>
</html>