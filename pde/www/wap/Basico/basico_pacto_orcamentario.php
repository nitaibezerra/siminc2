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
				<li><a data-theme="a" href="basico_crechepre.php" data-transition="slide" >Creches e Pré-Escolas </a></li>
				<li><a data-theme="a" href="basico_quadras.php"  data-transition="slide">Quadras </a></li>
				<li><a data-theme="a" href="basico_caminho.php"  data-transition="slide">Caminho da Escola</a></li>
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
				 <tr>

							<td class="fundo_td">
			<div>
                <img style="float:left" src="../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
				<div style="float:left;" class="titulo_box" >Redes que já realizaram a formação<br>inicial dos orientadores de estudo</div>
			</div>
			<?php
			$sql = "select tipo, id, total
					from (
						select 'Redes Municipais' as tipo, 1750 as id, sum(dsh.dshqtde) as total
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						where i.indid in (1750)
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						and sehstatus <> 'I'
						group by tipo, id
					union all
						select 'Redes Estaduais' as tipo, 1751 as id, sum(dsh.dshqtde) as total
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						where i.indid in (1751)
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						and sehstatus <> 'I'
						group by tipo, id
					) as foo
					order by tipo";
			$arrDados = $db->carregar($sql,null,3200);
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<?php foreach($arrDados as $dado): ?>
				<tr class="link" onclick="abreIndicadorPopUp('<?=$dado['id']?>');">
                	<td class="" ><?php echo $dado['tipo'] ?></td>
                	<td class="numero" ><?php echo number_format($dado['total'],0,",",".") ?></td>
				</tr>
				<?php endforeach; ?>
			</table>
		</td>
						</tr>
				<li>
					<a data-transition="flip" href="basico_pacto_processo.php">Indicadores de Processo</a>
				 </li>
					

	</ul>
                     
	</div>
</body>
</html>

