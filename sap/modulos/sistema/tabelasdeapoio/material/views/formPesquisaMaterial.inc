<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
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
	 * Aciona o cadastro de material em popup
	 * @name cadastrarMaterial
	 * @return void
	 */	
	function cadastrarMaterial(){
		window.location='?modulo=sistema/tabelasdeapoio/material/adicionarPopup&acao=A';
	}

	/**
	 * Limpa pesquisa
	 * @name limpar
	 * @return void
	 */
	function limpar(){
		location.href='?modulo=sistema/tabelasdeapoio/material/pesquisarMaterial&acao=A';
	}


	/**
	 * Redireciona para a página de exclusão
	 * @param id - Chave para exclusão
	 * @return void
	 */
	function redirecionaExcluir(id){

		if(confirm('Deseja excluir o registro?'))
			location.href = '?modulo=sistema/tabelasdeapoio/material/excluir&acao=A&popup=S&matid=' + id;

	}

	/**
	 * Redireciona para a página de edição
	 * @param id - Chave para edição
	 * @return void
	 */
	function redirecionaEditar(id){

		location.href = '?modulo=sistema/tabelasdeapoio/material/editarPopup&acao=A&matid=' + id;

	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?php echo campo_texto('matid', 'N', 'S', 'Código do Material', 3, 3, '', '', '', '', '', 'id="matid" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O Código deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_texto('matdsc', 'N', 'S', 'Descrição do Material', 100, 100, '', '', '', '', '', 'id="matdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo SIASG: </td>
			<td class="campo">
				<?php echo campo_texto('matcodsiasg', 'N', 'S', 'Código SIASG', 20, 20, '', '', '', '', '', 'id="matcodsiasg"');?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:filtrar('filtrar');" />
				<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpar();" />
				<input type='button' class="botao" name='btnNovo' id='btnNovo' value='Novo' onclick="javascript:cadastrarMaterial();" />
		    </td>
		</tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/material/views/partialsViews/listaPesquisaMaterial.inc'; ?>
