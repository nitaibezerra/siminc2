function ctrlDisplay(idAbre, idFecha){
	var d			 = document;
	var displayAbre  = ( navigator.appName.indexOf('Explorer') > -1 ? 'block' : 'table-row');
	var displayFecha = 'none';
	
	if (typeof(idAbre) != 'object' && idAbre != ''){
		idAbre = new Array(idAbre);
	}
	
	if (typeof(idFecha) != 'object'  && idFecha != ''){
		idFecha = new Array(idFecha);
	}
	
	// Abre
	for (i=0; i < idAbre.length; i++){
		obj = d.getElementById(idAbre[i]);
		obj.style.display = displayAbre;
	}
	
	// Fecha
	for (i=0; i < idFecha.length; i++){
		obj = d.getElementById(idFecha[i]);
		obj.style.display = displayFecha;
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

function redireciona(url){
	location.href = url;
}

/**
 ******************** Funcoes migradas do SIG **********
 **/
function Excluir(url, msg) {
	if(confirm(msg)) {
		window.location = url;
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
 
 
function validardadosespecificos() {
	
	if(document.getElementById('exiid').value == '') {
		alert('"Existência" deve ser obrigatória.');
		return false;
	}
	
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
 
    function selecionarCurso(cmpid){
            return windowOpen('?modulo=principal/selecionar_cursos&acao=A&cmpid='+cmpid,'blank','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
    }

    function inserirCursos(cmpid){
            return windowOpen('?modulo=principal/inserir_cursos&acao=A&cmpid='+cmpid,'blank','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
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
 
function academico_ajaxatualizar(params,iddestinatario) {	
	
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

function academico_Excluir(url, msg) {
	if(confirm(msg)) {
		window.location = url;
	}
}

function academico_inserirobrainaugurada( acao1, valor ) {
	return windowOpen('?modulo=principal/inserir_obrasinauguradas&acao=A&acao1=' + acao1 + '&valor=' + valor,'blank','height=500,width=600,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

/**
 **** FIM MIGRAÇÂO SIG ******
 */

function abreDetalhamentoLancamento(idclasse, param){

	var jan = window.open('academico.php?modulo=principal/popup/listaEdital&acao=A' + param + '&idclasse=' + idclasse, '_detalhamento', 'height=700,width=700,status=1,toolbar=0,menubar=no,scrollbars=1,resizable=1');
	jan.focus();

}

function abreportaria( prtid ){
	window.open('/academico/academico.php?modulo=principal/cadportaria&acao=A&prtid=' + prtid, '_TOP');
	
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

function listarCargos(tabela, clsid) {
	//return windowOpen('?modulo=principal/listarcargos&acao=A&tab='+tabela+'&clsid='+clsid,'blank','height=600,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
	var cargos = window.open('?modulo=principal/listarcargos&acao=A&tab='+tabela+'&clsid='+clsid,'cargo','height=600,width=500,status=1,toolbar=0,menubar=no,scrollbars=1,resizable=1'); 
	cargos.focus();
}

function listarCargosProf(tabela, clsid) {	
	var cargos = window.open('?modulo=principal/listarcargosprof&acao=A&tab='+tabela+'&clsid='+clsid,'cargo','height=600,width=500,status=1,toolbar=0,menubar=no,scrollbars=1,resizable=1'); 
	cargos.focus();
}

function cadastrarObs(id_obs_nefetivado) {	
	var obs_nefetivado = window.open('?modulo=principal/cadastrarobs&acao=C&id_obs_nefetivado='+id_obs_nefetivado,'obs','height=230,width=450,status=1,toolbar=0,menubar=no,scrollbars=0,resizable=0'); 
	obs_nefetivado.focus();
}

//ajax que verifica se é possível remover um determinado cargo
function excluirLinha(index, tabela_classe, lepid, tpeid) {

	var tabela = document.getElementById(tabela_classe);		
	
	var url = '/academico/ajax.php?ajax_excluircargo=1&lepid='+lepid+'&tpeid='+tpeid;
	new Ajax.Request(url, {
	  method: 'get',
	  onSuccess: function(transport) {
	   
		    if (transport.responseText){
		    	alert(transport.responseText);
		    }else{		    			    	
				tabela.deleteRow(index);
				
				calculaTotalGeral(tabela_classe);
		    }
	  }
	});
}
//calcula totais de todos os lançamentos por 
function calculaTotalGeral(tabela_classe) {
	
	var col_pub = 'publicado_'+tabela_classe;
	var col_hom = 'homologado_'+tabela_classe;
	var col_efe = 'efetivado_'+tabela_classe;
	var total_pub = 'total_pub_'+tabela_classe;
	var total_hom = 'total_hom_'+tabela_classe;
	var total_efe = 'total_efe_'+tabela_classe;
		
	var i; 
	var soma_pub = 0;
	var soma_hom = 0;
	var soma_efe = 0;
		
	form = document.getElementById("formulario");	
			
	for(i=0; i<form.length; i++) {
		
		id_form = form.elements[i].id.substr(0,(form.elements[i].id.lastIndexOf("_")));
	
		if(document.getElementById(total_pub)){		
			if((col_pub == id_form) && (form.elements[i].value != '')) {
				soma_pub = soma_pub + parseInt(form.elements[i].value);
			}		
		}
		
		if(document.getElementById(total_hom)){		
			if((col_hom == id_form) && (form.elements[i].value != '')) {
				soma_hom = soma_hom + parseInt(form.elements[i].value);
			}		
		}
		
		if(document.getElementById(total_efe)){		
			if((col_efe == id_form) && (form.elements[i].value != '')) {
				soma_efe = soma_efe + parseInt(form.elements[i].value);
			}		
		}
			
	}
	if(document.getElementById(total_pub))
		document.getElementById(total_pub).value = soma_pub;	
		
	if(document.getElementById(total_hom))
		document.getElementById(total_hom).value = soma_hom;
		
	if(document.getElementById(total_efe))
		document.getElementById(total_efe).value = soma_efe;
}

function calculaTotalInicial(id_coluna, id_total) {
	
	var i, soma = 0;
		
	form = document.getElementById("formulario");
			
	for(i=0; i<form.length; i++) {
		
		id_form = form.elements[i].id.substr(0,(form.elements[i].id.lastIndexOf("_")));
		
		if((id_coluna == id_form) && (form.elements[i].value != '')) {
			soma = soma + parseInt(form.elements[i].value);
		}	
	}
	if(document.getElementById(id_total))
		document.getElementById(id_total).value = soma;	

	return soma;
}

function calculaTotal(coluna, id_total) {
	
	var id_coluna = coluna.id.substr(0,(coluna.id.lastIndexOf("_")));
		
	var i, soma = 0;
		
	form = document.getElementById("formulario");
			
	for(i=0; i<form.length; i++) {
		
		id_form = form.elements[i].id.substr(0,(form.elements[i].id.lastIndexOf("_")));
		
		if((id_coluna == id_form) && (form.elements[i].value != '')) {
			soma = soma + parseInt(form.elements[i].value);
		}	
	}
	if(document.getElementById(id_total))
		document.getElementById(id_total).value = soma;	

	return soma;
}

function atualizaTotal(lancamento, total_id) {
	var val_lancamento  = parseInt(lancamento.value);
	var val_cargo 		= parseInt(document.getElementById("cargo_id").value);
	
	document.getElementById("lancamento_id").value = val_lancamento + val_cargo;
	
}

function academico_listaUnidades( uf, orgid ){
    var erro = 1;
    var param = '';

    if(document.getElementById('exiid').value != ""){
        param += '&filtrocmp[exiid]='+document.getElementById('exiid').value;
        erro = 0;
    }

    if(document.getElementById('cmpsituacao').value != ""){
        param += '&filtrocmp[cmpsituacao]='+document.getElementById('cmpsituacao').value;
        erro = 0;
    }

    if(document.getElementById('cmpinstalacao').value != ""){
        param += '&filtrocmp[cmpinstalacao]='+document.getElementById('cmpinstalacao').value;
        erro = 0;
    }
    document.getElementById('filtrogeral').value = uf;

    var url = 'academico.php?modulo=principal/painel&acao=A';
    if( erro == 0 ){
        new Ajax.Updater(
            'conteudolistaunidades', 
            url,{
                method: 'post',
                parameters: '&listarunidades=1&estuf=' + uf + '&orgid=' + orgid + param,
                asynchronous: false,
                onComplete: function(res){        
                    closeMessage();    	
                }
            }
        );	
    }else{
        alert( 'É necessário selecionar um filtro: "Existência do campus/uned", "Situação do campus/uned" ou "Instalações". Selecione um e tente novamente!' );
    }
}
 
function atualiza_div( acao, dado ){
	
	var url = 'academico.php?modulo=principal/painel&acao=A';
	
	var link = '';
	switch( acao ){
		case 'indicadores':
			link = 'listaindicadores'
		break;
		case 'obras':
			link = 'listaobras'
		break;
		case 'unidade':
			link = 'dadosunidade'
		break;
		case 'campus':
			link = 'dadoscampus'
		break;
		case 'academico':
			link = 'dadosacademico'
		break;
		case 'concursos':
			link = 'dadosconcursos'
		break;
		case 'financeiro':
			link = 'dadosfinanceiro'
		break;
		case 'listaCampus':
			link = 'listaCampus';
		break;
		case 'previstoRealizado':
			url = "academico.php?modulo=relatorio/acomprevreal&acao=A"
			link = 'previstoRealizado';
		break;
		
	}

	new Ajax.Updater('containerMapa', url,
		{
	    method: 'post',
	    parameters: '&requisicaoajax=' + link + '&dado=' + dado,
	    onComplete: function(res)
	    	{        
	    		extrairScript(res.responseText);
	        }
		}
	);
	
}

function ver_obras_situacao( stoid, estuf, entid ){

	var url = 'academico.php?modulo=principal/painel&acao=A';
	new Ajax.Updater('containerMapa', url,
		{
	    method: 'post',
	    parameters: '&requisicaoajax=situacaoobras&dado=' + stoid + '&estuf=' + estuf + '&entid=' + entid,
	    onComplete: function(res)
	    	{        
	    	
	        }
		}
	);
		
}

function acaVerificaTipoCurso( valor ){

	if ( valor == 'P' ){
		
		document.getElementById( "trVagasOfertadas" ).style.display = 'none';
		
		if ( document.selection ){
			document.getElementById( "trVagasPactuadas" ).style.display = 'block';
		}else{
			document.getElementById( "trVagasPactuadas" ).style.display = 'table-row';
		}
		
	}else{
		
		document.getElementById( "trVagasPactuadas" ).style.display = 'none';
		
		if ( document.selection ){
			document.getElementById( "trVagasOfertadas" ).style.display = 'block';
		}else{
			document.getElementById( "trVagasOfertadas" ).style.display = 'table-row';
		}
	}
	
}

function acaVerVagasPactuadas( cdtid ){
	
	janela('?modulo=principal/cursosevagas/cadCurso&acao=A&requisicao=vagaspactuadas&cdtid=' + cdtid, 600, 150, 'cadCurso');
	
}

function editarCursoA(obj){
	var curid = trim(obj.id.replace("img_edit_", ""));
	obj_tr = obj.parentNode.parentNode.parentNode;
	obj_td = obj.parentNode.parentNode;
	
	//Bloqueio do Check
	document.getElementById('ckc_' + curid ).disabled = true;
	
	//Bloqueio do botão editar
	document.getElementById('img_edit_' + curid ).src = "../imagens/alterar_01.gif";
	document.getElementById('img_edit_' + curid ).onclick = function(){  alert("Operação em andamento!") };
	
	//Bloqueio do botão excluir
	document.getElementById('img_delete_' + curid ).src = "../imagens/excluir_01.gif";
	document.getElementById('img_delete_' + curid ).onclick = function(){  alert("Operação Indisponível!") };
	
	var curdsc = trim(obj_tr.cells[1].innerHTML);
	var turdsc = trim(obj_tr.cells[2].innerHTML);
	atualizaAjax('academico.php?modulo=principal/editarcampus&acao=A','ajaxCursos=1&exec_function=carregaCursoDescricao&curid=' + curid + '&curdsc=' + curdsc, obj_tr.cells[1] );	
	atualizaAjax('academico.php?modulo=principal/editarcampus&acao=A','ajaxCursos=1&exec_function=carregaCursoTurno&curid=' + curid + '&turdsc=' + turdsc, obj_tr.cells[2] );
}

function excluirCurso(obj){
	var curid = trim(obj.id.replace("img_delete_", ""));
	var obj_tr = obj.parentNode.parentNode.parentNode;
	var obj_td = obj.parentNode.parentNode;
	var tpcid = document.getElementById('unidades').value;
	
	if( document.getElementById('ckc_' + curid ).checked == true ){
		alert('Operação não permitida!');
		return false;
	}else{
		if(confirm("Deseja realmente excluir este curo?")){
			var curdsc = trim(obj_tr.cells[1].innerHTML);
			atualizaAjax('academico.php?modulo=principal/editarcampus&acao=A','ajaxCursos=1&exec_function=excluirCurso&curid=' + curid + '&curdsc=' + curdsc, obj_tr.cells[1] );
			selecionatipocurso(tpcid);
			alert('Operação realizada com sucesso!');
		}
	}
}

function atualizaAjax(url,params, destino , posfuncao ){
	new Ajax.Request(url, {
	  method: 'post',
	  parameters: params ,
	  onSuccess: function(data) {
			if(destino)
				destino.innerHTML = data.responseText;
			if(posfuncao)
				posfuncao
	  }
	});
}

function salvarEdicaoCurso(curid){
	var curdsc = document.getElementById('curdsc_' + curid).value;
	var turid = document.getElementById('turid_' + curid).value;

	//Desbloqueio do Check
	document.getElementById('ckc_' + curid ).disabled = false;
	
	//Desbloqueio do botão editar
	document.getElementById('img_edit_' + curid ).src = "../imagens/alterar.gif";
	document.getElementById('img_edit_' + curid ).onclick = function(){  editarCurso(this) };
	
	//Desbloqueio do botão excluir
	document.getElementById('img_delete_' + curid ).src = "../imagens/excluir.gif";
	document.getElementById('img_delete_' + curid ).onclick = function(){  excluirCurso(this) };
	
	obj_tr = document.getElementById('curdsc_' + curid).parentNode.parentNode;
	
	atualizaAjax('academico.php?modulo=principal/editarcampus&acao=A','ajaxCursos=1&exec_function=salvaCursoDescricao&curid=' + curid + '&curdsc=' + curdsc, obj_tr.cells[1] );	
	atualizaAjax('academico.php?modulo=principal/editarcampus&acao=A','ajaxCursos=1&exec_function=salvaCursoTurno&curid=' + curid + '&turid=' + turid, obj_tr.cells[2] );
	
	//if( document.getElementById('ckc_' + curid ).checked == true){}

}

function exibeInserirNovoCurso( tpcid, cpcprevisto ){
	
	new Ajax.Request('academico.php?modulo=principal/editarcampus&acao=AajaxCursos=1&exec_function=exibeInserirNovoCurso', {
	  method: 'post',
	  parameters: 'tpcid=' + tpcid + '&cpcprevisto=' + cpcprevisto,
	  onSuccess: function(data) {
	  		
	  		var resultado = data.responseText;
			
			if( resultado.search("true") > 0 ){
				document.getElementById('inserir_curso1').style.display = '';
				document.getElementById('inserir_curso2').style.display = '';
				document.getElementById('inserir_curso3').style.display = '';
				document.getElementById('inserir_curso4').style.display = '';
			}else{
				document.getElementById('inserir_curso1').style.display = 'none';
				document.getElementById('inserir_curso2').style.display = 'none';
				document.getElementById('inserir_curso3').style.display = 'none';
				document.getElementById('inserir_curso4').style.display = 'none';
			}
				
	  }
	});
	
}
