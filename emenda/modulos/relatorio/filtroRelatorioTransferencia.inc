<?php
include APPRAIZ. '/includes/Agrupador.php';
require_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
unset($_SESSION['emenda']['relcoluna']);
monta_titulo( 'Relatórios Geral - Emendas', '<b>Filtros de Pesquisa</b>');


function colunas(){
	return array(
				array('codigo' => 'mapcod', 			 'descricao' => 'Modalidade'),
				array('codigo' => 'ptrnumconvenio', 	 'descricao' => 'Nº Convênio do FNDE'),
				array('codigo' => 'bennome', 			 'descricao' => 'Beneficiário'),
				array('codigo' => 'pactuado', 			 'descricao' => 'Valores Pactuados'),
				array('codigo' => 'repassado', 			 'descricao' => 'Valores Repassados'),
				array('codigo' => 'vigencia', 			 'descricao' => 'Vigência'),
				array('codigo' => 'pmcnumconveniosiafi', 'descricao' => 'Nº do Convênio SIAFI'),
				array('codigo' => 'enbcnpj', 			 'descricao' => 'CNPJ do Convenente'),
				);
}
?>
<script type="text/javascript" src="/includes/prototype.js"></script>
<form action="emenda.php?modulo=relatorio/relatorioInforTransferencia&acao=A" method="post" name="formulario" id="formulario">
<input type="hidden" name="limpaSession" id="limpaSession" value="true">
<table id="table" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<tr>
		<td class="SubTituloDireita" style="width: 33%" valign="top"><b>Colunas</b></td>
		<td>
			<?php
			
			$coluna = colunas();
			$campoColuna = new Agrupador( 'formulario' );
			$campoColuna->setOrigem( 'colunaOrigem', null, $coluna );
			$campoColuna->setDestino( 'coluna', null, array(
															array('codigo' => 'mapcod', 			 'descricao' => 'Modalidade'),
															array('codigo' => 'ptrnumconvenio', 	 'descricao' => 'Nº Convênio do FNDE'),
															array('codigo' => 'bennome', 			 'descricao' => 'Beneficiário'),
															array('codigo' => 'pactuado', 			 'descricao' => 'Valores Pactuados'),
															array('codigo' => 'repassado', 			 'descricao' => 'Valores Repassados'),
															array('codigo' => 'vigencia', 			 'descricao' => 'Vigência')
															));
			$campoColuna->exibir();
			?>
		</td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="button" value="Pesquisar" onclick="geraPopRelatorio();" style="cursor: pointer;"/>
		</td>
	</tr>
</table>
</form>
<script type="text/javascript">
	function geraPopRelatorio(){
		var form = $('formulario');
		
		if (form.elements['coluna'][0] == null){
			alert('Selecione pelo menos uma coluna!');
			return false;
		}

		selectAllOptions( form.coluna );
		form.target = 'tranferencia';
		var janela = window.open('emenda.php?modulo=relatorio/relatorioInforTransferencia&acao=A','tranferencia','toolbar=no,location=no,status=no,menubar=yes,scrollbars=yes,resizable=yes,fullscreen=yes');
		janela.focus();
		form.submit();
	}
</script>