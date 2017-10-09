<?php
// carrega as funções gerais
include_once "config.inc";
include_once "_constantes.php";
include ("../../includes/funcoes.inc");
include ("../../includes/classes_simec.inc");

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

?>
<!DOCTYPE html> 
<html> 
	<head> 
	<title><?php echo SIGLA_SISTEMA; ?> - Mobile</title> 
	<meta name="viewport" content="width=device-width, initial-scale=1"> 
	<link rel="stylesheet" href="/includes/jquery.mobile-1.0.1/jquery.mobile-1.0.1.min.css" />
	<script src="/includes/jquery.mobile-1.0.1/jquery-1.7.1.min.js"></script>	
	<script src="/includes/jquery.mobile-1.0.1/jquery.mobile-1.0.1.min.js"></script>
	<link type="text/css" href="/includes/jquery.mobile-1.0.1/jquery.mobile.datebox.min.css" rel="stylesheet" />
	<script type="text/javascript" src="/includes/jquery.mobile-1.0.1/jquery.mousewheel.min.js"></script>
	<script type="text/javascript" src="/includes/jquery.mobile-1.0.1/jquery.mobile.datebox.min.js"></script>
	
	</script>
</head> 
<body>
<?php 
	include APPRAIZ."includes/classes/Mobile.class.inc";
	include APPRAIZ."/pde/www/_funcoes_mobile.php";
	$mobile = new Mobile();
?>
<form name="formulario_mobile" method="post">
<div data-role="page">
	<div data-role="header" data-position="fixed">
		<div data-role="controlgroup" data-type="horizontal">
			<a data-transition="flip" href="estrategico.php?modulo=principal/mobile_estrategico&acao=A" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
			<a href="#" data-role="button" data-icon="arrow-r" data-ajax="false">Link 2 </a>
		</div>
		<a href="#" onclick="$('[name=data]').datebox('open');" data-icon="gear" class="ui-btn-right">Até: <span id="span_data" ><?php echo $_POST['data'] ? $_POST['data'] : date("d/m/Y", mktime(0,0,0,date("m")+1,date("d"),date("Y"))); ?></span></a>
		<div style="right:10px;position:absolute" >
			<input name="data" type="date" data-role="datebox" id="data" value="<?php $_POST['data'] ?>" size="35" data-options='{"mode":"flipbox"}' />
		</div>
		<div data-role="navbar">
			<ul>
				<li><a data-theme="c" class="ui-btn-active" href="#" onclick="javascript:filtraEstado('todos')" >Todas (<span id="span_total">0</span>)</a></li>
				<li><a data-theme="c" href="#" onclick="javascript:filtraEstado('estavel')">Estável (<span id="span_estavel">0</span>)</a></li>
				<li><a data-theme="c" href="#" onclick="javascript:filtraEstado('atencao')">Atenção (<span id="span_atencao">0</span>)</a></li>
				<li><a data-theme="c" href="#" onclick="javascript:filtraEstado('critico')">Crítico (<span id="span_critico">0</span>)</a></li>
				<li><a data-theme="c" href="#" onclick="javascript:filtraEstado('nao_executado')">Não Executado (<span id="span_nao_executado">0</span>)</a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
			<?php listaProjetosMobile(); ?>
	</div>
</body>
</html>