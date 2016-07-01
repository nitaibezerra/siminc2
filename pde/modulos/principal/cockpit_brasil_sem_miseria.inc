<?PHP
include_once APPRAIZ . 'pde/www/_funcoes_cockpit.php';

permissaoPerfilConsultaCockpit( $_SESSION['favurl'] );
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>
<body onload="refreshAutomatico();">
<style>
  	.fundo_td{background-color:#0F6D39}
  	.titulo_pagina{font-weight:bold;font-size:20px;color:#FFFFFF}
  	.titulo_box{font-weight:bold;font-size:18px;color:#FFFFFF;margin-top:15px;text-shadow:#000000 0px 1px 2px}
  	.subtitulo_box{font-weight:normal;font-size:10px;color:#FFFFFF}
  	.fundo_td:hover {background-color:#0D8845}
  	.fundo_td{text-align:left;vertical-align:top;}
  	.tabela_painel{font-weight:bold;font-size:8px;color:#FFFFFF;font-family:fantasy}
  	.lista_metas{float:left}
  	#busca{background: none repeat scroll 0% 0% rgb(255, 255, 255); width:400px;border-width: 1px; border-style: solid; border-color: rgb(204, 204, 204) rgb(153, 153, 153) rgb(153, 153, 153) rgb(204, 204, 204); color: rgb(0, 0, 0); font: 18px arial,sans-serif bold; height: 35px;}
  	.tabela_box{color:#FFFFFF;}
  	.tabela_box td{background-color:#3CB371;text-shadow:#000000 0px 2px 2px}
  	.tabela_box_azul td{background-color:#63B8FF;text-shadow:#000000 0px 2px 2px;color:#FFFFFF;}
  	.fundo_td_azul{background-color:#2B86EE}
  	.fundo_td_azul:hover{background-color:#01A2D8}
   	.fundo_td_laranja{background-color:#EE9200}
  	.fundo_td_laranja:hover{background-color:#EBB513}
  	.fundo_td_roxo{background-color:#5333AD}
  	.fundo_td_roxo:hover{background-color:#6A5ACD}
  	.fundo_td_azul_escuro{background-color:#152D56}
  	.fundo_td_azul_escuro:hover{background-color:#1F3864}
  	.div_fotos{background-color:#204481;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
  	 body{background-image:url('../imagens/degrade-fundo-preto.png');background-repeat:repeat-x;background-color:#00466A;margin:0px;padding-top:0px;}
  	.fundo_titulo{background-image:url('../imagens/cockpit/fundo_pronacampo.jpg');background-repeat:repeat-x;background-position:2px -10px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
  	.numero{text-align:right}
  	.center{text-align:center}
  	.titulo_box a{color:#FFFFFF;text-decoration:none;}
  	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
  	.div_fotos_interno{width:98%;height:85px}
  	.bold{font-weight:bold}
  	.esquerda{text-align:left;}
  	.link{cursor:pointer}
</style>

<script>
 	jQuery.noConflict();
	jQuery('.div_fotos_interno').cycle({
	    fx: 'scrollDown'
	});

	atualizaUsuario();
</script>

<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" >SIMEC<br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_brasil_sem_miseria&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
	<!-- Título-->
	<tr>
		<td class="fundo_titulo" style="text-align:center" colspan="4" ><div style="margin:28px" >Brasil sem Miséria</div></td>
	</tr>
	<!-- Fim Título-->
	<tr>
		<td width="25%" class="fundo_td" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/Presentation.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Painéis estratégicos</div>
			</div>
			<div style="clear:both;text-align:center;width:100%" >
				<table class="tabela_box link" cellpadding="2" cellspacing="1" width="80%" align="center" >
					<tr onclick="window.location.href='estrategico.php?modulo=principal/cockpit_mulheres_mil&acao=A'" >
						<td class="bold" >Mulheres Mil</td>
					</tr>
					<tr onclick="window.location.href='estrategico.php?modulo=principal/cockpit_mais_educacao&acao=A'" >
						<td class="bold" >Mais Educação</td>
					</tr>
					<tr onclick="window.location.href='estrategico.php?modulo=principal/cockpit_bolsa_formacao&acao=A'" >
						<td class="bold" >Bolsa-Formação</td>
					</tr>
				</table>
			</div>
		</td>

		<td width="50%" class="fundo_td" style="text-align:left;vertical-align:top;" colspan="2">
			<div>
				<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Metas</div>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" colspan="2">&nbsp;</td>
					<td class="center bold" colspan="4">Execução</td>
				</tr>
				<tr>
                	<td class="center bold" >Programas</td>
                	<td class="center bold" ><i>Meta até 2014</i></td>
                	<td class="center bold" >2011</td>
                	<td class="center bold" >2012</td>
                	<td class="center bold" >2013</td>
                	<td class="center bold" >Total</td>
				</tr>
				<?php
				$sql = "--Mais Educação
						select dpe.dpeanoref, SUM(dsh.dshqtde) AS Total
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						where i.indid in (761)
						and sehstatus <> 'I'
						and dpe.dpeanoref in ('2011', '2012', '2013')
						group by dpe.dpeanoref
						order by dpe.dpeanoref";
				$arrExecutado = $db->carregar($sql);
				$possui2013=false;
				?>
				<tr class="link" onclick="abreIndicadorPopUp(761);">
					<td class="" >Mais Educação (escolas que aderiram)</td>
					<td class="numero" ><i><?php echo number_format(30000,0,",",".") ?></i></td>
					<?php foreach($arrExecutado as $executado): ?>
						<td class="numero" ><?php echo number_format($executado['total'],0,",",".") ?></td>
					<?php
						$totalMais = $executado['total'];//Indicador acumulativo não soma.
						if($executado['dpeanoref']=='2013'){
							$possui2013=true;
						}
					endforeach;
					if(!$possui2013){
					?>
						<td class="numero" ><?php echo number_format(0,0,",",".") ?></td>
					<?php
					}
					?>
					<td class="numero" ><?php echo number_format($totalMais,0,",",".") ?></td>
				</tr>
				<?php
				$sql = "--Bolsa-Formação Inclusão Produtiva
						select dpe.dpeanoref, SUM(dsh.dshqtde) AS Total
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						where i.indid in (752)
						and tidid1 IN (2860, 2863) --Pré-matrículas ativas e Matrículas
						and tidid2 = 2865 --MDS
						and sehstatus <> 'I'
						and dpe.dpeanoref in ('2011', '2012', '2013')
						group by dpe.dpeanoref
						order by dpe.dpeanoref";
				$arrExecutado = $db->carregar($sql);
				$possui2013=false;
				?>
				<tr class="link" onclick="abreIndicadorPopUp(752);">
                	<td class="" >Bolsa-Formação Inclusão Produtiva  (matrículas e pré-matrículas)</td>
                	<td class="numero" ><i><?php echo number_format(1000000,0,",",".") ?></i></td>
                	<?php
					if($arrExecutado){
						foreach($arrExecutado as $executado): ?>
							<td class="numero" ><?php echo number_format($executado['total'],0,",",".") ?></td>
						<?php
							$totalBolsa += $executado['total'];
							if($executado['dpeanoref']=='2013'){
								$possui2013=true;
							}
						endforeach;
					}
					if(!$possui2013){
					?>
						<td class="numero" ><?php echo number_format(0,0,",",".") ?></td>
					<?php
					}
					?>
					<td class="numero" ><?php echo number_format($totalBolsa,0,",",".") ?></td>
				</tr>
				<?php
				$sql = "--Mulheres Mil (matrículas)
						select tid.tiddsc, SUM(dsh.dshqtde) AS Total
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
						where i.indid in (728)
						and sehstatus <> 'I'
						and tid.tiddsc in ('2011', '2012', '2013') -- Ciclos
						group by tid.tiddsc
						order by tid.tiddsc";
				$arrExecutado = $db->carregar($sql);
				$possui2013=false;
				?>
				<tr class="link" onclick="abreIndicadorPopUp(728);">
                	<td class="" >Mulheres Mil (matrículas)</td>
                	<td class="numero" ><i><?php echo number_format(100000,0,",",".") ?></i></td>
                	<?php foreach($arrExecutado as $executado): ?>
						<td class="numero" ><?php echo number_format($executado['total'],0,",",".") ?></td>
					<?php
						$totalMulheres += $executado['total'];
						if($executado['tiddsc']=='2013'){
							$possui2013=true;
						}
					endforeach;
					if(!$possui2013){
					?>
						<td class="numero" ><?php echo number_format(0,0,",",".") ?></td>
					<?php
					}
					?>
					<td class="numero" ><?php echo number_format($totalMulheres,0,",",".") ?></td>
				</tr>
			</table>
		</td>
		<td width="25%" class="fundo_td_azul_escuro" style="vertical-align:top;" rowspan="2" >
            <div>
				<img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;" />
				<div style="float:left;cursor:pointer;" class="titulo_box" >Mais Educação:<br>atividades 2012</div>
			</div>
			<div style="clear:both" id="div_fotos" >
				<?php
				$sql = "SELECT arqid, entid, memid, entnome, mundescricao, estuf, qtde_aluno from (
						select
						me.entid,
						me.memid,
						mun.estuf,
						mun.muncod,
						mun.mundescricao,
						ent.entnome,
						esd.esddsc,
						sum( map.mapquantidade) as qtde_aluno,
						(select arqid from pdeinterativo.galeriafoto gal where gal.pdeid = pde.pdeid and gfostatus = 'A' limit 1) as arqid
						from pdeescola.memaiseducacao me
						inner join entidade.entidade ent on ent.entid = me.entid
						inner join entidade.endereco ende on ende.entid = ent.entid
						inner join territorios.municipio mun on mun.muncod = ende.muncod
						inner join workflow.documento doc on doc.docid = me.docid
						inner join workflow.estadodocumento esd on esd.esdid = doc.esdid
						inner join pdeescola.mealunoparticipante map on map.memid = me.memid
						inner join pdeinterativo.pdinterativo pde ON pde.pdicodinep::integer = ent.entcodent::integer
						where memanoreferencia = 2012 and memstatus = 'A' and esd.esdid >= 34 -- igual ou além de finalizado
						and ent.entcodent IN (select distinct dshcod
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (761)
											and dpe.dpeanoref = '2012')
						group by mun.estuf,mun.muncod,ent.entnome,me.memid,me.entid,esd.esddsc,mun.mundescricao,pde.pdeid
						) foo
						WHERE arqid IS NOT NULL
						AND entid NOT IN (186415, 175848)
						ORDER BY random()
						LIMIT 50" ;
				$arrFotos = $db->carregar($sql);
				?>
				<?php for($x=5;$x>=1;$x--): ?>
					<div style="clear:both" class="div_fotos_interno" >
					<?php for($i=$x;$i<=(10+$x);$i++): ?>
						<div class="div_fotos">
						<table border="0" >
							<tr>
								<td>
									<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=pdeinterativo" />
								</td>
								<td style="color:#FFFFFF" onclick="acessarMaisEducacao('<?=$arrFotos[$i]['entid'] ?>','<?=$arrFotos[$i]['memid'] ?>');" >
									<b><?php echo $arrFotos[$i]['entnome'] ?></b><br/>
									<b>Município:</b> <?php echo $arrFotos[$i]['mundescricao'] ?>/<?php echo $arrFotos[$i]['estuf'] ?><br/>
									<b>Alunos participantes:</b> <?php echo $arrFotos[$i]['qtde_aluno'] ?><br/>
								</td>
							</tr>
						</table>
						</div>
					<?php endfor; ?>
					</div>
				<?php endfor; ?>
			</div>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" width="25%">
		<?php
			montaTabelaEstrategico('Mais Educação 2012/2013', 140635);
		?>
		</td>

		<td class="fundo_td link" width="50%" style="text-align:left;vertical-align:top;" colspan="2"  onclick="abreIndicadorPopUp(752);">
			<div>
				<img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
				<div style="float:left;" class="titulo_box" >Bolsa-Formação: esforço total - MDS</div>
				<div style="clear:both;" >
					<?php
					$sql = "select tidid, tiddsc, SUM(totalativas) as ativas, SUM(totalperdidas) as perdidas, SUM(totalmatriculas) as confirmadas, tidid2, dpeanoref
							from (
								select tid.tidid, tid.tiddsc, SUM(dsh.dshqtde) AS totalativas, 0 AS totalperdidas, 0 AS totalmatriculas, dsh.tidid2, dpe.dpeanoref
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
								inner join painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
								where i.indid in (752)
								and sh.sehstatus <> 'I'
								--and dpe.dpeanoref = '2012'
								and dsh.tidid1 = 2863 --Pré-matrículas ativas
								group by tid.tidid, tid.tiddsc, dsh.tidid2, dpe.dpeanoref
							union all
								select tid.tidid, tid.tiddsc, 0 AS totalativas, SUM(dsh.dshqtde) AS totalperdidas, 0 AS totalmatriculas, dsh.tidid2, dpe.dpeanoref
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
								inner join painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
								where i.indid in (752)
								and sh.sehstatus <> 'I'
								--and dpe.dpeanoref = '2012'
								and dsh.tidid1 = 2861 --Pré-matrículas perdidas
								group by tid.tidid, tid.tiddsc, dsh.tidid2, dpe.dpeanoref
							union all
								select tid.tidid, tid.tiddsc, 0 AS totalativas, 0 AS totalperdidas, SUM(dsh.dshqtde) AS totalmatriculas, dsh.tidid2, dpe.dpeanoref
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe ON dpe.dpeid = sh.dpeid
								inner join painel.detalhetipodadosindicador tid ON tid.tidid = dsh.tidid2
								where i.indid in (752)
								and sh.sehstatus <> 'I'
								--and dpe.dpeanoref = '2012'
								and dsh.tidid1 = 2860 --Matrículas
								group by tid.tidid, tid.tiddsc, dsh.tidid2, dpe.dpeanoref
							) as FOO
							where tidid2 = 2865 --MDS
							group by tidid, tiddsc, tidid2, dpeanoref
							order by dpeanoref, tiddsc";
					$arrDemandante = $db->carregar($sql);
					?>
					<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
						<tr>
							<td class="center bold" >Ano</td>
							<td class="center bold" >Pré-Matrículas<br>Ativas</td>
							<td class="center bold" >Pré-Matrículas<br>Perdidas</td>
							<td class="center bold" >Matrículas<br>Confirmadas</td>
						</tr>
						<?php
						if($arrDemandante){
							foreach($arrDemandante as $demandante): ?>
								<tr>
									<td class="numero" ><?php echo $demandante['dpeanoref'] ?></td>
									<td class="numero" ><?php echo number_format($demandante['ativas'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($demandante['perdidas'],0,",",".") ?></td>
									<td class="numero" ><?php echo number_format($demandante['confirmadas'],0,",",".") ?></td>
								</tr>
								<?php
								$total_ativas+=$demandante['ativas'];
								$total_perdidas+=$demandante['perdidas'];
								$total_confirmadas+=$demandante['confirmadas'];
							endforeach;
						}
						?>
					</table>
				</div>
			</div>
		</td>
	</tr>
	<tr>
		<td class="fundo_td" >
		<?php
			montaTabelaEstrategico('Mais Educação 2013/2014', 140859);
		?>
		</td>

		<td width="25%" class="fundo_td">
			<div onclick="abreIndicadorPopUp(761);" >
				<?php
				$sql = "select SUM(dsh.dshqtde) AS qtde, count(distinct dshcodmunicipio) as totalmun
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						where i.indid in (761)
						and sehstatus <> 'I'
						and dpe.dpeanoref = '2012'
						LIMIT 1";
				$arrValores = $db->pegaLinha($sql);
				?>
                <img style="float:left" src="../imagens/icones/icons/mapas.png" style="vertical-align:middle;"  />
				<div style="float:left;cursor:pointer;" class="titulo_box" >Mais Educação: <br/> Brasil sem Miséria<br/><span class="subtitulo_box" ><a href="#" >Clique no mapa para visualizar os detalhes.</a></span>
				<br /><br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?> </span><span class="subtitulo_box" >escolas que aderiram ao Mais Educação</span>
				<br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['totalmun'],0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
				</div>
				<img class="link" onclick="abreIndicadorPopUp(761);" src="../imagens/cockpit/mapa_cockpit.png"  />
			</div>
		</td>
		<td width="25%" class="fundo_td">
			<div onclick="abreIndicadorPopUp(728);" >
				<?php
				$sql = "select sum(dsh.dshqtde) as qtde, count(distinct dshcodmunicipio) as totalmun
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					where i.indid in (728)
					and sh.sehstatus <> 'I'";
				$arrValores = $db->pegaLinha($sql);
				?>
                <img style="float:left" src="../imagens/icones/icons/mapas.png" style="vertical-align:middle;"  />
				<div style="float:left;cursor:pointer;" class="titulo_box" >Mulheres Mil<br/><span class="subtitulo_box" ><a href="#" >Clique no mapa para visualizar os detalhes.</a></span>
				<br /><br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['qtde'],0,",",".") ?> </span><span class="subtitulo_box" >matrículas</span>
				<br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['totalmun'],0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
				</div>
				<img class="link" onclick="abreIndicadorPopUp(728);" src="../imagens/cockpit/mapa_cockpit.png"  />
			</div>
		</td>

		<td width="25%" class="fundo_td" style="cursor:pointer;" onclick="abreIndicadorPopUp(728);">
			<div>
				<img style="float:left" src="../imagens/icones/icons/executive.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Mulheres Mil:<br>matrículas por ciclo</div>
			</div>
			<?php
			$sql = "select tid.tiddsc, sum(dsh.dshqtde) as total
					from painel.indicador i
					inner join painel.seriehistorica sh on sh.indid=i.indid
					inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
					inner join painel.detalhetipodadosindicador tid on tid.tidid = dsh.tidid1
					where i.indid in (728)
					and sh.sehstatus <> 'I'
					group by tid.tiddsc
					order by tid.tiddsc";
			$arrMat = $db->carregar($sql);
			$arrMat = !$arrMat ? array() : $arrMat;
			?>
			<div style="clear:both;text-align:center;width:100%" >
				<table class="tabela_box" cellpadding="2" cellspacing="1" width="90%" align="center" >
					<tr>
						<td class="center bold" >Ciclo</td>
						<td class="center bold" >Matrículas</td>
					</tr>
					<?php foreach($arrMat as $mat) : ?>
						<tr>
							<td align="left" ><?php echo str_replace('2011','2011/2012', str_replace('2012','2012/2013',$mat['tiddsc'])) ?></td>
							<td class="numero" ><?php echo number_format($mat['total'],0,"",".") ?></td>
						</tr>
					<?php endforeach; ?>
				</table>
			</div>
		</td>
	</tr>
	<tr>
		<td class="fundo_td_laranja" colspan="4">
		    <div style="text-align:center;"  >
		    <img src="../imagens/icones/icons/executiverel.png"  style="vertical-align:middle;"  />
		    <input type="text" onclick="this.style.color='#000000';this.value='';"  name="busca" size="61" maxlength="60" value="Digite aqui o que você procura" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);if(this.value==''){this.style.color='#D3D3D3';this.value='Digite aqui o que você procura'}" id='busca' onkeyup='exibeBuscaRegionalizacaoEnter(event)' style='color:#D3D3D3;'    title='' class=' normal' />
		    <img src="../imagens/icones/icons/Find.png"  style="vertical-align:middle;width:35px;height:35px;cursor:pointer;" onclick="buscar(document.getElementById('busca').value);"  />
		    </div>
	    </td>
	</tr>
</table>
</body>
</html>