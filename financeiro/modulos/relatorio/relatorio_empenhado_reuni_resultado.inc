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
  sum( round( ( coalesce(rssdotacaoautorizadainvest,0) - coalesce(rsscreditoespecialinvest,0) - coalesce(rssemendasinvest,0) ), 0) + round( ( coalesce(rssportariainvest,0) - coalesce(rssdevolucaoinvest,0) ), 0) ) as rssdisponibilizadoinvest,
  sum(rssdisponibilizadobolsas) as rssdisponibilizadobolsas
  
from financeiro.relatoriosesu s
--inner join entidade.entidade e ON e.entunicod = s.rssunicod
--inner join entidade.funcaoentidade fe ON fe.entid = e.entid
--where fe.funid = 12";
$dados1 = $db->carregar($sql);

$sql = "select
  rssunicod,
  rssunidsc,
  --rssunidsc,
  
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
  sum( ( coalesce(rssdotacaoautorizadainvest,0) - coalesce(rsscreditoespecialinvest,0) - coalesce(rssemendasinvest,0) ) + ( coalesce(rssportariainvest,0) - coalesce(rssdevolucaoinvest,0) ) ) as rssdisponibilizadoinvest,
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

<table width="95%">
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
				<table style="width:98.7%;" align="left" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<thead>
						<tr style="height:30px;">
							<!-- Quarta Tabela -->
							<th valign="top" colspan="14" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>EMPENHADO (os montantes empenhados incluem Emendas e Créditos)</b></td>
						</tr>
						<tr style="height:50px;">
							<!-- Quarta Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Direto na UO</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>DESCENTRALIZADO SESu</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL</strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Quarta Tabela -->
							<th width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>UO</strong></td>
							<th width="5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IFES</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Investimento</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa</font></strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Investimento</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa</font></strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Investimento</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="7.5%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br>exceto Bolsa</strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Quarta Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AZ<br>siafi</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BA<br>siafi</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BB<br>siafi</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BC<br>AZ + BA + BB</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BD<br>siafi</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BE<br>siafi</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BF<br>siafi</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BG<br>BD + BE + BF</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BH<br>AZ + BD</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BI<br>BA + BE</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BJ<br>BB + BF</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">BK<br>BH + BI + BJ</font></strong></td>
						</tr>
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
						// Quarta Tabela
						$AZ1 = (($dados1[0]["rssempenhadodiretouocusteio"] && $dados1[0]["rssempenhadodiretouocusteio"] != 0) ? number_format($dados1[0]["rssempenhadodiretouocusteio"], 0, ',', '.') : "-");
						$BA1 = (($dados1[0]["rssempenhadodiretouoinvest"] && $dados1[0]["rssempenhadodiretouoinvest"] != 0) ? number_format($dados1[0]["rssempenhadodiretouoinvest"], 0, ',', '.') : "-");
						$BB1 = (($dados1[0]["rssempenhadodiretouoinversao"] && $dados1[0]["rssempenhadodiretouoinversao"] != 0) ? number_format($dados1[0]["rssempenhadodiretouoinversao"], 0, ',', '.') : "-");
						$BC1 = ($dados1[0]["rssempenhadodiretouocusteio"] + $dados1[0]["rssempenhadodiretouoinvest"] + $dados1[0]["rssempenhadodiretouoinversao"] != 0) ? number_format(($dados1[0]["rssempenhadodiretouocusteio"] + $dados1[0]["rssempenhadodiretouoinvest"] + $dados1[0]["rssempenhadodiretouoinversao"]), 0, ',', '.') : "-";
						
						$BD1 = (($dados1[0]["rssempenhadodescentralizadocusteio"] && $dados1[0]["rssempenhadodescentralizadocusteio"] != 0) ? number_format($dados1[0]["rssempenhadodescentralizadocusteio"], 0, ',', '.') : "-");
						$BE1 = (($dados1[0]["rssempenhadodescentralizadoinvest"] && $dados1[0]["rssempenhadodescentralizadoinvest"] != 0) ? number_format($dados1[0]["rssempenhadodescentralizadoinvest"], 0, ',', '.') : "-");
						$BF1 = (($dados1[0]["rssempenhadodescentralizadoinversao"] && $dados1[0]["rssempenhadodescentralizadoinversao"] != 0) ? number_format($dados1[0]["rssempenhadodescentralizadoinversao"], 0, ',', '.') : "-");
						$BG1 = ($dados1[0]["rssempenhadodescentralizadocusteio"] + $dados1[0]["rssempenhadodescentralizadoinvest"] + $dados1[0]["rssempenhadodescentralizadoinversao"] != 0) ? number_format(($dados1[0]["rssempenhadodescentralizadocusteio"] + $dados1[0]["rssempenhadodescentralizadoinvest"] + $dados1[0]["rssempenhadodescentralizadoinversao"]), 0, ',', '.') : "-";
						
						$BH1 = ($dados1[0]["rssempenhadodiretouocusteio"] + $dados1[0]["rssempenhadodescentralizadocusteio"] != 0) ? number_format(($dados1[0]["rssempenhadodiretouocusteio"] + $dados1[0]["rssempenhadodescentralizadocusteio"]), 0, ',', '.') : "-";
						$BI1 = ($dados1[0]["rssempenhadodiretouoinvest"] + $dados1[0]["rssempenhadodescentralizadoinvest"] != 0) ? number_format(($dados1[0]["rssempenhadodiretouoinvest"] + $dados1[0]["rssempenhadodescentralizadoinvest"]), 0, ',', '.') : "-";
						$BJ1 = ($dados1[0]["rssempenhadodiretouoinversao"] + $dados1[0]["rssempenhadodescentralizadoinversao"] != 0) ? number_format(($dados1[0]["rssempenhadodiretouoinversao"] + $dados1[0]["rssempenhadodescentralizadoinversao"]), 0, ',', '.') : "-";
						$BK1 = ($dados1[0]["rssempenhadodiretouocusteio"] + $dados1[0]["rssempenhadodescentralizadocusteio"] + $dados1[0]["rssempenhadodiretouoinvest"] + $dados1[0]["rssempenhadodescentralizadoinvest"] + $dados1[0]["rssempenhadodiretouoinversao"] + $dados1[0]["rssempenhadodescentralizadoinversao"] != 0) ? number_format(($dados1[0]["rssempenhadodiretouocusteio"] + $dados1[0]["rssempenhadodescentralizadocusteio"] + $dados1[0]["rssempenhadodiretouoinvest"] + $dados1[0]["rssempenhadodescentralizadoinvest"] + $dados1[0]["rssempenhadodiretouoinversao"] + $dados1[0]["rssempenhadodescentralizadoinversao"]), 0, ',', '.') : "-";
			
						echo '<tr bgcolor="#c0c0c0" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
								<td colspan="2" align="left"><b>TOTAL</b></td>
							  	<!-- Quarta Tabela -->
							  	<td align="right"><b>'.$AZ1.'</b></td>
							  	<td align="right"><b>'.$BA1.'</b></td>
							  	<td align="right"><b>'.$BB1.'</b></td>
							  	<td align="right"><b>'.$BC1.'</b></td>
							  	<td align="right"><b>'.$BD1.'</b></td>
							  	<td align="right"><b>'.$BE1.'</b></td>
							  	<td align="right"><b>'.$BF1.'</b></td>
							  	<td align="right"><b>'.$BG1.'</b></td>
							  	<td align="right"><b>'.$BH1.'</b></td>
							  	<td align="right"><b>'.$BI1.'</b></td>
							  	<td align="right"><b>'.$BJ1.'</b></td>
							  	<td align="right"><b>'.$BK1.'</b></td>
							  </tr>';
			
						for($z=0; $z<count($dados); $z++) {
							// Quarta Tabela
							$AZ = (($dados[$z]["rssempenhadodiretouocusteio"] && $dados[$z]["rssempenhadodiretouocusteio"] != 0) ? number_format($dados[$z]["rssempenhadodiretouocusteio"], 0, ',', '.') : "-");
							$BA = (($dados[$z]["rssempenhadodiretouoinvest"] && $dados[$z]["rssempenhadodiretouoinvest"] != 0) ? number_format($dados[$z]["rssempenhadodiretouoinvest"], 0, ',', '.') : "-");
							$BB = (($dados[$z]["rssempenhadodiretouoinversao"] && $dados[$z]["rssempenhadodiretouoinversao"] != 0) ? number_format($dados[$z]["rssempenhadodiretouoinversao"], 0, ',', '.') : "-");
							$BC = ($dados[$z]["rssempenhadodiretouocusteio"] + $dados[$z]["rssempenhadodiretouoinvest"] + $dados[$z]["rssempenhadodiretouoinversao"] != 0) ? number_format(($dados[$z]["rssempenhadodiretouocusteio"] + $dados[$z]["rssempenhadodiretouoinvest"] + $dados[$z]["rssempenhadodiretouoinversao"]), 0, ',', '.') : "-";
							
							$BD = (($dados[$z]["rssempenhadodescentralizadocusteio"] && $dados[$z]["rssempenhadodescentralizadocusteio"] != 0) ? number_format($dados[$z]["rssempenhadodescentralizadocusteio"], 0, ',', '.') : "-");
							$BE = (($dados[$z]["rssempenhadodescentralizadoinvest"] && $dados[$z]["rssempenhadodescentralizadoinvest"] != 0) ? number_format($dados[$z]["rssempenhadodescentralizadoinvest"], 0, ',', '.') : "-");
							$BF = (($dados[$z]["rssempenhadodescentralizadoinversao"] && $dados[$z]["rssempenhadodescentralizadoinversao"] != 0) ? number_format($dados[$z]["rssempenhadodescentralizadoinversao"], 0, ',', '.') : "-");
							$BG = ($dados[$z]["rssempenhadodescentralizadocusteio"] + $dados[$z]["rssempenhadodescentralizadoinvest"] + $dados[$z]["rssempenhadodescentralizadoinversao"] != 0) ? number_format(($dados[$z]["rssempenhadodescentralizadocusteio"] + $dados[$z]["rssempenhadodescentralizadoinvest"] + $dados[$z]["rssempenhadodescentralizadoinversao"]), 0, ',', '.') : "-";
							
							$BH = ($dados[$z]["rssempenhadodiretouocusteio"] + $dados[$z]["rssempenhadodescentralizadocusteio"] != 0) ? number_format(($dados[$z]["rssempenhadodiretouocusteio"] + $dados[$z]["rssempenhadodescentralizadocusteio"]), 0, ',', '.') : "-";
							$BI = ($dados[$z]["rssempenhadodiretouoinvest"] + $dados[$z]["rssempenhadodescentralizadoinvest"] != 0) ? number_format(($dados[$z]["rssempenhadodiretouoinvest"] + $dados[$z]["rssempenhadodescentralizadoinvest"]), 0, ',', '.') : "-";
							$BJ = ($dados[$z]["rssempenhadodiretouoinversao"] + $dados[$z]["rssempenhadodescentralizadoinversao"] != 0) ? number_format(($dados[$z]["rssempenhadodiretouoinversao"] + $dados[$z]["rssempenhadodescentralizadoinversao"]), 0, ',', '.') : "-";
							$BK = ($dados[$z]["rssempenhadodiretouocusteio"] + $dados[$z]["rssempenhadodescentralizadocusteio"] + $dados[$z]["rssempenhadodiretouoinvest"] + $dados[$z]["rssempenhadodescentralizadoinvest"] + $dados[$z]["rssempenhadodiretouoinversao"] + $dados[$z]["rssempenhadodescentralizadoinversao"] != 0) ? number_format(($dados[$z]["rssempenhadodiretouocusteio"] + $dados[$z]["rssempenhadodescentralizadocusteio"] + $dados[$z]["rssempenhadodiretouoinvest"] + $dados[$z]["rssempenhadodescentralizadoinvest"] + $dados[$z]["rssempenhadodiretouoinversao"] + $dados[$z]["rssempenhadodescentralizadoinversao"]), 0, ',', '.') : "-";
							
							echo '<tr bgcolor="#f9f9f9" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
									<td width="5%" align="left">'.$dados[$z]["rssunicod"].'</td>
								  	<td width="5%" align="left">'.$dados[$z]["rssunidsc"].'</td>
								  	<!-- Quarta Tabela -->
								  	<td width="7.5%" align="right">'.$AZ.'</td>
								  	<td width="7.5%" align="right">'.$BA.'</td>
								  	<td width="7.5%" align="right">'.$BB.'</td>
								  	<td width="7.5%" align="right">'.$BC.'</td>
								  	<td width="7.5%" align="right">'.$BD.'</td>
								  	<td width="7.5%" align="right">'.$BE.'</td>
								  	<td width="7.5%" align="right">'.$BF.'</td>
								  	<td width="7.5%" align="right">'.$BG.'</td>
								  	<td width="7.5%" align="right">'.$BH.'</td>
								  	<td width="7.5%" align="right">'.$BI.'</td>
								  	<td width="7.5%" align="right">'.$BJ.'</td>
								  	<td width="7.5%" align="right">'.$BK.'</td>
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
<table width="100%" border="0" cellpadding="0" cellspacing="0">
	<tr>
		<td align="left"><strong><font color="red">*</font> OBS:</strong> Os recursos REUNI pactuados para bolsas de Doutorado, Mestrado e Pós-Doutorado, foram alocados em ação da CAPES.</td>
	</tr>
	<tr>
		<td align="left"><strong><font color="red">**</font> OBS:</strong> UNB-26271: A UNB emitiu em 2009 devolução referente ao projeto REUNI/2007 no valor de R$30.742.529,31 , conforme programação financeira (PF) emitida em 16/07/2009.</td>
	</tr>
	<tr>
		<td align="left"><strong><font color="red">***</font> OBS:</strong> Os valores de descentralização neste exercício estão em andamento. Estes dados foram atualizados pelo SIAFI Base: 14-SET-2009</td>
	</tr>
</table>