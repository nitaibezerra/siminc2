<?php
if ($_REQUEST['titleFor']) {
	
	if(!$_SESSION['par_var']['proid']) die("Processo não encontrado");
	
	
	if( $_REQUEST['tp'] == 2 ){
		$sql = "SELECT 
					p.pagnumeroempenho as numero,
					--p.pagvalorparcela as valor,
					pobvalorpagamento as valor,
					p.pagsituacaopagamento as situacao
				FROM 
					par.processoobra po
				INNER JOIN par.empenho e on e.empnumeroprocesso = po.pronumeroprocesso and empstatus = 'A'
				LEFT JOIN par.pagamento p on p.empid = e.empid
				LEFT JOIN par.pagamentoobra pob on pob.pagid = p.pagid
				WHERE 
					p.pagstatus = 'A' AND
					po.prostatus = 'A' and
					e.empid != ".$_REQUEST['empid']." AND 
					pob.preid = ".$_REQUEST['titleFor']." AND 
					po.proid = ".$_SESSION['par_var']['proid'];

		$arEmpenho = $db->carregar( $sql );
		$arEmpenho = $arEmpenho ? $arEmpenho : array();								            
		$html = '<table>
					<tr>
						<th>Nº Empenho</th>
						<th> - Valor Pago</th>
						<th> - Situação</th>
					</tr>';
		if( $arEmpenho ){
			$excP = array();
			foreach ($arEmpenho as $empenho) {
				$html .= '<tr>
							<td>'.$empenho['numero'].'</td>
							<td align="right">'.number_format($empenho['valor'],2,',','.').'</td>
							<td align="right">&nbsp;&nbsp;'.$empenho['situacao'].'</td>
						</tr>';
			}
		} else {
			echo 'Sem pagamentos';
			die();
		}
		$html .= '</table>';
	} else {
		$sql = "SELECT 
					--p.pagvalorparcela as valor,
					pobvalorpagamento as valor,
					p.pagsituacaopagamento as situacao
				FROM 
					par.processoobra po
				INNER JOIN par.empenho e on e.empnumeroprocesso = po.pronumeroprocesso and empstatus = 'A'
				LEFT JOIN par.pagamento p on p.empid = e.empid
				LEFT JOIN par.pagamentoobra pob on pob.pagid = p.pagid
				WHERE 
					p.pagstatus = 'A' AND
					po.prostatus = 'A'  and
					e.empid = ".$_REQUEST['empid']." AND 
					pob.preid = ".$_REQUEST['titleFor']." AND 
					po.proid = ".$_SESSION['par_var']['proid'];

		$arEmpenho = $db->carregar( $sql );
		$arEmpenho = $arEmpenho ? $arEmpenho : array();								            
		$html = '<table>
					<tr>
						<th> Valor Pago</th>
						<th> - Situação</th>
					</tr>';
		if( $arEmpenho ){
			$excP = array();
			foreach ($arEmpenho as $empenho) {
				$html .= '<tr>
							<td align="right">'.number_format($empenho['valor'],2,',','.').'</td>
							<td align="right">&nbsp;&nbsp;'.$empenho['situacao'].'</td>
						</tr>';
			}
		} else {
			echo 'Sem pagamentos';
			die();
		}
		$html .= '</table>';
	}
	echo $html;
	die();
}

include '_funcoes_pagamento.php';

if($_REQUEST['requisicao']) {
	$_REQUEST['requisicao']($_REQUEST);
	exit;
}
/* configurações do relatorio - Memoria limite de 1024 Mbytes */
ini_set("memory_limit", "1024M");
set_time_limit(0);
/* FIM configurações - Memoria limite de 1024 Mbytes */

if($_REQUEST['proid']) {
	$_SESSION['par_var']['proid'] = $_REQUEST['proid']; 
}
if($_REQUEST['empnumeroprocesso']) {
	$_SESSION['par_var']['empnumeroprocesso'] = $_REQUEST['empnumeroprocesso']; 
}

if(!$_SESSION['par_var']['proid']){
	echo "<script>
			alert('Processo não encontrado');
			window.close();
		</script>"; 
	exit();
}

$processo = $_REQUEST['processo'];

$retorno = verificaEmpenhoValorDivergente($processo, '', 'PAC');

if( !empty($retorno[0]['processo']) ){
	die("<script>
			alert('Para realizar o pagamento deste processo é necessário ajustar os dados de empenho.');
			window.close();
		</script>");
}
// Monta tarja vermelha com aviso de pendência de obras
montarAvisoCabecalho(null, null, null, null, $_REQUEST['proid']);


?>
<html>
<head>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
</head>
<body>
<?php
monta_titulo( $titulo_modulo, '<p align=center><b>SELECIONE UM EMPENHO</b> para visualizar os dados do pagamento</p>' );
?>
	<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
	<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>
	<script type="text/javascript" src="./js/par.js"></script>
	<link href="../library/bootstrap-3.0.0/css/bootstrap.min.css" rel="stylesheet" media="screen">
	
	<style type="">
		.ui-dialog-titlebar{
	    	text-align: center;	    	
	    }
	</style>
	
	<script type="text/javascript" src="../includes/ModalDialogBox/modal-message.js"></script>
	<script type="text/javascript" src="../includes/ModalDialogBox/ajax-dynamic-content.js"></script>
	<script type="text/javascript" src="../includes/ModalDialogBox/ajax.js"></script>
	<script type="text/javascript" src="/includes/remedial.js"></script>
	<script type="text/javascript" src="/includes/superTitle.js"></script>
	<link rel="stylesheet" href="../includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
	<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />

	<script>

	function abrirDadosObras(preid, preano){
		window.open('par.php?modulo=principal/programas/proinfancia/popupProInfancia&acao=A&preid='+preid+'&ano='+preano, 'obras','fullscreen=yes,scrollbars=yes' )
	}
	
	function mostraPendencia(){
		$( "#dialog_pendencia" ).show();
		$( '#dialog_pendencia' ).dialog({
				resizable: false,
				width: 900,
				height: 600,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					'Fechar': function() {
						$( this ).dialog( 'close' );
					}
				}
		});
	}
	function mostraRestricoes(){
		$( "#dialog_restricoes" ).show();
		$( '#dialog_restricoes' ).dialog({
				resizable: false,
				width: 900,
				height: 600,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					'Fechar': function() {
						$( this ).dialog( 'close' );
					}
				}
		});
	}
	
	function carregarListaEmpenhoPagamento( empid ) {
		$.ajax({
				type: "POST",
				url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
				data: "requisicao=listaEmpenho&empid="+empid,
				async: false,
				success: function(msg){
					$('#listapagamento').html(msg);
				}
			}
		);
	}
	
	function verDadosPagamento(empid) {

		
		if( $('#preidobrid').val() == '' )
		{
			var urlRes = "par.php?modulo=principal/solicitacaoPagamento&acao=A&processo=<?php echo $processo; ?>";
			
		}
		else
		{
    		if($('#sldid').val() != '')
    		{
    			var urlRes = "par.php?modulo=principal/solicitacaoPagamento&acao=A&processo=<?php echo $processo; ?>&preidobrid=" + $('#preidobrid').val() + "&sldid="+ $('#sldid').val();
    		}
    		else
        	{
            	var urlRes = "par.php?modulo=principal/solicitacaoPagamento&acao=A&processo=<?php echo $processo; ?>&preidobrid=" + $('#preidobrid').val();
        	}
				
		}
		
		divCarregando();
		$.ajax({
			type: "POST",
			url: urlRes, 
			data: "requisicao=dadosPagamento&empid="+empid,
			async: false,
			success: function(msg){
				$('#dadospagamento').html(msg);
				divCarregado();
			}
		});
	}
	
	$(document).ready(function() {
		carregarListaEmpenhoPagamento();
		if( $('#totPagSemComp').val() > 0 ){
			redistribuirPagamentos();
		}
	});
	
	function marcarPreObra(obj) {
		if(obj.checked) {
			obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].className='normal';
			obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].disabled=false;
			obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].className='normal';
			obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].disabled=false;
		} else {
			obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].className='disabled';
			obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].disabled=true;
			obj.parentNode.parentNode.parentNode.cells[12].childNodes[0].value='';
			obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].className='disabled';
			obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].disabled=true;
			obj.parentNode.parentNode.parentNode.cells[11].childNodes[0].value='';
		}
	}
	
	function cacularValorPagamento(obj) {
		
		var tabela = obj.parentNode.parentNode.parentNode;
	
		var valorDigitado 			= parseFloat(replaceAll(replaceAll(obj.value, ".", ""),",", "."));
		var valorEmpenhoObra		= obj.parentNode.parentNode.cells[4].childNodes[0].value;
		var valorPago				= obj.parentNode.parentNode.cells[7].childNodes[2].value;
		var valorPagoNesteEmpenho	= obj.parentNode.parentNode.cells[6].childNodes[3].value;
		var totalObra				= obj.parentNode.parentNode.cells[5].childNodes[0].value;
	
		if(obj.id == 'valorpagamentoobra'){
			
			//Trata se valor pago maior que valor empenhado
			if( (parseFloat(valorDigitado)+parseFloat(valorPagoNesteEmpenho)) > parseFloat(valorEmpenhoObra) ){
				alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
				valorDigitado = parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho);
				if( valorDigitado < 0 ){
					valorDigitado = 0;
				}
				obj.value = mascaraglobal('[.###],##',valorDigitado.toFixed(2));
			}
			// FIM Trata se valor pago maior que valor empenhado
			
			// Trata se valor pago é maior que valor da Obra
			if( parseFloat(parseFloat(valorDigitado)+parseFloat(valorPago)) > parseFloat(totalObra) ) {
				alert('"Valor pagamento (R$)" + "Valor total pago (R$)" não pode ser maior que o "Valor da Obra (R$)"');
				if( (parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho)) > (parseFloat(totalObra)-parseFloat(valorPago)) ){
					valorDigitado = parseFloat(totalObra)-parseFloat(valorPago);
				}else{
					valorDigitado = parseFloat(valorEmpenhoObra)-parseFloat(valorPagoNesteEmpenho);
				}
				if( valorDigitado < 0 ){
					valorDigitado = 0;
				}
				obj.value = mascaraglobal('[.###],##',valorDigitado.toFixed(2));
			}
			// FIM Trata se valor pago é maior que valor da Obra
			
			var porcentPagamento = (parseFloat(valorDigitado) * 100) / parseFloat(totalObra);		
//			obj.parentNode.parentNode.cells[9].childNodes[0].value =replaceAll(porcentPagamento, ".", ",").toFixed(2);
			obj.parentNode.parentNode.cells[11].childNodes[0].value = replaceAll(porcentPagamento.toFixed(2), ".", ",");
			
		} else {
			
			var porcentDigitado = parseFloat(replaceAll(obj.value, ",", "."));
			// Trata se Maior q 100%
			if( parseFloat(porcentDigitado.toFixed(0)) > 100 ){
				porcentDigitado = '100';
				obj.value = '100';
			}
			// Fim Trata se Maior q 100%
	
			var valorPagamento = (porcentDigitado/100)*totalObra;
	
			//Trata se valor pago maoir que valor empenhado (%)
			if( parseFloat(parseFloat(valorPagamento)+parseFloat(valorPagoNesteEmpenho)) > parseFloat(valorEmpenhoObra) ){
				alert('"Valor pagamento (R$)" + "Valor pago nesse empenho(R$)" não pode ser maior que o "Valor Empenhado (R$)"');
				porcentDigitado = ((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100;
				if( porcentDigitado < 0 ){
					porcentDigitado = 0;
				}
				obj.value = porcentDigitado.toFixed(0);
			}
			// FIM Trata se valor pago maoir que valor empenhado
			
			// Trata se valor pago é maior que valor da Obra (%)
			if( parseFloat(porcentDigitado) > parseFloat( 100 - (parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100) ) ) {
				alert('"Valor pagamento (R$)" + "Valor total pago (R$)" não pode ser maior que o "Valor da Obra (R$)"');
				if( (((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100) > ((100-(parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100)) ) ){
					porcentDigitado = (100-(parseFloat(parseFloat(valorPago)/parseFloat(totalObra))*100) );
				}else{
					porcentDigitado = ((valorEmpenhoObra-valorPagoNesteEmpenho)/totalObra)*100;
				}
				if( porcentDigitado < 0 ){
					porcentDigitado = 0;
				}
				obj.value = porcentDigitado.toFixed(0);
			}
			// FIM Trata se valor pago é maior que valor da Obra (%)
			
			
			valorPagamento = (porcentDigitado/100)*totalObra;

			obj.parentNode.parentNode.cells[12].childNodes[0].value = mascaraglobal('[.###],##',valorPagamento.toFixed(2));
		
		}
	
		var totalPagamento=0;
		var falta=0;
		var valorTotalSeraPago = 0;
		var valorempenho = document.getElementById('valempid').value; //quanto falta pra pagar.
		var valorjaPago = tabela.rows[(tabela.rows.length)-3].cells[5].childNodes[0].value; //valor pago.

		for(var i=0;i<(tabela.rows.length)-3;i++) {
			if(tabela.rows[i].cells[12].childNodes[0].value) {
				totalPagamento += parseFloat(replaceAll(replaceAll(tabela.rows[i].cells[12].childNodes[0].value,".",""),",","."));
			}
		}
	
		var emp = parseFloat(mascaraglobal('[###].##',parseFloat(valorempenho).toFixed(2)));
		var tot = parseFloat(replaceAll(replaceAll(document.getElementById('valorpagamento').value,".",""),",","."));
		
		if(totalPagamento > 0) {
			document.getElementById('solicitar').disabled=false;
			document.getElementById('valorpagamento').value = mascaraglobal('[.###],##',totalPagamento.toFixed(2));
			falta = valorempenho - totalPagamento;
			
			valorTotalSeraPago = parseFloat(totalPagamento) + parseFloat(valorjaPago);
			if( parseFloat(falta.toFixed(0)) < 0 ){
				tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = '<span style="color:red;">Valor empenhado<br>ultrapassado em R$ '+mascaraglobal('[.###],##',falta.toFixed(2))+'</span>';
				document.getElementById('solicitar').disabled=true;
			} else {
				tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = mascaraglobal('[.###],##',falta.toFixed(2));
			}
		} else {
			document.getElementById('solicitar').disabled=true;
			document.getElementById('valorpagamento').value = '';
			tabela.rows[(tabela.rows.length)-2].cells[1].innerHTML = mascaraglobal('[.###],##',parseFloat(valorempenho).toFixed(2));
		}
	
	}
	
	function carregarHistoricoPagamento(pagid, obj) {
		
		var linha 	= $(obj).parent().parent();
		var tabela 	= $(linha).parentNode;
		
		if( $(obj).attr('title') == 'mais' ){
			$(obj).attr('title','menos');
			$(obj).attr('src','../imagens/menos.gif');
			$(linha).after('<tr><td colspan="10" id="td_'+pagid+'" >Carregando...</td></tr>');
			$.ajax({
				type: "POST",
				url: window.location.hef,
				data: "requisicao=listaHistoricoPagamento&pagid="+pagid,
				async: false,
				success: function(msg){
					$('#td_'+pagid).html(msg);
				}
			});
		} else {
			$(obj).attr('title','mais');
			$(obj).attr('src','../imagens/mais.gif');
			$(linha).next().remove();
		}
		
	}
	
	function cancelarPagamento(pagid, processo) {
		jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
		
		$( '#dialog-aut' ).hide();
		$( '#dialog-aut' ).dialog({
			resizable: false,
			width: 400,
			modal: true,
			show: { effect: 'drop', direction: "up" },
			buttons: {
				'Ok': function() {
					$( this ).dialog( 'close' );
					cancelarPagamentoWS(pagid, processo);
				},
				'Cancel': function() {
					$( this ).dialog( 'close' );
				}
			}
		});
	}
	
	function cancelarPagamentoWS(wspagid, wsprocesso) {
	
		var wsusuario = $('#wsusuario').val();
		var wssenha = $('#wssenha').val();
		
		if(!wsusuario){
			alert('Favor informar o nome de usuário!');
			return false;
		}
		if(!wssenha){
			alert('Favor informar a senha!');
			return false;
		}
		
		divCarregando();
		
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=cancelarPagamento&pagid="+wspagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
	   		async: false,
	   		success: function(msg){
	   	   		alert(msg);
				window.location.reload();
	   	   	}
		});
		
		divCarregado();
		
	}

	function verObrasPagamento(pagid)
	{
		
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=obrasPagamento&pagid="+pagid,
	   		async: false,
	   		success: function(msg){
	   			jQuery('.ui-icon ui-icon-closethick').hide();
	   			jQuery( '#dialog-aut' ).hide();
	   			jQuery('#dialog-confirm').attr('title', 'Obras do Pagamento');
	   			jQuery( '#dialog-confirm' ).hide();
	   			jQuery( "#dialog-confirm" ).html(msg);
	   			jQuery( "#dialog-confirm" ).dialog({
					resizable: true,
					height:400,
					width:800,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						"Fechar": function() {
							jQuery( this ).dialog( "close" );
						}					
					}
				});
	   		}
		});
		jQuery('.ui-dialog-titlebar-close').hide();	
	}
	
	function consultarPagamento(pagid, processo) {
		jQuery('input[type="text"], input[type="password"]').css('font-size', '14px');
		$( '#dialog-aut' ).hide();
		$( '#dialog-aut' ).dialog({
			resizable: false,
			width: 450,
			modal: true,
			show: { effect: 'drop', direction: "up" },
			buttons: {
				'Ok': function() {
					$( this ).dialog( 'close' );
					consultarPagamentoWS( pagid, processo );
				},
				'Cancel': function() {
					$( this ).dialog( 'close' );
					//window.location.reload();
				}
			}
		});
	}
	
	function consultarPagamentoWS( pagid, processo ) {
	
		var wsusuario = $('#wsusuario').val();
		var wssenha   = $('#wssenha').val();
		
		if(!wsusuario){
			alert('Favor informar o nome de usuário!');
			return false;
		}
		if(!wssenha){
			alert('Favor informar a senha!');
			return false;
		}
		
		divCarregando();
		
		$.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=consultarPagamento&pagid="+pagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
	   		async: false,
	   		success: function(msg){
	   			$('#dialog-popup-content').html(msg);
	   			$('#dialog-popup-content').attr('title', 'Consulta do Pagamento');
				divCarregado();
				$('#dialog-popup-content').dialog({
					resizable: false,
					height:300,
					width:500,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						"OK": function() {
							$( this ).dialog( "close" );
							window.location.reload();								
						}
						
					}
				});
			}
		});
		
		divCarregado();
	}
	
	function visPag(){
		var dados = $('#formPagamento').serialize();
	
		var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
		empid = empid.replace('=','');
		$('#dialog-popup-content').attr('title', 'Vizualizar Pagamento');
	
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
			data: "requisicao=solicitarPagamento&tipo=visualiza&"+dados,
			async: false,
			success: function(msg){
				$('#dialog-popup-content').html(msg);
				divCarregado();
				$('#dialog-popup-content').dialog({
					resizable: false,
					height:800,
					width:1200,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						"OK": function() {
							$( this ).dialog( "close" );
							window.location.reload();								
						}
						
					}
				});
			}
		});
	}
	
	function solPag() {
		var dados = $('#formPagamento').serialize();
		
		/*$.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=verificaExecucaoFisicaObras&"+dados,
			async: false,
			success: function(msg){
				
				$('.ui-icon ui-icon-closethick').hide();
				$( '#dialog-aut' ).hide();
				if( msg != '' ){
					$( '#dialog-aut' ).html(msg);
				}
				$( '#dialog-confirm' ).hide();
				$( '#dialog-aut' ).dialog({
					resizable: false,
					width: 450,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						'Ok': function() {
							$( this ).dialog( 'close' );
							solicitarPagamento();
						},
						'Cancel': function() {
							$( this ).dialog( 'close' );
						}
					}
				});
			}
		});
		jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');*/

		jQuery('.ui-icon ui-icon-closethick').hide();
		jQuery( '#dialog-aut' ).hide();
		jQuery( '#dialog-confirm' ).hide();
		jQuery( '#dialog-aut' ).dialog({
				resizable: false,
				width: 450,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					'Ok': function() {
						jQuery( this ).dialog( 'close' );
						solicitarPagamento();
					},
					'Cancel': function() {
						jQuery( this ).dialog( 'close' );
					}
				}
			});
		jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
	}
	
	function solicitarPagamento(){
		var form = document.getElementById('formPagamento');
		
		var usuario = document.getElementById('wsusuario').value;
		var senha   = document.getElementById('wssenha').value;
		
		if(!usuario) {
			alert('Favor informar o usuário!');
			return false;
		}
		
		if(!senha) {
			alert('Favor informar a senha!');
			return false;
		}
		
		var erroPreid = '';
		var erroDeferido = '';
		$('[name="preid[]"]').each(function(){		
			var porcent = $('[name="porcent['+$(this).val()+']"]').val();
			/*
			var sldidcheck = $('[name="sldid['+$(this).val()+'][]"]:enabled:checked').length;
			var boInput = $('input[name="sldid['+$(this).val()+'][]"]').length;
			
			if( $(this).attr('checked') == true && sldidcheck == 0 && boInput > 0 ){
				erroDeferido += '->'+$('#td_nomeobra_'+$(this).val()).html()+'\n';
			}
			*/
			if( $(this).attr('checked') == true && porcent == '' ){
				erroPreid += '->'+$('#td_nomeobra_'+$(this).val()).html()+'\n';
			}
		});
		
		if( erroPreid != '' ){
			alert('Nas obras abaixo não foram informadas o % Pagamento:\n'+erroPreid);
			return false;
		}
		
		if( erroDeferido != '' ){
			alert('Nas obras abaixo não foram informadas o % Deferido:\n'+erroDeferido);
			return false;
		}
		
		divCarregando();

		var dados = $('#formPagamento').serialize();

		var empid = dados.substring(dados.indexOf('='),dados.indexOf('&parcela'));
		empid = empid.replace('=','');

		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
			data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarPagamento&"+dados,
			async: false,
			success: function(msg){
				$('.ui-icon ui-icon-closethick').hide();
				$( '#dialog-aut' ).hide();
				$( '#dialog-confirm' ).hide();
	   			$( "#dialog-confirm" ).html(msg);
				$( "#dialog-confirm" ).dialog({
					resizable: false,
					height:300,
					width:500,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						"OK": function() {
							$( this ).dialog( "close" );
							window.location.reload();								
						}
						
					}
				});
			}
		});
		
		if(empid){
			carregarListaEmpenhoPagamento(empid);
		}
		carregarListaPagamentoEmpenho();
		
		//document.getElementById('div_auth').style.display='none';
		document.getElementById('solicitar').disabled = false;
		divCarregado();	
	}
	
	
	</script>
<?php 
$sql = "select count(p.pagid)
		from par.pagamento p
		  left join par.pagamentoobra o on p.pagid = o.pagid
		  inner join par.empenho e on e.empid = p.empid and e.empstatus = 'A'
		where p.pagstatus = 'A' and e.empnumeroprocesso = '$processo'
		group by e.empnumeroprocesso
		having count(o.pobid) = 0";
$totPagSemComp = $db->pegaUm($sql);

cabecalhoSolicitacaoEmpenho();

$retornoHistorico = exibirHistoricoSigef($processo);
atualizaBasePagamentoSigef( $retornoHistorico, $processo, $_REQUEST['proid'] );

$_REQUEST['tooid'] = 1;
include_once APPRAIZ."par/modulos/principal/pagamento/redistribuicaoPagamentoObras.inc";
include_once APPRAIZ."par/modulos/principal/pagamento/deparaCorrecaoPagamentoEmpenhoObras.inc";

$sql = "SELECT 
			   inu.inuid
		FROM par.processoobra p
		INNER JOIN territorios.municipio m ON m.muncod = p.muncod
		INNER JOIN par.instrumentounidade inu ON m.muncod = inu.muncod
		WHERE p.prostatus = 'A' and p.proid = {$_SESSION['par_var']['proid']}";

$arrDados = $db->pegaLinha( $sql );
							
$inuId = $arrDados['inuid'];

$today = date('Y-m-d');
if($inuId)
{
	$sqlRestricoes = 
		"SELECT 
                    res.resdescricao,
			tpr.tprdescricao as tipo_restricao,
			CASE WHEN res.restemporestricao THEN
				to_char(resdatainicio, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_inicio,
			
			CASE WHEN res.restemporestricao THEN
				to_char(resdatafim, 'DD/MM/YYYY')
			ELSE
				'-'
			END
			as data_fim                        
                FROM par.restricaoentidade re
                INNER JOIN par.restricaofnde res ON re.resid = res.resid
                INNER JOIN par.tiporestricaofnde tpr ON res.tprid = tpr.tprid
                LEFT JOIN workflow.documento doc ON doc.docid = re.docid 
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
                WHERE re.inuid = {$inuId} 
                AND res.resstatus = 'A'
                AND esd.esdid NOT IN (".ESDID_CONCLUIDO.")
                ";
	
	$arrRestricoes = $db->carregar($sqlRestricoes);
	$arrBloqueioObra = verificaBloqueioObras( $inuId );
	
	$sql = "SELECT rstdsc, foo.obrid, o.obrnome
			FROM ( SELECT DISTINCT
			            esd.esdid as res_estado,
			            obrid,
			            r.rstdsc
			       FROM obras2.restricao r
			       INNER JOIN workflow.documento doc ON doc.docid = r.docid
                   INNER JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
			       WHERE obrid in (select pr.obrid from par.processoobra p 
			                          inner join par.processoobraspaccomposicao pc on pc.proid = p.proid  and pc.pocstatus = 'A'
			                          inner join obras2.obras pr on pr.preid = pc.preid AND pr.obrstatus = 'A' AND pr.obridpai IS NULL
			                      where p.prostatus = 'A'  and p.proid = {$_SESSION['par_var']['proid']}
			                          and pr.obridpai is null) 
			                AND rstitem = 'R' AND rststatus = 'A'
			        ) as  foo
              inner join obras2.obras o on o.obrid = foo.obrid
			where res_estado not in (1497, 1142, 1143)";
	
	$arrRestricoesSup = $db->carregar($sql);
	$arrRestricoesSup = $arrRestricoesSup ? $arrRestricoesSup : array();

	if( (count($arrRestricoes) && is_array($arrRestricoes)) || ($arrBloqueioObra['boBloqueia']) || !empty($arrRestricoesSup[0]) ) 
	{
?>
<table id="listaRestricoes" align="center" border="0" style="border-color: #CCCCCC; border-right: 1px solid #CCCCCC;border-style: solid;border-width: 1px;   font-size: xx-small;text-decoration: none; width: 95%;" cellpadding="3" cellspacing="1">
	<thead>
		<tr>
			<td  colspan="4"> Restrições do município </td>
		</tr>
	</thead>
	<tbody>
<?php
		if(count($arrRestricoes) && is_array($arrRestricoes))
		{
			foreach($arrRestricoes as $kRes => $vRes )
			{
				echo "
					<tr style=\"color:red;\">
						<td style=\"width:40%;\" > {$vRes['resdescricao']} </td>
					</tr>
					";	
			}
		}
		if( $arrBloqueioObra['boBloqueia'] )
		{
			
			echo "
				<tr style=\"color:red;\">
					<td style=\"width:40%;\" >";
			echo '<span style="color: red;">Existem pendências no Sistema '.$arrBloqueioObra['sistema'].'.</span><a href="#" onclick="mostraPendencia();";><span style="color: blue; cursor:pointer"> Clique aqui para verificar as pendências.</span></a>';
			echo " 	</td>
				 </tr>
					";	
			
		}
		
		if( !empty($arrRestricoesSup[0]) )
		{
			
			echo "
				<tr style=\"color:red;\">
					<td style=\"width:40%;\" >";
			echo '<span style="color: red;">Existem restrições não superadas no monitoramento de obras 2.0.</span><a href="#" onclick="mostraRestricoes();";><span style="color: blue; cursor:pointer"> Clique aqui para verificar as pendências.</span></a>';
			echo " 	</td>
				 </tr>
					";	
			
		}
?>
	</tbody>
</table>
<br>
<?php 
	}
} ?>
<form id="formPagamento">
	<input type="hidden" name="ws_proid" id="ws_proid" value="<?=$_REQUEST['proid']; ?>" />
	<input type="hidden" name="totPagSemComp" id="totPagSemComp" value="<?=$totPagSemComp; ?>" />
	<input type="hidden" name="tiposistema" id="tiposistema" value="PAC" />
	<input type="hidden" id="preidobrid" name="preidobrid" value="<?php if($_REQUEST['preidobrid']){echo $_REQUEST['preidobrid'];}?>" />
	<input type="hidden" id="sldid" name="sldid" value="<?php if($_REQUEST['sldid']){echo $_REQUEST['sldid'];}?>" />
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td><input type="button" value="Redistribuir Pagamentos" name="btnredistribuirPagamento" onclick="redistribuirPagamentos()" /></td>
		</tr>
		<tr>
			<td id="listapagamento">Carregando...</td>
		</tr>
		<tr>
			<td id="dadospagamento"></td>
		</tr>
	</table>
</form>
<?php
$largura = "250px";
$altura = "120px";
/* Modal de Autenticação
 * Alterada em 03/12/2013 por Eduardo Duncie, a pedido de Thiago Tasca.
 * */

/*$usuario = 'MECTHIAGOBARBOSA';
$senha   = '45389408';*/
?>
<style>
	#listaRestricoes thead tr td
	{
		background-color: #F7F7F7;
		font-weight: bold;
		font-size: 12px;
		font-family: arial;
	}
	.popup_alerta{
			width:<?php echo $largura ?>;height:<?php echo $altura ?>;position:absolute;z-index:0;top:50%;left:50%;margin-top:-<?php echo $altura/2 ?>;margin-left:-<?php echo $largura/2 ?>;border:solid 2px black;background-color:#FFFFFF;display:none}
	.popup_alerta2{
			width:90%;height:90%;position:absolute;z-index:0;top:50%;left:30%;margin-top:-250;margin-left:-250;border:solid 2px black;background-color:#FFFFFF;display:none}
</style>
<div class="demo">
	<div id="dialog-confirm" title="Solicitação de Pagamento" style="overflow-x: auto; overflow-y: auto; width:100%; height:250px; font-size: 12px;"></div>
</div>

<div id="dialog-aut" title="Autenticação" style="display:none;">
	<table align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
			<tr>
				<td width="30%" style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Usuário:</td>
				<td>					
					<input type="text" title="Usuário" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
						onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$usuario ?>" size="23" id="wsusuario" name="wsusuario">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
				</td>
			</tr>
			<tr>
				<td style="text-align: right; font-size: 1.5em; color: #333333; font-weight: bold;">Senha:</td>
				<td>
					<input type="password" class="obrigatorio normal"  title="Senha" onblur="MouseBlur(this);" onmouseout="MouseOut(this);" 
						onfocus="MouseClick(this);this.select();" onmouseover="MouseOver(this);" value="<?=$senha ?>" size="23" id="wssenha" name="wssenha">
					<img border="0" title="Indica campo obrigatório." src="../imagens/obrig.gif">
					<input type="hidden" name="ws_pagid" id="ws_pagid" value="" />
					<input type="hidden" name="ws_processo" id="ws_processo" value="" />
				</td>
			</tr>
		</table>
	<div id="resposta" style="color: red; font-size: 0.9em"></div>
</div>
<div id="visXML" class="popup_alerta2 <?php echo $classeCSS ?>" >
	<div style="width:100%;text-align:right">
		<img src="../imagens/fechar.jpeg" title="Fechar" style="margin-top:5px;margin-right:5px;cursor:pointer" onclick="document.getElementById('visXML').style.display='none'" />
	</div>
	<div style="padding:5px;text-align:justify;">
		<table class="tabela" align="center" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td width="10%" class="SubtituloDireita">Dados do Pagamento:</td>
			<td id="visXMLDadosPagamento"></td>
		</tr>
		</table>
	</div>
</div>
<div id="debug"></div>
<div id="dialog-popup" style="display:none;">
	<div id="dialog-popup-content" >
	</div>
</div>
<div id="dialog_pendencia" title="Pendências <?=$arrBloqueioObra['sistema']; ?>" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt; color: red;">
	<?php
	if(is_array($arrBloqueioObra) && count($arrBloqueioObra))
	{
		foreach ($arrBloqueioObra['pendencia'] as $obra => $pendencia) {
			echo ($obra + 1).' - '.$pendencia;
		}	
	}
	?>
	</div>
</div>

<div id="dialog_desembolso" title="Solicitação de Desembolso" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt;" id="dialog_retorno"></div>
</div>

<div id="dialog_restricoes" title="Restrições não Superadas" style="display: none" >
	<div style="padding:5px;text-align:justify; font-size: 8pt; color: red;">
	<?php
	if(is_array($arrRestricoesSup) && count($arrRestricoesSup))
	{
		foreach ($arrRestricoesSup as $obra => $restricao) {
			echo ($obra + 1).' - Obra: ('. $restricao['obrid'] . ') ' . $restricao['obrnome'] . ' - Restrição: ' . $restricao['rstdsc'].'<br>';
		}	
	}
	?>
	</div>
</div>