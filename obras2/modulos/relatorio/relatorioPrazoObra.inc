<?php
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório - Monitoramento de Obras', 'Relatório Validações' );
?>

<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<script src="../library/jquery/jquery.mask.min.js" type="text/javascript" charset="ISO-8895-1"></script>

<form action="obras2.php?modulo=relatorio/popupRelatorioPrazoObras&acao=A" method="post" name="filtro" id="filtro">
<input type="hidden" name="pesquisa" id="pesquisa" value="1"/>

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td class="SubTituloDireita" style="width: 190px;">ID Obras</td>
	<td>
		<?php echo campo_texto( 'obrid', 'N', 'S', '', 47, 60, '', '', 'left', '', 0, ''); ?>
	</td>
</tr>
	<?php
	// Programa
	$stSql = "SELECT
                        prfid AS codigo,
                        prfdesc AS descricao
                 FROM 
                        obras2.programafonte
                 ORDER BY
                        prfdesc ";
	mostrarComboPopup( 'Programa', 'prfid',  $stSql, $stSqlCarregados, 'Selecione o(s) Programa(s)' );
	
	// Fonte
	$stSql = "SELECT
                        tooid AS codigo,
                        toodescricao AS descricao
                 FROM 
                        obras2.tipoorigemobra
                 WHERE
                        toostatus = 'A'
                 ORDER BY
                        toodescricao ";
	
	$sql_carregados = "	SELECT
                                        tooid AS codigo,
                                        toodescricao AS descricao
                                FROM 
                                        obras2.tipoorigemobra
                                WHERE
                                        toostatus = 'A'
                                AND
                                        tooid = 1
                                ORDER BY
                                        toodescricao ";
	
	mostrarComboPopup( 'Fonte', 'tooid',  $stSql, $sql_carregados, 'Selecione a(s) Fonte(s)' );
?>
   

<tr>
	<td class="SubTituloDireita" style="width: 190px;">Situação da Obra:</td>
	<td>
		<?php 
			$sql = "SELECT 
                                        esdid as codigo, 
                                        esddsc as descricao 
                                FROM 
                                        workflow.estadodocumento 
                                WHERE
                                        esdstatus = 'A' AND tpdid = '".TPDID_OBJETO."'
                                ORDER BY 
                                        esddsc";
		
			$db->monta_combo( "esdid", $sql, "S", "Todas", "", "", "", "", "N", "esdid" );			
		?>
	</td>
</tr>
<tr>
	<td class="SubTituloDireita" style="width: 190px;">UF:</td>
	<td>
		<?php 
			$sql = "SELECT
                                        estuf as codigo,
                                        estdescricao as descricao
                                FROM
                                        territorios.estado
                                ORDER BY
                                        estdescricao";
		
			$db->monta_combo( "estuf", $sql, "S", "Todas", "", "", "", "", "N", "estuf" );			
		?>
	</td>
</tr>

<tr>
	<td class="SubTituloDireita" style="width: 190px;">Paralisação:</td>
	<td>
		<?php 	
                        $array_paralisacao = array(array("descricao" => "Sim","codigo" => "Sim"),array("descricao" => "Não","codigo" => "Não"));                        
			$db->monta_combo( "paralisacao", $array_paralisacao, "S", "Selecione", "", "", "", "", "N", "paralisacao" );			
		?>
	</td>
</tr>



<tr>
	<td bgcolor="#CCCCCC"></td>
	<td bgcolor="#CCCCCC">
		<input type="button" value="Visualizar" onclick="obras_exibeRelatorioGeral('exibir');" style="cursor: pointer;"/>
		<input type="button" value="Visualizar XLS" onclick="obras_exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
	</td>
</tr>
</table>
<script type="text/javascript">
var formulario = document.getElementById('filtro');

function obras_exibeRelatorioGeral(tipo){
			 
	/*if ( !document.getElementsByName('orgid[]').item(0).checked &&
		 !document.getElementsByName('orgid[]').item(1).checked &&
		 !document.getElementsByName('orgid[]').item(2).checked ){
		alert( 'Favor selecionar ao menos um tipo de ensino!' );
		return false;
	}*/
	
	if(document.getElementById('prfid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'prfid' ) );
	}
	if(document.getElementById('tooid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById( 'tooid' ) );
	}
	
	formulario.target = 'RelatorioValidacoes';
	var janela = window.open( 'obras2.php?modulo=relatorio/popupRelatorioPrazoObras&acao=A', 'RelatorioValidacoes', 'status=no,menubar=no,toolbar=no,scrollbars=1,resizable=no,fullscreen=yes' );
	janela.focus();
	
	// Tipo de relatorio
	document.getElementById('pesquisa').value= tipo ;
	formulario.submit();		
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
</form>
