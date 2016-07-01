<?php

//Quando come�a a editar uma pad limpa lista dos itens que ser�o impressos
unset($_SESSION['ITENS_MARCADOS_LIST']);

//verifica se o acesso à tela foi via aba para tornar a tela readonly ou não
if(empty($_GET['id']) && (!empty($_SESSION['ipaid']) || !empty($_SESSION['ipaids']))){
	$readOnly = 'S';
}

//mantendo dados em sessão para navegação entre abas
if(!empty($_GET['id']) && empty($_SESSION['id'])){
	$_SESSION['id'] = $_GET['id'];
}
else if(empty($_GET['id']) && !empty($_SESSION['id'])){
	$_GET['id'] = $_SESSION['id'];
}

if($_GET['redirectItensPad']) 
{
	
	$nota = + $_REQUEST['nota'];
	$ob = + $_REQUEST['ob'];
	
	echo	"<script>window.location = '?modulo=principal/pad/cadItemPad&acao=A&redirectItensPad=true&nota={$nota}&ob={$ob}'</script>";
}


// incluindo as classes necessárias
$oPad = new Pad();
$oFornecedor = new Fornecedor();
$oCampanha = new Campanha();

//caso tenha postado o formulário
if($_POST['requisicao'] == 'salvar'){
	
	//verifica se já existe o registro
	$repetido = $oPad->verifica_unico($_POST);
	
	$aux = explode('/',$_POST['pademissao']);
	$erroData = '';
	if($aux[2] < 2000){
		$erroData = 'anoinvalido';
	}
	
	//caso não encontre repetido, salva
	if(!$repetido && empty($erroData)){
		$resultado = $oPad->salvar($_POST);
		
		if($resultado[0]){
			direcionar('?modulo=principal/pad/'.$resultado[2].'&acao=A&id='.$resultado[1]);
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/pad/'.$resultado[2].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else if($repetido){
		$msg = utf8_decode('PAD Já Cadastrada!');
		alerta($msg);
		$_POST['erro'] = 'padcadastrada';
	}
	else if($erroData == 'anoinvalido'){
		$msg = utf8_decode('A data informada é inválida!');
		alerta($msg);
		$_POST['erroData'] = $erroData;
	}
	extract($_POST);
}
//caso seja via url
else if(!empty($_GET['id'])){

	$pad = $oPad->carrega_registro_por_id($_GET['id']);
	extract($pad);
	
	//caso o usuário tenha clicado em visualizar
	if($_GET['visualizar'] == 'S'){
		$_SESSION['visualizar'] = 'S';
	}
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

//verificações para exibir as abas corretas
if(!empty($_SESSION['ipaid']) || !empty($_SESSION['ipaids'])){
	if($_SESSION['ultimaPagina'] == 'glosar'){
		$arMnuid = array(ABA_PAGAMENTO_ITEM_PAD);
	}
	else if($_SESSION['ultimaPagina'] == 'pagar'){
		$arMnuid = array();
	}
	else if($_SESSION['ultimaPagina'] == 'faturar'){
		$arMnuid = array(ABA_GLOSA_ITEM_PAD,ABA_PAGAMENTO_ITEM_PAD);	
	}
}
else{
	$arMnuid = array(ABA_ITEM_PAD,ABA_FATURAMENTO_ITEM_PAD,ABA_GLOSA_ITEM_PAD,ABA_PAGAMENTO_ITEM_PAD);
}	
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('PAD','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
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
	 * Valida os campos obrigatórios
	 * @name validaFormularioCadastro
	 * @return bool
	 */	
	function validaFormularioCadastro(){
	
		if($('padnumero').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Número da PAD" é obrigatório!'));
			alert(msg);
			$('padnumero').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('padnumero').getValue().length < 8){
			var msg = decodeURIComponent(escape('Nº da PAD inválido!'));
			alert(msg);
			$('padnumero').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('pademissao').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Data de Emissão" é obrigatório!'));
			alert(msg);
			$('pademissao').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('padano').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Ano" é obrigatório!'));
			alert(msg);
			$('padano').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('forid').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Agência" é obrigatório!'));
			alert(msg);
			$('forid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('camid').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Campanha" é obrigatório!'));
			alert(msg);
			$('camid').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('padvalor').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Valor Total" é obrigatório!'));
			alert(msg);
			$('padvalor').focus();
			$('btnSalvar').enable();
	        return false;
		}
		
		return true;
	}


	/**
	 * Callback de sucesso na pesquisa do contrato por agência 
	 * @name successCarregarContrato
	 * @param object transport - Retorno
	 * @return void
	 */
	function successCarregarContrato(transport){

		 var resposta = transport.responseText.evalJSON();

		if(resposta.status == 'ok'){
			
			$('cttnumcontrato').setValue(resposta.cttnumcontrato);
			$('cttid').setValue(resposta.cttid);
		}else{
			limparContrato();

			var ag = document.getElementById('forid');
			var opt = ag.getElementsByTagName('option').value;

			if(opt){
				var msg = 'Contrato N�o Encontrado!';
				alert(msg);
			}
				
		}
			
	}
	
	/**
	 * Callback de falha na pesquisa do contrato por agência
	 * @name failureCarregarContrato
	 * @return void
	 */
	function failureCarregarContrato(){

		 limparContrato();
		 var msg = decodeURIComponent(escape('Contrato Não Encontrado!'));
		 alert(msg);
		
	}
	
	/**
	 * Callback de criação na pesquisa do contrato por agência
	 * @name createCarregarContrato
	 * @return void
	 */
	function createCarregarContrato(){
	
		enableButtons(false);
		 
	}
	
	/**
	 * Callback de complete na pesquisa do contrato por agência
	 * @name completeCarregarContrato
	 * @return void
	 */
	function completeCarregarContrato(){
		
		enableButtons();
		
	}


	/**
	 * Limpa os campos do contrato
	 * @name limparContrato
	 * @return void
	 */
	function limparContrato(){
		$('cttid').clear();
		$('cttnumcontrato').clear();
	}

	 Event.observe(window, 'load', function() {

		 //verifica se teve erro de data. 
		 //caso haja coloca o foco no campo correto
		 if($F('erroData') == 'anoinvalido'){
		     $('pademissao').focus();	
		 }
		 		
	 });


	 /**
	  * Avançar para a próxima página
	  * @name avancar
	  * @param int id - identificador do registro
	  * @return void
	  */
	 function avancar(id){
		 window.location = '?modulo=principal/pad/cadItemPad&acao=A&id='+id;
	 }
	
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero da PAD: </td>
			<td class="campo">
				<?php 
				if(empty($padnumero)){
					$padnumero = date('Y');
				}
				echo campo_texto('padnumero','S','S','Informe o N&uacute;mero da PAD',10,8,'####/###','','left','',0,'id="padnumero"');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Data de Emiss&atilde;o: </td>
			<td class="campo">
				<?php echo campo_data2('pademissao','S','S','Informe a Data de Emiss&atilde;o','N','','',$pademissao)?>
            </td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita"> N&ordm; Sidoc: </td>
			<td class="campo">
				<?=campo_texto('padnumsidoc','N','S','Informe o N&uacute;mero',22,20,'#####.######/####-##','','left','',0,'id="padnumsidoc"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Programa de Trabalho - PT: </td>
			<td class="campo">
				<?= campo_texto('padptres','',($edicaoBloqueada?'N':'S'),'Informe o Programa de Trabalho - PTRES',25,21,'##.###.####.####.####','','left','',0,'id="padptres"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Ano: </td>
			<td class="campo"><?php $oPad->monta_combo_ano('cadPad','S');?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Ag&ecirc;ncia: </td>
			<td class="campo">
			<?php 
			if($readOnly == 'S'){
				$oFornecedor->monta_combo_fornecedor($forid,'S',1);
			}
			else{
				$oFornecedor->monta_combo_fornecedor($forid,'S',1,'cadPad');
			}
			?>
			<script>
				var ag = document.getElementById('forid');
				var opt = ag.getElementsByTagName('option');

				for(var i = 0; i < opt.length; i++) {
					if(opt[0].value == '') {
						opt[0].value = 0;
					}						
				}
			</script>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Contrato: </td>
			<td class="campo">
				<?=campo_texto('cttnumcontrato','N','N','',10,8,'','','left','',0,'id="cttnumcontrato"');?>
				<input type='hidden' name='cttid' id='cttid' value='<?php echo $cttid ?>'>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Campanha: </td>
			<td class="campo"><?php	$oCampanha->monta_combo_campanha($camid,'S');?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor Total: </td>
			<td class="campo">
				<?php
				if(empty($padvalor) || (!empty($padvalor) && $erro == 'padcadastrada')){
					echo campo_texto('padvalor','S','S','',16,14,'[###.]###,##','','left','',0,'id="padvalor"');
				}
				else{
					echo campo_texto('padvalor','S','S','',16,14,'[###.]###,##','','left','',0,'id="padvalor"','',number_format($padvalor, 2, ',', '.'));
				}
				?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<?php 
            	if($_SESSION['visualizar'] != 'S'){
            	?>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnSalvar' id='btnSalvar' value='Pr&oacute;xima >>' onclick="javascript:salvar();">
            	<?php 
            	}
            	else if($_SESSION['visualizar'] == 'S'){
            	?>
            	<input type='button' class="botao" name='btnSalvar' id='btnSalvar' value='Pr&oacute;xima >>' onclick="javascript:avancar(<?php echo $padid; ?>);">
            	<?php 
            	}
            	?>
            	<input type='hidden' name='padid' id='padid' value='<?php echo $padid; ?>'>
            	<input type='hidden' name='padstatus' id='padstatus' value='<?php echo $padstatus; ?>'>
            	<input type='hidden' name='erroData' id='erroData' value='<?php echo $erroData; ?>'>
            	
            </td>
        </tr>
	</table>
</form>