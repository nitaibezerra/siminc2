/* 
// +----------------------------------------------------------------------+
// | Copyright (c) 2006 Adonias Malosso && Cristiano Cabral               |
// +----------------------------------------------------------------------+
// | Licensed under the Apache License, Version 2.0 (the "License");      |
// | you may not use this file except in compliance with the License.     |
// | You may obtain a copy of the License at                              |
// | http://www.apache.org/licenses/LICENSE-2.0                           |
// | Unless required by applicable law or agreed to in writing, software  |
// | distributed under the License is distributed on an "AS IS" BASIS,    |
// | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      |
// | implied. See the License for the specific language governing         |
// | permissions and limitations under the License.                       |
// +----------------------------------------------------------------------+
// | Author: Adonias Malosso <malosso@gmail.com>                          |
// | Author: Cristiano Cabral <cristiano.cabral@gmail.com>                |
// +----------------------------------------------------------------------+
*/
var evDelay = 2500;
var evTimeout = "";
var evXmlHttp = "";

evTimeout = setTimeout( estouVivo, evDelay );
var evOnTimeout = function ()
{
	try {
		if(typeof evXmlHttp != "undefined") 
			evXmlHttp.send(null);
	}
	catch(e) {}
};

var evOnLoad = function () 
{
	try {
		if (evXmlHttp.readyState == 4)
		{
			if ( (evXmlHttp.status==200) && ( evXmlHttp.responseText != "") )
			{
				evProcessaResultado(evXmlHttp);
			}

			if (evXmlHttp.dispose)
				evXmlHttp.dispose();
			evXmlHttp = null;
		}
	}
	catch(e) {}
};	

/**
 * Chama o script estouvivo.php
 * 
 * @param void
 * @return void
 */
function estouVivo()
{
	try {
		evXmlHttp = evCriaHttpRequest();
		evXmlHttp.open("GET", "/estouvivo.php", true);	
		//evXmlHttp.onreadystatechange = new Function("evOnLoad()");
		evXmlHttp.onreadystatechange = evOnLoad;
		window.setTimeout("evOnTimeout()", 300);
	}
	catch (e) {}
};


function evProcessaResultado( evXmlHttp )
{
	try 
	{
		if(evXmlHttp.responseText=='EXIT') document.location.href='../../login.php?expirou=s';
		try { var xmlDoc = evXmlHttp.responseXML.documentElement; } catch(e) {}
		if(typeof xmlDoc != "undefined") {
			var rdpUsuariosOnLine = document.getElementById("rdpUsuariosOnLine");
			var numUsuariosOnLine = xmlDoc.getAttribute("usuariosOnLine");
			rdpUsuariosOnLine.innerHTML = numUsuariosOnLine;
			var msgs = xmlDoc.getElementsByTagName("arrayOfMensagens");
			
			if ( ( typeof msgs != "undefined" ) && ( msgs.length > 0 ) )
			{
				msgs = msgs[0].getElementsByTagName("mensagem");
				chatProcessarMsgs( msgs );
			}
		}
	}
	catch(e) {
	}
	evTimeout = setTimeout( estouVivo, evDelay );
}

/**
 * Criar um objeto para requisição ajax
 * 
 * @param void
 * @return XMLHttpRequest
 */
function evCriaHttpRequest()
{	
	var http_request = false;
    if ( window.XMLHttpRequest ) { // Mozilla, Safari,...
        http_request = new XMLHttpRequest();
        if ( http_request.overrideMimeType ) {
            //http_request.overrideMimeType('text/xml');
            // See note below about this line
        }
    } else if (window.ActiveXObject) { // IE
        try {
            http_request = new ActiveXObject( "Msxml2.XMLHTTP" );
        } catch (e) {
            try {
                http_request = new ActiveXObject( "Microsoft.XMLHTTP" );
            } catch (e) {}
        }
    }
    if (!http_request) {
        return false;
    }
    return http_request;
}


/* 
// +----------------------------------------------------------------------+
// | Copyright (c) 2006 Adonias Malosso                                   |
// +----------------------------------------------------------------------+
// | Licensed under the Apache License, Version 2.0 (the "License");      |
// | you may not use this file except in compliance with the License.     |
// | You may obtain a copy of the License at                              |
// | http://www.apache.org/licenses/LICENSE-2.0                           |
// | Unless required by applicable law or agreed to in writing, software  |
// | distributed under the License is distributed on an "AS IS" BASIS,    |
// | WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or      |
// | implied. See the License for the specific language governing         |
// | permissions and limitations under the License.                       |
// +----------------------------------------------------------------------+
// | Author: Adonias Malosso <malosso@gmail.com>                          |
// +----------------------------------------------------------------------+
*/

var chWdws = new Array();
var chWdwPrefix = "chat_";
var chBaseUrl = "http://simec-d/geral/chat.php?cpf=";
var chWdwProperties = 'width=250,height=400,scrollbars=auto';
var chHttp = evCriaHttpRequest();
var chWdwFocus = "";

function chatProcessarMsgs(msgs) {
	var ids = new Array(msgs.length);
	var apagar = true;
	avisoChatRemoverTodos();
	if ( msgs.length == 0 ) {
		avisoChatEsconder();
	} else for( var i = 0; i < msgs.length; i++ ) {
		var remetente = msgs[i].getAttribute("remetente");
		var data = msgs[i].getAttribute("data");
		var hora = msgs[i].getAttribute("hora");
		var nome = msgs[i].getAttribute("nome");
		if ( typeof chWdws[remetente] == "undefined" || typeof chWdws[remetente].carregou == "unknown" )
		{
			chWdws[remetente] = new Object();
			chWdws[remetente].carregou = false;
		}
		try {
			if ( chWdws[remetente].carregou ) {
				chWdws[remetente].escreverMsg( msgs[i].getAttribute("id"), hora, msgs[i].firstChild.data, true );
				//ids[i] = msgs[i].getAttribute("id");
				//apagarMensagens( ids[i] );
				continue;
			}
			avisoChatAdicionar( nome, remetente );
		} catch(e) {/*if (msgs[i].getAttribute("id")) {apagarMensagens(msgs[i].getAttribute("id"));}*/}
	}
}

function enviarMensagem( usucpfdestino, msg ) {
	try {
		chHttp.open('POST',"/estouvivo.php",false);
		chHttp.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
		chHttp.send("op=enviar&usucpfdestino="+usucpfdestino+"&msg="+escape(msg));
		return evTrim(chHttp.responseText);
	}
	catch(e) {}
}

function apagarMensagens( ids ) {
	try {
		chHttp.open('POST',"/estouvivo.php",false);
		chHttp.setRequestHeader('Content-Type','application/x-www-form-urlencoded');
		chHttp.send("op=apagar&msglist="+ids);
		//return chHttp.responseText;
	}
	catch(e) {}
}

function abrirChat( cpforigem, nome ) {
	if( !chPopupAberto( cpforigem ) ) {
		chWdws[cpforigem] = window.open(
			'/geral/chat.php?cpf=' + cpforigem + '&nome=' + escape( nome ),
			chWdwPrefix+cpforigem,
			chWdwProperties
		);
		return false;
	}
	return true;
}

function chPopupAberto( cpforigem ) {
	chWdws[cpforigem] = window.open( '', chWdwPrefix+cpforigem, chWdwProperties );
	return chWdws[cpforigem].location.href == chBaseUrl+cpforigem;
}

function evTrim( text ) {
	if((text==null) || (typeof text == "undefined")) return null;
//   return text.replace(/^[ \n\t]+|[ \n\t]+$/g, "");
	return text.replace(/^\s*|\s*$/g, "");
}













// CONTROLE DA DIV

/**
 * Mostra a lista de usuários do chat caso esteja escondido, e
 * esconde caso contrário.
 * 
 * @return void
 */
function avisoChatMostrarEsconder()
{
	lista = document.getElementById( 'avisochat_lista' );
	lista.style.display = lista.style.display == 'none' ? 'block' : 'none' ;
}

/**
 * Mostra a lista de usuários do chat.
 * 
 * @return void
 */
function avisoChatMostrar()
{
	var lista = document.getElementById( 'avisochat_lista' );
	lista.style.display = 'block';
}

/**
 * Mostra a lista de usuários do chat.
 * 
 * @return void
 */
function avisoChatEsconder()
{
	var lista = document.getElementById( 'avisochat_lista' );
	lista.style.display = 'none';
}

var avisoChatId = new Array();

/**
 * Insere um registro na lista de usuário do chat.
 * 
 * @param string nome do usuário
 * @param string cpf do usuário
 * @return void
 */
function avisoChatAdicionar( nome, cpf )
{
	var id = 'avisoChat_id_' + cpf;
	var lista = document.getElementById( 'avisochat_lista' );
	if ( avisoChatId[id] )
	{
		avisoChatMostrar();
		return;
	}
	var tabela = document.getElementById( 'avisochat_tabela' );
	var linha = tabela.insertRow( tabela.rows.length );
		linha.id = id;
	var celula = linha.insertCell( 0 );
	var link = document.createElement( 'a' );
		link.href = '#';
	var onclick = new Function ( ' abrirChat( \'' + cpf + '\', \'' + nome + '\' ); avisoChatRemover( \'' + cpf + '\' ); ' );
	if (navigator.userAgent.indexOf("Safari") > 0) {
		link.addEventListener( "click", onclick, false );
	} else if ( navigator.product == "Gecko") {
		link.addEventListener( "click", onclick, false );
	} else {
		link.onkeyup = new Function( '' );
		link.attachEvent( 'onclick', onclick );
	}
	link.innerHTML = '&nbsp;'+nome;
	var img = document.createElement( 'img' );
		img.src = '/imagens/online.gif';
	celula.appendChild( img );
	celula.appendChild( link );
	avisoChatMostrar();
	avisoChatId[id] = id;
	return;
}

/**
 * Remove um registro da lista de chat. Caso o
 * usuário seja o único a tela é escondida por
 * completo.
 * 
 * @param string identificador da linha
 * @return void
 */
function avisoChatRemover( cpf )
{
	var id = 'avisoChat_id_' + cpf;
	var tr = document.getElementById( id );
	if ( tr )
	{
		tr.parentNode.removeChild( tr );
	}
	if ( avisoChatId[id] )
	{
		avisoChatId[id] = false;
	}
	var quantidade = 0;
	for ( var id in avisoChatId )
	{
		if ( id != false )
		{
			quantidade++;
		}
	}
	if ( quantidade > 0 )
	{
		avisoChatEsconder();
	}
}

/**
 * Remove todos os registros da lista de chat.
 * 
  * @return void
 */
function avisoChatRemoverTodos()
{
	var tr = null;
	for ( var id in avisoChatId )
	{
		tr = document.getElementById( id );
		if ( tr )
		{
			tr.parentNode.removeChild( tr );
			avisoChatId[id] = false;
		}
	}
}

// FIM CONTROLE DA DIV
