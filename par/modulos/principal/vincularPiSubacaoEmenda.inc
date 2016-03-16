<?php

if( $_POST['requisicao'] == 'salvardados'){
	//ver($_POST,d);
	if( is_array($_POST['sbdid']) ){
		foreach ($_POST['sbdid'] as $sbdid) {
			$emeid = $_POST['emeid'][$sbdid];
			$emecod = $_POST['emecod'][$sbdid];
			$acaid = $_POST['acaid'][$sbdid];
			$autid = $_POST['autid'][$sbdid];
			$autnome = $_POST['autnome'][$sbdid];
			$sbaid = $_POST['sbaid'][$sbdid];
			$sbadsc = $_POST['sbadsc'][$sbdid];
			$sbdano = $_POST['sbdano'][$sbdid];
			$valorsub = $_POST['valorsub'][$sbdid];
			$ptres = $_POST['ptres'][$sbdid];
			$plicod = $_POST['plicod'][$sbdid];
			
			$detalhepi = $_POST['detalhepi'][$sbdid];

						
			$valorsub = retiraPontosBD($valorsub);
			$valorsub = $valorsub ? $valorsub : 'null';
			//if( $ptres && $plicod){
				if( !empty($detalhepi) ){
					$sql = "UPDATE par.subacaoemendapi SET 
								emeid = $emeid, 
								emecod = '$emecod',
								acaid = $acaid,
								autid = $autid,
								autnome = '$autnome',
								sbaid = $sbaid,
								sbadsc = '$sbadsc',
								sbdano = $sbdano,
								valorsub = $valorsub,
								ptres = '$ptres',
								plicod = '$plicod'
							WHERE sbdid = $detalhepi";
				} else {
					$sql = "INSERT INTO par.subacaoemendapi(sbdid, emeid, emecod, acaid, autid, autnome, sbaid, sbadsc, sbdano, valorsub, ptres, plicod) 
							VALUES ($sbdid, $emeid, '$emecod', $acaid, $autid, '$autnome', $sbaid, '$sbadsc', $sbdano, $valorsub, '$ptres', '$plicod')";
				}
				$db->executar($sql);
			//}
		}
		$db->commit();
	}
	/*if( is_array($_POST['ptres']) ){
		foreach ($_POST['ptres'] as $sbdid => $ptres) {
			$plicod = $_POST['plicod'][$sbdid];
			$sql = "update par.subacaodetalhe set sbdptres = '$ptres', sbdplicod = '$plicod' where sbdid = $sbdid";
			$db->executar($sql);
		}
		$db->commit();
	}*/
	$db->sucesso('principal/vincularPiSubacaoEmenda');
	exit();
}

if( $_POST['requisicao'] == 'carregarPI'){
	$sqlPI = "SELECT DISTINCT 
				    pi.plicod as codigo, 
				    pi.plicod ||' - '||pi.plititulo as descricao
				FROM monitora.pi_planointerno pi
					inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
					inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
	                left join emenda.execucaofinanceira exf on exf.plicod = pi.plicod and exf.exfstatus = 'A'
				WHERE 
				    pt.acaid = {$_POST['acaid']}
				    and  pt.ptres = '{$_POST['ptres']}'
				    and pi.plisituacao in ('S', 'C', 'A')
				    and pi.plistatus = 'A'
				    and pi.pliano = '".date('Y')."'
				order by pi.plicod";
		$arrPI = $db->carregar($sqlPI);
		$arrPI = $arrPI ? $arrPI : array();
		
		$html = '<select id="plicod" class="CampoEstilo" name="plicod['.$_POST['sbdid'].']">';
		$html .= '<option value="">Todas</option>';	
				foreach ($arrPI as $pi) {
					$check = '';
					if( sizeof($arrPI) == 1 ) $check = 'selected="selected"';
					$html .= '<option value="'.$pi['codigo'].'" '.$check.'>'.$pi['descricao'].'</option>';	
				}
	$html .= '</select>';
	
	echo $html;
	exit();
}

include APPRAIZ."includes/cabecalho.inc";
echo'<br>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( $titulo_modulo, '&nbsp;' );

?>
<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" name="requisicao" id="requisicao" value="">
	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="4" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="20%">Emenda:</td>
			<td width="80%"><?php 
					$emecodp = $_REQUEST["emecodp"];
					echo campo_texto( 'emecodp', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Autor do Recurso:</td>
			<td>
				<?php 
					
					$autidp = $_REQUEST["autidp"];
					
					$sql = array();
					$sql ="SELECT DISTINCT 
							ea.autid as codigo,
							ea.autnome as descricao
						FROM
							emenda.autor ea
							INNER JOIN emenda.emenda ee ON ea.autid = ee.autid and ea.autstatus = 'A'
						ORDER BY
							ea.autnome";
					
					$db->monta_combo("autidp", $sql, "S", "Todos", '', '', '', '550', 'N','autidp', '', '', '', 'class="sel_chosen"');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Tipo de Subação:</td>
			<td>
			<?php 
				
				$ptsidp = $_REQUEST["ptsidp"];
				
				$sql = "SELECT
							ptsid as codigo,
							frmdsc || ' - ' || ptsdescricao as descricao
						FROM
							par.propostatiposubacao pts
						INNER JOIN par.formaexecucao frm ON frm.frmid = pts.frmid
						WHERE
							ptsstatus = 'A'
						ORDER BY
							descricao";
				
				$db->monta_combo("ptsidp",$sql,'S',"Todas", '','','','550','N','ptsidp', '', '', '', 'class="sel_chosen"');
			?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Subação:</td>
			<td>
			<?php 
				
				$sbaidp = $_REQUEST["sbaidp"];
				
				$sql = "select distinct
							s.sbaid as codigo,
						    s.sbadsc as descricao
						from
							par.subacao s
						    inner join par.subacaodetalhe sd on sd.sbaid = s.sbaid
						    inner join par.subacaoemendapta sp on sp.sbdid = sd.sbdid and sp.sepstatus = 'A'
						where
							sd.sbdano = '".date('Y')."'
						    and s.sbastatus = 'A'";
				
				$db->monta_combo("sbaidp",$sql,'S',"Todas", '','','','550','N','sbaidp', '', '', '', 'class="sel_chosen"');
			?>
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td colspan="2" align="center">
				<input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisaEmendas();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr bgcolor="#D0D0D0">
			<td colspan="2" align="center">
				<input type="button" value="Salvar PTRES/PI" id="btnSalvar" name="btnSalvar" onclick="salvarEmendas();" style="cursor: pointer;"/>
				<input type="button" value="Exportar Excel" id="btnExecel" name="btnExecel" onclick="exportarExcelEmendas();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
<?
$where = '';
if( $_POST['requisicao'] == 'pesquisar' ){
	if( !empty($_POST['emecodp']) ) $where .= " and e.emecod = '{$_POST['emecodp']}'";
	if( !empty($_POST['autidp']) ) $where .= " and a.autid = '{$_POST['autidp']}'";
	if( !empty($_POST['ptsidp']) ) $where .= " and s.ptsid = '{$_POST['ptsidp']}'";
	if( !empty($_POST['sbaidp']) ) $where .= " and s.sbaid = '{$_POST['sbaidp']}'";
}

$img = "'<img src=\"/imagens/alterar.gif\" title=\"Subacao\" onclick=\"w = window.open(\'par.php?modulo=principal/subacao&acao=A&sbaid='||s.sbaid||'\',\'Subacao\',\'scrollbars=yes,location=no,toolbar=no,menubar=no,width=800,height=600\'); w.focus();\">&nbsp;'";
			//
$sql = "select
			a.autnome,
            a.autid,
            e.emeid,
		    e.acaid,
		    e.emecod,
		    sum(sp.sepvalor) as valorsub,
		    sd.sbdano,
		    sd.sbdptres,
		    sd.sbdplicod,
		    sd.sbdid,
            sd.sbaid,
		    s.sbadsc,
		    si.sbdid as detalhepi,
            si.ptres as ptresdetalhe,
            si.plicod as pidetalhe
		from
			emenda.emenda e
		    inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
		    inner join emenda.autor a on a.autid = e.autid
		    inner join par.subacaoemendapta sp on sp.emdid = ed.emdid and sp.sepstatus = 'A'
		    inner join par.subacaodetalhe sd on sd.sbdid = sp.sbdid
		    inner join par.subacao s on s.sbaid = sd.sbaid
		    left join par.subacaoemendapi si on si.sbdid = sd.sbdid
		where
			e.emeano = '".date('Y')."'
		    and e.emetipo = 'E'
		    and s.sbastatus = 'A'
		    and s.frmid in (14, 15)
		    $where
		group by
			a.autnome,
		    e.acaid,
		    e.emecod,
		    sd.sbdano,
		    sd.sbdptres,
		    sd.sbdplicod,
		    s.sbadsc,
            sd.sbaid,
		    sd.sbdid,
            a.autid,
            e.emeid, si.sbdid, si.ptres, si.plicod
		order by emecod";
$arrDados = $db->carregar($sql);
$arrDados = $arrDados ? $arrDados : array();
$cabecalho = array ("Autor", "Emenda", /*"Localizador",*/ "Subação", "Ano", "Vlr Subação", "PTRES", "PI");
//echo $db->monta_lista_simples($sql, $cabecalho, 20, 5, '', '', '', true, '', '', true );

echo '<table class="listagem" width="100%" cellspacing="0" cellpadding="2" border="0" align="center" style="color:333333;">
		<thead>
			<tr>';
foreach ($cabecalho as $v) {
	echo '		<td class="title" valign="top" align="center" style="border-right: 1px solid #c0c0c0; border-bottom: 1px solid #c0c0c0; border-left: 1px solid #ffffff;">'.$v.'</td>';
}
echo 		'</tr>
		</thead>
		<tbody>';

$arrExcel = array();
if( $arrDados ){
	foreach ($arrDados as $key => $v) {
		$key % 2 ? $cor = "#dedfde" : $cor = "";
		
		$sqlPtres = "SELECT DISTINCT
						pt.ptres AS codigo,
						pt.ptres AS descricao
					FROM monitora.pi_planointerno pi
						inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
						inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
					WHERE pt.acaid = {$v['acaid']}
		            	and pi.pliano = '".date('Y')."'
				    	and pi.plisituacao in ('S', 'C', 'A')";
		$arrPtres = $db->carregar($sqlPtres);
		$arrPtres = $arrPtres ? $arrPtres : array();
		
		echo '<tr bgcolor="'.$cor.'" onmouseout="this.bgColor=\''.$cor.'\';" onmouseover="this.bgColor=\'#ffffcc\';">';
		echo 	'<td valign="middle">
					<input type="hidden" name="sbdid[]" id="sbdid" value="'.$v['sbdid'].'">
					<input type="hidden" name="emeid['.$v['sbdid'].']" id="emeid" value="'.$v['emeid'].'">
					<input type="hidden" name="detalhepi['.$v['sbdid'].']" id="detalhepi" value="'.$v['detalhepi'].'">
					<input type="hidden" name="emecod['.$v['sbdid'].']" id="emecod" value="'.$v['emecod'].'">
					<input type="hidden" name="acaid['.$v['sbdid'].']" id="acaid" value="'.$v['acaid'].'">
					<input type="hidden" name="autid['.$v['sbdid'].']" id="autid" value="'.$v['autid'].'">
					<input type="hidden" name="autnome['.$v['sbdid'].']" id="autnome" value="'.$v['autnome'].'">
					<input type="hidden" name="sbaid['.$v['sbdid'].']" id="sbaid" value="'.$v['sbaid'].'">
					<input type="hidden" name="sbadsc['.$v['sbdid'].']" id="sbadsc" value="'.$v['sbadsc'].'">
					<input type="hidden" name="sbdano['.$v['sbdid'].']" id="sbdano" value="'.$v['sbdano'].'">
					<input type="hidden" name="valorsub['.$v['sbdid'].']" id="valorsub" value="'.$v['valorsub'].'">
					'.$v['autnome'].'</td>';
		echo 	'<td valign="middle" align="right" style="color: rgb(0, 102, 204);">'.$v['emecod'].'</td>';
		//echo 	'<td valign="middle" style="color: rgb(0, 102, 204);">'.$v['codigo'].'</td>';
		echo 	'<td valign="middle">'.$v['sbadsc'].'</td>';
		echo 	'<td valign="middle" align="right" style="color: rgb(0, 102, 204);">'.$v['sbdano'].'</td>';
		echo 	'<td valign="middle" align="right" style="color: rgb(0, 102, 204);">'.number_format($v['valorsub'], 2, ',', '.').'</td>';
		//echo 	'<td valign="middle"><select id="ptres" class="CampoEstilo" name="ptres['.$v['sbdid'].']" onchange="carregarPI(this.value, '.$v['acaid'].', '.$v['sbdid'].')">';
		echo 	'<td valign="middle"><select id="ptres" class="CampoEstilo" name="ptres['.$v['sbdid'].']">';
						echo '<option value="">Todas</option>';
					foreach ($arrPtres as $ptres) {
						$check = '';					
						if( sizeof($arrPtres) == 1 || $ptres['codigo'] == $v['ptresdetalhe'] ) $check = 'selected="selected"';
						echo '<option value="'.$ptres['codigo'].'" '.$check.'>'.$ptres['descricao'].'</option>';	
					}
		echo 	'					</select></td>';
		echo 	'<td valign="middle"><div id="td_pi_'.$v['sbdid'].'">';
		
		//if( sizeof($arrPtres) == 1 && $arrPtres[0] || !empty($v['pidetalhe']) ){
			/*$sqlPI = "SELECT DISTINCT 
					    pi.plicod as codigo, 
					    pi.plicod ||' - '||pi.plititulo as descricao
					FROM monitora.pi_planointerno pi
						inner join monitora.pi_planointernoptres plpt on pi.pliid = plpt.pliid
						inner join monitora.ptres pt on pt.ptrid = plpt.ptrid
		                left join emenda.execucaofinanceira exf on exf.plicod = pi.plicod and exf.exfstatus = 'A'
					WHERE 
					    pt.acaid = {$v['acaid']}
					    and  pt.ptres = '{$arrPtres[0]['codigo']}'
					    and pi.plisituacao in ('S', 'C', 'A')
					    and pi.plistatus = 'A'
					    and pi.pliano = '".date('Y')."'
					order by pi.plicod";
			$arrPI = $db->carregar($sqlPI);
			$arrPI = $arrPI ? $arrPI : array();*/
			
			$arrPI = array(
						array('codigo' => 'RFF34B155DN', 'descricao' => 'RFF34B155DN - Ampliação'),
						array('codigo' => 'RFF34B154DN', 'descricao' => 'RFF34B154DN - Construção'),
						array('codigo' => 'RFF34B156DN', 'descricao' => 'RFF34B156DN - Reforma'),
						array('codigo' => 'GFF08B151DA', 'descricao' => 'GFF08B151DA - Bicicleta'),
						array('codigo' => 'GFF08B152DA', 'descricao' => 'GFF08B152DA - Ônibus Escolar'),
						array('codigo' => 'RFF61B154DN', 'descricao' => 'RFF61B154DN - Tablet'),
						array('codigo' => 'RFF61B156DN', 'descricao' => 'RFF61B156DN - Projetor'),
						array('codigo' => 'RFF61B155DN', 'descricao' => 'RFF61B155DN - Notebook'),
						array('codigo' => 'RFF34B157DN', 'descricao' => 'RFF34B157DN - Instrumentos musicais'),
						array('codigo' => 'RFF57B154DN', 'descricao' => 'RFF57B154DN - Uniforme escolar'),
						array('codigo' => 'RFF34B152DN', 'descricao' => 'RFF34B152DN - Mobiliário'),
						array('codigo' => 'RFF61B155DN', 'descricao' => 'RFF61B155DN - Computadores'),
						array('codigo' => 'RFF34B151DN', 'descricao' => 'RFF34B151DN - Equipamentos diversos'),
						array('codigo' => 'RFF34B158DN', 'descricao' => 'RFF34B158DN - Kit conzinha'),
						array('codigo' => 'FSW01G1501N', 'descricao' => 'FSW01G1501N - TUDO DA AÇÃO  0048'),
						array('codigo' => 'FFS01G9100N', 'descricao' => 'FFS01G9100N - COMPLEMENTO AÇÃO  0048')
						);
					
			echo '<select id="plicod" class="CampoEstilo" name="plicod['.$v['sbdid'].']">';
			echo	'<option value="">Todas</option>';	
			foreach ($arrPI as $pi) {
				$check = '';
				$arDescPi = explode(' - ', $pi['descricao']);
				if( sizeof($arrPI) == 1 || $pi['codigo'] == $v['pidetalhe'] || (strpos( str_to_upper($v['sbadsc']), str_to_upper($arDescPi[1]) ) !== false ) ) $check = 'selected="selected"';
				echo '<option value="'.$pi['codigo'].'" '.$check.'>'.$pi['descricao'].'</option>';	
			}
			echo '</select>';
		/*} else {
			echo '<select id="plicod" class="CampoEstilo" name="plicod['.$v['sbdid'].']">
					<option value="">Todas</option>
				</select>';
		}*/
		echo '</div></td></tr>';
		
		array_push($arrExcel, array('autor' => $v['autnome'],
									'emenda' => $v['emecod'],
									'subacao' => $v['sbadsc'],
									'ano' => $v['sbdano'],
									'valor' => number_format($v['valorsub'], 2, ',', '.'),
									'ptres' => $v['ptresdetalhe'],
									'pi' => $v['pidetalhe']
									)
					);
	}
echo '</tbody>
	</table>
	<table class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center">
	<tbody>
		<tr bgcolor="#ffffff">
			<td><b>Total de Registros: '.sizeof($arrDados).'</b></td>
		</tr>
	</tbody>
	</table>';
} else {
	echo '<table class="listagem" width="95%" cellspacing="0" cellpadding="2" border="0" align="center">
	<tbody>
		<tr><td align="center" style="color:#cc0000;" colspan="3">Não foram encontrados registros.</td></tr>
	</tbody>
	</table>';
}

if($_POST['requisicao'] == 'exportar'){
ob_clean();
header('content-type: text/html; charset=ISO-8859-1');
$db->sql_to_excel($arrExcel, 'relSubacaoEmenda', $cabecalho);
}
?>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr bgcolor="#D0D0D0">
			<td colspan="2" align="center">
				<input type="button" value="Salvar PTRES/PI" id="btnSalvar" name="btnSalvar" onclick="salvarEmendas();" style="cursor: pointer;"/>
				<input type="button" value="Exportar Excel" id="btnExecel" name="btnExecel" onclick="exportarExcelEmendas();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<script type="text/javascript">
jq('.sel_chosen').chosen({allow_single_deselect:true});

function carregarPI(ptres, acaid, sbdid){
	
	$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/vincularPiSubacaoEmenda&acao=A",
			data: "requisicao=carregarPI&ptres="+ptres+"&acaid="+acaid+'&sbdid='+sbdid,
			async: false,
			success: function(msg){
				$('#td_pi_'+sbdid).html(msg);
			}
	});
}

function pesquisaEmendas(){
	$('[name="requisicao"]').val('pesquisar');
	$('[name="formulario"]').submit();
}

function salvarEmendas(){
	$('[name="requisicao"]').val('salvardados');
	$('[name="formulario"]').submit();
}

function exportarExcelEmendas(){
	$('[name="requisicao"]').val('exportar');
	$('[name="formulario"]').submit();
}
</script>