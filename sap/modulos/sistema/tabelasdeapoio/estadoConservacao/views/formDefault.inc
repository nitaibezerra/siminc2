<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>


	/**
	 * Submete o formulário
	 * @name gravarEstadoConservacao
	 * @return void
	 */
	function gravarEstadoConservacao(){
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

		if($('ecodsc').getValue() == '') {
			alert('O campo "Descrição" é obrigatório!');
			$('ecodsc').focus();
			$('btnSalvar').enable();
	        return false;
		}
	
		return true;

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
	 * Cancela a ação
	 * @name cancelar
	 * @return void
	 */	
	function cancelar(){
		window.location='?modulo=sistema/tabelasdeapoio/estadoConservacao/pesquisar&acao=A';
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?=campo_texto('ecoid','N','N','',3,3,'','');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_texto('ecodsc','S','S','Informe a Descrição do Estado de Conservação',100,100,'','','left','',0,'id="ecodsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita" valign='top'> Motivos Associados: </td>
			<td class="campo">
				<?php $oMotivoEstadoConservacao->montaComboMultMotivoEstadoConservacao($oEstadoMotivos->pegarMotivosPorEstadoConservacao($ecoid)); ?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:gravarEstadoConservacao();" />
                <input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancelar();" />
            </td>
        </tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/estadoConservacao/views/partialsViews/lista.inc'; ?>
