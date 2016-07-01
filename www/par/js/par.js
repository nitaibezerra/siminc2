function carregarListaTermo(muncod, obj) { 
	
	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=7;
		ncol.innerHTML="Carregando...";
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/temroPac&acao=A",
			data: "requisicao=listaTermo&muncod="+muncod,
			async: false,
			success: function(msg){
			ncol.innerHTML="<div id='listatermoprocesso_" + muncod + "' >" + msg + "</div>";
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
	
}

function abretodos(obj){
	$.each($('input[name="chk[]"]'), function(i,v){
		if($(obj).attr('checked')){
			$(v).attr('checked', true);
			marcarChk(v);
		} else {
			$(v).attr('checked', false);
			marcarChk(v);
		}
	});
}

function calculaPercentualTodos(obj){
	var valor = obj.value;
	$.each($('input[name^="name_"]'), function(i,v){
		if($(v).attr('id') != 'id_total'){
			$(v).val();
			$(v).val(valor);
			calculaEmpenho(v);
			verificaPreenchimentoPorcentagem(v);
		}
	});
}

function carregarHistoricoEmpenhoFilhos(empnumerooriginalpai, obj) {

	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		var ncol1 = nlinha.insertCell(1);
		ncol1.colSpan=7;
		ncol.innerHTML="Carregando...";
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/solicitacaoEmpenho&acao=A",
	   		data: "requisicao=listaHistoricoEmpenhoFilhos&empnumerooriginalpai="+empnumerooriginalpai,
	   		async: false,
	   		success: function(msg){
				ncol.innerHTML='<img align="right" src="../imagens/seta_retorno.gif">';
				ncol1.innerHTML=msg;
	   		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}

}
 
function carregarHistoricoEmpenhoTelaDivergente(empid, processo, obj) {

	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=15;
		ncol.innerHTML="Carregando...";
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaEmpenhoPorProcessoTelaDivergente&empidpai="+empid+'&processo='+processo,
	   		async: false,
	   		success: function(msg){
				ncol.innerHTML=msg;
	   		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
}
 
function carregarHistoricoEmpenho(empid, processo, obj) {

	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=15;
		ncol.innerHTML="Carregando...";
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaEmpenhoPorProcesso&empidpai="+empid+'&processo='+processo,
	   		async: false,
	   		success: function(msg){
				ncol.innerHTML=msg;
	   		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
}

function carregarListaEmpenho(processo, obj, action) {

	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=7;
		ncol.innerHTML="Carregando...";
		jQuery.ajax({
	   		type: "POST",
	   		url: window.location.href,
	   		data: "requisicao=carregaEmpenhoPorProcesso&processo="+processo+"&action="+action,
	   		async: false,
	   		success: function(msg){
	   		ncol.innerHTML="<div id='listaempenhoprocesso_" + processo + "' >" + msg + "</div>";
	   		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
}

function visualizarHistorico(empid){
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenho&acao=A",
   		data: "requisicao=listaHistoricoEmpenho&empid="+empid,
   		async: false,
   		success: function(msg){
			document.getElementById('visHistorico').style.display='block';
			document.getElementById('visHistoricoDadosEmpenho').innerHTML=msg;
   		}
	});
}

function carregarHistoricoPagamento(pagid, obj) {
	
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=10;
		ncol.innerHTML="Carregando...";
		$.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=listaHistoricoPagamento&pagid="+pagid,
			async: false,
			success: function(msg){
			ncol.innerHTML=msg;
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
	
}

/*function carregarHistoricoPagamentoPar(pagid, obj) {
	
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=9;
		ncol.innerHTML="Carregando...";
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
			data: "requisicao=listaHistoricoPagamento&pagid="+pagid,
			async: false,
			success: function(msg){
			ncol.innerHTML=msg;
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
	
}*/

function carregarPagamento(empid, obj) {
	
	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=8;
		ncol.innerHTML="Carregando...";
		jQuery.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
			data: "requisicao=listaPagamento&empid="+empid,
			async: false,
			success: function(msg){
			ncol.innerHTML=msg;
		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}
	
}

function marcarChkObrasPar(obj) {
	var linha = obj.parentNode.parentNode;

	if(obj.checked) {
		document.getElementById('id_'+obj.value).className="normal";
		document.getElementById('id_'+obj.value).readOnly=false;
	} else {
		document.getElementById('id_'+obj.value).className="disabled";
		document.getElementById('id_'+obj.value).readOnly=true;
	}
	
	var tabela = obj.parentNode.parentNode.parentNode;
	var percPago = document.getElementById('porcentagem_'+obj.value).value;
	
	if( percPago > 0 ){
		var valorRestante = 100 - percPago;
		document.getElementById('id_'+obj.value).value = mascaraglobal('##,##',replaceAll(valorRestante.toFixed(2),'.',''));
		calculaEmpenho(document.getElementById('id_'+obj.value));
	}
	calcularTotal(tabela);
	
	//fa�o o AJAX para poder carregar o combo do Plano Interno
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
		data: "requisicao=carregarPlanoInterno&preid="+obj.value,
		async: false,
		success: function(msg){
			document.getElementById('planointernoSPAN').innerHTML=msg;
		}
	});
	
	filtraPTRESObrasPar( document.getElementById('planointerno').value, obj.value );
}

function marcarChk(obj) {
	var linha = obj.parentNode.parentNode;

	if(obj.checked) {
		document.getElementById('id_'+obj.value).className="normal";
		document.getElementById('id_'+obj.value).readOnly=false;
	} else {
		document.getElementById('id_'+obj.value).className="disabled";
		document.getElementById('id_'+obj.value).readOnly=true;
	}
	
	var tabela = obj.parentNode.parentNode.parentNode;
	calcularTotal(tabela);
}

function filtraPTRESObrasPar(plicod, preid) {

		if( preid ){
			dados = "requisicao=carregarPtres&plicod="+plicod+'&preid='+preid;
		} else {
			dados = "requisicao=carregarPtres&plicod="+plicod;
		}

		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
			data: dados,
			success: function(msg){
				document.getElementById("ptresSPAN").innerHTML = msg;
			}
		});
	}

function calcularTotal(tbl) {
	var total=0;
	for(var i=0;i<tbl.rows.length;i++) {
		var input = tbl.rows[i].cells[0].childNodes[0];
		if(input) {
			if(input.checked) {
				total = total + parseFloat(replaceAll(replaceAll(replaceAll(tbl.rows[i].cells[6].innerHTML, '<br>', ''), '.', ''), ',', '.'));
			}
		}
	}
	
	document.getElementById('id_total').value=mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
}

function verificaPreenchimentoPorcentagem(obj){
	var linha = obj.parentNode.parentNode;
	var valorEmpenhado = linha.cells[3].childNodes[2].value;
	var valorInformado = linha.cells[5].childNodes[0].value;
	var valor = 100 - valorEmpenhado; 

	if( parseFloat(Number(valorInformado).toFixed(2)) > parseFloat(Number(valor).toFixed(2)) ){
		alert('O valor informado para empenho ultrapassa 100% do valor da obra.');
		linha.cells[5].childNodes[0].value = mascaraglobal('###.###.###.###,##',replaceAll(Number(valor).toFixed(2),'.',''));
		var valorEmp = parseFloat(replaceAll(replaceAll(replaceAll(linha.cells[2].innerHTML, '<br>', ''), '.', ''), ',', '.'));
		var total = valorEmp*valor/100;
		linha.cells[6].innerHTML = mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
	}
	
}

function calculaEmpenho(obj) {
	var linha = obj.parentNode.parentNode;
	var valor = parseFloat(replaceAll(replaceAll(replaceAll(linha.cells[2].innerHTML, '<br>', ''), '.', ''), ',', '.'));
	var valorEmpenhado = linha.cells[3].childNodes[2].value;
	var total = valor*Number(replaceAll(obj.value, ',', '.'))/100;
	
	var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
	linha.cells[6].innerHTML = total_mac;
	linha.cells[5].childNodes[1].value = total;
	var tabela = obj.parentNode.parentNode.parentNode;
	calcularTotal(tabela);
}



function calculaEmpenhoSemNadaEmpenhado(obj) {
	var linha = obj.parentNode.parentNode;
	var valor = parseFloat(replaceAll(replaceAll(replaceAll(linha.cells[2].innerHTML, '<br>', ''), '.', ''), ',', '.'));
	var total = valor*obj.value/100;
	var total_mac = mascaraglobal('###.###.###.###,##',replaceAll(total.toFixed(2),'.',''));
	linha.cells[6].innerHTML = total_mac;
	linha.cells[5].childNodes[1].value = total;
	var tabela = obj.parentNode.parentNode.parentNode;
	calcularTotal(tabela);

}


/* Fun��o para subustituir todos */
function replaceAll(str, de, para) {
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}


function solicitarEmpenho() {

	var form = document.getElementById('formpreobras');
	
	var usuario = document.getElementById('wsusuario').value;
	var senha = document.getElementById('wssenha').value;
	var proid = document.getElementById('proid').value;
	var frpid = document.getElementById('frpid').value;
	var processo = document.getElementById('processo').value;
	var id_total = document.getElementById('id_total').value;
	
	var marcado=false;
	for(var i=0;i<form.elements.length;i++) {
		if(form.elements[i].type) {
			if(form.elements[i].type == "checkbox" && form.elements[i].checked == true) {
				marcado = true;
			}
		}
	}
	if(!marcado) {
		alert('Nenhuma obra selecionada');
		return false;
	}
	
	if(!usuario) {
		alert('Favor informar o usu�rio!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	
	if(frpid == '') {
		alert('Favor informar a Fonte de Recurso!');
		return false;
	}
	
	if(id_total == '' || id_total == '0,00' ) {
		alert('O valor total de empenho est� vazio. Por favor, informe a porcentagem de empenho para a obra selecionada!');
		return false;
	}
	

	divCarregando();
	
	var dadosobras = $('#formpreobras').serialize();
	var filtrosobras = $('#formulario').serialize();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenho&acao=A",
   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarEmpenho&proid="+proid+"&"+filtrosobras+'&'+dadosobras,
   		async: false,
   		success: function(msg){
   			//document.getElementById('debug').innerHTML = msg;
   			alert(msg);
   		}
	});
	
	carregarListaPreObra();
	carregarListaEmpenhoProcesso(processo);

	divCarregado();
	
}

function excluirPagamento(pagid, processo){
	if( confirm('Tem certeza que deseja excluir esse pagamento que ainda n�o obteve retorno do SIGEF?') ){
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "pagid=" + pagid + "&processo=" + processo + "&requisicao=excluirPagamento",
			async: false,
			success: function(msg){
				jQuery('.ui-icon ui-icon-closethick').hide();
				jQuery( '#dialog-aut' ).hide();
				jQuery( '#dialog-confirm' ).hide();
				jQuery( "#dialog-confirm" ).html(msg);
				jQuery( "#dialog-confirm" ).dialog({
					resizable: false,
					height:300,
					width:500,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
						"Fechar": function() {
							jQuery( this ).dialog( "close" );
							window.location.reload();								
						}
						
					}
				});
			}
		});
	}
}

function solicitarPagamento() {

	var usuario = document.getElementById('wsusuario').value;
	var senha   = document.getElementById('wssenha').value;
	
	if(!usuario) {
		alert('Favor informar o usu�rio!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	
	var mes = document.getElementById('mes').value;
	var ano   = document.getElementById('ano').value;
	
	if(!mes) {
		alert('Favor informar o m�s!');
		return false;
	}
	
	if(!ano) {
		alert('Favor informar o ano!');
		return false;
	}

	
	divCarregando();
	
	var dados = $('#formPagamento').serialize();
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarPagamento&"+dados,
		async: false,
		success: function(msg){
		alert(msg);
	}
	});
	document.getElementById('div_auth').style.display='none';
	verDadosPagamento($('#empid').val());
	divCarregado();
	
	
}

function gerarTermo( muncod, tipoobra, proid ) {
	
	var preids = document.getElementsByName('preids[]');
	var arPreid = '';
	for(x=0; x<=preids.length-1; x++){
		if(arPreid==''){
			arPreid = preids[x].value;
		}else{
			arPreid += ","+preids[x].value;
		}
	}

	if(arPreid=='') {
		alert('Nenhuma obra selecionada');
		return false;
	}
	
	
	divCarregando();
	
//	window.open('par.php?modulo=principal/modeloTermoObra&acao=A&arPreid='+arPreid+'&muncod='+muncod+'&tipoobra='+tipoobra, 
	window.open('par.php?modulo=principal/modeloTermoAditivoObra&acao=A&arPreid='+arPreid+'&muncod='+muncod+'&tipoobra='+tipoobra+'&proid='+proid+'&acaoorigem=GERAR', 
		        'modelo', 
		        "height=600,width=400,scrollbars=yes,top=0,left=0" );
	
	carregarListaPreObraTermo(muncod,tipoobra);
	carregarListaTermos(muncod,proid);
	
	divCarregado();
	
}

function consultarTermo(terid) {
	
	window.open('par.php?modulo=principal/gerarTermoObra&acao=A&requisicao=download&terid='+terid, 
	        	'modelo', 
				"height=600,width=400,scrollbars=yes,top=0,left=0" );
}

function consultarEmpenho(empid, processo) {

	/*document.getElementById('ws_usuario_consulta').value;
	document.getElementById('ws_senha_consulta').value;
	document.getElementById('ws_empid').value = empid;
	document.getElementById('ws_processo').value = processo;
	
	document.getElementById('div_auth_consulta').style.display = 'block';
	document.body.scrollTop = 0;*/
	
	document.getElementById('wsempid').value = empid;
	telaLogin( 'consultar' );
}

function cancelarEmpenho(empid, processo, especie) {

	/*document.getElementById('ws_usuario_consulta').value;
	document.getElementById('ws_senha_consulta').value;
	document.getElementById('ws_empid').value = empid;
	document.getElementById('ws_processo').value = processo;
	
	document.getElementById('div_auth_cancela').style.display = 'block';
	document.body.scrollTop = 0;*/
	document.getElementById('wsempid').value = empid;
	document.getElementById('processo').value = processo;
	document.getElementById('ws_especie').value = especie;
	
	telaLogin( 'cancelar' );
}

function cancelarEmpenhoParObras(empid,processo) {

	/*document.getElementById('wsusuario').value;
	document.getElementById('wssenha').value;
	document.getElementById('wsempid').value = empid;*/
	
	document.getElementById('wsempid').value = empid;
	telaLogin( 'cancelar' );
	
	/*document.getElementById('div_auth_cancela').style.display = 'block';
	document.body.scrollTop = 0;*/
}

/*function cancelarPagamento(pagid,processo) {

	document.getElementById('ws_usuario_consulta').value;
	document.getElementById('ws_senha_consulta').value;
	document.getElementById('ws_pagid').value = pagid;
	document.getElementById('ws_processo').value = processo;
	
	document.getElementById('div_auth_cancela').style.display = 'block';
	document.body.scrollTop = 0;
}*/

/*function consultarPagamento(pagid,processo) {

	document.getElementById('ws_usuario_consulta').value;
	document.getElementById('ws_senha_consulta').value;
	document.getElementById('ws_pagid').value = pagid;
	document.getElementById('ws_processo').value = processo;
	
	document.getElementById('div_auth_consulta').style.display = 'block';
	document.body.scrollTop = 0;
}

function consultarPagamentoWS() {

	var wsusuario = document.getElementById('ws_usuario_consulta').value;
	var wssenha = document.getElementById('ws_senha_consulta').value;
	var wspagid = document.getElementById('ws_pagid').value;
	var wsprocesso = document.getElementById('ws_processo').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	document.getElementById('div_auth_consulta').style.display = 'none';
	
	divCarregando();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
   		data: "requisicao=consultarPagamento&pagid="+wspagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	
	carregarListaPagamentoEmpenho('',wsprocesso);
	
	divCarregado();	
}*/

/*function consultarPagamentoWSPAR() {

	var wsusuario = document.getElementById('ws_usuario_consulta').value;
	var wssenha = document.getElementById('ws_senha_consulta').value;
	var wspagid = document.getElementById('ws_pagid').value;
	var wsprocesso = document.getElementById('ws_processo').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	document.getElementById('div_auth_consulta').style.display = 'none';
	
	divCarregando();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
   		data: "requisicao=consultarPagamento&pagid="+wspagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	
	//carregarListaPagamentoEmpenho('',wsprocesso);
	
	divCarregado();	
}*/

function consultarPagamento(pagid, processo) {
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
				consultarPagamentoWS( pagid, processo );
			},
			'Cancel': function() {
				jQuery( this ).dialog( 'close' );
				//window.location.reload();
			}
		}
	});
	jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
}

function consultarPagamentoWS( pagid, processo ) {

	var wsusuario = document.getElementById('wsusuario').value;
	var wssenha   = document.getElementById('wssenha').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	//document.getElementById('div_auth_consulta').style.display = 'none';
	
	divCarregando();
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=consultarPagamento&pagid="+pagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){
   			jQuery('.ui-icon ui-icon-closethick').hide();
   			jQuery( '#dialog-aut' ).hide();
   			jQuery( '#dialog-confirm' ).hide();
   			jQuery( "#dialog-confirm" ).html(msg);
   			jQuery( "#dialog-confirm" ).dialog({
				resizable: false,
				height:300,
				width:500,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					"OK": function() {
						jQuery( this ).dialog( "close" );
						window.location.reload();								
					}
					
				}
			});
		}
	});
	
	//carregarListaPagamentoEmpenho('',wsprocesso);
	
	divCarregado();
	
}

function consultarEmpenhoWS() {

	var wsusuario = document.getElementById('wsusuario').value;
	var wssenha = document.getElementById('wssenha').value;
	var wsempid = document.getElementById('wsempid').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	//document.getElementById('div_auth_consulta').style.display = 'none';
	
	divCarregando();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenho&acao=A",
   		data: "requisicao=consultarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){
   			alert(msg);
   		}
	});
	var processo = document.getElementById('processo').value;
	
	carregarListaEmpenhoProcesso(processo);
	
	divCarregado();
	
}
function consultarEmpenhoObrasParWS() {

	var wsusuario = document.getElementById('wsusuario').value;
	var wssenha = document.getElementById('wssenha').value;
	var wsempid = document.getElementById('wsempid').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	//document.getElementById('div_auth_consulta').style.display = 'none';
	
	divCarregando();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
   		data: "requisicao=consultarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){
   		//document.getElementById('debug').innerHTML = msg;
   		alert(msg);}
	});
	
	//carregarListaEmpenhoProcessoObrasPAR(wsempid,wsprocesso);
	var processo = document.getElementById('processo').value;
	carregarListaEmpenhoProcesso(processo);
	
	divCarregado();
	
}

function cancelarPagamento(pagid, processo) {
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
				cancelarPagamentoWS(pagid, processo);
			},
			'Cancel': function() {
				jQuery( this ).dialog( 'close' );
			}
		}
	});
	jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
}

function cancelarPagamentoWS(pagid, processo) {

	var wsusuario = document.getElementById('wsusuario').value;
	var wssenha   = document.getElementById('wssenha').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	divCarregando();
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=cancelarPagamento&pagid="+pagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){
   			jQuery('.ui-icon ui-icon-closethick').hide();
   			jQuery( '#dialog-aut' ).hide();
   			jQuery( '#dialog-confirm' ).hide();
   			jQuery( "#dialog-confirm" ).html(msg);
   			jQuery( "#dialog-confirm" ).dialog({
				resizable: false,
				height:300,
				width:500,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					"OK": function() {
						jQuery( this ).dialog( "close" );
						window.location.reload();								
					}
					
				}
			});
   		}
	});
	
	//carregarListaPagamentoEmpenho('',wsprocesso);
	
	divCarregado();
	
}

/*function cancelarPagamentoWS() {

	var wsusuario = document.getElementById('ws_usuario_cancela').value;
	var wssenha = document.getElementById('ws_senha_cancela').value;
	var wspagid = document.getElementById('ws_pagid').value;
	var wsprocesso = document.getElementById('ws_processo').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	document.getElementById('div_auth_cancela').style.display = 'none';
	
	divCarregando();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
   		data: "requisicao=cancelarPagamento&pagid="+wspagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	
	carregarListaPagamentoEmpenho('',wsprocesso);
	
	divCarregado();
	
}*/

/*function cancelarPagamentoWSPAR() {

	var wsusuario = document.getElementById('ws_usuario_cancela').value;
	var wssenha = document.getElementById('ws_senha_cancela').value;
	var wspagid = document.getElementById('ws_pagid').value;
	var wsprocesso = document.getElementById('ws_processo').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	document.getElementById('div_auth_cancela').style.display = 'none';
	
	divCarregando();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoPagamentoPar&acao=A",
   		data: "requisicao=cancelarPagamento&pagid="+wspagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	
	carregarListaPagamentoEmpenho('',wsprocesso);
	
	divCarregado();
	
}*/


function cancelarEmpenhoWS() {
	
	var wsusuario = document.getElementById('wsusuario').value;
	var wssenha = document.getElementById('wssenha').value;
	var wsempid = document.getElementById('wsempid').value;
	var proid = document.getElementById('proid').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	//document.getElementById('div_auth_cancela').style.display = 'none';
	
	divCarregando();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenho&acao=A",
   		data: "requisicao=cancelarEmpenho&proid="+proid+"&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	var processo = document.getElementById('processo').value;
	
	carregarListaPreObra();
	carregarListaEmpenhoProcesso(processo);
	
	divCarregado();
	
}

function cancelarEmpenhoObrasParWS() {

	var wsusuario = document.getElementById('wsusuario').value;
	var wssenha = document.getElementById('wssenha').value;
	var wsempid = document.getElementById('wsempid').value;
	var proid = document.getElementById('proid').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usu�rio!');
		return false;
	}
	if(!wssenha){
		alert('Favor informar a senha!');
		return false;
	}
	
	//document.getElementById('div_auth_cancela').style.display = 'none';
	
	divCarregando();
	
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
   		data: "requisicao=cancelarEmpenho&proid="+proid+"&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){
   		//document.getElementById('debug').innerHTML = msg; 
   		alert(msg);}
	});
	
	//carregarListaEmpenhoProcessoObrasPAR(wsempid,wsprocesso);
	var processo = document.getElementById('processo').value;;
	
	carregarListaObraPar(proid);
	carregarListaEmpenhoProcesso( processo );
	
	divCarregado();
	
}

function carregarListaPreObra() {
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenho&acao=A",
   		data: "requisicao=listaPreObras",
   		async: false,
   		success: function(msg){
   			document.getElementById('listapreobra').innerHTML = msg;
   			if(msg.search("checkbox") < 0 ){
   				document.getElementById('formulario').innerHTML = "";
   			}
   		}
	});
	
}

function carregarListaObraPar(proid) {
	var proid = proid;
	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
   		data: "requisicao=listaPreObras&proid="+proid,
   		async: false,
   		success: function(msg){
   			document.getElementById('listapreobra').innerHTML = msg;
   			if(msg.search("checkbox") < 0 ){
   				document.getElementById('formulario').innerHTML = "";
   			}
   		}
	});
	
}

/*function carregarListaEmpenhoProcessoObrasPAR(empid,processo) {

	$.ajax({
   		type: "POST",
   		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
   		data: "requisicao=listaEmpenhoProcesso&empid=" + empid + "&empnumeroprocesso=" + processo,
   		async: false,
   		success: function(msg){
   			if(!document.getElementById('listaempenhoprocesso')){
   				document.getElementById('listaempenhoprocesso_' + processo).innerHTML = msg;
   			}
   			else{
   				document.getElementById('listaempenhoprocesso').innerHTML = msg;
   			}
   		}
	});
	
}*/

function carregarListaEmpenhoPagamento() {
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
		data: "requisicao=listaEmpenho",
		async: false,
		success: function(msg){
		document.getElementById('listapagamento').innerHTML = msg;
	}
	});
	
}

function carregarListaPreObraTermo( muncod, tipoobra, estuf  ) {
	if(muncod){
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/gerarTermoObra&acao=A",
			data: "requisicao=listaPreObrasTermo&muncod="+muncod+"&tipoobra="+tipoobra,
			async: false,
			success: function(msg){
			document.getElementById('listapreobra').innerHTML = msg;
			if(msg.search("checkbox") < 0 ){
				document.getElementById('formulario').innerHTML = "";
			}
		}
		});
	}
	if(estuf){
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/gerarTermoObra&acao=A",
			data: "requisicao=listaPreObrasTermo&estuf="+estuf+"&tipoobra="+tipoobra,
			async: false,
			success: function(msg){
			document.getElementById('listapreobra').innerHTML = msg;
			if(msg.search("checkbox") < 0 ){
				document.getElementById('formulario').innerHTML = "";
			}
		}
		});
	}
}

function carregarListaEmpenhoProcesso(processo) {

	$.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=carregaEmpenhoPorProcesso&processo=" + processo,
   		async: false,
   		success: function(msg){
   			/*if(!document.getElementById('listaempenhoprocesso')){
   				document.getElementById('listaempenhoprocesso_' + processo).innerHTML = msg;
   			}
   			else{
   				document.getElementById('listaempenhoprocesso').innerHTML = msg;
   			}*/
   			document.getElementById('listaempenhoprocesso').innerHTML = msg;
   		}
	});
	
}

function carregarListaPagamentoEmpenho(empid,processo) {
	
	$.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
		data: "requisicao=listaPagamentoEmpenho&empid=" + empid + "&empnumeroprocesso=" + processo,
		async: false,
		success: function(msg){
		if(!document.getElementById('listapagamentoprocesso')){
			document.getElementById('listapagamentoprocesso_' + processo).innerHTML = msg;
		}
		else{
			document.getElementById('listapagamentoprocesso').innerHTML = msg;
		}
	}
	});
	
}

function carregarListaPagamentoEmpenhoPar(empid,processo) {
	
	jQuery.ajax({
		type: "POST",
		url: "par.php?modulo=principal/solicitacaoPagamento&acao=A",
		data: "requisicao=listaPagamentoEmpenhoPar&empid=" + empid + "&empnumeroprocesso=" + processo,
		async: false,
		success: function(msg){
		if(!document.getElementById('listapagamentoprocesso')){
			document.getElementById('listapagamentoprocesso_' + processo).innerHTML = msg;
		}
		else{
			document.getElementById('listapagamentoprocesso').innerHTML = msg;
		}
	}
	});
	
}

function carregarListaTermos(muncod, proid, estuf) {
	if(muncod){
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/gerarTermoObra&acao=A",
			data: "requisicao=listaTermo&proid="+proid+"&muncod=" + muncod,
			async: false,
			success: function(msg){
				document.getElementById('listatermo').innerHTML = msg;
			}
		});
	}
	if(estuf){
		$.ajax({
			type: "POST",
			url: "par.php?modulo=principal/gerarTermoObra&acao=A",
			data: "requisicao=listaTermo&proid="+proid+"&estuf=" + estuf,
			async: false,
			success: function(msg){
				document.getElementById('listatermo').innerHTML = msg;
			}
		});
	}
}

function excluirTermos(terid) {
	if(confirm('Deseja excluir o termo?')){
		var url = window.location;
		
		$.ajax({
			type: "POST",
			url: url,
			data: "requisicao=excluirTermo&terid=" + terid,
			async: false,
			success: function(msg){
			window.location = url;
		}
		});
	}
}

function carregarListaTermos2(muncod, estuf, proid, obj) {

	var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=7;
		ncol.innerHTML="Carregando...";
		if(muncod){
			$.ajax({
				type: "POST",
				url: "par.php?modulo=principal/termoPac&acao=A",
				data: "requisicao=listaTermo&proid="+ proid +"&muncod=" + muncod,
				async: false,
				success: function(msg){
				ncol.innerHTML="<div id='listatermo_" + muncod + "' >" + msg + "</div>";
			}
			});
		}
		if(estuf){
			$.ajax({
				type: "POST",
				url: "par.php?modulo=principal/termoPac&acao=A",
				data: "requisicao=listaTermo&proid="+ proid +"&estuf=" + estuf,
				async: false,
				success: function(msg){
				ncol.innerHTML="<div id='listatermo_" + muncod + "' >" + msg + "</div>";
			}
			});
		}
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}

}

function listaTermoHistorico(muncod, estuf, proid, obj) {

        var linha = obj.parentNode.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncolbranco = nlinha.insertCell(0);
		ncolbranco.innerHTML = '&nbsp;';
		var ncol = nlinha.insertCell(1);
		ncol.colSpan=7;
		ncol.innerHTML="Carregando...";
		if(muncod !== 'false'){
			$.ajax({
				type: "POST",
				url: "par.php?modulo=principal/termoPac&acao=A",
				data: "requisicao=listaTermoHistorico&proid="+ proid +"&muncod=" + muncod,
				async: false,
				success: function(msg){
				ncol.innerHTML="<div id='listatermo_" + muncod + "' >" + msg + "</div>";
			}
			});
		} else {
			$.ajax({
				type: "POST",
				url: "par.php?modulo=principal/termoPac&acao=A",
				data: "requisicao=listaTermoHistorico&proid="+ proid +"&estuf=" + estuf,
				async: false,
				success: function(msg){
				ncol.innerHTML="<div id='listatermo_" + estuf + "' >" + msg + "</div>";
			}
			});
		}
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}

}

function enviarAnexoTermo() {
	if(document.getElementById('arquivo').value == '') {
		alert('Selecione um arquivo');
		return false;
	}
	
	document.getElementById('formularioanexo').submit();

}

function carregarObrasEmpenhadas(empid, obj, proid) {

	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=8;
		ncol.innerHTML="Carregando...";
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/solicitacaoEmpenho&acao=A",
	   		data: "requisicao=listaObrasEmpenhadas&empid="+empid+"&proid="+proid,
	   		async: false,
	   		success: function(msg){
				ncol.innerHTML=msg;
	   		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}

}

function carregarObrasEmpenhadasPAR(empid, obj, proid) {

	var linha = obj.parentNode.parentNode;
	var tabela = obj.parentNode.parentNode.parentNode;
	
	if(obj.title == 'mais') {
		obj.title='menos';
		obj.src='../imagens/menos.gif';
		var nlinha = tabela.insertRow(linha.rowIndex);
		var ncol = nlinha.insertCell(0);
		ncol.colSpan=8;
		ncol.innerHTML="Carregando...";
		$.ajax({
	   		type: "POST",
	   		url: "par.php?modulo=principal/solicitacaoEmpenhoObrasPar&acao=A",
	   		data: "requisicao=listaObrasEmpenhadas&empid="+empid+"&proid="+proid,
	   		async: false,
	   		success: function(msg){
				ncol.innerHTML=msg;
	   		}
		});
	} else {
		obj.title='mais';
		obj.src='../imagens/mais.gif';
		var nlinha = tabela.deleteRow(linha.rowIndex);
	}

}

function tramitar(documento){
	var num_documenta = documento;
	
	if( num_documenta != '' ){
	
		var num_documenta_0 =  num_documenta.substring(0,7);
		var num_documenta_1 =  num_documenta.substring(7 );
		
		//num_documenta = num_documenta.split('/');
		var zero = '';
		if( num_documenta_0.length < 7 ){
			for(i=num_documenta_0.length; i<7; i++){
				zero = zero + '0';					
			}
			num_documenta_0 = zero+num_documenta_0;
		}
		num_documenta_1 = num_documenta_1.substr(0,4);
		window.open( "http://www.fnde.gov.br/pls/tramita_fnde/!tramita_fnde.tmtconrelatorio_pc?cha="+num_documenta_1 + num_documenta_0+"&usu=03468", '_blank');
	}
}

function historicoCancelamento(empid) {
	jQuery.ajax({
		type: "POST",
		url: window.location.href,
		data: "requisicao=historicoCancelamento&empid="+empid,
		async: false,
		success: function(msg){
			jQuery( "#div_dialog" ).show();
			jQuery( "#mostra_dialog" ).html(msg);
			jQuery( '#div_dialog' ).dialog({
					resizable: false,
					width: 900,
					modal: true,
					show: { effect: 'drop', direction: "up" },
					buttons: {
					'Fechar': function() {
						jQuery( this ).dialog( 'close' );
					}
				}
			});
		}
	});
}

function cadastraDocumenta( processo, pagina ){
	divCarregando();	
	displayMessage('par.php?modulo=principal/popupCadastraDocumenta&acao=A&processo='+processo+'&pagina='+pagina );	
	divCarregado();
}

function cadastraDocumenta( processo, pagina ){
	divCarregando();	
	displayMessage('par.php?modulo=principal/popupCadastraDocumenta&acao=A&processo='+processo+'&pagina='+pagina );	
	divCarregado();
}

function carregaTelaEmpenhoDivergente(){
	
	if( jQuery('[name="empdivergente"]').val() != '' ){
		jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=carregaTelaEmpenhoDivergente&processo="+jQuery('[name="empdivergente"]').val()+'&sistema='+jQuery('[name="tiposistema"]').val(),
			async: false,
			success: function(msg){
				var closeOnEscape = jQuery( "#div_divergente" ).dialog( "option", "closeOnEscape" );
				jQuery( "#div_divergente" ).show();
				jQuery( "#mostra_divergente" ).html(msg);
				jQuery( '#div_divergente' ).dialog({
						resizable: true,
						closeOnEscape: false,
						width: 1000,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
						'Salvar': function() {
							var vrlempenhado	= 0; 
							var vrlcomposicao	= 0;
							var vrldivergente	= 0;
							var notaempenho		= '';
							var erro 			= false;
							var mensagem 		= '';
							
							jQuery('[name="empenho[]"]').each(function(){
								var empenho = jQuery(this).val();
								vrlempenhado	= parseFloat(jQuery('[name="vrlempenhado['+empenho+']"]').val());
								vrlcomposicao	= parseFloat(jQuery('[name="vrlcomposicao['+empenho+']"]').val());
								vrldivergente	= jQuery('[name="vrldivergente['+empenho+']"]').val();
								notaempenho		= jQuery('[name="notaempenho['+empenho+']"]').val();
								
								vrldivergente = retiraPontos(vrldivergente);
								
								var empvalor = 0;
								var txtMsg = 'O valor informado n�o pode ser maior que o valor dispon�vel:';
								if ( typeof jQuery('[name="codigo['+empenho+'][]"]').val() !== "undefined" && jQuery('[name="codigo['+empenho+'][]"]').val()) {
									jQuery('[name="codigo['+empenho+'][]"]').each( function(){
										var codigo 			= jQuery(this).val();
										var valor			= jQuery('[name="empvalor['+empenho+']['+codigo+']"]').val();
										var descricao		= jQuery('[name="descricao['+empenho+']['+codigo+']"]').val();
										var valordisponivel	= parseFloat(jQuery('[name="valordisponivel['+empenho+']['+codigo+']"]').val());
										
										valor = retiraPontos(valor);
										
										if( Math.abs(parseFloat(valor)) > Math.abs(parseFloat(valordisponivel)) && parseFloat(valor) > 0   ){
											txtMsg = txtMsg + '\n Descri��o: \t\t\t'+codigo+' - '+descricao+' \n Valor Informado: \t\tR$ '+number_format(valor, 2, ',', '.' )+' \n Valor Dispon�vel: \t R$ '+number_format(valordisponivel, 2, ',', '.' )+' \n Nota de Empenho: \t '+notaempenho+' \n\n ';
											jQuery('[name="empvalor['+empenho+']['+codigo+']"]').val('0.00');
											
											jQuery('[id="tr_'+empenho+'_'+codigo+'"]').css('color', 'red');											
																						
											erro = true;
										}
										empvalor = parseFloat(empvalor) + parseFloat(valor);
									});
									
									if( erro == true ){
										alert(txtMsg);
									} else {
										jQuery('[id^="tr_'+empenho+'_"]').css('color', '#333333');
									}
									
									if( Math.abs(parseFloat(empvalor).toFixed(2)) > Math.abs(parseFloat(vrldivergente)) && erro == false ){
										mensagem = 'O valor da distibui��o n�o pode ser diferente do valor divergente: \n Valor Empenhado: R$ '+number_format(vrlempenhado, 2, ',', '.' )+' \n Valor Divergente: R$ '+number_format(vrldivergente, 2, ',', '.' )+' \n Valor Distribu�do: R$ '+number_format(empvalor, 2, ',', '.' )+' \n Nota de Empenho: '+notaempenho+'';
										alert(mensagem );
										erro = true;
										return false;
									}
								}
							});
							if( erro == false ){
								jQuery('[name="requisicao"]').val('salvarEmpenhoDivergente');
								jQuery('[name="formDivergente"]').submit();
							}
							return false;
						} ,
						'Fechar': function() {
							window.close();
							jQuery( this ).dialog( 'close' );
						}
					}
				});
			}
		});
		jQuery('.ui-dialog-titlebar-close').hide();
		jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
		
		calculaValorTotalDistribuido();
	}
}

function validaValorInformado( empenho, codigo ){	
	var valor			= jQuery('[name="empvalor['+empenho+']['+codigo+']"]').val();
	var valordisponivel	= parseFloat(jQuery('[name="valordisponivel['+empenho+']['+codigo+']"]').val());
	var notaempenho		= jQuery('[name="notaempenho['+empenho+']"]').val();
	var descricao		= jQuery('[name="descricao['+empenho+']['+codigo+']"]').val();
	
	valor = retiraPontos(valor);
	
	if( Math.abs(parseFloat(valor)) > Math.abs(parseFloat(valordisponivel)) && parseFloat(valor) > 0  ){
		alert('O valor informado n�o pode ser maior que o valor dispon�vel: \n Descri��o: '+codigo+' - '+descricao+' \n Valor Informado: R$ '+number_format(valor, 2, ',', '.' )+' \n Valor Dispon�vel: R$ '+number_format(valordisponivel, 2, ',', '.' )+' \n Nota de Empenho: '+notaempenho+'');
		jQuery('[name="empvalor['+empenho+']['+codigo+']"]').val('0,00');
		jQuery('[name="empvalor['+empenho+']['+codigo+']"]').focus();
		return false;
	}
	
	calculaValorTotalDistribuido();
}

function calculaValorTotalDistribuido(){
	
	var empenho 	 = '';
	var vrlempenhado = 0;
	var empvalor 	 = 0;
	var valortotal	 = 0;
	var vlrRestante  = 0;
	
	jQuery('[name="empenho[]"]').each(function(){
		empenho 	 = jQuery(this).val();
		
		vrlempenhado = parseFloat(jQuery('[name="vrlempenhado['+empenho+']"]').val());
		valortotal 	 = 0;
		vlrRestante  = 0;
		
		if ( typeof jQuery('[name="codigo['+empenho+'][]"]').val() !== "undefined" && jQuery('[name="codigo['+empenho+'][]"]').val()) {
			jQuery('[name="codigo['+empenho+'][]"]').each( function(){
				var codigo 	= jQuery(this).val();				
				var empvalor	= jQuery('[name="empvalor['+empenho+']['+codigo+']"]').val();
				var valor	= jQuery('[name="valordisponivel['+empenho+']['+codigo+']"]').val();
				
				if( valor == '' ) valor = '0.00';
				if( empvalor == '' ) empvalor = '0,00';
				
				empvalor = retiraPontos(empvalor);
				
				valortotal = (parseFloat(valortotal) /*+ parseFloat(valor))*/ + parseFloat(empvalor));
			});
		}
		//vlrRestante =  parseFloat(vrlempenhado) - parseFloat(empvalor);
		//vlrRestante =  parseFloat(valortotal);
		
		var vrldivergente	= jQuery('[name="vrldivergente['+empenho+']"]').val();
		
		vrldivergente = retiraPontos(vrldivergente);
		vlrRestante =  parseFloat(vrldivergente) - parseFloat(valortotal) ;
		
		jQuery('[name="vrltotalDistribuido['+empenho+']"]').val( number_format(valortotal, 2, ',', '.' ) );
		jQuery('[name="vrltotalRestante['+empenho+']"]').val( number_format(vlrRestante, 2, ',', '.' ) );
	});
}

function retiraPontos(v){
	if( v != 0 ){
		var valor = v.replace(/\./gi,"");
		valor = valor.replace(/\,/gi,".");
	} else {
		var valor = v;
	}
	
	return valor;
}

function adcionaSubacaoaoEmpenho(empenho, processo, tipo){
	window.open('par.php?modulo=principal/popupAddSubacaoEmpenho&acao=A&empenho='+empenho+'&processo='+processo+'&tipo='+tipo, 
	        'modelo', "height=600,width=900,scrollbars=yes,top=0,left=0" );
}

function reenviarPagamento(pagid, processo) {	
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
				var wsusuario = jQuery('[name="wsusuario"]').val();
				var wssenha   = jQuery('[name="wssenha"]').val();
				
				if(!wsusuario){
					alert('Favor informar o nome de usu�rio!');
					return false;
				}
				if(!wssenha){
					alert('Favor informar a senha!');
					return false;
				}
				jQuery( this ).dialog( 'close' );
				reenviarPagamentoWS(pagid, processo);
			},
			'Cancel': function() {
				jQuery( this ).dialog( 'close' );
			}
		}
	});
	jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');
}

function reenviarPagamentoWS(pagid, processo) {
	
	divCarregando();
	
	var wsusuario = jQuery('[name="wsusuario"]').val();
	var wssenha   = jQuery('[name="wssenha"]').val();
	var tiposistema   = jQuery('[name="tiposistema"]').val();
	
	jQuery.ajax({
   		type: "POST",
   		url: window.location.href,
   		data: "requisicao=reenviarPagamento&pagid="+pagid+"&wsusuario="+wsusuario+"&wssenha="+wssenha+"&processo="+processo+"&tiposistema="+tiposistema,
   		async: false,
   		success: function(msg){
   			jQuery('.ui-icon ui-icon-closethick').hide();
   			jQuery( '#dialog-aut' ).hide();
   			jQuery( '#dialog-confirm' ).hide();
   			jQuery( "#dialog-confirm" ).html(msg);
   			jQuery( "#dialog-confirm" ).dialog({
				resizable: true,
				height:400,
				width:600,
				modal: true,
				show: { effect: 'drop', direction: "up" },
				buttons: {
					"Fechar": function() {
						jQuery( this ).dialog( "close" );
						window.location.reload();								
					}					
				}
			});
   		}
	});
	jQuery('.ui-dialog-titlebar-close').hide();	
	divCarregado();	
}

function abrePlanoTrabalho(entidadePar, estuf, muncod, inuid){

	var data = new Array();
		data.push({name : 'requisicao', value : 'verificaInstrumentoUnidade'},
				  {name : 'entidadePar', value : entidadePar},
				  {name : 'estuf', value : estuf},
				  {name : 'muncod', value : muncod},
				  {name : 'inuid', value : inuid}
				 );

		jQuery.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(msg){
									var inuid = msg;
									if(inuid > 0){
										return jQuery(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=arvore');
										//return jQuery(location).attr('href', 'par.php?modulo=principal/planoTrabalho&acao=A&tipoDiagnostico=programa');
									}
					  }
		 });

}

function abreExecucaoOrcamento(entidadePar, estuf, muncod, inuid){

	var data = new Array();
		data.push({name : 'requisicao', value : 'verificaInstrumentoUnidade'},
				  {name : 'entidadePar', value : entidadePar},
				  {name : 'estuf', value : estuf},
				  {name : 'muncod', value : muncod},
				  {name : 'inuid', value : inuid}
				 );

		jQuery.ajax({
		   type		: "POST",
		   url		: "ajax.php",
		   data		: data,
		   async    : false,
		   success	: function(msg){
			   var inuid = msg;
			   if(inuid > 0){
				   return jQuery(location).attr('href', 'par.php?modulo=principal/administracaoDocumentos&acao=A');
			   }
		   }
		 });

}

function abrirDadosNutricionista( cpf ){
	
	/*jQuery( '#div_nutricionista' ).hide();
	jQuery( '#mostra_nutricionista' ).html( jQuery(location).attr('href', 'par.php?modulo=principal/cadastroNutricionista&acao=A') );
	jQuery( '#div_nutricionista' ).dialog({
		resizable: false,
		width: 450,
		modal: true,
		show: { effect: 'drop', direction: "up" },
		buttons: {
			'Fechar': function() {
				jQuery( this ).dialog( 'close' );
			}
		}
	});
	jQuery('input[type="text"], input[type="button"], input[type="password"]').css('font-size', '14px');*/
	
	window.open('par.php?modulo=principal/popUpDadosNutricionista&acao=A&cpf='+cpf+'&popup=S', 
	        'modelo', 
	        "height=800,width=1000,scrollbars=yes,top=0,left=0" );
}

function abrirSolicitacaoDesembolso( obrid ){
	jQuery.ajax({
			type: "POST",
			url: window.location.href,
			data: "requisicao=listaSolicitacaoDesembolso&obrid="+obrid,
			async: false,
			success: function(msg){				
	 
				jQuery( "#dialog_desembolso" ).show();
				jQuery('#dialog_retorno').html(msg);
				jQuery( '#dialog_desembolso' ).dialog({
						resizable: false,
						width: 900,
						height: 400,
						modal: true,
						show: { effect: 'drop', direction: "up" },
						buttons: {
							'Fechar': function() {
								jQuery( this ).dialog( 'close' );
							}
						}
				});
			}
		}
	);
}