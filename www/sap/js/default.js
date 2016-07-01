AbrirPopUp = function(url,nome,param){
    var a = window.open(url,nome,param);
    a.focus();
}

function executarScriptPai(funcao){
	(verificaNavegadorIE())? window.opener.execScript(funcao) : window.opener.eval(funcao);
}

function verificaNavegadorIE(){
	 var nom = navigator.appName;
	 var browserIE = false;

	 if (nom == 'Microsoft Internet Explorer'){
		 browserIE = true;
	 }else if (nom == 'Netscape'){
		 browserIE = false;
	 }
	 
	 return browserIE;
}

/**
 * Função que formata o valor para monetário
 * @name formataValor
 * @param field - Campo que chamou a função
 * @return void
 */
function formataValor(field){
	var valorFormatado = MascaraMonetario(field.value);
	$(field.id).setValue(valorFormatado);
}

/**
 * Verifica se o valor digitado é numérico
 * @name verificaSomenteNumeros
 * @param field - Campo que chama a função
 * @param msg - Mensagem que vai ser alertada
 * @return bool
 */
function verificaSomenteNumeros(field,msg){
	if(isNaN(field.value)){
		alert(msg);
		$(field.id).clear();
		return false;
	}
}

/**
 * Verifica se o valor digitado é numérico, 
 * porém retirando caracteres permitidos na digitação de cnpj ou cpf
 * @name verificaSomenteNumerosCpfCnpj
 * @param field - Campo que chama a função
 * @param msg - Mensagem que vai ser alertada
 * @return bool
 */
function verificaSomenteNumerosCpfCnpj(field,msg){
	
	//retira os caracteres que são permitidos neste caso
	//pois o campo é cnpj ou cpf
	var valor = field.value;
	valor = valor.replace('.','');
	valor = valor.replace('.','');
	valor = valor.replace('/','');
	valor = valor.replace('-','');
	
	//se mesmo após retirar os caracteres
	//o valor ainda não for numérico
	if(isNaN(valor)){
		alert(msg);
		$(field.id).clear();
		return false;
	}
	
}

/**
 * Verifica se o valor digitado é numérico, 
 * porém retirando caracteres permitidos na digitação dos valores monetários
 * @name verificaSomenteNumerosMonetario
 * @param field - Campo que chama a função
 * @param msg - Mensagem que vai ser alertada
 * @return bool
 */
function verificaSomenteNumerosMonetario(field,msg){
	
	//retira os caracteres que são permitidos neste caso
	//pois o campo é monetário
	var valor = field.value;
	
	while (valor.indexOf('.') != -1) {
		valor = valor.replace('.', '');
	}
	
	while (valor.indexOf(',') != -1) {
		valor = valor.replace(',', '');
	}
	
	//se mesmo após retirar os caracteres
	//o valor ainda não for numérico
	if(isNaN(valor)){
		alert(msg);
		$(field.id).clear();
		return false;
	}
	
}

/**
 * Verifica o tamanho do campo. Criado pois nos campos monetários
 * se pressionar o teclado sem soltar, passa o maxlength
 * @name verificaTamanho
 * @param field - Campo que chama a função
 * @param msg - Mensagem que vai ser alertada
 * @return bool
 */
function verificaTamanho(field,$msg){
	if(field.value.length > $(field.id).getAttribute('maxlength')){
		alert($msg);
		$(field.id).clear();
		return false;
	}
}

/**
 * Habilita/Desabilita todos os botões da tela
 * @name enableButtons
 * @param bool enable - Habilitar ou desabilitar 
 */
function enableButtons(enable){
	
	if(typeof(enable) == 'undefined' || enable == true)		
		$$('input[type=button]').invoke('enable')
	else
		$$('input[type=button]').invoke('disable')
	
}


// Evento de carregamento das telas
Event.observe(window, 'load', function() {
	if(navigator.appName == 'Microsoft Internet Explorer' && !$('menu1'))
		$$('.tituloLista').invoke('addClassName', 'tituloListaPopup');
	
	//retira o atributo onclick e title da coluna comando do monta lista
	//para que não seja possível ordenar por esta coluna
	var celulaComando = $$('table.listagem td.title').first();
	if(celulaComando){
		celulaComando.writeAttribute({onclick:'void(0)', title:null});
	}
});

/**
 * Envia o documento para impressão
 * @name imprimir
 * @return void
 */
function imprimir(){
	window.print();
}