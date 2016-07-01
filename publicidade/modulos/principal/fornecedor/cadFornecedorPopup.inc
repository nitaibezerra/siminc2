<?php
// incluindo as classes necessÃ¡rias
$oFornecedor = new Fornecedor();
$oEstado = new Estado();
$oMunicipio = new Municipio();

//caso tenha postado o formulÃ¡rio
if($_POST['requisicao'] == 'salvar'){
	
	//verifica se jÃ¡ existe o registro
	$repetido = $oFornecedor->verifica_unico($_POST);
	
	//caso nÃ£o encontre repetido, salva
	if(!$repetido){
		$resultado = $oFornecedor->salvar($_POST);
		
		if($resultado[0]){
			alert('Operação realizada com sucesso!');
		}
		else if(!$resultado[0]){
			alert('Operação falhou!');
		}
	}
	else{
		alerta('CNPJ já Cadastrado!');
		extract($_POST);
	}
}
//caso seja via url
else if(!empty($_GET['forid'])){
	
	//carrega o registro por id
	$fornecedor = $oFornecedor->carrega_registro_por_id($_GET['forid']);
	extract($fornecedor);
	
	//formata os dados
	$dddcomercial = substr($fortel,0,2);
	if(!empty($fortel)){
		$fortel = substr($fortel,2,4).'-'.substr($fortel,6);
	}
	
	$dddfax = substr($forfax,0,2);
	if(!empty($forfax)){
		$forfax = substr($forfax,2,4).'-'.substr($forfax,6);
	}
		
}

require_once APPRAIZ . "www/includes/webservice/pj.php";

// monta cabeÃ§alho do sistema
//include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o tÃ­tulo da tela
monta_titulo('Fornecedor','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>

<script language='javascript' type='text/javascript' src='../includes/funcoes.js'></script>
<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='/includes/webservice/pj.js'></script>
<script language='javascript' type='text/javascript'>
	/**
	 * Redireciona para a pesquisa
	 * @name voltar
	 * @return void
	 */
	function voltar(){
		window.location = '?modulo=principal/fornecedor/listaFornecedor&acao=A';
	}
	
	/**
	 * Submete o formulÃ¡rio
	 * @name salvar
	 * @return void
	 */	
	function salvar(){
		$('btnSalvar').disable();
		if(validaFormularioCadastro()){
			$('formularioCadastro').submit();
		}
	}
	
	/**
	 * Valida os campos obrigatÃ³rios
	 * @name validaFormularioCadastro
	 * @return bool
	 */	
	function validaFormularioCadastro(){
	
		if($('forcnpj').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('forcnpj').focus();
			$('btnSalvar').enable();
	        return false;
		}
	
		if($('fornome').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('fornome').focus();
			$('btnSalvar').enable();
	        return false;
		}
		
		if($('dddcomercial').getValue() != '' && $('dddcomercial').getValue().length < 2){
			alert('DDD Inválido!');
			$('dddcomercial').focus()
			$('btnSalvar').enable();
	        return false;
		}
		
		if($('fortel').getValue() != '' && $('fortel').getValue().length < 9){
			alert('Telefone Comercial Inválido!');
			$('fortel').focus()
			$('btnSalvar').enable();
	        return false;
		}
		
		if($('dddfax').getValue() != '' && $('dddfax').getValue().length < 2){
			alert('DDD Inválido!');
			$('dddfax').focus()
			$('btnSalvar').enable();
	        return false;
		}
		
		if($('forfax').getValue() != '' && $('forfax').getValue().length < 8){
			alert('Fax Inválido!');
			$('forfax').focus()
			$('btnSalvar').enable();
	        return false;
		}
	
		return true;
	}
	
	/**
	 * FunÃ§Ã£o responsÃ¡vel por buscar os dados do Fornecedor no webservice da Receita
	 * @name buscaFornecedor
	 * @return void
	 */
	function buscaFornecedor(obj){
		 
		if(obj.value.length == 18){
			if(!validarCnpj(obj.value)){
				alert('CNPJ Inválido');
				obj.value = '';
				return false;
			}else{
				var comp = new dCNPJ();
				comp.buscarDados(obj.value);
				$('fornome').setValue(comp.dados.no_empresarial_rf);
			}
		}
		else if(obj.value.length > 0 && obj.value.length < 18){
			alert('CNPJ Inválido');
			obj.value = '';
		}
	}
	
	
	/**
	 * Callback de sucesso na pesquisa do endereÃ§o por cep 
	 * @name successCarregarEndereco
	 * @param object transport - Retorno
	 * @return void
	 */
	function successCarregarEndereco(transport){
			
		var resposta = transport.responseText.evalJSON();
		
		if(resposta.status == 'ok'){
			
			$('forlog').setValue(resposta.logradouro);
			$('forbai').setValue(resposta.bairro);
			$('estuf').setValue(resposta.estado);
			carregarMunicipio(resposta.estado,resposta.muncod,'N');
			
		}else{
			
			limparEndereco();
			alert('CEP Inválido!');
				
		}
			
	}
	
	/**
	 * Callback de falha na pesquisa do endereÃ§o por cep
	 * @name failureCarregarEndereco
	 * @return void
	 */
	function failureCarregarEndereco(){
	
		limparEndereco();
		alert('CEP Inválido!');
		
	}
	
	/**
	 * Callback de criaÃ§Ã£o na pesquisa do endereÃ§o por cep
	 * @name createCarregarEndereco
	 * @return void
	 */
	function createCarregarEndereco(){
	
		enableButtons(false);
		 
	}
	
	/**
	 * Callback de complete na pesquisa do endereÃ§o por cep
	 * @name completeCarregarEndereco
	 * @return void
	 */
	function completeCarregarEndereco(){
		
		enableButtons();
		
	}
	
	/**
	 * Limpa os campos de endereÃ§o
	 * @name limparEndereco
	 * @return void
	 */
	function limparEndereco(){
		$('forcep').clear();
		$('forlog').clear();
		$('forcomp').clear();
		$('fornum').clear();
		$('forbai').clear();
		$('estuf').clear();
		carregarMunicipio('','','N');
	}
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> CNPJ: </td>
			<td class="campo">
				<?=campo_texto('forcnpj','S','S','Informe o CNPJ',18,18,'','','left','',0,'id="forcnpj"', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);',null,"verificaSomenteNumerosCpfCnpj(this,'O CNPJ deve ser numérico!');buscaFornecedor(this);");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Razão Social: </td>
			<td class="campo">
				<?=campo_texto('fornome','S','S','Informe a Razão Social',100,255,'','','left','',0,'id="fornome"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> E-mail: </td>
			<td class="campo">
				<?=campo_texto('foremail','N','S','Informe o E-mail',50,255,'','','left','',0,'id="foremail"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Telefone Comercial: </td>
			<td class="campo">
				(<?=campo_texto('dddcomercial','N','S','Informe o DDD',2,2,'','','left','',0,'id="dddcomercial" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O DDD deve ser numérico!');");?>)
				 <?=campo_texto('fortel','N','S','Informe o Telefone',9,9,'','','left','',0,'id="fortel"','this.value=mascaraglobal(\'####-####\',this.value);',null,"verificaSomenteNumerosCpfCnpj(this,'O Telefone deve ser numérico!');");?>
				 Ramal: <?=campo_texto('forramal','N','S','Informe o Ramal',4,4,'','','left','',0,'id="forramal"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Fax: </td>
			<td class="campo">
				(<?=campo_texto('dddfax','N','S','Informe o DDD',2,2,'','','left','',0,'id="dddfax" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O DDD deve ser numérico!');");?>)
				 <?=campo_texto('forfax','N','S','Informe o Fax',9,9,'','','left','',0,'id="forfax" onkeypress="return somenteNumeros(event);"','this.value=mascaraglobal(\'####-####\',this.value);',null,"verificaSomenteNumerosCpfCnpj(this,'O Fax deve ser numérico!');");?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Situação: </td>
			<td class="campo"><?php $oFornecedor->monta_combo_situacao();?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Observação:</td>
			<td class="campo">
				<?php echo campo_textarea('forobs','N', 'S', '', 54, 6, 500,'','','','','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Agência:</td>
			<td class="campo">
				<?php echo campo_radio_sim_nao('foragencia','h'); ?>
			</td>
		</tr>
		<tr>
			<th class="SubtituloCentro" colspan="2"> Endereço Comercial </th>
		</tr>
		<tr>
			<td class="SubtituloDireita"> CEP: </td>
			<td class="campo">
				<?php echo campo_texto('forcep','N','S','Informe o CEP',8,8,'','','left','',0,'id="forcep" onkeypress="return somenteNumeros(event);"','',null,"verificaSomenteNumeros(this,'O CEP deve ser numérico!');carregarEndereco(this.value,'successCarregarEndereco','failureCarregarEndereco','createCarregarEndereco','completeCarregarEndereco');");?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Logradouro: </td>
			<td class="campo">
				<?php echo campo_texto('forlog','N','N','',100,255,'','','left','',0,'id="forlog"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Complemento: </td>
			<td class="campo">
				<?php echo campo_texto('forcomp','N','S','Informe o Complemento',100,255,'','','left','',0,'id="forcomp"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Número: </td>
			<td class="campo">
				<?php echo campo_texto('fornum','N','S','Informe o Número',30,30,'','','left','',0,'id="fornum"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Bairro: </td>
			<td class="campo">
				<?php echo campo_texto('forbai','N','N','',100,255,'','','left','',0,'id="forbai"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> UF: </td>
			<td class="campo"><?php $oEstado->monta_combo_uf('','N','cadastro');?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Município: </td>
			<td class="campo"><div id='municipio'><?php $oMunicipio->monta_combo_municipio('');?></div></td>
		</tr>

		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='forid' id='forid' value='<?php echo $forid; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
				<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
</form>
<?php 
if($resultado[0]){
	echo "<script>executarScriptPai('recarregarFornecedores()');self.close();</script>";
}

if((!empty($forid)) && !empty($forcep)){
	echo "<script>carregarEndereco(".$forcep.",'successCarregarEndereco','failureCarregarEndereco','createCarregarEndereco','completeCarregarEndereco');</script>";
}
else if(!empty($forid) && !empty($muncod) && !empty($estuf) && empty($forcep)){
	echo "<script>carregarMunicipio('".$estuf."','".$fornecedor['muncod']."')</script>";
}
?>