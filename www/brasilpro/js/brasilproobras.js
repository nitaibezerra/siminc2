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
			url: "brasilpro.php?modulo=principal/obras/termoPac&acao=A",
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

function carregarListaEmpenho(processo, obj) {

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
	   		url: "brasilpro.php?modulo=principal/obras/empenhoPac&acao=A",
	   		data: "requisicao=listaEmpenhoProcesso&empnumeroprocesso="+processo,
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


function carregarHistoricoEmpenho(empid, obj) {

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
	   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
	   		data: "requisicao=listaHistoricoEmpenho&empid="+empid,
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

function carregarHistoricoPagamento(pagid, obj) {
	
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
			url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamento&acao=A",
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

	if( valorInformado > valorEmpenhado ){
		alert('O valor informado para empenho ultrapassa 100% do valor da obra.');
		linha.cells[5].childNodes[0].value = 0;
	}
	
}

function calculaEmpenho(obj) {
	var linha = obj.parentNode.parentNode;
	var valor = parseFloat(replaceAll(replaceAll(replaceAll(linha.cells[2].innerHTML, '<br>', ''), '.', ''), ',', '.'));
	var valorEmpenhado = linha.cells[3].childNodes[2].value;
	var total = valor*obj.value/100;
	//alert(total);
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


/* Função para subustituir todos */
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
		alert('Favor informar o usuário!');
		return false;
	}
	
	if(!senha) {
		alert('Favor informar a senha!');
		return false;
	}
	

	divCarregando();
	
	var dadosobras = $('#formpreobras').serialize();
	var filtrosobras = $('#formulario').serialize();
	

	$.ajax({
   		type: "POST",
   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
   		data: "wsusuario=" + usuario + "&wssenha=" + senha + "&requisicao=executarEmpenho&"+filtrosobras+'&'+dadosobras,
   		async: false,
   		success: function(msg){
   		alert(msg);
   		}
	});
	
	carregarListaPreObra();
	carregarListaEmpenhoProcesso();

	divCarregado();
	
}

function solicitarPagamento() {

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
	
	var mes = document.getElementById('mes').value;
	var ano   = document.getElementById('ano').value;
	
	if(!mes) {
		alert('Favor informar o mês!');
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
		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamento&acao=A",
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
	
	window.open('brasilpro.php?modulo=principal/obras/modeloTermoObra&acao=A&arPreid='+arPreid+'&muncod='+muncod+'&tipoobra='+tipoobra, 
		        'modelo', 
		        "height=600,width=400,scrollbars=yes,top=0,left=0" );
	
	carregarListaPreObraTermo(muncod,tipoobra);
	carregarListaTermos(muncod,proid);
	
	divCarregado();
	
}

function consultarTermo(terid) {
	
	window.open('brasilpro.php?modulo=principal/obras/gerarTermoObra&acao=A&requisicao=download&terid='+terid, 
	        	'modelo', 
				"height=600,width=400,scrollbars=yes,top=0,left=0" );
}

function consultarEmpenho(empid,processo) {

	document.getElementById('ws_usuario_consulta').value;
	document.getElementById('ws_senha_consulta').value;
	document.getElementById('ws_empid').value = empid;
	document.getElementById('ws_processo').value = processo;
	
	document.getElementById('div_auth_consulta').style.display = 'block';
	document.body.scrollTop = 0;
}

function cancelarEmpenho(empid,processo) {

	document.getElementById('ws_usuario_consulta').value;
	document.getElementById('ws_senha_consulta').value;
	document.getElementById('ws_empid').value = empid;
	document.getElementById('ws_processo').value = processo;
	
	document.getElementById('div_auth_cancela').style.display = 'block';
	document.body.scrollTop = 0;
}

function cancelarPagamento(pagid,processo) {

	document.getElementById('ws_usuario_consulta').value;
	document.getElementById('ws_senha_consulta').value;
	document.getElementById('ws_pagid').value = pagid;
	document.getElementById('ws_processo').value = processo;
	
	document.getElementById('div_auth_cancela').style.display = 'block';
	document.body.scrollTop = 0;
}

function consultarPagamento(pagid,processo) {

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
		alert('Favor informar o nome de usuário!');
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
   		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamento&acao=A",
   		data: "requisicao=consultarPagamento&pagid="+wspagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	
	carregarListaPagamentoEmpenho('',wsprocesso);
	
	divCarregado();
	
}


function consultarEmpenhoWS() {

	var wsusuario = document.getElementById('ws_usuario_consulta').value;
	var wssenha = document.getElementById('ws_senha_consulta').value;
	var wsempid = document.getElementById('ws_empid').value;
	var wsprocesso = document.getElementById('ws_processo').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usuário!');
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
   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
   		data: "requisicao=consultarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	
	carregarListaEmpenhoProcesso(wsempid,wsprocesso);
	
	divCarregado();
	
}

function cancelarPagamentoWS() {

	var wsusuario = document.getElementById('ws_usuario_cancela').value;
	var wssenha = document.getElementById('ws_senha_cancela').value;
	var wspagid = document.getElementById('ws_pagid').value;
	var wsprocesso = document.getElementById('ws_processo').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usuário!');
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
   		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamento&acao=A",
   		data: "requisicao=cancelarPagamento&pagid="+wspagid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	
	carregarListaPagamentoEmpenho('',wsprocesso);
	
	divCarregado();
	
}


function cancelarEmpenhoWS() {

	var wsusuario = document.getElementById('ws_usuario_cancela').value;
	var wssenha = document.getElementById('ws_senha_cancela').value;
	var wsempid = document.getElementById('ws_empid').value;
	var wsprocesso = document.getElementById('ws_processo').value;
	
	if(!wsusuario){
		alert('Favor informar o nome de usuário!');
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
   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
   		data: "requisicao=cancelarEmpenho&empid="+wsempid + "&wsusuario=" + wsusuario + "&wssenha=" + wssenha,
   		async: false,
   		success: function(msg){alert(msg);}
	});
	
	carregarListaEmpenhoProcesso(wsempid,wsprocesso);
	
	divCarregado();
	
}

function carregarListaPreObra() {
	$.ajax({
   		type: "POST",
   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
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

function carregarListaEmpenhoPagamento() {
	$.ajax({
		type: "POST",
		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamento&acao=A",
		data: "requisicao=listaEmpenho",
		async: false,
		success: function(msg){
		document.getElementById('listapagamento').innerHTML = msg;
	}
	});
	
}

function carregarListaPreObraTermo( muncod, tipoobra  ) {
	$.ajax({
		type: "POST",
		url: "brasilpro.php?modulo=principal/obras/gerarTermoObra&acao=A",
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

function carregarListaEmpenhoProcesso(empid,processo) {

	$.ajax({
   		type: "POST",
   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
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
	
}

function carregarListaPagamentoEmpenho(empid,processo) {
	
	$.ajax({
		type: "POST",
		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamento&acao=A",
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

function carregarListaTermos(muncod, proid) {
	
	$.ajax({
		type: "POST",
		url: "brasilpro.php?modulo=principal/obras/gerarTermoObra&acao=A",
		data: "requisicao=listaTermo&proid="+proid+"&muncod=" + muncod,
		async: false,
		success: function(msg){
			document.getElementById('listatermo').innerHTML = msg;
		}
	});
	
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

function carregarListaTermos2(muncod, proid, obj) {

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
	   		url: "brasilpro.php?modulo=principal/obras/termoPac&acao=A",
	   		data: "requisicao=listaTermo&proid="+ proid +"&muncod=" + muncod,
	   		async: false,
	   		success: function(msg){
	   		ncol.innerHTML="<div id='listatermo_" + muncod + "' >" + msg + "</div>";
	   		}
		});
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

function carregarObrasEmpenhadas(empid, obj) {

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
	   		url: "brasilpro.php?modulo=principal/obras/solicitacaoEmpenhoPac&acao=A",
	   		data: "requisicao=listaObrasEmpenhadas&empid="+empid,
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

function carregarListaPagamentoEmpenhoPar(empid,processo) {
	
	jQuery.ajax({
		type: "POST",
		url: "brasilpro.php?modulo=principal/obras/solicitacaoPagamentoPac&acao=A",
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

