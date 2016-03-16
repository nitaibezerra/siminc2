<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

if( $_POST['requisicaoajax'] == 'executarEmpenho' ){
	executarEmpenho($_POST);
	exit();
}

include APPRAIZ . "includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, $parametros);
monta_titulo( $titulo_modulo, '' );

$largura 	= "250px";
$altura 	= "120px";
$id 		= "div_auth";
?>

<script language="javascript" type="text/javascript" src="../includes/JQuery/jquery-ui-1.8.4.custom/js/jquery-1.4.2.min.js"></script>
<style>
			#listaSub100Empenhada thead tr td, #listaSubASeremEmpenhada thead tr td
			{
				background-color: #F7F7F7;
				font-weight: bold;
				font-size: 12px;
				font-family: arial;
			}
			
			.popup_alerta
			{
				width:<?php echo $largura ?>;
				height:<?php echo $altura ?>;
				position:absolute;
				z-index:0;
				top:50%;
				left:50%;
				margin-top:-<?php echo $altura/2 ?>;
				margin-left:-<?php echo $largura/2 ?>;
				border:solid 2px black;
				background-color:#FFFFFF;
				display:none
			}
			.popup_alerta2
			{
				width:90%;
				height:90%;
				position:absolute;
				z-index:0;
				top:50%;
				left:30%;
				margin-top:-250;
				margin-left:-250;
				border:solid 2px black;
				background-color:#FFFFFF;
				display:none
			}
			.popup_alerta3
			{
				width:60%;
				height:60%;
				position:absolute;
				z-index:0;
				top:50%;
				left:30%;
				margin-top:-250;
				margin-left:-250;
				border:solid 2px black;
				background-color:#FFFFFF;
				display:none
			}
		</style>

<form action="" method="post" name="formulario" id="formulario" >
	<input type="hidden" name="requisicao" id="requisicao" value="">
	<input type="hidden" id="sbdidH" name="sbdidH" value="">
<table class="tabela" align="center" style="width: 95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
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

<script type="text/javascript">
function carragarMunicipio(valor){
	$("[name='formulario']").submit();
}

$("#filtrar").click(function() {
	selectAllOptions( formulario.muncod );
	$('#requisicao').val('pesquisar');
	$("[name='formulario']").submit();
});
</script>
<?
//if( $_POST['requisicao'] == 'pesquisar' ){
	
	$arWhere = array();
	if( $_POST['muncod'][0] ) $arWhere[] = "pp.muncod in ('".implode( $_REQUEST['muncod'], "','" )."')";
	
	/*
	 *  dados.sbdid,
			      dados.sbaid,
			      dados.sbdano,
			      dados.subacao,
			      dados.valorsubacao,
			      --(SUM(dados.valorempenhado)*100/dados.valorsubacao) as percentualempenhado,
			      0 as percentualempenhado,
			      SUM(dados.valorempenhado) as valorempenhado,
			       0 as percentualaempenhar,
			      cast((dados.valorsubacao - SUM(dados.valorempenhado)) as numeric(20,2)) as valoraempenhar,
			      dados.planointerno,
			      dados.ptres,
			      dados.sbdano as ano,
			      dados.reforco,
			      cast((dados.valorsubacao - SUM(dados.valorempenhado)) as numeric(20,2)) as valoraempenharT
	 * */
	
	$sql = "SELECT 
                '<input type=checkbox name=chk[] onclick=marcarChk(this); id=chk_'||sd.sbdid||' value='||sd.sbdid||'>' as chk,
                '<a href=\"javascript:listarSubacao(\'' || s.sbaid || '\', \'' || sd.sbdano || '\', \'' || iue.inuid || '\')\" ><img src=\"../imagens/consultar.gif\" title=\"Visualizar Subação\" border=\"0\"></a>' as consulta,
                pp.prpnumeroprocesso,
                iu.muncod,
                t.mundescricao,
                sd.sbdid,
                sd.sbaid,
                iue.inuid,
                s.sbadsc as subacao, 
                (SELECT par.recuperaValorValidadosSubacaoPorAno(sd.sbaid, '2013')) as valorsubacao,
                coalesce(es.eobvalorempenho,0) as valorempenhado,
                sd.sbdplanointerno as planointerno,
                sd.sbdptres as ptres,
                sd.sbdano as ano,
				(SELECT par.recuperaValorValidadosSubacaoPorAno(sd.sbaid, '2013'))::numeric(20,2) as valoraempenhar
FROM par.processopar pp
INNER JOIN par.processoparcomposicao pc ON pp.prpid = pc.prpid and pc.ppcstatus = 'A'
INNER JOIN par.subacaodetalhe sd ON sd.sbdid = pc.sbdid AND sd.sbdano = '2013' AND sd.ssuid IN (2, 12, 13, 18, 17)
INNER JOIN par.subacao s ON s.sbaid = sd.sbaid and s.sbastatus = 'A'
INNER JOIN par.instrumentounidadeentidade iue ON iue.inuid = pp.inuid AND iue.iuecnpj = pp.prpcnpj 
INNER JOIN par.instrumentounidade iu ON iu.inuid = iue.inuid
INNER JOIN territorios.municipio t on t.muncod = iu.muncod
LEFT JOIN par.empenhosubacao es ON es.sbaid = sd.sbaid AND es.eobano = sd.sbdano and eobstatus = 'A'
LEFT JOIN par.empenho e  ON e.empid = es.empid and empstatus = 'A'  
WHERE -- prpnumeroprocesso = '23400004004201382' AND
/*s.prgid = 153 -- subações de onibus acessivel
AND*/ iue.iueenviado = 't' -- habilitado
					";
	
	//".($arWhere ? ' and ' . implode(' and ', $arWhere) : '');
	
	$arrDados = $db->carregar($sql);
	
	//ver($arrDados,d);
	if( is_array($arrDados) ){
		?>
		<table class="tabela" align="center" style="width: 95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
			<tr>
				<td valign="middle"><input id="todos" type="checkbox" value="todos" onclick="marcarTodos(this);" name="todos">Marcar Todos</td>
			</tr>
		</table>
		<table class="listagem" align="center" style="width: 95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<thead>
			<tr>
				<td align="Center" class="title" width="05%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>&nbsp;</strong></td>
				<td align="Center" class="title" width="10%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Processo</strong></td>
				<td align="Center" class="title" width="15%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Municípios</strong></td>
				<td align="Center" class="title" width="05%" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;"
					onmouseover="this.bgColor='#c0c0c0';" onmouseout="this.bgColor='';"><strong>Estado</strong></td>
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
				<td valign="top" title=" "><input id="chk_<?=$v['sbdid']; ?>" type="checkbox" value="<?=$v['sbdid']; ?>" onclick="marcarChk(this);" name="chk[]">
						<a href="javascript:listarSubacao('<?=$v['sbaid']; ?>', '<?=$v['ano']; ?>', '<?=$v['inuid']; ?>')"> <img border="0" title="Visualizar Subação" src="../imagens/consultar.gif"></a></td>
				<td valign="top" title="Processo"><?=$v['processo'] ?></td>
				<td valign="top" title="Municipios"><?=$v['processo'] ?></td>
				<td valign="top" title="Estado"><?=$v['processo'] ?></td>
				<td valign="top" title="Subação"><?=$v['subacao'] ?></td>
				<td valign="top" align="right" title="Valor da Subação" style="color:#999999;"><?=number_format($v['valorsubacao'], 2, ',', '.') ?>
						<input id="valorsubacao_<?=$v['sbdid']; ?>" type="hidden" value="<?=number_format($v['valorsubacao'], 2, ',', '.') ?>" name="valorsubacao[<?=$v['sbdid']; ?>]"></td>
				<td valign="top" align="right" title="% Empenhado" style="color:#999999;"><?=number_format('0', 2, ',', '.') ?></td>
				<td valign="top" align="right" title="Valor Empenhado" style="color:#999999;"><?=number_format($v['valorempenhado'], 2, ',', '.') ?></td>
				<td valign="top" title="% a Empenhar">
						<input id="emcpercentualemp_<?=$v['sbdid']; ?>" class="disabled" type="text" onfocus="this.select();" readonly="readonly" onblur="verificaPreenchimentoPorcentagem(this)" 
								onkeyup="calculaEmpenhoPorPorcento(this);" size="6" name="emcpercentualemp[<?=$v['sbdid']; ?>]" value="100">
								
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
				<input id="vlrTotal_<?=$v['sbdid']; ?>" type="hidden" value="<?=$v['valoraempenhar'] ?>" name="vlrTotal[<?=$v['sbdid']; ?>]" onkeyup="this.value=mascaraglobal('[.###],##',this.value);" onblur="this.value=mascaraglobal('[.###],##',this.value);">
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
			<td align="right" title="Processo"></td>
			<td align="right" title="Municipios"></td>
			<td align="right" title="Estado"></td>
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
			<tr>
				<td class="SubTituloDireita" width="30%"><b>Plano Interno:</b></td>
				<td>
					<input type="text" value="WFF08B90PR" name="planointerno" id="planointerno" readonly="readonly">
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><b>PTRES:</b></td>
				<td>
					<input type="text" value="061649" name="ptresSPAN" id="ptresSPAN" readonly="readonly">
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita"><b>Fonte de Recurso:</b></td>
				<td>
					<input type="text" value="0113000000" name="fonte" id="fonte" readonly="readonly">
				</td>
			</tr>
			<?php //if( in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) || in_array( PAR_PERFIL_ADMINISTRADOR, pegaArrayPerfil($_SESSION['usucpf']) ) || in_array( PAR_PERFIL_EMPENHADOR, pegaArrayPerfil($_SESSION['usucpf']) )  ){ ?>
			<tr>
				<td class="SubTituloDireita"><b>Centro de Gestão:</b></td>
				<td>
					<input type="text" value="" name="gestaosolicitacao" id="gestaosolicitacao" readonly="readonly">
				</td>
			</tr>			
			<tr>
				<td class="SubTituloDireita"><b>Natureza de Despesa:</b></td>
				<td>
					<input type="text" value="44400000" name="gestaosolicitacao" id="gestaosolicitacao" readonly="readonly">
				</td>
			</tr>
			<tr>
				<td colspan="2" class="SubTituloDireita" style="text-align: center;">
					<input type="button" name="solicitar" <?php echo $disabled; ?> id="solicitar" value="Solicitar empenho" onclick="document.getElementById('div_auth').style.display='block'" />
						<input type=button id=visualizar name=visualizar  value=Visualizar XML onclick=visEmp(); />				
				</td>
			</tr>
		</table>
		<?
	} else {
		print '<table width="95%" align="center" border="0" cellspacing="0" cellpadding="2" class="listagem">';
		print '<tr><td align="center" style="color:#cc0000;">Não foram encontrados Registros.</td></tr>';
		print '</table>';
	}
//}
?>
<div class="demo">
	<div id="dialog-confirm" title="Resposta da solicitação de Empenho:"></div>
</div>
<!-- Div para solicitação de Empenho -->
<div id="<?php echo $id ?>" class="popup_alerta <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="30%" class="SubtituloDireita">Usuário:</td>
			<td><?php echo campo_texto("wsusuario","S","S","Usuário","22","","","","","","","id='wsusuario'") ?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Senha:</td>
			<td>
				<input type="password" class="obrigatorio normal" title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$wssenha; ?>" size="23" id="wssenha" name="wssenha">
				<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#D5D5D5" colspan="2">
				<input type="button" name="btn_enviar" onclick="solicitarEmpenhoPar()" value="ok" />
				<input type="button" name="btn_cancelar" onclick="document.getElementById('<?php echo $id ?>').style.display='none'" value="cancelar" />
			</td>
		</tr>
		</table>
	</div>
</div>
</form>
<script language="javascript" type="text/javascript">
document.getElementById('div_auth').style.display='none';

function marcarTodos(obj){
	if(obj.checked == true){
		$('[id*="chk_"]').each(function(){			
			$("#"+this.id).attr('checked', true);
			marcarChk(this);
		});
	} else {
		$('[id*="chk_"]').each(function(){			
			$("#"+this.id).attr('checked', false);
			marcarChk(this);
		});
	}
}

function marcarChk(obj) {
	document.getElementById('sbdidH').value = obj.value;
	
	var codigo 				= obj.value;
	var emcpercentualemp 	= $('#emcpercentualemp_'+codigo);
	var emcvaloremp 		= $('#emcvaloremp_'+codigo);	
	
	if( obj.checked ) {
		emcpercentualemp.attr('className', 'normal');
		emcpercentualemp.attr('readOnly', false);
		
		emcvaloremp.attr('className', 'normal');
		emcvaloremp.attr('readOnly', false);
		emcpercentualemp.focus();
	} else {
		emcpercentualemp.attr('className', 'disabled');
		emcpercentualemp.attr('readOnly', true);
		
		emcvaloremp.attr('className', 'disabled');
		emcvaloremp.attr('readOnly', true);
	}
	
	calcularTotal();
}

function calculaEmpenhoPorPorcento(obj) {
	var codigo = obj.id.split('_');
		codigo = codigo[1];
	var porcento = obj.value;
	var valorsubacao = retiraPontos($('#valorsubacao_'+codigo).val());
	
	var total = (parseFloat(valorsubacao) * parseFloat(porcento) ) / 100;
	var total_mac = mascaraglobal('###.###.###.###,##',total.toFixed(2));
	//$('#debug').html(total_mac);
	
	$('#emcvaloremp_'+codigo).val(total_mac);
	
	calcularTotal();
}

function calculaEmpenhoPorValor(obj) {	
	var codigo 			= obj.id.split('_');
		codigo 			= codigo[1];
	var valorinformado 	= retiraPontos(obj.value);
	var valorsubacao 	= retiraPontos($('#valorsubacao_'+codigo).val());	
	var total_mac 		= mascaraglobal('###.###.###.###,##',valorinformado);
	var percent 		= (parseFloat(valorinformado) * 100) / parseFloat(valorsubacao);
	
	$('#emcpercentualemp_'+codigo).val(percent.toFixed(2));

	calcularTotal();
}

function calcularTotal() {
	
	var total = 0;
	var valor = 0;
	
	$('[id*="chk_"]').each(function(){
		var codigo 	= this.id.split('_');
			codigo 	= codigo[1];
		
		if(this.checked == true ){
			valor = retiraPontos( $('#emcvaloremp_'+codigo).val() );
			valor = parseFloat(valor);
			total = total + valor;
		}
	});
	
	$('#vlrtotalempenho').val( mascaraglobal('###.###.###.###,##',total.toFixed(2)) );
}

function verificaPreenchimentoPorcentagem(obj){
	
	var codigo = obj.id.split('_');
		codigo = codigo[1];
	
	var valorSubacao 	= retiraPontos($('#valorsubacao_'+codigo).val());
	var valorInformado 	= retiraPontos($('#emcvaloremp_'+codigo).val());
	
	if( parseFloat(valorInformado) > parseFloat(valorSubacao) ){
		alert('O valor informado para empenho ultrapassa 100% do valor da Subação.');
		$('#emcvaloremp_'+codigo).val('0,00');
		$('#emcpercentualemp_'+codigo).val('0');
		calcularTotal();
	}
	
}

function retiraPontos(v){
	var valor = v.replace(/\./gi,"");
	valor = valor.replace(/\,/gi,".");
	
	return valor;
}

function solicitarEmpenhoPar() {
	
	var form = document.getElementById('formulario');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha = document.getElementById('wssenha').value;
	
	var marcado=false;
	for(var i=0;i<form.elements.length;i++) {
		if(form.elements[i].type) {
			if(form.elements[i].type == "checkbox" && form.elements[i].checked == true) {
				marcado = true;
			}
		}
	}
	if(!marcado) {
		alert('Nenhuma Subação selecionada');
		return false;
	}
	
	if(!usuario) {
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}

	divCarregando();
	
	var dadosSubacao = $('#formulario').serialize();

	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/empenhoLote&acao=A",
   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicaoajax=executarEmpenho&"+dadosSubacao,
   		async: false,
   		success: function(msg){
   			
   			$( "#dialog-confirm" ).html(msg);
   		//	$( "#dialog:ui-dialog" ).dialog( "destroy" );

			$( "#dialog-confirm" ).dialog({
				resizable: true,
				height:400,
				width:400,
				modal: true,
				buttons: {
					"OK": function() {
						$( this ).dialog( "close" );
						window.location.reload();
						
					}
					
				}
			});
   		}
	});
	
	//carregarListaSubacao();
	//carregarListaEmpenhoProcesso();

	divCarregado();
	
}
</script>

<?php 
function executarEmpenho($dados) {
	global $db;

	if( atualizaValoresSubacao( $dados ) ){

		$_SESSION['par_var']['esfera'] = $db->pegaUm("SELECT CASE WHEN itrid = 1 THEN 'estadual' ELSE 'municipal' END as esfera FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
		$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."'");

		$obHabilita = new Habilita();
		$cnpj 		= pegaCnpj($_SESSION['par_var']['inuid']);
		$habilitado = $obHabilita->consultaHabilitaEntidade($cnpj);

		if($dadosse['prptipoexecucao'] == 'C'){

			if($habilitado == 'Habilitado'){
				$res_acc = atualizaDadosContaCorrentePar( $dados );
				if( $res_acc ){
					$res_sp = solicitarProcesso($dados);
					$res_cc = consultarContaCorrente($dados);
					if(!$res_cc) $res_sc = solicitarContaCorrente($dados);
					$dadosConvenioFNDE = solicitarConvenio($dados);
					$nu_convenio  = (int) $dadosConvenioFNDE->nu_convenio;
					$an_exercicio = (int) $dadosConvenioFNDE->an_exercicio;
					if($dadosConvenioFNDE){
						$res_se = solicitarEmpenho($dados, $nu_convenio ,$an_exercicio );
						if($res_se === true){ // mando para o SIGARP
							//enviarItensSigarp($dados);
						}
					}
				}
			}else{
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
								ERRO AO VERIFICAR A ENTIDADE NO SISTEMA DE HABILITAÇÃO:
								<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
									Erro: '.$habilitado.'
								</div>
							</div>
						 	<br>
							';
			}
				
		}else{
			//$habilitado = 'Habilitado';
			if($habilitado == 'Habilitado'){
				$res_acc = atualizaDadosContaCorrentePar( $dados );
				if( $res_acc ){
					$res_sp = solicitarProcesso($dados);
					if($res_sp){
						$res_cc = consultarContaCorrente($dados);
						if($res_cc){
							$res_sc = solicitarContaCorrente($dados);
						}

						if($res_sp && ($res_sp || $res_sc)  ){
							$res_se = solicitarEmpenho($dados, null ,null );
							if($res_se === true){ // mando para o SIGARP
								//enviarItensSigarp($dados);
							}
						}
					}
				}else{
						
					$res_sp = solicitarProcesso($dados);
					if($res_sp){
						$res_sc = solicitarContaCorrente($dados);
						if($res_sc && $res_sp ){
							$res_se = solicitarEmpenho($dados, null ,null );
							if($res_se === true){ // mando para o SIGARP
								//enviarItensSigarp($dados);
							}
						}
					}
						
						
				}
			}else{
				echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
								ERRO AO VERIFICAR A ENTIDADE NO SISTEMA DE HABILITAÇÃO:
								<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
									Erro: '.$habilitado.'
								</div>
							</div>
						 	<br>
							';
			}
		}
	} else {
		return false;
	}
}
function atualizaValoresSubacao( $dados ){

	global $db;

	// Verifico os itens da subação
	$sql = "SELECT
					s.sbaid,
					sd.sbdano,
					sic.icoid,
					pic.idsigarp,
					CASE WHEN sbacronograma = 1 --global
					THEN
						CASE WHEN sic.icovalidatecnico IS NULL -- antigos
						THEN coalesce(sic.icoquantidade,0)
						ELSE -- novos
							CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
								sum(coalesce(sic.icoquantidadetecnico,0))
							END
						END
					ELSE -- escolas
						CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
						THEN -- escolas sem itens
							CASE WHEN se.sesvalidatecnico IS NULL -- antigos
								THEN
									sum(coalesce(se.sesquantidade,0))
								ELSE -- novos
									sum(coalesce(se.sesquantidadetecnico,0))
								END
							ELSE -- escolas com itens
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN
									sum(coalesce(ssi.seiqtd,0))
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(ssi.seiqtdtecnico,0))
									END
								END
							END
					END as quantidade,
					sic.icovalor as valor
				FROM
					par.subacao s
				INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
			--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid
				LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano
				LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
				LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
				INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
				WHERE
					s.sbastatus = 'A' AND sd.sbdid IN ( ".implode(", ", $dados['chk'])." ) AND
			
					 CASE WHEN sbacronograma = 1
                     THEN sic.icovalidatecnico <> 'N'
                     ELSE
                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
                        	se.sesvalidatecnico <> 'N'
                        ELSE
                        	sic.icovalidatecnico <> 'N'
                        END
                     END
                group by
					s.sbaid, sd.sbdano, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor";
	//		pr.prpid = ".$_SESSION['par_var']['prpid'];

	$itens = $db->carregar( $sql );

	$it = array();
	$erro = array();

	if( is_array($itens) ){
		foreach( $itens as $item ){
			if( !$item['idsigarp'] == '' ){
				$valorSigarp = diponibilizaPregoesItens($item['idsigarp'],'valor');
				if( $valorSigarp ){
					$valor = (string) $valorSigarp;
					$valorSimec = $item['valor'];
					$it[$item['icoid']]['icoid'] = $item['icoid'];
					$it[$item['icoid']]['idsigarp'] = $item['idsigarp'];
					$it[$item['icoid']]['valor'] = $item['valor'];
					$it[$item['icoid']]['valorSigarp'] = $valor;
					if( $valorSimec != $valor ){
						$erro[] = $item['icoid'];
					}
				}
			}
		}
	} else {
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						Não existem itens aprovados na subação. <BR>
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							 Necessário analisar a subação!
						</div>
					</div>
				 	<br>
					';
		return false;
	}

	if( is_array($erro) && $erro[0] ){ // O valor é diferente então atualizo os valores dos itens.
		foreach( $erro as $err ){
			$sqlAtualizaItens .= " UPDATE par.subacaoitenscomposicao SET icovalor = ".$it[$err]['valorSigarp']." WHERE icoid = ".$it[$err]['icoid']."; ";
		}
		$db->executar( $sqlAtualizaItens );
		$db->commit();
			
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						VALORES DOS ITENS DE COMPOSIÇÃO DA SUBAÇÃO FORAM ATUALIZADOS DE ACORDO COM O PREGÃO. <BR>
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Solicite novamente o empenho!
						</div>
					</div>
				 	<br>
					';
			
		return false;
	}
	return true;

}

function diponibilizaPregoesItens($item, $tipo){

	global $db;

	try {

		$data_created = date("c");

		$usuario = USUARIO_SIGARP;
		$senha   = SENHA_SIGARP;
		
		$sql = "SELECT
					CASE WHEN estuf IS NOT NULL
						THEN estuf
						ELSE mun_estuf
					END as estuf
				FROM
					par.instrumentounidade
				WHERE
					inuid = ".$_SESSION['par_var']['inuid'];
		$estuf = $db->pegaUm( $sql );

		$arqXml = <<<XML
<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>$data_created</created>
    </header>
    <body>
        <auth>
            <usuario>$usuario</usuario>
            <senha>$senha</senha>
        </auth>
        <params>
            <nu_seq_item>{$item}</nu_seq_item>
            <uf>{$estuf}</uf>
        </params>
    </body>
</request>
XML;

	 /*
	  echo '<pre>';
		echo simec_htmlentities($arqXml);
		die();
		*/
		if( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//			$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/pregao';
			$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/pregao';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'listar') )
		->execute();

		$xmlRetorno = $xml;
		$xml = simplexml_load_string( stripslashes($xml));

		$result = (integer) $xml->status->result;

		//print_r ( (string) $xml->body->VL_ITEM );
		//die();

		if(!$result) {
				
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'diponibilizaPregoesItens - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();
				
			$erros = $xml->status->error->message->text;
			/* -- ARRUMAR DEPOIS
			 echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
			ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
			<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
			'.iconv("UTF-8", "ISO-8859-1", $erros).'
			</div>
			</div>
			<br>
			';
			*/
			//echo "*** Descrição do erro ***\n\n";
			//$erros = $xml->status->error->message->text;
			//echo "* ".iconv("UTF-8", "ISO-8859-1", $erros);
		} else { //sucesso
				
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'diponibilizaPregoesItens - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			if( $tipo == 'valor' ){
				return $xml->body->VL_ITEM ? $xml->body->VL_ITEM : null;
			} else {
				return (integer) $xml->body->NU_SEQ_PREGAO ? $xml->body->NU_SEQ_PREGAO : false;
			}
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Temporariamente indisponível.Favor tente mais tarde.".'\n';
			/* -- ARRUMAR DEPOIS
			 echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
			ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
			<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
			Temporariamente indisponível.Favor tente mais tarde.
			</div>
			</div>
			<br>
			';
			*/
				
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );
		/* -- ARRUMAR DEPOIS
		 echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
		ERRO AO CONSULTAR ITENS DO PREGÃO (SIGARP)
		<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
		Erro-WS SIGARP: '.$erroMSG.'
		<br>
		<pre>
		'.print_r($e).'
		</pre>
		</div>
		</div>
		<br>
		';
		*/

		//echo "Erro-WS SIGARP: $erroMSG";
		//echo "<pre>";
		//print_r($e);

	}
}

function aderirPregao( $dados, $tipo = 1 ){

	global $db;

	$item = $db->carregarColuna( "SELECT icoid FROM par.empenhopregaoitemperdido WHERE sbdid = ".$dados['sbdid'] );

	try {

		// Verifico os itens da subação
		$sql = "SELECT
					s.sbaid,
					sd.sbdano,
					sd.sbdid,
					sic.icoid,
					pic.idsigarp,
					CASE WHEN sbacronograma = 1 --global
					THEN
						CASE WHEN sic.icovalidatecnico IS NULL -- antigos
						THEN coalesce(sic.icoquantidade,0)
						ELSE -- novos
							CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
								sum(coalesce(sic.icoquantidadetecnico,0))
							END
						END
					ELSE -- escolas
						CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
						THEN -- escolas sem itens
							CASE WHEN se.sesvalidatecnico IS NULL -- antigos
								THEN
									sum(coalesce(se.sesquantidade,0))
								ELSE -- novos
									sum(coalesce(se.sesquantidadetecnico,0))
								END
							ELSE -- escolas com itens
								CASE WHEN sic.icovalidatecnico IS NULL -- antigos
								THEN
									sum(coalesce(ssi.seiqtd,0))
								ELSE -- novos
									CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
										sum(coalesce(ssi.seiqtdtecnico,0))
									END
								END
							END
					END as quantidade,
					sic.icovalor as valor
				FROM
					par.subacao s
				INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
			--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid
				INNER JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano AND sic.icoid IN ( ".implode(', ', $item)." )
				LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
				LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
				INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
				WHERE
					s.sbastatus = 'A' AND sd.sbdid IN ( ".$dados['sbdid']." ) AND
			
					 CASE WHEN sbacronograma = 1
                     THEN sic.icovalidatecnico <> 'N'
                     ELSE
                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
                        	se.sesvalidatecnico <> 'N'
                        ELSE
                        	sic.icovalidatecnico <> 'N'
                        END
                     END
                group by
					s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor";
		//		pr.prpid = ".$_SESSION['par_var']['prpid'];

		$arrItens[$dados['sbdid']] = $db->carregar( $sql );

		$cnpj = pegaCnpj($_SESSION['par_var']['inuid']);

		$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."'");

		$data_created = date("c");
		$usuario = USUARIO_SIGARP;
		$senha   = SENHA_SIGARP;

		$itEnviado = array();
		$itPerdido = array();
		$it = 0;
		$cont = 0;
		$montou = false;
		$ultimaSubacao = 0;
		$itens2 = array();

		$arqXmlSt =
		'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo>
        	<nu_seq_tipo_pagamento>'.$tipo.'</nu_seq_tipo_pagamento>';
		foreach( $arrItens as $subacao => $itens ){
			$monta = 0;
			if( $itens[0] ){
				foreach( $itens as $item ){
					$itens2[$cont]['sbdid'] = $item['sbdid'];
					$itens2[$cont]['icoid'] = $item['icoid'];
					$cont++;
					if($item['idsigarp']){
						$pregao = diponibilizaPregoesItens($item['idsigarp'],'pregao',$prpid);
						if( $item['quantidade'] <> 0 ){
							if( $pregao ){
								$it++;
								$montou = true;
								if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
									$arqXmlSt .= '
				        							</sub_acao>';
								}
								$ultimaSubacao = $subacao;
								if( $monta == 0 ){
									$monta++;
									$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
								}
								$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao>
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item>
											<qt_item>'.$item['quantidade'].'</qt_item>
											<vl_pagamento>'.$item['valor'].'</vl_pagamento>
										</item>';
								$itEnviado[] = array('sbdid' => $item['sbdid'],
										'icoid' => $item['icoid']);
							}
						}
					}
					$pregao = false;
				}
			}
		}
		if( $montou == true ){
			$arqXmlSt .= '
        					</sub_acao>';
		}
		$arqXmlSt .= '</params>
    </body>
</request>';

		$arqXml = <<<XML
{$arqXmlSt}
XML;

if( $it == 0 ){ //Não tem nenhum item para enviar!
	echo "WS-Sigarp: Nenhum item para enviar. Verifique os pregões.";
	return true;
}

/*
 echo '<pre>';
echo $arqXml;
die();
*/
if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
	$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
} else {
	$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
}
$xml = Fnde_Webservice_Client::CreateRequest()
->setURL($urlWS)
->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
->execute();

$xmlRetorno = $xml;
	
$xml = simplexml_load_string( stripslashes($xml));

$result = (integer) $xml->status->result;

//	print_r ($xml);
//	die();

if(!$result) { //ERRO
		
	$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'enviarManualmenteItensSigarp - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

	$db->executar($sql);
	$db->commit();
		
		
	$erros = $xml->status->error->message->text;
	echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							'.iconv("UTF-8", "ISO-8859-1", $erros).'
						</div>
					</div>
				 	<br>
					';
		
} else {
	// Deu certo
		
	$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'enviarManualmenteItensSigarp - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

	$db->executar($sql);
	$db->commit();
		
	if( is_array($itEnviado) && $itEnviado[0] ){
		foreach( $itEnviado as $it ){
			if($it['sbdid'] && $it['icoid'] && $_SESSION['par_var']['prpid'] ){
				$sqlDelete .= " DELETE FROM par.empenhopregaoitemperdido WHERE
									prpid = ".$_SESSION['par_var']['prpid']." AND
									sbdid = ".$it['sbdid']." AND
									icoid = ".$it['icoid']."; ";
			}
		}
		$db->executar($sqlDelete);
		$db->commit();
	}
		
	echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						SOLICITAÇÃO DE ADESÃO REALIZADA COM SUCESSO
					</div>
				 	<br>
					';
}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
						</div>
					</div>
				 	<br>
					';
				
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";

		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
						</div>
					</div>
				 	<br>
					';

	}


}

function enviarItensSigarp($dados){
	global $db;

	try {

		$sql = "SELECT
					sd.sbaid,
					sd.sbdano
				FROM
					par.subacaodetalhe sd
				INNER JOIN par.subacao s ON s.sbaid = sd.sbaid
				WHERE
					sd.sbdid IN (".implode(", ", $dados['chk']).") AND s.sbastatus = 'A'";

		$sbaids = $db->carregar($sql);

		$arrItens = array();
		foreach( $sbaids as $sba ){

			// Verifico os itens da subação
			$sql = "SELECT
						s.sbaid,
						sd.sbdano,
						sd.sbdid,
						sic.icoid,
						pic.idsigarp,
						CASE WHEN sbacronograma = 1 --global
						THEN
							CASE WHEN sic.icovalidatecnico IS NULL -- antigos
							THEN coalesce(sic.icoquantidade,0)
							ELSE -- novos
								CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
									sum(coalesce(sic.icoquantidadetecnico,0))
								END
							END
						ELSE -- escolas
							CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 )
							THEN -- escolas sem itens
								CASE WHEN se.sesvalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(se.sesquantidade,0))
									ELSE -- novos
										sum(coalesce(se.sesquantidadetecnico,0))
									END
								ELSE -- escolas com itens
									CASE WHEN sic.icovalidatecnico IS NULL -- antigos
									THEN
										sum(coalesce(ssi.seiqtd,0))
									ELSE -- novos
										CASE WHEN sic.icovalidatecnico = 'S' THEN -- validado (caso não o item não é contado)
											sum(coalesce(ssi.seiqtdtecnico,0))
										END
									END
								END
						END as quantidade,
						sic.icovalor as valor
					FROM
						par.subacao s
					INNER JOIN par.subacaodetalhe 		     sd  ON sd.sbaid = s.sbaid
				--	INNER JOIN par.processopar 	             pr  ON pr.prpid = sd.prpid
					LEFT  JOIN par.subacaoitenscomposicao 	 sic ON sic.sbaid = s.sbaid  AND sic.icoano = sd.sbdano
					LEFT  JOIN par.subacaoescolas 		     se  ON se.sbaid = sic.sbaid AND se.sesano = sic.icoano
					LEFT  JOIN par.subescolas_subitenscomposicao ssi ON ssi.sesid = se.sesid AND ssi.icoid = sic.icoid
					INNER JOIN par.propostaitemcomposicao        pic ON pic.picid = sic.picid
					WHERE
						s.sbastatus = 'A' AND sd.sbaid IN ( ".$sba['sbaid']." ) AND sd.sbdano IN ( ".$sba['sbdano']." ) AND

						 CASE WHEN sbacronograma = 1
	                     THEN sic.icovalidatecnico <> 'N'
	                     ELSE
	                     	CASE WHEN (s.frmid = 2) OR ( s.frmid = 4 AND s.ptsid = 42 ) OR ( s.frmid = 12 AND s.ptsid = 46 ) THEN
	                        	se.sesvalidatecnico <> 'N'
	                        ELSE
	                        	sic.icovalidatecnico <> 'N'
	                        END
	                     END
	                group by
						s.sbaid, sd.sbdano, sd.sbdid, sic.icoid, pic.idsigarp, s.sbacronograma, sic.icovalidatecnico, sic.icoquantidade, s.frmid, s.ptsid, se.sesvalidatecnico, sic.icovalor
					ORDER BY
						s.sbaid";
			//		pr.prpid = ".$_SESSION['par_var']['prpid'];

			$arrItens[$sba['sbaid']] = $db->carregar( $sql );
			$arrAno[$sba['sbaid']] = $sba['sbdano'];	
		}
			
		$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."'");

		$cnpj = pegaCnpj($_SESSION['par_var']['inuid']);
		/*
		 $cnpj = $db->pegaUm("SELECT iue.iuecnpj
		 		FROM par.instrumentounidade iu
		 		inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		 		WHERE
		 		iu.inuid = {$_SESSION['par_var']['inuid']}
		 		and iue.iuestatus = 'A'
		 		and iue.iuedefault = true");*/
		//$cnpj = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);

		$data_created = date("c");
		$usuario = USUARIO_SIGARP;
		$senha   = SENHA_SIGARP;

		$itPerdido = array();
		$it = 0;
		$cont = 0;
		$montou = false;
		$ultimaSubacao = 0;
		$itens2 = array();

		$arqXmlSt =
		'<?xml version="1.0" encoding="ISO-8859-1"?>
<request>
    <header>
        <app>SIGARP</app>
        <version>1.0</version>
        <created>'.$data_created.'</created>
    </header>
    <body>
        <auth>
            <usuario>'.$usuario.'</usuario>
            <senha>'.$senha.'</senha>
        </auth>
        <params>
        	<cnpj>'.$cnpj.'</cnpj>
        	<nu_processo>'.$dadosse['prpnumeroprocesso'].'</nu_processo>
        	<nu_seq_tipo_pagamento>1</nu_seq_tipo_pagamento>';
		foreach( $arrItens as $subacao => $itens ){
			$monta = 0;
			if( $itens[0] ){
				foreach( $itens as $item ){
					$itens2[$cont]['sbdid'] = $item['sbdid'];
					$itens2[$cont]['icoid'] = $item['icoid'];
					$cont++;
					if($item['idsigarp']){
						$pregao = diponibilizaPregoesItens($item['idsigarp'],'pregao');
						if( $item['quantidade'] <> 0 ){
							if( $pregao ){
								$it++;
								$montou = true;
								if( $subacao != $ultimaSubacao && $ultimaSubacao != 0 ){
									$arqXmlSt .= '
				        							</sub_acao>';
								}
								$ultimaSubacao = $subacao;
								if( $monta == 0 ){
									$monta++;
									$arqXmlSt .= '
				        							<sub_acao>
				        								<nu_sub_acao>'.$subacao.'</nu_sub_acao>';
								}
								$arqXmlSt .= '
										<item>
											<nu_seq_pregao>'.$pregao.'</nu_seq_pregao>
											<nu_seq_item>'.$item['idsigarp'].'</nu_seq_item>
											<qt_item>'.$item['quantidade'].'</qt_item>
											<vl_pagamento>'.diponibilizaPregoesItens($item['idsigarp'],'valor').'</vl_pagamento>
										</item>';
							} else {
								$itPerdido[] = array('sbdid' => $item['sbdid'],
										'icoid' => $item['icoid']);
							}
						}
					}
					$pregao = false;
				}
			}
		}
		if( $montou == true ){
			$arqXmlSt .= '
        					</sub_acao>';
		}
		$arqXmlSt .= '</params>
    </body>
</request>';

		//ver(simec_htmlentities($arqXmlSt));
		//die();

		$arqXml = <<<XML
{$arqXmlSt}
XML;


if( $it == 0 ){ //Não tem nenhum item para enviar!
	foreach( $itens2 as $item ){ // Salvo todos os itens que estão sendo empenhados!
		if($item['sbdid'] && $item['icoid'] && $_SESSION['par_var']['prpid'] ){
			$sqlInsertItem .= " INSERT INTO par.empenhopregaoitemperdido (
								prpid,
								sbdid,
								icoid )
							VALUES
								(".$_SESSION['par_var']['prpid'].",
								".$item['sbdid'].",
								".$item['icoid']."); ";
		}
	}
	$db->executar($sqlInsertItem);
	$db->commit();
	return true;
}

if( $_SESSION['baselogin'] == "simec_desenvolvimento" ){
	$urlWS = 'http://172.20.65.98/webservices/wssigarp/ws/item';
} else {
	$urlWS = 'http://www.fnde.gov.br/webservices/wssigarp/ws/item';
}

$xml = Fnde_Webservice_Client::CreateRequest()
->setURL($urlWS)
->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
->execute();

$xmlRetorno = $xml;
	
$xml = simplexml_load_string( stripslashes($xml));

$result = (integer) $xml->status->result;

if(!$result) { //ERRO
		
	$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'enviarItensSigarp - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

	$db->executar($sql);
	$db->commit();
		
		
	$erros = $xml->status->error->message->text;
	echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							'.iconv("UTF-8", "ISO-8859-1", $erros).'
						</div>
					</div>
				 	<br>
					';
		
	if( is_array($itens2) && $itens2[0] ){
		foreach( $itens2 as $item ){
			if($item['sbdid'] && $item['icoid'] && $_SESSION['par_var']['prpid'] ){
				$sqlInsert .= " INSERT INTO par.empenhopregaoitemperdido (
								prpid,
								sbdid,
								icoid )
							VALUES
								(".$_SESSION['par_var']['prpid'].",
								".$item['sbdid'].",
								".$item['icoid']."); ";
			}
		}
		$db->executar($sqlInsert);
		$db->commit();
	}
		
	/*
	 echo "*** Descrição do erro ***\n\n";
	$erros = $xml->status->error->message->text;
	echo "* ".iconv("UTF-8", "ISO-8859-1", $erros);
	*/
} else {
	// Deu certo
		
	$dados = ($xml->body->params);
	foreach( $dados->adesaopregao as $adesao ){
		$sub = (string) $adesao->nu_sub_acao;
		$ano = $arrAno[$sub];
		$ad = (integer) $adesao->nu_seq_solicitacao_adesao;
		foreach( $adesao->item as $item ){
			$it = (integer) $item->nu_seq_item;
			$sqlItensEnviados .= "INSERT INTO par.empenhopregaoitensenviados (prpid, sbaid, epiano, idsigarp, adesao, usucpf) VALUES ( ".$_SESSION['par_var']['prpid'].", ".$sub.", ".$ano.", ".$it.", ".$ad.", '".$_SESSION['usucpf']."' );";
		}
	}
		
	if( $sqlItensEnviados ){
		$db->executar( $sqlItensEnviados );
	}
		
	$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'enviarItensSigarp - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

	$db->executar($sql);
	$db->commit();
		
	if( is_array($itPerdido) && $itPerdido[0] ){
		foreach( $itPerdido as $it ){
			if($_SESSION['par_var']['prpid'] && $it['sbdid'] && $it['icoid'] ){
				$sqlInsert .= " INSERT INTO par.empenhopregaoitemperdido (
									prpid,
									sbdid,
									icoid )
								VALUES
									(".$_SESSION['par_var']['prpid'].",
									".$it['sbdid'].",
									".$it['icoid']."); ";
			}
		}
		$db->executar($sqlInsert);
		$db->commit();
	}
		
	return true;
}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.".'\n';
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-Serviço do SIGARP encontra-se temporariamente indisponível. Favor tente mais tarde.
						</div>
					</div>
				 	<br>
					';
				
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		//echo "Erro-WS Enviar dados para o SIGARP: $erroMSG";

		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
						ERRO AO TENTAR ADERIR AO PREGÃO
						<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
							Erro-WS Enviar dados para o SIGARP: '.$erroMSG.'
						</div>
					</div>
				 	<br>
					';

	}
}

function solicitarEmpenho($dados, $nu_convenio = null ,$an_exercicio = null) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

		$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prpbanco, prpagencia, muncod, prptipo, inuid, prptipoexecucao FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."'");
		$emendas = $db->carregar("SELECT DISTINCT sbaemenda FROM par.subacao s
	    			inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid
	    			WHERE sbdid IN (".implode(', ', $dados['chk']).") ");
	  
		$totalsubEmenda = count($emendas);

		if($totalsubEmenda > 1){
			if(is_array($emendas) && $emendas[0]){
				foreach($emendas as $emenda ){
					if($emenda['sbaemenda'] == NULL or $emenda['sbaemenda'] == '' ){
						echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
							REGRA DE NEGÓCIO PARA EMPENHO
							<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
								Uma das Subações é de Emenda e outra não. É necessário empenhar separadamente.
							</div>
						</div>
					 	<br>
						';

						return false;
					}
				}
			}
		}
	  

		$tipoemenda = $emendas[0]['sbaemenda'];
		// PI
		$sql = "select distinct sbdplanointerno
	    		from par.subacaodetalhe
	    		where sbdid IN (".implode(", ", $dados['chk']).") ";
		$pis = $db->carregar($sql);
		$totalPis = count($pis);
		if($totalPis > 1){
			//echo "Existe subações com PIs diferente. So pode ser empenhado subações com mesmo PIs.";

			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					REGRA DE NEGÓCIO PARA EMPENHO
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Existe subações com PIs diferente. So pode ser empenhado subações com mesmo PIs.
					</div>
				</div>
			 	<br>
				';

			return false;
		}
		if(is_array($pis) && $pis[0] ){
			foreach($pis as $pi){
				if($pi['sbdplanointerno'] == '' || $pi['sbdplanointerno'] == NULL ){
					//echo "Existe subações sem PI Cadastrado. É necessário inserir os PIs nas subações.";
					echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
								REGRA DE NEGÓCIO PARA EMPENHO
								<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
									Existe subações sem PI Cadastrado. É necessário inserir os PIs nas subações. <br>
									Clique no botão: <img src="../imagens/consultar.gif\" title="Visualizar Subação" border="0"> para visualizar a subação.
								</div>
							</div>
						 	<br>
							';
					return false;
				}
			}
		}

		//PTRES
		$sql = "select distinct sbdptres
	    		from par.subacaodetalhe
	    		where sbdid IN (".implode(", ", $dados['chk']).") ";
		$ptres = $db->carregar($sql);
		$totalPtres = count($ptres);
		if($totalPtres > 1){
			// echo "Existe PIs com PTRES diferente. So pode ser empenhado subações com mesmo PTRES.";

			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
								REGRA DE NEGÓCIO PARA EMPENHO
								<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
									Existe PIs com PTRES diferente. So pode ser empenhado subações com mesmo PTRES.
								</div>
							</div>
						 	<br>
							';

			return false;
		}

		$PI = $db->pegaLinha("select distinct sbdplanointerno, sbdptres
								from par.subacaodetalhe s
								inner join par.subacao sb on sb.sbaid = s.sbaid
								WHERE
									s.sbdid IN (".implode(", ", $dados['chk']).")");


		// Caso ele mude nas opções eu reconheco o que ele marcou agora.
		if( $dados['planointerno'] && $dados['ptres'] ){
			$PI['sbdplanointerno'] = $dados['planointerno'];
			$PI['sbdptres'] = $dados['ptres'];
		}

		$sql = "SELECT
					(((((((pt.funcod::text) || pt.sfucod::text)) || pt.prgcod::text)) || pt.acacod::text)) || pt.loccod::text AS fupfuncionalprogramatica
				FROM monitora.pi_planointerno pi
				inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
				inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
				WHERE
					pt.ptres = '".$PI['sbdptres']."'";

		$funcional = $db->pegaUm( $sql );

		if($dadosse) {
			/*
			 $sql = "select distinct sbanaturezadespesa
			from par.subacaodetalhe s
			inner join par.subacao sb on sb.sbaid = s.sbaid
			WHERE
			s.sbdid IN (".implode(", ", $dados['chk']).")";

				
			$naturezaDespesa = $db->pegaUm( $sql );
			*/
			 
			if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
			$_SESSION['baselogin'] == "simec_espelho_producao" ){

				$nu_processo='23034655466200900';
				$co_fonte_recurso_solic="0100000000";
				$co_plano_interno_solic= $PI['sbdplanointerno'];
				//$co_ptres_solic="020990";
				$co_ptres_solic = $PI['sbdptres'];
				$co_natureza_despesa_solic="44504200";



			} else {
					
				$nu_processo=$dadosse['prpnumeroprocesso'];
				// constante=4042
				$co_natureza_despesa_solic="44404200";

				// fonte selecionada pelo usuario no formulario
				$co_fonte_recurso_solic=$dados['fonte'];

				if($dadosse['protipo'] == 'P') {
					// constante=MEC00001
					$co_plano_interno_solic= $PI['sbdplanointerno'];
					// constante=037825
					//$co_ptres_solic="024788";
					$co_ptres_solic = $PI['sbdptres'];
					// constante=12365144812KU0001
					//$frpfuncionalprogramatica="14480E53262980001";
					$frpfuncionalprogramatica = $funcional;
				} else {
					// constante=MEC00002
					$co_plano_interno_solic= $PI['sbdplanointerno'];
					// constante=037826
					//$co_ptres_solic="024788";
					$co_ptres_solic = $PI['sbdptres'];
					// constante=12365144812KV0001
					//$frpfuncionalprogramatica="14480E53262980001";
					$frpfuncionalprogramatica = $funcional;
				}
			}
		}

		$nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
		/*
		 $nu_cnpj_favorecido = $db->pegaUm("SELECT iue.iuecnpj
		 		FROM par.instrumentounidade iu
		 		inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		 		WHERE
		 		iu.inuid = {$_SESSION['par_var']['inuid']}
		 		and iue.iuestatus = 'A'
		 		and iue.iuedefault = true");*/
		// $nu_cnpj_favorecido = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);

		if($_SESSION['par_var']['esfera']=='estadual') {
			$co_natureza_despesa_solic = '44304200';
		}else{
			$co_natureza_despesa_solic = '44404200';
		}

		if($_SESSION['par_var']['inuid'] == 1 ){ // DF
			$co_natureza_despesa_solic = '44304200';
		}

		$nu_sistema="7";
		if($dadosse['prptipoexecucao'] == 'C'){
			//$co_centro_gestao_solic="61500000000";
			$co_programa_fnde="03";
			$an_convenio=$an_exercicio;
			$nu_convenio=$nu_convenio;
			$co_tipo_empenho="5";
		}else if($dadosse['prptipoexecucao'] == 'T'){
			// $co_centro_gestao_solic="61700000000";
			$co_programa_fnde= "CM"; // "ED";
			$an_convenio=null;
			$nu_convenio=null;
			$co_tipo_empenho="3";
		}
	  
		// nulo
		$nu_empenho_original=null;
		// nulo
		$an_exercicio_original=null;
		// total do empenho, calculado na tela
		$vl_empenho=str_replace(array(".",","),array("","."),$dados['name_total']);
		// constante=01
		$co_especie_empenho="01";
		// constante=1
		$co_esfera_orcamentaria_solic="1";
		// constante=2
		$co_observacao="2";

		$co_descricao_empenho="0010";
		// constante=15253
		$co_gestao_emitente="15253";
		// condição tipoobra=5(Quadra) entao programa=CN senao programa=BW
		// constante=153173
		$co_unidade_gestora_emitente="153173";
		// constante=26298
		$co_unidade_orcamentaria_solic="26298";
		// nulo
		$nu_proposta_siconv=null;

		// Centro de Gestão -  selecionada pelo usuario no formulario
		$co_centro_gestao_solic=$dados['gestaosolicitacao'];

		if( !in_array( PAR_PERFIL_SUPER_USUARIO, pegaArrayPerfil($_SESSION['usucpf']) ) ){
			// Caso seja uma subação de Emenda o Sistema muda o centro de gestão para o abaixo
			if($tipoemenda){
				$co_centro_gestao_solic="61700000000";
			}
		}

		$co_natureza_despesa_solic  = $dados['naturezadespesasolicitacao'];

		if($co_plano_interno_solic == 'BS892910806'){
			$co_esfera_orcamentaria_solic = "2";
		}



		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
		<nu_cnpj_favorecido>$nu_cnpj_favorecido</nu_cnpj_favorecido>
		<nu_empenho_original>$nu_empenho_original</nu_empenho_original>
		<an_exercicio_original>$an_exercicio_original</an_exercicio_original>
		<vl_empenho>$vl_empenho</vl_empenho>
		<nu_processo>$nu_processo</nu_processo>
		<co_especie_empenho>$co_especie_empenho</co_especie_empenho>
		<co_plano_interno_solic>$co_plano_interno_solic</co_plano_interno_solic>
		<co_esfera_orcamentaria_solic>$co_esfera_orcamentaria_solic</co_esfera_orcamentaria_solic>
		<co_ptres_solic>$co_ptres_solic</co_ptres_solic>
		<co_fonte_recurso_solic>$co_fonte_recurso_solic</co_fonte_recurso_solic>
		<co_natureza_despesa_solic>$co_natureza_despesa_solic</co_natureza_despesa_solic>
		<co_centro_gestao_solic>$co_centro_gestao_solic</co_centro_gestao_solic>
		<an_convenio>$an_convenio</an_convenio>
		<nu_convenio>$nu_convenio</nu_convenio>
		<co_observacao>$co_observacao</co_observacao>
		<co_tipo_empenho>$co_tipo_empenho</co_tipo_empenho>
		<co_descricao_empenho>$co_descricao_empenho</co_descricao_empenho>
		<co_gestao_emitente>$co_gestao_emitente</co_gestao_emitente>
		<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		<co_unidade_gestora_emitente>$co_unidade_gestora_emitente</co_unidade_gestora_emitente>
		<co_unidade_orcamentaria_solic>$co_unidade_orcamentaria_solic</co_unidade_orcamentaria_solic>
		<nu_proposta_siconv>$nu_proposta_siconv</nu_proposta_siconv>
		<nu_sistema>$nu_sistema</nu_sistema>
		</params>
	</body>
</request>
XML;

		if( $dados['tipo'] == 'visualiza' ){
			echo '<pre>';
			echo simec_htmlentities($arqXml);
			exit;
		}
		 
		//echo $arqXml;
		//return false;
		if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
			$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/orcamento/ne';
		} else {
			$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/orcamento/ne';
		}

		$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
		->execute();



		/*
		 $xml = <<<XML
		<?xml version='1.0' encoding='iso-8859-1'?>
		<response><header><app>SIGEF</app><version>v0.0.3</version><created>2011-05-20T17:42:11</created></header><status><result>1</result><message><code>1</code><text>Solicitação realizada com sucesso!</text></message></status><body><nu_seq_ne>170245</nu_seq_ne></body></response>
		XML;
		*/
		$xmlRetorno = $xml;
		//echo $xmlRetorno;
		$xml = simplexml_load_string( stripslashes($xml));

		//echo "------ SOLICITAÇÃO DE EMPENHO ------\n\n";
		//echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";

		$result = (integer) $xml->status->result;
		if(!$result) {
				
			$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
				ERRO AO TENTAR SOLICITAR O EMPENHO:
				<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " > ';
				
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					//echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $err->text).' ';
				}
			}
			$mensagem .='</div>
						</div>
		 				<br>';
			echo $mensagem;
				
			//echo "*** Descrição do erro ***\n\n";
			/*
			 $erros = $xml->status->error->message;
			if(count($erros)>0) {
			foreach($erros as $err) {
			echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
			}
			}
			*/
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarEmpenho - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return false;
		} else {

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarEmpenho - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);


			$sql = "INSERT INTO par.empenho(
            empcnpj,
            empnumerooriginal,
            empanooriginal,
            empvalorempenho,
            empnumeroprocesso,
            empcodigoespecie,
            empcodigopi,
            empcodigoesfera,
            empcodigoptres,
            empfonterecurso,
            empcodigonatdespesa,
            empcentrogestaosolic,
            empanoconvenio,
            empnumeroconvenio,
            empcodigoobs,
            empcodigotipo,
            empdescricao,
            empgestaoeminente,
            empunidgestoraeminente,
            empprogramafnde,
            empnumerosistema,
            usucpf,
            empprotocolo,
            empsituacao
            )
		    VALUES (".(($nu_cnpj_favorecido)?"'".$nu_cnpj_favorecido."'":"NULL").",
		    		".(($nu_empenho_original)?"'".$nu_empenho_original."'":"NULL").",
		    		".(($an_exercicio_original)?"'".$an_exercicio_original."'":"NULL").",
		    		".(($vl_empenho)?"'".$vl_empenho."'":"NULL").",
		            ".(($nu_processo)?"'".$nu_processo."'":"NULL").",
		            ".(($co_especie_empenho)?"'".$co_especie_empenho."'":"NULL").",
		            ".(($co_plano_interno_solic)?"'".$co_plano_interno_solic."'":"NULL").",
		            ".(($co_esfera_orcamentaria_solic)?"'".$co_esfera_orcamentaria_solic."'":"NULL").",
		            ".(($co_ptres_solic)?"'".$co_ptres_solic."'":"NULL").",
		            ".(($co_fonte_recurso_solic)?"'".$co_fonte_recurso_solic."'":"NULL").",
		            ".(($co_natureza_despesa_solic)?"'".$co_natureza_despesa_solic."'":"NULL").",
		            ".(($co_centro_gestao_solic)?"'".$co_centro_gestao_solic."'":"NULL").",
		            ".(($an_convenio)?"'".$an_convenio."'":"NULL").",
		            ".(($nu_convenio)?"'".$nu_convenio."'":"NULL").",
		            ".(($co_observacao)?"'".$co_observacao."'":"NULL").",
		            ".(($co_tipo_empenho)?"'".$co_tipo_empenho."'":"NULL").",
		            ".(($co_descricao_empenho)?"'".$co_descricao_empenho."'":"NULL").",
		            ".(($co_gestao_emitente)?"'".$co_gestao_emitente."'":"NULL").",
		            ".(($co_unidade_gestora_emitente)?"'".$co_unidade_gestora_emitente."'":"NULL").",
		            ".(($co_programa_fnde)?"'".$co_programa_fnde."'":"NULL").",
		            ".(($nu_sistema)?"'".$nu_sistema."'":"NULL").",
		            '".$_SESSION['usucpf']."',
		            '".$xml->body->nu_seq_ne."',
		            '8 - SOLICITAÇÃO APROVADA'
		            ) RETURNING empid;";

			$empid = $db->pegaUm($sql);

			$sql = "INSERT INTO par.historicoempenho(
            		usucpf, empid, hepdata, empsituacao)
    				VALUES ('".$_SESSION['usucpf']."', '".$empid."', NOW(), '8 - SOLICITAÇÃO APROVADA');";

			$db->executar($sql);

			if($dados['chk']) {
				foreach($dados['chk'] as $sbdid)
				{
					$valor = str_replace('.','', $dados['name_vlr_'.$sbdid]);
					$valorPorSubacao = str_replace(',','.', $valor);
					//					$valorPorSubacao = str_replace(array(".",","),array("","."),number_format($dados['name_vlr_'.$sbdid], 2, ',', '.'));
					//					$valorPorSubacao = str_replace(array(".",","),array("","."),number_format($dados['vlrTotal_'.$sbdid], 2, ',', '.'));
					//					$valorPorSubacao = str_replace(array(".",","),array("","."),$dados['vlr_'.$sbaid]);
					$sql = "SELECT sbaid, sbdano FROM par.subacaodetalhe WHERE sbdid = ".$sbdid;
					$linha = $db->pegaLinha( $sql );
						
					$sql = "INSERT INTO par.empenhosubacao(
            				sbaid, empid, eobpercentualemp, eobvalorempenho, eobano)
    						VALUES ('".$linha['sbaid']."', '".$empid."', '".$dados['name_'.$sbdid]."', '".$valorPorSubacao."', '".$linha['sbdano']."');";

					$db->executar($sql);
						
					$sql = "UPDATE par.subacaodetalhe SET sbdptres = '".$co_ptres_solic."' , sbdplanointerno = '".$co_plano_interno_solic."' WHERE sbdid = ".$sbdid;
					$db->executar($sql);
						
					if( $dados['name_'.$sbdid] == '100' ){
						$sql = "UPDATE par.subacaodetalhe SET prpid = ".$_SESSION['par_var']['prpid'].", ssuid = 13 WHERE sbdid = ".$sbdid;
					} else {
						$sql = "UPDATE par.subacaodetalhe SET prpid = ".$_SESSION['par_var']['prpid'].", ssuid = 18  WHERE sbdid = ".$sbdid;
					}
					$db->executar($sql);
					$db->commit();
				}
			}
				
				
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					EMPENHO EFETIVADO COM SUCESSO
					</div>
			 	<br>
				';
				
			$db->commit();
			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					ERRO AO EXECUTAR A SOLICITAÇÃO DE EMPENHO:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-Serviço encontra-se temporariamente indisponível.Favor tente mais tarde.
					</div>
				</div>
			 	<br>
				';
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";
		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					ERRO AO EXECUTAR A SOLICITAÇÃO DE EMPENHO:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro: '.$erroMSG.'
					</div>
				</div>
			 	<br>
				';


	}
}

function solicitarProcesso($dados) {
	global $db;

	$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prptipoexecucao, muncod, prpbanco, prpagencia, prpdatainclusao, usucpf, prpseqconta, prptipo, seq_conta_corrente, nu_conta_corrente, inuid
    						   FROM par.processopar
    						   WHERE prpid='".$_SESSION['par_var']['prpid']."'");

	if($dadosse) {
		$an_processo = date("Y");
		$nu_processo=$dadosse['prpnumeroprocesso'];
		$tp_processo=1; // O que vai ser no PAR

		if($dadosse['prptipoexecucao'] == 'C'){
			$co_programa_fnde="03";
		}else if($dadosse['prptipoexecucao'] == 'T'){
			$co_programa_fnde= "CM";//"ED";
		}
		 
		$nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
		/*
		 $nu_cnpj_favorecido = $db->pegaUm("SELECT iue.iuecnpj
		 		FROM par.instrumentounidade iu
		 		inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		 		WHERE
		 		iu.inuid = {$_SESSION['par_var']['inuid']}
		 		and iue.iuestatus = 'A'
		 		and iue.iuedefault = true");*/
		// $nu_cnpj_favorecido = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
	  
	}

	$data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	//$usuario = 'MECTIAGOT';
	//$senha   = 'M3135689';

	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<params>
      	<nu_cnpj>$nu_cnpj_favorecido</nu_cnpj>
      	<nu_processo>$nu_processo</nu_processo>
      	<tp_processo>$tp_processo</tp_processo>
      	<an_processo>$an_processo</an_processo>
      	<co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;
	//echo $arqXml;
	//return true;

	//	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
	//		$urlWS = 'http://172.20.200.116/webservices/corp/integracao/web/dev.php/processo/solicitar';
	//	} else {
	$urlWS = 'http://www.fnde.gov.br/webservices/corp/index.php/processo/solicitar';
	//	}

	$xml = Fnde_Webservice_Client::CreateRequest()
	->setURL($urlWS)
	->setParams( array('xml' => $arqXml, 'login' => $usuario, 'senha' => $senha) )
	->execute();

	$xmlRetorno = $xml;

	$xml = simplexml_load_string( stripslashes($xml));

	//echo "\n\n------ SOLICITAÇÃO DE PROCESSO ------\n\n";
	//echo "|".$xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";


	$result = (integer) $xml->status->result;

	if(!$result) {

		$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
							ERRO AO SOLICITAR PROCESSO NO SISTEMA DOCUMENTA:
		 					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >';
			
		$erros = $xml->status->error->message;
		if(count($erros)>0) {
			foreach($erros as $err) {
				$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
			}
		}
		$mensagem .= '</div>
			 			</div>
			 			<br>';

		echo $mensagem;
			
		//echo "*** Descrição do erro ***\n\n";
		/*
			$erros = $xml->status->error->message;
		if(count($erros)>0) {
		foreach($erros as $err) {
		echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
		echo "\n";
		}
		}
		*/
		$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarProcesso - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();

		return false;
	} else {

		$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarProcesso - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();

		return true;
	}

}

function atualizaDadosContaCorrentePar($dados) {
	global $db;

	$dadosse = $db->pegaLinha("SELECT prpnumeroprocesso, prptipoexecucao, muncod, prpbanco, prpagencia, prpdatainclusao, usucpf, prpseqconta, prptipo, seq_conta_corrente, nu_conta_corrente, inuid
    						   FROM par.processopar
    						   WHERE prpid='".$_SESSION['par_var']['prpid']."'");

	if($dadosse) {
		$an_processo = date("Y");
		$nu_processo=$dadosse['prpnumeroprocesso'];
		$tp_processo=1; // O que vai ser no PAR

		if($dadosse['prptipoexecucao'] == 'C'){
			$co_programa_fnde="03";
		}else if($dadosse['prptipoexecucao'] == 'T'){
			$co_programa_fnde= "CM";//"ED";
		}
		 
		$nu_cnpj_favorecido = pegaCnpj($_SESSION['par_var']['inuid']);
		/*
		 $nu_cnpj_favorecido = $db->pegaUm("SELECT iue.iuecnpj
		 		FROM par.instrumentounidade iu
		 		inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		 		WHERE
		 		iu.inuid = {$_SESSION['par_var']['inuid']}
		 		and iue.iuestatus = 'A'
		 		and iue.iuedefault = true");*/
		//$nu_cnpj_favorecido = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);
	  
	}

	$data_created = date("c");
	$usuario = $dados['wsusuario'];
	$senha   = $dados['wssenha'];
	$somente_conta_ativa	= 'N';
	$numero_de_linhas		= '200';
	/*$usuario = 'MECTIAGOT';
	 $senha   = 'M3135689';*/

	$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>{$data_created}</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
			<nu_identificador>$nu_cnpj_favorecido</nu_identificador>
			<nu_processo>$nu_processo</nu_processo>
			<somente_conta_ativa>$somente_conta_ativa</somente_conta_ativa>
			<numero_de_linhas>$numero_de_linhas</numero_de_linhas>
		</params>
	</body>
</request>
XML;

	if ( $_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		//$urlWS = 'http://dev.fnde.gov.br/webservices/sigef/integracao/public/index.php/financeiro/cr';
		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		//$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	} else {
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
	}
ver($arqXml,d);
	$xml = Fnde_Webservice_Client::CreateRequest()
	->setURL($urlWS)
	->setParams( array('xml' => $arqXml, 'method' => 'consultarAndamentoCC') )
	->execute();

	$xmlRetorno = $xml;

	$xml = simplexml_load_string( stripslashes($xml));

	//echo "\n\n------ SOLICITAÇÃO DE PROCESSO ------\n\n";
	//echo "|".$xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";


	$result = (integer) $xml->status->result;

	if(!$result) {

		$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
							ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF:
		 					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >';
			
		$erros = $xml->status->error->message;
		if(count($erros)>0) {
			foreach($erros as $err) {
				$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $err->text);
			}
		}
		$mensagem .= '</div>
			 			</div>
			 			<br>';

		echo $mensagem;

		$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

		$db->executar($sql);
		$db->commit();

		return false;
	} else {
		$obContaCorrenteWS = $xml->body->row->children();
			
		$seq_solic_cr 		= !empty($obContaCorrenteWS->seq_solic_cr) ? "'".(int)$obContaCorrenteWS->seq_solic_cr."'" : 'null';
		$seq_conta 			= !empty($obContaCorrenteWS->seq_conta) ? "'".(int)$obContaCorrenteWS->seq_conta."'" : 'null';
		$dt_movimento 		= !empty($obContaCorrenteWS->dt_movimento) ? "'".(string)$obContaCorrenteWS->dt_movimento."'" : 'null';
		$nu_banco 			= !empty($obContaCorrenteWS->nu_banco) ? "'".(string)$obContaCorrenteWS->nu_banco."'" : 'null';
		$nu_agencia 		= !empty($obContaCorrenteWS->nu_agencia) ? "'".(string)$obContaCorrenteWS->nu_agencia."'" : 'null';
		$nu_conta_corrente	= !empty($obContaCorrenteWS->nu_conta_corrente) ? "'".(string)$obContaCorrenteWS->nu_conta_corrente."'" : 'null';
		$fase_solicitacao	= !empty($obContaCorrenteWS->fase_solicitacao) ? "'".(string)$obContaCorrenteWS->fase_solicitacao."'" : 'null';
		$co_situacao_conta	= !empty($obContaCorrenteWS->co_situacao_conta) ? "'".(string)$obContaCorrenteWS->co_situacao_conta."'" : 'null';
		$situacao_conta 	= !empty($obContaCorrenteWS->situacao_conta) ? "'".(string)$obContaCorrenteWS->situacao_conta."'" : 'null';
		$nu_processo 		= !empty($obContaCorrenteWS->nu_processo) ? "'".(string)$obContaCorrenteWS->nu_processo."'" : 'null';
		$nu_identificador 	= !empty($obContaCorrenteWS->nu_identificador) ? "'".(string)$obContaCorrenteWS->nu_identificador."'" : 'null';
		$ds_razao_social 	= !empty($obContaCorrenteWS->ds_razao_social) ? "'".(string)$obContaCorrenteWS->ds_razao_social."'" : 'null';
		$ds_problema		= "'-'";
		$rnum 				= (int)		$obContaCorrenteWS->rnum;
		$status 			= (string)	$obContaCorrenteWS->status;
		$co_status			= substr( $status, 0, 1 );
			
		if( trim($co_status) != 0 ){
			$sql = "UPDATE par.processopar SET
			prpbanco = $nu_banco,
			prpagencia = $nu_agencia,
			prpseqconta = $seq_solic_cr,
			seq_conta_corrente = $seq_conta,
			nu_conta_corrente = $nu_conta_corrente,
			dt_movimento = $dt_movimento,
			fase_solicitacao = $fase_solicitacao,
			co_situacao_conta = $co_situacao_conta,
			situacao_conta = $situacao_conta,
			nu_identificador = $nu_identificador,
			ds_razao_social = $ds_razao_social
			WHERE
			prpid = {$_SESSION['par_var']['prpid']}";
					$db->executar($sql);
					$db->commit();

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

				            $db->executar($sql);
				            $db->commit();

				            return true;

				            } else {
				$mensagem = '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
							ERRO AO ATUALIZAR DADOS CONTA CORRENTE NO SIGEF:
		 					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >';
		 						
			
				$mensagem .= ' Descrição: '.iconv("UTF-8", "ISO-8859-1", $status);
				$mensagem .= '</div>
						</div>
						<br>';
							
						echo $mensagem;
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				        VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'atualizaDadosContaCorrentePar - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    				NOW(),
				    						'".$_SESSION['usucpf']."');";

				    						$db->executar($sql);
			$db->commit();
				return false;
			}
		}

}

function consultarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

		$prpseqconta = $db->pegaUm("SELECT prpseqconta FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."'");
		/*
		 if(!$prpseqconta) {
		$existeAndamentoConta = consultarAndamentoContaCorrente($dados);
		if(!$existeAndamentoConta){
		// RETORNO FALSE - SE NÃO EXISTE CONTA  EM ANDAMENTO PARA SER ABERTA SOLICITA CONTA.
		$r = solicitarContaCorrente($dados);
		if($r){
		return "cc_criado_sucesso";
		}else{
		return false;
		}
		}
		}*/

		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <seq_solic_cr>$prpseqconta</seq_solic_cr>
		</params>
	</body>
</request>
XML;

		//	if($_SESSION['baselogin'] == "simec_desenvolvimento" || $_SESSION['baselogin'] == "simec_espelho_producao" ){
		//		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		//	} else {
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		//	}

		if($prpseqconta) {

			$xml = Fnde_Webservice_Client::CreateRequest()
			->setURL($urlWS)
			->setParams( array('xml' => $arqXml, 'method' => 'consultar') )
			->execute();

			$xmlRetorno = $xml;

			$xml = simplexml_load_string( stripslashes($xml));

			if($xml->body->row->seq_conta) {
				$db->executar("UPDATE par.processopar SET nu_conta_corrente='".$xml->body->row->nu_conta_corrente."', seq_conta_corrente='".$xml->body->row->seq_conta."' WHERE prpseqconta='".$prpseqconta."'");
				$db->commit();
			}

			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					CONSULTA DE CONTA CORRENTE EFETUADA COM SUCESSO:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Status da conta: '.iconv("UTF-8", "ISO-8859-1", $xml->body->row->status).'<br>
						Fase solicitação: '.(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-').'
					</div>
				</div>
			 	<br>
				';
			/*
			 echo "------ CONSULTA DE CONTA CORRENTE ------\n\n";
			echo iconv("UTF-8", "ISO-8859-1", $xml->body->row->status)."\n\n";
			echo "*** Detalhes da consulta ***\n\n";
			echo "* Data movimento:".(($xml->body->row->dt_movimento)?$xml->body->row->dt_movimento:'-')."\n";
			echo "* Fase solicitação:".(($xml->body->row->fase_solicitacao)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->fase_solicitacao):'-')."\n";
			echo "* Entidade:".(($xml->body->row->ds_razao_social)?iconv("UTF-8", "ISO-8859-1", $xml->body->row->ds_razao_social):'-')."(".(($xml->body->row->nu_identificador)?$xml->body->row->nu_identificador:'-').")\n\n";
			*/
			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'consultarContaCorrente',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			$result = (integer) $xml->status->result;

			if($result) {
				return false;
			} else {
				return true;
			}

		} else {
			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
				
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					ERRO AO CONSULTAR CONTA CORRENTE:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.
					</div>
				</div>
			 	<br>
				';
				
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";

		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					ERRO AO CONSULTAR CONTA CORRENTE:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-WS Consultar Conta Corrente no SIGEF: '.$erroMSG.'
					</div>
				</div>
			 	<br>
				';

	}
}

function solicitarContaCorrente($dados) {
	global $db;

	try {

		$data_created = date("c");
		$usuario = $dados['wsusuario'];
		//$usuario = 'MECTIAGOT';
		$senha   = $dados['wssenha'];
		//$senha   = 'M3135689';

		$dadoscc = $db->pegaLinha("SELECT prpnumeroprocesso,prptipoexecucao, prpbanco, prpagencia, muncod, prptipo, inuid FROM par.processopar WHERE prpid='".$_SESSION['par_var']['prpid']."'");

		if($dadoscc) {
			// numero do processo (No desenvolvimento é fixo)
			//	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
			//	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
			//$nu_processo='23034655466200900';
			//	   	$nu_processo=$dadoscc['prpnumeroprocesso'];//234000005642011
			//	} else {
			$nu_processo=$dadoscc['prpnumeroprocesso'];
			//	}

			// constante=001
			$nu_banco=$dadoscc['prpbanco'];
			// esperando envio
			$nu_agencia=trim((int)$dadoscc['prpagencia']);
		}

		// CNPJ da prefeitura
		$nu_identificador = pegaCnpj($_SESSION['par_var']['inuid']);
		/*
		 $nu_identificador = $db->pegaUm("SELECT iue.iuecnpj
		 		FROM par.instrumentounidade iu
		 		inner join par.instrumentounidadeentidade iue on iue.inuid = iu.inuid
		 		WHERE
		 		iu.inuid = {$_SESSION['par_var']['inuid']}
		 		and iue.iuestatus = 'A'
		 		and iue.iuedefault = true");*/
		//$nu_identificador = $db->pegaUm("SELECT inucnpj FROM par.instrumentounidade WHERE inuid = ".$_SESSION['par_var']['inuid']);

		// constante=1
		$tp_identificador="1";

		// constante=nulo
		$nu_conta_corrente=null;
		// constante=01
		$tp_solicitacao="01";
		// constante=0032
		$motivo_solicitacao="0032";
		// constante=nulo
		$convenio_bb=null;
		// constante=N
		$tp_conta="N";
		// condição tipoobra=5(Quadra) entao programa=CN senao programa=BW

		if($dadoscc['prptipoexecucao'] == 'C'){
			$co_programa_fnde="03";
			$nu_sistema="2";
		}else if($dadoscc['prptipoexecucao'] == 'T'){
			$co_programa_fnde="CM"; //"ED";
			$nu_sistema="7";
		}
		 



		$arqXml = <<<XML
<?xml version='1.0' encoding='iso-8859-1'?>
<request>
	<header>
		<app>string</app>
		<version>string</version>
		<created>$data_created</created>
	</header>
	<body>
		<auth>
			<usuario>$usuario</usuario>
			<senha>$senha</senha>
		</auth>
		<params>
        <nu_identificador>$nu_identificador</nu_identificador>
        <tp_identificador>$tp_identificador</tp_identificador>
        <nu_processo>$nu_processo</nu_processo>
        <nu_banco>$nu_banco</nu_banco>
        <nu_agencia>$nu_agencia</nu_agencia>
        <nu_conta_corrente>$nu_conta_corrente</nu_conta_corrente>
        <tp_solicitacao>$tp_solicitacao</tp_solicitacao>
        <motivo_solicitacao>$motivo_solicitacao</motivo_solicitacao>
        <convenio_bb>$convenio_bb</convenio_bb>
        <tp_conta>$tp_conta</tp_conta>
        <nu_sistema>$nu_sistema</nu_sistema>
        <co_programa_fnde>$co_programa_fnde</co_programa_fnde>
		</params>
	</body>
</request>
XML;

		//	if($_SESSION['baselogin'] == "simec_desenvolvimento" ||
		//	   $_SESSION['baselogin'] == "simec_espelho_producao" ){
		//		$urlWS = 'http://172.20.200.116/webservices/sigef/integracao/public/index.php/financeiro/cr';
		//	} else {
		$urlWS = 'http://www.fnde.gov.br/webservices/sigef/index.php/financeiro/cr';
		//	}

		$xml = Fnde_Webservice_Client::CreateRequest()
		->setURL($urlWS)
		->setParams( array('xml' => $arqXml, 'method' => 'solicitar') )
		->execute();

		$xmlRetorno = $xml;

		$xml = simplexml_load_string( stripslashes($xml));

		/*
		 echo "------ SOLICITAÇÃO DE CONTA CORRENTE ------\n\n";
		echo $xml->status->message->code." - ".iconv("UTF-8", "ISO-8859-1", $xml->status->message->text)."\n\n";
		*/

		$result = (integer) $xml->status->result;
		if(!$result) {
				
			$mensagem =
			'<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					ERRO AO SOLICITAR ABERTURA DE CONTA CORRENTE:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Descrição do erro: ';
			$erros = $xml->status->error->message;
			if(count($erros)>0) {
				foreach($erros as $err) {
					//echo "* ".iconv("UTF-8", "ISO-8859-1", $err->text);
					$mensagem .= ' '.iconv("UTF-8", "ISO-8859-1", $err->text).' ';
				}
			}
			$mensagem .= '
					</div>
				</div>
			 	<br>
				';
			echo $mensagem;
				
				
			//echo "*** Descrição do erro ***\n\n";
				

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarContaCorrente - Erro',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return false;
		} else {
				
			$db->executar("UPDATE par.processopar SET prpseqconta='".$xml->body->seq_solic_cr."', seq_conta_corrente='".$xml->body->nu_seq_conta."' WHERE prpid='".$_SESSION['par_var']['prpid']."'");

			$sql = "INSERT INTO par.historicowsprocessopar(
				    	prpid,
				    	hwpwebservice,
				    	hwpxmlenvio,
				    	hwpxmlretorno,
				    	hwpdataenvio,
				        usucpf)
				    VALUES ('".$_SESSION['par_var']['prpid']."',
				    		'solicitarContaCorrente - Sucesso',
				    		'".addslashes($arqXml)."',
				    		'".addslashes($xmlRetorno)."',
				    		NOW(),
				            '".$_SESSION['usucpf']."');";

			$db->executar($sql);
			$db->commit();

			return true;
		}

	} catch (Exception $e){

		# Erro 404 página not found
		if($e->getCode() == 404){
			//echo "Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.".'\n';
				
			echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					ERRO AO SOLICITAR A CONTA CORRENTE:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						Erro-Serviço Conta Corrente encontra-se temporariamente indisponível.Favor tente mais tarde.
					</div>
				</div>
			 	<br>
				';
				
		}
		$erroMSG = str_replace(array(chr(13),chr(10)), ' ',$e->getMessage());
		$erroMSG = str_replace( "'", '"', $erroMSG );

		//echo "Erro-WS Consultar Conta Corrente no SIGEF: $erroMSG";

		echo '<div style=" border: 1px solid #B7B7B7; font-size: 10px; font-style: normal; font-family: arial; padding: 5px 5px 5px 5px;">
					ERRO AO SOLICITAR A CONTA CORRENTE:
					<div style=" border-top: 1px solid #B7B7B7; padding-top: 5px; " >
						'.$erroMSG.'
					</div>
				</div>
			 	<br>
				';

	}
}
?>