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
<body>
<?php 
	include APPRAIZ."includes/classes/Mobile.class.inc";
	include APPRAIZ."/pde/www/_funcoes_mobile.php";
//	$mobile = new Mobile();

?>
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
				<li><a class="ui-btn-active" data-theme="a" href="basico_maiseducacao.php"  data-transition="slide">Mais Educação </a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="basico_maiseducacao_adesao.php">Adesões 2013</a>
				 </li>
				<li>
					<a data-transition="flip" href="basico_maiseducacao_agendas.php">Agendas 2013</a>
				</li>
					 <tr>
						<td class="fundo_td" >
							<div>
								<img style="float:left" src="../../../imagens/icones/icons/doc.png" style="vertical-align:middle;" />
								<div style="float:left" class="titulo_box" ><br><b>Agendas 2013</b><br/></div>
							</div>
							<?php
							$sql = "select count(dshqtde) AS qtde
									from painel.indicador i
									inner join painel.seriehistorica sh on sh.indid=i.indid
									inner join painel.detalheseriehistorica dsh on dsh.sehid=sh.sehid
									inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
									where i.indid in (1205)
									and sehstatus <> 'I'
									and tidid2 = 3192 --rural
									and dpeanoref = '2013'";
							$total_pronacampo = $db->pegaUm($sql);
							$sql = "select sh.sehqtde AS Total
									from painel.indicador i
									inner join painel.seriehistorica sh on sh.indid=i.indid
									inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
									where i.indid in (761)
									and sehstatus <> 'I'
									and dpeanoref = '2013'
									order by sehdtcoleta desc
									limit 1";
							$total_sem_miseria = $db->pegaUm($sql);
							?>
							<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
								<tr>
									<td class="center bold" >Agenda</td>
									<td class="center bold" >Escolas que aderiram</td>
								</tr>
								<tr class="link" onclick="abreIndicadorPopUp(1205);">
									<td>Pronacampo</td>
									<td class="numero" ><?php echo number_format($total_pronacampo,0,",",".") ?></td>
								</tr>
								<tr class="link" onclick="abreIndicadorPopUp(761);">
									<td>Brasil sem Miséria</td>
									<td class="numero" ><?php echo number_format($total_sem_miseria,0,",",".") ?></td>
								</tr>
							</table>
						</td>
					</tr>
				<li>
					<a data-transition="flip" href="basico_maiseducacao_escolasaderiram.php">Escolas que Aderiram</a>
				</li>
					

	</ul>
                     
	</div>
</body>
</html>

