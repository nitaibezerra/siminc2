var btSalvar	= document.getElementById("bt_salvar");
var btCancelar	= document.getElementById("bt_cancelar");
var form		= document.getElementById("formMinutaConvenio");

document.getElementById('texto').value = document.getElementById('imitexto').value;

tinyMCE.init({
		// General options
		mode : "textareas",
		theme : "advanced",
		language: "pt",
		editor_selector : "emendatinymce",
		plugins : "safari,pagebreak,style,layer,table,save,advhr,advimage,advlink,emotions,iespell,inlinepopups,insertdatetime,preview,media,searchreplace,print,contextmenu,paste,directionality,fullscreen,noneditable,visualchars,nonbreaking,xhtmlxtras,template,wordcount",

		// Theme options
		theme_advanced_buttons1 : "bold,italic,underline,strikethrough,|,justifyleft,justifycenter,justifyright,justifyfull,|,formatselect,fontselect,fontsizeselect",
		theme_advanced_buttons2 : "cut,copy,paste,pastetext,pasteword,|,search,replace,|,bullist,numlist,|,outdent,indent,blockquote,|,undo,redo,|,link,unlink,anchor,image,cleanup,help,code,|,insertdate,inserttime,preview,|,forecolor,backcolor",
		theme_advanced_buttons3 : "tablecontrols,|,hr,removeformat,visualaid,|,sub,sup,|,charmap,emotions,iespell,media,advhr,|,print,|,ltr,rtl,|,fullscreen",
		theme_advanced_buttons4 : "insertlayer,moveforward,movebackward,absolute,|,styleprops,|,cite,abbr,acronym,del,ins,attribs,|,visualchars,nonbreaking,template,pagebreak",
		theme_advanced_toolbar_location : "top",
		theme_advanced_toolbar_align : "left",
		theme_advanced_statusbar_location : "",
		theme_advanced_resizing : true,

		// Example content CSS (should be your site CSS)
		content_css : "css/content.css",

		// Drop lists for link/image/media/template dialogs
		template_external_list_url : "lists/template_list.js",
		external_link_list_url : "lists/link_list.js",
		external_image_list_url : "lists/image_list.js",
		media_external_list_url : "lists/media_list.js",

		// Replace values for the template plugin
		template_replace_values : {
			username : "Some User",
			staffid : "991234"
		}
	});
	
function excluirIntervenienteConvenio(itcid, pcmid){

	if(confirm('Deseja realmente excluir este registro?')) {
		var myajax = new Ajax.Request('emenda.php?modulo=principal/minutaConvenio&acao=A', {
		        method:     'post',
		        parameters: '&excluir=true&itcid='+itcid,
		        asynchronous: false,
		        onComplete: function (res){
					alert('Operação Realizada com Sucesso!');
					listaIntervenienteConvenio( pcmid );
		        }
		  });
	}
}

function listaIntervenienteConvenio(pmcid){
	var myajax = new Ajax.Request('emenda.php?modulo=principal/minutaConvenio&acao=A', {
		        method:     'post',
		        parameters: '&listainterveniente=true&pmcid='+pmcid,
		        asynchronous: false,
		        onComplete: function (res){
					$('lista').innerHTML = res.responseText;
		        }
		  });
}

function popUpManterInterveniente(itcid, pcmid) {
	var janela = window.open( 'emenda.php?modulo=principal/popupAssociaInterveniente&acao=A&itcid='+itcid+'&pcmid='+pcmid, 'popup_Interveniente_altera', 'width=800,height=250,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();;
}
function popUpIncluirCronogramaDesenbolso() {
	document.getElementById('labelCronograma').innerHTML = 'Clique aqui para visualizar o Cronograma Desembolso do Convenio';	
	var janela = window.open( 'emenda.php?modulo=principal/cronogramaExecuccaoDesembolso&acao=A&chamada=minutaConvenio', 'popup_Cronograma', 'width=800,height=700,status=0,menubar=0,toolbar=0,scrollbars=1,resizable=0' );
	janela.focus();
}

function salvarMinutaIniciativa() {
	
	/*if(document.getElementById("boCronograma").value == ""){
		alert('É necessário cadastrar o cronograma de desembolso do convenio');
		return false;
	} else */
	if($('vigdias').value == ""){
		alert('O campo dias de vigência e de preenchimento obrigatório');
		$('vigdias').focus();
		return false;
	}
	
	if(document.getElementById("bointerveniente")) {
		if(document.getElementById("bointerveniente").value ){
			if($('bointercadastrado')) {
				if($('bointercadastrado').value != "true" ){
					alert('Para salvar os dados e necessário vincular os intervenientes e seus dirigentes!');
					return false;
				}
			}
		}
	}
	selectAllOptions( document.getElementById( 'obcid' ) );
	
	var obcid	  = document.getElementById("obcid");
	var j = obcid.options.length;
	if( obcid.options[0].value == '' ){
		alert('Objeto de convênio é de preenchimento obrigatório');
		return false;
	}
	$('bt_salvar').disabled 	= true;
	$('bt_cancelar').disabled 	= true;
	document.getElementById("submetido").value = "1";
	document.getElementById("salvar1").value = "1";
	form.submit();
}

function salvarMinutaTermoAditivo() {
	selectAllOptions( document.getElementById( 'obcid' ) );
	
	var obcid	  = document.getElementById("obcid");
	var mdoid	  = document.getElementById("mdoid").value;
	var j = obcid.options.length;
	if( obcid.options[0].value == '' ){
		alert('Objeto de convênio é de preenchimento obrigatório');
		return false;
	}
	
	if( mdoid == '' ){
		alert('Modelo do termo é de preenchimento obrigatório');
		return false;
	}
	
	document.getElementById("submetido").value = "1";	
	document.getElementById("salvar1").value = "1";
	form.submit();
}

function carregaMinutaIniciativa() {
	var mdoid = document.getElementById('modelo').value;
	document.getElementById("submetido").value = "1";
	document.getElementById("mdoid").value = mdoid;
	selectAllOptions( document.getElementById( 'obcid' ) );
	$('bt_salvar').disabled 	= true;
	$('bt_cancelar').disabled 	= true;
	if( mdoid ){
		salvarMinutaDados();
		form.submit();
	}
}
function carregaTexto( mdoid ) {
	selectAllOptions( document.getElementById( 'obcid' ) );
	document.getElementById("submetido").value = "1";
	//document.getElementById("mdoid").value = mdoid;
	form.submit();
}

function carregaMinutaPTA(pmcid) {
	document.getElementById("submetido").value = "1";
	document.getElementById("pmcid").value = pmcid;
	form.submit();
}

/**
	Função criada para salvar a minuta toda vez que houver alteração no campo objeto, resolução e dias de vigencia,
	porque o usuario alterava estes campos mais não clicava no botão salvar e sim selecionava outro modelo de documento,
	com isso perdia todos os dados digitados nestes campos.
*/
function salvarMinutaDados(){
	var myajax = new Ajax.Request('emenda.php?modulo=principal/minutaConvenio&acao=A', {
		        method:     'post',
		        parameters: '&alterarminuta=true&'+$('formMinutaConvenio').serialize(),
		        asynchronous: false,
		        onComplete: function (res){
					$('erro').innerHTML = res.responseText;
		        }
		  });
}

function imprimirMinutaPTA(pmcid) {
	var janela = window.open("emenda.php?modulo=principal/popupDicaImpressao&acao=A&parametro=" + pmcid+'&tipo=M',"relatorio", "toolbar=no,location=no,status=yes,menubar=yes,scrollbars=yes,resizable=no,height=600,width=800");
	janela.focus();
}
function imprimirMinutaTermoPTA(refid) {
	var janela = window.open("emenda.php?modulo=principal/popupDicaImpressao&acao=A&parametro=" + refid+'&tipo=T',"relatorio", "toolbar=no,location=no,status=yes,menubar=yes,scrollbars=yes,resizable=no,height=600,width=800");
	janela.focus();
}

function carregaTipoMinuta( tipo ){
	if( tipo == 'T' )
		window.location.href = 'emenda.php?modulo=principal/minuta&acao=A&tipo=T';
	else
		window.location.href = 'emenda.php?modulo=principal/minuta&acao=A&tipo=C';
}

function carregaMinutaConvenio( pmcid ){
	document.getElementById("pmcid").value = pmcid;
	document.getElementById("mdoid").value = '';
	document.getElementById("salvar1").value = "alterar";
	document.getElementById("submetido").value = "";
	form.submit();
}
function carregaTermoAditivo( refid, ptrid ){
	document.getElementById("refid").value = refid;
	document.getElementById("ptridalterar").value = ptrid;
	document.getElementById("mdoid").value = '';
	document.getElementById("salvar1").value = "alterar";
	document.getElementById("submetido").value = "";
	form.submit();
}