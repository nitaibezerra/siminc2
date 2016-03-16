<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript' src='../includes/calendario.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script type="text/javascript">

	/**
	 * Aciona o filtro
	 * @name filtrar
	 * @param requisição - Requisição que será executada
	 * @return void
	 */	
	function filtrar(requisicao){
		$('requisicao').setValue(requisicao);
		$('formularioPesquisa').submit();
	}

	/**
	 * Abre o cadastro de fornecedor
	 * @name cadastrarFornecedor
	 * @return void
	 */	
	function cadastrarFornecedor(){
		window.location='?modulo=principal/fornecedor/adicionarPopup&acao=A&popup=S&viaEntradaBens=S';
	}

	/**
	 * Limpa os campos da pesquisa
	 * @name limpar
	 * @return void
	 */	
	function limpar(){
		var popup = $('popup').getValue();
		window.location='?modulo=principal/fornecedor/pesquisarFornecedor&acao=A&popup='+popup;
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
			$('forcpfcnpj').setValue('');
			$('forcpfcnpj').setAttribute('maxlength','18');
			$('forcpfcnpj').setAttribute('onkeyup',"this.value=mascaraglobal('##.###.###/####-##',this.value);");

			$('forrazaosocial').clear();
			$('forrazaosocial').setAttribute('class','normal');
			$('forrazaosocial').removeAttribute('readonly','readonly');
		}
		else if(valor == 'PF'){
			$('forcpfcnpj').setValue('');
			$('forcpfcnpj').setAttribute('maxlength','14');
			$('forcpfcnpj').setAttribute('onkeyup',"this.value=mascaraglobal('###.###.###-##',this.value);");

			$('forrazaosocial').clear();
			$('forrazaosocial').setAttribute('class','disabled');
			$('forrazaosocial').setAttribute('readonly','readonly');
		}
		else if(valor == 'IE'){
			$('forcpfcnpj').setValue('');
			$('forcpfcnpj').setAttribute('maxlength','20');
			$('forcpfcnpj').setAttribute('onkeyup',"this.value=mascaraglobal('####################',this.value);");

			$('forrazaosocial').clear();
			$('forrazaosocial').setAttribute('class','disabled');
			$('forrazaosocial').setAttribute('readonly','readonly');
		}
	}


	/**
	 * Redireciona para a edição
	 * @name detalharFornecedor
	 * @param forcpfcnpj - Documento do fornecedor
	 * @return void
	 */
	function detalharFornecedor(forcpfcnpj){
		window.location = "?modulo=principal/fornecedor/editarPopup&acao=A&forcpfcnpj="+forcpfcnpj+"&popup=S&viaEntradaBens=S";
	}


	/**
	 * Redireciona para a exclusão
	 * @name removerFornecedor
	 * @param forcpfcnpj - Documento do fornecedor
	 * @return void
	 */
	function removerFornecedor(forcpfcnpj){
		window.location = "?modulo=principal/fornecedor/excluir&acao=A&forcpfcnpj="+forcpfcnpj+"&popup=S&viaEntradaBens=S";
	}	

</script>

<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		
		<tr>
			<td class="SubtituloDireita"> CNPJ/CPF/IE: </td>
			<td class="campo">
				<?=campo_texto('forcpfcnpj','N','S','Informe o CNPJ/CPF/IE',25,18,'','','left','',0,'id="forcpfcnpj"', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);', null,"verificaSomenteNumerosCpfCnpj(this,'O CNPJ/CPF/IE deve ser numérico!');");?>
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
			<td class="SubtituloDireita"> Nome / Nome Fantasia: </td>
			<td class="campo">
				<?=campo_texto('fornomefantasia','N','S','',100,150,'','','left','',0,'id="fornomefantasia"');?>
            </td>
		</tr>

		<tr class="buttons">
			<td colspan='2'>
				<input type='hidden' name='requisicao' id='requisicao' />
				<input type='hidden' name='popup' id='popup' value='<?php echo $_GET['popup']; ?>' />
				<input type='button' class="botao" name='btnFiltrar' id='btnFiltrar' value='Filtrar' onclick="javascript:filtrar('filtrar');" />
				<input type='button' class="botao" name='btnLimpar' id='btnLimpar' value='Limpar' onclick="javascript:limpar();" />
				<input type='button' class="botao" name='btnNovo' id='btnNovo' value='Novo' onclick="javascript:cadastrarFornecedor();" />
		    </td>
		</tr>
	</table>
</form>

<?php include_once APPSAP . 'modulos/principal/fornecedor/views/partialsViews/listaPesquisaFornecedor.inc'; ?>
