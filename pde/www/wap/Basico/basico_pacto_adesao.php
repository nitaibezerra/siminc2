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
				<li><a class="ui-btn-active" data-theme="a" href="basico_pacto.php"  data-transition="slide">Pacto pela Alfabetização</a></li>
			</ul>
		</div>
	</div>
	<div data-role="content">
	
	 <ul data-role="listview" data-inset="true">
				<li>
					<a data-transition="flip" href="basico_pacto_adesao.php">Adesão</a>
				 </li>
				 <tr>

							<td class="fundo_td">
			<div>
                <img style="float:left" src="../../../imagens/icones/icons/doc.png" style="vertical-align:middle;"  />
				<div style="float:left;" class="titulo_box" > <br><b>Adesão</b></div>
			</div>
			<?php
			$sql = "select tipo, situacao, total
					from (
						select '1696' as tipo,
							case tidid1
								when 4046 then 'Adesão concluída'
								when 4047 then 'Adesão não concluída'
								when 4048 then 'Não aderiu'
								when 4049 then 'Não se manifestou'
							end as situacao,
							SUM(dsh.dshqtde) as total
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						where i.indid in (1696)
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						and sehstatus <> 'I'
						group by situacao
					union all
						select '1695' as tipo,
							case tidid1
								when 4042 then 'Adesão concluída'
								when 4043 then 'Adesão não concluída'
								when 4044 then 'Não aderiu'
								when 4045 then 'Não se manifestou'
							end as situacao,
							SUM(dsh.dshqtde) as total
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						where i.indid in (1695)
						and sh.dpeid = (SELECT MAX(dpeid) FROM painel.seriehistorica s where s.indid = sh.indid)
						and sehstatus <> 'I'
						group by situacao
					) as foo
					order by tipo desc, situacao";
			$arrDados = $db->carregar($sql,null,3200);
			if($arrDados){
					foreach($arrDados as $dado){
						$arrTipo[$dado['tipo']][$dado['situacao']]['total'][] = $dado['total'];
				}
			}
			?>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
			<tr height="30">
                	<td class="center bold" style="background-color:#3B8550">&nbsp;</td>
					<td class="center bold" style="background-color:#3B8550">Adesão concluída</td>
                	<td class="center bold" style="background-color:#3B8550">Adesão não concluída</td>
                	<td class="center bold" style="background-color:#3B8550">Não aderiu</td>
                	<td class="center bold" style="background-color:#3B8550">Não se manifestou</td>
			</tr>
			<?php foreach($arrTipo as $chave => $onb): ?>
				<tr  height="30" >
					<td class="" style="background-color:#3B8550"><?=$chave=='1696' ? 'Estados' : 'Municípios' ?></td>
					<td class="numero" style="background-color:#3B8550"><?=is_array($onb['Adesão concluída']['total'])?number_format(array_sum($onb['Adesão concluída']['total']),0,",","."):0 ?></td>
					<td class="numero" style="background-color:#3B8550"><?=is_array($onb['Adesão não concluída']['total'])?number_format(array_sum($onb['Adesão não concluída']['total']),0,",","."):0 ?></td>
					<td class="numero" style="background-color:#3B8550"><?=is_array($onb['Não aderiu']['total'])?number_format(array_sum($onb['Não aderiu']['total']),0,",","."):0 ?></td>
					<td class="numero" style="background-color:#3B8550"><?=is_array($onb['Não se manifestou']['total'])?number_format(array_sum($onb['Não se manifestou']['total']),0,",","."):0 ?></td>
				</tr>
			<?php endforeach; ?>
			</table>
		</td>
						</tr>
				<li>
					<a data-transition="flip" href="basico_pacto_redes.php">Redes que já realizaram a formação inicial dos orientadores de estudo</a>
				 </li>
				<li>
					<a data-transition="flip" href="basico_pacto_processo.php">Indicadores de Processo</a>
				 </li>
					

	</ul>
                     
	</div>
</body>
</html>

