
function abreDadosSec( estuf, tipo ){
	if( tipo == 'inicio' ){
		window.location = 'emi.php?modulo=principal/arvoreSecretaria&acao=A&estuf=' + estuf;
	}else if( tipo == 'previsao' ){
		window.location = 'emi.php?modulo=relatorio/previsaoOrcamentaria&acao=A&estuf=' + estuf;
	}else if( tipo == 'dimensao' ){
		window.location = 'emi.php?modulo=relatorio/relatorioDimensao&acao=A&estuf=' + estuf;
	}else if( tipo == 'aprovacao' ){
		window.location = 'emi.php?modulo=principal/aprovacao&acao=A&estuf=' + estuf;
	}else if( tipo == 'classificacaodespesas' ){
		window.location = 'emi.php?modulo=relatorio/classificacaoDespesas&acao=A&estuf=' + estuf;
	}
}
 
function selecionaCoordenador( entid ){
	
	if ( entid != '' ){
		window.location = 'emi.php?modulo=principal/cadastraCoordenador&acao=A&entid=' + entid;
	}else{
		window.location = 'emi.php?modulo=principal/cadastraCoordenador&acao=A';
	}
	
}

function envirarFormulario( emeid, tipo){
	if ( tipo == 'secretaria'  ){
		window.location = 'emi.php?modulo=principal/uploadFormulario&acao=A&emeid=' + emeid;
	}else if ( tipo == 'escola' ){
		window.location = 'emi.php?modulo=principal/uploadFormulario&acao=C&emeid=' + emeid;
	}
	
}

function validaUpload(){

	var arquivo   = document.getElementById( 'arquivo' );
	var descricao = document.getElementById( 'arqdescricao' );

	var mensagem = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;
	
	if ( arquivo.value == '' ){
		mensagem += 'Arquivo \n';
		validacao = false;
	}

	if ( descricao.value == '' ){
		mensagem += 'Descrição \n';
		validacao = false;
	}

	if ( !validacao ){
		alert( mensagem );
	}else{
		document.getElementById( 'emiUploadFormulario' ).submit();
	}

}

function selecionaEscolas(){
	window.location = 'emi.php?modulo=principal/selecionaEscolas&acao=A&';
}

function enviaEscolas(){

	selectAllOptions( document.getElementById( 'escolas' ) );
	document.getElementById( 'emiSelecionaEscolas' ).submit();

}

function dadosEscolas( emeid ){
	window.location = 'emi.php?modulo=principal/dadosEscola&acao=A&emeid=' + emeid;
}

function pap( emeid, tppid ){
	if ( tppid == 1 ){
		window.location = 'emi.php?modulo=principal/papsEscola&acao=A&emeid=' + emeid;
	}else if ( tppid == 2 ){
		window.location = 'emi.php?modulo=principal/papsEscola&acao=C&emeid=' + emeid;
	}
}

function gap( emeid, tppid ){
	if ( tppid == 1 ){
		window.location = 'emi.php?modulo=principal/gapsEscola&acao=A&emeid=' + emeid;
	}else if ( tppid == 2 ){
		window.location = 'emi.php?modulo=principal/gapsEscola&acao=C&emeid=' + emeid;
	}
}

function insereAcaoPap( comid, tppid ){

	window.open("?modulo=principal/popupAcaoPap&acao=A&comid=" + comid + "&tppid=" + tppid, "AcaoPap","menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=600");

}

function salvaDadosPap(){

	var mensagem  = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;

	var acao = document.getElementById('papcaoatividade');
	var meta = document.getElementById('papmeta');
	
	if( acao.value == '' ){
		mensagem += 'Ação/Atividade \n';
		validacao = false;
	}
	
	if( meta.value == '' ){
		mensagem += 'Meta \n';
		validacao = false;
	}
	
	if( !validacao ){
		alert( mensagem );
		return false;
	}else{
		document.getElementById('formulario').submit();
	}
	
}

function alterarPAP( papid ){
	
	var url = 'emi.php?modulo=principal/popupAcaoPap&acao=A';
	var parametros = "&requisicao=alterar&papid=" + papid;

	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				
				var json = resp.responseText.evalJSON();
				
				$('papid').value 		   = json.papid;
				$('papcaoatividade').value = json.papcaoatividade;
				$('papmeta').value         = json.papmeta;

			}
			
		}
		
	);
	
}

function alterarGAP( papid ){
	
	var url = 'emi.php?modulo=principal/popupAcaoGap&acao=A';
	var parametros = "&requisicao=pegaDadosGap&papid=" + papid;

	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				
				var json = resp.responseText.evalJSON();
				
				$('papid').value 		   = json.papid;
				$('papcaoatividade').value = json.papcaoatividade;
				$('papmeta').value         = json.papmeta;

			}
			
		}
		
	);
	
}

function excluirPAP( papid ){

	if( confirm("Deseja realmente excluir esta Ação e Meta?") ){
		window.location = 'emi.php?modulo=principal/popupAcaoPap&acao=A&requisicao=excluir&papid=' + papid;
	}

}

function excluirGAP( papid ){

	if( confirm("Deseja realmente excluir esta Ação e Meta?") ){
		window.location = 'emi.php?modulo=principal/popupAcaoGap&acao=A&requisicao=excluirGap&papid=' + papid;
	}

}

function preencheMatriz( papid, tppid ){
	if ( tppid == 1 ){
		window.location = 'emi.php?modulo=principal/matriz&acao=A&papid=' + papid;
	}else if( tppid == 2 ){
		window.location = 'emi.php?modulo=principal/matriz&acao=C&papid=' + papid;
	}
}

function preencheMatrizGAP( papid, tppid ){
	if ( tppid == 1 ){
		window.location = 'emi.php?modulo=principal/matrizGap&acao=A&papid=' + papid;
	}else if( tppid == 2 ){
		window.location = 'emi.php?modulo=principal/matrizGap&acao=C&papid=' + papid;
	}
}

function preencheCriticaPap( papid ) {
	var janela = window.open("?modulo=principal/popupCritica&acao=A&papid=" + papid, "critica", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=500");
	janela.focus();
}

function preencheCriticaGap( papid ) {
	var janela = window.open("?modulo=principal/popupCriticaGap&acao=A&papid=" + papid, "critica", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=500");
	janela.focus();
}

function salvarCriticaPap( tipo ) {
	var crpdsccritica	=	document.getElementsByName("crpdsccritica")[0];
	var crpdscresposta 	=	document.getElementsByName("crpdscresposta")[0];
	var observacao 		= 	document.getElementsByName('crpobs');
	
	var form			=	document.getElementById("formCritica");
	
	if(tipo == "cadastrador") {
		if( crpdscresposta.value == "") {
			alert("O campo 'Resposta' deve ser informado.");
			crpdscresposta.focus();
			return;
		}
		
		var textoConfirm = (observacao[0].checked == true) ? "Esta será sua resposta à Observação. Deseja continuar?" : "Esta será sua resposta defitiva à Crítica. Deseja continuar?";
		
		if(confirm(textoConfirm)) form.submit();
	} else {
		if( crpdsccritica.value == "") {
			var textoCampo = (observacao[0].checked == true) ? "O campo 'Observação' deve ser informado." : "O campo 'Crítica' deve ser informado.";
			alert(textoCampo);
			crpdsccritica.focus();
			return;
		}
		form.submit();
	}
}

function preencheCriticaMatriz( mdoid ) {
	var janela = window.open("?modulo=principal/popupCriticaMatriz&acao=A&mdoid=" + mdoid, "criticamatriz", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=500");
	janela.focus();
}

function preencheCriticaMatrizGap( mdoid ) {
	var janela = window.open("?modulo=principal/popupCriticaMatrizGap&acao=A&mdoid=" + mdoid, "criticamatriz", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=500");
	janela.focus();
}

function salvarCriticaMatriz( tipo ) {
	var crmdsccritica	=	document.getElementsByName("crmdsccritica")[0];
	var crmdscresposta 	=	document.getElementsByName("crmdscresposta")[0];
	var observacao 		= 	document.getElementsByName('crmobs');
	
	var form			=	document.getElementById("formCritica");
	
	if(tipo == "cadastrador") {
		if( crmdscresposta.value == "") {
			alert("O campo 'Resposta' deve ser informado.");
			crmdscresposta.focus();
			return;
		}
		
		var textoConfirm = (observacao[0].checked == true) ? "Esta será sua resposta à Observação. Deseja continuar?" : "Esta será sua resposta defitiva à Crítica. Deseja continuar?";
		
		if(confirm(textoConfirm)) form.submit();
	} else {
		if( crmdsccritica.value == "") {
			var textoCampo = (observacao[0].checked == true) ? "O campo 'Observação' deve ser informado." : "O campo 'Crítica' deve ser informado.";
			alert(textoCampo);
			crmdsccritica.focus();
			return;
		}
		form.submit();
	}
}

function preencheTotal(){

	var qtd   = document.getElementById( 'mdoqtd' ).value;
	var mdovalorunitario  = document.getElementById( 'mdovalorunitario' ).value;
	var total = 0;
	
	if( mdovalorunitario == '' ) mdovalorunitario = '1';
	
	mdovalorunitario = mdovalorunitario.replace(".", "");
	mdovalorunitario = mdovalorunitario.replace(".", "");
	mdovalorunitario = mdovalorunitario.replace(".", "");
	mdovalorunitario = mdovalorunitario.replace(",", ".");

	total = Number( qtd * mdovalorunitario );

	total = total.toFixed(2);

	total = mascaraglobal('###.###.###.###,##', total);
	
	document.getElementById( 'mdototal' ).value = total;
	

}

function emiValidaQtdEscolas(){

	var qtd = document.getElementById('emeqtdescolas');

	if ( qtd.value == '' || qtd.value == 0 ){
	
		alert( 'Favor preencher o campo Quantidades de Escolas!' );
	
	}else{
		document.getElementById('formulario').submit();
	}

}

function emiValidaParecer(){

	var parecer = document.getElementById('prcparecer');

	if ( parecer.value == '' ){
	
		alert( 'Favor preencher o campo Parecer!' );
	
	}else{
		document.getElementById('formulario').submit();
	}

}

function emiExcluirParecer( prcid ){

	if ( confirm("Deseja realmente excluir este parecer?") ){

		window.location.href = "emi.php?modulo=principal/parecer&acao=A&requisicao=excluirparecer&prcid=" + prcid;
	
	}
	
}

function insereDadosMatriz(){

	var mensagem  = 'O(s) seguinte(s) campo(s) deve(m) ser preenchido(s): \n \n';
	var validacao = true;
	
	var itfid 			 = document.getElementById('itfid');
	var mdoespecificacao = document.getElementById('mdoespecificacao');
	var undid 			 = document.getElementById('undid');
	var mdoqtd 			 = document.getElementById('mdoqtd');
	var mdovalorunitario = document.getElementById('mdovalorunitario');
	var mdototal		 = document.getElementById('mdototal');

	if( itfid.value == '' ){
		mensagem += 'Itens Financiáveis \n';
		validacao = false;
	}
	
	if( mdoespecificacao.value == '' ){
		mensagem += 'Especificação \n';
		validacao = false;
	}
	
	if( undid.value == '' ){
		mensagem += 'Unidade \n';
		validacao = false;
	}
	
	if( mdoqtd.value == '' ){
		mensagem += 'Quantidade \n';
		validacao = false;
	}
	
	if( mdovalorunitario.value == '' ){
		mensagem += 'Valor Unitário \n';
		validacao = false;
	}
	
	if( mdototal.value == '' ){
		mensagem += 'Total \n';
		validacao = false;
	}

	if( !validacao ){
		alert(mensagem);
	}else{
		document.getElementById('requisicao').value = "salvaDados";
		document.getElementById('emiSelecionaEscolas').submit();
	}

}

function limpaDadosMatriz(){

	var itfid 			 = document.getElementById('itfid').value = '';
	var mdoespecificacao = document.getElementById('mdoespecificacao').value = '';
	var undid 			 = document.getElementById('undid').value = '';
	var mdoqtd 			 = document.getElementById('mdoqtd').value = '';
	var mdovalorunitario = document.getElementById('mdovalorunitario').value = '';
	var mdototal		 = document.getElementById('mdototal').value = '';

}

function alterarItemMatriz ( mdoid, acao ){

	var url = 'emi.php?modulo=principal/matriz&acao=' + acao;
	var parametros = '&requisicao=alteraritem&mdoid=' + mdoid;

	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				var json = resp.responseText.evalJSON();
				
				$('mdoid').value			= json.mdoid;
				$('itfid').value 			= json.itfid;
				$('undid').value 			= json.unddid;
				$('mdoespecificacao').value = json.mdoespecificacao;
				$('mdoqtd').value 			= json.mdoqtd;
				$('mdovalorunitario').value = json.mdovalorunitario;
				$('mdototal').value 		= json.mdototal;
				
			}
		}
	);
	
}

function alterarItemMatrizGap ( mdoid, acao ){

	var url = 'emi.php?modulo=principal/matrizGap&acao=' + acao;
	var parametros = '&requisicao=alteraritemGap&mdoid=' + mdoid;

	var myAjax = new Ajax.Request(
		url,
		{
			method: 'post',
			parameters: parametros,
			asynchronous: false,
			onComplete: function(resp) {
				var json = resp.responseText.evalJSON();
				
				$('mdoid').value			= json.mdoid;
				$('itfid').value 			= json.itfid;
				$('undid').value 			= json.unddid;
				$('mdoespecificacao').value = json.mdoespecificacao;
				$('mdoqtd').value 			= json.mdoqtd;
				$('mdovalorunitario').value = json.mdovalorunitario;
				$('mdototal').value 		= json.mdototal;
				
			}
		}
	);
	
}

function excluiItemMatriz( mdoid, acao ){
	if( confirm("Deseja realmente excluir este item?") ){
		window.location.href = "emi.php?modulo=principal/matriz&acao=" + acao + "&requisicao=excluiritem&mdoid=" + mdoid;
	}
}

function excluiItemMatrizGap( mdoid, acao ){
	if( confirm("Deseja realmente excluir este item?") ){
		window.location.href = "emi.php?modulo=principal/matrizGap&acao=" + acao + "&requisicao=excluiritemGap&mdoid=" + mdoid;
	}
}

function calculaTotalProfissionais(mcpid)
{
	jQuery.noConflict();
	var num_prof   = replaceAll(jQuery("[name='num_prof_" + mcpid + "']").val(),'.','')*1;
	var num_equipe = replaceAll(jQuery("[name='num_equipe_" + mcpid + "']").val(),'.','')*1;
	var num_outros = replaceAll(jQuery("[name='num_outros_" + mcpid + "']").val(),'.','')*1;
	document.getElementById('td_total_profissionais_' + mcpid ).innerHTML = this.value=mascaraglobal('[.###]',num_prof + num_equipe + num_outros);
}

function addAcaoAtividade(emeid,mcpid,tppid)
{
	var janela = window.open("?modulo=principal/popupAcaoGap&acao=A&emeid=" + emeid + "&mcpid=" + mcpid + "&tppid=" + tppid, "acaoatividade", "menubar=no,toolbar=no,scrollbars=yes,resizable=no,left=20,top=20,width=800,height=500");
	janela.focus();
}

function salvaDadosGap()
{
	var erro = 0;
	if(document.formulario.papcaoatividade.value == ''){
		alert('Informe a Atividade!');
		erro = 1;
		return false;
	}
	if(document.formulario.papmeta.value == ''){
		alert('Informe a Meta!');
		erro = 1;
		return false;
	}
	if(erro == 0){
		document.formulario.submit();
	}
}

function salvarProfissionais(mcpid)
{
	document.emiSelecionaEscolas.submit();
}

function totalBeneficiarios(ano)
{
	var num_1 = replaceAll(document.emiSelecionaEscolas.mat_1.value,'.','')*1;
	var num_2 = replaceAll(document.emiSelecionaEscolas.not_1.value,'.','')*1;
	var num_3 = replaceAll(document.emiSelecionaEscolas.vesp_1.value,'.','')*1;
	
	var num_4 = replaceAll(document.emiSelecionaEscolas.mat_2.value,'.','')*1;
	var num_5 = replaceAll(document.emiSelecionaEscolas.not_2.value,'.','')*1;
	var num_6 = replaceAll(document.emiSelecionaEscolas.vesp_2.value,'.','')*1;
	
	var num_7 = replaceAll(document.emiSelecionaEscolas.mat_3.value,'.','')*1;
	var num_8 = replaceAll(document.emiSelecionaEscolas.not_3.value,'.','')*1;
	var num_9 = replaceAll(document.emiSelecionaEscolas.vesp_3.value,'.','')*1;
	
	document.getElementById('td_total_beneficiarios').innerHTML = this.value=mascaraglobal('[.###]',num_1 + num_2 + num_3 + num_4 + num_5 + num_6 + num_7 + num_8 + num_9);
	
}

function salvarBeneficiarios()
{
	document.emiSelecionaEscolas.submit();
}

function abreRelatorioDespesas(entid)
{
	window.location = 'emi.php?modulo=relatorio/classificacaoDespesas&acao=A&escola=' + entid;
}