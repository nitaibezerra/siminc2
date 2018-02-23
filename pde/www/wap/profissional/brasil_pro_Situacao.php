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
<style>
	.fundo_td_wap{background-color:#3B8550}
	.bold{font-weight:bold}
	.fundo_tr_wap{height:50px}
	.tabela_box{color:#FFFFFF;}
	.link{cursor:pointer}
</style>
<form name="formulario_mobile" method="post">
<div data-theme="a" data-role="page">
	<div data-role="header" data-position="fixed" data-theme="a">
		<div data-role="controlgroup" data-type="horizontal">
			<a data-transition="slidedown" href="../" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
			<a href="../profissional" data-role="button" data-icon="arrow-r" data-ajax="false">Educação Profissional</a>
		</div>
		<h1>Brasil Profissionalizado</h1>
		<div data-role="navbar">
			<ul>
				<li><a data-theme="a" class="ui-btn-active" href="brasil_pro.php" data-transition="slide" >Brasil Profissionalizado </a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="brasil_pro_Vistoria.php">Vistorias</a>
				 </li>
				<li>
					<a data-transition="flip" href="brasil_pro_Situacao.php">Situação das Obras</a>
				 </li>
				 <tr>
					<td class="fundo_td_azul" colspan="3" >
                		<div>
                			<img style="float:left" src="../../../imagens/icones/icons/obras.png" style="vertical-align:middle;"  />
	                		<div style="float:left" class="titulo_box" ><font color = white ><br><b>Situação das Obras</b></font></div>
	                	</div>
	                	   		<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="100%" >
			                		<tr>
		                				<td class="center" style="background-color:#d4ae73"><font color = white ><b>Situação</b></font></td>
			                			<td class="center" style="background-color:#d4ae73"><font color = white ><b>Construção</b></font></td>
			                			<td class="center" style="background-color:#d4ae73"><font color = white ><b>Reforma</b></font></td>
			                			<td class="center" style="background-color:#d4ae73"><font color = white ><b>Ampliação</b></font></td>
			                			<td class="center" style="background-color:#d4ae73"><font color = white ><b>Reforma/Ampliação</b></font></td>
										<td class="center" style="background-color:#d4ae73"><font color = white ><b>Total</b></font></td>
			                		</tr>
				                	<?php
									$sql = "SELECT
											tobdesc AS tipo,
											esd.esddsc AS situacao,
											count(0) AS total
										FROM obras2.obras o
										INNER JOIN obras2.empreendimento e ON e.empid = o.empid AND e.empstatus = 'A'
										INNER JOIN obras2.tipoobra tob ON tob.tobid = o.tobid
										INNER JOIN workflow.documento d ON d.docid = o.docid
										INNER JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid
										WHERE o.obrstatus = 'A'
										AND e.orgid=3
										AND d.esdid <> 770 --Etapa Concluída
										AND o.obridpai IS NULL
										AND o.obrid NOT IN (7828,7829,7840,1000015,1000046,1000049) --Obras de teste
										AND e.prfid = 40
										GROUP BY tipo, situacao
										ORDER BY tipo, situacao";
									$arrDados = $db->carregar( $sql, null, 3200 );
					                	
									foreach( $arrDados as $dado ) :
										$tipo[$dado['situacao']][$dado['tipo']] = $dado['total'];
									endforeach;
										
									foreach( $tipo as $situacao => $dado ) :
										$totalSituacao=$dado['Construção']+$dado['Reforma']+$dado['Ampliação']+$dado['Ampliação/Reforma'];
										$totalConstrucao+=$dado['Construção'];
										$totalReforma+=$dado['Reforma'];
										$totalAmpliacao+=$dado['Ampliação'];
										$totalAmpliacaoReforma+=$dado['Ampliação/Reforma'];
				                	?>
			                		<tr>
			                			<td style="background-color:#d4ae73"><font color = white ><?=$situacao ?></font></td>
			                			<td class="numero" style="background-color:#d4ae73"><font color = white ><?=number_format($dado['Construção'],0,",",".") ?></font></td>
			                			<td class="numero" style="background-color:#d4ae73" ><font color = white ><?=number_format($dado['Reforma'],0,",",".") ?></font></td>
			                			<td class="numero" style="background-color:#d4ae73"><font color = white ><?=number_format($dado['Ampliação'],0,",",".") ?></font></td>
			                			<td class="numero" style="background-color:#d4ae73"><font color = white ><?=number_format($dado['Ampliação/Reforma'],0,",",".") ?></font></td>
										<td class="numero" style="background-color:#d4ae73"><font color = white ><b><?=number_format($totalSituacao,0,",",".") ?></b></font></td>
			                		</tr>
				                	<? 
									endforeach; 
									$totalGeral=$totalConstrucao+$totalReforma+$totalAmpliacao+$totalAmpliacaoReforma;
									?>
									<tr>
			                			<td style="background-color:#d4ae73"><font color = white ><b>Total</b></td>
			                			<td class="numero" style="background-color:#d4ae73"><font color = white ><b><?=number_format($totalConstrucao,0,",",".") ?></b></font></td>
			                			<td class="numero" style="background-color:#d4ae73"><font color = white ><b><?=number_format($totalReforma,0,",",".") ?></b></font></td>
			                			<td class="numero" style="background-color:#d4ae73"><font color = white ><b><?=number_format($totalAmpliacao,0,",",".") ?></b></font></td>
			                			<td class="numero" style="background-color:#d4ae73"><font color = white ><b><?=number_format($totalAmpliacaoReforma,0,",",".") ?></b></font></td>
										<td class="numero" style="background-color:#d4ae73"><font color = white ><b><?=number_format($totalGeral,0,",",".") ?></b></font></td>
			                		</tr>
			                		</table>
						           
			                </div>
	                	</td>
	                </tr>
		</ul>
	</div>
</form>
</body>
</html>