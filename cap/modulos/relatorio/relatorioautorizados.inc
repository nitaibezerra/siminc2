<?php
if($_REQUEST['requisicao'] == 'atualizar_area'){
	header('Content-type: text/html; charset=ISO-8859-1');
	extract($_POST);
	
	$afpdtrealizacaoinicial = formata_data_sql($afpdtrealizacaoinicial);
	$afpdtrealizacaofinal = formata_data_sql($afpdtrealizacaofinal);
	
	if(!empty($afpdtrealizacaoinicial) && !empty($afpdtrealizacaofinal)){
		$aryWhere[] = "( afa.afpdtrealizacaoinicial BETWEEN '{$afpdtrealizacaoinicial}'  AND '{$afpdtrealizacaofinal}' AND  afa.afpdtrealizacaofinal BETWEEN '{$afpdtrealizacaoinicial}'  AND '{$afpdtrealizacaofinal}')";
	}
	
	/*if(!empty($afpdtrealizacaoinicial)){
		$aryWhere[] = "'{$afpdtrealizacaoinicial}' BETWEEN afa.afpdtrealizacaoinicial AND afa.afpdtrealizacaofinal";
	}
	
	if(!empty($afpdtrealizacaofinal)){
		$aryWhere[] = "'{$afpdtrealizacaofinal}' BETWEEN afa.afpdtrealizacaoinicial AND afa.afpdtrealizacaofinal";
	}*/
	
	$sql = "SELECT 			DISTINCT afa.uamid AS codigo,
							arm.uamsigla || ' - ' || arm.uamdsc AS descricao, 
							arm.uamsigla, arm.uamdsc	
			FROM 			cap.afastamento afa
			INNER JOIN		public.unidadeareamec arm ON arm.uamid = afa.uamid
							".(is_array($aryWhere) ? ' WHERE '.implode(' AND ', $aryWhere) : '')."
			ORDER BY 		arm.uamsigla, arm.uamdsc";

	combo_popup('uamid', $sql, 'Selecione a(s) Área(s) MEC', '400x400', 0, array(), '', 'S', true , true , 4, 400, null, null, $dados_conexao, null, null, true, false, null, true);
	exit();
}

if($_POST['req']){
	ini_set("memory_limit","2048M");
	$true = true;
	include("relatorioautorizadosresultado.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Afastamento do País p/ Área MEC <br> (Autorizados)', '&nbsp;' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/dateFunctions.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>	
<script type="text/javascript">
function gerarRelatorio(req){
	var formulario = document.formulario;

	selectAllOptions(formulario.uamid);
	document.getElementById('req').value = req;
	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';	
	formulario.submit();
	janela.focus();
}

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' ) {
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	} else {
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function atualizarArea(){
	if($('#afpdtrealizacaoinicial').val() == '' || $('#afpdtrealizacaofinal').val() == ''){
		alert('O campo "Período" é obrigatório!');
	} else { 
		$.ajax({
		url: 'cap.php?modulo=relatorio/relatorioautorizados&acao=A',
		data: { requisicao: 'atualizar_area', afpdtrealizacaoinicial: $('#afpdtrealizacaoinicial').val(), afpdtrealizacaofinal: $('#afpdtrealizacaofinal').val()},
		async: false,
		type: 'POST',
		success: function(data) {
				$("#div_area").html(data);
	    	}
		});
	}
}	
</script>

<form name="formulario" id="formulario" action="" method="post">
	<input type="hidden" name="req" id="req" value="" />	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" valign="top">Período:</td>
			<td>
				<?php echo campo_data2('afpdtrealizacaoinicial', 'S', 'S', 'Período Inicial', 'S', '', 'atualizarArea();', $afpdtrealizacaoinicial); ;?>&nbsp;&nbsp;&nbsp;&nbsp;a
				&nbsp;&nbsp;&nbsp;&nbsp;<?php echo campo_data2('afpdtrealizacaofinal', 'S', 'S', 'Período Final', 'S', '', 'atualizarArea();', $afpdtrealizacaofinal);  ?>		
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Lotação:</td>
			<td id="div_area">
				<?php
				$sql_combo = "SELECT 		uamid AS codigo, 
											uamsigla || ' - ' || uamdsc AS descricao 
							  FROM 			public.unidadeareamec 
							  ORDER BY 		uamsigla, uamdsc";
				combo_popup('uamid', $sql_combo, 'Selecione a(s) Área(s) MEC', '400x400', 0, array(), '', 'S', true, true, 4, 400, null, null, $dados_conexao, null, null, true, false, null, true); ?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">País:</td>
			<td><?php 
				$sql = "SELECT  	paiid as codigo,
									paidescricao as descricao
						FROM		territorios.pais";
				
				$db->monta_combo('paiid', $sql, 'S', 'Selecione', '', '', '', ''); ?>			
			</td>
		</tr>
		<tr>
			<td align="center" bgcolor="#CCCCCC" colspan="2">
				<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio(1);"/>
				<input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio(2);"/>
			</td>
		</tr>
	</table>
</form>