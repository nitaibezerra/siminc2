<link href="http://code.google.com/apis/maps/documentation/javascript/examples/default.css" rel="stylesheet" type="text/css" />
<link href="/includes/maps/maps.css" rel="stylesheet" type="text/css" />
<script type="text/javascript" src="http://maps.google.com/maps/api/js?sensor=false"></script>
<script type="text/javascript" src="/includes/agrupador.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script>var base_url = '<?=LINK_PADRAO ?>';</script>
<script type="text/javascript" src="/includes/maps/maps.js"></script>
<script type="text/javascript" src="/includes/maps/markerclusterer.js"></script>
<script type="text/javascript" src="/includes/JQuery/xcolor.js"></script>
<script type="text/javascript" src="/includes/JQuery/colorPicker.js"></script>
<script src="http://ajax.googleapis.com/ajax/libs/jqueryui/1.8/jquery-ui.min.js"></script>

<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
	<td colspan="2" align="center" bgcolor="#dedede">
	<TABLE width="100%" cellspacing="0" cellpadding="3" border ="1">
		<tr id="linha_uf">
			<?
				$sql = "SELECT	estuf, estdescricao	FROM territorios.estado	ORDER BY estuf ";
				$arrDados = $db->carregar($sql);
				if($arrDados){
					for ( $i = 0; $i < count( $arrDados ); $i++ )
						{
						$clicar[] = "clicaTodos('".trim($arrDados[$i]['estuf'])."', document.getElementById('td_".trim($arrDados[$i]['estuf'])."'));";
			?>
			<td bgcolor="#dedede" id="td_<?=trim($arrDados[$i]['estuf']); ?>" align="center" onclick="clicaEstado('<?=trim($arrDados[$i]['estuf']);?>', this);"><div title="<?php echo $arrDados[$i]['estdescricao'] ?>" id="estuf<?=$arrDados[$i]['estuf'];?>"><img src="../imagens/bandeiras/mini/<?=$arrDados[$i]['estuf'] ?>.png"><br><div><?=$arrDados[$i]['estuf'];?></div></div></td>
			<?
						}

				}
			?>
			<td bgcolor="#dedede" align="center" onclick="<?=implode("",$clicar) ?>"><div id="estuftodos"><img src="../imagens/bandeiras/bandeira_todas.jpg" width=25 height=17><br><div>Todos</div></div></td>
		</tr>
	</TABLE>
	<div id="nome_mun" style="color: black;font-family: Arial;font-size: 12pt;font-weight: bolder;">Brasil</div>
	</td>
	</tr>
</table>
<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
	<tr>
		<td valign="top" style="width:350px;height:400px">
			<div id="accordion">
				<a class="tituloAcordion" href="#">FILTROS<hr></a>
				<form method="post" id="filtros">
				<div><? mapaFiltros(); ?></div>
				</form>
				<a class="tituloAcordion" href="#">INFORMAÇÃO DO MUNICÍPIO<hr></a>
				<div>
					<div id="inf_mun" style="color: black;font-family: Arial;font-size: 14pt;font-weight: bolder;"></div>
				</div>
				<a class="tituloAcordion" href="#">RELATÓRIO QUANTITATIVO<hr></a>
				<div>
					<div style="overflow:auto;height:398px;" id="colunainfo"></div>
				</div>
				<a class="tituloAcordion" href="#">ROTAS<hr></a>
				<div>
					<table class="tabela" width="100%" >
						<tr>
							<td class="SubtituloDireita" style="font-weight:bold" >Origem:</td>
							<td>
								<input type="text" size="30" id="start" name="from" value=""/>
								<input type="hidden" id="start_lat" name="start_lat" value=""/>
								<input type="hidden" id="start_lng" name="start_lng" value=""/>
							</td>
						</tr>
						<tr>
							<td class="SubtituloDireita" style="font-weight:bold" >Destino:</td>
							<td>
								<input type="text" size="30" id="end" name="to" value="" />
								<input type="hidden" id="end_lat" name="end_lat" value=""/>
								<input type="hidden" id="end_lng" name="end_lng" value=""/>
							</td>
						</tr>
						<tr>
							<td class="SubtituloEsquerda" colspan="2"><input name="submit" type="button" onclick="calcRoute()" value="Exibir Rota" />   <input name="submit" type="button" onclick="removeRoute()" value="Remover Rota" /> </td>
						</tr>
					</table>
				  <div id="directionsPanel" style="width: 300px"></div>
				  <div id="warnings_panel" style="width: 300px"></div>
				</div>
				<a class="tituloAcordion" href="#">LOCALIZAR NO MAPA<hr></a>
				<div>
					<table class="tabela" width="100%" >
						<tr>
						    <td align='right' class="SubTituloDireita"><b>Texto:</b></td>
						    <td>
						    	<?=campo_texto('busca_municipio','N','S',30,20,'','','','','','','id="busca_municipio"','BuscaMunicipioEnter2(event);') ?>
						    	 <input type="button" value="OK" onclick="BuscaMunicipio2();">
						    </td>
						</tr>
						<tr>
							<td colspan=2>
								<div id="resultado_pesquisa">
								</div>
							</td>
						</tr>
					</table>
				</div>
				<a class="tituloAcordion" href="#">TEMAS<hr></a>
				<div>
					<?php mapaTemas() ?>
				</div>
				<?php if(strstr($_SESSION['favurl'],"mapaPadrao") || strstr($_SESSION['favurl'],"mapaPlanejamento")): ?>
					<a class="tituloAcordion" href="#">BUSCA AVANÇADA<hr></a>
					<div>
						<?php buscaAvancada() ?>
					</div>
				<?php endif; ?>
			</div>
		</td>
		<td valign="top">
		<div style="position:absolute;z-index:3;text-align:center;padding:1px;width:60px;margin-left:80px;margin-top:10px;"><input type="button" value="Brasil"  onclick="centralizaBrasil()" /></div>
		<div id="map_canvas" style="width:100%;height:600px;position:relative;"></div>
		</td>
	</tr>
</table>
<script>initialize();</script>