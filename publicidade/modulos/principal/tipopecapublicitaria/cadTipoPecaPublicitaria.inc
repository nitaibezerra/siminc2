<?php
// incluindo as classes necessÃ¡rias
$oTipoPeca = new TipoPeca();

//caso tenha postado o formulÃ¡rio
if($_POST['requisicao'] == 'salvar'){
	
	//verifica se jÃ¡ existe o registro
	$repetido = $oTipoPeca->verifica_unico($_POST);
	
	//caso nÃ£o encontre repetido, salva
	if(!$repetido){
		$resultado = $oTipoPeca->salvar($_POST);
		
		if($resultado[0]){
			$msg = 'Operação realizada com sucesso!';
			direcionar('?modulo=principal/tipopecapublicitaria/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
		else if(!$resultado[0]){
			$msg = 'Operação falhou!';
			direcionar('?modulo=principal/tipopecapublicitaria/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else{
		$msg = 'Tipo de Peça Publicitária já Cadastrado!';
		alerta($msg);
		extract($_POST);
	}
}
//caso seja via url
else if(!empty($_GET['id'])){
	$tipopeca = $oTipoPeca->carrega_registro_por_id($_GET['id']);
	extract($tipopeca);
}

// monta cabeÃ§alho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o tÃ­tulo da tela
monta_titulo('Tipo de Peça Publicitária','');
?>
<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>
	/**
	 * Redireciona para a pesquisa
	 * @name voltar
	 * @return void
	 */
	function voltar(){
		window.location = '?modulo=principal/tipopecapublicitaria/listaTipoPecaPublicitaria&acao=A';
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
	 * Valida os campos obrigatï¿½rios
	 * @name validaFormularioCadastro
	 * @return bool
	 */	
	function validaFormularioCadastro(){
	
		if($('tppdsc').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('tppdsc').focus();
			$('btnSalvar').enable();
	        return false;
		}
	
		return true;
	}
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Tipo de Peça Publicitária: </td>
			<td class="campo">
				<?=campo_texto('tppdsc','S','S','Informe o Tipo de Peça Publicitária',100,255,'','','left','',0,'id="tppdsc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Possui Período de Veiculação: </td>
			<td class="campo">
				<input type='radio' name='tppperiodo' id='tppperiodo' value='S' <?php if($tppperiodo == 'S'){ echo 'checked'; } ?>>Sim
				<input type='radio' name='tppperiodo' id='tppperiodo' value='N' <?php if($tppperiodo == 'N' || empty($tppperiodo)){ echo 'checked'; } ?>>Não
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='tppid' id='tppid' value='<?php echo $tppid; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
				<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
</form>