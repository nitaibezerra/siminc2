<?php

//verifica se o acesso à tela foi via aba para tornar a tela readonly ou não
if(empty($_GET['id']) && (!empty($_SESSION['ipaid']) || !empty($_SESSION['ipaids']))){
	$readOnly = 'S';
}

$editarItem = false;
//qdo o acesso à tela for feito via botões e não via aba, limpa as sessões
if(!empty($_GET['id'])){
	unset($_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina']);

}  
//
if(!empty($_GET['ipaid'])){
	unset($_SESSION['ipaid'],$_SESSION['ipaids'],$_SESSION['ultimaPagina']);
	$editarItem = true;	
}

//mantendo dados em sessão para navegação entre abas
if(!empty($_GET['id']) && empty($_SESSION['id'])){
	$_SESSION['id'] = $_GET['id'];
}
else if(empty($_GET['id']) && !empty($_SESSION['id'])){ 
	$_GET['id'] = $_SESSION['id'];
}
else if(empty($_GET['id']) && empty($_SESSION['id'])){
	direcionar('?modulo=principal/pad/listaPad&acao=A');
}

if( $_POST['requisicao'] == 'ajax_check')
{

	// Item que est� requisitando
	$itemCheck = $_POST['itenpad'];
	
	// Caso tenha pesquisado outra pad, � necess�rio esvaziar os arrays das marcadas e modificar o valor da pad atual
	if( $_SESSION['PAD_ATUAL_LIST'] != $_POST['pad_atual'] )
	{
		// Atualiza a pad atual
		$_SESSION['PAD_ATUAL_LIST'] = $_POST['pad_atual'];
		// retira os itens da outra pad
		unset($_SESSION['ITENS_MARCADOS_LIST']);
	}
	
	//caso seja ele incrementa array de marcados 
	if( $_POST['check'] ) 
	{
		$_SESSION['ITENS_MARCADOS_LIST'][$itemCheck] = $itemCheck;
	}
	else
	{
		unset($_SESSION['ITENS_MARCADOS_LIST'][$itemCheck]);
	}
	
	$_SESSION['PAD_ATUAL_LIST'] = $_POST['pad_atual'];
	die();
}

// incluindo as classes necessárias
$oPad = new Pad();
$oItemPad = new ItemPad();
$oFornecedor = new Fornecedor();
$oTipoServico = new TipoServico();
$oTipoPeca = new TipoPeca();
$oContratoHonorario = new ContratoHonorario();

//caso tenha postado o formulário
if($_POST['requisicao'] == 'incluir'){
     
	$erro = $oItemPad->verifica_saldo_disponivel($_POST);

	if(!$erro){
		//salva
		$resultado = $oItemPad->salvar($_POST);

		if($resultado[0]){
			$msg = utf8_decode('Operação realizada com sucesso!');
			direcionar('?modulo=principal/pad/'.$resultado[3].'&acao=A&id='.$resultado[1],$msg);
		}
		else if(!$resultado[0]){
			$msg = utf8_decode('Operação falhou!');
			direcionar('?modulo=principal/pad/'.$resultado[3].'&acao=A&id='.$resultado[1],$msg);
		}
	}
	else if($erro){
		$msg = utf8_decode('O Item da PAD não pode ser incluído. Saldo da PAD insuficiente.');
		direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_POST['id'],$msg);
	}
}
else if($_POST['requisicao'] == 'salvar'){
	
	if(!empty($_POST['ipadsc']))
	{
		//$erro = $oItemPad->verifica_saldo_disponivel($_POST);
                $erro = null;
		if(!$erro){
			//salva
			$resultado = $oItemPad->salvar($_POST);

			if($resultado[0]){
				$msg = utf8_decode('Operação realizada com sucesso!');
				direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_POST['id'],$msg);
			}
			else if(!$resultado[0]){
				$msg = utf8_decode('Operação falhou!');
				direcionar('?modulo=principal/pad/'.$resultado[3].'&acao=A&id='.$resultado[1],$msg);
			}
		}
		else if($erro){
			$msg = utf8_decode('O Item da PAD não pode ser incluído. Saldo da PAD insuficiente.');
			direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_POST['id'],$msg);
		}
	}
	else if(empty($_POST['ipadsc']) && $_POST['qtdItemPad'] >= 1){
		$msg = utf8_decode('Operação realizada com sucesso!');
		direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_POST['id'],$msg);
	}
	else if(empty($_POST['ipadsc']) && $_POST['qtdItemPad'] < 1){
		$msg = utf8_decode('Necessário adicionar pelo menos um item.');
		direcionar('?modulo=principal/pad/cadItemPad&acao=A&id='.$_POST['id'],$msg);
	}

}
//caso seja via url
else if(!empty($_GET['ipaid'])){
	$itempad = $oItemPad->carrega_registro_por_id($_GET['ipaid']);
	extract($itempad);
}


$isVisualizarItem = false;
// Trata a requisi��o para ver os detalhes um item
if($_POST['requisicaoitem'] == 'visualizaritem' )
{
	$urlAtual = $_SERVER['REQUEST_URI'];
	$numeroItem = $_POST['numeroitem'];
	$isVisualizarItem = true;
	
	$sqlDadosIten = "SELECT 
			
	      CASE WHEN item.ipastatus = 'A' THEN 'Aprovado' 
			      ELSE 
				  CASE WHEN item.ipastatus = 'C' THEN 'Cancelado'
				      ELSE
					  CASE WHEN item.ipastatus = 'F' THEN 'Faturado'
					      ELSE
						  CASE WHEN item.ipastatus = 'P' THEN 'Pago'
						      ELSE
							  CASE WHEN item.ipastatus = 'G' THEN 'Glosado'
							  END
					      END
					  END
			      END	
	      END AS situacao, -- situacao
	      item.ipanumitempad, -- n�mero
	      item.ipadsc, -- descricao
	      fornec.fornome, -- fornecedor
	      tipo.tsedsc, -- tipo servico
	      cat.catdsc, -- categoria
	      tpp.tppdsc,-- tipo peca publicitaria
	      item.ipaqtd, -- quantidade
		  fornec.forid as forid, -- id fornecedor
	      tipo.tseid as tseid,
		  tpp.tppid as tppid,
		  cat.catid as catid,
		  
	      item.ipavalorservico,
	      item.ipadesconto,
	      item.ipahonorario,
	      item.ipavalorhonorario,
	      item.ipavaloritem,
	      item.ipavalortotal
	      
	  FROM publicidade.itenspad item
	  JOIN publicidade.pad pad ON item.padid=pad.padid
	  JOIN publicidade.fornecedor fornec ON item.forid=fornec.forid
	  JOIN publicidade.tiposervico tipo ON item.tseid=tipo.tseid
	  JOIN publicidade.categoria cat ON item.catid = cat.catid
	  JOIN publicidade.tipopeca tpp ON item.tppid = tpp.tppid
	  LEFT JOIN publicidade.faturamentoitempad fatura ON item.ipaid=fatura.ipaid and fatura.fipstatus = 'A'
	  LEFT JOIN publicidade.glosaitempad glosa ON item.ipaid=glosa.ipaid
	  LEFT JOIN publicidade.pagamentoitempad pag ON item.ipaid=pag.ipaid and pag.pipstatus = 'A'
	   WHERE item.ipanumitempad = '{$numeroItem}'
	  ORDER BY item.ipanumitempad ";
	
	$itemVisu = $db->carregar($sqlDadosIten);
	$itemVisu = $itemVisu[0];
	
}

if ($_REQUEST['status'] == 'cancelarPagamento') {
    $sqlPagamento = "UPDATE publicidade.pagamentoitempad
   SET pipstatus='I'
 WHERE ipaid = '{$_REQUEST['ipaid']}'";
    $db->executar($sqlPagamento);
    $sqlCancelarPagamento = "UPDATE  publicidade.itenspad 
   SET ipastatus='F'
 WHERE ipaid = '{$_REQUEST['ipaid']}'";
    $db->executar($sqlCancelarPagamento);
    if ($db->commit()) {
        echo "<script>alert('Pagamento cancelado com sucesso!')</script>";
    } else {
        echo "<script>alert('N�o foi poss�vel cancelar o pagamento.')</script>";
    }
}

if ($_REQUEST['status'] == 'cancelarFaturamento') {
    $sqlPagamento = "UPDATE publicidade.faturamentoitempad
   SET fipstatus='I'
 WHERE ipaid = '{$_REQUEST['ipaid']}'";
    $db->executar($sqlPagamento);
    $sqlCancelarPagamento = "UPDATE  publicidade.itenspad 
   SET ipastatus='A'
 WHERE ipaid = '{$_REQUEST['ipaid']}'";
    $db->executar($sqlCancelarPagamento);
    if ($db->commit()) {
        echo "<script>alert('Faturamento cancelado com sucesso!')</script>";
    } else {
        echo "<script>alert('N�o foi poss�vel cancelar o faturamento.')</script>";
    }
}


//pega os dados via get e carrega os dados necessários do registro pai
$pad = $oPad->carrega_registro_por_id($_GET['id']);
$padid = $_GET['id'];
$ipaid = $_GET['ipaid'];
$agencia = $pad['forid'];
$cttid = $pad['cttid'];
$padnumero = $pad['padnumero'];
$padvalor = number_format($pad['padvalor'], 2, ',', '.');
$padstatus = $pad['padstatus'];

//recupera o saldo disponível da PAD
$saldo = $oItemPad->pegar_saldo_pad($_GET['id'],$padvalor);

//recupera o último número de item da pad em questão
if(empty($ipanumitempad)){
	$ipanumitempad = $oItemPad->pega_ultimo_numero($padid);
	if(empty($ipanumitempad)){
		$ipanumitempad = $padnumero.'.'.'1';
	}
	else{
		$pos = strrpos($ipanumitempad,'.');
		$sequencial = substr($ipanumitempad,$pos+1)+1;
		$ipanumitempad = $padnumero.'.'.$sequencial;
	}
}

//pega a qtd de itens da pad
$qtdItemPad = $oItemPad->pega_qtd_itens($padid);

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
	$arMnuid = array(ABA_FATURAMENTO_ITEM_PAD,ABA_GLOSA_ITEM_PAD,ABA_PAGAMENTO_ITEM_PAD);
}
$db->cria_aba($abacod_tela, $url, '', $arMnuid);

// monta o título da tela
if($_SESSION['visualizar'] != 'S'){
	monta_titulo('Dados do Item da PAD','');
}
?>

<link type="text/css" rel="stylesheet" href="./css/default.css" />

<script language='javascript' type='text/javascript' src='../includes/prototype.js'></script>
<script language='javascript' type='text/javascript' src='./js/default.js'></script>
<script language='javascript' type='text/javascript' src='./js/ajax.js'></script>

<script language='javascript' type='text/javascript'>

/**
 * Habilita/desabilita o botão imprimir ao selecionar/deselecionar os itens da pad
 * @name habilitaImpressao
 * @return void
 */
function habilitaImpressao( value, check, npad ){
	
	// Caso tenha sido marcado
	if(check)
	{
		ischeck = 1;
	}
	else
	{
		ischeck = 0;
	}
	// Submete via ajax para poder registrar os que foram marcados qundo houver a troca de p�ginas
	new Ajax.Updater('check', "publicidade.php?modulo=principal/pad/cadItemPad&acao=A", {
		method : 'post',
		asynchronous: false,
		parameters: { 'requisicao':'ajax_check', 'itenpad':value, 'check' :ischeck, 'pad_atual':npad },
	});
	
	var item = '';
	item = $$('input[name=checkitempad]').filter(function(el){
		return $F(el);
	}).size();

}


<?php if(! $isVisualizarItem) {?>
Event.observe(window, 'load', function() {
	
	$('ipadesconto').className = 'disabledCorFonteDiferente';
	$('ipavalorhonorario').className = 'disabledCorFonteDiferente';
	$('ipavalortotal').className = 'disabledCorFonteDiferente';
	$('saldo').className = 'disabledCorFonteDiferente';
});
<?php }?>

	/**
	 * Submete o formulário
	 * @name salvar
	 * @return void
	 */	
	function salvar()
	{
		$('btnSalvar').disable();
		if($F('qtdItemPad') < 1){
			if(validaFormularioCadastro()){
				$('formularioCadastro').submit();
			}
		}
		else if($F('qtdItemPad') >= 1){
			$('formularioCadastro').submit();
		}
	}
	
	/**
	 * Valida os campos obrigatórios
	 * @name validaFormularioCadastro
	 * @return bool
	 */	
	function validaFormularioCadastro(){
	
		if($('ipadsc').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Descrição" é obrigatório!'));
			alert(msg);
			$('ipadsc').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('forid').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Fornecedor" é obrigatório!'));
			alert(msg);
			$('forid').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('tseid').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Tipo de Serviço" é obrigatório!'));
			alert(msg);
			$('tseid').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('catid').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Categoria" é obrigatório!'));
			alert(msg);
			$('catid').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('tppid').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Tipo de Peça Publicitária" é obrigatório!'));
			alert(msg);
			$('tppid').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('ipaqtd').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Quantidade" é obrigatório!'));
			alert(msg);
			$('ipaqtd').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('ipavalorservico').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Valor do Serviço" é obrigatório!'));
			alert(msg);
			$('ipavalorservico').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('ipadesconto').getValue() == ''){
			var msg = decodeURIComponent(escape('O campo "Desconto Contratual" é obrigatório!'));
			alert(msg);
			$('ipadesconto').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		
		var ipahonorario = '';
		ipahonorario = $$('input[name=ipahonorario]').filter(function(el){
		    return $F(el);
		}).size();
		
		if(ipahonorario < 1){
			var msg = decodeURIComponent(escape('O campo "Honorário" é obrigatório!'));
			alert(msg);
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}
		
		

		if($('ipavalorhonorario').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Valor do Honorário" é obrigatório!'));
			alert(msg);
			$('ipavalorhonorario').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('ipavaloritem').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Valor do Item" é obrigatório!'));
			alert(msg);
			$('ipavaloritem').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
	        return false;
		}

		if($('ipavalortotal').getValue() == '') {
			var msg = decodeURIComponent(escape('O campo "Valor Total" é obrigatório!'));
			alert(msg);
			$('ipavalortotal').focus();
			$('btnSalvar').enable();
			$('btnIncluir').enable();
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
		window.location = '?modulo=principal/pad/cadPad&acao=A&id='+padid;
	}

	/**
	 * Grava registro filho
	 * @name incluir
	 * @return void
	 */	
	function incluir(){
		$('btnIncluir').disable();
		$('btnSalvar').disable();
		if(validaFormularioCadastro()){
			$('requisicao').setValue('incluir');
			$('formularioCadastro').submit();
		}
	}
	
	function visualizar( itemNumero ){

		$('requisicaoitem').setValue('visualizaritem');
		$('numeroitem').setValue(itemNumero);
		$('visualizaritem').submit();
		
	}

	function recarrega(urlAtual)
	{
		document.location.href = urlAtual;
	}
	
	/**
	 * Cacula os valores
	 * @name calcularValores
	 * @return void
	 */	
	function calcularValores(){

		//pega a descrição do tipo de serviço selecionado
		var tsedsc = $F('tsedsc');

		//instancia o objeto para cálculos
		var obCal = new Calculo();

		//pega a % de honorários
		var radioGroup = $('ipahonorario').name;
        var el = $('ipahonorario').form;
        var ipahonorario = '';
        var checked = $(el).getInputs('radio', radioGroup).find(
            function(re) {return re.checked;}
        );
        if(checked){
			var ipahonorario = $F(checked);
        }

		//caso seja produção
		if(tsedsc.toLowerCase().substr(0,1) == 'p' && ipahonorario != ''){

			ipahonorario = ipahonorario.replace('.', ',');
			
			//calcula o valor do honorário
			var valorHonorario = obCal.operacao(mascaraglobal('[###.]###,##', obCal.operacao($F('ipavalorservico'), ipahonorario, '*')), '100', '/');
			$('ipavalorhonorario').setValue(mascaraglobal('[###.]###,##', valorHonorario));

			//calcula o valor do item
			var valorItem = obCal.operacao($F('ipavalorservico'), $F('ipavalorhonorario'), '+');
			$('ipavaloritem').setValue(mascaraglobal('[###.]###,##', valorItem));
			
			//calcula o valor total(mesmo valor do item)
			$('ipavalortotal').setValue(mascaraglobal('[###.]###,##', valorItem));

			//desconto contratual
			$('ipadesconto').setValue('0,00');
		}
		else if(tsedsc.toLowerCase().substr(0,1) == 'm'){

			if($F('ipavaloritem') != ''){

				//calcula o valor do desconto
				var valorDesconto = obCal.operacao(mascaraglobal('[###.]###,##',obCal.operacao($F('ipavaloritem'),'5','*')),'100','/');
				$('ipadesconto').setValue(mascaraglobal('[###.]###,##',valorDesconto));
			
				//calcula o valor do honorário
				var valorHonorario = obCal.operacao(mascaraglobal('[###.]###,##',obCal.operacao($F('ipavaloritem'),'15','*')),'100','/');
				$('ipavalorhonorario').setValue(mascaraglobal('[###.]###,##',valorHonorario));
			
				//calcula o valor do serviço
				var valorServico = obCal.operacao(mascaraglobal('[###.]###,##',obCal.operacao($F('ipavaloritem'),'80','*')),'100','/');
				$('ipavalorservico').setValue(mascaraglobal('[###.]###,##',valorServico));
			
				//calcula o valor total
				var valorTotal = obCal.operacao($F('ipavalorservico'),$F('ipavalorhonorario'),'+');
				$('ipavalortotal').setValue(mascaraglobal('[###.]###,##',valorTotal));
			}
		}
	}

	/**
	 * Habilita/Desabilita campos 
	 * @name habilitarDesabilitarCamposValor
	 * @param string acao - M(mídia) (P)produção
	 * @return void
	 */
	function habilitarDesabilitarCamposValor(acao){

		if(acao == 'M'){
			$('ipavaloritem').removeAttribute('readOnly');
			$('ipavaloritem').className = 'obrigatorio normal';
			$('ipavalorservico').setAttribute('readOnly','readOnly');
			$('ipavalorservico').className = 'disabledCorFonteDiferente';
		}
		else if(acao == 'P'){
			$('ipavalorservico').removeAttribute('readOnly');
			$('ipavalorservico').className = 'obrigatorio normal';
			$('ipavaloritem').setAttribute('readOnly','readOnly');
			$('ipavaloritem').className = 'disabledCorFonteDiferente';
		}
	}

	/**
	 * Limpar campos 
	 * @name limparCamposValor
	 * @return void
	 */
	function limparCamposValor(){
		$('ipavalorservico').clear();
    	$('ipavalorhonorario').clear();
    	$('ipavalortotal').clear();
    	$('ipadesconto').clear();
    	$('ipavaloritem').clear();
	}

	/**
	 * Callback de sucesso na pesquisa da descrição do tipo de serviço 
	 * @name successPegarDscTipoServico
	 * @param object transport - Retorno
	 * @return void
	 */
	function successPegarDscTipoServico(transport){
			
		var resposta = transport.responseText.evalJSON();
		
		if(resposta.status == 'ok'){
			
			$('tsedsc').setValue(resposta.tsedsc);
			
		}
	}
	
	/**
	 * Callback de falha na pesquisa da descrição do tipo de serviço
	 * @name failurePegarDscTipoServico
	 * @return void
	 */
	function failurePegarDscTipoServico(){
	
		alert('Erro');
		
	}
	
	/**
	 * Callback de criação na pesquisa da descrição do tipo de serviço
	 * @name createPegarDscTipoServico
	 * @return void
	 */
	function createPegarDscTipoServico(){
	
		enableButtons(false);
		 
	}
	
	/**
	 * Callback de complete na pesquisa da descrição do tipo de serviço
	 * @name completePegarDscTipoServico
	 * @return void
	 */
	function completePegarDscTipoServico(){
		
		enableButtons();
		
	}


	/**
	 * Verificações de acordo com o fornecedor 
	 * @name verificarHonorarioFornec
	 * @param int forid - Identificador do registro
	 * @return void
	 */ 
	function verificarHonorarioFornec(forid){

		limparCamposValor();

		if(forid == $F('agencia')){
			$('bloquearHons').setValue('S');
			$('hons').innerHTML = '<input type="radio" value="0.00" id="ipahonorario" name="ipahonorario" checked>0.00%';
		}else if($F('forid') == 0){
			limparCamposValor();
			carregarHons($F('forid'),tseid); // utilizado para 'limpar' os radiobuttons
		}else if($F('forid') != '' && $F('forid') != 0){
			$('bloquearHons').clear();
			//tseid � o tipo de contrato, se � m�dia ou produ��o
			verificarHonorarioTipoServico($F('tseid'));			
		}
	}

	
	/**
	 * Verificações de acordo com o tipo de serviço 
	 * @name verificarHonorarioTipoServico
	 * @param int tseid - Identificador do registro
	 * @return void
	 */ 
	function verificarHonorarioTipoServico(tseid,limparCampos){


		if(limparCampos != 'N'){
			limparCamposValor();
		}

		if($F('bloquearHons') != 'S'){			
			//carregarHons($F('cttid'),tseid);
			//carregarHons($F('forid'),tseid);
			carregarHons($F('agencia'),tseid);
		}

		if($F('tsedsc').toLowerCase().substr(0,1) == 'm' && $F('bloquearHons') != 'S'){
			var existeVinte = '';
			
			if($('ipahonorario')){
				$$('input[name=ipahonorario]').each(function(el){
				    if (el.value == '20.00'){
				        $(el).setValue(1);
				        existeVinte = 'S';

				        habilitarDesabilitarCamposValor('M');
				        
				    } else {
				       $(el).disable();
				    }
				});
			}
			else{
				var msg = 'N�o foi(ram) cadastrado(s) honor�rio(s) para este "Tipo de Servi�o"';
				alert(msg);
				$('tseid').focus();
				return false;
			}
			if(existeVinte == ''){
				var msg = 'O valor de honor�rio padr�o para "Tipo de Servi�o" igual a m��dia (20%) n�o foi cadastrado. Favor realizar o cadastro antes de prosseguir com a inclus�o do item da PAD';
				alert(msg);
				$('tseid').focus();
			}	
		}
		else if($F('tsedsc').toLowerCase().substr(0,1) == 'p' || !$F('tsedsc')){
			if(!$('ipahonorario')){
				var msg = 'N�o foi(ram) cadastrado(s) honor�rio(s) para este "Tipo de Servi�o"';
				alert(msg);
				$('tseid').focus();
			}
			else{
				habilitarDesabilitarCamposValor('P');
			}
		}
	}


	/**
	 * Redireciona para a edição
	 * @name detalharItem
	 * @param id - Id do registro
	 * @return void
	 */
	function detalharItem(ipaid){
		var padid = $F('id');
		window.location = '?modulo=principal/pad/cadItemPad&acao=A&id='+padid+'&ipaid='+ipaid;
	}
	
	/**
	 * Redireciona para a inclusaonovamente
	 * @name detalharItem
	 * @param id - Id do registro
	 * @return void
	 */
	function voltarInclusao(){
		var padid = $F('id');
		window.location = '?modulo=principal/pad/cadItemPad&acao=A&id='+padid;
	}
	

	/**
	 * Seta o honorário na edição
	 * @name setarHonorario
	 * @param ipahonorario - Valor advindo do banco
	 * @return void
	 */
	function setarHonorario(ipahonorario){

		 //alert('ipahonorario'+ipahonorario);
		 Form.getInputs('formularioCadastro','radio','ipahonorario').find(
		     function(el){ 
			     if(el.value == ipahonorario){
				     el.checked = true;	
			     } 
			 }
		 );
	}

	/**
	 * Cancelar/Reativar um item
	 * @name cancelarReativarItem
	 * @param ipaid - Identificador do registro
	 * @param ipastatus - Status atual do item
	 * @return void
	 */
	function cancelarReativarItem(ipaid,ipastatus){

		if($F('padstatus') != 'A'){
			var msg = decodeURIComponent(escape('Item não pode ser Cancelado/Reativado!'));
			alert(msg);
			return false;
		}
		else{
			if(ipastatus == 'A'){
				if(confirm('Deseja cancelar o Item da PAD?')){
					var padid = $F('id');
					window.location = '?modulo=principal/pad/cancelarReativarItemPad&acao=A&id='+padid+'&ipaid='+ipaid+'&ipastatus='+ipastatus;					
				}
			}
			else if(ipastatus == 'C'){
				if(confirm('Deseja reativar o Item da PAD?')){
					var padid = $F('id');
					window.location = '?modulo=principal/pad/cancelarReativarItemPad&acao=A&id='+padid+'&ipaid='+ipaid+'&ipastatus='+ipastatus;
				}
			}
		}
	}


	/**
	 * Faturar um item
	 * @name faturarItem
	 * @param ipaid - Identificador do registro
	 * @param ipastatus - Status atual do item
	 * @return void
	 */
	function faturarItem(ipaid,ipastatus){
		 var itensSelecionados = pegarValoresCheckbox();
		 if(itensSelecionados != ''){
		     window.location = '?modulo=principal/pad/faturarItemPad&acao=A&ipaid='+itensSelecionados;
		 }
		 else{
			 window.location = '?modulo=principal/pad/faturarItemPad&acao=A&ipaid='+ipaid;
		 }
	}

	/**
	 * Glosar um item
	 * @name glosarItem
	 * @param ipaid - Identificador do registro
	 * @param ipastatus - Status atual do item
	 * @return void
	 */
	function glosarItem(ipaid,ipastatus){
		 var itensSelecionados = pegarValoresCheckbox();
		 if(itensSelecionados != ''){
			 window.location = '?modulo=principal/pad/glosarItemPad&acao=A&ipaid='+itensSelecionados;  
		 }
		 else{
			 if(ipaid){
				 window.location = '?modulo=principal/pad/glosarItemPad&acao=A&ipaid='+ipaid+',';
			 }
		 }    
	} 


	/**
	 * Pagar um item
	 * @name pagarItem
	 * @param ipaid - Identificador do registro
	 * @param ipastatus - Status atual do item
	 * @return void
	 */
	function pagarItem(ipaid,ipastatus){
		 var itensSelecionados = pegarValoresCheckbox();
		 if(itensSelecionados != ''){
			 window.location = '?modulo=principal/pad/pagarItemPad&acao=A&ipaid='+itensSelecionados;
		 }
		 else{
		 	window.location = '?modulo=principal/pad/pagarItemPad&acao=A&ipaid='+ipaid;
		 }
	}
        
        function cancelarPagamento(ipaid,ipastatus){
		 	window.location = window.location+'&status=cancelarPagamento&ipaid='+ipaid;
	}
        
        function cancelarFaturamento(ipaid,ipastatus){
		 	window.location = window.location+'&status=cancelarFaturamento&ipaid='+ipaid;
	}
        
        

	/**
	 * Pegar os valores das checkbox
	 * @name pegarValoresCheckbox
	 * @return string
	 */
	function pegarValoresCheckbox(){
		var itens = '';
		with(document.formularioItens){
			for(var i=0; i < checkitempad.length; i++){
				if(checkitempad[i].checked){
					itens += checkitempad[i].value + ',';
				}
			}
		}
		return itens;
	}


	/**
	 * Cadastrar um fornecedor
	 * @name cadastrarFornecedor
	 * @return void
	 */
	function cadastrarFornecedor(){
		AbrirPopUp('?modulo=principal/fornecedor/cadFornecedorPopup&acao=A&topo=N','cadastrarFornecedor','scrollbars=yes,width=900,height=770');
	} 

	/**
	 * Recarregar os fornecedores
	 * @name recarregarFornecedores
	 * @return void
	 */
	function recarregarFornecedores(){
		refreshFornecedores();
	}

	/**
	 * Chama o relatório espelho da pad
	 * @name imprimir
	 * @return void
	 */	
	function imprimir(){
	
		var valoresCheckBox =  pegarValoresCheckbox();
		var padid = $F('id');
		AbrirPopUp('?modulo=relatorio/espelhopad/visualizarEspelhoPad&acao=A&padid='+padid+'&valChecks='+valoresCheckBox ,'visualizaEspelhoPad','scrollbars=yes,width=800,height=570');
	}


	 Event.observe(window, 'load', function() {
		if($F('padstatus') != 'A'){
			$('btnIncluir').disable();
			$('btnSalvar').disable();
		}
	});
	
	/**
	 * Chama o relatório espelho da pad
	 * @name imprimir
	 * @return void
	 */	
	function imprimirXLS(){
	
		var valoresCheckBox =  pegarValoresCheckbox();
		var padid = $F('id');
		
		AbrirPopUp('?modulo=relatorio/espelhopad/visualizarEspelhoPad&acao=A&padid='+padid+'&valChecks='+valoresCheckBox + '&isXLS=Excel' ,'visualizaEspelhoPad','scrollbars=yes,width=800,height=570');
	}


	 Event.observe(window, 'load', function() {
		if($F('padstatus') != 'A'){
			$('btnIncluir').disable();
			$('btnSalvar').disable();
		}
	});
</script>

<form name="visualizaritem" id="visualizaritem" method="post">
	<input type='hidden' name='requisicaoitem' id='requisicaoitem' value='' >
	<input type='hidden' name='numeroitem' id='numeroitem' value='' >
</form>

<?php
if( $isVisualizarItem )
{
?>
<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5"
		cellspacing="2">
		
		<tr>
			<td class="SubtituloDireita">Tipo de Servi&ccedil;o:</td>
			<td class="campo">
				<?= $numeroItem ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Status:</td>
			<td class="campo">
				<?= $itemVisu[situacao]?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Descri&ccedil;&atilde;o:</td>
			<td class="campo"> <?php echo $itemVisu['ipadsc'];?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita" >Fornecedor:</td>
			<td class="campo">
				<?php echo $itemVisu['fornome']; ?>
			</td>
		</tr>
		
		<tr>
			<td class="SubtituloDireita">Tipo de Servi&ccedil;o:</td>
			<td class="campo">
				<?= $itemVisu['tsedsc']; ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Categoria:</td>
			<td class="campo">
				<?= $itemVisu['catdsc']; ?>
		</tr>
		<tr>
			<td class="SubtituloDireita">Tipo de Pe&ccedil;a Publicit&aacute;ria:
			</td>
			<td class="campo">
				<?= $itemVisu['tppdsc']; ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Quantidade:</td>
			<td class="campo">
				<?= $itemVisu['ipaqtd']; ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Valor do Servi&ccedil;o:</td>
			<td class="campo">
			
				<?= str_replace(".", "," , $itemVisu['ipavalorservico']); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Desconto Contratual:</td>
			<td class="campo">
				<?= str_replace(".", "," , $itemVisu['ipadesconto']); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Honor&aacute;rio:</td>
			<td class="campo">
				
				<?= $itemVisu['ipahonorario']  ?>%
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Valor do Honor&aacute;rio:</td>
			<td class="campo">
				<?= str_replace(".", "," , $itemVisu['ipavalorhonorario']);  ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Valor do Item:</td>
			<td class="campo">
				<?= str_replace(".", "," , $itemVisu['ipavaloritem']);  ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Valor Total:</td>
			<td class="campo">
				<?= str_replace(".", "," , $itemVisu['ipavalortotal']);  ?>
			</td>
		</tr>
		<tr>
			<td colspan='2' align='center' class="campo"> 
				<input type='hidden' name='requisicao' id='requisicao' value='salvar'> 
				<input type='hidden' name='id' id='id' value='<?php echo $padid; ?>'> 
				<input type='hidden' name='ipaid' id='ipaid' value='<?php echo $ipaid; ?>'>
				<input type='hidden' name='tsedsc' id='tsedsc' value='<?php echo $tsedsc; ?>'> 
				<input type='hidden' name='agencia' id='agencia' value='<?php echo $agencia; ?>'> 
				<input type='hidden' name='cttid' id='cttid' value='<?php echo $cttid; ?>'> 
				<input type='hidden' name='padnumero' id='padnumero' value='<?php echo $padnumero; ?>'> 
				<input type='hidden' name='padvalor' id='padvalor' value='<?php echo $padvalor; ?>'> 
				<input type='hidden' name='padstatus' id='padstatus' value='<?php echo $padstatus; ?>'> 
				<input type='hidden' name='ipastatus' id='ipastatus' value='<?php echo $ipastatus; ?>'> 
				<input type='hidden' name='qtdItemPad' id='qtdItemPad' value='<?php echo $qtdItemPad; ?>'> 
				<input type='hidden' name='bloquearHons' id='bloquearHons' value=''>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Saldo da PAD:</td>
			<td class="campo"><?=campo_texto('saldo','N','N','',13,11,'','','left','',0,'id="saldo"');?>
			</td>
		</tr>
	</table>
</form>
<input type='button' class="botao" name='voltar' id='voltar' value='Voltar' onclick="javascript:recarrega('<?= $urlAtual ?>');">

<?php	
}
else if($_SESSION['visualizar'] != 'S'){
?>

<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5"
		cellspacing="1">
		<tr>
			<td class="SubtituloDireita">N&uacute;mero:</td>
			<td class="campo"><?=campo_texto('ipanumitempad','N','N','',12,10,'','','left','',0,'id="ipanumitempad"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Descri&ccedil;&atilde;o:</td>
			<td class="campo"><?php echo campo_textarea('ipadsc','S', 'S', '', 54, 6, 4000,'','','','','',''); ?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Fornecedor:</td>
			<td class="campo">
				<div id='fornecedor' style='display: inline'>
					<?php $oFornecedor->monta_combo_fornecedor($forid,'S','','cadItemPad');?>
					<script>
						var ag = document.getElementById('forid');
						var opt = ag.getElementsByTagName('option');
		
						for(var i = 0; i < opt.length; i++) {
							if(opt[0].value == '') {
								opt[0].value = 0;
							}						
						}
					</script>
				</div> <a href='javascript:cadastrarFornecedor();'><img border="0"
					title="Incluir Fornecedor" src="../imagens/mais.gif"> Incluir
					Fornecedor </a>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Tipo de Servi&ccedil;o:</td>
			<td class="campo"><?php 
			if($readOnly == 'S'){
				$oTipoServico->monta_combo_tipo_servico($tseid,'S');
			}
			else{
				$oTipoServico->monta_combo_tipo_servico($tseid,'S','cadastro');
			}
			?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Categoria:</td>
			<td class="campo"><div id='categoria'>
					<?php $oTipoServico->monta_combo_categoria('','','S');?>
				</div></td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Tipo de Pe&ccedil;a Publicit&aacute;ria:
			</td>
			<td class="campo"><?php $oTipoPeca->monta_combo_tipo_peca($tppid,'S');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Quantidade:</td>
			<td class="campo"><?=campo_texto('ipaqtd','S','S','',13,11,'[###.]###','','left','',0,'id="ipaqtd"');?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Valor do Servi&ccedil;o:</td>
			<td class="campo"><?php
			if(!empty($ipavalorservico)){
					$ipavalorservico = number_format($ipavalorservico, 2, ',', '.');
				}
				echo campo_texto('ipavalorservico','S','N','',13,11,'[###.]###,##','','left','',0,'id="ipavalorservico"','calcularValores();');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Desconto Contratual:</td>
			<td class="campo"><?php
			if(!empty($ipadesconto)){
					$ipadesconto = number_format($ipadesconto, 2, ',', '.');
				}
				echo campo_texto('ipadesconto','S','N','',13,11,'[###.]###,##','','left','',0,'id="ipadesconto"');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Honor&aacute;rio:</td>
			<td class="campo">
				<div id='hons' style='display: inline'>
					
				</div> <img border="0" title="Indica campo obrigatório."
				src="../imagens/obrig.gif">
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Valor do Honor&aacute;rio:</td>
			<td class="campo"><?php
			if(!empty($ipavalorhonorario)){
					$ipavalorhonorario = number_format($ipavalorhonorario, 2, ',', '.');
				}
				echo campo_texto('ipavalorhonorario','S','N','',13,11,'[###.]###,##','','left','',0,'id="ipavalorhonorario"');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Valor do Item:</td>
			<td class="campo"><?php
			if(!empty($ipavaloritem)){
					$ipavaloritem = number_format($ipavaloritem, 2, ',', '.');
				}
				echo campo_texto('ipavaloritem','S','N','',13,11,'[###.]###,##','','left','',0,'id="ipavaloritem"','calcularValores();');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Valor Total:</td>
			<td class="campo"><?php
			if(!empty($ipavalortotal)){
					$ipavalortotal = number_format($ipavalortotal, 2, ',', '.');
				}
				echo campo_texto('ipavalortotal','S','N','',13,11,'[###.]###,##','','left','',0,'id="ipavalortotal"');
				?>
			</td>
		</tr>
		<tr>
			<td colspan='2' align='center' class="campo">
				<?php if ($editarItem) { ?>
					<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnSalvar' id='btnSalvar' value='Salvar' onclick="javascript:salvar();"> 
					<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnVoltarItem' id='btnVoltarItem' value='Novo' onclick="javascript:voltarInclusao();"> 
				<?php } else { ?>
					<input type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao" name='btnIncluir' id='btnIncluir' value='Incluir' onclick="javascript:incluir();"> 
				<?php }?>
				<input type='hidden' name='requisicao' id='requisicao' value='salvar'> 
				<input type='hidden' name='id' id='id' value='<?php echo $padid; ?>'> 
				<input type='hidden' name='ipaid' id='ipaid' value='<?php echo $ipaid; ?>'>
				<input type='hidden' name='tsedsc' id='tsedsc' value='<?php echo $tsedsc; ?>'> 
				<input type='hidden' name='agencia' id='agencia' value='<?php echo $agencia; ?>'> 
				<input type='hidden' name='cttid' id='cttid' value='<?php echo $cttid; ?>'> 
				<input type='hidden' name='padnumero' id='padnumero' value='<?php echo $padnumero; ?>'> 
				<input type='hidden' name='padvalor' id='padvalor' value='<?php echo $padvalor; ?>'> 
				<input type='hidden' name='padstatus' id='padstatus' value='<?php echo $padstatus; ?>'> 
				<input type='hidden' name='ipastatus' id='ipastatus' value='<?php echo $ipastatus; ?>'> 
				<input type='hidden' name='qtdItemPad' id='qtdItemPad' value='<?php echo $qtdItemPad; ?>'> 
				<input type='hidden' name='bloquearHons' id='bloquearHons' value=''>
			</td>
		</tr>
		<tr>
			<td class="SubtituloDireita">Saldo da PAD:</td>
			<td class="campo"><?=campo_texto('saldo','N','N','',13,11,'','','left','',0,'id="saldo"');?>
			</td>
		</tr>
	</table>
</form>
<?php 
}
else if($_SESSION['visualizar'] == 'S'){
?>
<form name="formularioCadastro" id="formularioCadastro" method="post">
	<table class="tabela" align="center" border="0" cellpadding="5"
		cellspacing="1">
		<tr>
			<td class="SubtituloDireita">Saldo da PAD:</td>
			<td class="campo"><?=campo_texto('saldo','N','N','',13,11,'','','left','',0,'id="saldo"');?>
				<input type='hidden' name='id' id='id' value='<?php echo $padid; ?>'>
				<input type='hidden' name='ipaid' id='ipaid' value='<?php echo $ipaid; ?>'> 
				<input type='hidden' name='tsedsc' id='tsedsc' value='<?php echo $tsedsc; ?>'> 
				<input type='hidden' name='agencia' id='agencia' value='<?php echo $agencia; ?>'>
				<input type='hidden' name='cttid' id='cttid' value='<?php echo $cttid; ?>'>
				<input type='hidden' name='padnumero' id='padnumero' value='<?php echo $padnumero; ?>'> 
				<input type='hidden' name='padvalor' id='padvalor' value='<?php echo $padvalor; ?>'> 
				<input type='hidden' name='padstatus' id='padstatus' value='<?php echo $padstatus; ?>'> 
				<input type='hidden' name='ipastatus' id='ipastatus' value='<?php echo $ipastatus; ?>'> 
				<input type='hidden' name='qtdItemPad' id='qtdItemPad' value='<?php echo $qtdItemPad; ?>'> 
				<input type='hidden' name='bloquearHons' id='bloquearHons' value=''>
			</td>
		</tr>
	</table>
</form>
<?php 
}
?>
<!-- TRECHO DOS ITENS -->
<table class="tabela" align="center" border="0">
	<?php
	//lista os itens da pad
	if(!empty($padid) && $qtdItemPad >= 1){
		?>
	<tr>
		<td bgcolor='#e9e9e9' align='center' colspan='4'>Lista de Itens da PAD
		</td>
	</tr>
</table>

<?php 
if($_GET['redirectItensPad']) 
{
	$nota = ($_GET['nota']) ? $_GET['nota'] : false;
	$ob = ($_GET['ob']) ? $_GET['ob'] : false;
	
	$arrParams = array(
		'fipnumfaturaagencia' => $nota,
		'pipnumordembancaria' => $ob
	);
	
	$oItemPad->listar_por_id($padid,$readOnly, $arrParams);
}
else
{	
	$oItemPad->listar_por_id($padid,$readOnly);
}
}
?>
<table class="tabela" align="center" border="0" cellpadding="5"
	cellspacing="1">
	<tr>
		<td colspan='2' align='left'>
			<!-- Botao Glosar --> <?php if($_SESSION['visualizar'] != 'S'):?> <input
			type='button' <?php if($readOnly == 'S'){ echo 'disabled'; } ?>
			class="botao" name='btnGlosarItemPad' id='btnGlosarItemPad'
			value='Glosar itens da PAD' onclick="glosarItem( '','G' );"
			style="cursor: pointer; cursor: hand;"> <?php endif;?>
		</td>
	</tr>
	<tr class="buttons">
		<td colspan='2' align='center'><input type='button'
		<?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao"
			name='btnVoltar' id='btnVoltar' value='<< Anterior'
			onclick="javascript:voltar();"> <?php 
			if($_SESSION['visualizar'] != 'S'){
            	?> <input type='button'
            	<?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao"
			name='btnSalvar' id='btnSalvar' value='Salvar'
			onclick="javascript:salvar();"> <?php 
            	}
            	?> <input type='button'
            	<?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao"
			name='btnImprimir' id='btnImprimir' value='Imprimir'
			onclick="javascript:imprimir();">
			
			<input type='button'
            	<?php if($readOnly == 'S'){ echo 'disabled'; } ?> class="botao"
			name='btnImprimir' id='btnImprimir' value='Imprimir Excel'
			onclick="javascript:imprimirXLS();">
		</td>
	</tr>
</table>
<?php 
if(!empty($ipaid) && !empty($tseid)){
	echo "<script>carregarCategoria(".$tseid.",".$catid.",'S');</script>";
	echo "<script>verificarHonorarioTipoServico(".$tseid.",'N')</script>";
	echo "<script>setarHonorario('".$ipahonorario."');</script>";
}
?>