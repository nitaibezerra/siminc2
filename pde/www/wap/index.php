<?php
// carrega as funções gerais
include_once "config.inc";
include_once "../_constantes.php";
include ("../../../includes/funcoes.inc");
include ("../../../includes/classes_simec.inc");

// abre conexão com o servidor de banco de dados
//$db = new cls_banco();

?>
<!DOCTYPE html> 
<html> 
	<head> 
	<title><?php echo SIGLA_SISTEMA; ?> - Mobile</title> 
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="/includes/mobile-simec/SIMEC.min.css" />
	<link rel="stylesheet" href="http://code.jquery.com/mobile/1.3.0-rc.1/jquery.mobile.structure-1.3.0-rc.1.min.css" /> 
	<script src="http://code.jquery.com/jquery-1.8.3.min.js"></script> 
	<script src="http://code.jquery.com/mobile/1.3.0-rc.1/jquery.mobile-1.3.0-rc.1.min.js"></script> 
	</head>  
<body>
<?php 
	include APPRAIZ."includes/classes/Mobile.class.inc";
	include APPRAIZ."/pde/www/_funcoes_mobile.php";
//	$mobile = new Mobile();
?>
<form name="formulario_mobile" method="post">
<div data-role="page" data-theme="c">
	<div data-role="header" data-position="fixed" data-theme="c">
		<a data-transition="flip" href="#" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
		<h1><?php echo SIGLA_SISTEMA; ?> - Mobile</h1>
	</div>

	<ul data-role="listview" data-inset="true" >
	    <li data-role="list-divider" data-theme="e">Agendas</li>
	    	 <li><a data-theme="e" href="#" data-transition="slideup">Viver Sem Limite</a></li>
	    	 <li><a href="#" data-transition="slideup">Pronatec</a></li>
		   	 <li><a href="#" data-transition="slideup">Pronacampo</a></li>
	    <li data-role="list-divider" data-theme="a">Educação Básica</li>
	   		 <li><a href="Basico/basico_crechepre.php" data-transition="slideup">Creches e Pré-Escolas</a></li>
		   	 <li><a href="Basico/basico_quadras.php" data-transition="slideup">Quadras</a></li>
		   	 <li><a href="Basico/basico_caminho.php" data-transition="slideup">Caminho da Escola</a></li>       
		   	 <li><a href="Basico/basico_pacto.php" data-transition="slideup">Pacto pela Alfabetização na Idade Certa</a></li>       
		   	 <li><a href="Basico/basico_maiseducacao.php" data-transition="slideup">Mais Educação</a></li>       
	    <li data-role="list-divider" data-theme="b" data-transition="slideup">Educação Profissional</li>
			<li><a href="profissional/brasil_pro.php" data-transition="slideup">Brasil Profissionalizado</a></li>
			<li><a href="profissional/expansao_ept.php" data-transition="slideup">Expansão da Rede Federal de EPT</a></li>
	    <li data-role="list-divider" data-theme="d" data-transition="slideup">Educação Superior</li>
			<li><a href="superior/" data-transition="slideup">Expansão da Educação Superior</a></li>
	</ul>
</div>
</body>
</html>