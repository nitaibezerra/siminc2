<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="X-UA-Compatible" content="IE=9" />
<meta http-equiv="Content-Type" content="text/html;  charset=ISO-8859-1">
<title>Sistema Integrado de Monitoramento Execu&ccedil;&atilde;o e Controle</title>
<body onload="refreshAutomatico();">
<?php
include APPRAIZ . 'pde/www/_funcoes_cockpit.php';
if( $_REQUEST['useronline'] ){
	echo pegaUsuarioOnline('99,15,23');
	exit;
}
?>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/jquery-cycle/jquery.cycle.all.js"></script>
<script language="javascript" type="text/javascript" src="js/estrategico.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/highcharts.js"></script>
<script language="javascript" src="../includes/Highcharts-3.0.0/js/modules/exporting.js"></script>

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
  	.fundo_td_vermelho{background-color:#BB0000}
  	.fundo_td_roxo{background-color:#5333AD}
  	.fundo_td_roxo:hover{background-color:#6A5ACD}
  	.fundo_td_vermelho:hover{background-color:#DD0000}
  	.div_fotos{background-color:#7B68EE;cursor:pointer;margin-bottom:3px;text-shadow:#000000 0px 1px 2px;width:350px;margin-bottom:2px}
  	body{background-image:url('../imagens/fundo_cockpit.jpg');background-repeat:repeat-x;background-color:#00466A;margin:0px;padding-top:0px;}
  	.fundo_titulo{background-image:url('../imagens/fundobrasilpro.jpg');background-repeat:repeat-xt;background-position:2px -50px;font-weight:bold;font-size:30px;color:#FFFFFF;text-shadow:#000000 0px 4px 2px;}
  	.numero{text-align:right}
  	.center{text-align:center}
  	.titulo_box a{color:#FFFFFF;text-decoration:none;}
  	.titulo_box a:hover{color:#FFFFFF;text-decoration:none;}
  	.div_fotos_interno{margin-bottom:2px;width:98%}
  	.bold{font-weight:bold}
  	.link{cursor:pointer}
 </style>
<script>

	jQuery.noConflict();

		jQuery('.div_fotos_interno').cycle({
		    fx: 'scrollDown'
		});

	atualizaUsuario();

	function abreMapaPainel()
	{
		window.open('/painel/painel.php?modulo=principal/mapas/mapaPadrao&acao=A&mapid=20&cockpit=1&id_104=checked','Indicadores','scrollbars=yes,height=768,width=1024,status=no,toolbar=no,menubar=no,location=no');
	}

</script>
<table border="0" align="center" width="100%" cellspacing="0" cellpadding="5" class="tabela_painel">
	<tr>
		<td class="titulo_pagina" >
		<div style="cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/atividade_estrategico/projetos&acao=A';">
            <img style="float:left" src="../imagens/icones/icons/control.png" style="vertical-align:middle;"  />
        	<div style="float:left" class="titulo_box" ><?php echo SIGLA_SISTEMA; ?><br/><span class="subtitulo_box" >Monitoramento Estratégico</span></div>
		</div>
		<img width="40px" style="float:right;cursor:pointer;" onclick="history.back(-1);"  src="cockpit/images/voltar.png" style="vertical-align:middle;" />
		<div style="float:right;cursor:pointer;" onclick="window.location='estrategico.php?modulo=principal/cockpit_pronatec&acao=A';">
			<img src="../imagens/icones/icons/Refresh.png" style="vertical-align:middle;" />
		</div>
		</td>
	</tr>
</table>
<table border="0" align="center" width="98%" cellspacing="4" cellpadding="5" class="tabela_painel">
  <!-- Título-->
  <tr>
                <td class="fundo_titulo" style="text-align:center" colspan="6" ><br>Pronatec<br><br></td>
  </tr>
  <!-- Fim Título-->
  <tr>
                <!-- Tabela Indicadores-->
                <td class="fundo_td" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/indicador.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" ><a href="#" >Indicadores</a><br/>
                			<span class="subtitulo_box" >Impacto | Produto | Processo</span>
                		</div>
                	</div>
                	<div style="clear:both;text-align:center;width:100%" >
	                	<table class="tabela_box link" cellpadding="2" cellspacing="1" width="80%" align="center" >
	                		<tr onclick="abreIndicadores(129397,'')" >
	                			<td class="bold" >Bolsa-Formação</td>
	                		</tr>
	                		<tr onclick="abreIndicadores(129399,'')" >
	                			<td class="bold" >Brasil Profissionalizado</td>
	                		</tr>
	                		<tr onclick="abreIndicadores(129373,'')" >
	                			<td class="bold" >Expansão da Rede Federal de EPT</td>
	                		</tr>
	                		<tr onclick="abreIndicadores(129326,'')" >
	                			<td class="bold" >Acordo de Gratuidade com Sistema S</td>
	                		</tr>
	                		<tr onclick="abreIndicadores(129347,'')" >
	                			<td class="bold" >E-Tec</td>
	                		</tr>
	                	</table>
                	</div>
                </td>
                <td class="fundo_td" >
                	<div style="cursor:pointer" >
                		<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" >Meta 2011-2014</div>
                	</div>
                	<div style="clear:both;text-align:center;text-shadow:#000000 0px 1px 2px" >
                		<span style="font-weight:bold;font-size:20px" >7.944.775</span><br/>vagas
                	</div>
                </td>
                <td class="fundo_td" >
                	<div style="cursor:pointer" >
                		<img style="float:left" src="../imagens/icones/icons/alvo.png" style="vertical-align:middle;"  />
                		<div style="float:left;cursor:pointer;" class="titulo_box" >Meta 2012</div>
                	</div>
                	<div style="clear:both;text-align:center;text-shadow:#000000 0px 1px 2px" >
                		<span style="font-weight:bold;font-size:20px" >1.656.348</span><br/>vagas
                	</div>
                </td>
                <td class="fundo_td">
                	<div style="clear:both" >
                		<img style="float:left;width: 50px;height: 50px" src="../imagens/icones/icons/executive.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Vagas</div>
                	</div>
                	<?php $sql = "--Vagas
									select SUM(dsh.dshqtde) AS total
									from painel.indicador i
									inner join painel.seriehistorica sh on sh.indid=i.indid
									inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
									inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
									where i.indid in (1084)
									and sehstatus <> 'I'
									and dpe.dpeanoref = '2012'";
                	$vagas = $db->pegaUm($sql); ?>
                	<div style="clear:both;text-align:center;text-shadow:#000000 0px 1px 2px;padding-top:10px" >
                		<span style="font-weight:bold;font-size:20px" ><?php echo number_format($vagas,0,",",".") ?></span><br/>em <?php echo date("d/m/Y") ?>
                	</div>
                </td>
                <!-- Fim Tabela Indicadores-->
                <!-- Tabela Lateral-->
                <td class="fundo_td_roxo" style="vertical-align:top;"  rowspan="5">
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/configs.png" style="vertical-align:middle;"  />
                		<div style="float:left;" class="titulo_box" >Obras da Expansão da <br/>Rede Federal de <br/>Educação Profissional, <br/>Científica e Tecnológica</div>
                	</div>
                	<div style="clear:both" id="div_fotos"  >
	                	<?php

	                	$sql = "SELECT obr.obrid, obr.obrdesc,
								ent.entnome AS unidade,
								mun.mundescricao AS municipio,
								mun.estuf AS uf,
								coalesce(obr.obrvlrrealobra,0) as valor,
								arqid
								FROM obras.obrainfraestrutura obr
								INNER JOIN entidade.entidade ent ON ent.entid = obr.entidunidade
								INNER JOIN obras.tipoobra tob ON tob.tobaid = obr.tobraid
								INNER JOIN obras.situacaoobra sto ON sto.stoid = obr.stoid
								INNER JOIN entidade.endereco edc ON edc.endid = obr.endid
								INNER JOIN territorios.municipio mun ON mun.muncod = edc.muncod
								INNER JOIN obras.fotos fot ON fot.obrid = obr.obrid
								WHERE obr.obsstatus = 'A'
								AND ((prfid = 40 AND tobaid = 1) OR (prfid = 34)) -- Brasil Profissionalizado e EXPANSÃO RFEPT
								AND obr.obrpercexec >= 50
								AND arqid IS NOT NULL
								ORDER BY random()
								LIMIT 50";
               				$arrFotos = $db->carregar($sql);
	                		?>
		                	<?php for($x=8;$x>=1;$x--): ?>
		                		<div style="clear:both" class="div_fotos_interno"  >
	                			<?php for($i=$x;$i<=(10+$x);$i++): ?>
		                			<div class="div_fotos" style="cursor:pointer;" onclick="acessarObras(<?php echo $arrFotos[$i]['obrid'] ?>);">
				                		<table border="0" >
				                			<tr>
				                				<td>
				                					<img width="80" height="60" src="../slideshow/slideshow/verimagem.php?arqid=<?php echo $arrFotos[$i]['arqid'] ?>&newwidth=100&newheight=85&_sisarquivo=obras" />
				                				</td>
				                				<td style="color:#FFFFFF" >
				                					<?php echo substr($arrFotos[$i]['obrdesc'],0,60) ?>...<br/>
													<?php echo substr($arrFotos[$i]['unidade'],0,60) ?>...<br/>
				                					<?php echo $arrFotos[$i]['municipio'] ?> / <?php echo $arrFotos[$i]['uf'] ?><br/>
													R$ <?php echo number_format($arrFotos[$i]['valor'],2,",",".") ?>
				                				</td>
				                			</tr>
				                		</table>
			                		</div>
		                		<?php endfor; ?>
	                			</div>
	                	<?php endfor; ?>
	                </div>
                </td>
                <!-- Fim Tabela Lateral-->
  </tr>
  <tr>
    <!-- Tabela Mapa -->
                <td class="fundo_td" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/Presentation.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Painéis estratégicos</div>
                	</div>
                	<div style="clear:both;text-align:center;width:100%" >
	                	<table class="tabela_box link" cellpadding="2" cellspacing="1" width="80%" align="center" >
	                		<tr onclick="window.location.href='estrategico.php?modulo=principal/cockpit_bolsa_formacao&acao=A'" >
	                			<td class="bold" >Bolsa-Formação</td>
	                		</tr>
	                		<tr onclick="window.location.href='estrategico.php?modulo=principal/cockpit_brasil_pro&acao=A'" >
	                			<td class="bold" >Brasil Profissionalizado</td>
	                		</tr>
	                		<tr onclick="window.location.href='estrategico.php?modulo=principal/cockpit_rede_federal&acao=A'" >
	                			<td class="bold" >Expansão da Rede Federal de EPT</td>
	                		</tr>
	                	</table>
                	</div>
                </td>
                <!-- Fim Tabela Mapa -->
                <!-- Obras -->
                <td class="fundo_td_azul" colspan="3" valign="top" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Bolsa-Formação</div>
                	</div>
                	<?php
					$sql = "select tipo, sum(meta) as meta2012, sum(pactuadas) as pactuadas2012, sum(ofertadas) as ofertadas2012, sum(matriculas) as matriculas2012 from (
								select case dsh.tidid1
									when 2692 then 'Bolsa-Formação - Cursos Técnicos'
									when 2693 then 'Bolsa-Formação - FIC'
									end as tipo,
									case dsh.tidid1
									when 2692 then 99149
									when 2693 then 590937
									end as meta,
									SUM(dsh.dshqtde) AS pactuadas,
									0 as ofertadas,
									0 as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (709)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								and dsh.tidid1 in (2692, 2693)
								group by tipo, meta
							union all
								select case dsh.tidid1
									when 2802 then 'Bolsa-Formação - Cursos Técnicos'
									when 2801 then 'Bolsa-Formação - FIC'
									end as tipo,
									0 as meta,
									0 as pactuadas,
									SUM(dsh.dshqtde) AS ofertadas,
									0 as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (733)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								and dsh.tidid1 in (2802, 2801)
								group by tipo
							union all
								select 'Bolsa-Formação - Cursos Técnicos' as tipo,
									0 as meta,
									0 as pactuadas,
									0 AS ofertadas,
									SUM(dsh.dshqtde) as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (739)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								group by tipo
							union all
								select 'Bolsa-Formação - FIC' as tipo,
									0 as meta,
									0 as pactuadas,
									0 AS ofertadas,
									SUM(dsh.dshqtde) as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (740)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								group by tipo
							) as foo
							group by tipo
							order by tipo";
					$arrVagas = $db->carregar($sql);
					$arrVagas = $arrVagas ? $arrVagas : array();
                	?>
                	<div style="clear:both;text-align:center;width:100%" >
	                	<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="80%" align="center" >
	                		<tr>
	                			<td class="bold" >2012</td>
	                			<td class="bold" >Meta</td>
	                			<td class="bold" >Vagas Pactuadas</td>
	                			<td class="bold" >Vagas Ofertadas</td>
	                			<td class="bold" >Matrículas</td>
	                		</tr>
	                		<?php foreach($arrVagas as $vag): ?>
								<?php
								switch ( $vag['tipo'] ){
									case 'Bolsa-Formação - FIC':
										$id=740;
									break;
									case 'Bolsa-Formação - Cursos Técnicos':
										$id=739;
									break;
								} ?>
	                			<tr>
		                			<td ><?php echo $vag['tipo'] ?></td>
		                			<td class="numero" ><?php echo number_format($vag['meta2012'],0,",",".") ?></td>
		                			<td class="numero" style="cursor:pointer;" onclick="abreIndicadorPopUp(709);"><?php echo number_format($vag['pactuadas2012'],0,",",".") ?></td>
		                			<td class="numero" style="cursor:pointer;" onclick="abreIndicadorPopUp(733);"><?php echo number_format($vag['ofertadas2012'],0,",",".") ?></td>
		                			<td class="numero" style="cursor:pointer;" onclick="abreIndicadorPopUp(<?php echo $id ?>);"><?php echo number_format($vag['matriculas2012'],0,",",".") ?></td>
		                		</tr>
	                		<?php endforeach; ?>
	                	</table>
                	</div>
                </td>
    			<!-- Fim Obras -->
  </tr>
  <tr>
  	 <td class="fundo_td" width="390" >
		<div onclick="abreMapaPainel()" >
			<?php
			$sql = "SELECT count(municipio) as mun,sum(vliqtd_191) as polos,sum(vliqtd_192) as alunos FROM (
					SELECT municipio,SUM(vliqtd_191) as vliqtd_191,SUM(vliqtd_192) as vliqtd_192 FROM (

					SELECT  ''||m.mundescricao||'' as municipio
						    , CASE WHEN di.dtiid=191 THEN COALESCE(vliqtd,0) ELSE 0 END as vliqtd_191,CASE WHEN di.dtiid=192 THEN COALESCE(vliqtd,0) ELSE 0 END as vliqtd_192
					FROM mapa.valorindicador q
					INNER JOIN mapa.detalheindicador di ON di.dtiid = q.dtiid
					INNER JOIN mapa.indicador ind ON ind.indid = di.indid
					INNER JOIN territoriosgeo.municipio m on m.muncod = q.muncod

					INNER JOIN territoriosgeo.estado e on e.estuf = m.estuf
					INNER JOIN territoriosgeo.regiao r on r.regcod = e.regcod
					WHERE m.munlatlong IS NOT NULL AND ind.indid in ('104') ) foo
					GROUP BY municipio) as foo2  ";
			$arrValores = $db->pegaLinha($sql);
			?>
                <img style="float:left" src="../imagens/icones/icons/mapas.png" style="vertical-align:middle;"  />
				<div style="float:left;cursor:pointer;" class="titulo_box" >Polos E-Tec
				<br /><br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['polos'],0,",",".") ?> </span><span class="subtitulo_box" >polos</span>
				<br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['alunos'],0,",",".") ?> </span><span class="subtitulo_box" >alunos</span>
			<br /><span style="font-weight:bold;font-size:14px" ><?php echo number_format($arrValores['mun'],0,",",".") ?> </span><span class="subtitulo_box" >municípios</span>
			</div>
			<img class="link" onclick="abreMapaPainel()" src="../imagens/cockpit/mapa_cockpit.png"  />
		</div>
	 </td>
	 <td class="fundo_td_azul" colspan="3" valign="top" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/recycle.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Demais iniciativas</div>
                	</div>
                	<?php
                	$sql = "--iniciativa/meta/Ingressantes/Matrículas
							select tipo, sum(meta) as meta2012, sum(Ingressantes) as ingressantes2012, sum(matriculas) as matriculas2012 from (
								select case dsh.tidid1
									when 3134 then 'Acordo de Gratuidade - Sistema S'
									when 3136 then 'E-Tec'
									when 3135 then 'Brasil Profissionalizado'
									when 3133 then 'Expansão da Rede Federal de EPT'
									end as tipo,
									0 as meta,
									SUM(dsh.dshqtde) AS Ingressantes,
									0 as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (1084)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								and tidid1 in (3133, 3136, 3134, 3135)
								group by tipo, meta
							union all
								select 'Acordo de Gratuidade - Sistema S' as tipo,
									76119 as meta,
									0 AS Ingressantes,
									SUM(dsh.dshqtde) as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (767)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								and tidid2 in (2938, 2939)
								group by tipo
							union all
								select 'E-Tec' as tipo,
									150000 as meta,
									0 AS Ingressantes,
									SUM(dsh.dshqtde) as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (222)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								group by tipo
							union all
								select 'Brasil Profissionalizado' as tipo,
									90563 as meta,
									0 AS Ingressantes,
									SUM(dsh.dshqtde) as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (715)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								group by tipo
							union all
								select 'Expansão da Rede Federal de EPT' as tipo,
									79560 as meta,
									0 AS Ingressantes,
									SUM(dsh.dshqtde) as matriculas
								from painel.indicador i
								inner join painel.seriehistorica sh on sh.indid=i.indid
								inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
								inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
								where i.indid in (571)
								and sehstatus <> 'I'
								and dpe.dpeanoref = '2012'
								and tidid2 = 1968
								group by tipo
							) as foo
							group by tipo
							order by tipo";
                	$arrVagas = $db->carregar($sql);
					$arrVagas = $arrVagas ? $arrVagas : array();
                	?>
                	<div style="clear:both;text-align:center;width:100%" >
	                	<table class="tabela_box_azul" cellpadding="2" cellspacing="1" width="80%" align="center" >
	                		<tr>
	                			<td class="bold" >Iniciativa</td>
	                			<td class="bold" >Meta</td>
	                			<td class="bold" >Ingressantes</td>
	                			<td class="bold" >Matrículas</td>
	                		</tr>
	                		<?php foreach($arrVagas as $vag): ?>
								<?php
								switch ( $vag['tipo'] ){
									case 'Acordo de Gratuidade - Sistema S':
										$id=767;
									break;
									case 'E-Tec':
										$id=222;
									break;
									case 'Brasil Profissionalizado':
										$id=715;
									break;
									case 'Expansão da Rede Federal de EPT':
										$id=571;
									break;
								} ?>
	                			<tr>
		                			<td class="" ><?php echo $vag['tipo'] ?></td>
		                			<td class="numero" ><?php echo number_format($vag['meta2012'],0,",",".") ?></td>
		                			<td class="numero" style="cursor:pointer;" onclick="abreIndicadorPopUp(1084);"><?php echo number_format($vag['ingressantes2012'],0,",",".") ?></td>
		                			<td class="numero" style="cursor:pointer;" onclick="abreIndicadorPopUp(<?php echo $id ?>);"><?php echo number_format($vag['matriculas2012'],0,",",".") ?></td>
		                		</tr>
	                		<?php endforeach; ?>
	                	</table>
                	</div>
                </td>
  </tr>
  <tr>
                <!-- Tabela Financeiro -->
                 <td class="fundo_td" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/mais.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Acordo de Gratuidade com Sistema S<br/><span class="subtitulo_box" >Matrículas 2012</span></div>
                	</div>
                	<div style="clear:both;text-align:center;width:100%" >
	                	<?php
						$sql = "select tiddsc, '' as geral, SUM(dsh.dshqtde) AS gratuidade
						from painel.indicador i
						inner join painel.seriehistorica sh on sh.indid=i.indid
						inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
						inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
						inner join painel.detalhetipodadosindicador tdi ON dsh.tidid1 = tdi.tidid
						where i.indid in (767)
						and sehstatus <> 'I'
						and dpe.dpeanoref = '2012'
						and tidid1 in (2936, 2937) --SENAI e SENAC
						group by tiddsc
						order by tiddsc";
						$arrS = $db->carregar($sql);
						$arrS = $arrS ? $arrS : array();
	                	$total_gratuidade=0;
						?>
	                	<table class="tabela_box" cellpadding="2" cellspacing="1" width="80%" align="center" >
	                		<tr>
	                			<td class="" ></td>
	                			<td class="bold center" >Geral</td>
	                			<td class="bold center" >Gratuidade</td>
	                		</tr>
	                		<?php foreach($arrS as $s): ?>
	                			<tr>
		                			<td class="" ><?php echo $s['tiddsc'] ?></td>
		                			<td class="numero" ><?php echo number_format($s['geral'],0,",",".") ?></td>
		                			<td class="numero" ><?php $total_gratuidade+=$s['gratuidade'];echo number_format($s['gratuidade'],0,",",".") ?></td>
		                		</tr>
	                		<?php endforeach; ?>
	                		<tr>
	                			<td class="bold" >Total</td>
	                			<td class="numero" ><?php echo number_format($total_geral,0,",",".") ?></td>
	                			<td class="numero" ><?php echo number_format($total_gratuidade,0,",",".") ?></td>
	                		</tr>
	                	</table>
                	</div>
                </td>
                <td class="fundo_td" colspan="3" rowspan="2" >
			<div>
				<img style="float:left" src="../imagens/icones/icons/financeiro.png" style="vertical-align:middle;"  />
				<div style="float:left" class="titulo_box" >Orçamentário / Financeiro</div>
			</div>
			<table class="tabela_box" cellpadding="2" cellspacing="1" width="100%" >
				<tr>
					<td class="center bold" rowspan="2" >Descrição</td>
					<td class="center bold" colspan="2" >2011</td>
					<td class="center bold" colspan="2" >2012</td>
				</tr>
				<tr>
					<!--<td class="center bold" >Dotação</td>-->
					<td class="center bold" >Valor empenhado</td>
					<td class="center bold" >Valor pago</td>
					<!--<td class="center bold" >Dotação</td>-->
					<td class="center bold" >Valor empenhado</td>
					<td class="center bold" >Valor pago</td>
				</tr>
				<?php
				// Parâmetros para a nova conexão com o banco do SIAFI
					$servidor_bd = $servidor_bd_siafi;
					$porta_bd    = $porta_bd_siafi;
					$nome_bd     = $nome_bd_siafi;
					$usuario_db  = $usuario_db_siafi;
					$senha_bd    = $senha_bd_siafi;

					// Cria o novo objeto de conexão
					$db2 = new cls_banco();

					// Parâmetros da nova conexão com o banco do SIAFI para o componente 'combo_popup'.
					$dados_conexao = array(
						 'servidor_bd' => $servidor_bd_siafi,
						 'porta_bd' => $porta_bd_siafi,
						 'nome_bd' => $nome_bd_siafi,
						 'usuario_db' => $usuario_db_siafi,
						 'senha_bd' => $senha_bd_siafi
						);

			   $sql = "select 	case
								when cod_agrupador1 in ('20RW') then 'Bolsa-Formação'
								when cod_agrupador1 in ('8652') then 'Brasil Profissionalizado'
								when cod_agrupador1 in ('20RG') then 'Expansão da Rede Federal de EPT'
								when cod_agrupador1 in ('8252', '20RH', '20RO' )  then 'Expansão da Rede Federal de EPT' end as descricao,
							sum(empenho2011) as empenhado2011, sum(pago2011) as pago2011,
							sum(empenho2012) as empenhado2012, sum(pago2012) as pago2012
						from (

						SELECT
										acacod AS cod_agrupador1,
										acadsc AS dsc_agrupador1,
										sum(valor1) AS empenho2011,sum(valor2) AS pago2011,
										0 AS empenho2012, 0 AS pago2012
										FROM
										(SELECT
										   sld.acacod,
										   aca.acadsc,
										   CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2
										   FROM
										   dw.saldo2011 sld
										   LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod
										   WHERE sld.acacod in ('20RG','20RW','8652') AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130301','292410403')) as foo
										WHERE
										valor1 <> 0 OR valor2 <> 0
										GROUP BY
										acacod,
										acadsc

						union all

						SELECT
										acacod AS cod_agrupador1,
										acadsc AS dsc_agrupador1,
										sum(valor1) AS empenho2011,sum(valor2) AS pago2011,
										0 AS empenho2012, 0 AS pago2012
										FROM
										(SELECT
										   sld.acacod,
										   aca.acadsc,
										   CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2
										   FROM
										   dw.saldo2011 sld
										   LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod
											WHERE sld.plicod in ('PFD01M06E1P','PFD01M3300P','PFD01M5700P','PFD01P9301N','RFD01B992DP','RFD01M331DP')
											AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130301','292410403')
											AND trim(sld.acacod) in ( '8252', '20RH', '20RO' )
											) as foo
										WHERE
										valor1 <> 0 OR valor2 <> 0
										GROUP BY
										acacod,
										acadsc

						union all

						/* 2012 */
						select * from (
						SELECT
										acacod AS cod_agrupador1,
										acadsc AS dsc_agrupador1,
										0 AS empenho2011, 0 AS pago2011,
										sum(valor1) AS empenho2012,sum(valor2) AS pago2012
										FROM
										(SELECT
										   sld.acacod,
										   aca.acadsc,
										   CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2
										   FROM
										   dw.saldo2012 sld
										   LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod
										   WHERE sld.acacod in ('20RG','20RW','8652') AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130301','292410403')) as foo
										WHERE
										valor1 <> 0 OR valor2 <> 0
										GROUP BY
										acacod,
										acadsc

						union all

						SELECT
										acacod AS cod_agrupador1,
										acadsc AS dsc_agrupador1,
										0 AS empenho2011, 0 AS pago2011,
										sum(valor1) AS empenho2012,sum(valor2) AS pago2012
										FROM
										(SELECT
										   sld.acacod,
										   aca.acadsc,
										   CASE WHEN sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301') THEN
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor1,CASE WHEN sld.sldcontacontabil in ('292130301','292410403') THEN
									 CASE WHEN sld.ungcod='154004' then (sld.sldvalor) ELSE (sld.sldvalor) END
									 ELSE 0 END AS valor2
										   FROM
										   dw.saldo2012 sld
										   LEFT JOIN dw.acao aca ON aca.acacod = sld.acacod
										   WHERE sld.plicod in ('PFD01M06E1P','PFD01M3300P','PFD01M5700P','PFD01P9301N','RFD01B992DP','RFD01M331DP') AND sld.sldcontacontabil in ('292130100','292130201','292130202','292130203','292130301','292130301','292410403')
											AND sld.acacod in ( '8252', '20RH', '20RO' )
						) as foo
										WHERE
										valor1 <> 0 OR valor2 <> 0
										GROUP BY
										acacod,
										acadsc
						) as foo
						) as z
						group by 1";
			   $arrDadosFin = $db2->carregar($sql,null,3200);
				?>
				<?php foreach($arrDadosFin as $dado): ?>
				<tr>
					<td class="" ><?php echo $dado['descricao'] ?></td>
					<!--<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2011&buscaagrupadorOrigem=UNI&escala=1&agrupador[0]=uo&agrupador[1]=gnd&buscaagrupadorOrigemColunas=RP&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=168&agrupadorColunas[3]=7&agrupadorColunas[4]=92&acacod[0]=00IG&acacod[1]=20RK&acacod[2]=4002&acacod[3]=4009&acacod[4]=8282&alterar_ano=0')" ><?php $total_dotacao2011+=$dado['dotacao2011']; echo number_format($dado['dotacao2011'],2,",",".") ?></td>-->
					<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2011&buscaagrupadorOrigem=UNI&escala=1&agrupador[0]=uo&agrupador[1]=gnd&buscaagrupadorOrigemColunas=RP&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=168&agrupadorColunas[3]=7&agrupadorColunas[4]=92&acacod[0]=00IG&acacod[1]=20RK&acacod[2]=4002&acacod[3]=4009&acacod[4]=8282&alterar_ano=0')" ><?php $total_empenhado2011+=$dado['empenhado2011']; echo number_format($dado['empenhado2011'],2,",",".") ?></td>
					<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2011&buscaagrupadorOrigem=UNI&escala=1&agrupador[0]=uo&agrupador[1]=gnd&buscaagrupadorOrigemColunas=RP&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=168&agrupadorColunas[3]=7&agrupadorColunas[4]=92&acacod[0]=00IG&acacod[1]=20RK&acacod[2]=4002&acacod[3]=4009&acacod[4]=8282&alterar_ano=0')" ><?php $total_pago2011+=$dado['pago2011']; echo number_format($dado['pago2011'],2,",",".") ?></td>
					<!--<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2012&buscaagrupadorOrigem=UNI&escala=1&agrupador[0]=uo&agrupador[1]=gnd&buscaagrupadorOrigemColunas=RP&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=168&agrupadorColunas[3]=7&agrupadorColunas[4]=92&acacod[0]=00IG&acacod[1]=20RK&acacod[2]=4002&acacod[3]=8282&alterar_ano=0')" ><?php $total_dotacao2012+=$dado['dotacao2012']; echo number_format($dado['dotacao2012'],2,",",".") ?></td>-->
					<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2012&buscaagrupadorOrigem=UNI&escala=1&agrupador[0]=uo&agrupador[1]=gnd&buscaagrupadorOrigemColunas=RP&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=168&agrupadorColunas[3]=7&agrupadorColunas[4]=92&acacod[0]=00IG&acacod[1]=20RK&acacod[2]=4002&acacod[3]=8282&alterar_ano=0')" ><?php $total_empenhado2012+=$dado['empenhado2012']; echo number_format($dado['empenhado2012'],2,",",".") ?></td>
					<td class="numero link" onclick="abreRelatorio('painel=1&submetido=1&ano=2012&buscaagrupadorOrigem=UNI&escala=1&agrupador[0]=uo&agrupador[1]=gnd&buscaagrupadorOrigemColunas=RP&agrupadorColunas[0]=19&agrupadorColunas[1]=6&agrupadorColunas[2]=168&agrupadorColunas[3]=7&agrupadorColunas[4]=92&acacod[0]=00IG&acacod[1]=20RK&acacod[2]=4002&acacod[3]=8282&alterar_ano=0')" ><?php $total_pago2012+=$dado['pago2012']; echo number_format($dado['pago2012'],2,",",".") ?></td>
				</tr>
				<?php endforeach; ?>
				<tr>
					<td class="bold" >Total</td>
					<!--<td class="numero bold" ><?php echo number_format($total_dotacao2011,2,",",".") ?></td>-->
					<td class="numero bold" ><?php echo number_format($total_empenhado2011,2,",",".") ?></td>
					<td class="numero bold" ><?php echo number_format($total_pago2011,2,",",".") ?></td>
					<!--<td class="numero bold" ><?php echo number_format($total_dotacao2012,2,",",".") ?></td>-->
					<td class="numero bold" ><?php echo number_format($total_empenhado2012,2,",",".") ?></td>
					<td class="numero bold" ><?php echo number_format($total_pago2012,2,",",".") ?></td>
				</tr>
			</table>
			</div>
                </td>
                <!-- Tabela Financeiro -->
  </tr>
  <tr>
                <!-- Tabela Financeiro -->
                 <td class="fundo_td" >
                	<div>
                		<img style="float:left" src="../imagens/icones/icons/executiverel.png" style="vertical-align:middle;"  />
                		<div style="float:left" class="titulo_box" >Fies Técnico e Empresa</div>
                	</div>
                	<div style="clear:both;text-align:center;width:100%" >
	                	<table class="tabela_box" cellpadding="2" cellspacing="1" width="80%" align="center" >
							<tr>
	                			<td class="center bold">Tipo</td>
	                			<td class="center bold" >Escolas</td>
	                		</tr>
	                		<tr style="cursor:pointer;" onclick="abreIndicadorPopUp(730);">
	                			<td >Instituições habilitadas</td>
	                			<?php
								$sql = "select dpeordem, SUM(dsh.dshqtde) AS total
											from painel.indicador i
											inner join painel.seriehistorica sh on sh.indid=i.indid
											inner join painel.detalheseriehistorica dsh on dsh.sehid = sh.sehid
											inner join painel.detalheperiodicidade dpe on sh.dpeid = dpe.dpeid
											where i.indid in (730)
											and sehstatus <> 'I'
											and tidid1 in (3102) --Processos Habilitados
											group by dpeordem
											order by dpeordem desc
											limit 1";
	                			$inst_hab = $db->pegaLinha($sql); ?>
	                			<td class="numero" ><?php echo number_format($inst_hab['total'],0,",",".") ?></td>
	                		</tr>
	                		<!--<tr>
	                			<td >Matrículas (Fies Técnico)</td>
	                			<td class="numero" >X.XX</td>
	                		</tr>
	                		<tr>
	                			<td >Matrículas (Fies Empresa)</td>
	                			<td class="numero" >X.XX</td>
	                		</tr>-->
	                	</table>
                	</div>
                </td>
  </tr>
  <tr>
                <td class="fundo_td_laranja" colspan="6">
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