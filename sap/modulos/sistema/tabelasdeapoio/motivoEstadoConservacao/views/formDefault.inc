<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Envia para a tela de pesquisa
	 * @name pesquisar
	 * @return void
	 */
	function pesquisar(){

		location.href='?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/pesquisar&acao=A';
		
	}

	/**
	 * Salvar o registro
	 * @name salvar
	 * @param requisicao - Requisição que será executada
	 * @return boolean
	 */	
	function salvar(requisicao){

		$('btnSalvar').disable();

		if($('mecdsc').getValue() == ''){

			alert('Preencha a descrição.');
			$('btnSalvar').enable();
			return false;
			
		}else{

			$('requisicao').setValue(requisicao);
			$('formularioCadastro').submit();
			
		}
		
	}

	/**
	 * Cancela a ação
	 * @name cancelar
	 * @return void
	 */	
	function cancelar(){

		window.location='?modulo=sistema/tabelasdeapoio/motivoEstadoConservacao/pesquisar&acao=A';

	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?php echo campo_texto('mecid', 'N', 'N', '', 3, 3, '###', '', '', '', '', 'id="mecid"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_texto('mecdsc', 'S', 'S', 'Informe a Descrição do Motivo de Estado de Conservação', 100, 100, '', '', '', '', '', 'id="mecdsc"');?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar('salvar');" />
				<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick='javascript:cancelar();' />
		    </td>
		</tr>
	</table>
</form>


<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/motivoEstadoConservacao/views/partialsViews/lista.inc'; ?>
