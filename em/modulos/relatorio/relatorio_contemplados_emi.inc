<?php
if ( $_REQUEST['req'] != '' ){
	include "relatorio_contemplados_emi_resultado.inc";
	exit;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Escolas Contempladas PROEMI - 2012";
monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );

?>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function exibeRelatorio(tipoXLS){
	document.getElementById('req').value = tipoXLS;
	var formulario = document.formulario;

	selectAllOptions( document.getElementById( 'estuf' ) );
	
	formulario.target = 'contemplados';
	var janela = window.open( '?modulo=relatorio/relatorio_contemplados_emi&acao=A', 'contemplados', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.submit();
	janela.focus();
}
	
/* Função para substituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

/**
 * Alterar visibilidade de um bloco.
 * 
 * @param string indica o bloco a ser mostrado/escondido
 * @return void
 */
function onOffBloco( bloco )
{
	var div_on = document.getElementById( bloco + '_div_filtros_on' );
	var div_off = document.getElementById( bloco + '_div_filtros_off' );
	var img = document.getElementById( bloco + '_img' );
	var input = document.getElementById( bloco + '_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '0';
		img.src = '/imagens/menos.gif';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '1';
		img.src = '/imagens/mais.gif';
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
</script>
<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<form action="" method="post" name="formulario" id="filtro">
	<input type="hidden" id="req" name="req" value=""/>	
		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<?php
			$arrVisivel = array("descricao");
			$arrOrdem = array("descricao");
			
			// Estado
			$stSql = " SELECT
							estuf AS codigo,
							estuf AS descricao
						FROM 
							territorios.estado";
			mostrarComboPopup( 'Estado:', 'estuf',  $stSql, '', 'Selecione o(s) Estado(s)' ,null,null,null,null,$arrVisivel,$arrOrdem);
		?>
		<tr>
			<td class="SubTituloDireita">Expandir Relatório:</td>
			<?
			$arExp = array(
							array("codigo" => 'true',
								  "descricao" => 'Sim'),
							array("codigo" => '',
								  "descricao" => 'Não')								
						  );
			?>
			<td><?=$db->monta_combo("expandir", $arExp, 'S','','', '', '','','N','expandir', '', '', 'Expandir'); ?></td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar"    onclick="exibeRelatorio('gera');" style="cursor: pointer;"/>
				<input type="button" value="VisualizarXLS" onclick="exibeRelatorio('geraxls');">
			</td>
		</tr>
	</table>
</form>
