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
				<table style="width:98.8%;" align="left" border="0" cellspacing="2" cellpadding="2" class="listagem">
					<thead>
						<tr style="height:30px;">
							<!-- Segunda Tabela -->
							<th valign="top" colspan="18" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>RECURSOS ALOCADOS DIRETO NA UO</b></td>
						</tr>
						<tr style="height:50px;">
							<!-- Segunda Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>SIAFI - Dotação Autorizada</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>SIAFI - Créd. Especial NÃO EMP exerc. Anter.</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>EMENDAS</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL Disponibilizado na UO</strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Segunda Tabela -->
							<th width="4%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>UO</strong></td>
							<th width="4%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IFES</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa</font></strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br>Reaberto</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa</font></strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Segunda Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">T<br>siafi "UO exec09"</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">U<br>siafi "UO exec09"</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">V<br>siafi "UO exec09"</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">W<br>T + U + V</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">X<br>siafi "UO exec09"</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">Y<br>siafi "UO exec09"</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">Z<br>siafi "UO exec09"</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AA<br>X + Y + Z</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AB<br>site Câmara</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AC<br>site Câmara</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AD<br>site Câmara</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AE<br>AB + AC + AD</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AF<br>T - X - AB</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AG<br>U - Y - AC</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AH<br>V - Z - AD</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AI<br>AF + AG + AH</font></strong></td>
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
						// Segunda Tabela
						$T1 = (($dados1[0]["rssdotacaoautorizadacusteio"] && $dados1[0]["rssdotacaoautorizadacusteio"] != 0) ? number_format($dados1[0]["rssdotacaoautorizadacusteio"], 0, ',', '.') : "-");
						$U1 = (($dados1[0]["rssdotacaoautorizadainvest"] && $dados1[0]["rssdotacaoautorizadainvest"] != 0) ? number_format($dados1[0]["rssdotacaoautorizadainvest"], 0, ',', '.') : "-");
						$V1 = (($dados1[0]["rssdotacaoautorizadainversao"] && $dados1[0]["rssdotacaoautorizadainversao"] != 0) ? number_format($dados1[0]["rssdotacaoautorizadainversao"], 0, ',', '.') : "-");
						$W1 = ($dados1[0]["rssdotacaoautorizadacusteio"] + $dados1[0]["rssdotacaoautorizadainvest"] + $dados1[0]["rssdotacaoautorizadainversao"] != 0) ? number_format(($dados1[0]["rssdotacaoautorizadacusteio"] + $dados1[0]["rssdotacaoautorizadainvest"] + $dados1[0]["rssdotacaoautorizadainversao"]), 0, ',', '.') : "-";
						
						$X1 = (($dados1[0]["rsscreditoespecialcusteio"] && $dados1[0]["rsscreditoespecialcusteio"] != 0) ? number_format($dados1[0]["rsscreditoespecialcusteio"], 0, ',', '.') : "-");
						$Y1 = (($dados1[0]["rsscreditoespecialinvest"] && $dados1[0]["rsscreditoespecialinvest"] != 0) ? number_format($dados1[0]["rsscreditoespecialinvest"], 0, ',', '.') : "-");
						$Z1 = (($dados1[0]["rsscreditoespecialinversao"] && $dados1[0]["rsscreditoespecialinversao"] != 0) ? number_format($dados1[0]["rsscreditoespecialinversao"], 0, ',', '.') : "-");
						$AA1 = ($dados1[0]["rsscreditoespecialcusteio"] + $dados1[0]["rsscreditoespecialinvest"] + $dados1[0]["rsscreditoespecialinversao"] != 0) ? number_format(($dados1[0]["rsscreditoespecialcusteio"] + $dados1[0]["rsscreditoespecialinvest"] + $dados1[0]["rsscreditoespecialinversao"]), 0, ',', '.') : "-";
						
						$AB1 = (($dados1[0]["rssemendascusteio"] && $dados1[0]["rssemendascusteio"] != 0) ? number_format($dados1[0]["rssemendascusteio"], 0, ',', '.') : "-");
						$AC1 = (($dados1[0]["rssemendasinvest"] && $dados1[0]["rssemendasinvest"] != 0) ? number_format($dados1[0]["rssemendasinvest"], 0, ',', '.') : "-");
						$AD1 = (($dados1[0]["rssemendasinversao"] && $dados1[0]["rssemendasinversao"] != 0) ? number_format($dados1[0]["rssemendasinversao"], 0, ',', '.') : "-");
						$AE1 = ($dados1[0]["rssemendascusteio"] + $dados1[0]["rssemendasinvest"] + $dados1[0]["rssemendasinversao"] != 0) ? number_format(($dados1[0]["rssemendascusteio"] + $dados1[0]["rssemendasinvest"] + $dados1[0]["rssemendasinversao"]), 0, ',', '.') : "-";
						
						$AF1 = ($dados1[0]["rssdotacaoautorizadacusteio"] - $dados1[0]["rsscreditoespecialcusteio"] - $dados1[0]["rssemendascusteio"] != 0) ? number_format(($dados1[0]["rssdotacaoautorizadacusteio"] - $dados1[0]["rsscreditoespecialcusteio"] - $dados1[0]["rssemendascusteio"]), 0, ',', '.') : "-";
						$AG1 = ($dados1[0]["rssdotacaoautorizadainvest"] - $dados1[0]["rsscreditoespecialinvest"] - $dados1[0]["rssemendasinvest"] != 0) ? number_format(($dados1[0]["rssdotacaoautorizadainvest"] - $dados1[0]["rsscreditoespecialinvest"] - $dados1[0]["rssemendasinvest"]), 0, ',', '.') : "-";
						$AH1 = ($dados1[0]["rssdotacaoautorizadainversao"] - $dados1[0]["rsscreditoespecialinversao"] - $dados1[0]["rssemendasinversao"] != 0) ? number_format(($dados1[0]["rssdotacaoautorizadainversao"] - $dados1[0]["rsscreditoespecialinversao"] - $dados1[0]["rssemendasinversao"]), 0, ',', '.') : "-";
						$AI1 = (($dados1[0]["rssdotacaoautorizadacusteio"] - $dados1[0]["rsscreditoespecialcusteio"] - $dados1[0]["rssemendascusteio"]) + ($dados1[0]["rssdotacaoautorizadainvest"] - $dados1[0]["rsscreditoespecialinvest"] - $dados1[0]["rssemendasinvest"]) + ($dados1[0]["rssdotacaoautorizadainversao"] - $dados1[0]["rsscreditoespecialinversao"] - $dados1[0]["rssemendasinversao"]) != 0) ? number_format((($dados1[0]["rssdotacaoautorizadacusteio"] - $dados1[0]["rsscreditoespecialcusteio"] - $dados1[0]["rssemendascusteio"]) + ($dados1[0]["rssdotacaoautorizadainvest"] - $dados1[0]["rsscreditoespecialinvest"] - $dados1[0]["rssemendasinvest"]) + ($dados1[0]["rssdotacaoautorizadainversao"] - $dados1[0]["rsscreditoespecialinversao"] - $dados1[0]["rssemendasinversao"])), 0, ',', '.') : "-";
						
						echo '<tr bgcolor="#c0c0c0" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
								<td colspan="2" align="left"><b>TOTAL</b></td>
							  	<!-- Segunda Tabela -->
							  	<td align="right"><b>'.$T1.'</b></td>
							  	<td align="right"><b>'.$U1.'</b></td>
							  	<td align="right"><b>'.$V1.'</b></td>
							  	<td align="right"><b>'.$W1.'</b></td>
							  	<td align="right"><b>'.$X1.'</b></td>
							  	<td align="right"><b>'.$Y1.'</b></td>
							  	<td align="right"><b>'.$Z1.'</b></td>
							  	<td align="right"><b>'.$AA1.'</b></td>
							  	<td align="right"><b>'.$AB1.'</b></td>
							  	<td align="right"><b>'.$AC1.'</b></td>
							  	<td align="right"><b>'.$AD1.'</b></td>
							  	<td align="right"><b>'.$AE1.'</b></td>
							  	<td align="right"><b>'.$AF1.'</b></td>
							  	<td align="right"><b>'.$AG1.'</b></td>
							  	<td align="right"><b>'.$AH1.'</b></td>
							  	<td align="right"><b>'.$AI1.'</b></td>
							  </tr>';
			
						for($z=0; $z<count($dados); $z++) {
							// Segunda Tabela
							$T = (($dados[$z]["rssdotacaoautorizadacusteio"] && $dados[$z]["rssdotacaoautorizadacusteio"] != 0) ? number_format($dados[$z]["rssdotacaoautorizadacusteio"], 0, ',', '.') : "-");
							$U = (($dados[$z]["rssdotacaoautorizadainvest"] && $dados[$z]["rssdotacaoautorizadainvest"] != 0) ? number_format($dados[$z]["rssdotacaoautorizadainvest"], 0, ',', '.') : "-");
							$V = (($dados[$z]["rssdotacaoautorizadainversao"] && $dados[$z]["rssdotacaoautorizadainversao"] != 0) ? number_format($dados[$z]["rssdotacaoautorizadainversao"], 0, ',', '.') : "-");
							$W = ($dados[$z]["rssdotacaoautorizadacusteio"] + $dados[$z]["rssdotacaoautorizadainvest"] + $dados[$z]["rssdotacaoautorizadainversao"] != 0) ? number_format(($dados[$z]["rssdotacaoautorizadacusteio"] + $dados[$z]["rssdotacaoautorizadainvest"] + $dados[$z]["rssdotacaoautorizadainversao"]), 0, ',', '.') : "-";
							
							$X = (($dados[$z]["rsscreditoespecialcusteio"] && $dados[$z]["rsscreditoespecialcusteio"] != 0) ? number_format($dados[$z]["rsscreditoespecialcusteio"], 0, ',', '.') : "-");
							$Y = (($dados[$z]["rsscreditoespecialinvest"] && $dados[$z]["rsscreditoespecialinvest"] != 0) ? number_format($dados[$z]["rsscreditoespecialinvest"], 0, ',', '.') : "-");
							$Z = (($dados[$z]["rsscreditoespecialinversao"] && $dados[$z]["rsscreditoespecialinversao"] != 0) ? number_format($dados[$z]["rsscreditoespecialinversao"], 0, ',', '.') : "-");
							$AA = ($dados[$z]["rsscreditoespecialcusteio"] + $dados[$z]["rsscreditoespecialinvest"] + $dados[$z]["rsscreditoespecialinversao"] != 0) ? number_format(($dados[$z]["rsscreditoespecialcusteio"] + $dados[$z]["rsscreditoespecialinvest"] + $dados[$z]["rsscreditoespecialinversao"]), 0, ',', '.') : "-";
							
							$AB = (($dados[$z]["rssemendascusteio"] && $dados[$z]["rssemendascusteio"] != 0) ? number_format($dados[$z]["rssemendascusteio"], 0, ',', '.') : "-");
							$AC = (($dados[$z]["rssemendasinvest"] && $dados[$z]["rssemendasinvest"] != 0) ? number_format($dados[$z]["rssemendasinvest"], 0, ',', '.') : "-");
							$AD = (($dados[$z]["rssemendasinversao"] && $dados[$z]["rssemendasinversao"] != 0) ? number_format($dados[$z]["rssemendasinversao"], 0, ',', '.') : "-");
							$AE = ($dados[$z]["rssemendascusteio"] + $dados[$z]["rssemendasinvest"] + $dados[$z]["rssemendasinversao"] != 0) ? number_format(($dados[$z]["rssemendascusteio"] + $dados[$z]["rssemendasinvest"] + $dados[$z]["rssemendasinversao"]), 0, ',', '.') : "-";
							
							$AF = ($dados[$z]["rssdotacaoautorizadacusteio"] - $dados[$z]["rsscreditoespecialcusteio"] - $dados[$z]["rssemendascusteio"] != 0) ? number_format(($dados[$z]["rssdotacaoautorizadacusteio"] - $dados[$z]["rsscreditoespecialcusteio"] - $dados[$z]["rssemendascusteio"]), 0, ',', '.') : "-";
							$AG = ($dados[$z]["rssdotacaoautorizadainvest"] - $dados[$z]["rsscreditoespecialinvest"] - $dados[$z]["rssemendasinvest"] != 0) ? number_format(($dados[$z]["rssdotacaoautorizadainvest"] - $dados[$z]["rsscreditoespecialinvest"] - $dados[$z]["rssemendasinvest"]), 0, ',', '.') : "-";
							$AH = ($dados[$z]["rssdotacaoautorizadainversao"] - $dados[$z]["rsscreditoespecialinversao"] - $dados[$z]["rssemendasinversao"] != 0) ? number_format(($dados[$z]["rssdotacaoautorizadainversao"] - $dados[$z]["rsscreditoespecialinversao"] - $dados[$z]["rssemendasinversao"]), 0, ',', '.') : "-";
							$AI = (($dados[$z]["rssdotacaoautorizadacusteio"] - $dados[$z]["rsscreditoespecialcusteio"] - $dados[$z]["rssemendascusteio"]) + ($dados[$z]["rssdotacaoautorizadainvest"] - $dados[$z]["rsscreditoespecialinvest"] - $dados[$z]["rssemendasinvest"]) + ($dados[$z]["rssdotacaoautorizadainversao"] - $dados[$z]["rsscreditoespecialinversao"] - $dados[$z]["rssemendasinversao"]) != 0) ? number_format((($dados[$z]["rssdotacaoautorizadacusteio"] - $dados[$z]["rsscreditoespecialcusteio"] - $dados[$z]["rssemendascusteio"]) + ($dados[$z]["rssdotacaoautorizadainvest"] - $dados[$z]["rsscreditoespecialinvest"] - $dados[$z]["rssemendasinvest"]) + ($dados[$z]["rssdotacaoautorizadainversao"] - $dados[$z]["rsscreditoespecialinversao"] - $dados[$z]["rssemendasinversao"])), 0, ',', '.') : "-";
							
							echo '<tr bgcolor="#f9f9f9" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
									<td width="4%" align="left">'.$dados[$z]["rssunicod"].'</td>
								  	<td width="4%" align="left">'.$dados[$z]["rssunidsc"].'</td>
								  	<!-- Segunda Tabela -->
								  	<td width="5.75%" align="right">'.$T.'</td>
								  	<td width="5.75%" align="right">'.$U.'</td>
								  	<td width="5.75%" align="right">'.$V.'</td>
								  	<td width="5.75%" align="right">'.$W.'</td>
								  	<td width="5.75%" align="right">'.$X.'</td>
								  	<td width="5.75%" align="right">'.$Y.'</td>
								  	<td width="5.75%" align="right">'.$Z.'</td>
								  	<td width="5.75%" align="right">'.$AA.'</td>
								  	<td width="5.75%" align="right">'.$AB.'</td>
								  	<td width="5.75%" align="right">'.$AC.'</td>
								  	<td width="5.75%" align="right">'.$AD.'</td>
								  	<td width="5.75%" align="right">'.$AE.'</td>
								  	<td width="5.75%" align="right">'.$AF.'</td>
								  	<td width="5.75%" align="right">'.$AG.'</td>
								  	<td width="5.75%" align="right">'.$AH.'</td>
								  	<td width="5.75%" align="right">'.$AI.'</td>
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