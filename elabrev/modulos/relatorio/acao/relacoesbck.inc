<?php

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório de Propostas em Ações', '' );


/* PROPOSTA DE SQL PARA SUBSTITUIR A ABAIXO.
 * LUCIANO F. RIBEIRO - 06/12/2012
$sql_acao = "
Select	distinct acaid as codigo,
	'( '|| prgcod || ' / ' || acacod || ' - ' || acadsc ||' )' as descricao
	
From elabrev.ppaacao_proposta

Where acastatus in ('A', 'N') and prsano = '".$_SESSION[ 'exercicio' ]."'
Order by descricao;
";
*/

$sql_acao = "Select distincit a.acaid as codigo,
					' ( ' || a.prgcod || '\'-\'' 	. ' || ' . 
						'a.acacod' 	. ' || ' . 
						'.' 		. ' || ' . 
						'a.acadsc' 	. 
					' ) ' .
					' AS ' . 'descricao' .
				' FROM ' .
					'elabrev.ppaacao_proposta a' .
				' WHERE ' .
					'a.acastatus' .
					' IN ' . 
						'(' .
							'\'A\'' . 
							' , ' .
							''\N\'' .
						')' .
					' AND ' .
						'a.prsano'  . ' = ' . '\'' . $_SESSION[ 'exercicio' ] . '\'' .
				' ORDER BY ' . 'descricao';
?>

<form action="" method="post" name="formulario">
	<!-- indica envio de formulário -->
	<input type="hidden" name="act" value="0"> 
	<input type="hidden" name="geral" value="a" id="geral">
	<table class="tabela" align="center" bgcolor="#f5f5f5" 
	cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
        	<td align='right' class="SubTituloDireita">Ações:</td>
	        <td>
    	    	<?
       				combo_popup( 'acaid', $sql_acao, 'Selecione a(s) Ação (ões)', '360x460' )
				?>
			</td>      
    	</tr>   
		<tr>
			<td align='right' class="SubTituloDireita">
				Filtros:
			</td>
			<td>
				<input type="checkbox" name="cbexclusao" <? if ($_REQUEST['cbexclusao']) print ' checked'?>" />
				Exclusão&nbsp;&nbsp;
				<input type="checkbox" name="cbmigracao" <? if ($_REQUEST['cbmigracao']) print ' checked'?>" />
				Migração&nbsp;&nbsp;
				<input type="checkbox" name="cbfusao" <? if ($_REQUEST['cbfusao']) print ' checked'?>" />
				Fusão&nbsp;&nbsp;
				<input type="checkbox" name="cbalteracao" <? if ($_REQUEST['cbalteracao']) print ' checked'?>" />
				Alteração de atributos
				</td>      
			</tr>
		<tr>
			<td align='right' class="SubTituloDireita">
				Status de decisão:
			</td>
			<td>
				<input type="checkbox" name="cbmec" <? if ($_REQUEST['cbmec']) print ' checked'?>" />
				Coordenador da UMA&nbsp;&nbsp;
				<input type="checkbox" name="cbuma" <? if ($_REQUEST['cbuma']) print ' checked'?>" />
				UMA&nbsp;&nbsp;
				<input type="checkbox" name="cbunid" <? if ($_REQUEST['cbunid']) print ' checked'?>" />
				Unidade&nbsp;&nbsp;
				<input type="checkbox" name="cbsemdec" <? if ($_REQUEST['cbsemdec']) print ' checked'?>" />
				Sem decisão
			</td>      
		</tr>  
</table>
	
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" 
	cellspacing="1" cellpadding="3" style="border-top:none;">
			<tr>
				<td align="center">
					<input type="button" name="Gerar Relatório" value="Gerar Relatório" 
					onclick="gera_relatorio( );" />
					&nbsp;
					<input type="button" name="Exportar Planilha" value="Exportar Planilha" 
					onclick="javascript:submeterFormulario( 'planilha' );" />
				</td>
			</tr>
	</table>
</form>
<?
	unset($_SESSION['rsdados']);
  
	if ($_REQUEST['act']=='gerar')
	{
		include "geraproposta.inc";
	}
?>
<script type="text/javascript">

function gera_relatorio()
{
    prepara_formulario();
    if ( ! validaBranco( document.formulario.acaid, 'Escolha pelo menos uma ação!')) 
    	return tinyMCE.execCommand('mceFocus', true, 'acaid');
    document.formulario.act.value='gerar'	;
    document.formulario.submit();
}
	
function gerarpdf()
{
	//Abre popup em branco
	janela = window.open(
		'about:blank',
		"relatorio",
		"menubar=no,location=no,open=yes,resizable=yes,scrollbars=yes,status=yes,width=600,height=400'"
		);
	//Abre relatório pdf no popup
	janela.location = "<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/prn_prop_aca&acao=R";
}	
</script>