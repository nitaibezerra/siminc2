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

function cancelarReformulacao(){
	window.location.href = 'emenda.php?modulo=principal/reformulacao&acao=A';
}
function salvarRegistro ()
{
	var objs = document.getElementById('opcoes').getElementsByTagName('input');
	var checked = 0;
	
	// Percurre opcoes
	for (i = 0; i < objs.length; i++)
	{
		// Guarda quais opcoes estao marcadas
		if (objs[i].checked == true) checked = 1;
	}

	if (checked == 0)
	{
		alert('Nenhuma opção para reformulação foi selecionada.');
		
		return false;
	}
	else
	{
		if (document.getElementById(1).style.display == '')
		{
			if (document.getElementById('mdoid').value == '0')
			{
				alert('O campo Modelo do Termo aditivo é um campo obrigatório');
				return false;
			}
		}
			
		if (document.getElementById(3).style.display == '')
		{
			if (salvarAssinatura() == false) return false;
		}
	}

	document.formReformulacao.submit();
}

function alteracao (obj, i)
{
	var objs = document.getElementById('opcoes').getElementsByTagName('input');
	
	var sit = i;
	var i = 0;
	var j = 0;
	var formulario = new Array();
	var loadform = true;
	
	// Percurre opcoes
	for (i = 0; i < objs.length; i++)
	{
		if (document.getElementsByName("trefid")[i].checked == true) {
			formulario[document.getElementsByName("trefid")[i].value] = 1;
			loadform = false;
		}
		
		
		/*
		if( document.getElementById(document.getElementsByName("trefid")[i].value) ){
			document.getElementById(document.getElementsByName("trefid")[i].value).style.display = 'none';
		}
		*/
		// Guarda quais opcoes estao marcadas
//		if (document.getElementsByName("trefid")[i].checked == true) {
	//		alert(document.getElementsByName("trefid")[i].value);
//			formulario[objs[i].value] = 1;
//			loadform = false;
//		}
		// Fecha todos os formularios
//		document.getElementById(objs[i].value).style.display = 'none';
	}
	
	var radiobtn = '';
	if(loadform == false) {
		for(i=0;i<document.getElementsByName("situacao").length;i++) {
			if(document.getElementsByName("situacao")[i].checked) {
				radiobtn = document.getElementsByName("situacao")[i].value;
			}
		}
		if(radiobtn == '') {
			alert('Selecione a situação');
			obj.checked = false;
			return false;
		}
	}
	

	// Percorre opcoes marcadas
	for (var i in formulario)
	{
		if (document.getElementById(i))
		{
			// Abre formulário selecionado
			
			if(radiobtn == 'A' && i == 1) {
				document.getElementById("1").style.display = 'none';
			} else {
				if(radiobtn == 'T'){
					document.getElementById("1").style.display = '';
				}
				document.getElementById(i).style.display = '';
			}



			
			// Sempre abre o formulário 1 quando outro formulário esta aberto e a situação for termo aditivo.
			//if (document.getElementById('situacao').value == 'T'){
			//	document.getElementById(1).style.display = '';
			//}
		}
	}

	document.getElementById("1").style.display = 'none';
	
//	if( sit == 2 ){
//		document.getElementById(2).style.display = '';
//	} else {
//		document.getElementById(2).style.display = 'none';
//	}
	
	if( sit == 3 ){
		if( $('diasVencimento').value < 60 ){
			alert('A solicitação de prorrogação de vigência só pode ser realizada com no máximo 60 dias de antecedência do seu vencimento.');
			obj.checked = false;
			document.getElementById(3).style.display = 'none'
			//window.location.href = 'emenda.php?modulo=principal/reformulacao&acao=A';
			return false;
		}
	}else {
		document.getElementById(3).style.display = 'none';
	}
}
/**
	Atualiza o texto do richtext ja com as alteracoes de macro
*/
function carregaTexto (){
	var myajax = new Ajax.Request('emenda.php?modulo=principal/reformulacao&acao=A', {
		        method:     'post',
		        parameters: '&mdoid='+document.getElementById('mdoid').value,
		        asynchronous: false,
		        onComplete: function (res){
		        	var ed = tinyMCE.get('texto');
		        	ed.setContent(res.responseText);					
		        }
		  });
}

function calculaDiasVigencia(){
	var datainicio = $('vigdatainicio').value;
	var diavigencia = $('vigdias').value;	 
	
	if( diavigencia && datainicio){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/reformulacao&acao=A', {
		        method:     'post',
		        parameters: '&pegaDataFinal=true&vigdias='+diavigencia+'&vigdatainicio='+datainicio,
		        asynchronous: false,
		        onComplete: function (res){
		        	var retorno = res.responseText.split('_');
					$('vigdatafim').value = retorno[0];
					$('diasVencimento').value = retorno[1];
		        }
		  });
	}	
}

function salvarAssinatura(){
	var dataFin = document.getElementById('vigdatafim');
	var vigdias = document.getElementById('vigdias').value;
	var vigdias_atual = document.getElementById('vigdias_atual').value;
	
	if( $('vigdatainicio').value == "" ){
		alert( 'O campo Data Celebração é de preenchimento obrigatório!' );
		$('vigdatainicio').focus();
		return false;
	}
	if( $('vigdias').value == "" ){
		alert( 'O campo Dias de Vigência é de preenchimento obrigatório!' );
		$('vigdatainicio').focus();
		return false;
	}	
	if(!validaData(document.getElementById('vigdatainicio'))) {
		alert('Data de inicio incorreta');
		return false;
	}
	
	if( $('diasVencimento').value < 60 ){
		alert('A solicitação de prorrogação de vigência só pode ser realizada com no máximo 60 dias de antecedência do seu vencimento.');
		window.location.href = 'emenda.php?modulo=principal/reformulacao&acao=A';
		return false;
	}

	if( vigdias < vigdias_atual ){
		alert( 'O numero de dias nao pode ser menor que o numero atual do convenio: '+ vigdias_atual + ' dias!');
		return false;
	}
	
	if(  !validaDataMaiorQueHoje(dataFin) ){
		alert("A data final não pode ser menor que a atual.");
		return false;
	}

	return true;
}

alteracao(0,0);
calculaDiasVigencia();