<?
#
# Sistema Simec
# Setor responsável: SPO-MEC
# Desenvolvedor: Equipe Consultores Simec
# Analista: Gilberto Arruda Cerqueira Xavier
# Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
# Programador: Thiago Mata (e-mail thiago.henrique.mata@gmail.com )
# Módulo:relprogramas.inc
# Finalidade: permitir a construção de relatorio de propostas de alteração em programas
#


include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório de Propostas em Programas', '' );

$sql_unid = ' SELECT ' . 
				' DISTINCT ' .
					'u.unicod' . ' AS '. 'codigo' .
				' , ' .
					' ( ' .
						' u.unicod' . ' || ' . 
						'\'-\'' 	. ' || ' . 
						'u.unidsc' 	. 
					' ) ' .
					' AS ' . 'descricao' .
				' FROM ' .
					' unidade u ' .
				' WHERE ' .
					'u.unistatus' .
					' IN ' . 
						'(' .
							'\'A\'' . 
						')' .
					' AND ' .
						'u.orgcod'  . ' = ' . '\'' . $_SESSION[ 'ittorgao' ] . '\'' .
				' ORDER BY ' . 'descricao';	
				
$sql_prg = ' SELECT ' . 
				' DISTINCT ' .
					'p.prgid' . ' AS '. 'codigo' .
				' , ' .
					' ( ' .
						' p.prgcod' . ' || ' . 
						'\'-\'' 	. ' || ' . 
						'p.prgdsc' 	. 
					' ) ' .
					' AS ' . 'descricao' .
				' FROM ' .
					'elabrev.ppaprograma_proposta p' .
				' WHERE ' .
				    'p.orgcod=\''.$_SESSION['ittorgao'].'\' AND '.
					'p.prgstatus' .
					' IN ' . 
						'(' .
							'\'A\',\'N\'' . 
						')' .
					' AND ' .
						'p.prsano'  . ' = ' . '\'' . $_SESSION[ 'exercicio' ] . '\'' .
				' ORDER BY ' . 'descricao';	
?>

<form action="" method="post" name="formulario">
	<!-- indica envio de formulário -->
	<input type="hidden" name="act" value="0"> 
	<input type="hidden" name="geral" value="a" id="geral">
	<table class="tabela" align="center" bgcolor="#f5f5f5" 
	cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<!--<tr>
        	<td align='right' class="SubTituloDireita">Unidades:</td>
	        <td>
    	    	<?
       				//combo_popup( 'unicod', $sql_unid, 'Selecione a(s) Unidade(s)', '360x460' )
				?>
			</td>      
    	</tr>	-->
		<tr>
        	<td align='right' class="SubTituloDireita">Programas:</td>
	        <td>
    	    	<?
       				combo_popup( 'prgid', $sql_prg, 'Selecione o(s) Programa(s)', '360x460' )
				?>
			</td>      
    	</tr>
    	<tr>	
		<td align='right' class="SubTituloDireita">
				Filtros:
			</td>
			<td>
				<input type="checkbox" name="cbexclusao" <? if ($_REQUEST['cbexclusao']) print ' checked'?>>
				Exclusão&nbsp;&nbsp;
				<input type="checkbox" name="cbalteracao" <? if ($_REQUEST['cbalteracao']) print ' checked'?>>
				Alteração de atributos
				</td>      
			</tr>
		<tr>
			<td align='right' class="SubTituloDireita">
				Status de decisão:
			</td>
			<td>
				<input type="checkbox" name="cbmec" <? if ($_REQUEST['cbmec']) print ' checked'?>>
				Coordenador da UMA&nbsp;&nbsp;
				<input type="checkbox" name="cbuma" <? if ($_REQUEST['cbuma']) print ' checked'?>>
				UMA&nbsp;&nbsp;
				<input type="checkbox" name="cbunid" <? if ($_REQUEST['cbunid']) print ' checked'?>>
				Unidade&nbsp;&nbsp;
				<input type="checkbox" name="cbsemdec" <? if ($_REQUEST['cbsemdec']) print ' checked'?>>
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
					<!--<input type="button" name="Exportar Planilha" value="Exportar Planilha" 
					onclick="javascript:submeterFormulario( 'planilha' );" />-->
				</td>
			</tr>
	</table>
</form>
<?
 
	if ($_REQUEST['act']=='gerar')
	{
		include "gerapropostaprg.inc";
	}
?>
<script type="text/javascript">

function gera_relatorio()
{
    prepara_formulario();
    if (document.formulario.prgid.value.length==0 )
    {
      alert ('Escolha pelo menos um Programa!');
      return tinyMCE.execCommand('mceFocus', true, 'prgid');
    }
    document.formulario.act.value='gerar'	;
    document.formulario.submit();
}
	

</script>