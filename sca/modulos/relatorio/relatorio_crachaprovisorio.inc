<?php

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){

	include "crachaprovisorio_resultado.inc";
	exit;

}

include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Crachás Provisórios";
monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );

?>

<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<script type="text/javascript">

function exibeRelatorioCracha(){

	var formulario = document.formulario;

	// Tipo de relatorio
	formulario.pesquisa.value='1';

	prepara_formulario();

	formulario.action = 'sca.php?modulo=relatorio/relatorio_crachaprovisorio&acao=A';
	window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';

	formulario.submit();

}



</script>
<script language='javascript' type='text/javascript' src='./js/sca.js'></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script language="javascript" type="text/javascript" src="../includes/calendario.js"></script>
<script language="javascript" type="text/javascript" src="../includes/prototype.js"></script>
<script language="javascript" type="text/javascript">
function limpaSaida(){

	var formulario = document.formulario;
	formulario.dtfim.value = '';
	formulario.hrfim.value = '';
	document.getElementsByName('hrfim')[0].id = 'hrfim';

	 if($('saida').checked){
         $('dtfim').disable();
         $('hrfim').disable();

         $('dtfim').setAttribute('class', 'disabled');
         $('hrfim').setAttribute('class', 'disabled');
     }
     else{
    	 $('dtfim').enable();
    	 $('hrfim').enable();
    	 $('hrfim').setValue('23:59');

         $('dtfim').setAttribute('class', 'normal');
         $('hrfim').setAttribute('class', 'normal');
     }

}
</script>

<form action="" method="post" name="formulario" id="filtro">

	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->


	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<tr>
			<td class="SubTituloDireita">Data Inicial:</td>
			<td>
				<?php echo campo_data2( 'dtinicio', 'N', 'S', '', 'S' ); ?>
				<?php echo campo_texto('hrinicio','','S','',5,5,'##:##','','','','','','','00:00','validaHora(this);'); ?> Horas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Data Final:</td>
			<td>
				<?php echo campo_data2( 'dtfim', 'N', 'S', '', 'S' ); ?>
				<?php echo campo_texto( 'hrfim','','S','',5,5,'##:##','','','','','','','23:59','validaHora(this);'); ?> Horas
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita"> Filtros para o relatório: </td>
			<td>
				<input type='checkbox' name='saida' id='saida' value='1' onclick="limpaSaida()" /> Servidores que não devolveram o crachá
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorioCracha();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>

