<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script type="text/javascript">


	function exibeRelatorioMovimentacaoEquipamento(){

		var formulario = document.formulario;

		// Tipo de relatorio
		formulario.pesquisa.value='1';

		prepara_formulario();


		var formulario = document.formulario;
		//var agrupador  = document.getElementById( 'agrupador' );


		//valida data
		//if(formulario.mvedatentrada.value != ''){
			
		//	if(!validaData(formulario.mvedatentrada)){
			//	alert("Data Inicial Inválida.");
			//	formulario.mvedatentrada.focus();
			//	return false;
		//	}
			
			
		//}
		//else
		//{
		//	alert("É necessário o preencher a data de entrada/saída");
		//	formulario.mvedatentrada.focus();
		//	return false
		//}
				
		//if(formulario.mvedatsaida.value != ''){
			//if(!validaData(formulario.mvedatsaida)){
			//	alert("Data Final Inválida.");
				//formulario.mvedatsaida.focus();
				//return false;
			//}
			
		//}
		//else
		//{
		//	alert("É necessário o preencher a data de saída");
			//formulario.mvedatentrada.focus();
			//return false
		//}


		formulario.target = 'resultadoMovimentacaoEquipamento';
		var janela = window.open( '?modulo=relatorio/relatorio_processos&acao=A', 'resultadoMovimentacaoEquipamento', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );

		formulario.submit();
		janela.focus();



	}
</script>
<?php
// exibe consulta
if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){

	switch($_REQUEST['pesquisa']) {
		case '1':
			include "movimentacaoequipamento_resultado.inc";
			exit;
		case '2':
			include "movimentacaoequipamento_resultado.inc";
			exit;
	}

}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$titulo_modulo = "Relatório de Movimentação de Equipamentos";
monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );

?>



<form action="" method="post" name="formulario" id="filtro">

	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>


	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">

		<tr>
			<td class="SubtituloDireita" width="170px" >Data Inicial:</td>
			<td>
				<?=campo_data2('mvedatentrada','N','S','Data Inicial','S');?>
				<?=campo_texto( 'horaentrada', 'N', 'S', '', 5, 5, '##:##', '', 'left', '', 0, 'id="horaentrada"','','00:00','validaHora(this);'); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" width="170px" >Data Final:</td>
			<td>
				<?=campo_data2('mvedatsaida','N','S','Data Final','S');?>
				<?=campo_texto( 'horasaida', 'N', 'S', '', 5, 5, '##:##', '', 'left', '', 0, 'id="horasaida"','','23:59','validaHora(this);'); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorioMovimentacaoEquipamento();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
