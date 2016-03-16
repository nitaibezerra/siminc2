<?

$sql = "select
  sum(round(rssdotacaoautorizadacusteio,0)) as rssdotacaoautorizadacusteio,
  sum(round(rssdotacaoautorizadainvest,0)) as rssdotacaoautorizadainvest,
  sum(round(rssdotacaoautorizadainversao,0)) as rssdotacaoautorizadainversao,
  
  sum(round(rsscreditoespecialcusteio,0)) as rsscreditoespecialcusteio,
  sum(round(rsscreditoespecialinvest,0)) as rsscreditoespecialinvest,
  sum(round(rsscreditoespecialinversao,0)) as rsscreditoespecialinversao,
  
  sum(round(rssemendascusteio,0)) as rssemendascusteio,
  sum(round(rssemendasinvest,0)) as rssemendasinvest,
  sum(round(rssemendasinversao,0)) as rssemendasinversao,
  
  sum(round(rssportariacusteio,0)) as rssportariacusteio,
  sum(round(rssportariainvest,0)) as rssportariainvest,
  sum(round(rssportariainversao,0)) as rssportariainversao,
  
  sum(round(rssdevolucaocusteio,0)) as rssdevolucaocusteio,
  sum(round(rssdevolucaoinvest,0)) as rssdevolucaoinvest,
  sum(round(rssdevolucaoinversao,0)) as rssdevolucaoinversao,
  
  sum(round(rssdescentralizacaoliqcusteio,0)) as rssdescentralizacaoliqcusteio,
  sum(round(rssdescentralizacaoliqinvest,0)) as rssdescentralizacaoliqinvest,
  sum(round(rssdescentralizacaoliqinversao,0)) as rssdescentralizacaoliqinversao,
  
  sum(round(rssempenhadodiretouocusteio,0)) as rssempenhadodiretouocusteio,
  sum(round(rssempenhadodiretouoinvest,0)) as rssempenhadodiretouoinvest,
  sum(round(rssempenhadodiretouoinversao,0)) as rssempenhadodiretouoinversao,
  
  sum(round(rssempenhadodescentralizadocusteio,0)) as rssempenhadodescentralizadocusteio,
  sum(round(rssempenhadodescentralizadoinvest,0)) as rssempenhadodescentralizadoinvest,
  sum(round(rssempenhadodescentralizadoinversao,0)) as rssempenhadodescentralizadoinversao,
  
  sum(round(rsspactuadocusteio,0)) as rsspactuadocusteio,
  sum(round(rsspactuadoinvest,0)) as rsspactuadoinvest,
  sum(round(rsspactuadobolsas,0)) as rsspactuadobolsas,
  
  sum( round( ( coalesce(rssdotacaoautorizadacusteio,0) - coalesce(rsscreditoespecialcusteio,0) - coalesce(rssemendascusteio,0) ), 0) + round( ( coalesce(rssportariacusteio,0) - coalesce(rssdevolucaocusteio,0) ), 0) ) as rssdisponibilizadocusteio,
  sum( round( ( coalesce(rssdotacaoautorizadainvest,0) - coalesce(rsscreditoespecialinvest,0) - coalesce(rssemendasinvest,0) ), 0) + round( ( coalesce(rssportariainvest,0) - coalesce(rssdevolucaoinvest,0) ), 0) + round( ( coalesce(rssdotacaoautorizadainversao,0) + coalesce(rsscreditoespecialinversao,0) + coalesce(rssemendasinversao,0) + coalesce(rssportariainversao,0) + coalesce(rssdevolucaoinversao,0) + coalesce(rssdescentralizacaoliqinversao,0) + coalesce(rssempenhadodiretouoinversao,0) + coalesce(rssempenhadodescentralizadoinversao,0) ), 0) ) as rssdisponibilizadoinvest,
  sum(rssdisponibilizadobolsas) as rssdisponibilizadobolsas
  
from financeiro.relatoriosesu s
--inner join entidade.entidade e ON e.entunicod = s.rssunicod
--inner join entidade.funcaoentidade fe ON fe.entid = e.entid
--where fe.funid = 12";
$dados1 = $db->carregar($sql);

$sql = "select
  rssunicod,
--  entsig as rssunidsc,
  rssunidsc,
  
  sum(rssdotacaoautorizadacusteio) as rssdotacaoautorizadacusteio,
  sum(rssdotacaoautorizadainvest) as rssdotacaoautorizadainvest,
  sum(rssdotacaoautorizadainversao) as rssdotacaoautorizadainversao,
  
  sum(rsscreditoespecialcusteio) as rsscreditoespecialcusteio,
  sum(rsscreditoespecialinvest) as rsscreditoespecialinvest,
  sum(rsscreditoespecialinversao) as rsscreditoespecialinversao,
  
  sum(rssemendascusteio) as rssemendascusteio,
  sum(rssemendasinvest) as rssemendasinvest,
  sum(rssemendasinversao) as rssemendasinversao,
  
  sum(rssportariacusteio) as rssportariacusteio,
  sum(rssportariainvest) as rssportariainvest,
  sum(rssportariainversao) as rssportariainversao,
  
  sum(rssdevolucaocusteio) as rssdevolucaocusteio,
  sum(rssdevolucaoinvest) as rssdevolucaoinvest,
  sum(rssdevolucaoinversao) as rssdevolucaoinversao,
  
  sum(rssdescentralizacaoliqcusteio) as rssdescentralizacaoliqcusteio,
  sum(rssdescentralizacaoliqinvest) as rssdescentralizacaoliqinvest,
  sum(rssdescentralizacaoliqinversao) as rssdescentralizacaoliqinversao,
  
  sum(rssempenhadodiretouocusteio) as rssempenhadodiretouocusteio,
  sum(rssempenhadodiretouoinvest) as rssempenhadodiretouoinvest,
  sum(rssempenhadodiretouoinversao) as rssempenhadodiretouoinversao,
  
  sum(rssempenhadodescentralizadocusteio) as rssempenhadodescentralizadocusteio,
  sum(rssempenhadodescentralizadoinvest) as rssempenhadodescentralizadoinvest,
  sum(rssempenhadodescentralizadoinversao) as rssempenhadodescentralizadoinversao,
  
  sum(rsspactuadocusteio) as rsspactuadocusteio,
  sum(rsspactuadoinvest) as rsspactuadoinvest,
  sum(rsspactuadobolsas) as rsspactuadobolsas,
  
  sum( ( coalesce(rssdotacaoautorizadacusteio,0) - coalesce(rsscreditoespecialcusteio,0) - coalesce(rssemendascusteio,0) ) + ( coalesce(rssportariacusteio,0) - coalesce(rssdevolucaocusteio,0) ) ) as rssdisponibilizadocusteio,
  sum( round( ( coalesce(rssdotacaoautorizadainvest,0) - coalesce(rsscreditoespecialinvest,0) - coalesce(rssemendasinvest,0) ), 0) + round( ( coalesce(rssportariainvest,0) - coalesce(rssdevolucaoinvest,0) ), 0) + round( ( coalesce(rssdotacaoautorizadainversao,0) + coalesce(rsscreditoespecialinversao,0) + coalesce(rssemendasinversao,0) + coalesce(rssportariainversao,0) + coalesce(rssdevolucaoinversao,0) + coalesce(rssdescentralizacaoliqinversao,0) + coalesce(rssempenhadodiretouoinversao,0) + coalesce(rssempenhadodescentralizadoinversao,0) ), 0) ) as rssdisponibilizadoinvest,
  sum(rssdisponibilizadobolsas) as rssdisponibilizadobolsas
  
from financeiro.relatoriosesu s
--inner join entidade.entidade e ON e.entunicod = s.rssunicod
--inner join entidade.funcaoentidade fe ON fe.entid = e.entid
--where fe.funid = 12
group by 
  rssunicod,
   rssunidsc,ordem
order by ordem,
  rssunidsc";
$dados = $db->carregar($sql);

?>

<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<style type="text/css">
@media screen {
.div_rolagem{ overflow: auto; width:100%; height:400px; }
}

.noPadding{padding:0;}

TD
{
    FONT: 8pt Arial,verdana
}
TH
{
	text-align: center;
    FONT: 8pt Arial,verdana;
    font-weight: bold;
}
</style>

<table width="100%">
	<thead>
		<tr>
			<th class="noPadding" align="left">
				<table width="100%" border="0" cellpadding="0" cellspacing="0">
					<tr>
						<td valign="top" width="50" rowspan="2">
							<img src="../imagens/brasao.gif" width="45" height="45" border="0">
						</td>
						<td nowrap align="left" valign="top" height="1" style="padding:5px 0 0 0;">
							<font style="font-size:11px;">
								MEC - SESu - DIFES<br/>
								<b>REUNI - Acompanhamento Orçamentário<br/>
								Reestruturação e Expansão das Universidades Federais - REUNI</b><br />
								Execução 2009
							</font>
						</td>
						<td align="right" valign="top" height="1" style="padding:5px 0 0 0;">
							<font style="font-size:11px;">
								Impresso por: <b><?= $_SESSION['usunome'] ?></b><br/>
								<b>Hora da Impressão: <?= date( 'd/m/Y - H:i:s' ) ?></b><br/>
							</font>
						</td>
						<tr><td>&nbsp;</td></tr>
				</table>
				<table style="width:98.8%;" align="left" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<thead>
						<tr style="height:50px;">
							<!-- Primeira Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PACTUADO</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>DISPONIBILIZADO</strong></td>
							<th valign="top" colspan="3" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red" size="1">*<sup>1</sup> </font>DIFERENÇA: Pactuado - Disponib.</strong></td>
							<th valign="top" colspan="3" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red" size="1">**<sup>1</sup> </font>% Pactuado Devido</strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Primeira Tabela -->
							<th width="4%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>UO</strong></td>
							<th width="4%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IFES</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio<br>AE + UBC</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red" size="1">*<sup>2</sup> </font><font color="blue">exceto Bolsa</font></strong></td>
							<!--  <th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">*</font> Bolsas<br>Rec. Capes</strong></td> -->
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br>com Bolsa Capes</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest. + Inv. Financ</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red" size="1">*<sup>3</sup> </font><font color="blue">exceto Bolsa</font></strong></td>
							<!-- <th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">*</font> Bolsas<br>Rec. Capes</strong></td> -->
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br>com Bolsa Capes</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red" size="1">*<sup>2</sup> </font><font color="blue">exceto Bolsa Capes</font></strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>C</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>I</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>T</strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Primeira Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">D<br>pactuado</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">E<br>pactuado</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">F</font></strong></td>
							<!-- <th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">G<br>pactuado</font></strong></td> -->
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">H</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">I</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">J</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">K</font></strong></td>
							<!-- <th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">L<br>capes</font></strong></td> -->
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">M</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">N</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">O</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">P</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">Q</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">R</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="blue">S</strong></td>
						</tr>
						<?php 
						$D1 = (($dados1[0]["rsspactuadocusteio"] && $dados1[0]["rsspactuadocusteio"] != 0) ? number_format($dados1[0]["rsspactuadocusteio"], 0, ',', '.') : "-");
						$E1 = (($dados1[0]["rsspactuadoinvest"] && $dados1[0]["rsspactuadoinvest"] != 0) ? number_format($dados1[0]["rsspactuadoinvest"], 0, ',', '.') : "-");
						$F1 = ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"] != 0) ? number_format(($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]), 0, ',', '.') : "-";
						
						//$G1 = (($dados1[0]["rsspactuadobolsas"] && $dados1[0]["rsspactuadobolsas"] != 0) ? number_format($dados1[0]["rsspactuadobolsas"], 0, ',', '.') : "-");
						$H1 = (($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) + $dados1[0]["rsspactuadobolsas"] != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) + $dados1[0]["rsspactuadobolsas"]), 0, ',', '.') : "-";
						
						$I1 = (($dados1[0]["rssdisponibilizadocusteio"] && $dados1[0]["rssdisponibilizadocusteio"] != 0) ? number_format($dados1[0]["rssdisponibilizadocusteio"], 0, ',', '.') : "-");
						$J1 = (($dados1[0]["rssdisponibilizadoinvest"] && $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format($dados1[0]["rssdisponibilizadoinvest"], 0, ',', '.') : "-");
						$K1 = ($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
						
						//$L1 = (($dados1[0]["rssdisponibilizadobolsas"] && $dados1[0]["rssdisponibilizadobolsas"] != 0) ? number_format($dados1[0]["rssdisponibilizadobolsas"], 0, ',', '.') : "-");
						
						$M1 = (($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]) + $dados1[0]["rssdisponibilizadobolsas"] != 0) ? number_format((($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]) + $dados1[0]["rssdisponibilizadobolsas"]), 0, ',', '.') : "-";
						$N1 = ($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"] != 0) ? number_format(($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]), 0, ',', '.') : "-";
						$O1 = ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
						$P1 = (($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])), 0, ',', '.') : "-";
						
						$Q1 = ($dados1[0]["rsspactuadocusteio"]==0) ? "-" : (($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) / $dados1[0]["rsspactuadocusteio"] != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) / $dados1[0]["rsspactuadocusteio"]), 0, ',', '.') : "-";
						$R1 = ($dados1[0]["rsspactuadoinvest"]==0) ? "-" : (($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) / $dados1[0]["rsspactuadoinvest"] != 0) ? number_format((($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) / $dados1[0]["rsspactuadoinvest"]), 0, ',', '.') : "-";
						$S1 = ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]==0) ? "-" : ((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])) / ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) != 0) ? number_format(((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])) / ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"])), 0, ',', '.') : "-";
			
						/*<td align="right"><b>'.$G1.'</b></td>
						<td align="right"><b>'.$L1.'</b></td>*/
						echo '<tr bgcolor="#c0c0c0" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
								<td colspan="2" align="left"><b>TOTAL</b></td>
							  	<!-- Primeira Tabela -->
							  	<td align="right"><b>'.$D1.'</b></td>
							  	<td align="right"><b>'.$E1.'</b></td>
							  	<td align="right"><b>'.$F1.'</b></td>
							  	
							  	<td align="right"><b>'.$H1.'</b></td>
							  	<td align="right"><b>'.$I1.'</b></td>
							  	<td align="right"><b>'.$J1.'</b></td>
							  	<td align="right"><b>'.$K1.'</b></td>
							  	
							  	<td align="right"><b>'.$M1.'</b></td>
							  	<td align="right"><b>'.$N1.'</b></td>
							  	<td align="right"><b>'.$O1.'</b></td>
							  	<td align="right"><b>'.$P1.'</b></td>
							  	<td align="right"><b>'.$Q1.'</b></td>
							  	<td align="right"><b>'.$R1.'</b></td>
							  	<td align="right"><b>'.$S1.'</b></td>
							  </tr>';
						?>
					</thead>
				</table>
			</th>
		</tr>
	</thead>
	<tbody>
		<tr>
			<td class="noPadding" align="center">
				<div class="div_rolagem">
					<table style="width:100%;" align="left" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<?
						/*$D1 = (($dados1[0]["rsspactuadocusteio"] && $dados1[0]["rsspactuadocusteio"] != 0) ? number_format($dados1[0]["rsspactuadocusteio"], 0, ',', '.') : "-");
						$E1 = (($dados1[0]["rsspactuadoinvest"] && $dados1[0]["rsspactuadoinvest"] != 0) ? number_format($dados1[0]["rsspactuadoinvest"], 0, ',', '.') : "-");
						$F1 = ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"] != 0) ? number_format(($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]), 0, ',', '.') : "-";
						
						$G1 = (($dados1[0]["rsspactuadobolsas"] && $dados1[0]["rsspactuadobolsas"] != 0) ? number_format($dados1[0]["rsspactuadobolsas"], 0, ',', '.') : "-");
						$H1 = (($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) + $dados1[0]["rsspactuadobolsas"] != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) + $dados1[0]["rsspactuadobolsas"]), 0, ',', '.') : "-";
						
						$I1 = (($dados1[0]["rssdisponibilizadocusteio"] && $dados1[0]["rssdisponibilizadocusteio"] != 0) ? number_format($dados1[0]["rssdisponibilizadocusteio"], 0, ',', '.') : "-");
						$J1 = (($dados1[0]["rssdisponibilizadoinvest"] && $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format($dados1[0]["rssdisponibilizadoinvest"], 0, ',', '.') : "-");
						$K1 = ($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
						
						$L1 = (($dados1[0]["rssdisponibilizadobolsas"] && $dados1[0]["rssdisponibilizadobolsas"] != 0) ? number_format($dados1[0]["rssdisponibilizadobolsas"], 0, ',', '.') : "-");
						
						$M1 = (($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]) + $dados1[0]["rssdisponibilizadobolsas"] != 0) ? number_format((($dados1[0]["rssdisponibilizadocusteio"] + $dados1[0]["rssdisponibilizadoinvest"]) + $dados1[0]["rssdisponibilizadobolsas"]), 0, ',', '.') : "-";
						$N1 = ($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"] != 0) ? number_format(($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]), 0, ',', '.') : "-";
						$O1 = ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
						$P1 = (($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])), 0, ',', '.') : "-";
						
						$Q1 = ($dados1[0]["rsspactuadocusteio"]==0) ? "-" : (($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) / $dados1[0]["rsspactuadocusteio"] != 0) ? number_format((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) / $dados1[0]["rsspactuadocusteio"]), 0, ',', '.') : "-";
						$R1 = ($dados1[0]["rsspactuadoinvest"]==0) ? "-" : (($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) / $dados1[0]["rsspactuadoinvest"] != 0) ? number_format((($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"]) / $dados1[0]["rsspactuadoinvest"]), 0, ',', '.') : "-";
						$S1 = ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]==0) ? "-" : ((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])) / ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"]) != 0) ? number_format(((($dados1[0]["rsspactuadocusteio"] - $dados1[0]["rssdisponibilizadocusteio"]) + ($dados1[0]["rsspactuadoinvest"] - $dados1[0]["rssdisponibilizadoinvest"])) / ($dados1[0]["rsspactuadocusteio"] + $dados1[0]["rsspactuadoinvest"])), 0, ',', '.') : "-";
			
						echo '<tr bgcolor="#c0c0c0" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
								<td colspan="2" align="left"><b>TOTAL</b></td>
							  	<!-- Primeira Tabela -->
							  	<td align="right"><b>'.$D1.'</b></td>
							  	<td align="right"><b>'.$E1.'</b></td>
							  	<td align="right"><b>'.$F1.'</b></td>
							  	<td align="right"><b>'.$G1.'</b></td>
							  	<td align="right"><b>'.$H1.'</b></td>
							  	<td align="right"><b>'.$I1.'</b></td>
							  	<td align="right"><b>'.$J1.'</b></td>
							  	<td align="right"><b>'.$K1.'</b></td>
							  	<td align="right"><b>'.$L1.'</b></td>
							  	<td align="right"><b>'.$M1.'</b></td>
							  	<td align="right"><b>'.$N1.'</b></td>
							  	<td align="right"><b>'.$O1.'</b></td>
							  	<td align="right"><b>'.$P1.'</b></td>
							  	<td align="right"><b>'.$Q1.'</b></td>
							  	<td align="right"><b>'.$R1.'</b></td>
							  	<td align="right"><b>'.$S1.'</b></td>
							  </tr>';*/
			
						for($z=0; $z<count($dados); $z++) {
							if($z%2==0)
								$zebra = "#cecece"; 
							else
								$zebra = "#FFFFFF";
							// Primeira Tabela
							$D = (($dados[$z]["rsspactuadocusteio"] && $dados[$z]["rsspactuadocusteio"] != 0) ? number_format($dados[$z]["rsspactuadocusteio"], 0, ',', '.') : "-");
							$E = (($dados[$z]["rsspactuadoinvest"] && $dados[$z]["rsspactuadoinvest"] != 0) ? number_format($dados[$z]["rsspactuadoinvest"], 0, ',', '.') : "-");
							$F = ($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"] != 0) ? number_format(($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"]), 0, ',', '.') : "-";
							
							//$G = (($dados[$z]["rsspactuadobolsas"] && $dados[$z]["rsspactuadobolsas"] != 0) ? number_format($dados[$z]["rsspactuadobolsas"], 0, ',', '.') : "-");
							$H = (($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"]) + $dados[$z]["rsspactuadobolsas"] != 0) ? number_format((($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"]) + $dados[$z]["rsspactuadobolsas"]), 0, ',', '.') : "-";
							
							$I = (($dados[$z]["rssdisponibilizadocusteio"] && $dados[$z]["rssdisponibilizadocusteio"] != 0) ? number_format($dados[$z]["rssdisponibilizadocusteio"], 0, ',', '.') : "-");
							$J = (($dados[$z]["rssdisponibilizadoinvest"] && $dados[$z]["rssdisponibilizadoinvest"] != 0) ? number_format($dados[$z]["rssdisponibilizadoinvest"], 0, ',', '.') : "-");
							$K = ($dados[$z]["rssdisponibilizadocusteio"] + $dados[$z]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados[$z]["rssdisponibilizadocusteio"] + $dados[$z]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
							
							//$L = (($dados[$z]["rssdisponibilizadobolsas"] && $dados[$z]["rssdisponibilizadobolsas"] != 0) ? number_format($dados[$z]["rssdisponibilizadobolsas"], 0, ',', '.') : "-");
							
							$M = (($dados[$z]["rssdisponibilizadocusteio"] + $dados[$z]["rssdisponibilizadoinvest"]) + $dados[$z]["rssdisponibilizadobolsas"] != 0) ? number_format((($dados[$z]["rssdisponibilizadocusteio"] + $dados[$z]["rssdisponibilizadoinvest"]) + $dados[$z]["rssdisponibilizadobolsas"]), 0, ',', '.') : "-";
							$N = ($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"] != 0) ? number_format(($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]), 0, ',', '.') : "-";
							$O = ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"] != 0) ? number_format(($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"]), 0, ',', '.') : "-";
							$P = (($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) + ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"]) != 0) ? number_format((($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) + ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"])), 0, ',', '.') : "-";
							
							$Q = ($dados[$z]["rsspactuadocusteio"]==0) ? "-" : (($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) / $dados[$z]["rsspactuadocusteio"] != 0) ? number_format(($dados[$z]["rsspactuadocusteio"]==0) ? "-" : (($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) / $dados[$z]["rsspactuadocusteio"]), 0, ',', '.') : "-";
							$R = ($dados[$z]["rsspactuadoinvest"]==0) ? "-" : (($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"]) / $dados[$z]["rsspactuadoinvest"] != 0) ? number_format(($dados[$z]["rsspactuadoinvest"]==0) ? "-" : (($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"]) / $dados[$z]["rsspactuadoinvest"]), 0, ',', '.') : "-";
							
							$S = (($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"])==0) ? "-" : (((($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) + ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"])) / ($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"]) != 0) ? number_format(((($dados[$z]["rsspactuadocusteio"] - $dados[$z]["rssdisponibilizadocusteio"]) + ($dados[$z]["rsspactuadoinvest"] - $dados[$z]["rssdisponibilizadoinvest"])) / ($dados[$z]["rsspactuadocusteio"] + $dados[$z]["rsspactuadoinvest"])), 0, ',', '.') : "-");
							/*
							<td width="5.75%" align="right">'.(strlen($G)>1&&substr($G, 0,1)=='-'?'<font color="red">':'').$G.'</td>
							<td width="5.75%" align="right">'.(strlen($L)>1&&substr($L, 0,1)=='-'?'<font color="red">':'').$L.'</td>
							*/
							echo '<tr bgcolor="'.$zebra.'" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\''.$zebra.'\';">
									<td width="4%" align="left">'.$dados[$z]["rssunicod"].'</td>
								  	<td width="4%" align="left">'.$dados[$z]["rssunidsc"].'</td>
								  	<!-- Primeira Tabela -->
								  	<td width="5.75%" align="right">'.(strlen($D)>1&&substr($D, 0,1)=='-'?'<font color="red">':'').$D.'</td>
								  	<td width="5.75%" align="right">'.(strlen($E)>1&&substr($E, 0,1)=='-'?'<font color="red">':'').$E.'</td>
								  	<td width="5.75%" align="right">'.(strlen($F)>1&&substr($F, 0,1)=='-'?'<font color="red">':'').$F.'</td>
								  	
								  	<td width="5.75%" align="right">'.(strlen($H)>1&&substr($H, 0,1)=='-'?'<font color="red">':'').$H.'</td>
								  	<td width="5.75%" align="right">'.(strlen($I)>1&&substr($I, 0,1)=='-'?'<font color="red">':'').$I.'</td>
								  	<td width="5.75%" align="right">'.(strlen($J)>1&&substr($J, 0,1)=='-'?'<font color="red">':'').$J.'</td>
								  	<td width="5.75%" align="right">'.(strlen($K)>1&&substr($K, 0,1)=='-'?'<font color="red">':'').$K.'</td>
								  	
								  	<td width="5.75%" align="right">'.(strlen($M)>1&&substr($M, 0,1)=='-'?'<font color="red">':'').$M.'</td>
								  	<td width="5.75%" align="right">'.(strlen($N)>1&&substr($N, 0,1)=='-'?'<font color="red">':'').$N.'</td>
								  	<td width="5.75%" align="right">'.(strlen($O)>1&&substr($O, 0,1)=='-'?'<font color="red">':'').$O.'</td>
								  	<td width="5.75%" align="right">'.(strlen($P)>1&&substr($P, 0,1)=='-'?'<font color="red">':'').$P.'</td>
								  	<td width="5.75%" align="right">'.(strlen($Q)>1&&substr($Q, 0,1)=='-'?'<font color="red">':'').$Q.'</td>
								  	<td width="5.75%" align="right">'.(strlen($R)>1&&substr($R, 0,1)=='-'?'<font color="red">':'').$R.'</td>
								  	<td width="5.75%" align="right">'.(strlen($S)>1&&substr($S, 0,1)=='-'?'<font color="red">':'').$S.'</td>
								  </tr>';
						}
					?>
					</table>
				</div>
			</td>
		</tr>
	</tbody>
</table>
<br />
<hr size="1" width="100%">

<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td><hr size="1" ></hr></td>
	</tr>
	<tr>
		<td align="left"><strong><font color="red">*1</font> OBS:</strong> COLUNA: "Diferença (Disponibilizado - Pactuado)" apresenta resultados positivos e/ou negativos com a seguinte leitura:<br> 
																			a) NEGATIVOS: valor pactuado previsto para exercício futuro e recebido pela instituição antecipadamente;<br>
																			b) POSITIVOS: valor pactuado até o presente exercício, não recebido e programado para futuro repasse à instituição.</td>
	</tr>
	<tr>
		<td align="left"><strong><font color="red">**1</font> OBS:</strong> COLUNA: "Pactuado Devido" demonstra somente o percentual de pactuação devido. As instituições com valores antecipados dispensam demonstrativo nesta coluna.</td>
	</tr>
	<tr>
		<td align="left"><strong><font color="red">*2</font> OBS:</strong> Os recursos REUNI pactuados para Bolsas de Doutorado, Mestrado e Pós-Doutorado, foram alocados em ação da CAPES cujo demonstrativo de Execução encontra-se em relatório específico.</td>
	</tr>
	<tr>
		<td align="left"><strong><font color="red">*3</font> OBS:</strong> *3: UNB-26271: Em 2007 a SESu descentralizou da ação-8551 para a UNB-26271 R$ 30.742.529,31 como antecipação à 1ª parcela Reuni de 2008 - Processo nº 030326/2007-41, NC: 1832, Portaria: 1085, publicação 
DOU: 31/12/2007. A instituição alocou indevidamente o recurso em sua fundação de apoio e, por esta razão o TCU determinou a devolução, conf. MEMO DAF 23/2009, de 22abril2009, UNBDOC-37484/2009, 22abril2009, 
Notas Empenhos 2007NE008203/008205.  Em 2008 houve repactuação do Plano de Trabalho para ajustar a distribuição dos valores entre os exercícios de 2008 a 2012, sem alteração do valor total originalmente pactuado.</td>
	</tr>
</table>