function conjur_DownloadArquivo( id ){
	window.location = '/conjur/conjur.php?modulo=principal/documento&acao=A&requisicao=download&arqid='+id;
}

function $_(input)
{
	return document.getElementById(input);
}

visualizarProcessos = function(tp){
	var form = window.document.getElementById("pesquisar");
	form.submit();
}

function CrossEvent(evt)
{
	evt = evt? evt: (window.event? window.event: null);
	if (evt)
	{
		this.originalEvent = evt;
		this.type = evt.type;
		this.screenX = evt.screenX;
		this.screenY = evt.screenY;

		// IE: srcElement
		this.target = evt.target? evt.target: evt.srcElement;

		// N4: modifiers
		if (evt.modifiers)
		{
			this.altKey   = evt.modifiers & Event.ALT_MASK;
			this.ctrlKey  = evt.modifiers & Event.CONTROL_MASK;
			this.shiftKey = evt.modifiers & Event.SHIFT_MASK;
			this.metaKey  = evt.modifiers & Event.META_MASK;
		}
		else
		{
			this.altKey   = evt.altKey;
			this.ctrlKey  = evt.ctrlKey;
			this.shiftKey = evt.shiftKey;
			this.metaKey  = evt.metaKey;
		}

		// N4: which // N6+: charCode
		this.charCode = !isNaN(evt.charCode)? evt.charCode: !isNaN(evt.keyCode)? evt.keyCode: evt.which;
		this.keyCode = !isNaN(evt.keyCode)? evt.keyCode: evt.which;
		this.button = !isNaN(evt.button)? evt.button: !isNaN(evt.which)? evt.which-1: null;
		this.debug = "c:" + evt.charCode + " k:" + evt.keyCode
			+ " b:" + evt.button + " w:" + evt.which;
	}
};

function dataType(Campo,mask){
var self = Campo;
self.mask = mask;
self.event = null;
self.stringNoFormat = null;
self.stringFormat = Campo.value;
	/**								**/	
	self.noFormat = function(){
		var charReplace = new String(self.value);
		self.stringNoFormat = charReplace.replace(/[\(\)(/)-.]/g, "");
	};
	/**								**/	
	self.numbersonly = function (event){
		self.event = new CrossEvent(event);
		keychar = String.fromCharCode(self.event.charCode);
		var stValidos = "0123456789";
		if ( stValidos.indexOf(keychar) == -1 && self.event.charCode != 0) {
			return false;
		}else{
			return true;
		}
	};
	/**								**/	
	self.formataCampos = function(event){
		self.event = new CrossEvent(event);
		arrmask = new Array( self.mask.length);
		for (var i = 0 ; i <self.mask.length; i++)
		{ 
			arrmask[i] = self.mask.slice(i,i+1) 
		}
		if ( ( ( ( ( arrmask[ self.value.length ] == "#" ) || ( arrmask[ self.value.length ] == "9" ) ) ) || 
			   ( ( ( arrmask[ self.value.length + 1 ] != "#" ) || ( arrmask[ self.value.length + 1 ] != "9" ) ) ) 
			  ) ) 
		{ 
			
			if ((self.event.charCode >= 37 && self.event.charCode <= 40)||(self.event.charCode >= 48 && self.event.charCode <= 57)||(self.event.charCode >= 96 && self.event.charCode <= 105)||(self.event.charCode == 9) ||(self.event.charCode == 46) ||(self.event.charCode == 13))
			{ 				
				self.Organiza_Casa(arrmask[self.value.length],self.event.charCode);
			} 
			else 
			{ 
				self.Detona_Event();
			} 
		} 		
		
	};
	
	self.CaracterNumerico = function(chCaractere){
		var retorno;
		var stValidos = "0123456789";
		if ( stValidos.indexOf(chCaractere) == -1 ) {
			retorno = false;
		}else{
			retorno = true;
		}
		//alert(chCaractere  + retorno);
		return retorno;
	};
	
	/**								**/		
	self.Organiza_Casa = function(arrpos,teclapres_key) {
		if (((arrpos == "/") || (arrpos == ".") || (arrpos == ",") || (arrpos == ":") || (arrpos == " ") || (arrpos == "-")) && !(teclapres_key == 8)) { 
			separador = arrpos; 
			masktext = self.value + separador;
			self.value = masktext;
		} 
	};
	/**								**/		
	self.Detona_Event = function() 
	{ 
		if ( self.value != "" ) 
		{ 
			self.value = self.value;
		}
	};
	
	self.noFormat();
	return self;
};

function classTime(campo){
var self = new dataType(campo,'99:99');
self.datatype = 'hora';
//self.setAttribute('maxlength', 4);

	self.onkeypress = function(event){
		self.formataCampos(event);
		return self.numbersonly(event);
	};
	return self;
};

function classCpf(campo){
var self = new dataType(campo,'999.999.999-99');
self.datatype = 'cpf';
self.setAttribute('maxlength', 14);
	/**								**/	
	self.onkeypress = function(event){
		self.formataCampos(event);
		return self.numbersonly(event);
	};
	/**								**/	
	self.validaCpf =  function()
	{
		if(self.stringNoFormat.length == 0){
			return true;
		}
		var numeros, digitos, soma, i, resultado, digitos_iguais;
		digitos_iguais = 1;
		if (self.stringNoFormat.length < 11){
			return false;
		}
		for (i = 0; i < self.stringNoFormat.length - 1; i++){
			if (self.stringNoFormat.charAt(i) != self.stringNoFormat.charAt(i + 1))
			{
				digitos_iguais = 0;
				break;
			}
		}	
			if (!digitos_iguais)
			{
				numeros = self.stringNoFormat.substring(0,9);
				digitos = self.stringNoFormat.substring(9);
				soma = 0;
				for (i = 10; i > 1; i--){
					soma += numeros.charAt(10 - i) * i;
				}
				resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
				
				if (resultado != digitos.charAt(0)){
					return false;
				}
				numeros = self.stringNoFormat.substring(0,10);
				soma = 0;
				for (i = 11; i > 1; i--){
					soma += numeros.charAt(11 - i) * i;
				}
				resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
				
				if (resultado != digitos.charAt(1)){
					return false;
				}
					return true;
			}else{
				return false;
			}
	};
	
	return self;
	
};

function classCnpj(campo){
var self = new dataType(campo,'99.999.999/9999-99');
self.datatype = 'cnpj';
self.setAttribute('maxlength', 18);

	/**								**/	
	self.onkeypress = function(event){
		self.formataCampos(event);
		return self.numbersonly(event);
	};
	self.validaCnpj = function(){
		if(self.stringNoFormat.length == 0){
			return true;
		}
		var numeros, digitos, soma, i, resultado, pos, tamanho, digitos_iguais;
		digitos_iguais = 1;
		if (self.stringNoFormat.length < 14 && self.stringNoFormat.length < 15){
			return false;
		}
		for (i = 0; i < self.stringNoFormat.length - 1; i++){
			if (self.stringNoFormat.charAt(i) != self.stringNoFormat.charAt(i + 1))
			{
				digitos_iguais = 0;
				break;
			}
		}
		if (!digitos_iguais)
		{
			tamanho = self.stringNoFormat.length - 2;
			numeros = self.stringNoFormat.substring(0,tamanho);
			digitos = self.stringNoFormat.substring(tamanho);
			soma = 0;
			pos = tamanho - 7;
			for (i = tamanho; i >= 1; i--)
			{
				soma += numeros.charAt(tamanho - i) * pos--;
				if (pos < 2){
					pos = 9;
				}
			}
			resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
			if (resultado != digitos.charAt(0)){
				return false;
			}
			tamanho = tamanho + 1;
			numeros = self.stringNoFormat.substring(0,tamanho);
			soma = 0;
			pos = tamanho - 7;
			for (i = tamanho; i >= 1; i--)
			{
				soma += numeros.charAt(tamanho - i) * pos--;
				if (pos < 2){
					pos = 9;
				}
			}
			resultado = soma % 11 < 2 ? 0 : 11 - soma % 11;
			if (resultado != digitos.charAt(1)){
                  return false;
			}
            return true;
		}
		else{
			return false;
		}
            
	};	
return self;
};

function openPopup(url){
	return windowOpen(url,'blank','height=450,width=400,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}

function inserirInteressado(){
		alert('dsadasd');
		return windowOpen( '?modulo=principal/inserir_interessado&acao=A','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}
function atualizaInteressado(ent){
	var entid = ent.replace('linha_', '');
	return windowOpen( '?modulo=principal/inserir_interessado&acao=A&busca=entnumcpfcnpj&entid=' + entid + '&tr=' + ent,'blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' )	;
}


/* FUNÇÕES UTILIZADAS NO CADASTROPROCESSO/EDITARPROCESSO.INC */
function apagarProcesso(prcid){
	if(confirm('Deseja realmente excluir o Processo?')){
		document.getElementById('evento').value = 'E';
		document.getElementById('prcid').value = prcid;
		document.getElementById('formulario').submit();
	}
}

function submeteFormCadastroProcesso() {
	document.getElementById('botaosubmeter').disabled=true;
	
	if(document.formulario.elements['tprid'].value == "") {
		document.formulario.elements['tprid'].focus();
		alert('"Processo na CONJUR" é obrigatório');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	var campoNumeroProcessoSidoc = document.getElementById('prcnumsidoc');
	
	if(document.formulario.elements['prcnumsidoc'].value == "") {
		alert('"Número do Processo " ' + document.formulario.elements['tipoNumeracao'].value + ' é obrigatório');
		document.getElementById('prcnumsidoc').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
 
	if (document.formulario.elements['tipoNumeracao'].value == "SIDOC")
	{
		if(!conferirDigitoVerificadorModulo11(campoNumeroProcessoSidoc)){
			document.getElementById('botaosubmeter').disabled=false;
			return false;
		}
	}
	else
	{
		if(document.formulario.elements['prcnumsidoc'].value.length != 9 ){
			alert('Número EMEC inválido');
			document.getElementById('botaosubmeter').disabled=false;
			return false;
		}
	}
	
	if(document.formulario.elements['prcnomeinteressado'].value == "") {
		alert('"Nome do Interessado" é obrigatório');
		document.getElementById('prcnomeinteressado').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	if(document.formulario.elements['prcdtentrada'].value == "") {
		alert('"Data de Entrada do Processo" é obrigatório');
		document.getElementById('prcdtentrada').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	if(document.formulario.elements['prcdtentrada'].value == "") {
		alert('"Data de Entrada do Processo" é obrigatório');
		document.getElementById('prcdtentrada').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	if(!validaData(document.formulario.elements['prcdtentrada'])) {
		alert('"Data de Entrada do Processo" é inválida');
		document.getElementById('prcdtentrada').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}

	if(document.formulario.elements['unpid'].value == "") {
		alert('"Tipo de Procedência" é obrigatório');
		document.getElementById('unpid').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	
	if(document.formulario.elements['unpid'].value !="") {
		if(document.formulario.elements['prodsc'].value == "") {
			alert('"Procedência" é obrigatório');
			document.getElementById('prodsc').focus();
			document.getElementById('botaosubmeter').disabled=false;
			return false;
		}
	}
	
	/*if(document.formulario.elements['tasdsc'].value == "") {
		alert('"Tema" é obrigatório');
		document.getElementById('tasdsc').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}*/
	
	/*if(document.formulario.elements['tacid'].value == "") {
		alert('"Tipo de Ação" é obrigatório');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}*/
	
	if(document.formulario.elements['tipid'].value == "") {
		alert('"Prioridade" é obrigatório');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}

	if(document.formulario.elements['prcnomeinteressado'].value == "") {
		alert('"Interessado" é obrigatório');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	
	if(document.formulario.elements['prazo']){
		if(document.formulario.elements['prazo'].value == "") {
			alert('Campo obrigatório');
			document.formulario.elements['prazo'].focus();
			document.getElementById('botaosubmeter').disabled=false;
			return false;
		}
	}

	document.getElementById('formulario').submit();
}


function editaFormCadastroProcesso() {
	document.getElementById('botaosubmeter').disabled=true;
	
	if(document.formulario.elements['tprid'].value == "") {
		document.formulario.elements['tprid'].focus();
		alert('"Processo na CONJUR" é obrigatório');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	var campoNumeroProcessoSidoc = document.getElementById('prcnumsidoc');
	
	if(document.formulario.elements['prcnumsidoc'].value == "") {
		alert('"Número do Processo " ' + document.formulario.elements['tipoNumeracao'].value + ' é obrigatório');
		document.getElementById('prcnumsidoc').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
 
	if(document.formulario.elements['prcnomeinteressado'].value == "") {
		alert('"Nome do Interessado" é obrigatório');
		document.getElementById('prcnomeinteressado').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	if(document.formulario.elements['prcdtentrada'].value == "") {
		alert('"Data de Entrada do Processo" é obrigatório');
		document.getElementById('prcdtentrada').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	if(document.formulario.elements['prcdtentrada'].value == "") {
		alert('"Data de Entrada do Processo" é obrigatório');
		document.getElementById('prcdtentrada').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	if(!validaData(document.formulario.elements['prcdtentrada'])) {
		alert('"Data de Entrada do Processo" é inválida');
		document.getElementById('prcdtentrada').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}

	if(document.formulario.elements['unpid'].value == "") {
		alert('"Tipo de Procedência" é obrigatório');
		document.getElementById('unpid').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	
	if(document.formulario.elements['unpid'].value !="") {
		if(document.formulario.elements['prodsc'].value == "") {
			alert('"Procedência" é obrigatório');
			document.getElementById('prodsc').focus();
			document.getElementById('botaosubmeter').disabled=false;
			return false;
		}
	}
	
	/*if(document.formulario.elements['tasdsc'].value == "") {
		alert('"Tema" é obrigatório');
		document.getElementById('tasdsc').focus();
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}*/
	
	/*if(document.formulario.elements['tacid'].value == "") {
		alert('"Tipo de Ação" é obrigatório');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}*/
	
	if(document.formulario.elements['tipid'].value == "") {
		alert('"Prioridade" é obrigatório');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}

	if(document.formulario.elements['prcnomeinteressado'].value == "") {
		alert('"Interessado" é obrigatório');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	
	if(document.formulario.elements['prazo']){
		if(document.formulario.elements['prazo'].value == "") {
			alert('Campo obrigatório');
			document.formulario.elements['prazo'].focus();
			document.getElementById('botaosubmeter').disabled=false;
			return false;
		}
	}

	document.getElementById('formulario').submit();
}

function abreJanelaCadastroInteressados() {
	windowOpen('?modulo=principal/inserir_interessado&acao=A','blank','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}


RemoveLinha = function(index) {
	table = window.document.getElementById("tabela_interessado");
	table.deleteRow(index);
}

removeExpressao = function(index) {
	table = window.document.getElementById("tabela_expressao");
	table.deleteRow(index);
}
/* FIM - FUNÇÕES UTILIZADAS NO CADASTROPROCESSO/EDITARPROCESSO.INC */

function abreJanelaCadastroExpressaoChave() {
	windowOpen('?modulo=principal/inserir_expressao_chave&acao=A','blank','height=200,width=450,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}


function deletarExpressao(idlinha) {
	if(confirm("Deseja realmente excluir a expressão?")) {
		var tabela = document.getElementById('tabela_expressao');
		var linha = tabela.rows[idlinha];
		tabela.deleteRow(linha.rowIndex);
	}
}

function editarExpressao(idlinha) {
	var tabela = document.getElementById('tabela_expressao');
	if(document.getElementById('expressao_chave').value) {
		cadastrarExpressao();
		var linha = tabela.rows[idlinha+1];
	} else {
		var linha = tabela.rows[idlinha];
	}
	document.getElementById('expressao_chave').value = linha.cells[1].innerHTML;
	tabela.deleteRow(linha.rowIndex);
}

function cadastrarExpressao() {
	var expressao = document.getElementById('expressao_chave').value;
	var tabela = document.getElementById('tabela_expressao');
	var contador = document.getElementById('contador_exp');
	var linha = tabela.insertRow(2);
	cell1 = linha.insertCell(0);
	cell2 = linha.insertCell(1);
	cell1.style.textAlign = "center";
	cell1.innerHTML = "<img src='/imagens/alterar.gif' style='cursor:pointer;' border='0' title='Alterar' onclick='editarExpressao(this.parentNode.parentNode.rowIndex);'> " +
					  "<img src='/imagens/excluir.gif' style='cursor:pointer;' border='0' title='Excluir' onclick='deletarExpressao(this.parentNode.parentNode.rowIndex);'>" +
				  	  "<input type='hidden' name='expressaochave[]' value='"+expressao+"'>";
	cell2.innerHTML = expressao;	  	  
	document.getElementById('expressao_chave').value = '';
}


function RemoveLinhaChaveExpressao(index) {
	table = window.document.getElementById("tabela_expressao");
	table.deleteRow(index);
}


function submeteFormAnexo() {
	document.getElementById('botaoanexosubmeter').disabled=true;
	if(document.anexo.elements['tpaid'].value == "") {
		alert('"Tipo de arquivo" é obrigatório');
		document.getElementById('botaoanexosubmeter').disabled=false;
		return false;
	}
	document.getElementById('anexo').submit();
}

function submeteFormExpediente() {
	document.getElementById('botaosubmeter').disabled=true;
	
	if(document.formulario.elements['expdtinclusaoadvogado'].value == "") {
		alert('"Data do parecer do advogado" é obrigatória');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}
	if(!validaData(document.formulario.elements['expdtinclusaoadvogado'])) {
		alert('"Data do parecer do advogado" é inválida');
		document.getElementById('botaosubmeter').disabled=false;
		return false;
	}

	if(document.formulario.elements['expdtinclusaoconjur'].value != '') {
		if(!validaData(document.formulario.elements['expdtinclusaoconjur'])) {
			alert('"Data do parecer da conjur" é inválida');
			document.getElementById('botaosubmeter').disabled=false;
			return false;
		}
	}
	
	document.getElementById('formulario').submit();
}


function Excluir(url, msg) {
	if(confirm(msg)) {
		window.location = url;
	}
}

function abreJanelaProcessosVinculados() {
	windowOpen('?modulo=principal/vincular_processo&acao=A','blank','height=600,width=800,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes');
}
function deletarlinhainteressado(t) {
	if(confirm("Deseja realmente excluir o interessado?")) {
		var tab = document.getElementById('tabela_interessado'); 
		var line = t.parentNode.parentNode.parentNode.rowIndex;
		tab.deleteRow(line);
	}
}

function deletarlinha(t,prvid) {
	
	if(confirm("Deseja realmente desanexar o processo vinculado?")) {

		if(!t.id) t.id = prvid;

		var tab = document.getElementById('tabela_vincprocessos');
		var line = t.parentNode.parentNode.parentNode.parentNode.rowIndex;
		tab.deleteRow(line);

		new Ajax.Request('conjur.php?modulo=principal/editarprocesso&acao=A&prvid='+t.id+'&req=deletaAnexo',
		  {
		    method:'get',
		    onSuccess: function(transport){
				alert("Registro desanexado com sucesso.");
				window.location.reload();
		    },
		    onFailure: function(){
				alert('Não foi possível desanexar o processo vinculado!');
				window.location.reload();
			}
		  });
	}
}

function verprocesso( prcid ){
	window.location = '?modulo=principal/editarprocesso&acao=A&prcid=' + prcid;
}

String.prototype.trim = function() {
   a = this.replace(/^\s+/, '');
   return a.replace(/\s+$/, '');
};

function conferirDigitoVerificadorModulo11(campoNumeroProcessoSidoc) {

	var txtDigitado = campoNumeroProcessoSidoc.value;
	var tamanho		= txtDigitado.length;

	if( tamanho == 15 ){
		var ano = parseInt(txtDigitado.substr(11,2));
		if( 90 <= ano && ano <= 99 ){
			txtDigitado = '00'+txtDigitado;
			tamanho		= txtDigitado.length;
		}else{
			return false;
		}
	}
	
	if( tamanho == 12 ){
		txtDigitado = '00000'+txtDigitado;
		tamanho		= txtDigitado.length;
	}
	
	if( tamanho == 17 ){
		
		var digito = txtDigitado.substr(15,2);
		var modulo1 = 0;
		var i = 16;
		
		for( x = 0; x<15; x++ ){
			modulo1 = modulo1 + (txtDigitado.substr(x,1) * i);
			i--;
		}
		
		modulo1 = modulo1%11;
		
		if( modulo1 == 1 ){
			modulo1 = 0;
		}else if( modulo1 == 0){
			modulo1 = 1;
		}else{
			modulo1 = 11 - modulo1;
		}
		
		var modulo2 = 0;
		var i = 17;
		
		for( x = 0; x<15; x++ ){
			modulo2 = modulo2 + (txtDigitado.substr(x,1) * i);
			i--;
		}
		
		modulo2 = modulo2 + modulo1 * i;
		modulo2 = modulo2%11;
		
		if( modulo2 == 1 ){
			modulo2 = 0;
		}else if( modulo2 == 0){
			modulo2 = 1;
		}else{
			modulo2 = 11 - modulo2;
		}
		
		var digito2 = ''+parseInt(modulo1)+parseInt(modulo2);
		
		if( digito == digito2 ){
			return true;
		}else{
			alert('Digito verificador incorreto.');
			campoNumeroProcessoSidoc.focus();
			return false;
		}
		
	}else if( tamanho != 0 ){
		alert('Número de Processo inválido.');
		campoNumeroProcessoSidoc.focus();
		return false;
	}
}  


function conferirDigitoVerificadorModulo11SemMensagem(campoNumeroProcessoSidoc) {

	var txtDigitado = campoNumeroProcessoSidoc.value;
	var tamanho		= txtDigitado.length;

	if( tamanho == 15 ){
		var ano = parseInt(txtDigitado.substr(11,2));
		if( 90 <= ano && ano <= 99 ){
			txtDigitado = '00'+txtDigitado;
			tamanho		= txtDigitado.length;
		}else{
			return false;
		}
	}
	
	if( tamanho == 12 ){
		txtDigitado = '00000'+txtDigitado;
		tamanho		= txtDigitado.length;
	}
	
	if( tamanho == 17 ){
		
		var digito = txtDigitado.substr(15,2);
		var modulo1 = 0;
		var i = 16;
		
		for( x = 0; x<15; x++ ){
			modulo1 = modulo1 + (txtDigitado.substr(x,1) * i);
			i--;
		}
		
		modulo1 = modulo1%11;
		
		if( modulo1 == 1 ){
			modulo1 = 0;
		}else if( modulo1 == 0){
			modulo1 = 1;
		}else{
			modulo1 = 11 - modulo1;
		}
		
		var modulo2 = 0;
		var i = 17;
		
		for( x = 0; x<15; x++ ){
			modulo2 = modulo2 + (txtDigitado.substr(x,1) * i);
			i--;
		}
		
		modulo2 = modulo2 + modulo1 * i;
		modulo2 = modulo2%11;
		
		if( modulo2 == 1 ){
			modulo2 = 0;
		}else if( modulo2 == 0){
			modulo2 = 1;
		}else{
			modulo2 = 11 - modulo2;
		}
		
		var digito2 = ''+parseInt(modulo1)+parseInt(modulo2);
		
		if( digito == digito2 ){
			return true;
		}else{
			return false;
		}
		
	}else if( tamanho != 0 ){
		return false;
	}
}  


function procederCampoInvalido(mensagem,campo) {
	campo.focus();
	alert(mensagem); 
}

