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
	 * @name detalharTipoEntradaSaida
	 * @param tesid - Id do tipo de entrada/saída
	 * @return void
	 */
	function detalharTipoEntradaSaida(tesid){
		window.location = "?modulo=sistema/tabelasdeapoio/tipoEntradaSaida/editar&acao=A&tesid="+tesid;
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerTipoEntradaSaida
	 * @param tesid - Id do tipo de entrada/saída
	 * @return void
	 */
	function removerTipoEntradaSaida(tesid){
		window.location = "?modulo=sistema/tabelasdeapoio/tipoEntradaSaida/excluir&acao=A&tesid="+tesid;
	}


	/**
	 * Limpa pesquisa
	 * @name limpar
	 * @return void
	 */
	function limpar(){
		window.location = '?modulo=sistema/tabelasdeapoio/tipoEntradaSaida/pesquisar&acao=A';
	}
	
</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Sigla: </td>
			<td class="campo">
				<?=campo_texto('tesslg','N','S','Informe a Sigla',4,4,'','','left','',0,'id="tesslg"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_texto('tesdsc','N','S','Informe a Descrição do Tipo de Entrada/Saída',100,100,'','','left','',0,'id="tesdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Tipo de Documento: </td>
			<td class="campo"><?php $oTipoEntradaSaida->montaComboTipoDocumento();?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Tipo: </td>
			<td class="campo">
				<input type='radio' name='testpo' id='testpo' value='T' <?php echo !$testpo || $testpo == 'T' ? 'checked' : ''; ?>>Todos
				<input type='radio' name='testpo' id='testpo' value='E' <?php echo $testpo == 'E' ? 'checked' : ''; ?>>Entrada
				<input type='radio' name='testpo' id='testpo' value='S' <?php echo $testpo == 'S' ? 'checked' : ''; ?>>Saída
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

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoEntradaSaida/views/partialsViews/lista.inc'; ?>
