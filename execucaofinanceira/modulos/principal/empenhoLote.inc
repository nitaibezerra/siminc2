<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( $titulo_modulo, '' );
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<script language="javascript" type="text/javascript" src="js/execucaofinanceira.js"></script>

<form action="" method="post" name="formulario" id="formulario" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
<table class="tabela" align="center" style="width: 95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="subtituloDireita" style="font-weight: bold;">Tipo do Processo:</td>
		<td>
			<? 
			$tipid = $_POST['tipid'];
			$sql = "SELECT tipid as codigo, tipdescricao as descricao FROM execucaofinanceira.tipoprocesso WHERE tipstatus = 'A'";
			$db->monta_combo( "tipid", $sql, 'S', 'Todas os Tipos do Processo', '', '', '', '', 'S', 'tipid', '', '', 'Tipo do Processo' ); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="font-weight: bold;">Programa:</td>
		<td colspan="3">
		<?php
			$prgid = $_POST['prgid'] ;
			$sql = "SELECT distinct prgid as codigo, prgdsc as descricao FROM par.programa WHERE prgstatus = 'A' order by prgdsc";
			$db->monta_combo( "prgid", $sql, 'S', 'Todas os Programas', '', '', '', 500 );
		?>
		</td>
		
	</tr>
	<tr>
		<td class="subtituloDireita" style="font-weight: bold;">Grupos de Municípios</td>
		<td>
		<?php
		// Filtros do relatório
		$stSql = "SELECT
					tpmid AS codigo,
					tpmdsc AS descricao
				  FROM
				  	territorios.tipomunicipio mtq
				  WHERE
				  	tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)";
		
		if( $_REQUEST['grupo'][0] != '' ){
			$grupos = implode( $_REQUEST['grupo'], "," );
			$sql = "SELECT
					tpmid AS codigo,
					tpmdsc AS descricao
				  FROM
				  	territorios.tipomunicipio mtq
				  WHERE
				  	tpmid IN ({$grupos})";
			$grupo = $db->carregar( $sql );
		}
	
		combo_popup( "grupo", $stSql, "Grupos de Municípios", "400x400", 0, array(), "", "S", false, false, 5, 400 );
					
		?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="font-weight: bold;">Estado:</td>
		<td colspan="3">
		<?php
			$estuf = $_POST['estuf'];
			$sql = "select e.estuf as codigo, e.estdescricao as descricao from territorios.estado e order by e.estdescricao asc";
			$db->monta_combo( "estuf", $sql, 'S', 'Todas as Unidades Federais', 'carragarMunicipio', '' );
		?>
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita" style="font-weight: bold;">Municípios</td>
		<td>
		<?php
		// Filtros do relatório
		
		if( $_POST['estuf'] ) $filtroest = " where m.estuf = '{$_POST['estuf']}'";
		
		$stSql = "select distinct m.muncod as codigo, m.mundescricao as descricao from territorios.municipio m $filtroest order by m.mundescricao";
		
		if( $_REQUEST['muncod'][0] != '' ){
			$muncod = implode( $_REQUEST['muncod'], "','" );
			$sql = "select distinct m.muncod as codigo, m.mundescricao as descricao from territorios.municipio m
				  	where
				  		muncod IN ('{$muncod}')
				  	order by m.mundescricao";
				  	
			$muncod = $db->carregar( $sql );
		}
	
		combo_popup( "muncod", $stSql, "Municípios", "400x400", 0, array(), "", "S", false, false, 5, 400 );
					
		?>
		</td>
	</tr>
	<tr>
		<td class="subtituloDireita"><div id="debug"></div></td>
		<td>
			<input type="button" name="filtrar" id="filtrar" value="Filtrar">
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
function carragarMunicipio(valor){
	$("[name='formulario']").submit();
}

$("#filtrar").click(function() {
	selectAllOptions( formulario.grupo );
	selectAllOptions( formulario.muncod );
	$('#requisicao').val('pesquisar');
	$("[name='formulario']").submit();
});
</script>
<?
if( $_POST['requisicao'] == 'pesquisar' ){
	
	$arWhere = array();
	if( $_POST['prgid'] ) $arWhere[] = "s.prgid = '{$_POST['prgid']}'";
	if( $_POST['tipid'] ) $arWhere[] = "pro.tipid = '{$_POST['tipid']}'";
	if( $_POST['muncod'][0] ) $arWhere[] = "pro.muncod in ('".implode( $_REQUEST['muncod'], "','" )."')";
	if( $_POST['grupo'][0] ){
		$grupos = implode( $_POST['grupo'], "," );
		$innerjoin = "inner join territorios.muntipomunicipio mtm on mtm.muncod = pro.muncod and mtm.estuf = pro.estuf and mtm.tpmid IN (".$grupos.") ";
	}
	
	$sql = "SELECT
			      dados.sbdid,
			      dados.sbaid,
			      dados.sbdano,
			      dados.subacao,
			      dados.valorsubacao,
			      --(SUM(dados.valorempenhado)*100/dados.valorsubacao) as percentualempenhado,
			      0 as percentualempenhado,
			      SUM(dados.valorempenhado) as valorempenhado,
			      /*CASE WHEN dados.tprid = 2 
			          THEN
			              (99 - (SUM(dados.valorempenhado)*100/dados.valorsubacao))
			          ELSE
			              (100 - (SUM(dados.valorempenhado)*100/dados.valorsubacao))
			          END*/ 0 as percentualaempenhar,
			      cast((dados.valorsubacao - SUM(dados.valorempenhado)) as numeric(20,2)) as valoraempenhar,
			      dados.planointerno,
			      dados.ptres,
			      dados.sbdano as ano,
			      dados.reforco,
			      cast((dados.valorsubacao - SUM(dados.valorempenhado)) as numeric(20,2)) as valoraempenharT
			  FROM (
			SELECT 
				sd.sbdid,
			    sd.sbaid,
			    s.sbadsc as subacao,
			    --(SELECT par.recuperaValorValidadosSubacaoPorAno(sd.sbaid, sd.sbdano)) as valorsubacao,
			    prc.prcvalortotal as valorsubacao,
			    /*CASE WHEN emr.reevlr IS NOT NULL THEN 
			        coalesce(es.eobvalorempenho,0)+coalesce(emr.reevlr,0)
			    ELSE
			        coalesce(es.eobvalorempenho,0)
			    END*/ 0 as valorempenhado,
			    pro.tprid,
			    sd.sbdplanointerno as planointerno,
			    sd.sbdptres as ptres,
			    sd.sbdano,
			    sd.sbdreforco as reforco
			FROM
				execucaofinanceira.processos pro
			    inner join execucaofinanceira.processocomposicao prc on prc.proid = pro.proid
			    inner join par.subacaodetalhe sd on sd.sbdid = prc.sbdid
			    inner join par.subacao s on s.sbaid = sd.sbaid
			WHERE
				sd.ssuid IN (2, 12, 13, 18, 17)
			    ".($arWhere ? ' and ' . implode(' and ', $arWhere) : '')."
			) as dados
			GROUP BY
			      dados.sbaid,
			      dados.sbdid,
			      dados.subacao,
			      dados.valorsubacao,
			      dados.tprid,
			      dados.planointerno,
			      dados.ptres,
			      dados.sbdano,
			      dados.reforco";
	
	$arrDados = $db->carregar($sql);
	
	//ver($arrDados,d);
	if( is_array($arrDados) ){
		?>
		<table class="listagem" align="center" style="width: 95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<thead>
			<tr>
				<td align="Center" class="title" width="03%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>&nbsp;</strong></td>
				<td align="Center" class="title" width="03%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>&nbsp;</strong></td>
				<td align="Center" class="title" width="40%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Subação</strong></td>
				<td align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor da Subação</strong></td>
				<td align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>% Empenhado</strong></td>
				<td align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor Empenhado</strong></td>
				<td align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>% a Empenhar</strong></td>
				<td align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Valor a Empenhar</strong></td>
				<td align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Plano Interno</strong></td>
				<td align="Center" class="title" width="05%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>PTRES</strong></td>
				<td align="Center" class="title" width="05" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Ano</strong></td>
				<td align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Reforço</strong></td>
			</tr>
		</thead>
		<tbody>
		<?
		foreach ($arrDados as $key => $v) {
			$key % 2 ? $cor = "#dedfde" : $cor = "";
			?>
			<tr bgcolor="<?=$cor ?>" id="tr_<?=$key; ?>" onmouseout="this.bgColor='<?=$cor?>';" onmouseover="this.bgColor='#ffffcc';">
				<td valign="top" title=" "><input id="chk_<?=$v['sbdid']; ?>" type="checkbox" value="<?=$v['sbdid']; ?>" onclick="marcarChk(this);" name="chk[]"></td>
				<td valign="top" title=" "><a href="javascript:listarSubacao('<?=$v['sbaid']; ?>', '<?=$v['ano']; ?>', '<?=$v['inuid']; ?>')"> <img border="0" title="Visualizar Subação" src="../imagens/consultar.gif"></a></td>
				<td valign="top" title="Subação"><?=$v['subacao'] ?></td>
				<td valign="top" align="right" title="Valor da Subação" style="color:#999999;"><?=number_format($v['valorsubacao'], 2, ',', '.') ?>
						<input id="valorsubacao_<?=$v['sbdid']; ?>" type="hidden" value="<?=number_format($v['valorsubacao'], 2, ',', '.') ?>" name="valorsubacao[<?=$v['sbdid']; ?>]"></td>
				<td valign="top" align="right" title="% Empenhado" style="color:#999999;"><?=number_format($v['percentualempenhado'], 2, ',', '.') ?></td>
				<td valign="top" align="right" title="Valor Empenhado" style="color:#999999;"><?=number_format($v['valorempenhado'], 2, ',', '.') ?></td>
				<td valign="top" title="% a Empenhar">
						<input id="emcpercentualemp_<?=$v['sbdid']; ?>" class="disabled" type="text" onfocus="this.select();" readonly="readonly" onblur="verificaPreenchimentoPorcentagem(this)" 
								onkeyup="calculaEmpenhoPorPorcento(this);" size="6" name="emcpercentualemp[<?=$v['sbdid']; ?>]" value="<?=$v['percentualaempenhar'] ?>">
								
						<input id="pecentual_<?=$v['sbdid']; ?>" type="hidden" value="<?=number_format($v['percentualaempenhar'], 2, ',', '.') ?>" name="pecentual[<?=$v['sbdid']; ?>]">						
						<input id="anosubacao_<?=$v['sbdid']; ?>" type="hidden" value="<?=$v['ano'] ?>" name="anosubacao_<?=$v['sbdid']; ?>"></td>
				<td valign="top" title="Valor a Empenhar"><input id="emcvaloremp_<?=$v['sbdid']; ?>" class="disabled" type="text" onfocus="this.select();" readonly="readonly" 
						onkeyup="this.value=mascaraglobal('[.###],##',this.value); calculaEmpenhoPorValor(this);" onblur="this.value=mascaraglobal('[.###],##',this.value); calculaEmpenhoPorValor(this);" 
						size="15" name="emcvaloremp[<?=$v['sbdid']; ?>]" value="<?=number_format($v['valoraempenhar'], 2, ',', '.') ?>">
				</td>
				<td valign="top" title="Plano Interno"><?=$v['planointerno'] ?></td>
				<td valign="top" title="PTRES"><?=$v['ptres'] ?></td>
				<td valign="top" title="Ano"><?=$v['ano'] ?> </td>
				<td valign="top" title="Reforço"><?=$v['reforco'] ?></td>
				<td valign="top" title="">
				<input id="vlrTotal_<?=$v['sbdid']; ?>" type="hidden" value="<?=$v['valoraempenhar'] ?>" name="vlrTotal[<?=$v['sbdid']; ?>]" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" onblur="this.value=mascaraglobal('[.###],##',this.value);">
				</td>
			</tr>
			<?
			$totalSubacao += (float)$v['valorsubacao'];
			$totalEmpenhado = (float)$v['percentualempenhado'];
			$totalVlEmpenhado = (float)$v['valorempenhado'];
		}
		?>
		</tbody>
		<tfoot>
			<tr>
			<td align="right" title=" ">Totais: </td>
			<td align="right" title=" "></td>
			<td align="right" title="Subação"></td>
			<td align="right" title="Valor da Subação"><?=number_format($totalSubacao, 2, ',', '.'); ?></td>
			<td align="right" title="% Empenhado"><?=number_format($totalEmpenhado, 2, ',', '.'); ?></td>
			<td align="right" title="Valor Empenhado"><?=number_format($totalVlEmpenhado, 2, ',', '.'); ?></td>
			<td align="right" title="% a Empenhar"></td>
			<td align="right" title="Valor a Empenhar"></td>
			<td align="right" title="Plano Interno"></td>
			<td align="right" title="PTRES"></td>
			<td align="right" title="Ano"></td>
			<td align="right" title="Reforço"></td>
			<td align="right" title=""></td>
			</tr>
		</tfoot>		
		</table>
		<table class="listagem" align="center" style="width: 95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td class="SubTituloDireita"><b>Valor total de empenho:</b></td>
				<td><input type="text" id="vlrtotalempenho" name="vlrtotalempenho" size="30" class="normal" style="text-align:right;" readonly="readonly" value="0,00"></td>
			</tr>
		</table>
		<?
	} else {
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
		print '</table>';
	}
}
?>