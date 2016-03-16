<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
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
	 * @name detalharCatalogo
	 * @param catid - Id do catálogo a ser editado
	 * @return void
	 */
	function detalharCatalogo(catid){
		window.location = "?modulo=sistema/tabelasdeapoio/catalogo/editar&acao=A&catid="+catid;
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerCatalogo
	 * @param catid - Id do catálogo a ser excluído
	 * @return void
	 */
	function removerCatalogo(catid){
		window.location = "?modulo=sistema/tabelasdeapoio/catalogo/excluir&acao=A&catid="+catid;
	}

	/**
	 * Limpa pesquisa
	 * @name limpar
	 * @return void
	 */
	function limpar(){
		window.location = '?modulo=sistema/tabelasdeapoio/catalogo/pesquisar&acao=A';
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		
		<tr>
			<td class="SubtituloDireita"> Classe: </td>
			<td class="campo"><?php $oClasse->montaComboClasse($clscodclasse);?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Conta Cont&aacute;bil: </td>
			<td class="campo"><?php $oContaContabil->montaComboContaContabil($ccbid);?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Item da Conta Cont&aacute;bil: </td>
			<td class="campo">
				<div id='itemconta'>
					<?php $oItemContaContabil->montaComboItemContaContabil($ccbid,$icbid);?>
				</div>
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

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/views/partialsViews/lista.inc'; ?>
