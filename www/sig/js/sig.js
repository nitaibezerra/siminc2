function ajaxcombobox(params,iddestinatario) {	
	
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				document.getElementById(iddestinatario).innerHTML = 'Carregando...';
			}
		});
}

function ajaxatualizar(params,iddestinatario) {	
	
	var myAjax = new Ajax.Request(
		window.location.href,
		{
			method: 'post',
			parameters: params,
			asynchronous: false,
			onComplete: function(resp) {
				//alert(resp.responseText);
				if(iddestinatario) {
					document.getElementById(iddestinatario).innerHTML = resp.responseText;
				} 
			},
			onLoading: function(){
				document.getElementById(iddestinatario).innerHTML = 'Carregando...';
			}
		});
}


function Excluir(url, msg) {
	if(confirm(msg)) {
		window.location = url;
	}
}

function validarFormularioCadastrarCampus() {
	if(document.getElementById('unidades').value == "") {
		alert("'Instituição ' é um campo obrigatório.");
		document.getElementById('unidades').focus();
		return false;
	}
	if(document.getElementById('cmpid').value == "") {
		alert("'Campus ' é um campo obrigatório.");
		document.getElementById('cmpid').focus();
		return false;
	}
	document.getElementById('formulario').submit();
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
function downloadfileszip() {
	window.open('../geral/downloadfileszip.php','Observações','scrollbars=no,height=200,width=500,status=no,toolbar=no,menubar=no,location=no');
}

function abreobservacao(id) {
	window.open('verobservacoes.php?id='+id,'Observações','scrollbars=no,height=200,width=500,status=no,toolbar=no,menubar=no,location=no');
}

function removeAllOptions(selectbox)
{
	var i;
	for(i=selectbox.options.length-1;i>=0;i--)
	{
	selectbox.remove(i);
	}
}

function inserirCursos(cmpid){
	return windowOpen('?modulo=principal/inserir_cursos&acao=A&cmpid='+cmpid,'blank','height=600,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}


function listarUnidades(tpeid) {
	document.formulario.estuf.value = ''; 
	document.formulario.mundescricao.value = '';
	campus  = document.getElementById('campus');
	campus.options[0].selected = true;
	campus.disabled = true;
	unidades  = document.getElementById('unidades');			 	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('sig.php?modulo=principal/editarcampus&acao=A', {
							        method:     'post',
							        parameters: '&tpeid=' + tpeid +  '&exec_function=listarUnidadesAjax',
							        onComplete: function (res)
							        {	
										unidades.innerHTML = res.responseText;										
							        }
							  });
}

function listarCampus(entid_unidade) {
	campus  = document.getElementById('campus');
	campus.disabled = false;
	document.formulario.estuf.value = ''; 
	document.formulario.mundescricao.value = '';
	document.formulario.entuniorcid.value = entid_unidade;
	document.formulario.pesquisar_entidadeuo.disabled = true;
	document.formulario.pesquisar_entidadecm.disabled = true;
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('sig.php?modulo=principal/editarcampus&acao=A', {
							        method:     'post',
							        parameters: '&entid_unidade=' + entid_unidade + '&exec_function=listarCampusAjax',
							        onComplete: function (res)
							        {	
							        	//alert(res.responseText);return;
										campus.innerHTML = res.responseText;										
							        }
							  });
}

function listarMunicipios(cmpid, tpeid) {
	
	estuf 	= document.formulario.estuf; 	
	muncod  = document.formulario.mundescricao; 	
	
	document.formulario.estuf.value = ''; 
	document.formulario.mundescricao.value = '';

	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('sig.php?modulo=principal/editarcampus&acao=A', {
							        method:     'post',
							        parameters:  '&cmpid=' + cmpid + '&exec_function=listarMunicipioAjax',
							        onComplete: function (res)
							        {								        	
							        	var rs = res.responseText.split("|");							        	
										estuf.value = rs[0];	
										muncod.value = rs[1];										
							        }
							  });
}


function listarCampusCadastro(unicod, funid) {
	td_campus 	   = document.getElementById('nome_campus');	
	select_campus  = document.getElementById('cmpid');	
	estuf 	= document.formulario.estuf.value = ''; 	
	muncod  = document.formulario.muncod.value = '';
	var sfunid="";
	if(funid.length > 0) {
		for(i=0;i<funid.length;i++) {
			sfunid += '&funid['+i+']='+funid[i];
		}
	} else {
		sfunid = '&funid='+funid;
	} 	
	// Torna invisivel o <select>, caso exista
	if (select_campus) select_campus.style.visibility = 'hidden';
	// Faz uma requisição ajax, via POST
	var req = new Ajax.Request('sig.php?modulo=principal/cadastrarcampus&acao=A', {
							        method:     'post',
							        parameters: '&unicod=' + unicod + '&exec_function=listarCampusCadastroAjax' + sfunid,
							        onComplete: function (res)
							        {	
										td_campus.innerHTML = res.responseText;										
							        }
							  });
}

function listarMunicipiosCadastro(cmpid) {
	
	estuf 	= document.formulario.estuf; 	
	muncod  = document.formulario.muncod; 	
	
	// Faz uma requisição ajax, passando o parametro 'ordid', via POST
	var req = new Ajax.Request('sig.php?modulo=principal/cadastrarcampus&acao=A', {
							        method:     'post',
							        parameters: '&cmpid=' + cmpid + '&exec_function=listarMunicipioCadastroAjax',
							        onComplete: function (res)
							        {	
							        	var rs = res.responseText.split("|");							        	
										estuf.value = rs[0];	
										muncod.value = rs[1];										
							        }
							  });
}

function inserirEntidade(entid,iscampus,tpeid){
	if(!entid) {
		alert('Selecione uma entidade');
		return false;
	} else {
		return windowOpen( '?modulo=principal/inserir_entidade&acao=A&tpeid='+tpeid+'&iscampus='+ iscampus +'&entid=' + entid ,'blank','height=700,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
	}
}

function calculacoluna(campo) {
	// pegando a coluna 
	var coluna = campo.parentNode.cellIndex;
	// pegando a tabela
	var tabela = campo.parentNode.parentNode.parentNode;
	var tot = 0;
	for(var i=0; i < (tabela.rows.length-1); i++) {
		if(tabela.rows[i].cells[coluna].childNodes[0].value != "") {
			tot = tot + parseFloat(tabela.rows[i].cells[coluna].childNodes[0].value);
		}
	}
	if(tot) {
		tabela.rows[(tabela.rows.length-1)].cells[coluna].childNodes[0].value = tot;
	}

}

function validaprocessoseletivo() {
	if(document.getElementById('prsinscricaoini').value == "") {
		alert("Data inicial das inscrições é obrigatória.");
		return false;
	}
	if(!validaData(document.getElementById('prsinscricaoini'))) {
		alert("Data inicial das inscrições é inválida.");
		return false;
	}

	if(document.getElementById('prsinscricaofim').value == "") {
		alert("Data final das inscrições é obrigatória.");
		return false;
	}
	if(!validaData(document.getElementById('prsinscricaofim'))) {
		alert("Data final das inscrições é inválida.");
		return false;
	}

	if(document.getElementById('prsprovaini').value == "") {
		alert("Data inicial das provas é obrigatória.");
		return false;
	}
	if(!validaData(document.getElementById('prsprovaini'))) {
		alert("Data final das provas é inválida.");
	}

	if(document.getElementById('prsprovafim').value == "") {
		alert("Data final das provas é obrigatória.");
		return false;
	}
	if(!validaData(document.getElementById('prsprovafim'))) {
		alert("Data final das inscrições é inválida.");
		return false;
	}

	if(document.getElementById('prsinicioaula').value == "") {
		alert("Data de início das aulas é obrigatória.");
		return false;
	}
	if(!validaData(document.getElementById('prsinicioaula'))) {
		alert("Data de início das aulas é inválida.");
		return false;
	}
	
	document.getElementById('formulario').submit();


}

function verificasituacaocampus(opt) {
	// F - Funcionando, se tiver abre as outras opções
	if(opt.value=='F') {
		document.getElementById('trcmpinstalacao').style.display='';
		document.getElementById('cmpinstalacaoD').disabled=false;
		document.getElementById('cmpinstalacaoP').disabled=false;
	} else {
		document.getElementById('trcmpinstalacao').style.display='none';
		document.getElementById('cmpinstalacaoD').disabled=true;
		document.getElementById('cmpinstalacaoP').disabled=true;
	}
}

function validardadosespecificos() {
	
	if(document.getElementById('cmpdataimplantacao').value != '') {
		if(document.getElementById('cmpdataimplantacao').value.length != 7) {
			alert('"Data de implantação" não esta no formato correto.');
			return false;
		}
	}
	if(document.getElementById('cmpdatainauguracao').value != '') {
		if(!validaData(document.getElementById('cmpdatainauguracao'))) {
			alert('"Data de implantação" é inválida.');
			return false;
		}
	}
	return true;
}

function inserirobrainaugurada() {
	return windowOpen('?modulo=principal/inserir_obrasinauguradas&acao=A&cmpid=','blank','height=500,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function validarsolicitacao(btn) {

	if(document.getElementById('solprazodata').value == "") {
		alert('Data do prazo de resposta é obrigatória');
		return false;
	}
	
	if(document.getElementById('solprazohora').value == "") {
		alert('Hora do prazo de resposta é obrigatória');
		return false;
	}
	
	if(document.getElementById('soldesc').value == "") {
		alert('Descrição é obrigatória');
		return false;
	}
	
	document.getElementById('formulario').submit();
	
	btn.disabled=true
}

function validarencaminhamento(btn) {
	if(document.getElementById('pessoas').value == "") {
		alert('Selecione destinatarios');
		return false;
	}

	if(document.getElementById('encdesc').value == "") {
		alert('Mensagem é obrigatória');
		return false;
	}

	document.getElementById('formulario').submit();
	btn.disabled=true
}

function validarresposta(btn) {

	if(document.getElementById('rsptxtresposta').value == "") {
		alert('Resposta é obrigatória');
		return false;
	}

	document.getElementById('formulario').submit();
	btn.disabled=true
}

function inserirNovosArquivos() {
	var tabela = document.getElementById('anexos');
	for(i=0;i<1;i++) {
		var line = tabela.insertRow((tabela.rows.length-2));
		var cell = line.insertCell(0);
		cell.innerHTML = "<input type=\"file\" name=\"arquivo[]\">";
		var cell1 = line.insertCell(1);
		cell1.innerHTML = "Nome do arquivo : <input class=\"normal\" type=\"text\" name=\"arquivonome[]\">";
		var cell2 = line.insertCell(2);
		cell2.innerHTML = "<img src=\"../imagens/excluir.gif\" onclick=\"document.getElementById('anexos').deleteRow(this.parentNode.parentNode.rowIndex);\">";
	}
}