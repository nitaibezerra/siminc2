<?php 
require_once APPRAIZ . "www/includes/webservice/cpf.php";
require_once APPRAIZ . "www/includes/webservice/pj.php";
?>
<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>


<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='/includes/webservice/cpf.js'></script>
<script language='javascript' type='text/javascript' src='/includes/webservice/pj.js'></script>
<script language='javascript' type='text/javascript'>


	/**
	 * Submete o formulário
	 * @name gravarFornecedor
	 * @return void
	 */	
	function gravarFornecedor(){
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

		if($('forcpfcnpj').getValue() == '') {
			alert('O campo "CNPJ/CPF/IE" é obrigatório!');
			$('forcpfcnpj').focus();
			$('btnSalvar').enable();
	        return false;
		}

		//apenas valida cep se o tipo de fornecedor não for estrangeiro
		var tipo = Form.getInputs('formularioCadastro','radio','fortipo').find(function(radio) { return radio.checked; }).value;
		if(tipo != 'IE'){
			if($('forcep').getValue() == '') {
				alert('O campo "CEP" é obrigatório!');
				$('forcep').focus();
				$('btnSalvar').enable();
	       	 return false;
			}
		}

		return true;

	}


	/**
	 * Função responsável por alterar os atributos dos campos do form
	 * de acordo com a seleção de pessoa física, jurídica ou estrangeiro 
	 * @name mudaCampo
	 * @param string valor - Valor checked no radio de tipo de fornecedor
	 * @return void
	 */	
	function mudaCampo(valor,forcpfcnpj){
		if(valor == 'PJ'){

			if(forcpfcnpj != ''){
				var value = $('forcpfcnpj').getValue();
				var campo = '<input type="text" class="disabled" title="Informe o CNPJ/CPF/IE" style="width: 28ex; text-align: left;" readonly="readonly" id="forcpfcnpj" onkeyup="this.value=mascaraglobal(\'##.###.###/####-##\',this.value);" value="'+value+'" maxlength="18" size="26" name="forcpfcnpj">';
			}
			else{
				var value = '';
				var campo = '<input type="text" class="obrigatorio normal" title="Informe o CNPJ/CPF/IE" id="forcpfcnpj" onblur="MouseBlur(this);verificaSomenteNumerosCpfCnpj(this,\'O CNPJ/CPF/IE deve ser numérico!\');buscaFornecedor(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'##.###.###/####-##\',this.value);" value="'+value+'" maxlength="18" size="26" name="forcpfcnpj">';
			}

			
			$('campocpfcnpj').innerHTML = campo;

			restauraCampos();
		}
		else if(valor == 'PF'){

			if(forcpfcnpj != ''){
				var value = $('forcpfcnpj').getValue();
				var campo = '<input type="text" class="disabled" title="Informe o CNPJ/CPF/IE" readonly="readonly" id="forcpfcnpj" onkeyup="this.value=mascaraglobal(\'###.###.###-##\',this.value);" value="'+value+'" maxlength="14" size="26" name="forcpfcnpj">';
			}
			else{
				var value = '';
				var campo = '<input type="text" class="obrigatorio normal" title="Informe o CNPJ/CPF/IE" id="forcpfcnpj" onblur="MouseBlur(this);verificaSomenteNumerosCpfCnpj(this,\'O CNPJ/CPF/IE deve ser numérico!\');buscaFornecedor(this);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'###.###.###-##\',this.value);" value="'+value+'" maxlength="14" size="26" name="forcpfcnpj">';
			}

			
			$('campocpfcnpj').innerHTML = campo;

			restauraCampos();
		}
		else if(valor == 'IE'){

			if(forcpfcnpj != ''){
				var value = $('forcpfcnpj').getValue();
				var campo = '<input type="text" class="disabled" title="Informe o CNPJ/CPF/IE" readonly="readonly" id="forcpfcnpj" onkeyup="this.value=mascaraglobal(\'####################\',this.value);" value="'+value+'" maxlength="20" size="26" name="forcpfcnpj">';
			}
			else{
				var value = '';
				var campo = '<input type="text" class="obrigatorio clsMouseFocus" title="Informe o CNPJ/CPF/IE" id="forcpfcnpj" onblur="verificaSomenteNumerosCpfCnpj(this,\'O CNPJ/CPF/IE deve ser numérico!\');" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'####################\',this.value);" value="'+value+'" maxlength="20" size="26" name="forcpfcnpj">';
			}
			
			
			$('campocpfcnpj').innerHTML = campo;

			modificaCampos();
		}
	}



	/**
	 * Função que restaura os campos para a forma inicial
	 * @name restauraCampos
	 * @return void
	 */	
	function restauraCampos(){

		//campo razão social
		var valorRazaoSocial = $('forrazaosocial').getValue();
		var campoRazaoSocial = "<input type='text' class='disabled' title='Razão Social' style='width: 103ex; text-align: left;' readonly='readonly' id='forrazaosocial' value='"+valorRazaoSocial+"' maxlength='150' size='100' name='forrazaosocial'>";
		$('razaosocial').innerHTML = campoRazaoSocial;


		//campo cep
		var valorCep = $('forcep').getValue();
		var campoCep = '<input type="text" class="obrigatorio normal" title="Informe o CEP" id="forcep" onblur="MouseBlur(this);carregarEndereco(this.value);" onmouseout="MouseOut(this);" onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" onkeyup="this.value=mascaraglobal(\'##.###-###\',this.value);" value="'+valorCep+'" maxlength="10" size="13" name="forcep">'+"&nbsp;<img border='0' title='Indica campo obrigatório.' src='../imagens/obrig.gif'>"+"&nbsp;<input type='button' name='btnBuscaCep' id='btnBuscaCep' value='Buscar CEP' onclick=\"javascript:pesquisaCep();\">";
		$('cep').innerHTML = campoCep;


		//campo uf
		var valorUf = $('foruf').getValue();
		var campoUf = "<input type='text' class='disabled' title='UF' style='width: 5ex; text-align: left;' readonly='readonly' id='foruf' value='"+valorUf+"' maxlength='2' size='2' name='foruf'>";
		$('uf').innerHTML = campoUf;


		//campo nomefantasia
		var valorNomeFantasia = $('fornomefantasia').getValue();
		var campoNomeFantasia = "<input type='text' class='disabled' title='Nome Fantasia' style='width: 103ex; text-align: left;' readonly='readonly' id='fornomefantasia' value='"+valorNomeFantasia+"' maxlength='150' size='100' name='fornomefantasia'>";
		$('nomefantasia').innerHTML = campoNomeFantasia;


		//campo logradouro
		var valorLogradouro = $('forlog').getValue();
		var campoLogradouro = "<input type='text' class='disabled' title='Logradouro' style='width: 103ex; text-align: left;' readonly='readonly' id='forlog' value='"+valorLogradouro+"' maxlength='100' size='100' name='forlog'>";
		$('logradouro').innerHTML = campoLogradouro;


		//campo bairro
		var valorBairro = $('forbai').getValue();
		var campoBairro = "<input type='text' class='disabled' title='Bairro' style='width: 103ex; text-align: left;' readonly='readonly' id='forbai' value='"+valorBairro+"' maxlength='150' size='100' name='forbai'>";
		$('bairro').innerHTML = campoBairro;


		//campo cidade
		var valorCidade = $('forcid').getValue();
		var campoCidade = "<input type='text' class='disabled' title='Cidade' style='width: 103ex; text-align: left;' readonly='readonly' id='forcid' value='"+valorCidade+"' maxlength='100' size='100' name='forcid'>";
		$('cidade').innerHTML = campoCidade;


	}



	/**
	 * Função que modifica os campos ao selecionar a opção estrangeiro
	 * @name modificaCampos
	 * @return void
	 */	
	function modificaCampos(){

		//campo razão social
		var campoRazaoSocial = "<input type='text' class='disabled' title='Razão Social' style='width: 103ex; text-align: left;' readonly='readonly' id='forrazaosocial' value='' maxlength='150' size='100' name='forrazaosocial'>";
		$('razaosocial').innerHTML = campoRazaoSocial;


		//campo cep
		var campoCep = "<input type='text' readonly='readonly' class='disabled' title='CEP' id='forcep' onblur='MouseBlur(this);' onmouseout='MouseOut(this);' onfocus='MouseClick(this);this.select();' onmouseover='MouseOver(this);' value='' maxlength='10' size='13' name='forcep'>";
		$('cep').innerHTML = campoCep;


		//campo uf
		var campoUf = "<input type='text' class='disabled' title='UF' style='width: 5ex; text-align: left;' readonly='readonly' id='foruf' value='' maxlength='2' size='3' name='foruf'>";
		$('uf').innerHTML = campoUf;


		//campo nome fantasia
		var valorNomeFantasia = $('fornomefantasia').getValue();
		var campoNomeFantasia = "<input type='text' class='obrigatorio normal' title='Nome Fantasia' style='width: 103ex; text-align: left;' id='fornomefantasia' value='"+valorNomeFantasia+"' maxlength='150' size='100' name='fornomefantasia'>";
		$('nomefantasia').innerHTML = campoNomeFantasia;


		//campo logradouro
		var valorLogradouro = $('forlog').getValue();
		var campoLogradouro = "<input type='text' class='obrigatorio normal' title='Logradouro' style='width: 103ex; text-align: left;' id='forlog' value='"+valorLogradouro+"' maxlength='100' size='100' name='forlog'>";
		$('logradouro').innerHTML = campoLogradouro;


		//campo bairro
		var valorBairro = $('forbai').getValue();
		var campoBairro = "<input type='text' class='obrigatorio normal' title='Bairro' style='width: 103ex; text-align: left;' id='forbai' value='"+valorBairro+"' maxlength='150' size='100' name='forbai'>";
		$('bairro').innerHTML = campoBairro;


		//campo cidade
		var valorCidade = $('forcid').getValue();
		var campoCidade = "<input type='text' class='obrigatorio normal' title='Cidade' style='width: 103ex; text-align: left;' id='forcid' value='"+valorCidade+"' maxlength='100' size='100' name='forcid'>";
		$('cidade').innerHTML = campoCidade;
	
	}

	


	/**
	 * Função que limpa os campos de endereço
	 * @name limparEndereco
	 * @return void
	 */	
	function limparEndereco(){

		//limpa apenas os campos de endereço
		$('forcep').clear();
		$('forlog').clear();
		$('forbai').clear();
		$('forcid').clear();
		$('foruf').clear();
		 
	}

	/**
	 * Função que abre a pesquisa de cep
	 * @name pesquisaCep
	 * @return void
	 */	
	function pesquisaCep(){
		AbrirPopUp('?modulo=principal/endereco/pesquisarCep&acao=A','buscarCep','scrollbars=yes,width=800,height=570');
	}
	

	/**
	 * Função responsável por buscar os dados do Fornecedor no webservice da Receita
	 * @name buscaFornecedor
	 * @return void
	 */
	function buscaFornecedor( obj ){
		 
		//pega o valor selecionado no campo tipo fornecedor
		//apenas fará a consulta no webservice se o tipo for diferente de estrangeiro
		var tipo = Form.getInputs('formularioCadastro','radio','fortipo').find(function(radio) { return radio.checked; }).value;
		if(tipo != 'IE' && obj.value != ''){
			if(tipo == 'PJ' && obj.value.length == 18){
				if( !validarCnpj( obj.value  ) ){
					alert( "CNPJ inválido!\nFavor informar um CNPJ válido!" );
					obj.value = "";
					return false;
				} else {
					var comp     = new dCNPJ();
					comp.buscarDados( obj.value );
					$('forrazaosocial').setValue( comp.dados.no_empresarial_rf );
					$('fornomefantasia').setValue( comp.dados.no_fantasia_rf );
					var cep = comp.dados.nu_cep;
					cep = cep.substr(0, 2 ) + '.' + cep.substr(2, 3 ) + '-' + cep.substr(5, 3 );
					$('forcep').setValue( cep );
					$('forlog').setValue( comp.dados.ds_logradouro );
					$('forcomp').setValue( comp.dados.ds_logradouro_comp );
					$('fornum').setValue( comp.dados.ds_numero );
					$('forbai').setValue( comp.dados.ds_bairro );
					$('foruf').setValue( comp.dados.sg_uf );
					carregarEndereco(cep);
				}
			} else if(tipo == 'PF' && obj.value.length == 14){
				var valor = obj.value.replace(".", "");
				valor = valor.replace(".", "");
				valor = valor.replace("-", "");
				
				if( !validar_cpf( valor ) ){
					alert( "CPF inválido!\nFavor informar um CPF válido!" );
					obj.value = "";	
					return false;
				} else {		
					var comp = new dCPF();
					comp.buscarDados( valor );
					var cep = comp.dados.nu_cep;
					cep = cep.substr(0, 2 ) + '.' + cep.substr(2, 3 ) + '-' + cep.substr(5, 3 );
					$('forrazaosocial').setValue( comp.dados.no_pessoa_rf );
					$('fornomefantasia').setValue( comp.dados.no_pessoa_rf );
					$('forcep').setValue( cep );
					$('forlog').setValue( comp.dados.ds_logradouro );
					$('forcomp').setValue( comp.dados.ds_logradouro_comp );
					$('fornum').setValue( comp.dados.ds_numero );
					$('forbai').setValue( comp.dados.ds_bairro );
					//$('forcid').setValue( comp.dados.co_cidade );
					$('foruf').setValue( comp.dados.sg_uf );
					carregarEndereco(cep);
				}			
			}
			else if(tipo == 'PJ' && obj.value.length != 18){
				alert('CNPJ inválido!\nFavor informar um CNPJ válido!');
				obj.value = "";
			}
			else if(tipo == 'PF' && obj.value.length != 14){
				alert( "CPF inválido!\nFavor informar um CPF válido!" );
				obj.value = "";
			}
		}
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
	 * Fecha a tela e chama a função que limpa os dados de fornecedor
	 * @name cancelaOperacao
	 * @return void
	 */
	function cancelaOperacao(viaEntradaBens){
		if(viaEntradaBens == 'S'){
			executarScriptPai('limparBuscaFornecedor()');
			this.close();
		}
		else{
			window.location = '?modulo=principal/fornecedor/adicionar&acao=A';
		}
	}

	
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		
		<tr>
			<td class="SubtituloDireita"> CNPJ/CPF/IE: </td>
			<td class="campo">
				<div id='campocpfcnpj' style="display:inline">
				<?=campo_texto('forcpfcnpj','S','S','Informe o CNPJ/CPF/IE',25,18,'','','left','',0,'id="forcpfcnpj"', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);', '', 'verificaSomenteNumerosCpfCnpj(this,\'O CNPJ/CPF/IE deve ser numérico!\');buscaFornecedor(this);');?>
				</div>
				<?php 
				if(!empty($forcpfcnpj)){
				?>
				<input type='radio' name='fortipo' id='fortipo' value='PJ' disabled onclick="javascript:mudaCampo(this.value,'');" <?php if($fortipo == 'PJ'){ echo 'checked'; } ?>>Pessoa Jurídica
				<input type='radio' name='fortipo' id='fortipo' value='PF' disabled onclick="javascript:mudaCampo(this.value,'');" <?php if($fortipo == 'PF'){ echo 'checked'; } ?>>Pessoa Física
				<input type='radio' name='fortipo' id='fortipo' value='IE' disabled onclick="javascript:mudaCampo(this.value,'');" <?php if($fortipo == 'IE'){ echo 'checked'; } ?>>Estrangeiro
				<?php 
				}else{
				?>
				<input type='radio' name='fortipo' id='fortipo' value='PJ' onclick="javascript:mudaCampo(this.value,'');" checked>Pessoa Jurídica
				<input type='radio' name='fortipo' id='fortipo' value='PF' onclick="javascript:mudaCampo(this.value,'');">Pessoa Física
				<input type='radio' name='fortipo' id='fortipo' value='IE' onclick="javascript:mudaCampo(this.value,'');">Estrangeiro
				<?php 
				}
				?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Raz&atilde;o Social: </td>
			<td class="campo">
				<div id='razaosocial'>
					<?=campo_texto('forrazaosocial','N','N','',100,150,'','','left','',0,'id="forrazaosocial"');?>
            	</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Nome Fantasia: </td>
			<td class="campo">
				<div id='nomefantasia'>
					<?=campo_texto('fornomefantasia','N','N','',100,150,'','','left','',0,'id="fornomefantasia"');?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<div id='cep'>
					<?=campo_texto('forcep','S','S','Informe o CEP',12,10,'##.###-###','','left','',0,'id="forcep"', '', '', 'carregarEndereco(this.value);');?>
					<input type='button' name='btnBuscaCep' id='btnBuscaCep' value='Buscar CEP' onclick="javascript:pesquisaCep();">
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Logradouro: </td>
			<td class="campo">
				<div id='logradouro'>
					<?=campo_texto('forlog','N','N','',100,100,'','','left','',0,'id="forlog"');?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Complemento: </td>
			<td class="campo">
				<?=campo_texto('forcomp','N','S','Informe o Complemento',100,100,'','','left','',0,'id="forcomp"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero: </td>
			<td class="campo">
				<?=campo_texto('fornum','N','S','Informe o N&uacute;mero',4,4,'','','left','',0,'id="fornum"','',null,"verificaSomenteNumeros(this,'O Número deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Bairro: </td>
			<td class="campo">
				<div id='bairro'>
					<?=campo_texto('forbai','N','N','',100,150,'','','left','',0,'id="forbai"');?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Cidade: </td>
			<td class="campo">
				<div id='cidade'>
					<?=campo_texto('forcid','N','N','',100,100,'','','left','',0,'id="forcid"');?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo">
				<div id='uf'>
					<?=campo_texto('foruf','N','N','',2,2,'','','left','',0,'id="foruf"');?>
				</div>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> E-Mail: </td>
			<td class="campo">
				<?=campo_texto('foremail','N','S','Informe o E-Mail',50,50,'','','left','',0,'id="foremail"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data do Cadastro: </td>
			<td class="campo">
				<?php 
				if(empty($dtcadastro)){
					$dtcadastro = date('d/m/Y');
				}
				?>
				<?=campo_texto('dtcadastro','N','N','',12,12,'','','left','',0,'id="dtcadastro"','',$dtcadastro);?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='edicao' id='edicao' value='<?php if(!empty($forcpfcnpj)) { echo 'S'; } else {  echo 'N'; }?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:gravarFornecedor();">
				<input type='button' class="botao" name='btnCancelar' id='btnCancelar' value='Cancelar' onclick="javascript:cancelaOperacao('<?php echo $_GET['viaEntradaBens']; ?>');">
            </td>
        </tr>
	</table>
</form>

<?php 
if(!empty($forcpfcnpj)){
	echo "<script>mudaCampo('".$fortipo."','".$forcpfcnpj."')</script>";
}

if($_GET['popup'] != 'S'){
	include_once APPSAP . 'modulos/principal/fornecedor/views/partialsViews/lista.inc'; 
}
?>