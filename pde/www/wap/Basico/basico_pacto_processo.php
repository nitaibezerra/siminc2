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
				<li><a class="ui-btn-active" data-theme="a" href="basico_pacto.php"  data-transition="slide">Pacto pela Alfabetização</a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="basico_pacto_adesao.php">Adesão</a>
				 </li>
				 <li>
					<a data-transition="flip" href="basico_pacto_redes.php">Redes que já realizaram a formação inicial dos orientadores de estudo</a>
				 </li>
				<li>
					<a data-transition="flip" href="basico_pacto_processo.php">Indicadores de Processo</a>
				 </li>
				 <tr>
				 <td class="fundo_td" width="40%" >
			<div>
                <img style="float:left" src="../../../imagens/icones/icons/casas.png" style="vertical-align:middle;"  />
				<div style="float:left;" class="titulo_box" ><br><b>Indicadores de Processo</b></div>
			</div>
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1696)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					and dsh.tidid1 = 4046
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(1696)";>
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> UFs aderiram</td>
				</tr>
			</table>
			
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1695)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					and dsh.tidid1 = 4042
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(1695)";>
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> municípios aderiram</td>
				</tr>
			</table>
			
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1751)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(1751)";>
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> redes estaduais formaram orientadores</td>
				</tr>
			</table>
			
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1750)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(1750)";>
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> redes municipais formaram orientadores</td>
				</tr>
			</table>
			
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1804)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" onclick="abreIndicadorPopUp(1804)";>
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> bolsas estimadas</td>
				</tr>
			</table>
			
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1805)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> bolsas ativas</td>
				</tr>
			</table>
			
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1806)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> professores alfabetizadores iniciaram formação</td>
				</tr>
			</table>
			
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1807)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> professores alfabetizadores em formação</td>
				</tr>
			</table>
			
			<?php
			$sql = "select sum(dsh.dshqtde) as total, dpe.dpeanoref as ano
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
					where i.indid in (1808)
					and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
					and sehstatus <> 'I'
					group by ano";
			$arrDados = $db->pegaLinha($sql,null,3200);
			?>
			<table class="tabela_box link" cellpadding="2" cellspacing="1" width="100%" >
				<tr height="30">
                	<td class="bold" style="background-color:#3B8550"><span class="titulo_box"><?php echo number_format($arrDados['total'],0,",",".") ?></span> professores alfabetizadores formados</td>
				</tr>
			</table>
		</td>
		
		</tr>
					

	</ul>
                     
	</div>
</body>
</html>

