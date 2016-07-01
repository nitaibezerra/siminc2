function proximoPasso(){
	window.location.href = "emenda.php?modulo=principal/ListarIniciativasPlanoTrabalho&acao=A";
}

function Entidade(entid){
	window.open('emenda.php?modulo=principal/cadastrarEntidadeBeneficiada&acao=A&entid='+entid,'page','toolbar=no,location=no,status=yes,menubar=no,scrollbars=yes,resizable=no,fullscreen=yes');
}

function atualizaEntidadeBaseFNDE( enbid, ptrid ){
	/*var url = "ajax_emenda.php";
	var pars = 'atualizaEntidadeFNDE=atualiza&ptrid='+ptrid;*/
	
	var largura = 673;
	var altura = 500;
	//pega a resolução do visitante
	w = screen.width;
	h = screen.height;
	
	//divide a resolução por 2, obtendo o centro do monitor
	meio_w = w/2;
	meio_h = h/2;
	
	//diminui o valor da metade da resolução pelo tamanho da janela, fazendo com q ela fique centralizada
	altura2 = altura/2;
	largura2 = largura/2;
	meio1 = meio_h-altura2;
	meio2 = meio_w-largura2;
		
	window.open('emenda.php?modulo=principal/popupatualizaEntidadeFNDE&acao=A&enbid='+enbid+'&ptrid='+ptrid,'','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'');
	
	/*document.getElementById( 'aguarde' ).style.visibility = 'visible';
	document.getElementById('aguarde').style.display = '';*/
	
	/*var myajax = new Ajax.Updater('', url, {
		        method:     'post',
		        parameters: pars,
		        asynchronous: false
			  });*/
	window.location.href = window.location;
	/*document.getElementById( 'aguarde' ).style.visibility = 'hidden';
	document.getElementById('aguarde').style.display = 'none';*/
}

function teste(x){
	if(x.selectedIndex != 0){
		$('mostraTitle').style.display = '';
		$('mostraTitle').innerHTML = x.options[x.selectedIndex].text;
		positionDiv(x);
	}
	
}

function positionDiv(inputObj){
	//alert(inputObj.offsetHeight);
	mostraTitle.style.left = getleftPos(inputObj) + 'px';
	mostraTitle.style.top = getTopPos(inputObj) + 'px';
	/*if(iframeObj){
		iframeObj.style.left = mostraTitle.style.left;
		iframeObj.style.top =  mostraTitle.style.top;
		//// fix for EI frame problem on time dropdowns 09/30/2006
		iframeObj2.style.left = mostraTitle.style.left;
		iframeObj2.style.top =  mostraTitle.style.top;
	}*/
		
}

function carregaTextOption(v){
	if(v.selectedIndex != 0){
		$('spanLabel').style.display = '';
		$('spanLabel').innerHTML = v.options[v.selectedIndex].text;
	} else {
		$('spanLabel').style.display = 'none';	
		$('spanLabel').innerHTML = '';
	}
}

function saida(){
	$('mostraTitle').style.display = 'none';
	$('mostraTitle').innerHTML = '';
}

function montaComboPTRES(arValor, id, emeid, tipoEmenda){
	var url = "ajax_emenda.php";
	var pars = 'ExecucaoOrcamentaria=PTRES&arValor='+arValor+'&id='+id+'&emeid='+emeid+'&tipoemenda='+tipoEmenda;
	var arPtres = arValor.split("|");
	if( arPtres[1] ){
		populaHiddenExecucaoOrcamentariaPtres(arPtres[1], emeid);
	} else {
		populaHiddenExecucaoOrcamentariaPtres('', emeid);
	}
	
	var myajax = new Ajax.Updater('ComboPTRES', url, {
		        method:     'post',
		        parameters: pars,
		        asynchronous: false
			  });
}

function populaHiddenExecucaoOrcamentariaPtres( valor, id ){
	document.getElementById( 'ptres_pi[' + id + ']' ).value = valor;	
}

function salvarExecucaoOrcamentaria(){
	var form = $('formExecOrcamentaria');
	var nome = "";
	var exfvalor = 0;
	var valorTotal = 0;
	
	for(i=0; i<form.length; i++){
		
		if( form.elements[i].type == 'select-one' || form.elements[i].type == 'text' ){
			//alert( form.elements[i].disabled );
			if(form.elements[i].value == ""){
				alert('Existe(m) campo(s) em branco!');
				$(form.elements[i].id).focus();
				return false;
			}	
		} /*else {
			if(form.elements[i].type == 'hidden'){
				if( form.elements[i].id.indexOf('emeid') != -1 ){
					var emeid = form.elements[i].value;
				}
				if(form.elements[i].id.indexOf('exfvalor_['+emeid+']') != -1){
					exfvalor = parseFloat( form.elements[i].value );
				}
				if(form.elements[i].id == 'exfvalortotal['+emeid+']'){
					valorTotal = parseFloat( form.elements[i].value );
					alert( valorTotal +' - '+ exfvalor );
					exfvalor = 0;
					valorTotal = 0;
					
					if( valorTotal && exfvalor){
						if(valorTotal.toFixed(2) > exfvalor.toFixed(2) ){
							var restante = valorTotal - exfvalor;
							restante = 'R$ ' + mascaraglobal('###.###.###.###,##', restante.toFixed(2));
							alert('A soma dos valores preenchidos para os PIs é MENOR que o valor disponibilizado pelo recurso.\n' + 'Valor restante:\n\t'+ restante);
							return false;
						}
					}
				}
			}
		}*/
	}
	
	var url = "ajax_emenda.php";
	var pars = 'ExecucaoOrcamentaria=salvar&'+ document.getElementById('formExecOrcamentaria').serialize();
	$('loader-container').show();
	var myAjax = new Ajax.Request(
			url, 
			{
				method: 'post', 
				parameters: pars, 
				asynchronous: false,
				onComplete: mostraResposta
			});	
	$('loader-container').hide();
}

function mostraResposta(resp){
	//$('erro').innerHTML = resp.responseText;
	if( resp.responseText == '1' ){
		alert('Operação realizada com sucesso!');
		//$('validaPI').innerHTML = '<img src="../imagens/valida1.gif" title="Dados Armazenados"/>';
		window.location.href = 'emenda.php?modulo=principal/analiseGestorPTA&acao=A';
		//window.reload();
	}else{
		if( resp.responseText.length < 100 ){
			alert( resp.responseText );
		} else {
			alert('Falha na operação');
		}
	}
}

function validaTotalRecurso(emeid){
	var form = $('formExecOrcamentaria');
	var valorTotal = $('exfvalortotal['+emeid+']').value;
	var valor = 0;
	for(i=0; i<form.length;i++){
		if(form.elements[i].type == 'hidden'){
			if(form.elements[i].id.indexOf('exfvalor_['+emeid+']') != -1){
				valor = valor + parseFloat( form.elements[i].value );
			}
		}	
	}
	//alert(valor.toFixed(2)+' - '+valorTotal);
	/*var valorTotal = $('exfvalortotal').value;
	var valor = valor.replace(/\./gi, '');
	var valor = valor.replace(/\,/gi, '.');*/
	
	if(parseFloat(valor.toFixed(2)) > parseFloat(valorTotal) ){
		var v = valor - valorTotal;
		v = 'R$ ' + mascaraglobal('###.###.###.###,##', v.toFixed(2));
		alert('A soma dos valores preenchidos para os PIs é MAIOR que o valor disponibilizado pelo recurso.\n' + 'Valor remanescente:\n\t'+ v);
		$('btnSalvarExef').disabled = true;
		return false;
	} else {
		$('btnSalvarExef').disabled = false;
		return true;
	}
}

function populaHiddenExecucaoOrcamentaria( valor, id ){
	document.getElementById( 'pi[' + id + ']' ).value = valor;	
}
function populaHiddenExfValor( valor, emeid, id ){
	var valor = valor.replace(/\./gi, '');
	var valor = valor.replace(/\,/gi, '.');
	
	document.getElementById( 'exfvalor_[' + emeid + ']_'+ id ).value = valor;	
}

function abrePTAconsolidado( ptrid, tipo ) {
	if( tipo == 'entidade' ){
		var janela = window.open("emenda.php?modulo=principal/ptaConsolidadoEntidade&acao=A&ptrid=" + ptrid,"PTAConsolidadoEntidade", "scrollbars=yes,height=600,width=800");
		janela.focus();
	}else if( tipo == 'convenio' ){
		var janela = window.open("emenda.php?modulo=principal/popupDicaImpressao&acao=A&parametro=" + ptrid+'&tipo=minuta',"relatorio", "toolbar=no,location=no,status=yes,menubar=yes,scrollbars=yes,resizable=no,height=600,width=800");
		janela.focus();
	} else {
		var janela = window.open("emenda.php?modulo=principal/popupDicaImpressao&acao=A&parametro=" + ptrid+'&tipo=pta',"relatorio", "toolbar=no,location=no,status=yes,menubar=yes,scrollbars=yes,resizable=no,height=600,width=800");
		janela.focus();
	}
}

function abrePTAAnaliseRelatorio( ptrid, tipo ) {
	var janela = window.open("emenda.php?modulo=principal/popupImprimirAnalisePTA&acao=A&ptrid=" + ptrid + "&tipo="+tipo,"relatorio", "scrollbars=yes,height=600,width=800");
	janela.focus();
}

function visualizarTiposReformulacao( ptrid ) {
	var janela = window.open("emenda.php?modulo=principal/popupMostraTiposReformulacao&acao=A&ptrid=" + ptrid,"reformulacao", "scrollbars=yes,height=300,width=500");
	janela.focus();
}

function visualizaProposta(resid, enbid){
	var janela = window.open("emenda.php?modulo=principal/guiaVisualizaProposta&acao=A&resid="+resid+"&enbid="+enbid,"proposta", "scrollbars=yes,height=600,width=800");
	janela.focus();
}