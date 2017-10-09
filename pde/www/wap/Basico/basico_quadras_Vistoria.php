<?php
// carrega as funções gerais
include_once "config.inc";
include_once "../../_constantes.php";
include ("../../../../includes/funcoes.inc");
include ("../../../../includes/classes_simec.inc");

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

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
	include APPRAIZ."/pde/www/_funcoes_cockpit.php";
//	$mobile = new Mobile();
?>
<style>
	.fundo_td_wap{background-color:#3B8550}
	.bold{font-weight:bold}
	.fundo_tr_wap{height:50px}
	.tabela_box{color:#FFFFFF;}
	.link{cursor:pointer}
</style>
<form name="formulario_mobile" method="post">
	<div data-theme="a" data-role="page">
		<div data-role="header" data-position="fixed">
			<div data-role="controlgroup" data-type="horizontal">
				<a data-transition="slidedown" href="../" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
				<a href="../Basico" data-role="button" data-icon="arrow-r" data-ajax="false">Educação Básica </a>
			</div>
			<h1>Educação Básica</h1>
			<div data-role="navbar">
				<ul>
					<li><a class="ui-btn-active" data-theme="a" href="basico_quadras.php"  data-transition="slide">Quadras </a></li>
				</ul>
			</div>
		</div>
		<div data-role="content">
			<ul data-role="listview" data-inset="true">
				<li><a data-transition="flip" href="basico_quadras_Vistoria.php">Vistorias</a></li>
				<tr>
					<td>
						<div>
							<img style="float:left" src="../../../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
							<div style="float:left" class="titulo_box" ><br><span class="subtitulo_box" >Situação quanto ao nível de preenchimento</span></div>
						</div>
						<?=montaTabelaVistoriaObras(2, 'N', 'N');?>
					</td>
				</tr>
				<li>
					<a data-transition="flip" href="basico_quadras_Situacao.php">Situação das Obras</a>
				</li>
			</ul>	
		</div>
	</div>
</form>
</body>
</html>


