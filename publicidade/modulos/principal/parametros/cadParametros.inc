<?php
// incluindo as classes necessárias
$oParametro = new Parametro();

//caso tenha postado o formulário
if($_POST['requisicao'] == 'salvar'){
	
	//salva
	$resultado = $oParametro->salvar($_POST);
		
	if($resultado[0]){
		$msg = utf8_decode('Operação realizada com sucesso!');
		direcionar('?modulo=principal/parametros/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
	}
	else if(!$resultado[0]){
		$msg = utf8_decode('Operação falhou!');
		direcionar('?modulo=principal/parametros/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
	}
	
}
//caso seja via url
else{
	$parametros = $oParametro->carrega_registro();
	if(is_array($parametros) && count($parametros) >= 1){
		extract($parametros);
	}
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Par&acirc;metros','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript'>
		
	/**
	 * Submete o formulário
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
	 * Valida os campos obrigat�rios
	 * @name validaFormularioCadastro
	 * @return bool
	 */	
	function validaFormularioCadastro(){
	
		if($('parfiscaltitular').getValue() == '') {
			var msg = decodeURIComponent(escape('O Campo "Fiscal do Contrato - Titular" é Obrigatório!'));
			alert(msg);
			$('parfiscaltitular').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('parfiscalsubstituto').getValue() == '') {
			var msg = decodeURIComponent(escape('O Campo "Fiscal do Contrato - Substituto" é Obrigatório!'));
			alert(msg);
			$('parfiscalsubstituto').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('parchefetitular').getValue() == '') {
			var msg = decodeURIComponent(escape('O Campo "Chefe da ACS - Titular" é Obrigatório!'));
			alert(msg);
			$('parchefetitular').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('parchefesubstituto').getValue() == '') {
			var msg = decodeURIComponent(escape('O Campo "Chefe da ACS - Substituto" é Obrigatório!'));
			alert(msg);
			$('parchefesubstituto').focus();
			$('btnSalvar').enable();
	        return false;
		}
		
		return true;
	}
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Fiscal do Contrato - Titular: </td>
			<td class="campo">
				<?=campo_texto('parfiscaltitular','S','S','Informe o Fiscal Titular',100,200,'','','left','',0,'id="parfiscaltitular"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Fiscal do Contrato - Substituto: </td>
			<td class="campo">
				<?=campo_texto('parfiscalsubstituto','S','S','Informe o Fiscal Substituto',100,200,'','','left','',0,'id="parfiscalsubstituto"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Chefe da ACS - Titular: </td>
			<td class="campo">
				<?=campo_texto('parchefetitular','S','S','Informe o Chefe Titular',100,200,'','','left','',0,'id="parchefetitular"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Chefe da ACS - Substituto: </td>
			<td class="campo">
				<?=campo_texto('parchefesubstituto','S','S','Informe o Chefe Substituto',100,200,'','','left','',0,'id="parchefesubstituto"');?>
			</td>
		</tr>
		
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='parid' id='parid' value='<?php echo $parid; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
            </td>
        </tr>
	</table>
</form>