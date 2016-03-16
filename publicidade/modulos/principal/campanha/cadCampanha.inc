<?php
// incluindo as classes necessÃ¡rias
$oCampanha = new Campanha();
$oOrgaoSolicitante = new OrgaoSolicitante();

//caso tenha postado o formulÃ¡rio
if($_POST['requisicao'] == 'salvar'){
	
	//verifica se jÃ¡ existe o registro
	$repetido = $oCampanha->verifica_unico($_POST);
	
	//caso nÃ£o encontre repetido, salva
	if(!$repetido){
		$resultado = $oCampanha->salvar($_POST);
		
		if($resultado[0]){
			$msg = 'Operação realizada com sucesso!';
			direcionar('?modulo=principal/campanha/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
		else if(!$resultado[0]){
			$msg = 'Operação falhou!';
			direcionar('?modulo=principal/campanha/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else{
		$msg = 'Campanha Já Cadastrada!';
		alerta($msg);
		extract($_POST);
	}
}
//caso seja via url
else if(!empty($_GET['id'])){
	$campanha = $oCampanha->carrega_registro_por_id($_GET['id']);
	extract($campanha);
}

// monta cabeÃ§alho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array();
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o tÃ­tulo da tela
monta_titulo('Campanha','');
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
		window.location = '?modulo=principal/campanha/listaCampanha&acao=A';
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
	
		if($('camtitulo').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('camtitulo').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('orgid').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('orgid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('camdsc').getValue() == '') {
			alert('Preencher campo obrigatório!');
			$('camdsc').focus();
			$('btnSalvar').enable();
	        return false;
		}
		
		return true;
	}
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Campanha: </td>
			<td class="campo">
				<?=campo_texto('camtitulo','S','S','Informe a Campanha',100,255,'','','left','',0,'id="camtitulo"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Órgão Solicitante: </td>
			<td class="campo"><?php $oOrgaoSolicitante->monta_combo_orgao($orgid,'S');?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Descrição: </td>
			<td class="campo">
				<?php echo campo_textarea('camdsc','S', 'S', '', 54, 6, 4000,'','','','','',''); ?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='camid' id='camid' value='<?php echo $camid; ?>'>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
				<input type='button' class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
</form>