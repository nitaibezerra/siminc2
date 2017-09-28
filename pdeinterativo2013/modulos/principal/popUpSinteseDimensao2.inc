<?php 

if( !$_SESSION['pdeinterativo2013_vars']['pdeid'] ){
	echo "<script>alert('Erro de sessão.');window.close();</script>";
}

?>
<html>
	<head>
		<title><?php echo NOME_SISTEMA; ?></title>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
		<script language="JavaScript" src="../../includes/funcoes.js"></script>
		<script type="text/javascript" src="/includes/JQuery/jquery.js"></script>
	</head>
	<body>
		<?php $_GET['requisicao']() ?>
	</body>
</html>