<?php 
include_once "_funcoes_formacao.php";
?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
	</head>
	<body>
		<form method="post" name="frmCursos" id="frmCursos">
		<?php detalheDocente( $_REQUEST ); ?>
		</form>
	</body>
</html>