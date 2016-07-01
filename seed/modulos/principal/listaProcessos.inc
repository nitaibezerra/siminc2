<?php

unset( $_SESSION['seed']['prcid'] );
$obProcesso = new Processos();

if($_POST['requisicao'] == 'excluir'){
	$obProcesso->excluirProcesso( $_POST['prcid'] );	
}

if( $_POST['requisicao'] == 'pesquisar' ){
	$filtro = $obProcesso->montaFiltroProcesso( $_POST );
	extract( $_POST );
}

include  APPRAIZ."includes/cabecalho.inc";
echo '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo('SEED', 'Localizar Processos');
?>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="js/processo.js"></script>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form id="formulario" name="formulario" action="" method="post" enctype="multipart/form-data" >
<input type="hidden" name="requisicao" id="requisicao" value="">
<input type="hidden" name="prcid" id="prcid" value="">
<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Tipo Processo:</td>
		<td><?
			$sql = "SELECT 
					  tpcid as codigo,
					  tpcdsc as descricao
					FROM 
					  seed.tipoprocesso
					WHERE
						tpcstatus = 'A'";
			$db->monta_combo("tpcid", $sql, 'S', 'Selecione...', '', '', '', '265','','tpcid', '', '', 'Tipo Processo');
			?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Número Processo:</td>
		<td><?=campo_texto('prcnumprocesso', 'N', 'S', 'Número Processo', 30, 100, '#####.######/####-##', '','','','','id="prcnumprocesso"'); ?></td>
	</tr>
	<tr id="dataentrada" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Data Entrada:</td>
		<td><?=campo_data2('prcdataentradaccon', 'N','S','Data Entrada','','',''); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" style="width:39.5%;">Departamento Responsável:</td>
		<td><?
			$sql = "SELECT 
					  	dreid as codigo,
					  	dredsc as descricao
					FROM 
					  	seed.departamentoresponsavel
					WHERE
						drestatus = 'A'";
			$db->monta_combo("dreid", $sql, 'S', 'Selecione...', '', '', '', '265','','dreid', '', '', 'Departamento Responsável');?></td>
	</tr>
	<tr id="inivigencia" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Início Vigência:</td>
		<td><?=campo_data2('prcdatainiciovigencia', 'N','S','Início Vigência','','',''); ?></td>
	</tr>
	<tr id="fimvigencia" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Término Vigência:</td>
		<td><?=campo_data2('prcdatafimvigencia', 'N','S','Término Vigência','','',''); ?></td>
	</tr>
	<tr id="interessado" style="display: ''" >
		<td class="SubTituloDireita" style="width:39.5%;">Interessado:</td>
		<td><?
			$sql = "SELECT 
					  	intid as codigo,
					  	intdsc as descricao
					FROM 
					  seed.interessado
					WHERE
						intstatus = 'A'";
			$db->monta_combo("intid", $sql, 'S', 'Selecione...', '', '', '', '265','','intid', '', '', 'Interessado');
			?></td>
	</tr>
	<tr id="valor" style="display: ''">
		<td class="SubTituloDireita" style="width:39.5%;">Valor (R$):</td>
		<td><?=campo_texto('prcvalor', 'N', 'S', 'Valor', 20, 15, '[###.]###,##', '','','','','id="prcvalor"'); ?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Pesquisar" onclick="pesquisar();" style="cursor: pointer;"/>
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: left;">
			<a href="#" onclick="window.location.href = 'seed.php?modulo=principal/cadastrarProcessos&acao=A';" style="cursor:pointer">
				<label id="labelCPF"><img src="../imagens/gif_inclui.gif" border="0">Cadastrar Processo</label>
			</a>
		</td>
	</tr>
</table>
</form>
<?
monta_titulo('SEED', 'Processos da SEED');
$obProcesso->listaProcessos( $filtro );
?>