<?php
 
if ( $_REQUEST['buscar'] )
{
	include "sustentabilidade_adeq_imp.inc";
	exit();
}



include APPRAIZ . 'includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
$db->cria_aba( $abacod_tela, $url, '' );
monta_titulo( "Relatório de Sustentabilidade e Adequação", "" );
//monta_titulo_cte( $titulo_modulo );
?>

<style>
<!--
table.ex
{
color:#000000;
background-color:#f1f1f1;
font-size: 100%;
padding:0px;
border-top: 1px solid black;
border-left: 1px solid black;
border-bottom: 0px solid black;
border-right: 0px solid black;
}

table.ex th, table.ex td 
{
padding-bottom:2px;
border-top: 0px solid black;
border-left: 0px solid black;
border-bottom: 1px solid black;
border-right: 1px solid black;
}
-->
</style>

<script type="text/javascript">

	function exibirRelatorio()
	{
		var formulario = document.formulario;
		
		// verifica se tem algum agrupador selecionado
		if ( formulario.estuf.value == '' )
		{
			alert( 'Selecione um Estado.' );
			return;
		}
	
	    var objCheckBoxes = document.forms['formulario'].elements['chkano[]'];   
    	var countCheckBoxes = objCheckBoxes.length;
    	var ct = 0;
	    for(var i = 0; i < countCheckBoxes; i++)
	    {   
        	if(objCheckBoxes[i].checked == true){
        		ct = 1;
        		break;
        	}
        	 
 		}
		if(ct == 0)
		{
			alert( 'Selecione pelo menos um Ano.' );
			return;
		}
		
		
		formulario.target = 'resultadoRel';
		var janela = window.open( '', 'resultadoRel', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.submit();
		janela.focus();		
	}

</script>
<script language="JavaScript" src="../includes/funcoes.js"></script>
<script src="../includes/calendario.js"></script>


<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="buscar" value="1">
	<table class="tabela" cellspacing="1" cellpadding="1" bgcolor="#f5f5f5" border="0" align="center" style="border-top: medium none; border-bottom: medium none;">		
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Estado:
			</td>
			<td>
				<?
				$sqlComboEstado = "
					select
						estuf as codigo,
						estdescricao as descricao
					from territorios.estado
					order by
						estdescricao
				";
				
				$estuf = $_REQUEST["estuf"];
				$db->monta_combo("estuf", $sqlComboEstado, "S", "Selecione um Estado", null, null); 
				?>
				&nbsp;
				<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
    	</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Ano:
			</td>
			<td>
				<?$ano=2008;
				for($i=0;$i<4;$i++){?>
					<input type="checkbox" name="chkano[]" id="chkano[]" value="<?=$ano+$i;?>">
					<?=$ano+$i;?>
					&nbsp;&nbsp;&nbsp;				
				<?}?>
				<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' />
			</td>
    	</tr>
    	<!-- 
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Secretário(a) Educação:
			</td>
			<td>
				<?//= campo_texto( 'secretario_educacao', 'N', 'S', '', 50, 50, '', '' ); ?>
			</td>
    	</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Secretário(a) Ciência e Tecnologia:
			</td>
			<td>
				<?//= campo_texto( 'secretario_ciencia_tec', 'N', 'S', '', 50, 50, '', '' ); ?>
			</td>
    	</tr>
    	 -->
    	<!-- 
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Valor Total até 2011 <br>para Assistência Financeira:
			</td>
			<td>
				<?//= campo_texto( 'vlr2011', 'N', 'S', '', 17, 15, '###.###.###,##', '', 'left', '', 0, ''); ?>
			</td>
    	</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" width="20%">
				Valor em 2008 <br>para Assistência Financeira:
			</td>
			<td>
				<?//= campo_texto( 'vlr2008', 'N', 'S', '', 17, 15, '###.###.###,##', '', 'left', '', 0, ''); ?>
			</td>
    	</tr>
    	 -->
	    <tr>
			<td class="SubTituloDireita" valign="top">
				&nbsp;
			</td>
			<td class="SubTituloDireita" style="text-align:left;">
				<input	type="button" name="filtrar" value="Visualizar"	onclick="exibirRelatorio();">
			</td>
		</tr>
	</table>	
</form>

