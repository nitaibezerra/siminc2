function abreProtocolo( ){
	window.open('ies.php?modulo=principal/verProtocolo&acao=A', 'Protocolo', 'width=500,height=300');
}

function iesListaInstituicao( esdid ){
	window.location.href = "ies.php?modulo=principal/listaInstituicoes&acao=A&esdid=" + esdid;
}

function iesFiltrarLista(){
	document.getElementById('formFiltro').submit();
}

function abreDadosIES( iesid ){
	window.location.href = "ies.php?modulo=principal/dadosResponsavel&acao=C&iesid=" + iesid;
}

function cancelaIES( iesid ){
	if( confirm("Deseja realmente inativar este projeto?") ){
		window.location.href = "ies.php?modulo=principal/listaInstituicoes&acao=A&requisicao=cancelarprojeto&iesid=" + iesid;
	}
}

function abreProjetoIES( pbiid ){
}

function iesSalvarProjeto(){
	
	var projeto = document.getElementById('projeto').value;
	
	if ( projeto == '' ){
		alert('Favor anexar o Projeto!');
		return false;
	}else{
		document.getElementById('formProjeto').submit();
	}
	
}

function iesDownloadArquivo( arqid, acao ){
	window.location.href = "ies.php?modulo=principal/projeto&acao=" + acao + "&requisicao=downloadarquivo&arqid=" + arqid;
}

function iesExcluiAnexo( aprid ){
	if( confirm("Deseja realmente excluir este arquivo?") ){
		window.location.href = "ies.php?modulo=principal/projeto&acao=A&requisicao=excluirarquivo&aprid=" + aprid;
	}
}

function iesPreencherIntencao(){
	window.location.href = "ies.php?modulo=principal/dadosInstituicao&acao=A";
}

function iesConfirmarDados( iesid ){
	window.location.href = "ies.php?modulo=principal/dadosResponsavel&acao=A&requisicao=criarprojeto&iesid=" + iesid;
}