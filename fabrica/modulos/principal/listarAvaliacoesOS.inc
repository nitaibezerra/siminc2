<?
if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";

echo '<br>';

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="./js/fabrica.js"></script>
<script type="text/javascript" src="/includes/JQuery/jquery-1.4.2.min.js"></script>
<script type="text/javascript">

function limparCampos(){
	$('input:not(:submit,:hidden,:button,[readonly=""],[readonly="readonly"]),textarea,select').val("");
}

</script>
<?

$titulo_modulo = "Listar Avaliações OS";
monta_titulo($titulo_modulo, '');

?>

<form name="formulario" id="form_listar_contratos" method="post" action="">
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubtituloDireita" width="25%">Número</td>
			<td><input type="text" id="odsid" name="odsid" value="<?php echo $_REQUEST['odsid']; ?>"></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Previsão de Início</td>
			<td><?php
					echo campo_data2('odsdtprevinicio','N', 'S', 'Expectativa de atendimento', 'S', '', '', formata_data_sql($_REQUEST['odsdtprevinicio']) );
		  		?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Previsão de Término</td>
			<td><?php
					echo campo_data2('odsdtprevtermino','N', 'S', 'Expectativa de término', 'S', '', '', formata_data_sql($_REQUEST['odsdtprevtermino']) ); 
				?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="25%">Detalhamento</td>
			<td><input type="text" id="odsdetalhamento" name="odsdetalhamento" value="<?php echo $_REQUEST['odsdetalhamento']; ?>"></td>
		</tr> 
		<tr>
			<td class="SubtituloDireita" width="25%">Situação</td>
			<td><?php $sql = "SELECT 
									esdid as codigo,
									esddsc as descricao
								FROM workflow.estadodocumento 
								WHERE tpdid = ".WORKFLOW_ORDEM_SERVICO."
								AND esdstatus = 'A'
								ORDER BY descricao";
		  		
		  			   $db->monta_combo('esdid', $sql, 'S', 'Selecione', '', '', '', '', 'N', 'esdid', '', $_REQUEST['esdid']); 
		  		?>
			</td>
		</tr> 
		<tr>
			<td colspan="2">
				<input type="submit" value="Pesquisar">
				&nbsp;
				<input type="button" value="Limpar" id="limpar" onclick="limparCampos();">
			</td>
		</tr>
	</table>
	<input type="hidden" name="pesquisar" value="pesquisar">
</form>

<?php

if( isset($_REQUEST['pesquisar']) ){
	
	if($_REQUEST['odsid']){
		$filtro[] = "os.odsid = '{$_REQUEST['odsid']}'";
		
	}
	if($_REQUEST['odsdtprevinicio']){
		$filtro[] = "os.odsdtprevinicio = '".formata_data_sql($_REQUEST['odsdtprevinicio'])."'";
		
	}
	if($_REQUEST['odsdtprevtermino']){
		$filtro[] = "os.odsdtprevtermino = '".formata_data_sql($_REQUEST['odsdtprevtermino'])."'";
		
	}
	if($_REQUEST['odsdetalhamento']){
		$filtro[] = "os.odsdetalhamento ILIKE '%{$_REQUEST['odsdetalhamento']}%'";
		
	}
	if($_REQUEST['esdid']){
		$filtro[] = "ed.esdid = '{$_REQUEST['esdid']}'";
		
	}
	
}

$sql = "SELECT '<img src=../imagens/alterar.gif style=cursor:pointer; onclick=\"window.open(\'fabrica.php?modulo=principal/cadOSExecucao&acao=A&odsid='||os.odsid||'\',\'Observações\',\'scrollbars=yes,height=600,width=800,status=no,toolbar=no,menubar=no,location=no\');\">' as acao,
			   os.odsdetalhamento, 
			   aos.avoobs, 
			   CASE WHEN avoprobres = TRUE THEN 'Sim' ELSE 'Não' END as avsprobres,
			   CASE 
			   		WHEN avotempo = 'M' THEN 'Muito bom' 
			   		WHEN avotempo = 'B' THEN 'Bom' 
			   		WHEN avotempo = 'A' THEN 'Aceitável'
			   		WHEN avotempo = 'T' THEN 'Teria que ser mais rápido'
			   END as avotempo,
			   CASE 
			   		WHEN avogeral = '1' THEN 'Ruim'
			   		WHEN avogeral = '2' THEN 'Regular'
			   		WHEN avogeral = '3' THEN 'Bom'
			   		WHEN avogeral = '4' THEN 'Ótimo' 
			   	END as avogeral	
		FROM fabrica.avaliacaoos aos 
		LEFT JOIN fabrica.ordemservico os ON os.odsid=aos.odsid 
		INNER JOIN workflow.documento doc ON doc.docid = os.docid
		INNER JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid 
		".(($filtro)?"WHERE ".implode(" AND ", $filtro):"");

$cabecalho     = array("", "Detalhamento", "Observações", "Solicitação atendida?", "Recebimento da OS?", "Atendimento?");
$tamanho       = array("3%","40%","40%","3%","9%","5%");
$alinhamento   = array("center","left","left","center","center","center");
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2,"",$tamanho,$alinhamento);

?>