<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Função que dispara a pesquisa no banco
	 * @name pesquisar
	 * @param requisicao - Indica que é para filtrar os dados
	 * @return void
	 */
	function pesquisar(requisicao){
		$('requisicao').setValue(requisicao);
		$('formularioPesquisa').submit();
	}


	/**
	 * Redireciona para a edição
	 * @name detalharEstadoConservacao
	 * @param ecoid - Id do estado de conservação
	 * @return void
	 */
	function detalharEstadoConservacao(ecoid){
		window.location = "?modulo=sistema/tabelasdeapoio/estadoConservacao/editar&acao=A&ecoid="+ecoid;
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerEstadoConservacao
	 * @param ecoid - Id do estado de conservação
	 * @return void
	 */
	function removerEstadoConservacao(ecoid){
		window.location = "?modulo=sistema/tabelasdeapoio/estadoConservacao/excluir&acao=A&ecoid="+ecoid;
	}

	/**
	 * Limpa o filtro
	 * @name limpar
	 * @return void
	 */	
	function limpar(){
		window.location='?modulo=sistema/tabelasdeapoio/estadoConservacao/pesquisar&acao=A';
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?=campo_texto('ecoid','N','S','Informe o Código do Estado de Conservação',3,3,'','','left','',0,'id="ecoid" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O Código deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_texto('ecodsc','N','S','Informe a Descrição do Estado de Conservação',100,100,'','','left','',0,'id="ecodsc"');?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' />
            	<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:pesquisar('filtrar');" />
                <input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick='javascript:limpar();' />
            </td>
        </tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/estadoConservacao/views/partialsViews/lista.inc'; ?>
