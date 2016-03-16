<?php 

if( !$_SESSION['pdeinterativo_vars']['pdeid'] ){
	echo "<script>alert('Erro de sessão.');window.close();</script>";
}

?>
<html>
	<head>
		<title>SIMEC - Sistema Integrado de Monitoramento Execução e Controle do Ministério da Educação</title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
	</head>
	<body>
		<?php $_GET['requisicao']() ?>
	</body>
</html>