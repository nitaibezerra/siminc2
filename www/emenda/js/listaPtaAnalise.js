function pesquisaPTAAnalise(){
	enbnome	=	document.getElementsByName("enbnome")[0];
	
	if(enbnome.value.length > 0 && enbnome.value.length < 3) {
		alert("Para realizar a busca por nome do município é necessário informar pelo menos 3 caracteres.");
		enbnome.focus();
		btnPesquisar.disabled 	= false;
		return;
	}

	
	if(document.getElementById('espid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById('espid') );
	}
	document.getElementById('requisicao').value = 'filtrapesquisa';
	document.getElementById('formulario').action = "";
	document.getElementById('formulario').target = '';
	document.getElementById('formulario').submit();
}
function selecionaPlanoTrabalho( ptrid, esdid ){
	
	switch( esdid ){
		case em_elaboracao:
		case em_elaboracao_impositivo:
	  	case em_analise_do_fnde:
	  	case em_identificacao_processo_impositivo:
	  	case em_ana_ref_processo:
	  	case em_diligencia:
	  	case em_identificacao_processo_reformulacao:
	  		window.location = "emenda.php?modulo=principal/informacoesGerais&acao=A&ptridAnalise=" + ptrid;
	  	break;
		case em_analise_dados:
		case vinculacao_unidades_gestoras_impositivo:
			window.location = "emenda.php?modulo=principal/analiseDadosPTA&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_analise_de_merito:
	  	case em_analise_de_merito_impositivo:
	  		window.location = "emenda.php?modulo=principal/analiseMeritoPTA&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_analise_tecnica:
	  	case em_analise_tecnica_impositivo:
	  	case em_analise_tec_reformulacao:
	  	case em_convenio_cancelado:
	  	case em_convenio_vigencia_expirada:
	  	case em_pta_indeferido:
	  		window.location = "emenda.php?modulo=principal/analiseTecnicaPTA&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_empenho:
	  	case em_empenho_impositivo:
	  	case em_empenho_reformulacao:
	  		window.location = "emenda.php?modulo=principal/execucaoPTA&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_geracao_minuta_convenio:
	  	case em_analise_profe:
	  	case em_geracao_ta:
	  		window.location = "emenda.php?modulo=principal/minuta&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_criacao_minuta_convenio:
	  		window.location = "emenda.php?modulo=principal/criarConvenioPTA&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_assinatura_concedente:
	  	case em_assinatura_convenente:
	  	case em_assinatura_reformulacao:
	  		window.location = "emenda.php?modulo=principal/assinaturasPTA&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_publicacao_no_dou:
	  	case em_pre_convenio:
	  	case em_analise_juridica:
	  	case em_republicacao:
	  	case em_publicacao_reformulacao:
	  	case em_analise_juridica_reformulacao:
	  		window.location = "emenda.php?modulo=principal/minutaConvenioDOU&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_liberacao_recurso:
	  	case recurso_liberado:
	  	case em_prestacao_conta:
	  	case em_liberacao_recurso_reformulacao:
	  		window.location = "emenda.php?modulo=principal/analisepta/pagamento/solicitar&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_solicitacao_reformulacao:
	  	case em_aceitacao_reformulacao:
	  	case em_reformulacao:
	  	case em_processo_reformulado:
	  	case em_reformulacao_processo:
	  		window.location = "emenda.php?modulo=principal/reformulacao&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_orcamento_impositivo:
	  		window.location = "emenda.php?modulo=principal/orcamentoImpositivo&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	case em_proposta_siconv:
	  		window.location = "emenda.php?modulo=principal/propostaSiconv&acao=A&ptridAnalise=" + ptrid;
	  	break;
	  	default:
	  		window.location = "emenda.php?modulo=principal/informacoesGerais&acao=A&ptridAnalise=" + ptrid;
	  	break;
	}

}
function exportarAnaliseExcel(){
	/*document.getElementById('btnPesquisar').disabled = true;
	document.getElementById('btnExportar').disabled = true;*/
	document.getElementById('formulario').action = "";
	document.getElementById('formulario').target = '';
	document.getElementById('requisicao').value = 'exportar';
	document.getElementById('formulario').submit();
}

function enviarEmailAnaliseEntidade(){
	enbnome	=	document.getElementsByName("enbnome")[0];
	
	if(enbnome.value.length > 0 && enbnome.value.length < 3) {
		alert("Para realizar a busca por nome do município é necessário informar pelo menos 3 caracteres.");
		enbnome.focus();
		btnPesquisar.disabled 	= false;
		return;
	}

	
	if(document.getElementById('espid_campo_flag').value == "1"){
		selectAllOptions( document.getElementById('espid') );
	}
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
	
	var form = document.getElementById('formulario');
	document.getElementById('requisicao').value = 'filtraemail';
	
	form.action = "emenda.php?modulo=principal/enviarEmailEmenda&acao=A";
	form.target = 'page';
	var janela = window.open('emenda.php?modulo=principal/enviarEmailEmenda&acao=A&tipoEmail=analisePTA','page','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'');
	janela.focus();
	form.submit();
	
	/*var largura = 673;
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
	
	window.open('emenda.php?modulo=principal/enviarEmailEmenda&acao=A&tipoEmail=analisePTA','','height=' + altura + ', width=' + largura + ', top='+meio1+', left='+meio2+'');*/
}

function onOffCampo( campo ){
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	
	if ( div_on.style.display == 'none' ){
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}else{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}
