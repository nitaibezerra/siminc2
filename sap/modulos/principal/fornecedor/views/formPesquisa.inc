<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>

	/**
	 * Aciona o filtro
	 * @name pesquisar
	 * @param requisicao - Requisição que será executada
	 * @return void
	 */	
	function pesquisar(requisicao){
		$('requisicao').setValue(requisicao);
		$('formularioPesquisa').submit();
		
	}
	
	/**
	 * Redireciona para a edição
	 * @name detalharFornecedor
	 * @param forcpfcnpj - Documento do fornecedor
	 * @return void
	 */
	function detalharFornecedor(forcpfcnpj){
		window.location = "?modulo=principal/fornecedor/editar&acao=A&forcpfcnpj="+forcpfcnpj;
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerFornecedor
	 * @param forcpfcnpj - Documento do fornecedor
	 * @return void
	 */
	function removerFornecedor(forcpfcnpj){
		window.location = "?modulo=principal/fornecedor/excluir&acao=A&forcpfcnpj="+forcpfcnpj;
	}

	/**
	 * Função responsável por alterar os atributos dos campos do form
	 * de acordo com a seleção de pessoa física, jurídica ou estrangeiro 
	 * @name mudaCampo
	 * @param string valor - Valor checked no radio de tipo de fornecedor
	 * @return void
	 */	
	function mudaCampo(valor){
		if(valor == 'PJ'){
			$('forcpfcnpj').clear();
			$('forcpfcnpj').setAttribute('maxlength','18');
			$('forcpfcnpj').setAttribute('onkeyup',"this.value=mascaraglobal('##.###.###/####-##',this.value);");

			$('forrazaosocial').clear();
			$('forrazaosocial').setAttribute('class','normal');
			$('forrazaosocial').removeAttribute('readonly','readonly');
		}
		else if(valor == 'PF'){
			$('forcpfcnpj').clear();
			$('forcpfcnpj').setAttribute('maxlength','14');
			$('forcpfcnpj').setAttribute('onkeyup',"this.value=mascaraglobal('###.###.###-##',this.value);");

			$('forrazaosocial').clear();
			$('forrazaosocial').setAttribute('class','disabled');
			$('forrazaosocial').setAttribute('readonly','readonly');
		}
		else if(valor == 'IE'){
			$('forcpfcnpj').clear();
			$('forcpfcnpj').setAttribute('maxlength','20');
			$('forcpfcnpj').setAttribute('onkeyup',"this.value=mascaraglobal('####################',this.value);");

			$('forrazaosocial').clear();
			$('forrazaosocial').setAttribute('class','disabled');
			$('forrazaosocial').setAttribute('readonly','readonly');
		}
	}

	/**
	 * Cancela a pesquisa
	 * @name cancelar
	 * @return void
	 */
	function cancelar(){
		window.location = '?modulo=principal/fornecedor/pesquisar&acao=A';
	}

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> CNPJ/CPF/IE: </td>
			<td class="campo">
				<?=campo_texto('forcpfcnpj','N','S','Informe o CNPJ/CPF/IE',25,18,'','','left','',0,'id="forcpfcnpj"', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);',null,"verificaSomenteNumerosCpfCnpj(this,'O CNPJ/CPF/IE deve ser numérico!');");?>
				<input type='radio' name='fortipo' id='fortipo' value='PJ' onclick="javascript:mudaCampo(this.value);" checked>Pessoa Jurídica
				<input type='radio' name='fortipo' id='fortipo' value='PF' onclick="javascript:mudaCampo(this.value);">Pessoa Física
				<input type='radio' name='fortipo' id='fortipo' value='IE' onclick="javascript:mudaCampo(this.value);">Estrangeiro
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Raz&atilde;o Social: </td>
			<td class="campo">
				<?=campo_texto('forrazaosocial','N','S','',100,150,'','','left','',0,'id="forrazaosocial"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Nome Fantasia: </td>
			<td class="campo">
				<?=campo_texto('fornomefantasia','N','S','',100,150,'','','left','',0,'id="fornomefantasia"');?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
            	<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:pesquisar('filtrar');">
                <input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:cancelar();">
            </td>
        </tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/principal/fornecedor/views/partialsViews/lista.inc'; ?>
