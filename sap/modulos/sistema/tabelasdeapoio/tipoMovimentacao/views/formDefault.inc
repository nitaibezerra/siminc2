<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>


	/**
	 * Submete o formulário
	 * @name gravarTipoMovimentacao
	 * @return void
	 */
	function gravarTipoMovimentacao(){
		$('btnSalvar').disable();
		if(validaFormulario()){
			$('formularioCadastro').submit();
		}
	}


	/**
	 * Valida os campos obrigatórios
	 * @name validaFormulario
	 * @return bool
	 */
	function validaFormulario(){

		if($('tmvdsc').getValue() == '') {
			alert('O campo "Descrição" é obrigatório!');
			$('tmvdsc').focus();
			$('btnSalvar').enable();
	        return false;
		}
	
		return true;

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
	 * Cancela a ação
	 * @name cancelar
	 * @return void
	 */	
	function cancelar(){
		window.location='?modulo=sistema/tabelasdeapoio/tipoMovimentacao/pesquisar&acao=A';
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?=campo_texto('tmvid','N','N','',3,3,'','');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_texto('tmvdsc','S','S','Informe a Descrição do Tipo de Movimentação',100,100,'','','left','',0,'id="tmvdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Situa&ccedil;&atilde;o: </td>
			<td class="campo">
				<input type='radio' name='tmvstatus' id='tmvstatus' value='A' checked>Ativo
				<input type='radio' name='tmvstatus' id='tmvstatus' value='I' <?php if($tmvstatus == 'I'){ echo 'checked'; } ?>>Inativo
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:gravarTipoMovimentacao();" />
                <input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick='javascript:cancelar();' />
            </td>
        </tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoMovimentacao/views/partialsViews/lista.inc'; ?>
