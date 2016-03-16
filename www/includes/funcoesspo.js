/**
 * Funções javascript do sistemas SPO.
 * $Id: funcoesspo.js 97987 2015-05-29 13:26:50Z lindalbertofilho $
 */

/**
 * Verifica se os campos com os ids inclusos em itemsParaValidacao estão preenchidos.
 * Para funcionar, este código deve ser executado dentro da estrutura do bootstrap, e
 * os padrões de criação de formulários (com formgroups) devem ser obdecidos.
 * Inclua o seguinte css para uma melhor apresentação do texto da modal:
 * <style>
 *      #modal-alert .modal-body ul{text-align:left;margin-top:5px;list-style:circle}
 * </style>
 *
 * @param {array} itemsParaValidacao Os IDs dos itens que deverão ser validados.
 * @param {string} formID O ID do formulário que será submetido.
 * @param {string} requisicao O nome da requisição que está sendo executada.
 * @returns {undefined}
 */
function validarFormulario(itemsParaValidacao, formID, requisicao)
{
	
    // -- Validando os itens do formulario obrigatorios para criar um novo pedido
    var msg = new Array();
    for (x=0; x < itemsParaValidacao.length; x++) {    	
        // -- Selecionando o input
        var $item = $('#' + itemsParaValidacao[x]);
        if ('' == $item.val()) { // -- validando o conteúdo do input e selecionando o label para montar msg de erro
        	if($item.parent().prev().children('label').text().replace(':', '') == ""){         		
        		msg.push($item.parents('div.col-md-10').prev().children('label').text().replace(':',''));
        		$item.parents('div.col-md-10').prev().addClass('has-error');        		
        	}else{        		
        		msg.push($item.parent().prev().children('label').text().replace(':', ''));
        		$item.parent().parent().addClass('has-error');
        	}        	
        }         
    }    
    // -- Se existir alguma mensagem, exibe para o usuário
    if (msg.length > 0) {
        var htmlMsg = '<div class="bs-callout bs-callout-danger">Antes de prosseguir, os seguintes campos devem ser preenchidos:<ul>';
        for (var x in msg) {
            if ('string' !== typeof (msg[x])) {
                continue;
            }

            htmlMsg += '<li>' + msg[x];
            if (x == msg.length - 1) {
                htmlMsg += '.';
            } else {
                htmlMsg += ';';
            }
            htmlMsg += '</li>';
        }
        htmlMsg += '</ul></div>';
        $('#modal-alert .modal-body').html(htmlMsg);
        $('#modal-alert').modal();
        return;
    }

    $('#requisicao').val(requisicao);
    $('#' + formID).submit();
}

/**
 * Inicia os comandos da tela de inicio: btnOn, btnNovaJanela, #btnCadastrar e #btnListar.
 * A URL é tratada dinamicamente, de forma a funcionar para todos os módulos.
 */
function inicio()
{
    // -- Iniciando os botões com a classe .btnOn
    $('.btnOn').click(function() {
        var uri = $(this).attr('data-request');
        var dataTarget = $(this).attr('data-target');
        if (!uri && !dataTarget) {
            bootbox.alert('Botão sem url (data-request ou data-target) definida.');
            return;
        }
        if(uri){
            location.href = uri;
        }
    });

    // -- Iniciando os botões com a classe .btnNovaJanela
    $('.btnNovaJanela').click(function() {
        var uri = $(this).attr('data-request');
        if (!uri) {
            alert('Botão sem url (data-request) definida.');
            return;
        }
        window.open(uri);
    });

    // -- Iniciando o botão de cadastrar comunicados
    $('#btnCadastrar').click(function() {
        var uri = window.location.href;
        uri = uri.replace(/\?.+/g, '?modulo=principal/comunicado/cadastrar&acao=A');
        window.location.href = uri;
    });

    // -- Iniciando o botão de listar comunicados
    $('#btnListar').click(function() {
        var uri = window.location.href;
        uri = uri.replace(/\?.+/g, '?modulo=principal/comunicado/listar&acao=A');
        window.location.href = uri;
    });
}

/**
 * Abre um arquivo cadastrado no sistema de comunicados.
 * @param {int} arqid O ID do arquivo no sistema.
 */
function abrirArquivo(arqid) {
    var uri = window.location.href;
    uri = uri.replace(/\?.+/g, '?modulo=principal/comunicado/visualizar&acao=A&download=S&arqid=' + arqid);
    window.location.href = uri;
}

function calculaMedia(ID_dividendo, ID_divisor, ID_destino, ehMoeda)
{
    var dividendo = $(ID_dividendo).val().replace(/\./g, '').replace(/,/g, '.');
    var divisor = $(ID_divisor).val().replace(/\./g, '').replace(/,/g, '.');
    var quociente = new Number(dividendo / divisor);

    if (ehMoeda) {
        // -- @todo corrigir este arredondamento (fazer um arredondamento inteligente)
        quociente = mascaraglobal('###.###.###.###,##', quociente.toFixed(2));
    } else {
        quociente = mascaraglobal('###.###.###.###', quociente);
    }
    $(ID_destino).val(quociente);
}


/*
objetivo: mascarar de acordo com a mascara passada
caracteres: # - caracter a ser mascarado
           | - separador de mascaras
modos (exemplos):
mascara simples: "###-####"	                 mascara utilizando a mascara passada
mascara composta: "###-####|####-####"       mascara de acordo com o tamanho (length) do valor passado
mascara dinâmica: "[###.]###,##"             multiplica o valor entre colchetes de acordo com o length do valor para que a mascara seja dinâmica ex: ###.###.###.###,##
utilizar no onkeyup do objeto
ex: onkeyup="this.value = mascara_global('#####-###',this.value);"
tratar o maxlength do objeto na página (a função não trata isso)

Obs.: Movido de funcoes.js 
*/

function mascaraglobal(mascara, valor){

        var mascara_utilizar;
        var mascara_limpa;
        var temp;
        var i;
        var j;
        var caracter;
        var separador;
        var dif;
        var validar;
        var mult;
        var ret;
        var tam;
        var tvalor;
        var valorm;
        var masct;
        tvalor = "";
        ret = "";
        caracter = "#";
        separador = "|";
        mascara_utilizar = "";
        valor = trim(valor);
        if (valor == "")return valor;
        temp = mascara.split(separador);
        dif = 1000;

        valorm = valor;
        //tirando mascara do valor já existente
        for (i=0;i<valor.length;i++){
                if (!isNaN(valor.substr(i,1))){
                        tvalor = tvalor + valor.substr(i,1);
                }
        }
        valor = tvalor;

        //formatar mascara dinamica
        for (i = 0; i<temp.length;i++){
                mult = "";
                validar = 0;
                for (j=0;j<temp[i].length;j++){
                        if (temp[i].substr(j,1) == "]"){
                                temp[i] = temp[i].substr(j+1);
                                break;
                        }
                        if (validar == 1)mult = mult + temp[i].substr(j,1);
                        if (temp[i].substr(j,1) == "[")validar = 1;
                }
                for (j=0;j<valor.length;j++){
                        temp[i] = mult + temp[i];
                }
        }


        //verificar qual mascara utilizar
        if (temp.length == 1){
                mascara_utilizar = temp[0];
                mascara_limpa = "";
                for (j=0;j<mascara_utilizar.length;j++){
                        if (mascara_utilizar.substr(j,1) == caracter){
                                mascara_limpa = mascara_limpa + caracter;
                        }
                }
                tam = mascara_limpa.length;
        }else{
                //limpar caracteres diferente do caracter da máscara
                for (i=0;i<temp.length;i++){
                        mascara_limpa = "";
                        for (j=0;j<temp[i].length;j++){
                                if (temp[i].substr(j,1) == caracter){
                                        mascara_limpa = mascara_limpa + caracter;
                                }
                        }

                        if (valor.length > mascara_limpa.length){
                                if (dif > (valor.length - mascara_limpa.length)){
                                        dif = valor.length - mascara_limpa.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else if (valor.length < mascara_limpa.length){
                                if (dif > (mascara_limpa.length - valor.length)){
                                        dif = mascara_limpa.length - valor.length;
                                        mascara_utilizar = temp[i];
                                        tam = mascara_limpa.length;
                                }
                        }else{
                                mascara_utilizar = temp[i];
                                tam = mascara_limpa.length;
                                break;
                        }
                }
        }

        //validar tamanho da mascara de acordo com o tamanho do valor
        if (valor.length > tam){
                valor = valor.substr(0,tam);
        }else if (valor.length < tam){
                masct = "";
                j = valor.length;
                for (i = mascara_utilizar.length-1;i>=0;i--){
                        if (j == 0) break;
                        if (mascara_utilizar.substr(i,1) == caracter){
                                j--;
                        }
                        masct = mascara_utilizar.substr(i,1) + masct;
                }
                mascara_utilizar = masct;
        }

        //mascarar
        j = mascara_utilizar.length -1;
        for (i = valor.length - 1;i>=0;i--){
                if (mascara_utilizar.substr(j,1) != caracter){
                        ret = mascara_utilizar.substr(j,1) + ret;
                        j--;
                }
                ret = valor.substr(i,1) + ret;
                j--;
        }
        return ret;
}

/////////////////////////////
//Eventos
//Funções para controlar eventos de campos de formulário
////////////////////////////
function MouseOver(objeto)
{
	if (objeto.type == "text" || objeto.type == "password"){
		if(objeto.className != 'clsMouseFocus'){
				objeto.className = objeto.className.replace("normal", "clsMouseOver");
		}

	}else if(objeto.type == "textarea"){
		if(objeto.className != 'txareaclsMouseFocus'){
				objeto.className = objeto.className.replace("txareanormal", "txareaclsMouseOver");
		}
	}
	return true;
}

function MouseOut(objeto)
{
	if (objeto.type == "text" || objeto.type == "password")
	{
		if(objeto.className != 'clsMouseFocus'){
					objeto.className = objeto.className.replace("clsMouseOver", "normal");
			}
	
	}else if(objeto.type == "textarea"){
		if(objeto.className != 'txareaclsMouseFocus'){
				objeto.className = objeto.className.replace("txareaclsMouseOver", "txareanormal");
		}
	}
	return true;
}


function MouseClick(objeto){
	if (objeto.type == "text" || objeto.type == "password"){
		objeto.className = objeto.className.replace("clsMouseOver", "clsMouseFocus");
		objeto.className = objeto.className.replace("normal", "clsMouseFocus");
	}else if(objeto.type == "textarea"){
		objeto.className = objeto.className.replace("txareanormal", "txareaclsMouseFocus");
		objeto.className = objeto.className.replace("txareaclsMouseOver", "txareaclsMouseFocus");
	}
}


function MouseBlur( objeto )
{
	if ( objeto.type == "text" || objeto.type == "textarea" || objeto.type == "password" )
	{
		if ( objeto.type == "textarea")
		{
			objeto.className = objeto.className.replace("txareaclsMouseOver", "txareanormal");
			objeto.className = objeto.className.replace("txareaclsMouseFocus", "txareanormal");
		}
		else
		{
			objeto.className = objeto.className.replace("clsMouseOver", "normal");
			objeto.className = objeto.className.replace("clsMouseFocus", "normal");
		}
	}
}

//Função para limitar o tamanho do Textarea 
function textCounter(field, countfield, maxlimit) {
	if ( !field || !field.value )
	{
		countfield.value = maxlimit;
		return;
	}
	if (field.value.length > maxlimit)
	field.value = field.value.substring(0, maxlimit);
	else 
	countfield.value = maxlimit - field.value.length;
}

function trim(valor){
	valor+='';
        for (i=0;i<valor.length;i++){
                if(valor.substr(i,1) != " "){
                        valor = valor.substr(i);
                        break;
                }
                if (i == valor.length-1){
                        valor = "";
                }
        }
        for (i=valor.length-1;i>=0;i--){
                if(valor.substr(i,1) != " "){
                        valor = valor.substr(0,i+1);
                        break;
                }
        }
        return valor;
}

function formatXml(xml) {
    var formatted = '';
    var reg = /(>)(<)(\/*)/g;
    xml = xml.replace(reg, '$1\r\n$2$3');
    var pad = 0;
    jQuery.each(xml.split('\r\n'), function(index, node) {
        var indent = 0;
        if (node.match( /.+<\/\w[^>]*>$/ )) {
            indent = 0;
        } else if (node.match( /^<\/\w/ )) {
            if (pad != 0) {
                pad -= 1;
            }
        } else if (node.match( /^<\w[^>]*[^\/]>.*$/ )) {
            indent = 1;
        } else {
            indent = 0;
        }

        var padding = '';
        for (var i = 0; i < pad; i++) {
            padding += '  ';
        }

        formatted += padding + node + '\r\n';
        pad += indent;
    });

    return formatted;
}