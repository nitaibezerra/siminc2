<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet" />
<link rel='stylesheet' type='text/css' href='../fabrica/css/jquery-ui-1.8.16.custom.css'/>
<link rel="stylesheet" type="text/css" href="../includes/jquery-validate/css/validate.css" />

<script type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script type='text/javascript' src='../includes/calendario.js'></script>
<script type="text/javascript" src="../fabrica/js/jquery-1.7.min.js"></script>
<script type="text/javascript" src="../fabrica/js/jquery-ui-1.8.16.custom.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/jquery.validate.min.js"></script>
<script type="text/javascript" src="../includes/jquery-validate/localization/messages_ptbr.js"></script>
<script type="text/javascript">
$(document).ready(function(){

	$("input:radio").click(function(){
		$("#tipo_relatorio").attr('value', $(this).val() );
	});	
	
	$("#formularioPesquisa").validate({
		rules:{
			dtentrada:{required: true},
			tipo_relatorio:{required: true}
		},
		messages:{
			dtentrada 		:"Campo Obrigatório",
			tipo_relatorio	:"Campo Obrigatório"
		}
	});

	$("#btnPesquisar").click( function(){
		
		if( $("#formularioPesquisa").valid() ){
			ajaxValidacao();
		}
	});

	$("#btnPesquisarXls").click( function(){
		
		if( $("#formularioPesquisa").valid() ){

			$("#formularioPesquisa").attr('action', $("#tipo_relatorio").val() +'&excel=ok' );
			$("#formularioPesquisa").attr('target', 'relatorioDepreciacao' );
			
        	window.open('', 'relatorioDepreciacaoExcel', 'scrollbars=yes,width=900,height=770');

			$("#formularioPesquisa").submit();
		}
	});
	
	function ajaxValidacao()
	{
		$.ajax({
	        type: 'post',
	        url: $("#tipo_relatorio").val() ,
	        dataType: 'json',
	        data: { validacao:'ok', dtentrada: $('#dtentrada').val() },
	        success: function(response){
	            if( response.status == true )
	            {
	    			$("#formularioPesquisa").attr('action', $("#tipo_relatorio").val() );
	    			$("#formularioPesquisa").attr('target', 'relatorioDepreciacao' );
	    			
	            	window.open('', 'relatorioDepreciacao', 'scrollbars=yes,width=900,height=770');

	    			$("#formularioPesquisa").submit();
	            }else{
	                alert( response.msg );
	            }
	        }
	    });
	}
	
});
</script>

<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

//Monta o Título da página abaixo da aba
monta_titulo("Relatório de Depreciação de Bens", obrigatorio()." Indica campos obrigatórios.");
?>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Bens com entrada até: </td>
			<td class="campo">
				<?=campo_data2('dtentrada','S','S','Informe a Data da Entrada','S','','','','','','dtentrada');?>
            </td>
		</tr>	
		<tr>
			<td class="SubtituloDireita"> Tipo de Relatório: </td>
			<td class="campo">
				<input type="radio" name="tipo" value="sap.php?modulo=relatorio/relatorioDepreciacaoBensAnalitico&acao=A" id="tipo" /> Analítico
				<input type="radio" name="tipo" value="sap.php?modulo=relatorio/relatorioDepreciacaoBensSintetico&acao=A" id="tipo" /> Sintético
				<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				<input type="hidden" id="tipo_relatorio" name="tipo_relatorio"  />
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2' class="SubTituloCentro" align="center">
				<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Gerar Relatório' />
				<input type='button' class="botao" name='btnPesquisarXls' id='btnPesquisarXls' value='Gerar Excel' />
				<input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar Campos' />
		    </td>
		</tr>
	</table>
</form>