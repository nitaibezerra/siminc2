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
  sum( round( ( coalesce(rssdotacaoautorizadacusteio,0) - coalesce(rsscreditoespecialcusteio,0) - coalesce(rssemendascusteio,0) ), 0) + round( ( coalesce(rssdotacaoautorizadainvest,0) - coalesce(rsscreditoespecialinvest,0) - coalesce(rssemendasinvest,0) ), 0) ) as rssportariatotal,
  
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
  sum( ( coalesce(rssdotacaoautorizadacusteio,0) - coalesce(rsscreditoespecialcusteio,0) - coalesce(rssemendascusteio,0) ) + ( coalesce(rssdotacaoautorizadainvest,0) - coalesce(rsscreditoespecialinvest,0) - coalesce(rssemendasinvest,0) ) ) as rssportariatotal,
  
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
							<!-- Terceira Tabela -->	
							<th valign="top" colspan="18" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><b>RECURSOS DESCENTRALIZADOS PELA SESu</b></td>
						</tr>
						<tr style="height:50px;">
							<!-- Terceira Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>	
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PORTARIAS - Notas Crédito Concedidas (siafi)</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>DEVOLUÇÕES - NCs Devolvidas (siafi)</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>SIAFI - Dotação Descentralizada Líquida</strong></td>
							<th valign="top" colspan="4" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL Disponibilizado por Descentralização</strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Terceira Tabela -->	
							<th width="4%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>UO</strong></td>
							<th width="4%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>IFES</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Investimento</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa</font></strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Invest.</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">Reaberto em 2009</font></strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Investimento</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br>exceto Bolsa</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Custeio</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Investimento</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Inversão Financeira</strong></td>
							<th width="5.75%" valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>TOTAL<br><font color="red">exceto Bolsa</font></strong></td>
						</tr>
						<tr style="height:60px;">
							<!-- Terceira Tabela -->
							<th valign="top" colspan="2" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';">&nbsp;</td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AJ<br>portarias<br>NC conced.</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AK<br>portarias<br>NC conced.</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AL<br>portarias<br>NC conced.</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AM<br>AF + AG</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AN<br>portarias<br>NC conced.</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AO<br>portarias<br>NC conced.</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AP<br>portarias<br>NC conced.</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AQ<br>AN + AO + AP</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AR<br>SIAFI</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AS<br>SIAFI</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AT<br>SIAFI</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AU<br>AR + AS + AT</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AV<br>AJ - AN</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AW<br>AK - AO</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AX<br>AL - AP</font></strong></td>
							<th valign="top" align="center" class="title" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;" onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong><font color="red">AY<br>AV + AW + AX</font></strong></td>
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
						// Terceira Tabela			
						$AJ1 = (($dados1[0]["rssportariacusteio"] && $dados1[0]["rssportariacusteio"] != 0) ? number_format($dados1[0]["rssportariacusteio"], 0, ',', '.') : "-");
						$AK1 = (($dados1[0]["rssportariainvest"] && $dados1[0]["rssportariainvest"] != 0) ? number_format($dados1[0]["rssportariainvest"], 0, ',', '.') : "-");
						$AL1 = (($dados1[0]["rssportariainversao"] && $dados1[0]["rssportariainversao"] != 0) ? number_format($dados1[0]["rssportariainversao"], 0, ',', '.') : "-");
						$AM1 = (($dados1[0]["rssportariatotal"] && $dados1[0]["rssportariatotal"] != 0) ? number_format($dados1[0]["rssportariatotal"], 0, ',', '.') : "-");
						
						$AN1 = (($dados1[0]["rssdevolucaocusteio"] && $dados1[0]["rssdevolucaocusteio"] != 0) ? number_format($dados1[0]["rssdevolucaocusteio"], 0, ',', '.') : "-");
						$AO1 = (($dados1[0]["rssdevolucaoinvest"] && $dados1[0]["rssdevolucaoinvest"] != 0) ? number_format($dados1[0]["rssdevolucaoinvest"], 0, ',', '.') : "-");
						$AP1 = (($dados1[0]["rssdevolucaoinversao"] && $dados1[0]["rssdevolucaoinversao"] != 0) ? number_format($dados1[0]["rssdevolucaoinversao"], 0, ',', '.') : "-");
						$AQ1 = ($dados1[0]["rssdevolucaocusteio"] + $dados1[0]["rssdevolucaoinvest"] + $dados1[0]["rssdevolucaoinversao"] != 0) ? number_format(($dados1[0]["rssdevolucaocusteio"] + $dados1[0]["rssdevolucaoinvest"] + $dados1[0]["rssdevolucaoinversao"]), 0, ',', '.') : "-";
						
						$AR1 = (($dados1[0]["rssdescentralizacaoliqcusteio"] && $dados1[0]["rssdescentralizacaoliqcusteio"] != 0) ? number_format($dados1[0]["rssdescentralizacaoliqcusteio"], 0, ',', '.') : "-");
						$AS1 = (($dados1[0]["rssdescentralizacaoliqinvest"] && $dados1[0]["rssdescentralizacaoliqinvest"] != 0) ? number_format($dados1[0]["rssdescentralizacaoliqinvest"], 0, ',', '.') : "-");
						$AT1 = (($dados1[0]["rssdescentralizacaoliqinversao"] && $dados1[0]["rssdescentralizacaoliqinversao"] != 0) ? number_format($dados1[0]["rssdescentralizacaoliqinversao"], 0, ',', '.') : "-");
						$AU1 = ($dados1[0]["rssdescentralizacaoliqcusteio"] + $dados1[0]["rssdescentralizacaoliqinvest"] + $dados1[0]["rssdescentralizacaoliqinversao"] != 0) ? number_format(($dados1[0]["rssdescentralizacaoliqcusteio"] + $dados1[0]["rssdescentralizacaoliqinvest"] + $dados1[0]["rssdescentralizacaoliqinversao"]), 0, ',', '.') : "-";
						
						$AV1 = ($dados1[0]["rssportariacusteio"] - $dados1[0]["rssdevolucaocusteio"] != 0) ? number_format(($dados1[0]["rssportariacusteio"] - $dados1[0]["rssdevolucaocusteio"]), 0, ',', '.') : "-";
						$AW1 = ($dados1[0]["rssportariainvest"] - $dados1[0]["rssdevolucaoinvest"] != 0) ? number_format(($dados1[0]["rssportariainvest"] - $dados1[0]["rssdevolucaoinvest"]), 0, ',', '.') : "-";
						$AX1 = ($dados1[0]["rssportariainversao"] - $dados1[0]["rssdevolucaoinversao"] != 0) ? number_format(($dados1[0]["rssportariainversao"] - $dados1[0]["rssdevolucaoinversao"]), 0, ',', '.') : "-";
						$AY1 = (($dados1[0]["rssportariacusteio"] - $dados1[0]["rssdevolucaocusteio"]) + ($dados1[0]["rssportariainvest"] - $dados1[0]["rssdevolucaoinvest"]) + ($dados1[0]["rssportariainversao"] - $dados1[0]["rssdevolucaoinversao"]) != 0) ? number_format((($dados1[0]["rssportariacusteio"] - $dados1[0]["rssdevolucaocusteio"]) + ($dados1[0]["rssportariainvest"] - $dados1[0]["rssdevolucaoinvest"]) + ($dados1[0]["rssportariainversao"] - $dados1[0]["rssdevolucaoinversao"])), 0, ',', '.') : "-";
			
						echo '<tr bgcolor="#c0c0c0" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
								<td colspan="2" align="left"><b>TOTAL</b></td>
							  	<!-- Terceira Tabela -->
							  	<td align="right"><b>'.$AJ1.'</b></td>
							  	<td align="right"><b>'.$AK1.'</b></td>
							  	<td align="right"><b>'.$AL1.'</b></td>
							  	<td align="right"><b>'.$AM1.'</b></td>
							  	<td align="right"><b>'.$AN1.'</b></td>
							  	<td align="right"><b>'.$AO1.'</b></td>
							  	<td align="right"><b>'.$AP1.'</b></td>
							  	<td align="right"><b>'.$AQ1.'</b></td>
							  	<td align="right"><b>'.$AR1.'</b></td>
							  	<td align="right"><b>'.$AS1.'</b></td>
							  	<td align="right"><b>'.$AT1.'</b></td>
							  	<td align="right"><b>'.$AU1.'</b></td>
							  	<td align="right"><b>'.$AV1.'</b></td>
							  	<td align="right"><b>'.$AW1.'</b></td>
							  	<td align="right"><b>'.$AX1.'</b></td>
							  	<td align="right"><b>'.$AY1.'</b></td>
							  </tr>';
			
						for($z=0; $z<count($dados); $z++) {
							// Terceira Tabela			
							$AJ = (($dados[$z]["rssportariacusteio"] && $dados[$z]["rssportariacusteio"] != 0) ? number_format($dados[$z]["rssportariacusteio"], 0, ',', '.') : "-");
							$AK = (($dados[$z]["rssportariainvest"] && $dados[$z]["rssportariainvest"] != 0) ? number_format($dados[$z]["rssportariainvest"], 0, ',', '.') : "-");
							$AL = (($dados[$z]["rssportariainversao"] && $dados[$z]["rssportariainversao"] != 0) ? number_format($dados[$z]["rssportariainversao"], 0, ',', '.') : "-");
							$AM = (($dados[$z]["rssportariatotal"] && $dados[$z]["rssportariatotal"] != 0) ? number_format($dados[$z]["rssportariatotal"], 0, ',', '.') : "-");
							
							$AN = (($dados[$z]["rssdevolucaocusteio"] && $dados[$z]["rssdevolucaocusteio"] != 0) ? number_format($dados[$z]["rssdevolucaocusteio"], 0, ',', '.') : "-");
							$AO = (($dados[$z]["rssdevolucaoinvest"] && $dados[$z]["rssdevolucaoinvest"] != 0) ? number_format($dados[$z]["rssdevolucaoinvest"], 0, ',', '.') : "-");
							$AP = (($dados[$z]["rssdevolucaoinversao"] && $dados[$z]["rssdevolucaoinversao"] != 0) ? number_format($dados[$z]["rssdevolucaoinversao"], 0, ',', '.') : "-");
							$AQ = ($dados[$z]["rssdevolucaocusteio"] + $dados[$z]["rssdevolucaoinvest"] + $dados[$z]["rssdevolucaoinversao"] != 0) ? number_format(($dados[$z]["rssdevolucaocusteio"] + $dados[$z]["rssdevolucaoinvest"] + $dados[$z]["rssdevolucaoinversao"]), 0, ',', '.') : "-";
							
							$AR = (($dados[$z]["rssdescentralizacaoliqcusteio"] && $dados[$z]["rssdescentralizacaoliqcusteio"] != 0) ? number_format($dados[$z]["rssdescentralizacaoliqcusteio"], 0, ',', '.') : "-");
							$AS = (($dados[$z]["rssdescentralizacaoliqinvest"] && $dados[$z]["rssdescentralizacaoliqinvest"] != 0) ? number_format($dados[$z]["rssdescentralizacaoliqinvest"], 0, ',', '.') : "-");
							$AT = (($dados[$z]["rssdescentralizacaoliqinversao"] && $dados[$z]["rssdescentralizacaoliqinversao"] != 0) ? number_format($dados[$z]["rssdescentralizacaoliqinversao"], 0, ',', '.') : "-");
							$AU = ($dados[$z]["rssdescentralizacaoliqcusteio"] + $dados[$z]["rssdescentralizacaoliqinvest"] + $dados[$z]["rssdescentralizacaoliqinversao"] != 0) ? number_format(($dados[$z]["rssdescentralizacaoliqcusteio"] + $dados[$z]["rssdescentralizacaoliqinvest"] + $dados[$z]["rssdescentralizacaoliqinversao"]), 0, ',', '.') : "-";
							
							$AV = ($dados[$z]["rssportariacusteio"] - $dados[$z]["rssdevolucaocusteio"] != 0) ? number_format(($dados[$z]["rssportariacusteio"] - $dados[$z]["rssdevolucaocusteio"]), 0, ',', '.') : "-";
							$AW = ($dados[$z]["rssportariainvest"] - $dados[$z]["rssdevolucaoinvest"] != 0) ? number_format(($dados[$z]["rssportariainvest"] - $dados[$z]["rssdevolucaoinvest"]), 0, ',', '.') : "-";
							$AX = ($dados[$z]["rssportariainversao"] - $dados[$z]["rssdevolucaoinversao"] != 0) ? number_format(($dados[$z]["rssportariainversao"] - $dados[$z]["rssdevolucaoinversao"]), 0, ',', '.') : "-";
							$AY = (($dados[$z]["rssportariacusteio"] - $dados[$z]["rssdevolucaocusteio"]) + ($dados[$z]["rssportariainvest"] - $dados[$z]["rssdevolucaoinvest"]) + ($dados[$z]["rssportariainversao"] - $dados[$z]["rssdevolucaoinversao"]) != 0) ? number_format((($dados[$z]["rssportariacusteio"] - $dados[$z]["rssdevolucaocusteio"]) + ($dados[$z]["rssportariainvest"] - $dados[$z]["rssdevolucaoinvest"]) + ($dados[$z]["rssportariainversao"] - $dados[$z]["rssdevolucaoinversao"])), 0, ',', '.') : "-";
							
							echo '<tr bgcolor="#f9f9f9" onmouseover="this.bgColor=\'#ffffcc\';" onmouseout="this.bgColor=\'#f9f9f9\';">
									<td width="4%" align="left">'.$dados[$z]["rssunicod"].'</td>
								  	<td width="4%" align="left">'.$dados[$z]["rssunidsc"].'</td>
								  	<!-- Terceira Tabela -->
								  	<td width="5.75%" align="right">'.$AJ.'</td>
								  	<td width="5.75%" align="right">'.$AK.'</td>
								  	<td width="5.75%" align="right">'.$AL.'</td>
								  	<td width="5.75%" align="right">'.$AM.'</td>
								  	<td width="5.75%" align="right">'.$AN.'</td>
								  	<td width="5.75%" align="right">'.$AO.'</td>
								  	<td width="5.75%" align="right">'.$AP.'</td>
								  	<td width="5.75%" align="right">'.$AQ.'</td>
								  	<td width="5.75%" align="right">'.$AR.'</td>
								  	<td width="5.75%" align="right">'.$AS.'</td>
								  	<td width="5.75%" align="right">'.$AT.'</td>
								  	<td width="5.75%" align="right">'.$AU.'</td>
								  	<td width="5.75%" align="right">'.$AV.'</td>
								  	<td width="5.75%" align="right">'.$AW.'</td>
								  	<td width="5.75%" align="right">'.$AX.'</td>
								  	<td width="5.75%" align="right">'.$AY.'</td>
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