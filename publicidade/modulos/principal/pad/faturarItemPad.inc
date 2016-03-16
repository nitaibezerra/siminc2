<?php

//verifica se o acesso à tela foi via aba para tornar a tela readonly ou não
if(empty($_GET['ipaid']) && (!empty($_SESSION['ipaid']) || !empty($_SESSION['ipaids'])) && $_SESSION['ultimaPagina'] != 'faturar'){
	$readOnly = 'S';
}

//controlando sessão de página acessada
if($_SESSION['ultimaPagina'] != 'glosar' && $_SESSION['ultimaPagina'] != 'pagar'){
	$_SESSION['ultimaPagina'] = 'faturar';
}

// incluindo as classes necessárias
$oItemPad = new ItemPad();
$oFaturamentoItemPad = new FaturamentoItemPad();

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
		if(!$oItemPad->isStatusAprovado() && $_SESSION['ultimaPagina'] != 'glosar' && $_SESSION['ultimaPagina'] != 'pagar'){
			$erro = 'S';
		}	
	}
	if($erro != 'S'){
		$ipaids = implode(',',$aux);
		$_GET['ipaid'] = $aux[0];
		$valores = $oItemPad->pega_somatorio_valores($ipaids);
		$_SESSION['ipaids'] = $ipaids;
	}
}
else if(count($aux) == 1){
	$oItemPad->carregarPorId($aux[0]);
	if(!$oItemPad->isStatusAprovado() && !$oItemPad->isStatusFaturado()){
		$erro = 'S';
	}
	$_GET['ipaid'] = $aux[0];	
}

if($erro == 'S'){
	$msg = utf8_decode('Não é permitido editar a fatura de mais de um item!');
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


$ipaid = $_GET['ipaid'];
if ($ipaid){
	$oItemPad->carregarPorId($ipaid);
	
	$padid = $oItemPad->padid;
	
	// Se o Item da PAD já estiver faturado ou pago(pago sendo permitido a partir de 06/03/12)
	if ($oItemPad->isStatusFaturado() || $oItemPad->isStatusPago()){ 
		$oFaturamentoItemPad->carregaFaturamentoIdItem($oItemPad->ipaid);
	} elseif ($oItemPad->isStatusAprovado()) {  // Se o Item da PAD estiver Aprovado
		$oFaturamentoItemPad->ipaid = $oItemPad->ipaid;
		$oFaturamentoItemPad->fipvalorservico = $oItemPad->ipavalorservico;
		$oFaturamentoItemPad->fipvalortotal = $oItemPad->ipavalortotal;
	} else { // para os outros casos
		$oFaturamentoItemPad->carregaFaturamentoIdItem($oItemPad->ipaid);
		$oFaturamentoItemPad->bloquearEdicao();
	}
} else {
	$oFaturamentoItemPad->bloquearEdicao();
}

//caso tenha postado o formulário
if($ipaid && $_POST['requisicao'] == 'salvar' && empty($_POST['ipaids'])){
	
	$resultado = $oFaturamentoItemPad->salvar($_POST);

	if($resultado[0]){
		
		//faz este teste pois agora é possível alterar os dados da fatura
		//de um item já pago, porém o status dele neste caso permanece como pago(P)
		if(!$oItemPad->isStatusPago()){
			$oItemPad->atualizar_status($resultado[2], ItemPad::ST_FATURADO);
		}
		
		unset($_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina']);
		
		$msg = utf8_decode('Operação realizada com sucesso!');
		direcionar('?modulo=principal/pad/'.$resultado[4].'&acao=A&id='.$resultado[3],$msg);
	}
	else if(!$resultado[0]){
		$msg = utf8_decode('Operação falhou!');
		direcionar('?modulo=principal/pad/'.$resultado[4].'&acao=A&id='.$resultado[3].'&ipaid='.$resultado[2],$msg);
	}
}
else if($_POST['requisicao'] == 'salvar' && !empty($_POST['ipaids'])){
	$aux = explode(',',$_POST['ipaids']);
	foreach($aux as $key => $value){
		
		//pega os valores dos itens
		//apesar de faturar em lote, os valores da fatura
		//são individuais e não a soma de todos os itens
		$dadosItem = $oItemPad->carrega_registro_por_id($value);

		$_POST['fipvalortotal'] = $dadosItem['ipavalortotal'];
		$_POST['fipvalorservico'] = $dadosItem['ipavalorservico'];
		
		$_POST['ipaid'] = $value;
		$resultado = $oFaturamentoItemPad->salvar($_POST);
		
		if($resultado[0]){
			$oItemPad->atualizar_status($resultado[2], ItemPad::ST_FATURADO);
		}
	}
	unset($_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina']);
	$msg = utf8_decode('Operação realizada com sucesso!');
	direcionar('?modulo=principal/pad/'.$resultado[4].'&acao=A&id='.$resultado[3],$msg);
}

extract($oFaturamentoItemPad->getDados());

//verifica se os valores estão carregados
if(!empty($valores['ipavalorservico'])){
	$fipvalorservico = $valores['ipavalorservico'];
}
if(!empty($valores['ipavalortotal'])){
	$fipvalortotal = $valores['ipavalortotal'];
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

//verificações para exibir as abas corretas
if($oItemPad->isStatusFaturado() || $oItemPad->isStatusGlosado() || $oItemPad->isStatusPago()){
	if($_SESSION['ultimaPagina'] == 'glosar'){
		$arMnuid = array(ABA_PAGAMENTO_ITEM_PAD);
	}
	else if($_SESSION['ultimaPagina'] == 'pagar'){
		$arMnuid = array();
	}
	else{
		$arMnuid = array(ABA_GLOSA_ITEM_PAD,ABA_PAGAMENTO_ITEM_PAD);
	}
}
else if($oItemPad->isStatusAprovado()){
	$arMnuid = array(ABA_GLOSA_ITEM_PAD,ABA_PAGAMENTO_ITEM_PAD);
}


$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('Faturar Item da PAD','');
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript'>
	/**
	 * Redireciona para a pesquisa
	 * @name voltar
	 * @return void
	 */
	function voltar(){
		window.location = '?modulo=principal/pad/cadItemPad&acao=A&id=<?=$oItemPad->padid?>';
	}
	
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
	 * Valida data da Fatura
	 *
	 * @name validaDataFatura
	 * @return bool
	 */	
	function validaDataFatura(dataGlosa){
		var data = new Data(); 
		var retorno = data.comparaData(dataGlosa,'01/01/2000','>=');
		return retorno;
	}
	
	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioCadastro
	 * @return bool
	 */	
	function validaFormularioCadastro(){
	
		if($F('fipdataftura') == '') {
			var msg = decodeURIComponent(escape('O campo "Data da Fatura" é obrigatório!'));
			alert(msg);
			$('fipdataftura').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if (!validaDataFatura($F('fipdataftura'))){
			var msg = decodeURIComponent(escape('A data informada é inválida'));
			alert(msg);
			$('fipdataftura').focus();
			$('btnSalvar').enable();
	        return false;
		}

		/*
		if($F('fipnumsidoc') == '') {
			var msg = decodeURIComponent(escape('O campo "Nº Sidoc" é obrigatório!'));
			alert(msg);
			$('fipnumsidoc').focus();
			$('btnSalvar').enable();
	        return false;
		}
		*/
		/*if($F('fipptres') == '') {
			var msg = decodeURIComponent(escape('O campo "Programa de Trabalho - PTRES" é obrigatório!'));
			alert(msg);
			$('fipptres').focus();
			$('btnSalvar').enable();
	        return false;
		}
		*/
		if($F('fipnumfaturaagencia') == '') {
			var msg = decodeURIComponent(escape('O campo "Nº da Fatura na Agência" é obrigatório!'));
			alert(msg);
			$('fipnumfaturaagencia').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($F('fipnumfaturafornecedor') == '') {
			var msg = decodeURIComponent(escape('O campo "Nº da Fatura do Fornecedor" é obrigatório!'));
			alert(msg);
			$('fipnumfaturafornecedor').focus();
			$('btnSalvar').enable();
	        return false;
		}
		
		return true;
	}
</script>

<script type="text/javascript">
	document.observe("dom:loaded", function(){
		// Executa o evento onkeyup dos campos para formação de máscara dos mesmos.
		$$('#fipnumsidoc','#fipptres').invoke('onkeyup');		
	});
</script>

<?php $edicaoBloqueada = $oFaturamentoItemPad->checkBloqueioEdicao(); ?>

<form name=formularioCadastro id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Data da Fatura: </td>
			<td class="campo">
				<?php 
				if(!empty($fipdataftura)){
					$fipdataftura = formata_data($fipdataftura);
				}
				
				if($edicaoBloqueada == 'S'){
					echo campo_texto('fipdataftura','S','N','',10,10,'','','left','',0,'id="fipdataftura"');
				}
				else{
					echo campo_data2('fipdataftura','S','S','Informe a Data da Fatura','N','','', $fipdataftura);
				}
				?>
			</td>
		</tr>
		<!-- Retirando campo conforme solicitado em 05/11/2013
		<tr>
			<td class="SubtituloDireita"> N&ordm; Sidoc: </td>
			<td class="campo">
				<?php //campo_texto('fipnumsidoc','',($edicaoBloqueada?'N':'S'),'Informe o N&ordm; Sidoc',20,20,'#####.######/####-##','','left','',0,'id="fipnumsidoc"');?>
			</td>
		</tr>-->
		<!-- Retirando campo conforme solicitado em 11/11/2013
		<tr>
			<td class="SubtituloDireita"> Programa de Trabalho - PT: </td>
			<td class="campo">
				<?= campo_texto('fipptres','',($edicaoBloqueada?'N':'S'),'Informe o Programa de Trabalho - PTRES',25,21,'##.###.####.####.####','','left','',0,'id="fipptres"');?>
			</td>
		</tr>
		-->
		<tr>
			<td class="SubtituloDireita"> N&ordm; da Fatura da Ag&ecirc;ncia : </td>
			<td class="campo">
				<?= campo_texto('fipnumfaturaagencia','S',($edicaoBloqueada?'N':'S'),'Informe o N&ordm; da Fatura na Ag&ecirc;ncia ',22,20,'####################','','left','',0,'id="fipnumfaturaagencia"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor Total : </td>
			<td class="campo">
				<?php 
				if(!empty($fipvalortotal)){
					$fipvalortotal = number_format($fipvalortotal,2,',','.');
				}
				echo campo_texto('fipvalortotal','S','N','Valor Total ',11,14,'[###.]###,##','','right','',0,'id="fipvalortotal"');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> N&ordm; da Fatura do Fornecedor : </td>
			<td class="campo">
				<?= campo_texto('fipnumfaturafornecedor','S',($edicaoBloqueada?'N':'S'),'Informe o N&ordm; da Fatura do Fornecedor ',22,20,'###################','','left','',0,'id="fipnumfaturafornecedor"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor do Servi&ccedil;o : </td>
			<td class="campo">
				<?php 
				if(!empty($fipvalorservico)){
					$fipvalorservico = number_format($fipvalorservico,2,',','.');
				}
				echo campo_texto('fipvalorservico','S','N','Valor do Servi&ccedil;o ',11,14,'[###.]###,##','','right','',0,'id="fipvalorservico"');
				?>
			</td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='fipid' id='fipid' value='<?php echo $fipid; ?>'>
            	<input type='hidden' name='ipaid' id='ipaid' value='<?php echo $ipaid; ?>'>
            	<input type='hidden' name='ipaids' id='ipaids' value='<?php echo $ipaids; ?>'>
            	<input type='hidden' name='id' id='id' value='<?php echo $padid; ?>'>
            	<?php if(!$edicaoBloqueada && $_SESSION['visualizar'] != 'S'){?>
              		<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
              	<?php } ?>
				<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
</form>