<?php

//verifica se o acesso à tela foi via aba para tornar a tela readonly ou não
if(empty($_GET['ipaid']) && (!empty($_SESSION['ipaid']) || !empty($_SESSION['ipaids'])) && $_SESSION['ultimaPagina'] != 'pagar'){
	$readOnly = 'S';
}

//controlando sessão de página acessada
if($_SESSION['ultimaPagina'] != 'faturar' && $_SESSION['ultimaPagina'] != 'glosar'){
	$_SESSION['ultimaPagina'] = 'pagar';
}


// incluindo as classes necessárias
$oItemPad = new ItemPad();
$oPagItemPad = new PagamentoItemPad();
$oFatItemPad = new FaturamentoItemPad();


if(!empty($_GET['ipaid'])){
	unset($_SESSION['ipaids']);
}


if(!empty($_SESSION['ipaids'])){
	$_GET['ipaid'] = $_SESSION['ipaids'].',';
}


//pega os ids caso o usuário tenha selecionado + de 1 item
$aux = explode(',',$_GET['ipaid'],-1);
$erro = '';
if(count($aux) > 1){
	foreach($aux as $key => $value){
		$oItemPad->carregarPorId($value);
		if(!$oItemPad->isStatusFaturado() && !$oItemPad->isStatusGlosado()){
			$erro = 'S';
		}	
	}
	if($erro != 'S'){
		$ipaids = implode(',',$aux);
		$_GET['ipaid'] = $aux[0];
		$_SESSION['ipaids'] = $ipaids;
	}
}
else if(count($aux) == 1){
	$oItemPad->carregarPorId($aux[0]);
	if(!$oItemPad->isStatusFaturado() && !$oItemPad->isStatusGlosado() && !$oItemPad->isStatusPago()){
		$erro = 'S';
	}
	$_GET['ipaid'] = $aux[0];	
}

if($erro == 'S'){
	$msg = utf8_decode('Não é permitido editar o pagamento de mais de um item!');
	direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_SESSION['id'],$msg);
}


//mantendo dados em sessão para navegação entre abas
if(!empty($_GET['ipaid']) && empty($_SESSION['ipaid'])){
	$_SESSION['ipaid'] = $_GET['ipaid'];
}
else if(empty($_GET['ipaid']) && !empty($_SESSION['ipaid'])){
	$_GET['ipaid'] = $_SESSION['ipaid'];
}
else if(empty($_GET['ipaid']) && empty($_SESSION['ipaid'])){
	direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_SESSION['id']);
}




//caso tenha postado o formulário
if($_POST['requisicao'] == 'salvar' && empty($_POST['ipaids'])){
	
	//verifica o ano
	$erroData = $oPagItemPad->verifica_data($_POST);
	
	if(empty($erroData)){
		
		$resultado = $oPagItemPad->salvar($_POST);
		
		if($resultado[0]){
			
			//atualiza o status do item
			$oItemPad->atualizar_status($resultado[2],ItemPad::ST_PAGO);
			
			unset($_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina']);
			
			$msg = utf8_decode('Operação realizada com sucesso!');
			direcionar('?modulo=principal/pad/'.$resultado[4].'&acao=A&id='.$resultado[3],$msg);
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/pad/'.$resultado[4].'&acao=A&id='.$resultado[3].'&ipaid='.$resultado[2],$msg);
		}
	}
	else{
		$msg = utf8_decode('A Data Informada é Inválida!');
		alerta($msg);
		$_POST['erroData'] = $erroData;
		extract($_POST);
	}
}
else if($_POST['requisicao'] == 'salvar' && !empty($_POST['ipaids'])){
	
	//verifica o ano
	$erroData = $oPagItemPad->verifica_data($_POST);
	if(empty($erroData)){
		$aux = explode(',',$_POST['ipaids']);
		foreach($aux as $key => $value){
			$_POST['ipaid'] = $value;
			$resultado = $oPagItemPad->salvar($_POST);
			
			if($resultado[0]){
				//atualiza o status do item
				$oItemPad->atualizar_status($resultado[2],ItemPad::ST_PAGO);
			}
			else{
				break;
				$msg = utf8_decode('Operação falhou!');
				direcionar('?modulo=principal/pad/'.$resultado[4].'&acao=A&id='.$resultado[3].'&ipaid='.$resultado[2],$msg);
			}
		}
		unset($_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina']);
		$msg = utf8_decode('Operação realizada com sucesso!');
		direcionar('?modulo=principal/pad/'.$resultado[4].'&acao=A&id='.$resultado[3],$msg);
	}
	else{
		$msg = utf8_decode('A Data Informada é Inválida!');
		alerta($msg);
		extract($_POST);
	}
	
	
}
//caso seja via url
else if(!empty($_GET['ipaid'])){
	
	//carrega os dados do item da pad
	$itemPad = $oItemPad->carrega_registro_por_id($_GET['ipaid']);
	
	//verifica se o item pode ser pago
	if($itemPad['ipastatus'] != ItemPad::ST_PAGO && $itemPad['ipastatus'] != ItemPad::ST_GLOSADO && $itemPad['ipastatus'] != ItemPad::ST_FATURADO){
		$oPagItemPad->bloquearEdicao();
	}
	
	//caso o item esteja PAGO, pega os dados do pagamento
	if($itemPad['ipastatus'] == ItemPad::ST_PAGO){
		$dadosItemPago = $oPagItemPad->carregar_registro_por_item($_GET['ipaid']);
		extract($dadosItemPago);
	}
	
	//caso não tenha carregado o valor na tabela de pagamento
	//carrega o valor que está na tabela de faturamento
	if(empty($pipvalorfaturado)){
		$oFatItemPad->carregaFaturamentoIdItem($_GET['ipaid']);
		$dadosItemFaturado = $oFatItemPad->getDados();
		$pipvalorfaturado = $dadosItemFaturado['fipvalortotal'];
	}
	
	//formata o valor
	$pipvalorfaturado = number_format($pipvalorfaturado, 2, ',', '.');
	
	//pega o id do registro pai
	$padid = $itemPad['padid'];
	$ipaid = $_GET['ipaid'];
	
	
}

$edicaoBloqueada = $oPagItemPad->checkBloqueioEdicao();

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';


$arMnuid = array();	
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Dados do Pagamento','');
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
	
		if($('pipdatapagamento').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Data do Pagamento" é obrigatório!'));
			alert(msg);
			$('pipdatapagamento').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($('pipnumordembancaria').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Nº da Ordem Bancária" é obrigatório!'));
			alert(msg);
			$('pipnumordembancaria').focus();
			$('btnSalvar').enable();
	        return false;
		}
		
		return true;
	}

	/**
	 * Volta à tela anterior
	 * @name voltar
	 * @return void
	 */	
	function voltar(){
		var padid = $('id').getValue();
		window.location = '?modulo=principal/pad/cadItemPad&acao=A&id='+padid;
	}


	Event.observe(window, 'load', function() {

		 //verifica se teve erro de data. 
		 //caso haja coloca o foco no campo correto
		 if($F('erroData') == 'anoinvalido'){
		     $('pipdatapagamento').focus();	
		 }
		 		
	 });

	
</script>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Data do Pagamento: </td>
			<td class="campo">
				<?php echo campo_data2('pipdatapagamento','S',($edicaoBloqueada?'N':'S'),'Informe a Data do Pagamento','N','','',$pipdatapagamento)?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&ordm; da Ordem Banc&aacute;ria: </td>
			<td class="campo">
				<?=campo_texto('pipnumordembancaria','S',($edicaoBloqueada?'N':'S'),'',20,20,'','','left','',0,'id="pipnumordembancaria" onkeypress="return somenteNumeros(event);"');?>
            </td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor Faturado: </td>
			<td class="campo">
				<?= campo_texto('pipvalorfaturado','N','N','',11,14,'[###.]###,##','','left','',0,'id="pipvalorfaturado"');?>
			</td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='ipaid' id='ipaid' value='<?php echo $ipaid; ?>'>
            	<input type='hidden' name='ipaids' id='ipaids' value='<?php echo $ipaids; ?>'>
            	<input type='hidden' name='id' id='id' value='<?php echo $padid; ?>'>
            	<input type='hidden' name='pipid' id='pipid' value='<?php echo $pipid; ?>'>
            	<input type='hidden' name='erroData' id='erroData' value='<?php echo $erroData; ?>'>
            	<?php if(!$edicaoBloqueada && $_SESSION['visualizar'] != 'S'){ ?>
            		<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
            	<?php } ?>
            	<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
</form>