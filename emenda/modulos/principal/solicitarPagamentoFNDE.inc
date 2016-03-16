<?php
require_once APPRAIZ . "emenda/classes/WSEmpenho.class.inc";
require_once APPRAIZ . "emenda/classes/ExecucaoFinanceira.class.inc";

$_SESSION["emenda"]["ptridAnalise"] = !empty( $_REQUEST["ptridAnalise"] ) ? $_REQUEST["ptridAnalise"] : $_SESSION["emenda"]["ptridAnalise"];
$_SESSION["emenda"]["ptridAnalise"] = !empty( $_SESSION["emenda"]["ptridAnalise"] ) ? $_SESSION["emenda"]["ptridAnalise"] : $_SESSION["emenda"]["ptrid"];

$ptrid = $_SESSION["emenda"]["ptridAnalise"];

if ( !$ptrid ){
	echo '<script>
			alert("A sessão do Plano de Trabalho foi perdida!");
			window.location = "emenda.php?modulo=principal/analiseDadosPTA&acao=A";
		  </script>';
	die;
}

unset( $_SESSION['emenda']['msgErro'] );
$_SESSION['emenda']['msgErro'] = array();

include_once APPRAIZ . 'includes/workflow.php';
include  APPRAIZ."includes/cabecalho.inc";
print "<br/>";

$boReformulacao = pegaPaiPTA( $_SESSION["emenda"]["ptridAnalise"] );
$boReformulacao = ( !empty($boReformulacao) ? true : false );

$estadoAtual = pegarEstadoAtual( $_SESSION["emenda"]["ptridAnalise"] );

montaAbasAnalise( $abacod_tela, $url, '' , $estadoAtual, $boReformulacao);
// Cria o título da tela
monta_titulo( 'Pagamento', "<img border='0' src='../imagens/obrig.gif' title='Indica campo obrigatório.' /> Indica os campos obrigatórios");

echo cabecalhoPlanoTrabalho($ptrid, true);

montaVisualizarPTA( $_SESSION["emenda"]["ptridAnalise"], false, $boReformulacao );

$arDados = carregaDadosEmpenhoPagamento( $ptrid );

$perfil = pegaPerfil( $_SESSION['usucpf'] );

if ( ($estadoAtual != EM_LIBERACAO_RECURSO && $estadoAtual != EM_LIBERACAO_RECURSO_REFORMULACAO) && ( $perfil != PAGAMENTO || $perfil != SUPER_USUARIO ) ) {
	$boDisabled = 'disabled="disabled"';
}
?>
<html>
<script type="text/javascript" src="/includes/funcoes.js"></script>
<script type="text/javascript" src="js/emenda.js"></script>
<link rel="stylesheet" type="text/css" href="css/emenda.css"/>
<body>
	<?php abrePTAconsolidado($ptrid); ?>
	<form action="emenda.php?modulo=principal/solicitarPagamentoFNDE&acao=A" name="formPagamentoPTA" id="formPagamentoPTA">
	<table id="tblform" class="tabela" width="95%" bgcolor="#f5f5f5" cellspacing="1" cellpadding="2" align="center">
		<tr>
			<td colspan="2" class="subtitulocentro">Dados para o Pagamento</td>
		</tr>
					
		<!-- <tr>
			<td class="SubTituloDireita" valign="middle"><b>Cronograma Desembolso:</b></td>
			<td >
				<a href="#" onclick="popUpIncluirCronogramaDesenbolso();" style="cursor:pointer">
					<label id="labelCronograma">Clique aqui para preencher o Cronograma Desembolso do Convenio</label>
				</a>
				<input type="hidden" name="boCronograma" id="boCronograma" value="<?=$boCronograma; ?>" />
			</td>
		</tr> -->
	</table>
	</form>
	<?	
	$sql = "SELECT  
			  	ord.orbvalorparcela,
			  	ord.orbanoparcela,
			  	ord.orbmesparcela,
			  	ptr.ptrexercicio,
			  	exf.exfid,
			  	ord.orbid,
			  	ord.orbnumsolicitacao
			FROM
				emenda.ordembancaria ord
			    inner join emenda.execucaofinanceira exf
			    	on exf.exfid = ord.exfid
				inner join emenda.planotrabalho ptr on ptr.ptrid = exf.ptrid
			    inner join emenda.ordembancaria orb on orb.exfid = exf.exfid
			    	and ord.orbvalorparcela is not null
			  		and orb.spgcodigo not in ('2', '9')
			WHERE
			  	ptr.ptrid = $ptrid
			  	and exf.exfstatus = 'A'
			ORDER BY
			    ord.orbanoparcela";
	
	$arParcela = $db->carregar($sql);
	$arParcela = ($arParcela) ? $arParcela : array();
	$totalParcela = sizeof( $arParcela );
	
	$arParcelaPaga = array();
	$valorTotalParcela = 0;
	foreach ($arParcela as $v) {
		if( $v['orbnumsolicitacao'] )
			$arParcelaPaga[] = $v['orbnumsolicitacao'];	
		$valorTotalParcela = (float)$arValorParcelaPaga + (float)$v['orbvalorparcela'];
	}
	
	montaTabelaSolicitacaoPagamento($arDados, $ptrid, $_GET['boAlterarPagamento'], $boDisabled, $valorTotalParcela, $totalParcela, $arParcelaPaga, $boReformulacao, $estadoAtual);
	
	/*$acaoAlterarHabilitada = "'<center><img src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"alterarPagamento( \''||exf.exfid||'\');\" border=0 alt=\"Ir\" title=\"Alterar\"> '";
	$acaoAlterarNaoHabilitada = "'<center><img src=\"/imagens/alterar_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Alterar\"> '";*/
	
	$acaoConsultarHabilitada = "'<center><img src=\"/imagens/refresh2.gif\" style=\"cursor: pointer\" onclick=\"consultarPagamentoPTA( \''||ptr.ptrid||'\', \''||exf.exfid||'\', \''||orb.orbid||'\' );\" border=0 alt=\"Ir\" title=\"Consultar Pagamento\"> '";
	$acaoConsultarNaoHabilitada = "'<center><img src=\"/imagens/refresh2_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Consultar Pagamento\"> '";
	
	$acaoconsultarPagamentoHistorico = "'&nbsp;<img src=\"/imagens/consultar.gif\" style=\"cursor: pointer\" onclick=\"consultarPagamentoHistorico( \''||ptr.ptrid||'\', \''||orb.orbid||'\' );\" border=0 alt=\"Ir\" title=\"Consultar Histórico Pagamento\"></center>'";
	
	$acaoAnulacaoTotalHabilitada = "'<img src=\"/imagens/excluir.gif\" style=\"cursor: pointer\" onclick=\"cancelarPagamento(\''||ptr.ptrid||'\', \''||exf.exfid||'\',\''||orb.orbnumsolicitacao||'\',\''||orb.orbid||'\');\" border=0 alt=\"Ir\" title=\"Anulação Total do Pagamento\">'";
	$acaoAnulacaoTotalNaoHabilitada = "'<img src=\"/imagens/excluir_01.gif\" style=\"cursor: pointer\" border=0 alt=\"Ir\" title=\"Anulação Total do Pagamento\">'";
	
	if($perfil == SUPER_USUARIO){
		$acaoConsultarNaoHabilitada = $acaoConsultarHabilitada;
	}
	
	/*$acaoAlterarHabilitada || $acaoConsultarHabilitada || $acaoAnulacaoTotalHabilitada else 
				$acaoAlterarNaoHabilitada || $acaoConsultarNaoHabilitada || $acaoAnulacaoTotalNaoHabilitada */
	
	$sql  = "SELECT DISTINCT
				(CASE WHEN orbnumordembancaria is null THEN 
				$acaoConsultarHabilitada || $acaoAnulacaoTotalHabilitada else 
				$acaoConsultarNaoHabilitada || $acaoAnulacaoTotalNaoHabilitada
				end)||
					$acaoconsultarPagamentoHistorico
				as acao,
			    vede.emecod,
                exf.exfnumempenhooriginal,
                orb.orbmesparcela,
                orb.orbnumparcela,
                orb.orbvalorparcela,
                orb.orbnumsolicitacao,
                orb.orbnumordembancaria,
                sip.spgdescricao,
                to_char(orb.orbdatainclusao, 'DD/MM/YYYY HH24:MI:SS') as orbdatainclusao
			FROM
            	emenda.planotrabalho ptr
                inner join emenda.execucaofinanceira exf
                	on exf.ptrid = ptr.ptrid
                inner join emenda.ordembancaria orb
                	on orb.exfid = exf.exfid
				inner join emenda.ptemendadetalheentidade ped
			  		on ped.ptrid = ptr.ptrid
				inner join emenda.v_emendadetalheentidade vede
			  		on vede.edeid = ped.edeid
                    and ped.pedid = exf.pedid
                inner join emenda.situacaopagamento sip
                	on sip.spgcodigo = orb.spgcodigo
			WHERE 
				ptr.ptrid = $ptrid 
                and exf.exfstatus = 'A'
                and vede.edestatus = 'A'
                and orb.orbnumsolicitacao is not null
			ORDER BY 
				vede.emecod";

	print '<table border="0" cellspacing="0" cellpadding="3" align="center" bgcolor="#DCDCDC" class="tabela" style="border-top: none; border-bottom: none;">';
	print '<tr><td bgcolor="#e9e9e9" align="center" style="FILTER: progid:DXImageTransform.Microsoft.Gradient(startColorStr=\'#FFFFFF\', endColorStr=\'#dcdcdc\', gradientType=\'1\')" class="subtitulocentro" >Lista de Pagamentos</td></tr></table>';
	
	$arDadosP = $db->carregar( $sql );
	$arDadosP = ( $arDadosP ? $arDadosP : array() );
	
	foreach ($arDadosP as $key => $v) {
		$registro[$key] = array(
							"acao" => $v['acao'],
							"emecod" => $v['emecod'],
							"ne" => $v['exfnumempenhooriginal'],
							"parcela" => $v['orbnumparcela'],
							"orbmesparcela" => $v['orbmesparcela'],
							"orbvalorparcela" => $v['orbvalorparcela'],
							"orbnumsolicitacao" => $v['orbnumsolicitacao'],
							"situacao" => $v['spgdescricao'],
							"datainclusao" => $v['orbdatainclusao'],
							"numordembancária" => $v['orbnumordembancaria'],
							);	
	}
	
	
	$cabecalho 	    = array("Ação","Código", "Nº Empenho", "Parcela", "Mês", "Valor", "Nº solicitacao", "Situaçao", "Data Inclusao", "Nº Ordem bancária");
	//$tamanho		= array( '10%', '7%', '7%', '10%', '5%', '10%', '15%', '15%', '10%', '15%', '15%' );															
	//$alinhamento	= array( 'center', 'left', 'right', 'center', 'right', 'center', 'right', 'center', 'center', 'center', 'center' );													
	//$db->monta_lista( $registro, $cabecalho, 25, 10, 'N', 'center', '', '', $tamanho, $alinhamento);
	$db->monta_lista_array($registro, $cabecalho, 5000, 20, '', 'center', '');
	?>
	<div id="erro"></div>
</body>
<!-- LINKS MODAL -->
<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
<script type="text/javascript"><!--

	messageObj = new DHTML_modalMessage();	// We only create one object of this class
	messageObj.setShadowOffset(5);	// Large shadow
	
	function displayMessage(url){
		messageObj.setSource(url);
		messageObj.setCssClassMessageBox(false);
		messageObj.setSize(560,345);
		messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
		messageObj.display();
	}
	
	function closeMessage(){
		messageObj.close();	
	}

	function solicitarPagamentoPTA(ptrid){
		var form = $('formExecPTA');
		var msg  = '';
		
		var boVazio = false;
		for(i=0; i<form.length; i++){
			if(form.elements[i].type == 'text'){
				if( form.elements[i].id.indexOf('orbmesparcela') != -1 || form.elements[i].id.indexOf('orbanoparcela') != -1 || form.elements[i].id.indexOf('orbvalorparcela') != -1 ){
					if( form.elements[i].value == '' ){
						boVazio = true;
					}
				}		
			}
		}
		
		if( boVazio == true || form.length == 1){
			msg += 'É necessário cadastrar a(s) parcela(s) do pagamento';
		}		
		
		if(msg){
			alert(msg);
			return false;			
		}
		var retorno = 'true';
		var totCampos = form.length;
		var i = 0;
		for(i=0; i<form.length; i++){
			if(form.elements[i].type == 'hidden'){
				if( form.elements[i].id.indexOf('exfid_p') != -1 ){
					retorno = validaValorParcela( form.elements[i].value, totCampos );
				}
			}
		}
		
		if( retorno == 'false' ){
			return false;
		}
		var registro = '';
		//if( $('boalterou').value == 'true' ){
			var url = "ajax_emenda.php";
			var pars = 'execucaoPTA=salvarParcela&'+ $('formExecPTA').serialize();
				
			var myAjax = new Ajax.Request(
					url, 
					{
						method: 'post', 
						parameters: pars, 
						asynchronous: false,
						onComplete: function(r){
							//$('divDebug').innerHTML = r.responseText;
							registro = r.responseText;
							/*if( trim(r.responseText) == '1'){
							}else{
								alert('Falha na operação.');
							}*/
						}
					});
		//}
		displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_pagamento=original&ptrid='+ptrid+'&solicitacao=pagamento&arparcela='+registro);
	}
	function consultarPagamentoPTA(ptrid,exfid, orbid){
		if(ptrid && exfid && orbid){
			displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_pagamento=consultaPagamento&solicitacao=pagamento&ptrid='+ptrid+'&exfid='+exfid+'&orbid='+orbid);
		} else {
			alert('Operação Consulta Situação pagamento falhou.');
			return false;
		}

	}
	function popUpIncluirCronogramaDesenbolso() {
		document.getElementById('labelCronograma').innerHTML = 'Clique aqui para visualizar o Cronograma Desembolso do Convenio';	
		var janela = window.open( 'emenda.php?modulo=principal/cronogramaExecuccaoDesembolso&acao=A&chamada=pagamento', 'popup_Cronograma', 'width=800,height=700,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
		janela.focus();
	}
	function cancelarPagamento(ptrid, exfid, nu_seg_pag, orbid){
				
		if(nu_seg_pag){
			if(!confirm('Deseja realmente cancelar o pagamento?')){
				return false;
			}
			displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_pagamento=anular&solicitacao=pagamento&ptrid='+ptrid+'&exfid='+exfid+'&orbnumsolicitacao='+nu_seg_pag+'&orbid='+orbid);
		}
		/*
		var boAnulacaoTotal = false;
		if(valorOriginal != '' && valorAtual != ''){
			if(valorAtual != valorOriginal){
				if(valorAtual > valorOriginal){
					var valorAtualLimpo    = mascaraglobal( "[###]", valorAtual );
		            var valorOriginalLimpo = mascaraglobal( "[###]", valorOriginal );
		            var novoValor 		   = mascaraglobal( "[###]###.##", parseInt( valorAtualLimpo ) - parseInt( valorOriginalLimpo ) );
					var especie_pagamento = 'reforco';
				} else {
					var valorAtualLimpo    = mascaraglobal( "[###]", valorAtual );
		            var valorOriginalLimpo = mascaraglobal( "[###]", valorOriginal );
		            var novoValor          = mascaraglobal( "[###]###.##", parseInt( valorOriginalLimpo ) - parseInt( valorAtualLimpo ) );
					var especie_pagamento = 'anulacao';
				}
			} else {
				// Anulação Total
				var valorOriginalLimpo = mascaraglobal( "[###]", valorOriginal );
				var novoValor       = mascaraglobal( "[###]###.##", valorOriginalLimpo );
				var especie_pagamento = 'anulacao';
				var boAnulacaoTotal = true;
			}

			if(ptrid && exfid){
				displayMessage('?modulo=principal/popUpSenhaFNDE&acao=A&especie_pagamento='+especie_pagamento+'&ptrid='+ptrid+'&exfid='+exfid+'&novoValor='+novoValor+'&boAnulacaoTotal='+boAnulacaoTotal+'&verificadoSiafi='+verificadoSiafi+'&verificadoCadin='+verificadoCadin);
			} else {
				alert('Operação Alterar o valor falhou.');
				return false;
			}
		}*/
		
	}
	
	function carregarParcela( orbmesparcela, orbanoparcela, orbvalorparcela, orbid, exfid, boAlterou, disabilita, parcela ){
		var tabela 	= document.getElementById( "tabelaParcela_" + exfid );
		var tbody   = document.getElementById( "tbodyTabela_" + exfid );
		var qtdLinha = tabela.rows.length;
		var qtd = qtdLinha - 2;
		var qtdL = parcela + 1;//qtd - 1;
		var boExcluir = true;
		
		if( boAlterou )
			$('boalterou').value = 'true';
		
		orbvalorparcela = mascaraglobal('[###.]###,##',orbvalorparcela);
										
		linha = tabela.insertRow( tabela.rows.length );
		tbody.appendChild( linha );
	
		linha.setAttribute( "id", "linha_" + qtd );
		linha.setAttribute( "style", "background: #f5f5f5;" );
		linha.setAttribute( "align", "center" );
												
		var img = document.createElement( "img" );
		if( disabilita != '' ){
			img.setAttribute( "src", "/imagens/excluir_01.gif" );
		} else {
			img.setAttribute( "src", "/imagens/excluir.gif" );
		}
		img.setAttribute( "title", "Excluir" );
		img.setAttribute( "alt", "Excluir" );
		
		if( disabilita == '' ){
			// OnClick
			if(window.addEventListener){ // Mozilla, Netscape, Firefox
				img.setAttribute( "onclick", "removerLinha( 'linha_" + qtd + "', '"+ exfid +"' )" );
			}
			else{ // IE
				img.attachEvent( "onclick", function() { removerLinha( "linha_" + qtd, exfid ) } );
			}
		}
		
		var input_parcela = document.createElement( "input" );
		input_parcela.setAttribute( "type", "text" );
		input_parcela.setAttribute( "name", "numparcela["+exfid+"][]");
		input_parcela.setAttribute( "id", "numparcela[]");
		input_parcela.setAttribute( "value", qtdL);
		input_parcela.setAttribute( "size", "2");
		input_parcela.setAttribute('readonly','readonly');
		
		var input_orbmesparcela = document.createElement( "input" );
		input_orbmesparcela.setAttribute( "type", "text" );
		input_orbmesparcela.setAttribute( "name", "orbmesparcela["+exfid+"][]");
		input_orbmesparcela.setAttribute( "id", "orbmesparcela[]");
		input_orbmesparcela.setAttribute( "value", orbmesparcela);
		input_orbmesparcela.setAttribute( "maxlength", "2");
		input_orbmesparcela.setAttribute( "size", "10");
		
		var input_orbanoparcela = document.createElement( "input" );
		input_orbanoparcela.setAttribute( "type", "text" );
		input_orbanoparcela.setAttribute( "name", "orbanoparcela["+exfid+"][]");
		input_orbanoparcela.setAttribute( "id", "orbanoparcela[]");
		input_orbanoparcela.setAttribute( "value", orbanoparcela);
		input_orbanoparcela.setAttribute( "maxlength", "4");
		input_orbanoparcela.setAttribute( "size", "10");
		
		var valor = '';
		if( orbvalorparcela ){
			var valor = orbvalorparcela.replace(/\./gi, '');
			var valor = valor.replace(/\,/gi, '.'); 
		}
		var valor1 = document.createElement( "input" );
		valor1.setAttribute( "type", "hidden" );
		valor1.setAttribute( "name", "valor1_["+exfid+"]"+ "_"+qtdL);
		valor1.setAttribute( "id", "valor1_["+exfid+"]"+"_"+qtdL);
		valor1.setAttribute( "value", valor);
		
		var input_orbvalorparcela = document.createElement( "input" );
		input_orbvalorparcela.setAttribute( "type", "text" );
		input_orbvalorparcela.setAttribute( "name", "orbvalorparcela["+exfid+"][]");
		input_orbvalorparcela.setAttribute( "id", "orbvalorparcela[]");
		input_orbvalorparcela.setAttribute( "value", orbvalorparcela);
		input_orbvalorparcela.setAttribute( "maxlength", "13");
		input_orbvalorparcela.setAttribute( "size", "15");
		
		var input_exfid = document.createElement( "input" );
		input_exfid.setAttribute( "type", "hidden" );
		input_exfid.setAttribute( "name", "exfid["+exfid+"][]");
		input_exfid.setAttribute( "id", "exfid["+qtdL+"]");
		input_exfid.setAttribute( "value", exfid);
		
		var input_orbid = document.createElement( "input" );
		input_orbid.setAttribute( "type", "hidden" );
		input_orbid.setAttribute( "name", "orbid["+exfid+"][]");
		input_orbid.setAttribute( "id", "orbid["+qtdL+"]");
		input_orbid.setAttribute( "value", orbid);

		// OnKeyUp
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_orbmesparcela.setAttribute( "onkeyup", "this.value=mascaraglobal('[#]',this.value);" );
		}
		else{ // IE
			//valor.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}
		// OnKeyUp ano parcela
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_orbanoparcela.setAttribute( "onkeyup", "this.value=mascaraglobal('[#]',this.value);" );
		}
		else{ // IE
			//valor.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}
		// OnKeyUp
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_orbvalorparcela.setAttribute( "onkeyup", "this.value=mascaraglobal('[###.]###,##',this.value);" );
		}
		else{ // IE
			//valor.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}
		// Onblur
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_orbvalorparcela.setAttribute( "onblur", "this.value=mascaraglobal('[###.]###,##',this.value); populaHiddenOrbvalorparcela(this.value,'"+exfid+"', "+ qtdL +");" );
		}
		else{ // IE
			//valor.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}
		
		// Onblur Mes Parcela
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_orbmesparcela.setAttribute( "onblur", "this.value=mascaraglobal('[#]',this.value); validaMesParcela(this,'"+exfid+"');" );
		}
		else{ // IE
			//input_orbmesparcela.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}
		
		// Onblur Ano Parcela
		if(window.addEventListener){ // Mozilla, Netscape, Firefox
			input_orbanoparcela.setAttribute( "onblur", "this.value=mascaraglobal('[#]',this.value); validaAnoParcela(this,'"+exfid+"');" );
		}
		else{ // IE
			//input_orbmesparcela.attachEvent( "onkeyup", function() { eventosIE( document.getElementById( 'duncpf_' + qtd ), "5" ); } );
		}

        if( boExcluir ){  
			celula = linha.insertCell( 0 );
			celula.appendChild( img );
		} else {
			var img = document.createElement( "img" );
			img.setAttribute( "src", "/imagens/excluir_01.gif" );
			img.setAttribute( "title", "Excluir" );
			img.setAttribute( "alt", "Excluir" );
			celula = linha.insertCell( 0 );
			celula.appendChild( img );
		}
		
		celula = linha.insertCell( 1 );
		celula.appendChild( input_parcela );
		
		var imgObrigatorio = document.createElement( "img" );
		imgObrigatorio.setAttribute( "src", "../imagens/obrig.gif" );
		imgObrigatorio.setAttribute( "title", "Indica campo obrigatório." );												
		
		celula = linha.insertCell( 2 );
		celula.appendChild( input_exfid );
		celula.appendChild( input_orbid );
		celula.appendChild( input_orbmesparcela );
		celula.appendChild( imgObrigatorio );
		
		var imgObrigatorio1 = document.createElement( "img" );
		imgObrigatorio1.setAttribute( "src", "../imagens/obrig.gif" );
		imgObrigatorio1.setAttribute( "title", "Indica campo obrigatório." );
		
		celula = linha.insertCell( 3 );
		//celula.setAttribute( 'colspan', 2 );
		celula.appendChild( input_orbanoparcela );
		celula.appendChild( imgObrigatorio1 );
		//celula.appendChild( label );
		
		var imgObrigatorio2 = document.createElement( "img" );
		imgObrigatorio2.setAttribute( "src", "../imagens/obrig.gif" );
		imgObrigatorio2.setAttribute( "title", "Indica campo obrigatório." );
		
		//linha.setAttribute( "align", "left" );
		celula = linha.insertCell( 4 );
		celula.appendChild( input_orbvalorparcela );
		celula.appendChild( imgObrigatorio2 );
		celula.appendChild( valor1 );
		
		montaNumeroParcela(exfid);
	}
	function removerLinha( idLinha, exfid ){		
		if (!confirm('Atenção! O item selecionado será apagado permanentemente!\nDeseja continuar?')) {
			return false;
		}
		
		var tabela 	= document.getElementById( "tabelaParcela_" + exfid );
		var linha 	= document.getElementById( idLinha );
		tabela.deleteRow( linha.rowIndex );
		
		montaNumeroParcela(exfid);
		$('boalterou').value = 'true';
	}
	
	function populaHiddenOrbvalorparcela( valor, exfid, id ){
		var valor = valor.replace(/\./gi, '');
		var valor = valor.replace(/\,/gi, '.');
		
		document.getElementById( 'valor1_[' + exfid + ']_'+ id ).value = valor;	
	}
	
	function montaNumeroParcela(exfid){
		var form = document.getElementById('formExecPTA');
		var parcela = 'parcela_'+exfid;
		var count = $(parcela).value;		
		
		for(i=0; i<form.length; i++){
			if(form.elements[i].type == 'text'){
				if(form.elements[i].id.indexOf('numparcela[]') != -1){
					count++;
					form.elements[i].value = count;
				}				
			}
		}
	}
	
	function validaValorParcela( exfid, totCampos ){
		var form1 = $('formExecPTA');
		var parcelapaga = 'parcelapaga_'+exfid;
		var valorparcelapaga = parseFloat( $(parcelapaga).value ); 
		var valorTotal = valorparcelapaga; 
		var exfvalor = 'exfvalor_'+exfid;
		var i = 0;
		
		for(i=0; i<totCampos; i++){
			if(form1.elements[i].type == 'hidden'){
				if(form1.elements[i].id.indexOf('valor1_[' + exfid + ']') != -1){
					valorTotal = valorTotal + parseFloat( form1.elements[i].value );
				}				
			}
		}
		var restante = $(exfvalor).value -  valorparcelapaga.toFixed(2);
		if( valorTotal.toFixed(2) > parseFloat( $(exfvalor).value ) ){
			alert('O valor das parcelas não pode ultrapassar o valor do empenho: \n Valor das Parcelas:    R$ '+ mascaraglobal('###.###.###.###,##', valorTotal.toFixed(2)) +' \n Valor do Empenho:    R$ ' +  mascaraglobal('###.###.###.###,##', $(exfvalor).value) );
			return 'false';
		} else {
			return 'true';
		}
	}
	
	function validaMesParcela(mes, exfid){
		var form = document.getElementById('formExecPTA');
		var arMes = new Array('01', '02', '03', '04', '05', '06', '07', '08', '09', '10', '11', '12');
		
		if( mes.value != '' ){
			if( mes.value.length == 1 ){
				mes.value = '0'+mes.value;
			}
			if( arMes.indexOf(mes.value) == -1 ){
				alert('Mês inválido!');
				mes.value = '';
				return false;
			}
		}
	}
	
	function validaAnoParcela(ano, exfid){
		var form = document.getElementById('formExecPTA');
		var count = 0;
		
		/*if( ano.value != '' ){
			for(i=0; i<form.length; i++){
				if(form.elements[i].type == 'text'){
					if( ( form.elements[i].name.indexOf('orbanoparcela[' + exfid + '][]') != -1 ) && ( form.elements[i].value != '' ) ){
						if( form.elements[i].value == ano.value ){
							count++;
						}
					}				
				}
			}
			if( count > 1 ){
				alert('Já foi informado uma parcela para este mês\ano!');
				ano.value = '';
				return false;
			}
		}*/
	}
	
	function consultarPagamentoHistorico(ptrid, orbid){
		if(ptrid){
			window.open('emenda.php?modulo=principal/popupConsultaPagamentoPTA&acao=A&ptrid='+ptrid+'&orbid='+orbid, 'Consulta Pagamento PTA', 'scrollbars=yes,height=800,width=1200,status=no,toolbar=no,menubar=no,location=no');
		} else {
			alert('Operação Consulta Execução falhou.');
			return false;
		}

	}
--></script>
</html>
<?	
	foreach ($arParcela as $key => $v) {
		$boSolicitacao = ( $v['orbnumsolicitacao'] ? true : false );
		$numparcela = sizeof($arParcelaPaga);
		if( !$boSolicitacao ){
		echo "<script>
			$('boalterou').value = 'false';
			
			var valorPag = 'valorigual_'+{$v['exfid']};
			var parcela = {$numparcela};
			
			//verifica se o valor da parcela e menor que o valor total do pagamento
			if($(valorPag).value == '1'){
				carregarParcela( '{$v['orbmesparcela']}', '{$v['orbanoparcela']}', '{$v['orbvalorparcela']}', '{$v['orbid']}', '{$v['exfid']}', '', '{$boDisabled}', parcela );
			}
			</script>";
		}
	}
	
?>
<div id="divDebug"></div>