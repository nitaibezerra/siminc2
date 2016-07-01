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
	$("#formularioPesquisa").validate({
		//Define as regras dos campos
		rules:{
			var_ano:{required: true},
			var_mes:{required: true}
		},
		//Define as mensagens de alerta
		messages:{
			var_ano :"Campo Obrigatório",
			var_mes	:"Campo Obrigatório"
		}
	});

	$("#btnPesquisarXls").click( function(){

		if( $("#formularioPesquisa").valid() ){

			$("#formularioPesquisa").attr('action', 'sap.php?modulo=relatorio/relatorioMensal&acao=A&excel=ok' );
			$("#formularioPesquisa").attr('target', 'relatorioMensal' );
			
	    	window.open('', 'relatorioMensalExcel', 'scrollbars=yes,width=900,height=770');

			$("#formularioPesquisa").submit();
		}
	});
});

AbrirPopUp = function(url,nome,param){
    var a = window.open(url,nome,param);
    a.focus();
}

function visualizarRelatorio( url ){

	var var_ano	= $('#var_ano').val();
	var var_mes	= $('#var_mes').val();
	
	if( $("#formularioPesquisa").valid() ){
		$.ajax({
	        type: 'get',
	        url:'?modulo=relatorio/relatorioMensal&acao=A',
	        dataType: 'json',
	        data: { validacao:'ok', var_ano: var_ano, var_mes:var_mes },
	        success: function(response){
	            if( response.status == true )
	            {
	            	AbrirPopUp('?modulo=relatorio/relatorioMensal&acao=A&var_ano=' + var_ano + '&var_mes=' + var_mes,'Relatório Mensal RMB','scrollbars=yes,width=900,height=770');
	            }else{
	                alert( response.msg );
	            }
	        }
	    });
	}
}

</script>

<?php 
include APPRAIZ . 'includes/cabecalho.inc';
print "<br/>";

//Monta o Título da página abaixo da aba
monta_titulo("RMB Mensal", obrigatorio()." Indica campos obrigatórios.");

$sql="SELECT mescod as codigo, mesdsc as descricao FROM public.meses";
?>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Ano: </td>
            <td class="campo">
            	<?=campo_texto('var_ano','S','S','Ano',5,4,'','','left','',0,'id="var_ano"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita" > Mês: </td>
			<td name="mesid" style="width: auto;" class="campo">
				<?php echo $db->monta_combo('var_mes', $sql, 'S', 'Selecione', '', '', '', '', 'S', 'var_mes', '', ''); ?>
			</td>
		</tr>

		<tr class="buttons">
			<td colspan='2' class="SubTituloCentro" align="center">
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Gerar Relatório' onclick="visualizarRelatorio()" />
				<input type='button' class="botao" name='btnPesquisarXls' id='btnPesquisarXls' value='Gerar Excel' />
				<input type='reset' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar Campos' />
		    </td>
		</tr>
	</table>
</form>