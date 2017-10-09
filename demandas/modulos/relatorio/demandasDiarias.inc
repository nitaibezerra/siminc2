<html>
<head>
		<title>Atendimento de demandas</title>
		<style type="text/css">
			
			@media print {.notprint { display: none }}

			@media screen {
			.notscreen { display: none;  }
			.div_rolagem{ overflow-x: auto; overflow-y: auto; width:19.5cm;height:350px;}
			.topo { position: absolute; top: 0px; margin: 0; padding: 5px; position: fixed; background-color: #ffffff;}
			}

			*{margin:0; padding:0; border:none; font-size:10px;font-family:Arial;}
			.alignRight{text-align:right !important;}
			.alignCenter{ text-align:center !important;}
			.alignLeft{text-align:left !important;}
			.bold{font-weight:bold !important;}
			.italic{font-style:italic !important;}
			.noPadding{padding:0;}
			.titulo{width:52px;}
			.tituloagrup{font-size:9px;}
			.titulolinha{font-size:9px;}
			.fundod{background-color: #d0d0d0;}
			.fundos{background-color: #E8E8E8;}
			
			#tabelaTitulos tr td, #tabelaTitulos tr th{border:2px solid black;border-left:none; border-right:none;}
			#orgao{margin:3px 0 0 0;}
			#orgao tr td{border:1px solid black;border-left:none;border-right:none;font-size:8px;}
			
			div.filtro { page-break-after: always; text-align: center; }
			
			table{width:98%;border-collapse:collapse;}
			th, td{font-weight:normal;padding:4px;vertical-align:top;border-bottom: solid 1px black;}
			thead{display:table-header-group;}
			table, tr{page-break-inside:avoid;}
			a{text-decoration:none;color:#3030aa;}
			a:hover{text-decoration:underline;color:#aa3030;}
			span.topo { position: absolute; top: 3px; margin: 0; padding: 5px; position: fixed; background-color: #f0f0f0; border: 1px solid #909090; cursor:pointer; }
			span.topo:hover { background-color: #d0d0d0; }
						
			
			
		</style>
		<script type="text/javascript" src="../includes/funcoes.js"></script>
		<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
		<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		
</head>
<body>
<table width="98%" border="0" cellpadding="0" cellspacing="0" align="center" >
		<tr bgcolor="#ffffff">
			<td width="4%" style="border-bottom: solid 0px black;">
				<img src="../imagens/brasao.gif" width="45" height="45" border="0">
			</td>
			<td style="border-bottom: solid 0px black;">
				<?php echo NOME_SISTEMA; ?><br/>
				Acompanhamento de Demandas<br/>
			</td>

			<td align="right" style="border-bottom: solid 0px black;">
				Impresso por: <b><? echo $_SESSION['usunome']; ?></b><br/>
				Hora da Impressão: <? echo date("d/m/Y  G:i:s"); ?>
			</td>
		</tr>
		<tr>
			<td colspan="3" align="center" style="border-bottom: solid 0px black;">
				<b><font style="font-size:14px;">Relatório de atendimento técnico</font></b>
			</td>
		</tr>
	</table> 
	
<?php


$sql =  "SELECT DISTINCT
		(CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
								 from demandas.pausademanda pp
								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
								 where pp.dmdid = d.dmdid) > 0 
							AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI')
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
	 								 where pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) is null 
							OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI')
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
 								 where pp.dmdid = d.dmdid
								 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
							THEN
								'<img src=\'../imagens/pause.gif\' border=0  title=\'Pausa\' align=\'absmiddle\'>'
						ELSE
							''
		 END) || d.dmdid AS id,
		 CASE 
		  	WHEN u.usunome != '' THEN upper(u.usunome) 
		  	ELSE upper(d.dmdnomedemandante)
		 END as demandante,
		 to_char(d.dmddatainclusao::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatainclusao,
		 replace(d.dmddsc, chr(13), '<br>') as descricao, 
		 --d.dmddsc as descricao,
		 --upper(unasigla)||' - '||unadescricao as setor, 	
		 upper(unasigla) as setor,
		 loc.lcadescricao as edificio,
		 aa.anddescricao AS andar,
		 d.dmdsalaatendimento as sala,
		 --'(' || u.usufoneddd || ') ' || u.usufonenum AS tel	
		 u.usufonenum AS tel,
		 to_char(d.dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') as dtprevfim,
		 p.pridsc,
		 p.priid
		  
		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
 								 --where doc2.esdid in (91,92,107,108)
								 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid		 
		 
		 WHERE d.usucpfexecutor = '{$_SESSION['usucpf']}'
		 AND doc.esdid in (91,92)
         AND ed.esdstatus = 'A' 
         --AND dmdidpausa IS NULL
         AND ( select count(pp.dmdid) 
			 from demandas.pausademanda pp
			 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
			 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
			 where pp.dmdid = d.dmdid) = 0
         order by p.priid desc
		 ";
//ver($sql,d);         
//$cabecalho = array( "Cód" , "Solicitante", "Data de Abertura", "Assunto", "<div style='width:355px'>Descrição</div>", "Setor","Edifício","Andar","Sala","Telefone");
//$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', '', '');
$demandas = $db->carregar( $sql );
$demandas = $demandas ? $demandas : array();
?>

<table width="98%" align="center" border="0"  cellpadding="3" cellspacing="0">
		<tr>
			<td colspan="2" align="left" style="border-bottom: solid 1px black;">
				<b><font style="font-size:14px;">Demandas Sem Pausa</font></b>
			</td>
		</tr>
</table>
<table width="98%" align="center" border="0"  cellpadding="3" cellspacing="0">
	<?php if ( count( $demandas ) ) : ?>
		<thead>
			<tr bgcolor="#dfdfdf" >
				<td align="center"><b>Cód.</b></td>
				<td align="center"><b>Prioridade</b></td>
				<td align="left"><b>Solicitante</b></td>
				<td align="center"><b>Data de Abertura</b></td>
				<td align="center"><b>Fim do Atendimento</b></td>
				<td align="left" width="55%"><b>Descrição</b></td>
				<td align="center"><b>Setor</b></td>
				<td align="center"><b>Edifício</b></td>
				<td align="center"><b>Andar</b></td>
				<td align="center"><b>Sala</b></td>
				<td align="center"><b>Telefone</b></td>
			</tr>
		</thead>
		<?php $cor = ''; ?>
		<?php foreach ( $demandas as $demanda ) : ?>
			<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
			<tr bgcolor="<?= $cor ?>"  onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
				<td align="center">
					<?echo $demanda['id'];?>
				</td>
				<td align="center">
					<?echo $demanda['pridsc'];?>
				</td>
				<td>
					<?echo $demanda['demandante'];?>
				</td>
				<td align="center">
					<?echo $demanda['dmddatainclusao'];?>
				</td>
				<td align="center">
					<?echo $demanda['dtprevfim'];?>
				</td>
				<td>
					<?echo $demanda['descricao'];?>
				</td>
				<td align="center">
					<?echo $demanda['setor'];?>
				</td>
				<td align="center">
					<?echo $demanda['edificio'];?>
				</td>
				<td align="center">
					<?echo $demanda['andar'];?>
				</td>
				<td align="center">
					<?echo $demanda['sala'];?>
				</td>
				<td align="center">
					<?echo $demanda['tel'];?>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else : ?>
		<tr>
			<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;" colspan="2">
				Não existem demandas.
			</td>
		</tr>
	<?php endif; ?>
</table>

<br><br><br>


<?php


$sql =  "SELECT DISTINCT
		(CASE 
					 	--WHEN dmdidpausa > 0 AND ( dttempopausa is null OR dttempopausa > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) THEN
					 	WHEN ( select count(pp.dmdid) 
								 from demandas.pausademanda pp
								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
								 where pp.dmdid = d.dmdid) > 0 
							AND ( ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI')
						 			 from demandas.pausademanda pp
	 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
	 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
	 								 where pp.dmdid = d.dmdid
									 group by dd.dmddatafimprevatendimento) is null 
							OR ( select to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI')
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
 								 where pp.dmdid = d.dmdid
								 group by dd.dmddatafimprevatendimento) > to_char(CURRENT_TIMESTAMP,'YYYY-MM-DD HH24:MI') ) 
							THEN
								'<img src=\'../imagens/pause.gif\' border=0  title=\'Pausa\' align=\'absmiddle\'>'
						ELSE
							''
		 END) || d.dmdid AS id,
		 CASE 
		  	WHEN u.usunome != '' THEN upper(u.usunome) 
		  	ELSE upper(d.dmdnomedemandante)
		 END as demandante,
		 to_char(d.dmddatainclusao::timestamp,'DD/MM/YYYY HH24:MI') AS dmddatainclusao,
		 replace(d.dmddsc, chr(13), '<br>') as descricao, 
		 --d.dmddsc as descricao,
		 --upper(unasigla)||' - '||unadescricao as setor, 	
		 upper(unasigla) as setor,
		 loc.lcadescricao as edificio,
		 aa.anddescricao AS andar,
		 d.dmdsalaatendimento as sala,
		 --'(' || u.usufoneddd || ') ' || u.usufonenum AS tel	
		 u.usufonenum AS tel,
		 to_char(d.dmddatafimprevatendimento::timestamp,'DD/MM/YYYY HH24:MI') as dtprevfim,
		 p.pridsc,
		 p.priid
		  
		 FROM
		 demandas.demanda d
		 LEFT JOIN demandas.tiposervico t ON t.tipid = d.tipid
		 LEFT JOIN demandas.origemdemanda od ON od.ordid = t.ordid
		 LEFT JOIN demandas.prioridade p ON p.priid = d.priid					 
		 LEFT JOIN workflow.documento doc ON doc.docid = d.docid
		 LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid	 
		 LEFT JOIN seguranca.usuario u ON u.usucpf = d.usucpfdemandante
		 LEFT JOIN seguranca.usuario u2 ON u2.usucpf = d.usucpfexecutor
		 LEFT JOIN demandas.localandaratendimento AS l ON l.laaid = d.laaid
		 LEFT JOIN demandas.andaratendimento aa on l.andid = aa.andid
		 LEFT JOIN  demandas.unidadeatendimento AS uni ON uni.unaid = d.unaid
		 LEFT JOIN  demandas.localatendimento AS loc ON loc.lcaid = l.lcaid
		 /*
		 LEFT JOIN ( select pp.dmdid, count(pp.dmdid) as dmdidpausa, to_char((dd.dmddatafimprevatendimento + sum(pp.pdmdatafimpausa-pp.pdmdatainiciopausa)),'YYYY-MM-DD HH24:MI') as dttempopausa 
					 			 from demandas.pausademanda pp
 								 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
 								 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
 								 --where doc2.esdid in (91,92,107,108)
								 group by dd.dmddatafimprevatendimento,pp.dmdid) ps ON ps.dmdid = d.dmdid		 
		 */
		 WHERE d.usucpfexecutor = '{$_SESSION['usucpf']}'
		 AND doc.esdid in (91,92)
         AND ed.esdstatus = 'A' 
         --AND dmdidpausa > 0
         AND ( select count(pp.dmdid) 
			 from demandas.pausademanda pp
			 inner join demandas.demanda dd ON dd.dmdid=pp.dmdid
			 inner join workflow.documento doc2 ON doc2.docid = dd.docid 
			 where pp.dmdid = d.dmdid) > 0
         order by p.priid desc
		 ";
//ver($sql,d);         
//$cabecalho = array( "Cód" , "Solicitante", "Data de Abertura", "Assunto", "<div style='width:355px'>Descrição</div>", "Setor","Edifício","Andar","Sala","Telefone");
//$db->monta_lista( $sql, $cabecalho, 500, 10, 'N', '', '');
$demandas = $db->carregar( $sql );
$demandas = $demandas ? $demandas : array();
?>

<table width="98%" align="center" border="0"  cellpadding="3" cellspacing="0">
		<tr>
			<td colspan="2" align="left" style="border-bottom: solid 1px black;">
				<b><font style="font-size:14px;">Demandas Com Pausa</font></b>
			</td>
		</tr>
</table>
<table width="98%" align="center" border="0"  cellpadding="3" cellspacing="0">
	<?php if ( count( $demandas ) ) : ?>
		<thead>
			<tr bgcolor="#dfdfdf" >
				<td align="center"><b>Cód.</b></td>
				<td align="center"><b>Prioridade</b></td>
				<td align="left"><b>Solicitante</b></td>
				<td align="center"><b>Data de Abertura</b></td>
				<td align="center"><b>Fim do Atendimento</b></td>
				<td align="left" width="55%"><b>Descrição</b></td>
				<td align="center"><b>Setor</b></td>
				<td align="center"><b>Edifício</b></td>
				<td align="center"><b>Andar</b></td>
				<td align="center"><b>Sala</b></td>
				<td align="center"><b>Telefone</b></td>
			</tr>
		</thead>
		<?php $cor = ''; ?>
		<?php foreach ( $demandas as $demanda ) : ?>
			<?php $cor = $cor == '#f5f5f5' ? '#fdfdfd' : '#f5f5f5' ; ?>
			<tr bgcolor="<?= $cor ?>"  onmouseout="this.style.backgroundColor='<?= $cor ?>';" onmouseover="this.style.backgroundColor='#ffffcc';">
				<td align="center">
					<?echo $demanda['id'];?>
				</td>
				<td align="center">
					<?echo $demanda['pridsc'];?>
				</td>
				<td>
					<?echo $demanda['demandante'];?>
				</td>
				<td align="center">
					<?echo $demanda['dmddatainclusao'];?>
				</td>
				<td align="center">
					<?echo $demanda['dtprevfim'];?>
				</td>
				<td>
					<?echo $demanda['descricao'];?>
				</td>
				<td align="center">
					<?echo $demanda['setor'];?>
				</td>
				<td align="center">
					<?echo $demanda['edificio'];?>
				</td>
				<td align="center">
					<?echo $demanda['andar'];?>
				</td>
				<td align="center">
					<?echo $demanda['sala'];?>
				</td>
				<td align="center">
					<?echo $demanda['tel'];?>
				</td>
			</tr>
		<?php endforeach; ?>
	<?php else : ?>
		<tr>
			<td style="text-align:center; padding:15px; background-color:#fafafa; color:#404040; font-weight:bold; font-size: 10px;" colspan="2">
				Não existem demandas.
			</td>
		</tr>
	<?php endif; ?>
</table>
		
</body>
</html>		
