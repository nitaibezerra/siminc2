<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link type="text/css" rel="stylesheet" href="../includes/Estilo.css" />
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Limpa a parte do formulario de busca de classe
	 * @name limpaBusca
	 * @return void
	 */
	function limparBusca(){

		 $$('#clscodclasse, #clsdescclasse').invoke('setValue', '');
		 
	}

	/**
	 * Submete o form
	 * @name gravarMaterial
	 * @return void
	 */
	function gravarMaterial(){
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

		if($('clscodclasse').getValue() == ''){
			alert('O campo "Classe" é obrigatório!');
			$('clscodclasse').focus();
			$('btnSalvar').enable();
			return false;
		}

		if($('ccbid').getValue() == ''){
			alert('O campo "Conta Contábil" é obrigatório!');
			$('ccbid').focus();
			$('btnSalvar').enable();
			return false;
		}

		if($('icbid').getValue() == ''){
			alert('O campo "Item da Conta Contábil" é obrigatório!');
			$('icbid').focus();
			$('btnSalvar').enable();
			return false;
		}

		if($('matdsc').getValue() == ''){
			alert('O campo "Descrição do Material" é obrigatório!');
			$('matdsc').focus();
			$('btnSalvar').enable();
			return false;
		}

		return true;
	}

	/**
	 * Buscar Classe
	 * @name buscarClasse
	 * @return void
	 */	
	function buscarClasse(){
		AbrirPopUp('?modulo=sistema/tabelasdeapoio/catalogo/pesquisarClasse&acao=A','buscarClasse','scrollbars=yes,width=800,height=470');
	}

	/**
	 * Cancela operação
	 * @name cancelar
	 * @return void
	 */
	function cancelar(){
	
		location.href='?modulo=sistema/tabelasdeapoio/material/pesquisarMaterial&acao=A';
		
	}

</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo: </td>
			<td class="campo">
				<?php echo campo_texto('matcodigo', 'N', 'N', '', 8, 8, '', '', '', '', '', 'id="matcodigo"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Classe: </td>
			<td class="campo">
				<?php echo campo_texto('clscodclasse', 'N', 'N', '', 5, 5, '', '', '', '', '', 'id="clscodclasse"');?>
				<?php echo campo_texto('clsdescclasse', 'N', 'N', '', 100, 100, '', '', '', '', '', 'id="clsdescclasse"');?>
				<input type='button' class="botao" name='btnBuscarClasse' id='btnBuscarClasse' value='Buscar Classe' onclick="javascript:buscarClasse();" />
				<!-- <input type='button' class="botao" name='btnLimparBusca' id='btnLimparBusca' value='Limpar Busca' onclick="limparBusca()" />  -->
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Conta Cont&aacute;bil: </td>
			<td class="campo">
				<div id='contacontabil'>
					<?php $oContaContabil->montaComboPorClasse($clscodclasse,$ccbid); ?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Item da Conta: </td>
			<td class="campo">
				<div id='itemconta'>
					<?php $oItemContaContabil->montaComboPorClasse($clscodclasse, $ccbid, $icbid); ?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descri&ccedil;&atilde;o do Material: </td>
			<td class="campo">
				<?php echo campo_texto('matdsc', 'S', 'S', 'Informe a Descrição do Material', 100, 100, '', '', '', '', '', 'id="matdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> C&oacute;digo SIASG: </td>
			<td class="campo">
				<?php echo campo_texto('matcodsiasg', 'N', 'S', 'Informe o Código SIASG', 20, 20, '', '', '', '', '', 'id="matcodsiasg"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita" valign="top"> Observa&ccedil;&atilde;o: </td>
			<td class="campo">
				<?php echo campo_textarea('matobs', 'N', 'S', 'Informe a Observação sobre o material', 120, 8, 500);?>
            </td>
		</tr>
		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='matid' id='matid' value='<?php echo $_GET['matid']; ?>' />
				<input type='hidden' name='requisicao' id='requisicao' value='salvar' />
				<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:gravarMaterial();" />
				<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancelar();" />
		    </td>
		</tr>
	</table>
</form>