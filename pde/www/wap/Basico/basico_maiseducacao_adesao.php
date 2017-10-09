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
//	$mobile = new Mobile();

?>
<form name="formulario_mobile" method="post" >
<div data-theme="a" data-role="page">
	<div data-role="header" data-position="fixed">
		<div data-role="controlgroup" data-type="horizontal">
			<a data-transition="slidedown" href="../" data-role="button" data-icon="home" class="inicio-rodape">Início</a>
			<a href="../Basico" data-role="button" data-icon="arrow-r" data-ajax="false">Educação Básica </a>
		</div>
		<h1>Educação Básica</h1>
		<div data-role="navbar">
			<ul>
				<li><a class="ui-btn-active" data-theme="a" href="basico_maiseducacao.php" data-transition="slide" >Mais Educação </a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="basico_maiseducacao_adesao.php">Adesões 2013</a>
				 </li>
				 <tr>
					<td class="fundo_td" style="cursor:pointer;" onclick="abreIndicadorPopUp(690);">
						<div>
							<img style="float:left" src="../../../imagens/icones/icons/mais.png" style="vertical-align:middle;" />
							<div style="float:left" class="titulo_box" ><br><b>Adesões 2013</b></div>
						</div>
						<div style="clear:both;" >
						<?php
						$sql = "select esd.esddsc doc, count(me.memid) as qtde
								from pdeescola.memaiseducacao me
								inner join workflow.documento doc on doc.docid = me.docid
								inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
								where memanoreferencia = 2013 and memstatus = 'A' and esd.esdid < 34 and esd.esdid != 32 -- igual ou além de finalizado
								group by doc
								union
								select 'Adesão efetivada' doc, count(me.memid) as qtde
								from pdeescola.memaiseducacao me
								inner join workflow.documento doc on doc.docid = me.docid
								inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
								where memanoreferencia = 2013 and memstatus = 'A' and esd.esdid >= 34 -- igual ou além de finalizado
								group by doc
								order by doc;";
						$arrDados = $db->carregar($sql);
						$total_adesao = 0;
						?>
						<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
							<?php foreach($arrDados as $dado): ?>
								<tr>
									<td><?php echo str_replace("Avaliação MEC","Em avaliação pelo MEC",$dado['doc']) ?></td>
									<td class="numero" ><?php $total_adesao+=$dado['qtde'];echo number_format($dado['qtde'],0,",",".") ?></td>
								</tr>
							<?php endforeach; ?>
							<tr>
								<td class="bold" >Total</td>
								<td class="numero" ><?php echo number_format($total_adesao,0,",",".") ?></td>
							</tr>
						</table>
						</div>
					</td>
				</tr>
				<li>
					<a data-transition="flip" href="basico_maiseducacao_agendas.php">Agendas 2013</a>
				</li>
				<li>
					<a data-transition="flip" href="basico_maiseducacao_escolasaderiram.php">Escolas que Aderiram</a>
				 </li>
					

	</ul>
                     
	</div>
</body>
</html>

