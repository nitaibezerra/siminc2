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
	 * @name detalharTipoMovimentacao
	 * @param tmvid - Id do tipo de movimentação
	 * @return void
	 */
	function detalharTipoMovimentacao(tmvid){
		window.location = "?modulo=sistema/tabelasdeapoio/tipoMovimentacao/editar&acao=A&tmvid="+tmvid;
	}


	 /**
	 * Redireciona para a exclusão
	 * @name removerTipoMovimentacao
	 * @param tmvid - Id do tipo de movimentação
	 * @return void
	 */
	function removerTipoMovimentacao(tmvid){
		window.location = "?modulo=sistema/tabelasdeapoio/tipoMovimentacao/excluir&acao=A&tmvid="+tmvid;
	}

	/**
	 * Limpa o filtro
	 * @name limpar
	 * @return void
	 */	
	function limpar(){
		window.location='?modulo=sistema/tabelasdeapoio/tipoMovimentacao/pesquisar&acao=A';
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?=campo_texto('tmvid','N','S','Informe o Código do Tipo de Movimentação',3,3,'','','left','',0,'id="tmvid" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O Código deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_texto('tmvdsc','N','S','Informe a Descrição do Tipo de Movimentação',100,100,'','','left','',0,'id="tmvdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Situa&ccedil;&atilde;o: </td>
			<td class="campo">
				<input type='radio' name='tmvstatus' id='tmvstatus' value='T' <?php echo $tmvstatus == 'T' || !$tmvstatus ? 'checked' : '' ; ?>>Todos
				<input type='radio' name='tmvstatus' id='tmvstatus' value='A' <?php echo $tmvstatus == 'A' ? 'checked' : '' ; ?>>Ativos
				<input type='radio' name='tmvstatus' id='tmvstatus' value='I' <?php echo $tmvstatus == 'I' ? 'checked' : '' ; ?>>Inativos
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

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoMovimentacao/views/partialsViews/lista.inc'; ?>
