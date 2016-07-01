<link type="text/css" rel="stylesheet" href="./css/default.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript'>


	/**
	 * Submete o formulário
	 * @name gravarTipoEntradaSaida
	 * @return void
	 */
	function gravarTipoEntradaSaida(){
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

		if($('tesslg').getValue() == ''){
			alert('O campo "Sigla" é obrigatório!');
			$('tesslg').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('tesdsc').getValue() == '') {
			alert('O campo "Descrição" é obrigatório!');
			$('tesdsc').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('tdcid').getValue() == '') {
			alert('O campo "Tipo de Documento" é obrigatório!');
			$('tdcid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		var option = '';
		for(var i=0; i<document.formularioCadastro.testpo.length; i++){
			if(document.formularioCadastro.testpo[i].checked){
				option = document.formularioCadastro.testpo[i].value
			}
		}
		if(option == ''){
			alert('O campo "Tipo" é obrigatório');
			$('btnSalvar').enable();
			return false;
		}
	
		return true;

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
	 * Cancela operação
	 * @name cancelar
	 * @return void
	 */
	function cancelar(){
		window.location = '?modulo=sistema/tabelasdeapoio/tipoEntradaSaida/pesquisar&acao=A';
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Sigla: </td>
			<td class="campo">
				<?=campo_texto('tesslg','S',($tesst == 't')?'N':'S','Informe a Sigla',4,4,'','','left','',0,'id="tesslg"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o: </td>
			<td class="campo">
				<?=campo_texto('tesdsc','S','S','Informe a Descrição do Tipo de Entrada/Saída',100,100,'','','left','',0,'id="tesdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Tipo de Documento: </td>
			<td class="campo"><?php $oTipoEntradaSaida->montaComboTipoDocumento('cadastro');?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Tipo: </td>
			<td class="campo">
				<input type='radio' name='testpo' id='testpo' value='E' <?php if($testpo == 'E'){ echo 'checked'; } ?>>Entrada
				<input type='radio' name='testpo' id='testpo' value='S' <?php if($testpo == 'S'){ echo 'checked'; } ?>>Saída
			</td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='tesid' id='tesid' value='<?php echo $tesid; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:gravarTipoEntradaSaida();" />
                <input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancelar();" />
            </td>
        </tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/sistema/tabelasdeapoio/tipoEntradaSaida/views/partialsViews/lista.inc'; ?>
