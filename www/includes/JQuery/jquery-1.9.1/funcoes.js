/*

 Colocar estas quatro linhas para funcionar direitinho as funcoes deste arquivo.

 <script src="/includes/JQuery/jquery-1.9.1/jquery-1.9.1.js" type="text/javascript"></script>
 <script src="/includes/JQuery/jquery-1.9.1/jquery-ui-1.10.3.custom.min.js" type="text/javascript"></script>
 <script src="/includes/JQuery/jquery-1.9.1/funcoes.js" type="text/javascript"></script>
 <link href="/includes/JQuery/jquery-1.9.1/css/jquery-ui-1.10.3.custom.min.css" rel="stylesheet">
 */

(function($) {

    /**
     * @see $.fn.msg
     * @param {string} txt
     * @param {object} options
     */
    $.msg = function(txt, options) {
        $().msg(txt, options);
    }

    /**
     * @see $.fn.isValid
     * @param {string} txt
     * @param {object} options
     */
    $.isValid = function() {
        $().isValid();
    }

    /**
     * Exibe a mensagem em um dialog caso seja de um input da focus navega na barra de rolagem at� o input passado
     *
     * @name msg
     * @param {string} txt            - Texto da mensagem
     * @param {object} options.title  - Titulo da janela
     * @param {object} options.heigth - Tamanho do comprimento da janela
     * @param {object} options.width  - Tamanho da largura da janela
     *
     * @author Ruy Junior Ferreira Silva - <ruy.silva@mec.gov.br>
     * @since 26/08/2013
     */
    $.fn.msg = function(txt, options) {
        var defaults = {title: 'Aviso!', heigth: 140, width: 300, close: false};
        var settings = $.extend({}, defaults, options);

        if (this) {
            var element = this;
        }

        $("#dialog_msg").remove();
        $('body').append('<div id="dialog_msg"></div>');
        $("#dialog_msg").html('<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>' + txt).dialog({
            title: settings.title,
            show: {
                effect: "fade",
                duration: 500
            },
            hide: {
                effect: "fade",
                duration: 500
            },
            position: {my: "top", at: "top", of: window},
            resizable: false,
            height: settings.heigth,
            width: settings.width,
            modal: true,
            buttons: {
                'Ok': function() {
                    $(this).dialog('close');

                    if (element) {
                        element.focus();
                        element.scrollTop(300);
                    }
                }
            },
            close: function() {
                if (settings.close) {
                    window.location.reload();
                }
            }
        });
    };

    $.fn.isValid = function(callback)
    {

        if (this)
            form = this;
        else
            form = 'form_save';

        var isValid = true;

        $('[required]').each(
            function() {
                if ($(this).val() == '') {
                    var txt = "O campo '" + $(this).parents('td').prev().text() + "' n�o pode ser vazio!";
                    $(this).msg(txt);
                    isValid = false;
                    return false;
                }
            }
        );

        callback = function (){
            return isValid;
        };


//        function callback(){
//            callback = isValid;
//        };
    }

})(jQuery);


/**
 * msg
 */
function msg(txt, element, title, status, heigth, width)
{
    if (!title)
        title = 'Aviso!';

    if (!heigth)
        heigth = 140;

    if (!width)
        width = 300;


    $("#dialog_msg").remove();
    $('body').append('<div id="dialog_msg"></div>');
    $("#dialog_msg").html('<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>' + txt).dialog({
        title: title,
        show: {
            effect: "fade",
            duration: 500
        },
        hide: {
            effect: "fade",
            duration: 500
        },
        position: {my: "top", at: "top", of: window},
        resizable: false,
        height: heigth,
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

//    alert( msg );
//    $( 'html, body' ).animate( { scrollTop: element.offset.top - 300 }, 500 );
//    $("html, body").animate({ scrollTop: element "2000px" }, 'slow');

}

function dialogConfirm(msg, method, data, method2, divId) {

    if (!divId) {
        divId = 'dialog';
    }


    $("#" + divId).remove();
    $('body').append('<div id="' + divId + '"></div>');
    $("#" + divId).html('<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>' + msg).dialog({
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
                eval(method + '(' + data.result + ')');
                $(this).dialog('close');
                return true;

            },
            'N�o': function() {
                $(this).dialog('close');
                if (method2) {
                    eval(method2 + '()');
                }
                return false;
            }
        }
    });
}

function dialogConfirmAjax(msg, url, data, method, divId) {

    if (!divId) {
        divId = 'dialog';
    }

    $("#" + divId).remove();
    $('body').append('<div id="' + divId + '"></div>');
    $("#" + divId).html('<span class="ui-icon ui-icon-info" style="float: left; margin-right: .3em;"></span>' + msg).dialog({
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
                $.post(url, data, function(result) {
                    console.info(result);
                    var result = $.parseJSON(result);
                    if (method) {
                        eval(method + '()');
                    }
                    msg(result.msg);
                });
                $(this).dialog('close');

            },
            'N�o': function() {
                $(this).dialog('close');
            }
        }
    });
}

/**
 * Envia dados do formulario utilizando ajax para realizar pesquisas
 *
 * @name pesquisarSubmitAjax
 * @param {string} [idForm] O valor informado deve ser um string e n�o um objeto
 * @param {string} [nameContainerResultado] O valor informado deve ser um string e n�o um objeto
 * @return {void}
 *
 * @author Ruy Ferreira <ruy.ferreira@squadra.com.br>
 */
function pesquisarSubmitAjax(idForm, nameContainerResultado)
{
    if (!idForm)
        idForm = 'form-search';
    if (!nameContainerResultado)
        nameContainerResultado = 'list';

    var dados = $("#" + idForm).serialize();
    var url = $("#" + idForm).attr('action');

    dadosFormulario = dados;

    $.post(url, dados, function(data) {
        $('#' + nameContainerResultado).empty().append(data).fadeIn('slow');
    });
}

/**
 * Chama o dialog do jQuery passando alguns parametros pre prontos e fazendo algumas funcionalidades para evitar repetimento de codigo.
 *
 * @name dialogAjax
 * @param {string} title - Titulo que e exibido no topo.
 * @param {array} data - Array de objetos para ser enviado via post no ajax.
 * @param {string} url - link para ser chamado no ajax (N�o e obrigatorio).
 * @param {string} divId - Se tiver uma div para ser o dialo se nao ele cria um proprio chamado 'dialog'(N�o e obrigatorio).
 * @param {bolean} close - Se for true ele recarrega a pagina ao fechar o dialog (N�o e obrigatorio).
 * @param {bolean} modal - Se for true ele bloqueia a pagina ao abrir o dialog (N�o e obrigatorio).
 * @param {bolean} resizable - Se for true ele permite redimensionar o dialog (N�o e obrigatorio).
 * @param {bolean} closeOnEscape - Se for true ele permite fechar o dialog ao pressionar a tecla ESC (N�o e obrigatorio).
 * @param {object} position - Permite especificar o posicionamento do dialog na tela (N�o e obrigatorio).
 *
 * @since 07/06/2013
 * @author Ruy Junior Ferreira Silva <ruy.silva@mec.gov.br>
 */
function dialogAjax(title, data, url, divId, close, modal, resizable, closeOnEscape, position, width, height)
{
    // Valores iniciais
    if (!url) url = window.location.href;
    if (!divId) divId = 'dialog';
    if (!close) close = false;
    if (!modal) modal = false;
    if (!resizable) resizable = false;
    if (!closeOnEscape) closeOnEscape = false;
    if (!position) position = ['center', 130];
    
    if(!width) {
    	width = 800;
    }
    
    if(!height) {
    	height = 300;
    }

    $1_11.post(url, data, function(data) {
        $1_11("#" + divId).remove();
        $1_11('body').append('<div id="' + divId + '"></div>');
        $1_11("#" + divId).html(data).dialog({
            close: function() {
                if (close) {
                    window.location.reload();
                }
            },
            title: title,
            show: {
                effect: "fade",
                duration: 500
            },
            hide: {
                effect: "fade",
                duration: 500
            },
            height: height,
            width: width,
            modal: modal,
            resizable: resizable,
            closeOnEscape: closeOnEscape,
            position: position
        });
    });
}

function isValid(formName)
{
    if (!formName)
        formName = 'form_save';

    var isValide = true;

    $('[required]').each(
            function() {
                if ($(this).val() == '') {
                    var txt = "O campo '" + $(this).parents('td').prev().text() + "' n�o pode ser vazio!";
                    msg(txt, $(this));
                    isValide = false;
                    return false;
                }
            }
    );

    return isValide;
}

/**
 * Envia dados do formulario utilizando ajax para salvar retornando os dados para exibir mensagem do resultado da operacao
 *
 * @name salvarSubmitAjax
 * @author Ruy Ferreira <ruy.ferreira@squadra.com.br>
 * @param {string} [idForm] O valor informado deve ser um string e n�o um objeto
 * @return {void}
 */
function saveSubmitAjax(idForm)
{
    if (!idForm)
        idForm = 'form_save';
    var dataForm = $('#' + idForm).serialize();
    var url = $('#' + idForm).attr('action');
    $.ajax({
        type: "POST",
        url: "",
        data: dataForm,
        dataType: 'json',
        success: function(html) {
            if (html['status'] == true) {
                msg(html['msg']);
                returnSaveSucess(html);
            } else {
                var campo = $("#" + html['name']);
                msg(html['msg'], campo);
                $('html, body').animate({scrollTop: campo.offset.top - 300}, 500);
            }
        }
    });
}

/**
 * Retira o formulario da tela
 */
function cancelar()
{
//    html = '<td colspan="2" style="text-align: center"><br /><br /><input type="button" value="Inserir" onclick="javascript:formulario();"/><br /><br /></td>';
//    $( '.container_form_save' ).hide().empty().append(html).fadeIn( 'slow' );
    $('.container_form_save').hide();

    //       $('.formulario').empty();
}

function number_format(number, decimals, dec_point, thousands_sep) {
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
            toFixedFix = function(n, prec) {
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

function str_replace(search, replace, subject, count) {
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
    jQuery('.' + classeSoma).each(function(i, obj) {
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
    if ('campo' == tipoCampo) {
        jQuery('#' + idCampoTotal).val(number_format(soma, 2, ',', '.'));
    } else {
        jQuery('#' + idCampoTotal).html(number_format(soma, 2, ',', '.'));
    }
}

function ver ( $obj ) {
    if ( window.console && window.console.log ) {
        window.console.log( "Par�metro: " + this );
        window.console.log( "Par�metro: " + $obj );
    }
};

//    $.fn.debug = function( $obj ) {
//        if ( window.console && window.console.log ) {
//            window.console.log( "Par�metro: " + this );
//            window.console.log( "Par�metro: " + $obj );
//        }
//    };