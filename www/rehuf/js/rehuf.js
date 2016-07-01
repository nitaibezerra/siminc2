/*
 * Controle dos detalhes dos tipos de linhas
 * Função predominantemente utilizada na estrutura de tabelas (gerenciarestrutura.inc)
 */
function carregardetalheslinhas(acao) {
	document.getElementById('linhafixasemsub').style.display = 'none';
	document.getElementById('linhafixacomsub').style.display = 'none';
	switch(acao) {
		case 'fixassemsub':
		document.getElementById('linhafixasemsub').style.display = '';
		break;
		case 'fixascomsub':
		document.getElementById('linhafixacomsub').style.display = '';
		break;
		case 'dinacomopc':
		break;
	}
}
/*
 * Controle de ordenação de diversas tabelas (ordenação via ajax)
 */
function ordenar(indiceselecionado, acao) {
	switch(acao) {
		case 'subirsubitemindicador':
			ajaxatualizar("subitemindicadoratual="+indiceselecionado+"&requisicao=ordenarsubitemindicador","");
			ajaxatualizar('requisicao=carregarsubitensindicadores','listasubitens');
			break;
		case 'descersubitemindicador':
			ajaxatualizar("desitemindicadoratual="+indiceselecionado+"&requisicao=ordenarsubitemindicador","");
			ajaxatualizar('requisicao=carregarsubitensindicadores','listasubitens');
			break;
		case 'subirgrupoitem':
			if(indiceselecionado != 0) {
				ajaxatualizar("grupoitematual="+document.getElementById('selectgrupoitem').options[indiceselecionado].value+"&grupoitemir="+document.getElementById('selectgrupoitem').options[(indiceselecionado-1)].value+"&requisicao=ordenargrupoitem");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=grupoitem','grupoitem');
				document.getElementById('selectgrupoitem').value = document.getElementById('selectgrupoitem').options[(indiceselecionado-1)].value;
			}
			break;
		case 'descergrupoitem':
			if(indiceselecionado != document.getElementById('selectgrupoitem').options.length) {
				ajaxatualizar("grupoitematual="+document.getElementById('selectgrupoitem').options[indiceselecionado].value+"&grupoitemir="+document.getElementById('selectgrupoitem').options[(indiceselecionado+1)].value+"&requisicao=ordenargrupoitem");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=grupoitem','grupoitem');
				document.getElementById('selectgrupoitem').value = document.getElementById('selectgrupoitem').options[(indiceselecionado+1)].value;
			}
			break;
		case 'subiragrupamentolinha':
			if(indiceselecionado != 0) {
				ajaxatualizar("agpatual="+document.getElementById('selectagrupamentolinha').options[indiceselecionado].value+"&agpir="+document.getElementById('selectagrupamentolinha').options[(indiceselecionado-1)].value+"&requisicao=ordenaragrupamento");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=agrupamento&islinha=true&agpid='+document.getElementById('selectagrupamentolinha').options[document.getElementById('selectagrupamentolinha').selectedIndex].value,'agrupamentolinha');
				document.getElementById('selectagrupamentolinha').value = document.getElementById('selectagrupamentolinha').options[(indiceselecionado-1)].value;
			}
			break;
		case 'subiragrupamentocoluna':
			if(indiceselecionado != 0) {
				ajaxatualizar("agpatual="+document.getElementById('selectagrupamentocoluna').options[indiceselecionado].value+"&agpir="+document.getElementById('selectagrupamentocoluna').options[(indiceselecionado-1)].value+"&requisicao=ordenaragrupamento");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=agrupamento&agpid='+document.getElementById('selectagrupamentocoluna').options[document.getElementById('selectagrupamentocoluna').selectedIndex].value,'agrupamentocoluna');
				document.getElementById('selectagrupamentocoluna').value = document.getElementById('selectagrupamentocoluna').options[(indiceselecionado-1)].value; 
			}
			break;

		case 'subirsubcoluna':
			if(indiceselecionado != 0) {
				ajaxatualizar("colunaatual="+document.getElementById('selectsubcoluna').options[indiceselecionado].value+"&colunair="+document.getElementById('selectsubcoluna').options[(indiceselecionado-1)].value+"&requisicao=ordenarcoluna");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=subcoluna&agpid='+document.getElementById('selectagrupamentocoluna').options[document.getElementById('selectagrupamentocoluna').selectedIndex].value,'subcoluna');
				document.getElementById('selectsubcoluna').value = document.getElementById('selectsubcoluna').options[(indiceselecionado-1)].value;
			}
			break;
		case 'subirsublinha':
			if(indiceselecionado != 0) {
				ajaxatualizar("linhaatual="+document.getElementById('selectsublinha').options[indiceselecionado].value+"&linhair="+document.getElementById('selectsublinha').options[(indiceselecionado-1)].value+"&requisicao=ordenarlinha");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=sublinha&agpid='+document.getElementById('selectagrupamentolinha').options[document.getElementById('selectagrupamentolinha').selectedIndex].value,'sublinha');
				document.getElementById('selectsublinha').value = document.getElementById('selectsublinha').options[(indiceselecionado-1)].value;
			}
			break;
		case 'subircoluna':
			if(indiceselecionado != 0) {
				ajaxatualizar("colunaatual="+document.getElementById('selectcoluna').options[indiceselecionado].value+"&colunair="+document.getElementById('selectcoluna').options[(indiceselecionado-1)].value+"&requisicao=ordenarcoluna");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=coluna','coluna');
				document.getElementById('selectcoluna').value = document.getElementById('selectcoluna').options[(indiceselecionado-1)].value;
			}
			break;
		case 'subirlinha':
			if(indiceselecionado != 0) {
				ajaxatualizar("linhaatual="+document.getElementById('selectlinha').options[indiceselecionado].value+"&linhair="+document.getElementById('selectlinha').options[(indiceselecionado-1)].value+"&requisicao=ordenarlinha");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=linha','linha');
				document.getElementById('selectlinha').value = document.getElementById('selectlinha').options[(indiceselecionado-1)].value;
			}
			break;
		case 'descersubcoluna':
			if(indiceselecionado != document.getElementById('selectsubcoluna').options.length) {
				ajaxatualizar("colunaatual="+document.getElementById('selectsubcoluna').options[indiceselecionado].value+"&colunair="+document.getElementById('selectsubcoluna').options[(indiceselecionado+1)].value+"&requisicao=ordenarcoluna");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=subcoluna&agpid='+document.getElementById('selectagrupamentocoluna').options[document.getElementById('selectagrupamentocoluna').selectedIndex].value,'subcoluna');
				document.getElementById('selectsubcoluna').value = document.getElementById('selectsubcoluna').options[(indiceselecionado+1)].value;
			}
			break;
		case 'desceragrupamentocoluna':
			if(indiceselecionado != document.getElementById('selectagrupamentocoluna').options.length) {
				ajaxatualizar("agpatual="+document.getElementById('selectagrupamentocoluna').options[indiceselecionado].value+"&agpir="+document.getElementById('selectagrupamentocoluna').options[(indiceselecionado+1)].value+"&requisicao=ordenaragrupamento");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=agrupamento&agpid='+document.getElementById('selectagrupamentocoluna').options[document.getElementById('selectagrupamentocoluna').selectedIndex].value,'agrupamentocoluna');
				document.getElementById('selectagrupamentocoluna').value = document.getElementById('selectagrupamentocoluna').options[(indiceselecionado+1)].value;
			}
			break;
		case 'desceragrupamentolinha':
			if(indiceselecionado != document.getElementById('selectagrupamentolinha').options.length) {
				ajaxatualizar("agpatual="+document.getElementById('selectagrupamentolinha').options[indiceselecionado].value+"&agpir="+document.getElementById('selectagrupamentolinha').options[(indiceselecionado+1)].value+"&requisicao=ordenaragrupamento");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=agrupamento&islinha=true&agpid='+document.getElementById('selectagrupamentolinha').options[document.getElementById('selectagrupamentolinha').selectedIndex].value,'agrupamentolinha');
				document.getElementById('selectagrupamentolinha').value = document.getElementById('selectagrupamentolinha').options[(indiceselecionado+1)].value;
			}
			break;

		case 'descersublinha':
			if(indiceselecionado != document.getElementById('selectsublinha').options.length) {
				ajaxatualizar("linhaatual="+document.getElementById('selectsublinha').options[indiceselecionado].value+"&linhair="+document.getElementById('selectsublinha').options[(indiceselecionado+1)].value+"&requisicao=ordenarlinha");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=sublinha&agpid='+document.getElementById('selectagrupamentolinha').options[document.getElementById('selectagrupamentolinha').selectedIndex].value,'sublinha');
				document.getElementById('selectsublinha').value = document.getElementById('selectsublinha').options[(indiceselecionado+1)].value;
			}
			break;
		case 'descercoluna':
			if(indiceselecionado != document.getElementById('selectcoluna').options.length) {
				ajaxatualizar("colunaatual="+document.getElementById('selectcoluna').options[indiceselecionado].value+"&colunair="+document.getElementById('selectcoluna').options[(indiceselecionado+1)].value+"&requisicao=ordenarcoluna");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=coluna','coluna');
				document.getElementById('selectcoluna').value = document.getElementById('selectcoluna').options[(indiceselecionado+1)].value;
			}
			break;
		case 'descerlinha':
			if(indiceselecionado != document.getElementById('selectlinha').options.length) {
				ajaxatualizar("linhaatual="+document.getElementById('selectlinha').options[indiceselecionado].value+"&linhair="+document.getElementById('selectlinha').options[(indiceselecionado+1)].value+"&requisicao=ordenarlinha");
				ajaxatualizar('requisicao=buscardadostabela&tipodado=linha','linha');
				document.getElementById('selectlinha').value = document.getElementById('selectlinha').options[(indiceselecionado+1)].value;
			}
			break;
		
		break;
	}
}

function ajaxatualizar(params,iddestinatario, pai) {
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}else{
						document.getElementById(iddestinatario).innerHTML = resp.responseText;
					}	
				} 
			},
			onLoading: function(){
				if(iddestinatario != "") {
					if (typeof(pai) != "undefined"){
						window.opener.document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}else{
						document.getElementById(iddestinatario).innerHTML = 'Carregando...';
					}
				}	
			}
		});
}

function Excluir(url, msg) {
	if(confirm(msg)) {
		window.location = url;
	}
}

function validarFormularioCadastrarItens(form) {
	if(form.tpiid.value == "") {
		alert("'Tipo de item' é um campo obrigatório.");
		form.tpiid.focus();
		return false;
	}
	if(form.itmdsc.value == "") {
		alert("'Descrição do item' é um campo obrigatório.");
		form.itmdsc.focus();
		return false;
	}
	return true;
}

function abreobservacao(id) {
	window.open('verobservacoes.php?id='+id,'Observações','scrollbars=no,height=300,width=500,status=no,toolbar=no,menubar=no,location=no');
}

function calculalinharelatorio(campo) {
	// pegando a coluna
	var linha = campo.parentNode.parentNode.rowIndex;
	// pegando a tabela
	var tabela = campo.parentNode.parentNode.parentNode;
	var tot = 0;
	// REGRAS PARA SOMAR!
	for(var i=0; i < (tabela.rows[linha].cells.length-1); i++) {
		if(typeof(tabela.rows[linha].cells[i].childNodes[0].name)!="undefined" && 
		   tabela.rows[linha].cells[i].childNodes[0].value != "" && 
		   tabela.rows[linha].cells[i].childNodes[0].type == "hidden" && 
		   tabela.rows[linha].cells[i].childNodes[0].id.substr(0,14) != "totalizadoragp" && 
		   tabela.rows[linha].cells[i].childNodes[0].id != "totalizadorcoluna") {

		   if(tabela.rows[linha].cells[i].childNodes[0].name.substr(0,12) == "conteudoitem") {
		   	var totpar = replaceAll(tabela.rows[linha].cells[i].childNodes[0].value,".","");
			totpar = replaceAll(totpar,",","");
			tot = tot + parseFloat(totpar);
		   }
		}
	}
	if(tot != 0) {
		if(tabela.rows[linha].cells[tabela.rows[linha].cells.length-1].childNodes[0].id == "totalizadorcoluna") {
			tabela.rows[linha].cells[tabela.rows[linha].cells.length-1].childNodes[0].value = Arredonda(tot,2);
			tabela.rows[linha].cells[tabela.rows[linha].cells.length-1].childNodes[0].onkeyup();
		} else {
		// caso a última coluna seja de 'ações'
			tabela.rows[linha].cells[tabela.rows[linha].cells.length-2].childNodes[0].value = Arredonda(tot,2);
			tabela.rows[linha].cells[tabela.rows[linha].cells.length-2].childNodes[0].onkeyup();
		}
	} else {
		tabela.rows[linha].cells[tabela.rows[linha].cells.length-1].childNodes[0].value = "";
	}

}


/* Função para subustituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}

function Arredonda( valor , casas ){
   var novo = Math.round( valor * Math.pow( 10 , casas ) ) / Math.pow( 10 , casas );
   var c = novo.toString();
   if(c.indexOf('.') == -1) {
   		return novo;
   } else {
   		return novo.toFixed(casas);
   }

}

function calculalinhasub(linid,cols,agpid) {
	var tot = 0;
	for(i=0;i<cols.length;i++) {
		var totpar = replaceAll(document.getElementById('id_'+linid+'_'+cols[i]).value,".","");
		totpar = replaceAll(totpar,",",".");
		if(totpar) {
			tot = tot + parseFloat(totpar);
		}
	}
	if(tot) {
		document.getElementById('totalizadoragp'+linid+'_'+agpid).value = Arredonda(tot,2);
		document.getElementById('totalizadoragp'+linid+'_'+agpid).onkeyup();
	}
}

function validarFormacaoDirigentes(form) {
	if(form.elements['edtcurso'].value == '') {
		alert('"Nome do curso" é obrigatório.');
		return false;
	}
	if(form.elements['edtlocalcurso'].value == '') {
		alert('"Local do curso" é obrigatório.');
		return false;
	}
	if(form.elements['edtdtconclusaocurso'].value == '') {
		alert('"Data de conclusão do curso" é obrigatório.');
		return false;
	}
	if(!validaData(form.elements['edtdtconclusaocurso'])) {
		alert('"Data de conclusão do curso" é inválida.');
		return false;
	}

	if(form.elements['edtnrhorascurso'].value == '') {
		alert('"Número de horas" é obrigatório.');
		return false;
	}
	return true;

}


function verificaAlteracao() {
	if ( document.getElementById('alteracaodados').value == "1" ) {
			return 'Atenção. Existem dados do formulário que não foram guardados.';
	}
}

function abregrupoitem(cnt, tabtid) {
	var linha = cnt.parentNode.parentNode.parentNode;
	var tabela = cnt.parentNode.parentNode.parentNode.parentNode;
	if(tabela.rows.length != linha.rowIndex) {
		if(tabela.rows[linha.rowIndex].cells[1].id == "") {
			cnt.src = '../imagens/menos.gif';
			var linhan = tabela.insertRow(linha.rowIndex);
			var cell0 = linhan.insertCell(0);
			cell0.innerHTML = '&nbsp';
			var cell1 = linhan.insertCell(1);
			cell1.colSpan = 3;
			cell1.id = 'id_'+tabtid+'_'+linhan.rowIndex;
			cell1.innerHTML = 'Carregando...';
			ajaxatualizar('requisicao=buscargrupoitem&tabtid='+tabtid,'id_'+tabtid+'_'+linhan.rowIndex);
		} else {
			cnt.src = '../imagens/mais.gif';
			tabela.deleteRow(linha.rowIndex);
		}
	} else {
			cnt.src = '../imagens/menos.gif';
			var linhan = tabela.insertRow(linha.rowIndex);
			var cell0 = linhan.insertCell(0);
			cell0.innerHTML = '&nbsp';
			var cell1 = linhan.insertCell(1);
			cell1.colSpan = 3;
			cell1.id = 'id_'+tabtid+'_'+linhan.rowIndex;
			cell1.innerHTML = 'Carregando...';
			ajaxatualizar('requisicao=buscargrupoitem&tabtid='+tabtid,'id_'+tabtid+'_'+linhan.rowIndex);
	}
}

function abrehospoitaisporunidade(cnt, unidadeid) {
	var linha = cnt.parentNode.parentNode;
	var tabela = cnt.parentNode.parentNode.parentNode.parentNode;
	if(tabela.rows.length != linha.rowIndex+1) {
		if(tabela.rows[linha.rowIndex+1].cells[1].id == "") {
			cnt.src = '../imagens/menos.gif';
			var linhan = tabela.insertRow(linha.rowIndex+1);
			var cell0 = linhan.insertCell(0);
			cell0.innerHTML = '&nbsp';
			var cell1 = linhan.insertCell(1);
			cell1.colSpan = 5;
			cell1.id = 'id'+linhan.rowIndex;
			ajaxatualizar('requisicao=carregarhospitaisporunidade&unidadeid='+unidadeid,'id'+linhan.rowIndex);
		} else {
			cnt.src = '../imagens/mais.gif';
			tabela.deleteRow(linha.rowIndex+1);
		}
	} else {
			cnt.src = '../imagens/menos.gif';
			var linhan = tabela.insertRow(linha.rowIndex+1);
			var cell0 = linhan.insertCell(0);
			cell0.innerHTML = '&nbsp';
			var cell1 = linhan.insertCell(1);
			cell1.colSpan = 5;
			cell1.id = 'id'+linhan.rowIndex;
			ajaxatualizar('requisicao=carregarhospitaisporunidade&unidadeid='+unidadeid,'id'+linhan.rowIndex);
	}
}
function calculalinha(campo) {
	dadoscampo = campo.id.split("_");
	var linha = dadoscampo[1];
	var tot = 0;	
	var form  = document.getElementById("formulario");
	var totfloat = false;
	for(var i=0; i < form.elements.length; i++) {
		if(form.elements[i].id != "totalizadorcoluna" &&
		   form.elements[i].value!="" &&
		   form.elements[i].value!="0" &&
		   form.elements[i].type=="text" && 
		  (form.elements[i].id.search("id_"+dadoscampo[1])!= -1)) {
			if(form.elements[i].value.indexOf(',') != -1) {
				totfloat = true;
				var totpar = replaceAll(form.elements[i].value,".","");
				totpar = replaceAll(totpar,",",".");
				tot = tot + parseFloat(totpar);
			} else {
				tot = tot + parseFloat(form.elements[i].value);
			}
		}
	}
	if(totfloat) {
		tot = tot + parseFloat("0.00");
		document.getElementById("totalizadorcoluna_"+linha).value = Arredonda(tot,2);
	} else {
		document.getElementById("totalizadorcoluna_"+linha).value = tot;
	}
	document.getElementById("totalizadorcoluna_"+linha).onkeyup();

}
function calculacoluna(campo) {
	dadoscampo = campo.id.split("_");
	var coluna = dadoscampo[2];
	var tot = 0;	
	var form  = document.getElementById("formulario");
	var totfloat = false;
	for(var i=0; i < form.elements.length; i++) {
		dadoscampo = form.elements[i].id.split("_");
		if(form.elements[i].value!="" &&
		   form.elements[i].value!="0" &&
		   form.elements[i].type=="text" && 
		  (form.elements[i].id.search("id_"+dadoscampo[1]+"_"+coluna)!= -1)) {
			if(form.elements[i].value.indexOf(',') != -1) {
				if(document.getElementById("colunaacum_"+coluna)) {
					if(form.elements[i].id.search("id_"+dadoscampo[1]+"_"+coluna+"_"+document.getElementById("colunaacum_"+coluna).value)!= -1) {
						totfloat = true;
						var totpar = replaceAll(form.elements[i].value,".","");
						totpar = replaceAll(totpar,",",".");
						tot = tot + parseFloat(totpar);
					}
				} else {
					totfloat = true;
					var totpar = replaceAll(form.elements[i].value,".","");
					totpar = replaceAll(totpar,",",".");
					tot = tot + parseFloat(totpar);
				}
			} else {
				if(document.getElementById("colunaacum_"+coluna)) {
					if(form.elements[i].id.search("id_"+dadoscampo[1]+"_"+coluna+"_"+document.getElementById("colunaacum_"+coluna).value)!= -1) {
						tot = tot + parseFloat(form.elements[i].value);
					}
				} else {
					tot = tot + parseFloat(form.elements[i].value);
				}
			}
		}
	}
	if(totfloat) {
		var tots = tot.toString();
		if(tots.indexOf('.') == -1) {
			tot = parseFloat(tot+".00");
		}
		document.getElementById("totalizadorlinha_"+coluna).value = Arredonda(tot,2);
	} else {
		document.getElementById("totalizadorlinha_"+coluna).value = tot;
	}
	document.getElementById("totalizadorlinha_"+coluna).onkeyup();
	
}
function totalizadorgeral(campo) {
	var tot = 0;	
	var form  = document.getElementById("formulario");
	var totfloat = false;
	for(var i=0; i < form.elements.length; i++) {
		if(form.elements[i].value!="" &&
		   form.elements[i].value!="0" &&
		   form.elements[i].id.substr(0,17)=="totalizadorcoluna") {
			if(form.elements[i].value.indexOf(',') != -1) {
				totfloat = true;
				var totpar = replaceAll(form.elements[i].value,".","");
				totpar = replaceAll(totpar,",",".");
				tot = tot + parseFloat(totpar);
			} else {
				tot = tot + parseFloat(form.elements[i].value);
			}
		}
	}
	if(totfloat) {
		tot = tot + parseFloat("0.00");
		document.getElementById("totalizadorlinha_colunatotal").value = Arredonda(tot,2);
	} else {
		document.getElementById("totalizadorlinha_colunatotal").value = tot;
	}
	document.getElementById("totalizadorlinha_colunatotal").onkeyup();
}

function verificarhospitalmarcado(obj) {
	obj.parentNode.parentNode.onclick=null;
	if(document.getElementById('marcatodos').checked) {
		for(i=0;i<document.getElementsByName('situacao[]').length;i++) {
			document.getElementsByName('situacao[]')[i].checked=true;
		}
	} else {
		for(i=0;i<document.getElementsByName('situacao[]').length;i++) {
			document.getElementsByName('situacao[]')[i].checked=false;
		}
	}
}

function mudarsituacao() {
	if(document.getElementsByName('situacao[]').length > 0) {
		var marcado = false;
		var entids = '';
		for(i=0;i<document.getElementsByName('situacao[]').length;i++) {
			if(document.getElementsByName('situacao[]')[i].checked) {
				entids = entids+'&entid[]='+document.getElementsByName('situacao[]')[i].value;
				marcado = true;
			}
		}
		if(!marcado) {
			alert('Selecione os hospitais');
			return false;
		}
	}
	displayMessage(window.location.href+'&requisicao=confirmarmudancasitucao'+entids,false);
}

function carregarestrutura(pai) {
	var tabelasparam='';
	for(i=0;i<document.formulario.elements['tabelas[]'].length;i++) {
		if(document.formulario.elements['tabelas[]'][i].selected) {
			tabelasparam += 'tabtid[]='+document.formulario.elements['tabelas[]'][i].value+'&';
		}
	}
	var gruposparam='';
	for(i=0;i<document.formulario.elements['grupos[]'].length;i++) {
		if(document.formulario.elements['grupos[]'][i].selected) {
			gruposparam += 'gitid[]='+document.formulario.elements['grupos[]'][i].value+'&';
		}
	}
	var linhasparam='';
	for(i=0;i<document.formulario.elements['linhas[]'].length;i++) {
		if(document.formulario.elements['linhas[]'][i].selected) {
			linhasparam += 'linid[]='+document.formulario.elements['linhas[]'][i].value+'&';
		}
	}
	var colunasparam='';
	for(i=0;i<document.formulario.elements['colunas[]'].length;i++) {
		if(document.formulario.elements['colunas[]'][i].selected) {
			colunasparam += 'colid[]='+document.formulario.elements['colunas[]'][i].value+'&';
		}
	}
	
	switch(pai) {
		case 'tabelas':
			document.getElementById('tdgrupos').innerHTML = 'Carregando...';
		default:
			document.getElementById('tdlinhas').innerHTML = 'Carregando...';
			document.getElementById('tdcolunas').innerHTML = 'Carregando...';
			document.getElementById('tdlinhasdin').innerHTML = 'Carregando...';
			document.getElementById('tdperiodos').innerHTML = 'Carregando...';
	}
	
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: tabelasparam+gruposparam+linhasparam+colunasparam+'requisicao=carregarestruturavalores',
			asynchronous: false,
			onComplete: function(resp) {
				var selects = resp.responseText.split("||");
				switch(pai) {
					case 'tabelas':
						document.getElementById('tdgrupos').innerHTML = selects[0];
					default:
						document.getElementById('tdlinhas').innerHTML = selects[1];
						document.getElementById('tdcolunas').innerHTML = selects[2];
						document.getElementById('tdlinhasdin').innerHTML = selects[3];
						document.getElementById('tdperiodos').innerHTML = selects[4];
				}
				
			}
		});
}
function atualizarselect() {
	var selectpadrao="(SELECT COALESCE(SUM(ctivalor),0) FROM rehuf.conteudoitem cdi";
	/*
	 * VERIFICA SE EXISTE ALGUMA OPÇÃO MARCADA
	 */
	var opcid = new Array();
	countreg=0;
	for(i=0;i<document.formulario.elements['opcid[]'].length;i++) {
		if(document.formulario.elements['opcid[]'][i].selected) {
			opcid[countreg] = "'"+document.formulario.elements['opcid[]'][i].value+"'";
			countreg++;
		}
	}
	if(countreg > 0) {
		selectpadrao += " LEFT JOIN rehuf.linha lin ON lin.linid = cdi.linid";
	}
	selectpadrao += " WHERE cdi.esuid IN('{esuid}')";
	/*
	 * FIM
	 * VERIFICA SE EXISTE ALGUMA OPÇÃO MARCADA
	 */
	
	var linid = new Array();
	var countreg=0;
	/*
	 * Filtrando as linhas que serão buscadas no select
	 */
	for(i=0;i<document.formulario.elements['linhas[]'].length;i++) {
		if(document.formulario.elements['linhas[]'][i].selected) {
			linid[countreg] = "'"+document.formulario.elements['linhas[]'][i].value+"'";
			countreg++;
		}
	}
	if(linid.length > 0) {
		if(linid.length == 1) {
			selectpadrao += " AND cdi.linid="+linid[0];
		} else {
			var auxlin=" AND cdi.linid IN(";
			for(i=0;i<linid.length;i++) {
				auxlin += linid[i];
				if(i<(linid.length-1))auxlin +=",";
			}
			auxlin += ") ";
			selectpadrao += auxlin; 
		}
	}
	/*
	 * FIM
	 * Filtrando as linhas que serão buscadas no select
	 */
	 
	/*
	 * Filtrando as colunas que serão buscadas no select
	 */
	var colid = new Array();
	countreg=0;
	for(i=0;i<document.formulario.elements['colunas[]'].length;i++) {
		if(document.formulario.elements['colunas[]'][i].selected) {
			colid[countreg] = "'"+document.formulario.elements['colunas[]'][i].value+"'";
			countreg++;
		}
	}
	if(colid.length > 0) {
		if(colid.length == 1) {
			selectpadrao += " AND colid="+colid[0];
		} else {
			var auxcol=" AND colid IN(";
			for(i=0;i<colid.length;i++) {
				auxcol += colid[i];
				if(i<(colid.length-1))auxcol +=",";
			}
			auxcol+= ") ";
			selectpadrao += auxcol; 
		}
	}
	/*
	 * FIM
	 * Filtrando as opções que serão buscadas no select
	 */

	/*
	 * Filtrando as periodos que serão buscadas no select
	 */
	var perid = new Array();
	countreg=0;
	for(i=0;i<document.formulario.elements['periodos[]'].length;i++) {
		if(document.formulario.elements['periodos[]'][i].selected) {
			perid[countreg] = "'"+document.formulario.elements['periodos[]'][i].value+"'";
			countreg++;
		}
	}
	if(perid.length > 0) {
		if(perid.length == 1) {
			selectpadrao += " AND cdi.perid="+perid[0];
		} else {
			var auxper=" AND cdi.perid IN(";
			for(i=0;i<perid.length;i++) {
				auxper += perid[i];
				if(i<(perid.length-1))auxper +=",";
			}
			auxper+= ") ";
			selectpadrao += auxper; 
		}
	}
	/*
	 * FIM
	 * Filtrando as periodos que serão buscadas no select
	 */
	 
	/*
	 * Filtrando as colunas que serão buscadas no select
	 */
	var opcid = new Array();
	countreg=0;
	for(i=0;i<document.formulario.elements['opcid[]'].length;i++) {
		if(document.formulario.elements['opcid[]'][i].selected) {
			opcid[countreg] = "'"+document.formulario.elements['opcid[]'][i].value+"'";
			countreg++;
		}
	}
	if(opcid.length > 0) {
		if(opcid.length == 1) {
			selectpadrao += " AND lin.opcid="+opcid[0];
		} else {
			var auxopc=" AND lin.opcid IN(";
			for(i=0;i<opcid.length;i++) {
				auxopc += opcid[i];
				if(i<(opcid.length-1))auxopc +=",";
			}
			auxopc+= ") ";
			selectpadrao += auxopc; 
		}
	}
	/*
	 * FIM
	 * Filtrando as colunas que serão buscadas no select
	 */
	 
	selectpadrao += " AND ctiexercicio='{ano}')";
	
	if(selectpadrao) {
		document.getElementById('tdinvselect').innerHTML = selectpadrao;
	} else {
		document.getElementById('tdinvselect').innerHTML = "NA";
	}
	document.getElementById('invselect').value = selectpadrao;
}

function formataPrecoPregao(valor){
		
		var mascaraFinal = '###.###.##0,0000';
		var mascara = mascaraFinal.replace(/0/gi, '#'); 
        var mascara_utilizar;
        var mascara_limpa;
        var temp;
        var i;
        var j;
        var caracter;
        var separador;
        var dif;
        var validar;
        var mult;
        var ret;
        var tam;
        var tvalor;
        var valorm;
        var masct;
        tvalor = "";
        ret = "";
        caracter = "#";
        caracterObrigatorio = "0";
        separador = "|";
        mascara_utilizar = "";
        valor = trim(valor);
        if (valor == "")return valor;
        temp = mascara.split(separador);
        dif = 1000;

        valorm = valor;
        //tirando mascara do valor já existente
        for (i=0;i<valor.length;i++){
                if (!isNaN(valor.substr(i,1))){
                        tvalor = tvalor + valor.substr(i,1);
                }
        }
        valor = tvalor;
        valor = new Number(valor);
        valor = new String(valor);
        
        while(valor.length < 5)
        {
        	valor = "0"+valor;
        }
        
        //formatar mascara dinamica
        for (i = 0; i<temp.length;i++){
                mult = "";
                validar = 0;
                for (j=0;j<temp[i].length;j++){
                        if (temp[i].substr(j,1) == "]"){
                                temp[i] = temp[i].substr(j+1);
                                break;
                        }
                        if (validar == 1)mult = mult + temp[i].substr(j,1);
                        if (temp[i].substr(j,1) == "[")validar = 1;
                }
                for (j=0;j<valor.length;j++){
                        temp[i] = mult + temp[i];
                }
        }


        //verificar qual mascara utilizar
        if (temp.length == 1){
                mascara_utilizar = temp[0];
                mascara_limpa = "";
                for (j=0;j<mascara_utilizar.length;j++){
                        if (mascara_utilizar.substr(j,1) == caracter){
                                mascara_limpa = mascara_limpa + caracter;
                        }
                }
                tam = mascara_limpa.length;
        }else{
                //limpar caracteres diferente do caracter da máscara
                for (i=0;i<temp.length;i++){
                        mascara_limpa = "";
                        for (j=0;j<temp[i].length;j++){
                                if (temp[i].substr(j,1) == caracter){
                                        mascara_limpa = mascara_limpa + caracter;
                                }
                        }

                        if (valor.length > mascara_limpa.length){
                                if (dif > (valor.length - mascara_limpa.length)){
                                        dif = valor.length - mascara_limpa.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else if (valor.length < mascara_limpa.length){
                                if (dif > (mascara_limpa.length - valor.length)){
                                        dif = mascara_limpa.length - valor.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else{
                                mascara_utilizar = temp[i];
                                tam = mascara_limpa.length;
                                break;
                        }
                }
        }

        //validar tamanho da mascara de acordo com o tamanho do valor
        if (valor.length > tam){
                valor = valor.substr(0,tam);
        }else if (valor.length < tam){
                masct = "";
                j = valor.length;
                for (i = mascara_utilizar.length-1;i>=0;i--){
                        if (j == 0) break;
                        if (mascara_utilizar.substr(i,1) == caracter){
                                j--;
                        }
                        masct = mascara_utilizar.substr(i,1) + masct;
                }
                mascara_utilizar = masct;
        }

        //mascarar
        j = mascara_utilizar.length -1;
        for (i = valor.length - 1;i>=0;i--){
                if (mascara_utilizar.substr(j,1) != caracter){
                        ret = mascara_utilizar.substr(j,1) + ret;
                        j--;
                }
                ret = valor.substr(i,1) + ret;
                j--;
        }
        
        //alert(ret);
        var retornoFinal = "";
        var diferenca = mascaraFinal.length - ret.length;
        var limiteObrigatorio = mascaraFinal.indexOf('0');
        for (i = mascaraFinal.length - 1;i>=0;i--){
        	/*alert(i);
        	alert(mascaraFinal.substr(i,1));
        	
        	alert(i-diferenca);
        	alert(ret.substr(i-diferenca,1));
        	*/
        	
        	var caracterFinal = ""; 
	        	
        	if(i-diferenca >= 0){
	        	caracterFinal = ret.substr(i-diferenca,1);
	        }
	        else if (mascaraFinal.substr(i,1) == caracterObrigatorio){ 
        		caracterFinal = 0;
        	}
			
			retornoFinal = caracterFinal + retornoFinal;
        	
        	//alert('Final: '+retornoFinal);
        }
        
        return retornoFinal;
}

function rehuf_visualiza_itens(preid, entid, flpreco){
	var com='';
	if(flpreco) com = '&flpreco='+flpreco;
	window.location.href = 'rehuf.php?modulo=pregao/preenchimentoPregao&acao=A&preid='+preid+'&entid='+entid+com;
}

function inserirLinhaPlantao(params) {
	var tabela = document.getElementById('tablePlantao');
	tabela.rows[tabela.rows.length-2].cells[1].innerHTML="Aguade...";
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {

				if((tabela.rows.length % 12) == 0) {
					var nlin = tabela.insertRow(tabela.rows.length-2);
					var col0 = nlin.insertCell(0);
					col0.innerHTML = "&nbsp;";
					var col1 = nlin.insertCell(1);
					col1.innerHTML = "SIAPE";
					col1.className = "SubTituloCentro";
					var col2 = nlin.insertCell(2);
					col2.innerHTML = "NOME";
					col2.className = "SubTituloCentro";
					var col3 = nlin.insertCell(3);
					col3.innerHTML = "CARGO";
					col3.className = "SubTituloCentro";
					var colunas = resp.responseText.split("##");
					
					for(var i=4;i<colunas.length-6;i++) {
						var col = nlin.insertCell(i);
						col.align = "center";
						col.innerHTML = i-3;
						switch(colunas[i].substr(0,16)) {
							case 'background-color':
								col.bgColor = colunas[i].substr(17,7);
								break;
							case 'classname-cellsj':
								col.className = colunas[i].substr(17);
								break;
						}
					}
					
					var col4 = nlin.insertCell(i);
					i++;
					col4.align = "center";
					col4.innerHTML = "Presencial dias úteis";
					col4.bgColor = "#808080";
					var col5 = nlin.insertCell(i);
					i++;
					col5.align = "center";
					col5.innerHTML = "Presencial final de semana e feriados";
					col5.bgColor = "#808080";
					var col6 = nlin.insertCell(i);
					i++;
					col6.align = "center";
					col6.innerHTML = "Sobreaviso dias úteis";
					col6.bgColor = "#808080";
					var col7 = nlin.insertCell(i);
					i++;
					col7.align = "center";
					col7.innerHTML = "Sobreaviso final de semana e feriados";
					col7.bgColor = "#808080";
					var col8 = nlin.insertCell(i);
					i++;
					col8.align = "center";
					col8.innerHTML = "Presencial dias úteis";
					col8.bgColor = "#808080";
					var col9 = nlin.insertCell(i);
					i++;
					col9.align = "center";
					col9.innerHTML = "Presencial final de semana e feriados";
					col9.bgColor = "#808080";
				}
				
				var nlin = tabela.insertRow(tabela.rows.length-2);
				var colunas = resp.responseText.split("##");
				for(var i=0;i<colunas.length;i++) {
					var col = nlin.insertCell(i);
					switch(colunas[i].substr(0,16)) {
						case 'background-color':
							col.bgColor = colunas[i].substr(17,7);
							break;
						case 'classname-cellsj':
							col.className = colunas[i].substr(17);
							break;
						default:
							col.innerHTML = colunas[i];
					
					}
				}
			},
			onLoading: function(){}
		});
	tabela.rows[tabela.rows.length-2].cells[1].innerHTML="";
	
}

function selecionarFuncionarioPlantao(obj) {
	var linha = obj.parentNode.parentNode;
	var nlin = linha.rowIndex;
	var tabela = document.getElementById('tablePlantao');
	tabela.rows[tabela.rows.length-2].cells[1].innerHTML="Aguade...";
	
	var funcexiste = new Array();
	for(var i=3; i<tabela.rows.length-2; i++) {
		if(funcexiste[tabela.rows[i].cells[2].childNodes[0].value]=="sim" && tabela.rows[i].cells[2].childNodes[0].value != "") {
			alert("Duplicidade nos funcionários");
			obj.value = "";
			linha.cells[1].childNodes[0].value="";
			tabela.rows[tabela.rows.length-2].cells[1].innerHTML="";
			return false;
		} else {
			if(typeof(tabela.rows[i].cells[2].childNodes[0].type) != "undefined")
				funcexiste[tabela.rows[i].cells[2].childNodes[0].value]="sim";
		}
	}
	var dadosp = document.getElementById('periodo').value.split("-");
	var ano = dadosp[0];
	var mes = dadosp[1];
	params = "requisicao=selecionarFuncionarioPlantao&fcoid="+obj.value+"&mes="+mes+"&ano="+ano;
	
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				var colunas = resp.responseText.split("##");
				tabela.rows[nlin].cells[1].childNodes[0].value = colunas[0];
				tabela.rows[nlin].cells[3].innerHTML = colunas[1];
				for(var i=2;i<colunas.length;i++) {
					tabela.rows[nlin].cells[(i+2)].innerHTML = colunas[i];
				}

			},
			onLoading: function(){}
		});
		
	tabela.rows[tabela.rows.length-2].cells[1].innerHTML="";

}

function calcularPlantao(obj) {
	var linha = obj.parentNode.parentNode;
	var nlin = linha.rowIndex;
	var tabela = document.getElementById('tablePlantao');
	var PDTot = 0;
	var PFTot = 0;
	var SDTot = 0;
	var SFTot = 0;
	
	for(i=3;i<=(tabela.rows[nlin].cells.length-7);i++) {
		switch(tabela.rows[nlin].cells[i].childNodes[0].value) {
			case 'PD':
				PDTot = PDTot + 1;
				break;
			case 'PF':
				PFTot = PFTot + 1;
				break;
			case 'SD':
				SDTot = SDTot + 1;
				break;
			case 'SF':
				SFTot = SFTot + 1;
				break;
		}
	}
	switch(obj.id) {
		case 'epltipomedio':
			if(PDTot)tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-2)].innerHTML = PDTot;
			else tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-2)].innerHTML = "";
			if(PFTot)tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-1)].innerHTML = PFTot;
			else tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-1)].innerHTML = "";
			break;
		case 'epltiposuperior':
			if(PDTot)tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-6)].innerHTML = PDTot;
			else tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-6)].innerHTML = "";
			if(PFTot)tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-5)].innerHTML = PFTot;
			else tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-5)].innerHTML = "";
			if(SDTot)tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-4)].innerHTML = SDTot;
			else tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-4)].innerHTML = "";
			if(SFTot)tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-3)].innerHTML = SFTot;
			else tabela.rows[nlin].cells[(tabela.rows[nlin].cells.length-3)].innerHTML = "";
			break;

	}

}

function atualizarGridPlantao(valor) {
	if(document.getElementById('setid').value &&
	   document.getElementById('periodo').value) {
		document.getElementById('gridPlantao').innerHTML="Carregando...";
	   	var dadosp = document.getElementById('periodo').value.split("-");
	   	var ano = dadosp[0];
	   	var mes = dadosp[1];
		ajaxatualizar('requisicao=gridPlantao&setid='+document.getElementById('setid').value+'&mes='+mes+'&ano='+ano, 'gridPlantao');
		redimencionarBodyData();
	} else {
		document.getElementById('gridPlantao').innerHTML="";
		redimencionarBodyData();	
	}
}


function excluirLinhaPlantao(obj) {
	var nlinha = obj.parentNode.parentNode.rowIndex;
	var tabela = document.getElementById('tablePlantao').deleteRow(nlinha);
}

function validarFuncionarioPlantao() {
	if(document.getElementById('fcocodigosiape').value == '') {
		alert('Código do SIAPE é obrigatório');
		return false;
	}
	if(document.getElementById('fconome').value == '') {
		alert('Nome do funcionário é obrigatório');
		return false;
	}
 
	if(document.getElementById('carid').value == '') {
		alert('Cargo é obrigatório');
		return false;
	}
	return true;
}

function selecionarLinhaPlantaoSIAPE(obj) {
	
	params = "requisicao=selecionarLinhaPlantaoSIAPE&fcocodigosiape="+obj.value;
	
	if(obj.value.length != 0) {
		var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: params,
				asynchronous: false,
				onComplete: function(resp) {
					if(resp.responseText == "naoexiste") {
						alert("Funcionário não existe");
						obj.value="";
						return false;
					} else {
						if(resp.responseText == "entidadenaoexiste") {
							alert("Problemas na identificação do hospital, salve os registros e redirecione para a página inicial.");
							obj.value="";
							return false;
						} else {
							if(resp.responseText == "outrohospital") {
								alert("Funcionário cadastrado em outro hospital");
								obj.value="";
								return false;
							} else {
								var linha = obj.parentNode.parentNode;
								linha.cells[2].childNodes[0].value=resp.responseText;
								linha.cells[2].childNodes[0].onchange();
							}
						}
					}
	
				},
				onLoading: function(){}
			});
	}
}

function inserirFuncionarios(fcoid) {
	var param='';
	if(fcoid) {
		param='&fcoid='+fcoid;
	}
	window.open('?modulo=plantao/cadastrofuncionarios&acao=A&vis=edicao'+param,'Funcionario','scrollbars=no,height=300,width=500,status=no,toolbar=no,menubar=no,location=no');
}

function pesquisarhospital() {
	var params='';
	if(document.getElementById('unidadeid').value) {
		params+='&unidadeid='+document.getElementById('unidadeid').value;
	}
	if(document.getElementById('hospitalid').value) {
		params+='&hospitalid='+document.getElementById('hospitalid').value;
	}
	if(document.getElementById('aderiuebserh').value) {
		params+='&aderiuebserh='+document.getElementById('aderiuebserh').value;
	}
	if(document.getElementsByName('pes_agrupamento')[0].checked) {
		params+='&pes_agrupamento='+document.getElementsByName('pes_agrupamento')[0].value;
	} else if(document.getElementsByName('pes_agrupamento')[1].checked) {
		params+='&pes_agrupamento='+document.getElementsByName('pes_agrupamento')[1].value;
	}
	document.getElementById('unidadeid').disabled=true;
	document.getElementById('hospitalid').disabled=true;
	window.location='rehuf.php?modulo=inicio&acao=C'+params;
}

function vertodoshospitais() {
	document.getElementById('unidadeid').disabled=true;
	document.getElementById('hospitalid').disabled=true;
	window.location='rehuf.php?modulo=inicio&acao=C';
}

function inserirDetalhamento(obj,params) {

	var tabela = obj.parentNode.parentNode.parentNode.parentNode;
	var linha  = obj.parentNode.parentNode.parentNode;
	
	if(obj.title=="mais") {
		obj.title        = "menos";
		obj.src          = "../imagens/menos.gif";
		var nlinha       = tabela.insertRow(linha.rowIndex);
		var ncoluna0     = nlinha.insertCell(0);
		ncoluna0.id      = "id_coluna_"+linha.rowIndex;
		ncoluna0.colSpan = 7;
		
		document.getElementById("id_coluna_"+linha.rowIndex).innerHTML = "Carregando...";
		
		var myAjax = new Ajax.Request(
			window.location.href,
			{
				method: 'post',
				parameters: params,
				asynchronous: false,
				onComplete: function(resp) {
					document.getElementById("id_coluna_"+linha.rowIndex).innerHTML = resp.responseText;
				},
				onLoading: function(){}
			});

		
	} else {
		obj.title        = "mais";
		obj.src          = "../imagens/mais.gif";
		tabela.deleteRow(linha.rowIndex);
	}

}