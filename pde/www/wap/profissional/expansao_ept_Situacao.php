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
	include APPRAIZ."/pde/www/_funcoes_mobile.php";
//	$mobile = new Mobile();
?>
<form name="formulario_mobile" method="post">
<div data-theme="a" data-role="page">
	<div data-role="header" data-position="fixed" data-theme="a">
		<div data-role="controlgroup" data-type="horizontal">
			<a data-transition="slidedown" href="../" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
			<a href="../profissional" data-role="button" data-icon="arrow-r" data-ajax="false">Educação Profissional</a>
		</div>
		<h1>Obras Expansão da Rede Federal de EPT</h1>
		<div data-role="navbar">
			<ul>
				<li><a class="ui-btn-active" data-theme="a" href="expansao_ept.php"  data-transition="slide">Expansão da Rede Federal de EPT </a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="expansao_ept_Vistoria.php">Vistorias</a>
				 </li>
				<li>
					<a data-transition="flip" href="expansao_ept_Situacao.php">Situação das Obras</a>
				 </li>
				 <tr>
				                 <td class="fundo_td_azul" colspan="3" valign="top" style="cursor:pointer;" onclick="abreRelatorioObras(2, 4, 34, '', '');">
                	<div>
                		<img style="float:left" src="../../../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" ><br><b>Obras por situação</b></div>
                	</div>
                	<?php
					$sql = "SELECT s.stodesc, o.stoid, count(s.stodesc) as qtd
							FROM obras.obrainfraestrutura o 
							INNER JOIN obras.situacaoobra s on s.stoid = o.stoid
							WHERE o.obsstatus = 'A' 
							AND prfid = 34 --EXPANSÃO RFEPT
							AND o.stoid not in (11) 
							AND o.orgid = 2
							GROUP BY o.stoid, s.stodesc, s.stoid";
                	$arrObras = $db->carregar($sql); 
                	$arrObras = !$arrObras ? array() : $arrObras;
                	$total_obras = 0; 
                	?>
                		<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" align="center" >
                			<tr>
	                			<td class="center bold" style="background-color:#d4ae73"><b>Situação</b></td>
	                			<td class="center bold" style="background-color:#d4ae73"><b>Obras</b></td>
	                		</tr>
                			<?php foreach($arrObras as $obr) : ?>
	                		<tr>
	                			<td align="left" style="background-color:#d4ae73"><?php echo str_replace(array("1 - ","2 - ","3 - ","4 -", "5 - "),"",$obr['stodesc']) ?></td>
	                			<td class="numero" style="background-color:#d4ae73"><?php $total_obras+=$obr['qtd'];echo number_format($obr['qtd'],0,"",".") ?></td>
	                		</tr>
	                		<?php endforeach; ?>
	                		<tr>
	                			<td align="left" class="bold" style="background-color:#d4ae73"><b>Total</b></td>
	                			<td class="numero" style="background-color:#d4ae73"><b><?php echo number_format($total_obras,0,"",".") ?></b></td>
	                		</tr>
	                	</table>
                </td>
                </tr>
                
             
		</ul>
	
	
	</div>
</body>
</html>