<?
// cabecalho padrão do SIMEC
include APPRAIZ . "includes/cabecalho.inc";
monta_titulo('Comunicação de Dados - SIOP', 'Log Erro WS');

?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<form method="post" name="formulario" id="formulario">	
	
<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td class="subtitulodireita"><b>Ação (acacod):</b></td>
		<td><input type="text" name="acacod" id="acacod" value="<?=$_POST['acacod']; ?>"></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Unidade (unicod):</b></td>
		<td><input type="text" name="unicod" id="unicod" value="<?=$_POST['unicod']; ?>"></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Tipo de LOG:</b></td>
		<td><?
			$sql = "SELECT DISTINCT 
						logtipo as codigo,
						logtipo as descricao
					FROM 
						elabrev.logerrows lo
					WHERE
						(logtipo <> '')
					ORDER BY logtipo";
			
			$logtipo = $_POST['logtipo'];
			$db->monta_combo("logtipo", $sql, 'S','-- Selecione um Tipo --','', '', '',350,'S','logtipo', '', '', 'Tipo de LOG');
			
			$data = formata_data_sql($_POST['data']);
			?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Lote:</b></td>
		<td><?
			$sql = "SELECT DISTINCT 
						loglote as codigo,
						loglote as descricao
					FROM 
						elabrev.logerrows lo
					WHERE
						(loglote is not null)
					ORDER BY loglote";
			
			$loglote = $_POST['loglote'];
			$db->monta_combo("loglote", $sql, 'S','-- Selecione um Tipo --','', '', '',100,'S','loglote', '', '', 'Lote');
			?></td>
	</tr>
	<tr>
		<td class="subtitulodireita"><b>Data Log:</b></td>
		<td><?=campo_data2( 'data', 'N', 'S', '', 'S' ); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Visualização:</td>
		<td>
			<input <?php echo $_REQUEST['view'] == "1" ? "checked='checked'" : "" ?> type="radio" name="view" value="1" />Pendência
			<input <?php echo $_REQUEST['view'] == "2" ? "checked='checked'" : "" ?> type="radio" name="view" value="2" />Sem Pendência
			<input <?php echo empty($_REQUEST['view']) ? "checked='checked'" : "" ?> type="radio" name="view" value=""  />Todos
		</td>
	</tr>
	<tr>
		<td colspan="2" align="center" style="background-color:#c0c0c0;">
			<input type="submit" class="botao" name="btnenvia" value="Pesquisar"></td>
	</tr>
</table>
</form>
<?php
if( $_POST['btnenvia'] ){
	$filtro = '';
	if( $_POST['acacod'] ) $filtro .= " and acacod = '{$_POST['acacod']}' ";
	if( $_POST['logtipo'] ) $filtro .= " and logtipo = '{$_POST['logtipo']}' ";
	if( $_POST['unicod'] ) $filtro .= " and lo.unicod = '{$_POST['unicod']}' ";
	if( $_POST['loglote'] ) $filtro .= " and lo.loglote = {$_POST['loglote']} ";
	if( $_POST['view'] == '1' ) $filtro .= " and lo.xmlresponse ilike '%PEND%' ";
	if( $_POST['view'] == '2' ) $filtro .= " and lo.xmlresponse not ilike '%PEND%' ";
	if( $_POST['data'] ) $filtro .= " and to_char(lo.logdatainclusao, 'YYYY-MM-DD') = '".formata_data_sql($_POST['data'])."' ";
	//ver($_POST,d);
	$sql = "SELECT logtipo, prgcod, acacod, lo.unicod, loccod, prgano, lo.usucpf, us.usunome, to_char(logdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as logdatainclusao,
			  	logerro, xmlrequest, xmlresponse, acaid
			FROM 
			  	elabrev.logerrows lo
	            inner join seguranca.usuario us on us.usucpf = lo.usucpf
			where 
				1=1
				$filtro
			order by logdatainclusao desc";
	//ver($_POST, $sql,d);
	$arrDados = $db->carregar($sql);
	$arrDados = $arrDados ? $arrDados : array();
	//ppo.prgcod||'.'||ppo.acacod||'.'||ppo.unicod||'.'||ppo.loccod
	
	foreach ($arrDados as $v) {
?>
<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
	<tr>
		<td width="23%" class="subtitulodireita"><b>logtipo:</b></td>
		<td><?=$v['logtipo']; ?></td>
	</tr>
	<tr>
		<td width="23%" class="subtitulodireita"><b>Ação:</b></td>
		<td><?=$v['prgcod'].'.'.$v['acacod'].'.'.$v['unicod'].'.'.$v['loccod']; ?></td>
	</tr>
	<tr>
		<td width="23%" class="subtitulodireita"><b>prgano:</b></td>
		<td><?=$v['prgano']; ?></td>
	</tr>
	<tr>
		<td width="23%" class="subtitulodireita"><b>usucpf:</b></td>
		<td><?=formatar_cpf($v['usucpf']).' - '.$v['usunome']; ?></td>
	</tr>
	<tr>
		<td width="23%" class="subtitulodireita"><b>logdatainclusao:</b></td>
		<td><?=$v['logdatainclusao']; ?></td>
	</tr>
	<tr>
		<td width="23%" class="subtitulodireita"><b>Total de Log:</b></td>
		<td><?=sizeof($arrDados); ?></td>
	</tr>
	<tr>
		<td colspan="2">
		<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">
			<tr>
				<th width="50%">request</th>
				<th width="50%">response</th>
			</tr>
			<tr>
				<td><?=formatarXML($v['xmlrequest']); ?></td>
				<td valign="top"><?=formatarXML($v['xmlresponse']); ?></td>
			</tr>
		</table>
		</td>
	</tr>
</table>
<br>
<?}
}
function formatarXML( $xml ){
	$xml = str_ireplace( '><', '>¬<', $xml );
	//$xml = str_ireplace( '> <', '>¬<', $xml );
	$arrXML = explode( '¬', $xml );
	$html = '<table align="center" width="95%" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="1" align="center">';
	foreach ($arrXML as $xml) {
		$html.= '<tr><td>'.simec_htmlentities(utf8_decode($xml)).'</td></tr>';
	}
	$html.= '</table>';
	return $html;
}

?>