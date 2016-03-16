/* 
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */

///**
// * msg
// */
//function msg(txt, element, title, status)
//{
//    if (!title)
//        title = 'Aviso!';
//
//    $("#dialog_msg").remove();
//    $('body').append('<div id="dialog_msg"></div>');
//    $("#dialog_msg").html('<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>' + txt).dialog({
//        title: title,
//        show: {
//            effect: "fade",
//            duration: 500
//        },
//        hide: {
//            effect: "fade",
//            duration: 500
//        },
//        resizable: false,
//        height: 140,
//        modal: true,
//        buttons: {
//            'Ok': function() {
//                $(this).dialog('close');
//
//                if (element) {
//                    element.focus();
//                    element.scrollTop(300);
//                }
//            }
//        }
//    });
//
////    alert( msg );
////    $( 'html, body' ).animate( { scrollTop: element.offset.top - 300 }, 500 );
////    $("html, body").animate({ scrollTop: element "2000px" }, 'slow');
//
//}

function isValidDate(element) {
    
            var date= element.val();
            var ardt=new Array;
            var ExpReg=new RegExp("(0[1-9]|[12][0-9]|3[01])/(0[1-9]|1[012])/[12][0-9]{3}");
            ardt=date.split("/");
            erro=false;
            if ( date.search(ExpReg)==-1){
            erro = true;
            }
            else if (((ardt[1]==4)||(ardt[1]==6)||(ardt[1]==9)||(ardt[1]==11))&&(ardt[0]>30))
            erro = true;
            else if ( ardt[1]==2) {
            if ((ardt[0]>28)&&((ardt[2]%4)!=0))
            erro = true;
            if ((ardt[0]>29)&&((ardt[2]%4)==0))
            erro = true;
            }
            if (erro) {
                msg(element, date + ' não é uma data válida!');
//            alert( valor + "não é uma data válida!!!");
//            campo.focus();
//            campo.value = "";
            return false;
            }
            return true;
        }

function isValid()
{
    var isValide = true;

    $('[required]').each(
            function() {
                if ($(this).val() == '') {
                    var txt = "O campo '" + $(this).parents('td').prev().text() + "' não pode ser vazio!";
                    msg(txt, $(this));
                    isValide = false;
                    return false;
                }
            }
    );

    return isValide;
}

/**
 * msg
 */
function msg( element, txt, title, status , height , width)
{
    if (!title)
        title = 'Aviso!';
    
    if(!height)
        height = 140;
    if(!width)
        width = 300;

    $("#dialog_msg").remove();
    $('body').append('<div id="dialog_msg"></div>');
    $("#dialog_msg").html('<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>' + txt).dialog({
        title: title,
        position: { my: "top", at: "top", of: window },
        show: {
            effect: "fade",
            duration: 500
        },
        hide: {
            effect: "fade",
            duration: 500
        },
        resizable: false,
        height: height,
        width: width,
        modal: true,
        buttons: {
            'Ok': function() {
                $(this).dialog('close');

                if (element) {
                    element.focus();
                    element.scrollTop(300);
                }
            }
        }
    });
}

function modalConfirmAjax(msg, url, data , method) {
    $("#dialog").remove();
    $('body').append('<div id="dialog"></div>');
    $("#dialog").html('<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>' + msg).dialog({
        title: 'Aviso!',
//        position : { my: "top center" },
        show: {
            effect: "fade",
            duration: 500
        },
        hide: {
            effect: "fade",
            duration: 500
        },
        resizable: false,
        height: 140,
        modal: true,
//                dialogClass: "ui-state-highlight ui-corner-all",
        buttons: {
            'Sim': function() {
                $.post(url, data, function(result) {
                    var result = $.parseJSON(result);
                    if(method){
                        eval(method + '()');
                    }
                    msg(result.msg);
                });
                $(this).dialog('close');

            },
            'Não': function() {
                $(this).dialog('close');
            }
        }
    });
}

function modalConfirm(msg, method, data , method2) {
    $("#dialog").remove();
    $('body').append('<div id="dialog"></div>');
    $("#dialog").html('<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>' + msg).dialog({
        title: 'Aviso!',
        show: {
            effect: "fade",
            duration: 500
        },
        hide: {
            effect: "fade",
            duration: 500
        },
        resizable: false,
        height: 140,
        modal: true,
//                dialogClass: "ui-state-highlight ui-corner-all",
        buttons: {
            'Sim': function() {
                eval(method + '('+ data.result +')');
                $(this).dialog('close');
                return true;

            },
            'Não': function() {
                $(this).dialog('close');
                if(method2){
                    eval(method2 + '()');
                }
                return false;
            }
        }
    });
}

    /**
     * Envia dados do formulario utilizando ajax para realizar pesquisas
     *
     * @name pesquisarSubmitAjax
     * @param {string} [idForm] O valor informado deve ser um string e não um objeto
     * @param {string} [nameContainerResultado] O valor informado deve ser um string e não um objeto
     * @return {void}
     * 
     * @author Ruy Ferreira <ruy.ferreira@squadra.com.br>
     */
    function pesquisarSubmitAjax(idForm, nameContainerResultado)
    {   
        if(!idForm) idForm = 'form-search';
        if(!nameContainerResultado) nameContainerResultado = 'list';
        
        var dados = $("#" + idForm).serialize();
        var url = $("#" + idForm).attr('action');
        
        dadosFormulario = dados;
        
        $.post(url, dados, function( data ) {
            $('#'+ nameContainerResultado).empty().append(data).fadeIn('slow');
        });
    }
    
    /**
     * Chama o dialog do jQuery passando alguns parametros pre prontos e fazendo algumas funcionalidades para evitar repetimento de codigo.
     * 
     * @name modalAjax
     * @param {string} title - Titulo que e exibido no topo.
     * @param {array} data - Array de objetos para ser enviado via post no ajax.
     * @param {string} url - link para ser chamado no ajax (Não e obrigatorio).
     * 
     * @since 07/06/2013
     * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
     */
    function modalAjax( title, data , url, buttonClose)
    {
        if(!url){
            url = window.location.href;
        }
        
        if(!buttonClose) buttonClose = false;
        if(buttonClose){
            $.post(url, data, function( data ) {
                $( "#dialog" ).remove();
                $('body').append('<div id="dialog"></div>');
                $( "#dialog" ).html(data).dialog({
                    title: title,
                    position: { my: "top", at: "top", of: window },
                    show: {
                        effect: "fade",
                        duration: 500
                    },
                    hide: {
                        effect: "fade",
                        duration: 500
                    },
                    height: 320,
                    width: 800,
                buttons: {
                    'Ok': function() {
                        $(this).dialog('close');

                        if (element) {
                            element.focus();
                            element.scrollTop(300);
                        }
                    }
                }
                });
            });
        } else {
            $.post(url, data, function( data ) {
                $( "#dialog" ).remove();
                $('body').append('<div id="dialog"></div>');
                $( "#dialog" ).html(data).dialog({
                    title: title,
                    position: { my: "center", at: "center", of: window },
                    show: {
                        effect: "fade",
                        duration: 500
                    },
                    hide: {
                        effect: "fade",
                        duration: 500
                    },
                    height: 320,
                    width: 800
                });
            });
        }
    }

function windowReload()
{
    history.go(0);
}

/**
 * Envia dados do formulario utilizando ajax para salvar retornando os dados para exibir mensagem do resultado da operacao
 *
 * @name salvarSubmitAjax
 * @author Ruy Ferreira <ruy.ferreira@squadra.com.br>
 * @param {string} [idForm] O valor informado deve ser um string e não um objeto
 * @return {void}
 */
function saveSubmitAjax( idForm, modal )
{
    if ( !idForm ) idForm = 'form_save';
    var dataForm = $( '#' + idForm ).serialize();
    var url = $( '#' + idForm ).attr( 'action' );
    $.ajax({
        type: "POST",
        url: "",
        data: dataForm,
        dataType: 'json',
        success: function(html) {
            
            //                                    html = '<td colspan="2" style="text-align: center;">' + html + "</td>";
            if (html['status'] == true) {
                msg(null,html['msg']);
                returnSaveSucess();
            } else {
                var campo = $("#" + html['name']);
                msg(campo , html['msg']);
                campo.scrollTop(300);
                campo.focus();

                $('html, body').animate({scrollTop: campo.offset.top - 300}, 500);
            }

            //Pegar a linha para inserir o conteudo html.
            //                                    $('.container_formulario').empty().append(html).fadeIn();
        }
    });
//    if ( !idForm )
//        idForm = 'form_save';
//    var dados = $( '#' + idForm ).serialize();
//    var url = $( '#' + idForm ).attr( 'action' );
//    $.post( url, dados, function( data ) {
//
//        if ( modal ) {
//            $( 'html, body' ).animate( {
//                scrollTop: $( "#dialog" ).offset().top
//            }, 500 );
//            $( '.aviso-salvar' ).empty().append( data ).fadeIn();
//        } else {
//            $( 'html, body' ).animate( {
//                scrollTop: $( "body" ).offset().top
//            }, 500 );
//
//            $( '.aviso-salvar' ).empty().append( data ).fadeIn();
//        }
//    } );
}

/**
 * Retira o formulario da tela
 */
function cancelar()
{
//    html = '<td colspan="2" style="text-align: center"><br /><br /><input type="button" value="Inserir" onclick="javascript:formulario();"/><br /><br /></td>';
//    $( '.container_form_save' ).hide().empty().append(html).fadeIn( 'slow' );
    $( '.container_form_save' ).hide();
    
    //       $('.formulario').empty();
}

function number_format (number, decimals, dec_point, thousands_sep) {
  // http://kevin.vanzonneveld.net
  // +   original by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
  // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +     bugfix by: Michael White (http://getsprink.com)
  // +     bugfix by: Benjamin Lupton
  // +     bugfix by: Allan Jensen (http://www.winternet.no)
  // +    revised by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
  // +     bugfix by: Howard Yeend
  // +    revised by: Luke Smith (http://lucassmith.name)
  // +     bugfix by: Diogo Resende
  // +     bugfix by: Rival
  // +      input by: Kheang Hok Chin (http://www.distantia.ca/)
  // +   improved by: davook
  // +   improved by: Brett Zamir (http://brett-zamir.me)
  // +      input by: Jay Klehr
  // +   improved by: Brett Zamir (http://brett-zamir.me)
  // +      input by: Amir Habibi (http://www.residence-mixte.com/)
  // +     bugfix by: Brett Zamir (http://brett-zamir.me)
  // +   improved by: Theriault
  // +      input by: Amirouche
  // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // *     example 1: number_format(1234.56);
  // *     returns 1: '1,235'
  // *     example 2: number_format(1234.56, 2, ',', ' ');
  // *     returns 2: '1 234,56'
  // *     example 3: number_format(1234.5678, 2, '.', '');
  // *     returns 3: '1234.57'
  // *     example 4: number_format(67, 2, ',', '.');
  // *     returns 4: '67,00'
  // *     example 5: number_format(1000);
  // *     returns 5: '1,000'
  // *     example 6: number_format(67.311, 2);
  // *     returns 6: '67.31'
  // *     example 7: number_format(1000.55, 1);
  // *     returns 7: '1,000.6'
  // *     example 8: number_format(67000, 5, ',', '.');
  // *     returns 8: '67.000,00000'
  // *     example 9: number_format(0.9, 0);
  // *     returns 9: '1'
  // *    example 10: number_format('1.20', 2);
  // *    returns 10: '1.20'
  // *    example 11: number_format('1.20', 4);
  // *    returns 11: '1.2000'
  // *    example 12: number_format('1.2000', 3);
  // *    returns 12: '1.200'
  // *    example 13: number_format('1 000,50', 2, '.', ' ');
  // *    returns 13: '100 050.00'
  // Strip all characters but numerical ones.
  number = (number + '').replace(/[^0-9+\-Ee.]/g, '');
  var n = !isFinite(+number) ? 0 : +number,
    prec = !isFinite(+decimals) ? 0 : Math.abs(decimals),
    sep = (typeof thousands_sep === 'undefined') ? ',' : thousands_sep,
    dec = (typeof dec_point === 'undefined') ? '.' : dec_point,
    s = '',
    toFixedFix = function (n, prec) {
      var k = Math.pow(10, prec);
      return '' + Math.round(n * k) / k;
    };
  // Fix for IE parseFloat(0.55).toFixed(0) = 0;
  s = (prec ? toFixedFix(n, prec) : '' + Math.round(n)).split('.');
  if (s[0].length > 3) {
    s[0] = s[0].replace(/\B(?=(?:\d{3})+(?!\d))/g, sep);
  }
  if ((s[1] || '').length < prec) {
    s[1] = s[1] || '';
    s[1] += new Array(prec - s[1].length + 1).join('0');
  }
  return s.join(dec);
}

function str_replace (search, replace, subject, count) {
  // http://kevin.vanzonneveld.net
  // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   improved by: Gabriel Paderni
  // +   improved by: Philip Peterson
  // +   improved by: Simon Willison (http://simonwillison.net)
  // +    revised by: Jonas Raoni Soares Silva (http://www.jsfromhell.com)
  // +   bugfixed by: Anton Ongson
  // +      input by: Onno Marsman
  // +   improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +    tweaked by: Onno Marsman
  // +      input by: Brett Zamir (http://brett-zamir.me)
  // +   bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
  // +   input by: Oleg Eremeev
  // +   improved by: Brett Zamir (http://brett-zamir.me)
  // +   bugfixed by: Oleg Eremeev
  // %          note 1: The count parameter must be passed as a string in order
  // %          note 1:  to find a global variable in which the result will be given
  // *     example 1: str_replace(' ', '.', 'Kevin van Zonneveld');
  // *     returns 1: 'Kevin.van.Zonneveld'
  // *     example 2: str_replace(['{name}', 'l'], ['hello', 'm'], '{name}, lars');
  // *     returns 2: 'hemmo, mars'
  var i = 0,
    j = 0,
    temp = '',
    repl = '',
    sl = 0,
    fl = 0,
    f = [].concat(search),
    r = [].concat(replace),
    s = subject,
    ra = Object.prototype.toString.call(r) === '[object Array]',
    sa = Object.prototype.toString.call(s) === '[object Array]';
  s = [].concat(s);
  if (count) {
    this.window[count] = 0;
  }

  for (i = 0, sl = s.length; i < sl; i++) {
    if (s[i] === '') {
      continue;
    }
    for (j = 0, fl = f.length; j < fl; j++) {
      temp = s[i] + '';
      repl = ra ? (r[j] !== undefined ? r[j] : '') : r[0];
      s[i] = (temp).split(f[j]).join(repl);
      if (count && s[i] !== temp) {
        this.window[count] += (temp.length - s[i].length) / f[j].length;
      }
    }
  }
  return sa ? s : s[0];
}

/**
 * Funcao utilizada para somar os valores dos campos com a mesma classe.
 * 
 * @param string classeSoma - Nome da classe dos campos a serem somados
 */
function somarCampos(classeSoma) 
{
	var soma = 0;
	jQuery('.' + classeSoma).each(function(i, obj){
		var valor = $(obj).val() ? str_replace(['.', ','], ['', '.'], $(obj).val()) : 0;
		soma = parseFloat(soma) + parseFloat(valor);
	});
	return soma;
}

/**
 * Funcao utilizada para atualizar um campo com o resultado da soma de campos com a mesma classe.
 * 
 * @param string classeSoma - Nome da classe dos campos a serem somados
 * @param string idCampoTotal - Id do campo a ser atualizado o valor total
 * @param string tipoCampo - Para atualizar campos do tipo input, utilizar "campo", para os demais sera atualizado o html do elemento
 */
function atualizaTotal(classeSoma, idCampoTotal, tipoCampo) 
{
	var soma = somarCampos(classeSoma);
	if('campo' == tipoCampo){
		jQuery('#'+idCampoTotal).val(number_format(soma, 2, ',', '.'));
	} else {
		jQuery('#'+idCampoTotal).html(number_format(soma, 2, ',', '.'));
	}
}

//Variaveis Globais - Pega a posição do mouse.
var Tleft = 0;
var Ttopo = 0;

$( "html" ).mousemove( function( mouse ) {
    Tleft = mouse.pageX + 10;
    Ttopo = mouse.pageY + 20;
} );

function titleNaturezaInfoHidden() {
    $( ".div_info" ).css( { visibility: "hidden" } );
}

function titleNaturezaInfoVisibilyt( dpsid ) {
    $.ajax( {
        type: "POST",
        url: window.location,
        data: "requisicao=titleNaturezaDespesa&dpsid=" + dpsid,
        async: false,
        success: function( msg ) {
            $( ".div_info" ).css( { visibility: "visible" } );
            $( ".div_info" ).html( msg );
            $( '.div_info' ).css( "top", Ttopo );
            $( '.div_info' ).css( "left", Tleft );
        }
    } );
}