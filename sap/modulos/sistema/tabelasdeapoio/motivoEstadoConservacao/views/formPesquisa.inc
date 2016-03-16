<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Envia para a tela de novo registro
	 * @name criarNovo
	 * @return void
	 */
	function criarNovo(){

		location.href='?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/adicionar&acao=A';
		
	}

	/**
	 * Aciona o filtro
	 * @name filtrar
	 * @param requisicao - Requisição que será executada
	 * @return boolean
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

		window.location='?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/pesquisar&acao=A';

	}

</script>


<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?php echo campo_texto('mecid', 'N', 'S', 'Informe o Código do Motivo de Estado de Conservação', 3, 3, '', '', '', '', '', 'id="mecid" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O Código deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_texto('mecdsc', 'N', 'S', 'Informe a Descrição do Motivo de Estado de Conservação', 100, 100, '', '', '', '', '', 'id="mecdsc"');?>
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


<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/views/partialsViews/lista.inc'; ?>
