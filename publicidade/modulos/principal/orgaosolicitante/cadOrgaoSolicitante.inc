<?php
// incluindo as classes necessÃ¡rias
$oOrgaoSolicitante = new OrgaoSolicitante();

//caso tenha postado o formulÃ¡rio
if($_POST['requisicao'] == 'salvar'){
	
	//verifica se jÃ¡ existe o registro
	$repetido = $oOrgaoSolicitante->verifica_unico($_POST);
	
	//caso nÃ£o encontre repetido, salva
	if(!$repetido){
		$resultado = $oOrgaoSolicitante->salvar($_POST);
		
		if($resultado[0]){
			$msg = 'Operação realizada com sucesso!';
			direcionar('?modulo=principal/orgaosolicitante/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
		else if(!$resultado[0]){
			$msg = 'Operação falhou!';
			direcionar('?modulo=principal/orgaosolicitante/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else{
		$msg = 'Sigla Já Cadastrada!';
		alerta($msg);
		extract($_POST);
	}
}
//caso seja via url
else if(!empty($_GET['id'])){
	$orgao = $oOrgaoSolicitante->carrega_registro_por_id($_GET['id']);
	extract($orgao);
}

// monta cabeÃ§alho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o tÃ­tulo da tela
$msg = 'Órgão Solicitante';
monta_titulo($msg,'');
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
		window.location = '?modulo=principal/orgaosolicitante/listaOrgaoSolicitante&acao=A';
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
	
		if($('orgsg').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('orgsg').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('orgdsc').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('orgdsc').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('orgstatus').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('orgstatus').focus();
			$('btnSalvar').enable();
	        return false;
		}
		
		return true;
	}
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Sigla: </td>
			<td class="campo">
				<?=campo_texto('orgsg','S','S','Informe a Sigla',30,30,'','','left','',0,'id="orgsg"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Órgão: </td>
			<td class="campo">
				<?=campo_texto('orgdsc','S','S','Informe o Órgão',100,255,'','','left','',0,'id="orgdsc"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Status: </td>
			<td class="campo">
			<?php 
			
			//valida se for cadastro status fica desabilitado, se for alteração fica habilitado.
			if(empty($orgid)){
				$oOrgaoSolicitante->monta_combo_status('A','S');
			}
			else{
				$oOrgaoSolicitante->monta_combo_status($orgstatus);
			}
			/*
			if(empty($orgid)){
				$oOrgaoSolicitante->monta_combo_status('A','N');
			}
			else{
				$oOrgaoSolicitante->monta_combo_status($orgstatus);
			}*/
			
			?>
			</td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='orgid' id='orgid' value='<?php echo $orgid; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
				<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
</form>