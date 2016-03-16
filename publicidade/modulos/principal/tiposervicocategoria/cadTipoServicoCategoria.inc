<?php
// incluindo as classes necessÃ¡rias
$oTipoServico = new TipoServico();

//caso tenha postado o formulÃ¡rio
if($_POST['requisicao'] == 'salvar'){
	
	//verifica se jÃ¡ existe o registro
	$repetido = $oTipoServico->verifica_unico($_POST);
	
	//caso nÃ£o encontre repetido, salva
	if(!$repetido){
		$resultado = $oTipoServico->salvar($_POST);
		
		if($resultado[0]){
			$msg = 'Operação realizada com sucesso!';
			direcionar('?modulo=principal/tiposervicocategoria/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
		else if(!$resultado[0]){
			$msg = 'Operação falhou!';
			direcionar('?modulo=principal/tiposervicocategoria/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else{
		$msg = 'Tipo de Serviço e Categoria já Cadastrado!';
		alerta($msg);
		extract($_POST);
	}
}
//caso seja via url
else if(!empty($_GET['id'])){
	$tiposervico = $oTipoServico->carrega_registro_por_id($_GET['id']);
	extract($tiposervico);
}

// monta cabeÃ§alho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o tÃ­tulo da tela
monta_titulo('Tipo de Serviço','');
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
		window.location = '?modulo=principal/tiposervicocategoria/listaTipoServicoCategoria&acao=A';
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
	
		if($('tseid').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('tseid').focus();
			$('btnSalvar').enable();
	        return false;
		}
		
		if($('catdsc').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('catdsc').focus();
			$('btnSalvar').enable();
	        return false;
		}
	
		return true;
	}
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Tipo de Serviço: </td>
			<td class="campo"><?php $oTipoServico->monta_combo_tipo_servico($tseid,'S');?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Categoria: </td>
			<td class="campo">
				<?=campo_texto('catdsc','S','S','Informe a Categoria',100,255,'','','left','',0,'id="catdsc"');?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='catid' id='catid' value='<?php echo $catid; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
				<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
</form>