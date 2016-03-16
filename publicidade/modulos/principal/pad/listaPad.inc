<?php

//limpando as sessões
unset($_SESSION['id'],$_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina'],$_SESSION['visualizar']);

// incluindo as classes necessárias
$oPad = new Pad();
$oFornecedor = new Fornecedor();
$oCampanha = new Campanha();

//caso seja para filtrar, verifica as datas
if($_POST['requisicao'] == 'filtrar'){
	$erroData = $oPad->verifica_data($_POST);
	
	if($erroData == 'datamenor'){
		$msg = utf8_decode('Data Inválida!');
		alerta($msg);
		$_POST['requisicao'] = '';
	}
	else if($erroData == 'anoinicialinvalido' || $erroData == 'anofinalinvalido'){
		$msg = utf8_decode('A Data Informada é Inválida!');
		alerta($msg);
		$_POST['requisicao'] = '';
	}
	$_POST['erroData'] = $erroData;
}
//caso seja para excluir
if($_GET['requisicao'] == 'excluir'){
	$oPad->inativar($_GET['id']);
	direcionar('?modulo=principal/pad/listaPad&acao=A','PAD cancelada com sucesso!');
}

// monta cabeçalho do sistema
include_once APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

$arMnuid = array(ABA_ITEM_PAD,ABA_FATURAMENTO_ITEM_PAD,ABA_GLOSA_ITEM_PAD,ABA_PAGAMENTO_ITEM_PAD);
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
monta_titulo('PAD','');

extract($_POST);
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>
<script language='javascript' type='text/javascript' src='../includes/JsLibrary/date/displaycalendar/displayCalendar.js'></script>
<script language='javascript' type='text/javascript'>
	/**
	 * Aciona o filtro
	 * @name pesquisar
	 * @param requisicao - Requisição que será executada
	 * @return void
	 */	
	function pesquisar(requisicao){
		//$('btnPesquisar').disable();
		
		if(validaFormularioPesquisa()){

			$('requisicao').setValue(requisicao);
			$('formularioPesquisa').submit();
		}
	}
	
	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioPesquisa
	 * @return bool
	 */	
	function validaFormularioPesquisa(){
		
		if($('padnumero').getValue() == '' && $('forid').getValue() == '' && $('fipnumfaturaagencia').getValue() == '' && $('pipnumordembancaria').getValue() == '' && $('padnumcontrato').getValue() == '' && $('padstatus').getValue() == '' && $('padano').getValue() == '' && $('camid').getValue() == '' && $('dtinicial').getValue() == '' && $('dtfinal').getValue() == '' && $('padvalornota').getValue() == '') {
			var msg = '� obrigat�rio o preenchimento de pelo menos um campo para a realiza��o da pesquisa!';
			alert(msg);
			//$('btnPesquisar').enable();
	        //return false;
		}
	
		return true;
	}
	
	/**
	 * Redireciona para a edição
	 * @name detalhar
	 * @param id - Id do registro
	 * @return void
	 */
	function detalhar(id){
		window.location = '?modulo=principal/pad/cadPad&acao=A&id='+id;
	}
	
		function filtraParaItens( id, nota, ob ){
		
		window.location = '?modulo=principal/pad/cadPad&acao=A&id='+id+'&redirectItensPad=true&nota='+nota+'&ob='+ob;
	}
	
	/**
	 * Redireciona para a exclusão
	 * @name remover
	 * @param id - Id do registro
	 * @return void
	 */
	function remover(id){
		 window.location = '?modulo=principal/pad/listaPad&acao=A&id='+id+'&requisicao=excluir';
	}

	/**
	 * Função responsável por direcionar à tela de inclusão
	 * @name adicionar
	 * @return void
	 */
	function adicionar(){
		window.location = '?modulo=principal/pad/cadPad&acao=A';
	}

	/**
	 * Redireciona para a visualização
	 * @name visualizar
	 * @param id - Id do registro
	 * @return void
	 */
	function visualizar(id){
		window.location = '?modulo=principal/pad/cadPad&acao=A&id='+id+'&visualizar=S';
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
			if($('padnumcontrato')){
				$('padnumcontrato').setValue(resposta.cttnumcontrato);
			}

			if($('cttid')){
				$('cttid').setValue(resposta.cttid);
			}
			
		}else{
			
			limparContrato();
			var msg = decodeURIComponent(escape('Contrato Não Encontrado!'));
			alert(msg);
				
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
	 * Função responsável por limpar os campos de contrato
	 * @name limparContrato
	 * @return void
	 */
	function limparContrato(){
		if($('padnumcontrato')){
			$('padnumcontrato').clear();
		}

		if($('cttid')){
			$('cttid').clear();
		}
	}

	Event.observe(window, 'load', function() {

		 //verifica se teve erro de data. 
		 //caso haja coloca o foco nos campos corretos
		 if($F('erroData') == 'datamenor'){
		     $('dtinicial').focus();	
		 }
		 else if($F('erroData') == 'anoinicialinvalido'){
			 $('dtinicial').focus();
		 }
		 else if($F('erroData') == 'anofinalinvalido'){
		     $('dtfinal').focus();
		 }
		 		
	 });

</script>
<form name="formularioPesquisa" id="formularioPesquisa" method="post">
	<table class="tabela" width="95%" align="center" border="0" cellpadding="5" cellspacing="1">
		<tr>
			<td class="SubtituloDireita"> N&uacute;mero da PAD: </td>
			<td class="campo">
				<?=campo_texto('padnumero','N','S','Informe o N&uacute;mero da PAD',10,8,'####/###','','left','',0,'id="padnumero"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Ag&ecirc;ncia: </td>
			<td class="campo"><?php $oFornecedor->monta_combo_fornecedor($forid,'N',1);?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Contrato: </td>
			<td class="campo">
				<?=campo_texto('padnumcontrato','N','S','Informe o Contrato',10,8,'###/####','','left','',0,'id="padnumcontrato"');?>
				<input type='hidden' name='cttid' id='cttid' value='<?php echo $cttid ?>'>
			</td>
		</tr>
				<tr>
			<td class="SubtituloDireita"> N� Nota: </td>
			<td class="campo">
				<?=campo_texto('fipnumfaturaagencia','N','S','Informe o Contrato',10,8,'##########','','left','',0,'id="fipnumfaturaagencia"');?>
			</td>
		</tr>
                
                <tr>
			<td class="SubtituloDireita"> Valor da Nota: </td>
			<td class="campo">
				<?=campo_texto('padvalornota','N','S','Informe o valor total da Nota',16,16,'#.###.###.###,##','','left','',0,'id="padvalornota"');?>
			</td>
		</tr>
                
                
                
                <!--
                
                <tr>
			<td class="SubtituloDireita"> Valor Total da PAD: </td>
			<td class="campo">
				<?=campo_texto('padvalor','N','S','Informe o valor total da PAD',16,16,'#.###.###.###,##','','left','',0,'id="padvalor"');?>
			</td>
		</tr>
                
                
                -->
              
                
                <tr>
			<td class="SubtituloDireita"> N� OB: </td>
			<td class="campo">
				<?=campo_texto('pipnumordembancaria','N','S','Informe o N�mero da Ordem Banc�ria',10,8,'##########','','left','',0,'id="pipnumordembancaria"');?>
			</td>
		</tr>
                
		<tr>
			<td class="SubtituloDireita"> Status: </td>
			<td class="campo"><?php $oPad->monta_combo_status();?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Ano: </td>
			<td class="campo"><select id="padano" style="width: auto" class="CampoEstilo" name="padano">
                                <option value="">Selecione...</option><
                                <option value="2015">2015</option>
                        </select></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Campanha: </td>
			<td class="campo"><?php	$oCampanha->monta_combo_campanha();?></td>
		</tr>
		<tr>
			<td class="SubtituloDireita"> Per&iacute;odo de Cadastro: </td>
			<td class="campo">
				<?php echo campo_data2('dtinicial','N','S','Informe a Data Inicial','N','','',$dtinicial);?> a 
				<?php echo campo_data2('dtfinal','N','S','Informe a Data Final','N','','',$dtfinal);?> 
            </td>
		</tr>
		<tr class="buttons">
        	<td colspan='2' align='center'>
        		<input type='hidden' name='requisicao' id='requisicao' />
        		<input type='hidden' name='erroData' id='erroData' value='<?php echo $erroData; ?>' />
            	<input type='button' class="botao" name='btnPesquisar' id='btnPesquisar' value='Consultar' onclick="javascript:pesquisar('filtrar');">
            	<input type='button' class="botao" name='btnAdicionar' id='btnAdicionar' value='Adicionar' onclick="javascript:adicionar();">
            </td>
        </tr>
	</table>
</form>
<?php
//verifica se é para disparar a consulta no banco por filtro ou buscar todos
if($_POST['requisicao'] == 'filtrar'){
	$oPad->listar_por_filtro($_POST);
}
else{
	$oPad->listar_por_filtro('');
}
?>
