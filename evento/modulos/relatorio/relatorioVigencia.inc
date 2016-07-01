<?php

if ($_POST){
	ini_set("memory_limit","256M");
	include("popUpRelatorioVigencia.inc");
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Vigência', 'Selecione o filtro desejado' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script src="../includes/calendario.js"></script>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript">

function gerarRelatorio(consulta){
	var formulario = document.formulario;
	formulario.consulta.value = consulta;
	
	var data1 = document.getElementById("dtinicio").value;
	var data2 = document.getElementById("dtfim").value;

	if (data1 == '' || data2 == ''){
		

		selectAllOptions( formulario.n_processo );
		selectAllOptions( formulario.responsavel );
		selectAllOptions( formulario.situacao );
		selectAllOptions( formulario.vencimento );
		selectAllOptions( formulario.contratada );
		selectAllOptions( formulario.objeto );

		var janela = window.open( '', 'relatorio', 'width=900,height=600,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
		formulario.target = 'relatorio';	
//		ajaxRelatorio();
		formulario.submit();
		
		janela.focus();

	}
	else{

		var nova_data1 = parseInt(data1.split("/")[2].toString() + data1.split("/")[1].toString() + data1.split("/")[0].toString());
		var nova_data2 = parseInt(data2.split("/")[2].toString() + data2.split("/")[1].toString() + data2.split("/")[0].toString());
		 
		 if (nova_data2 < nova_data1){
			 alert('A data final não pode ser menor que a data inicial.');
		 return false;
		 }		
	}

	selectAllOptions( formulario.n_processo );
	selectAllOptions( formulario.responsavel );
	selectAllOptions( formulario.situacao );
	selectAllOptions( formulario.vencimento );
	selectAllOptions( formulario.contratada );
	selectAllOptions( formulario.objeto );

	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
	formulario.target = 'relatorio';	
//	ajaxRelatorio();
	formulario.submit();
	
	janela.focus();

	
//valida data
	if(formulario.dtinicio.value != '' && formulario.dtfim.value != ''){
		
		if(!validaData(formulario.dtinicio)){
			alert("Data Início Inválida.");
			formulario.dtinicio.focus();
			return false;
		}		
		if(!validaData(formulario.dtfim)){
			alert("Data Fim Inválida.");
			formulario.dtfim.focus();
			return false;
		}		
	}
	
}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
//function obras_exibeRelatorioGeralXLS(){
//	
//	var formulario = document.formulario;
//	var agrupador  = document.getElementById( 'agrupador' );
//	
//	
//	
//	prepara_formulario();
//	selectAllOptions(formulario.agrupador);
//	
//	 
//		
//	if ( !agrupador.options.length ){
//		alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
//		return false;
//	}
//	
//	selectAllOptions( agrupador );
//	selectAllOptions( document.getElementById( 'vencimento' ) );
//	selectAllOptions( document.getElementById( 'contratada' ) );
//	selectAllOptions( document.getElementById( 'objeto' ) );
//	selectAllOptions( document.getElementById( 'n_processo' ) );
//	selectAllOptions( document.getElementById( 'responsavel' ) );
//	selectAllOptions( document.getElementById( 'situacao' ) );
//		
//	formulario.submit();
//	
//}
</script>
</head>
<body>
<form name="formulario" id="formulario" action="" method="post">	
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<input type="hidden" name="consulta" id="consulta" value="html">
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita">Vencimento:</td>
			<td>
				&nbsp;
				De:	<?= campo_data( 'dtinicio', 'N', 'S', '', 'S','Data Inicial','',''); ?>
				&nbsp;&nbsp;&nbsp;
				até
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfim', 'N', 'S', '', 'S','Data Final','','' ); ?>
			</td>
	</tr>
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('html');" style="cursor: pointer;"/>
			<input type="button" name="Gerar Relatório XLS" value="Gerar Relatório XLS" onclick="javascript:gerarRelatorio('xls');" style="cursor: pointer;"/>
			<!--<input type="button" value="Visualizar XLS" onclick="obras_exibeRelatorioGeralXLS();" style="cursor: pointer;"/>-->
		</td>
	</tr>
</table>
</form>
</body>
</html>