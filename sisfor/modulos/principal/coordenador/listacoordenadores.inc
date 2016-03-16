<?php
if($_REQUEST['orcid']){ 
	$habilitado = "N";
	$custo = visualizarCustoAtividade($_REQUEST['orcid'],'html');
	extract($custo);
} else {
	$habilitado = "S";
}

if($_SESSION['sisfor']['iusd']){
	$cies = recuperarCoordenadorIES($_SESSION['sisfor']['iusd']); 
}
?>

<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script language="javascript" type="text/javascript" src="../includes/funcoes.js"></script>

<br>

<?php monta_titulo('Coordenadores de Cursos','Catálogo de Curso - 2014'); ?>

<?php listarCoordenadores(); ?>
