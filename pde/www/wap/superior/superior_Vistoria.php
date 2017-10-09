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
				 				 <tr>
                  <td class="fundo_td" >
                	<div>
                		<img style="float:left" src="../../../imagens/icones/icons/cone.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" ><br><span class="subtitulo_box" >Situação quanto ao nível de preenchimento</span></div>
                	</div>
                	<?
					$sql = "select nivelpreenchimento, sum(contador) as total, corpreenchimento from (
                                               SELECT      CASE WHEN stoid IN (1, 2) THEN
                                                     (CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) < 45 THEN '1 - Obras atualizadas há menos de 45 dias atrás'
                                                           WHEN DATE_PART('days', NOW() - obrdtvistoria) BETWEEN 45 AND 60   THEN '2 - Obras atualizadas entre 45 e 60 dias'
                                                     ELSE '3 - Obras atualizadas há mais de 60 dias'
                                                     END)
                                                     WHEN stoid = 3 THEN '4 - Obras concluídas'
                                                     ELSE '5 - Não se aplica' END as nivelpreenchimento,
                                                     CASE WHEN stoid IN (1, 2) THEN
                                                     (CASE WHEN DATE_PART('days', NOW() - obrdtvistoria) < 45 THEN '#80BC44'
                                                     WHEN DATE_PART('days', NOW() - obrdtvistoria) BETWEEN 45 AND 60 THEN '#FFC211'
                                                     ELSE '#E95646'
                                                     END)
                                                     WHEN stoid = 3 THEN '#2B86EE'
                                               ELSE '#888888' END as corpreenchimento,
                                               1 AS CONTADOR
                                               FROM obras.obrainfraestrutura  
                                               where obsstatus = 'A'
                                               and stoid not in (11) 
                                               and orgid = 1  
                                         ) as foo
                                         group by nivelpreenchimento, corpreenchimento 
                                         order by 1";
					$vistoria = $db->carregar($sql,null,86400);
                	?>
                	<div style="clear:both;" >
                			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
		                		<tr height="50">
		                			<td class="center" style="background-color:#3B8550"><b>Preenchimento</b></td>
		                			<td class="center" style="background-color:#3B8550"><b>Total</b></td>
		                		</tr>
                			<? if($vistoria[0]) : ?>
                			<? foreach($vistoria as $vis) : ?>
		                		<tr height="50">
		                			<td style="background-color:<?=$vis['corpreenchimento'] ?>;" "><?=str_replace(array("1 - ","2 - ","3 - ","4 -", "5 - "),"",$vis['nivelpreenchimento'])?></td>
		                			<td style="background-color:<?=$vis['corpreenchimento'] ?>;" class="numero" ><?=$vis['total'] ?></td>
		                		</tr>
		                		<?
								$tottotal += $vis['total'];
		                		?>
                			<? endforeach; ?>
                			<? endif; ?>
		                		<tr height="50">
		                			<td class="bold" ><b>Total</b></td>
		                			<td class="numero" ><b><?=$tottotal ?></b></td>
		                		</tr>
		                	</table>
                		</div>
                </td>
                </tr>
				<li>
					<a data-transition="flip" href="superior_Situacao.php">Situação das Obras</a>
				 </li>
	
						</table>
					</div>
				</div>
			</div>
		</td>
	</tr>
	</ul>
	</div>
	
	

</div>
</body>
</html>