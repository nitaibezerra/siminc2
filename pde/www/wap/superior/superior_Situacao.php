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
	<title>SIMEC - Mobile</title> 
	<meta name="viewport" content="width=device-width, initial-scale=1">
	<link rel="stylesheet" href="/includes/mobile-simec/SIMEC.min.css" />
	<link rel="stylesheet" href="http://code.jquery.com/mobile/1.3.0-rc.1/jquery.mobile.structure-1.3.0-rc.1.min.css" /> 
	<script src="http://code.jquery.com/jquery-1.8.3.min.js"></script> 
	<script src="http://code.jquery.com/mobile/1.3.0-rc.1/jquery.mobile-1.3.0-rc.1.min.js"></script> 
</head> 
<body >
<?php 
	include APPRAIZ."includes/classes/Mobile.class.inc";
//	include APPRAIZ."/pde/www/_funcoes_mobile.php";
//	$mobile = new Mobile();
?>
<form name="formulario_mobile" method="post">
<div data-theme="a" data-role="page">
	<div data-role="header" data-position="fixed" data-theme="a">
		<div data-role="controlgroup" data-type="horizontal">
			<a data-transition="slidedown" href="../" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
		</div>
		<h1>Educação Superior</h1>
		<div data-role="navbar">
			<ul>
				<li><a data-theme="a" href="../superior/" data-transition="slide" >Expansão da Educação Superior </a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="superior_Vistoria.php">Vistorias</a>
				 </li>
				<li>
					<a data-transition="flip" href="superior_Situacao.php">Situação das Obras</a>
				 </li>
	<tr>
		<td class="fundo_td">
			<div>
				<img style="float:left" src="../../../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
				<div><br/>Expansão da Rede Federal <br/>de Ensino Superior - Obras<br/></div>
						<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
							<?php $sql = "	SELECT s.stodesc, o.stoid, count(s.stodesc) as qtd
											FROM obras.obrainfraestrutura o 
											INNER JOIN obras.situacaoobra s on s.stoid = o.stoid
											WHERE o.obsstatus = 'A' 
											AND o.orgid = 1
											AND o.stoid not in (11)
											GROUP BY o.stoid, s.stodesc, s.stoid";
							$arrObras = $db->carregar($sql) ?>
							<tr height="50">
	                			<td class="center bold" style="background-color:#bcad4e"><b>Situação</b></td>
	                			<td class="center bold" style="background-color:#bcad4e"><b>Obras</b></td>
	                		</tr>
							<?php foreach($arrObras as $o): ?>
							<tr height="50">
								<td style="background-color:#bcad4e"><?php echo $o['stodesc'] ?></td>
								<td class="numero" style="background-color:#bcad4e"><?php $total_obras+=$o['qtd'];echo number_format($o['qtd'],0,"",".") ?></td>
							</tr>
							<?php endforeach; ?>
							<tr height="50">
	                			<td align="left" class="bold" style="background-color:#bcad4e"><b>Total</b></td>
	                			<td class="numero" style="background-color:#bcad4e"><b><?php echo number_format($total_obras,0,"",".") ?></b></td>
	                		</tr>
						</table>
			</div>
		</td>
	</tr>
	</ul>
	</div>
	
	

</div>
</body>
</html>