function replaceAll(string, token, newtoken) {
    while (string.indexOf(token) != -1) {
        string = string.replace(token, newtoken);
    }
    return string;
}

function verificaNavegadorIE(){
    return (navigator.appName == 'Microsoft Internet Explorer');
}

function executarScriptPai(funcao){
    (verificaNavegadorIE()) ? window.opener.execScript(funcao) : window.opener.eval(funcao);
}

function limitarTextoCampo(campo, limiteMax) {
  	var conteudo = campo.value;

  	if (conteudo.length > limiteMax){
  		var texto = conteudo.substring(0, limiteMax);
  		campo.value = texto;
  	}
}

String.prototype.trim = function () {
	return this.replace(/^\s+|\s+$/g,"");
}

//left trim
String.prototype.ltrim = function () {
	return this.replace(/^\s+/,"");
}

//right trim
String.prototype.rtrim = function () {
	return this.replace(/\s+$/,"");
}

function selecionaCampoPorID(idCampo){
    var campo = document.getElementById(idCampo);
    campo.checked = (campo.checked)? false : true;
}

/**
 * Valida se hora é valida
 * @name validaHora
 * @author Silas Matheus
 * @param object campo (this do elemento html)
 * @return void
 */
function validaHora(campo){

	var hora = campo.value.split(':');

	if(hora[0] > 23 || hora[1] > 59){
		alert('Hora inválida');
		campo.focus();
	}		
	
}

AbrirPopUp = function(url,nome,param){
    var a = window.open(url,nome,param);
    a.focus();
    return a;
}

/**
 * Limpa a div que exibe a foto
 * @name limparFoto
 * @author Silas Matheus
 * @return void
 */
function limparFoto(){
	
	if($('fotoVisitante'))
		$('fotoVisitante').innerHTML = "";
	
}