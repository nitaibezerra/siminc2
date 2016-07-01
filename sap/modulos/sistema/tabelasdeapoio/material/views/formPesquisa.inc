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
	 * Limpa pesquisa
	 * @name limpar
	 * @return void
	 */
	function limpar(){
		location.href='?modulo=sistema/tabelasdeapoio/material/pesquisar&acao=A';
	}

</script>


<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?php echo campo_texto('matid', 'N', 'S', 'Informe o Código do Material', 3, 3, '', '', '', '', '', 'id="matid" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O Código deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_texto('matdsc', 'N', 'S', 'Informe a Descrição do Material', 100, 100, '', '', '', '', '', 'id="matdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo SIASG: </td>
			<td class="campo">
				<?php echo campo_texto('matcodsiasg', 'N', 'S', 'Informe o Código SIASG', 20, 20, '', '', '', '', '', 'id="matcodsiasg"');?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:filtrar('filtrar');" />
				<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick='javascript:limpar();' />
		    </td>
		</tr>
	</table>
</form>


<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/views/partialsViews/lista.inc'; ?>
