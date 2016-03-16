<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
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
		window.location = '?modulo=sistema/tabelasdeapoio/catalogo/pesquisarClasse&acao=A';
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?php echo campo_texto('clscodclasse', 'N', 'S', 'Informe o Código da Classe', 5, 5, '', '', '', '', '', 'id="clscodclasse" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O Código da Classe deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Classe: </td>
			<td class="campo">
				<?php echo campo_texto('clsdescclasse', 'N', 'S', 'Informe a Descrição da Classe', 100, 100, '', '', '', '', '', 'id="clsdescclasse"');?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:filtrar('filtrar');" />
				<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpar();" />
		    </td>
		</tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/views/partialsViews/listaClasse.inc'; ?>
