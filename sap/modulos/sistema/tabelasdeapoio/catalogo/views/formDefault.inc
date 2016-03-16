<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>


	/**
	 * Submete o formulário
	 * @name gravarCatalogo
	 * @return void
	 */	
	function gravarCatalogo(){
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

		if($('clscodclasse').getValue() == '') {
			alert('O campo "Classe" é obrigatório!');
			$('clscodclasse').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('ccbid').getValue() == '') {
			alert('O campo "Conta Contábil" é obrigatório!');
			$('ccbid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('icbid').getValue() == '') {
			alert('O campo "Item da Conta Contábil" é obrigatório!');
			$('icbid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		return true;

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
	 * Cancela Edicao
	 * @name cancelar
	 * @return void
	 */
	function cancelar(){
		window.location = '?modulo=sistema/tabelasdeapoio/catalogo/pesquisar&acao=A';
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Classe: </td>
			<td class="campo">
				<?php $oClasse->montaComboClasse($clscodclasse,'S');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Conta Cont&aacute;bil: </td>
			<td class="campo">
				<?php $oContaContabil->montaComboContaContabil($ccbid,'S');?> 
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Item da Conta Cont&aacute;bil: </td>
			<td class="campo">
				<div id='itemconta'>
					<?php $oItemContaContabil->montaComboItemContaContabil($ccbid,$icbid,'S');?>
				</div>
			</td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='catid' id='catid' value='<?php echo $_GET['catid']; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="gravarCatalogo();" />
                <input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick='cancelar()' />
            </td>
        </tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/catalogo/views/partialsViews/lista.inc'; ?>
