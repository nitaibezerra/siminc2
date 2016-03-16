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
//função que comprara as datas de entrada no sistema. 
function comparaDataRMBAnalitico(){
	var dataInicio = $("#dtinicio").val();
	var dataInicioConvertida = dataInicio.substring(6,10) + dataInicio.substring(3,5) + dataInicio.substring(0,2);

	var dataFim = $("#dtfim").val();
	var dataFimConvertida = dataFim.substring(6,10) + dataFim.substring(3,5) + dataFim.substring(0,2);

	if (dataInicioConvertida > dataFimConvertida){
    	alert('Período informado inválido: A data de fim não pode ser maior que a data de início');
    	return false;
	}
	else{
		return true;
	}
}

</script>

<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

//Monta o Título da página abaixo da aba
monta_titulo("RMB Analítico", obrigatorio()." Indica campos obrigatórios.");
?>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Opção Analítica: </td>
			<td class="campo">
				Data de início <?=campo_data2('dtinicio','S','S','Informe a Data de Início','S','','','','','','dtinicio');?>
				Data de fim    <?=campo_data2('dtfim','S','S','Informe a Data de Fim','S','','','','','','dtfim');?>
		</tr>	
		<tr>
			<td class="SubtituloDireita"> Tipo: </td>
			<td class="campo">
				<input type="radio" name="tipo" value="sap.php?modulo=relatorio/relatorioAnaliticoEntrada&acao=A" id="tipo" /> Entrada
				<input type="radio" name="tipo" value="sap.php?modulo=relatorio/relatorioAnaliticoSaida&acao=A" id="tipo" /> Saída
				<img border="0" src="../imagens/obrig.gif" title="Indica campo obrigatório.">
				<input type=hidden id="tipo_relatorio" name="tipo_relatorio"  />
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

<script type="text/javascript">


$(document).ready(function(){

	$("input:radio").click(function(){
		$("#tipo_relatorio").attr('value', $(this).val() );
	});	

	$("#formularioPesquisa").validate({
		//Define as regras dos campos
		rules:{
				dtinicio:{
					required: true
				},
				dtfim:{
					required: true
				},
				tipo_relatorio:{
					required: true
				}
		},
		//Define as mensagens de alerta
		messages:{
			dtinicio 	 	:"Campo Obrigatório",
			dtfim	 	 	:"Campo Obrigatório",
			tipo_relatorio	:"Campo Obrigatório"
		}
	});

	$("#btnPesquisar").click( function(){
		
		if( $("#formularioPesquisa").valid() ){
			
			if( !comparaDataRMBAnalitico() ){
				return false;
			}else{
				ajaxValidacao();
			}
		}
	});
	
	$("#btnPesquisarXls").click( function(){
		
		if( $("#formularioPesquisa").valid() ){

			$("#formularioPesquisa").attr('action', $("#tipo_relatorio").val() +'&excel=ok' );
			$("#formularioPesquisa").attr('target', 'relatorioAnalitico' );
			
        	window.open('', 'relatorioAnaliticoExcel', 'scrollbars=yes,width=900,height=770');

			$("#formularioPesquisa").submit();
		}
	});
	
	function ajaxValidacao()
	{
		$.ajax({
	        type: 'post',
	        url: $("#tipo_relatorio").val() ,
	        dataType: 'json',
	        data: { validacao:'ok', dtinicio: $('#dtinicio').val(), dtfim: $('#dtfim').val() },
	        success: function(response){
	            if( response.status == true )
	            {
	    			$("#formularioPesquisa").attr('action', $("#tipo_relatorio").val() );
	    			$("#formularioPesquisa").attr('target', 'relatorioAnalitico' );
	    			
	            	window.open('', 'relatorioAnalitico', 'scrollbars=yes,width=900,height=770');

	    			$("#formularioPesquisa").submit();
	            }else{
	                alert( response.msg );
	            }
	        }
	    });
	}
});

</script>