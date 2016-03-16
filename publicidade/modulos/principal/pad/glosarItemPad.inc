<?php

//verifica se o acesso à tela foi via aba para tornar a tela readonly ou não
if(empty($_GET['ipaid']) && (!empty($_SESSION['ipaid']) || !empty($_SESSION['ipaids'])) && $_SESSION['ultimaPagina'] != 'glosar'){
	$readOnly = 'S';
}

//controlando sessão de página acessada
if($_SESSION['ultimaPagina'] != 'pagar' && $_SESSION['ultimaPagina'] != 'faturar'){
	$_SESSION['ultimaPagina'] = 'glosar';
}

// incluindo as classes necessárias
$oItemPad = new ItemPad();
$oGlosaItemPad = new GlosaItemPad();
$oFaturamentoItemPad = new FaturamentoItemPad();

if(!empty($_GET['ipaid'])){
	unset($_SESSION['ipaids']);
}


if(!empty($_SESSION['ipaids'])){
	$_GET['ipaid'] = $_SESSION['ipaids'].',';
}


//pega os ids caso o usuário tenha selecionado + de 1 item
$aux = explode(',',$_GET['ipaid'],-1);

$_GET['ipaid'] = join( ',', $aux );
$erro = '';
if(count($aux) > 1){
	foreach($aux as $key => $value){
		$oItemPad->carregarPorId($value);
		if(!$oItemPad->isStatusFaturado() && $_SESSION['ultimaPagina'] != 'pagar'){
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
	if(!$oItemPad->isStatusFaturado() && !$oItemPad->isStatusGlosado()){
		$erro = 'S';
	}
	$_GET['ipaid'] = $aux[0];	
}

//if($erro == 'S'){
//	$msg = utf8_decode('Não é permitido editar a glosa de mais de um item!');
//	direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_SESSION['id'],$msg);
//}



// mantendo dados em sessão para navegação entre abas
if(!empty($_GET['ipaid']) && empty($_SESSION['ipaid'])){
	$_SESSION['ipaid'] = $_GET['ipaid'];
}
else if(empty($_GET['ipaid']) && !empty($_SESSION['ipaid'])){
	$_GET['ipaid'] = $_SESSION['ipaid'];
}
else if(empty($_GET['ipaid']) && empty($_SESSION['ipaid'])){
	direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_SESSION['id']);
}


//inicio bloco - necessario para adicionar valores ao formulário ( incluindo ids )
$ipaid = $_GET['ipaid'];
$lote = $aux;
if ($ipaid && count($lote) == 1){

	$oItemPad->carregarPorId($ipaid); //erro esta aqui.
	
	if ($oItemPad->isStatusGlosado()){ // Se o Item da PAD já estiver glosado
		$oGlosaItemPad->carregaGlosaIdItem($oItemPad->ipaid);
	} elseif ($oItemPad->isStatusFaturado()) {  // Se o Item da PAD estiver faturado
		$oGlosaItemPad->ipaid = $oItemPad->ipaid;
		$oFaturamentoItemPad->carregaFaturamentoIdItem($oItemPad->ipaid);
	} else { // para os outros casos
		$oGlosaItemPad->carregaGlosaIdItem($oItemPad->ipaid);
		$oGlosaItemPad->bloquearEdicao();
	}
} elseif ($ipaid && count($lote) > 1){
	$aGlosaItemPad = array();
	foreach($lote as $key => $value){
		$oItemPad->carregarPorId($value); // carrega o objeto
		if ($oItemPad->isStatusGlosado()){ // Se o Item da PAD já estiver glosado, ele não permite glosa em lote, para permitir remover as próximas 2 linhas e descomentar o primeiro comentário abaixo
			$msg = utf8_decode('Os items selecionados não poderão ser glosados em lote, pois já existe uma glosa.');
			direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_SESSION['id'],$msg);
			//$oGlosaItemPad->carregaGlosaIdItem($oItemPad->ipaid);
		} elseif ($oItemPad->isStatusFaturado()) {  // Se o Item da PAD estiver faturado
			$oGlosaItemPad->ipaid = $oItemPad->ipaid;
			$oFaturamentoItemPad->carregaFaturamentoIdItem($oItemPad->ipaid);
		} else { // para os outros casos
			$oGlosaItemPad->carregaGlosaIdItem($oItemPad->ipaid);
			$oGlosaItemPad->bloquearEdicao();
		}
		
		$aGlosaItemPad[] = $oGlosaItemPad->getDados();
	}
	
}
//fim bloco

//caso tenha postado o formulário // glosa de 1 item
if($ipaid && $_POST['requisicao'] == 'salvar' && count($lote) == 1){

	$_POST['ipavaloritem'] = $oItemPad->ipavaloritem;

	//verifica se o item possui saldo para ser glosado
	$naoPossuiSaldo = $oGlosaItemPad->verifica_saldo_para_glosa($_POST);
	
	if($naoPossuiSaldo){
		$msg = utf8_decode('O item selecionado não poderá ser glosado, pois não possui saldo suficiente.');
		direcionar('?modulo=principal/pad/glosarItemPad&acao=A&ipaid='.$_POST['ipaid'],$msg);
	}
	else{
		$oGlosaItemPad->popularDadosObjeto();
		$oItemPad->ipastatus = ItemPad::ST_GLOSADO;
	
		//salva a glosa
		$resultadoGlosa = $oGlosaItemPad->salvar();
		
		//atualiza o status do item
		$oItemPad->atualizar_status($oItemPad->ipaid, ItemPad::ST_GLOSADO);
		
		if ($oGlosaItemPad->gipvalor == '0'){
			$oGlosaItemPad->gipvalor = '0,00';
		}

		//atualiza os dados do item
		$oItemPad->atualizar_valores_item($_POST['ipaid'],$oGlosaItemPad->gipvalor,1);
		
		//pega os dados do item. 
		$dadosItem = $oItemPad->carrega_registro_por_id($_POST['ipaid']);
		
		//atualiza o faturamento do item 
		$oFaturamentoItemPad->atualizar_valores_fatura($_POST['ipaid'],$oGlosaItemPad->gipvalor,1,$dadosItem['ipavalorservico'],$dadosItem['ipavalortotal']);
	
		
	
		unset($_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina']);
		if($resultadoGlosa){
			$msg = utf8_decode('Operação realizada com sucesso!');
			direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_SESSION['id'],$msg);
		} else {
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$oItemPad->padid,$msg);
		}
	}
	
	
}
// glosa em lote
else if($_POST['requisicao'] == 'salvar' && count($lote) > 1){

	//verifica se o item possui saldo para ser glosado
	$naoPossuiSaldo = $oGlosaItemPad->verifica_saldo_para_glosa($_POST);
	
	if($naoPossuiSaldo[0]){
		$msg = utf8_decode('Os itens selecionados não poderão ser glosados, pois o(s) item(ns) '.$naoPossuiSaldo[1].' não possui(em) saldo suficiente.');
		direcionar('?modulo=principal/pad/glosarItemPad&acao=A&ipaid='.$_POST['ipaids'].',',$msg);
	}
	else{
		
		$aux_ipaid = explode(',',$_POST['ipaids']);
		$aux_gipid = explode(',',$_POST['gipid']);
		$aux_count = 0;//contador

		while($aux_count < count($aux_ipaid)){
			$popObj = Array ( 
						'gipid' => $aux_gipid[$aux_count],
						'ipaid' => $aux_ipaid[$aux_count],
						'gipdata' => $_POST['gipdata'], 
						'gipvalor' => $_POST['gipvalor'],
						'gipmotivo' => $_POST['gipmotivo'], 
						'gipstatus' => $_POST['gipstatus']
					);
			
			$oGlosaItemPad->popularDadosObjeto($popObj);
			$oGlosarItemPad->gipid = $aux_gipid[$aux_count];
			$oGlosaItemPad->ipaid = $aux_ipaid[$aux_count];
			$oItemPad->ipaid = $aux_ipaid[$aux_count];
			

			$oGlosaItemPad->gipvalor = str_replace('.', '', $oGlosaItemPad->gipvalor);
				
			$divide = new Math($oGlosaItemPad->gipvalor, count($aux_ipaid),2);
			$oGlosaItemPad->gipvalor = $divide->div()->getResult();
			
			$oGlosaItemPad->gipvalor = str_replace('.', ',',$oGlosaItemPad->gipvalor);
			
			//salva a glosa
			$resultadoGlosa = $oGlosaItemPad->salvar();
				
			//atualiza o status do item
			$oItemPad->atualizar_status($aux_ipaid[$aux_count], ItemPad::ST_GLOSADO);
				
			//atualiza os dados do item
			$oItemPad->atualizar_valores_item($aux_ipaid[$aux_count],$oGlosaItemPad->gipvalor,1);
				
			//pega os dados do item.
			$dadosItem = $oItemPad->carrega_registro_por_id($aux_ipaid[$aux_count]);

			//atualiza o faturamento do item
			$oFaturamentoItemPad->atualizar_valores_fatura($aux_ipaid[$aux_count],$oGlosaItemPad->gipvalor,count($aux_ipaid),$dadosItem['ipavalorservico'],$dadosItem['ipavalortotal']);
		
			$aux_count++;		
		}
		unset($_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina']);
		if($resultadoGlosa){
			$msg = utf8_decode('Operação realizada com sucesso!');
			direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_SESSION['id'],$msg);
		}
		else{
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$oItemPad->padid,$msg);
		}
	}
	
	
}

// popula formulario se for glosa única
if(count($lote) == 1){
	extract($oGlosaItemPad->getDados());
}
elseif (count($lote) > 1){ //popula formulario se for glosa em lote
	$gipid=''; $ipaid='';$gipdata='';$gipvalor='';$gipmotivo='';$gipstatus='';
	
	foreach ($aGlosaItemPad as $value){
		$gipid     .= $value['gipid'].',';
		$ipaid     .= $value['ipaid'].',';
		$gipdata   .= $value['gipdata'].',';
		$gipvalor  .= $value['gipvalor'].',';
		$gipmotivo .= $value['gipmotivo'].',';
		$gipstatus .= $value['gipstatus'].',';
	}
	//removo o ultimo caractere - virgula
	$gipid     = substr($gipid, 0, -1);
	$ipaids     = substr($ipaid, 0, -1);
	$gipdata   = '';//substr($gipdata, 0, -1);
	$gipvalor  = '';//substr($gipvalor, 0, -1);
	$gipmotivo = '';//substr($gipmotivo, 0, -1);
	$gipstatus = substr($gipstatus, 0, -1);
	$ipaid = '';//limpa valores de ipaid

}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

//verificações para exibir as abas corretas
if($oItemPad->isStatusFaturado()){
	if($_SESSION['ultimaPagina'] == 'pagar'){
		$arMnuid = array();
	}
	else{
		$arMnuid = array(ABA_PAGAMENTO_ITEM_PAD);
	}	
}
else if($oItemPad->isStatusGlosado()){
	if($_SESSION['ultimaPagina'] == 'glosar'){
		$arMnuid = array(ABA_PAGAMENTO_ITEM_PAD);
	}
	else if($_SESSION['ultimaPagina'] == 'pagar'){
		$arMnuid = array();
	}
}
else{
	$arMnuid = array();
}

$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// seta o padid para o botão voltar

if($oItemPad->padid)
{
	$_SESSION['padidbtvoltar'] = $oItemPad->padid;
}

// monta o título da tela
monta_titulo('Glosar Item da PAD','');
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
		//window.location = '?modulo=principal/pad/cadItemPad&acao=A&id=<?=$oItemPad->padid?>';
		window.location = '?modulo=principal/pad/cadItemPad&acao=A&id=<?=$_SESSION['padidbtvoltar']?>';
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
	 * Valida data da Glosa
	 *
	 * @name validaDataGlosa
	 * @return bool
	 */	
	function validaDataGlosa(dataGlosa){
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
	
		if($F('gipdata') == '') {
			var msg = decodeURIComponent(escape('O campo "Data da Glosa" é obrigatório!'));
			alert(msg);
			$('gipdata').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if (!validaDataGlosa($F('gipdata'))){
			var msg = decodeURIComponent(escape('A data informada é inválida'));
			alert(msg);
			$('gipdata').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($F('gipvalor') == '') {
			var msg = decodeURIComponent(escape('O campo "Valor da Glosa" é obrigatório!'));
			alert(msg);
			$('gipvalor').focus();
			$('btnSalvar').enable();
	        return false;
		}

		if($F('gipmotivo') == '') {
			var msg = decodeURIComponent(escape('O campo "Motivo da Glosa" é obrigatório!'));
			alert(msg);
			$('gipmotivo').focus();
			$('btnSalvar').enable();
	        return false;
		}

		return true;
	}
</script>

<script type="text/javascript">
	document.observe("dom:loaded", function(){
		// Executa o evento onkeyup dos campos para formação de máscara dos mesmos.
		$$('#gipvalor').invoke('onkeyup');		
	});
</script>

<?php $edicaoBloqueada = $oGlosaItemPad->checkBloqueioEdicao(); ?>

<form name=formularioCadastro id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> Data da Glosa: </td>
			<td class="campo">
				<?php 
				if(!empty($gipdata)){
					$gipdata = formata_data($gipdata);
				}
				
				if($edicaoBloqueada == 'S'){
					echo campo_texto('gipdata','S','N','',10,10,'','','left','',0,'id="gipdata"');
				}
				else{
					echo campo_data2('gipdata','S','S','Informe a Data da Fatura','N','','', $gipdata);
				}
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Valor da Glosa: </td>
			<td class="campo">
				<?= campo_texto('gipvalor','S',($edicaoBloqueada?'N':'S'),'Informe o Valor da Glosa',14,14,'[###.]###,##','','right','',0,'id="gipvalor"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Motivo da Glosa: </td>
			<td class="campo">
				<?= campo_textarea('gipmotivo','S', ($edicaoBloqueada?'N':'S'), '', 54, 6, 4000,'','','','','',''); ?>
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
            	<input type='hidden' name='requisicao' id='requisicao' value='salvar'>
            	<input type='hidden' name='gipid' id='gipid' value='<?php echo $gipid; ?>'>
            	<input type='hidden' name='ipaid' id='ipaid' value='<?php echo $ipaid; ?>'>
            	<input type='hidden' name='ipaids' id='ipaids' value='<?php echo $ipaids; ?>'>
            	<?php if(!$edicaoBloqueada && $_SESSION['visualizar'] != 'S'){?>
              		<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();">
              	<?php } ?>
				<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnVoltar' id='btnVoltar' value='Voltar' onclick="javascript:voltar();">
            </td>
        </tr>
	</table>
</form>