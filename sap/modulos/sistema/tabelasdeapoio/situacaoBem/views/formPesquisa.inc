<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Aciona o filtro
	 * @name filtrar
	 * @param requisicao - Requisição que será executada
	 * @return void
	 */	
	function filtrar(requisicao){
		$('requisicao').setValue(requisicao);
		$('formularioPesquisa').submit();
	}

	/**
	 * Limpa o filtro
	 * @name limpar
	 * @return void
	 */	
	function limpar(){
		window.location='?modulo=sistema/tabelasdeapoio/situacaoBem/pesquisar&acao=A';
	}

</script>


<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?=campo_texto('sbmcod', 'N', 'S', 'Informe o Código da Situação do Bem', 8, 5, '', '', '', '', '', 'id="sbmcod"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_texto('sbmdsc', 'N', 'S', 'Informe a Descrição da Situação do Bem', 100, 100, '', '', '', '', '', 'id="sbmdsc"');?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='hidden' name='sbmid' id='sbmid' value="<?php echo $sbmid; ?>" />
				<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:filtrar('filtrar');" />
				<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpar();" />
		    </td>
		</tr>
	</table>
</form>


<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/situacaoBem/views/partialsViews/lista.inc'; ?>
