(function ($) {

    /**
     * @see $.fn.isValid
     * @param {string} txt
     * @param {object} options
     */
    $.deleteItem = function (options) {
        $().deleteItem(options);
    };

    $.fn.deleteItem = function (options) {
        var defaults = {controller: '', action: '', text: 'Deseja realmente deletar este item?', id: '', functionSucsess: '', retorno: false};
        var setings = $.extend({}, defaults, options);

        $('#modal-confirm').find('.modal-body').html('<p>' + setings.text + '</p>');

        // HTML botoes.
        var buttons = '<button type="button" class="btn btn-danger buttonSim">Sim</button><button type="button" class="btn btn-primary" data-dismiss="modal">Não</button>';
        $('#modal-confirm').find('.modal-footer').html(buttons);

        $('#modal-confirm').find('.buttonSim').click(function (callback) {

            $.post(window.location.href, setings, function (result) {

                $('#modal-confirm').modal('hide');

                var html = result['msg'];
                $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);


                if (setings.functionSucsess) {
                    functionSucsess = setings.functionSucsess;
                    if (setings.retorno) {
                        eval(functionSucsess + '(result)');
                    } else {
                        eval(functionSucsess + '()');
                    }

                }

            }, 'json');
        });

        // Mostrando modal.
        $('#modal-confirm').modal('show');
    };

    /**
     * @see $.fn.isValid
     * @param {string} txt
     * @param {object} options
     */
    $.confirm = function (options) {
        $().confirm(options);
    };

    $.fn.confirm = function (options) {
    };

    /**
     * @see $.fn.isValid
     * @param {string} txt
     * @param {object} options
     */
    $.renderAjax = function (options) {
        $().renderAjax(options);
    };

    $.fn.renderAjax = function (options) {
        var defaults = {controller: '', action: '', container: 'container', dataForm: ''};
        var setings = $.extend({}, defaults, options);
        $.post(window.location.href, setings, function (result) {
            $('#' + setings.container).hide().html(result).fadeIn();
        });
    };

    /**
     * @see $.fn.isValid
     * @param {string} txt
     * @param {object} options
     */
    $.searchAjax = function () {
        $().saveAjax();
    };

    $.fn.searchAjax = function (options) {
        var form = $(this);
        var defaults = {controller: form.find('#controller').val(), action: '', container: 'container_list', dataForm: form.serializeArray()};
        var setings = $.extend({}, defaults, options);
        $.post(window.location.href, setings, function (result) {
            $('#' + setings.container).hide().html(result).fadeIn();
        });
    };

    /**
     * @see $.fn.isValid
     * @param {string} txt
     * @param {object} options
     */
    $.clearForm = function () {
        $().clearForm();
    };

    $.fn.clearForm = function () {
        form = $(this);
        form.each(function () {
            this.reset(); //Cada volta no laco o form atual sera resetado
        });

        form.find('input[type="text"], input[type="hidden"]').each(function () {
            $(this).val(''); //Cada volta no laco o form atual sera resetado
        });

        form.find('select').each(function () {
            $(this).val(''); //Cada volta no laco o form atual sera resetado
        });

        form.find('checkbox').each(function () {
            $(this).removeAttr('checked'); //Cada volta no laco o form atual sera resetado
        });

        form.find('.chosen-select').next().find('.chosen-single').addClass('chosen-default');
        form.find('.chosen-select').next().find('.chosen-single span').html('<span>Selecione</span>');
        form.find('.chosen-select').next().find('.chosen-single abbr').remove();
    };

    /**
     * @see $.fn.isValid
     * @param {string} txt
     * @param {object} options
     */
    $.saveAjax = function () {
        $().saveAjax();
    };

    $.fn.saveAjax = function (options) {
        var form = $(this);

        var defaults = {controller: form.find('input[name=controller]').val(), action: form.find('input[name=action]').val(), functionSucsess: '', retorno: false, clearForm: true, displayErrorsInput: true};
        var setings = $.extend({}, defaults, options);

        var dataForm = form.serialize() + '&' + '&controller=' + setings.controller + '&action=' + setings.action;

        $.post(window.location.href, dataForm, function (result) {

            if (result['status'] == true) {
                form.find('.has-error').removeClass('has-error');
                form.find('.erro_input').remove();
                var html = '<div class="col-lg-12"><div class="alert alert-dismissable alert-success"><strong>Sucesso! </strong>' + result['msg'] + '<a class="alert-link" href="#"></a></div></div>';

                if (setings.clearForm) {
                    // Limpando formulario.
                    form.clearForm();
                    $('#buttonClear').fadeIn();
                    $('#buttonSave').fadeIn();
                    $('#buttonSearch').fadeIn();
                    $('#buttonCancel').hide();
                    $('#buttonEdit').hide();
                    $('#buttonEdit').hide();
                }

                if (setings.functionSucsess) {
                    functionSucsess = setings.functionSucsess;
                    if (setings.retorno) {
                        eval(functionSucsess + '(result)');
                    } else {
                        eval(functionSucsess + '()');
                    }
                }
            } else {
                var html = '';

                $('.has-error').removeClass('has-error');
                form.find('.erro_input').remove();


                $(result['result']).each(function () {

                    if (this.name) {
                        element = form.find('#' + this.name);
                        label = form.find('label[for=' + this.name + ']').text();
                        if (!label) {
                            label = element.closest('.form-group').children('label').text();
                        }
                        html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> ' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
                        element.closest('.form-group').addClass('has-error');
                        if (!($("#block_error_" + this.name).length > 0) && setings.displayErrorsInput === true) {
                            element.parent().append('<p class="help-block erro_input">' + this.msg + '</p>');
                        }

                    }
                    else if(this.msg)
                    {
                        html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
                    }
                });

                if (html === '') {
                    html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + result['msg'] + '</div></div>'
                }
            }
            $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
        }, 'json');

    };

    $.saveAjaxRetorno = function () {
        $().saveAjax();
    };

    $.fn.saveAjaxRetorno = function (options) {
        var form = $(this);

        var defaults = {controller: form.find('input[name=controller]').val(), action: form.find('input[name=action]').val(), functionSucsess: '', retorno: false, funcaoRetornoInvalido: '', clearForm: true, displayErrorsInput: true};
        var setings = $.extend({}, defaults, options);

        var dataForm = form.serialize() + '&' + '&controller=' + setings.controller + '&action=' + setings.action;

        $.post(window.location.href, dataForm, function (result) {

            if (result['status'] == true) {
                form.find('.has-error').removeClass('has-error');
                form.find('.erro_input').remove();
                var html = '<div class="col-lg-12"><div class="alert alert-dismissable alert-success"><strong>Sucesso! </strong>' + result['msg'] + '<a class="alert-link" href="#"></a></div></div>';

                if (setings.clearForm) {
                    // Limpando formulario.
                    form.clearForm();
                    $('#buttonClear').fadeIn();
                    $('#buttonSave').fadeIn();
                    $('#buttonSearch').fadeIn();
                    $('#buttonCancel').hide();
                    $('#buttonEdit').hide();
                    $('#buttonEdit').hide();
                }

                if (setings.functionSucsess) {
                    functionSucsess = setings.functionSucsess;
                    if (setings.retorno) {
                        eval(functionSucsess + '(result)');
                    } else {
                        eval(functionSucsess + '()');
                    }
                }
            } else {
                if (setings.funcaoRetornoInvalido) {
                    funcaoRetornoInvalido = setings.funcaoRetornoInvalido;
                    if (setings.retorno) {
                        eval(funcaoRetornoInvalido + '(result)');
                    } else {
                        eval(funcaoRetornoInvalido + '()');
                    }
                } else {
                    var html = '';

                    $('.has-error').removeClass('has-error');
                    form.find('.erro_input').remove();

                    $(result['result']).each(function () {
                        element = form.find('#' + this.name);
                        label = form.find('label[for=' + this.name + ']').text();
                        if (!label) {
                            label = element.closest('.form-group').children('label').text();
                        }
                        html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> ' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
                        element.closest('.form-group').addClass('has-error');
                        if (!($("#block_error_" + this.name).length > 0) && setings.displayErrorsInput === true) {
                            element.parent().append('<p class="help-block erro_input">' + this.msg + '</p>');
                        }
                    });
                    if (html === '') {
                        html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + result['msg'] + '</div></div>'
                    }
                }
            }
            $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
        }, 'json');

    };

    /**
     * @see $.fn.msg
     * @param {string} txt
     * @param {object} options
     */
    $.msg = function (txt, options) {
        $().msg(txt, options);
    }

    $.fn.isValid = function (callback) {

        var form = $(this).attr('id');

        if (!form) {
            var form = 'form_save';
        }

        var html = '';
        var isValid = true;

        $('.has-error').removeClass('has-error');
        $('#' + form).find('.erro_input').remove();
        $('#' + form).find('[required]').each(
            function () {
                if ($(this).val() == '') {
                    msg = 'Não pode estar vazio'
                    element = $('#' + form).find('#' + this.id);
                    label = $('#' + form).find('label[for=' + this.name + ']').text();
                    if (!label) {
                        label = element.closest('.form-group').children('label').text();
                    }
                    html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> ' + msg + '.<a class="alert-link" href="#"></a></div></div>'
                    element.closest('.form-group').addClass('has-error');
                    if (!($("#block_error_" + this.name).length > 0)) {
                        element.parent().append('<p class="help-block erro_input">' + msg + '.</p>');
                    }

                    isValid = false;
//                    return false;
                }
            }
        );

        if (html != '') {
            $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
        }

        callback(isValid);
    };

})(jQuery);


// _____________________________________________________________________________


jQuery(function () {
    /*
     * Funcao utilizada para marcar todos os checkboxes da mesma classe
     * @autor: Orion Teles de Mesquita
     * @since: 26/09/2013
     */
    jQuery('.marcar-todos').click(function () {
        jQuery('.' + jQuery(this).attr('marcar')).prop('checked', jQuery(this).prop('checked'));
    });

    /*
     * Funcao utilizada para deixar somente n meros em um input
     * @autor: Orion Teles de Mesquita
     * @since: 26/09/2013
     */
    jQuery('input.inteiro').keyup(function () {
        jQuery(jQuery(this)).val(jQuery(this).val().replace(/[^0-9]/g, ''));
    });
});

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

function implode(glue, pieces) {
    // http://kevin.vanzonneveld.net
    // +   original by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // +   improved by: Waldo Malqui Silva
    // +   improved by: Itsacon (http://www.itsacon.net/)
    // +   bugfixed by: Brett Zamir (http://brett-zamir.me)
    // *     example 1: implode(' ', ['Kevin', 'van', 'Zonneveld']);
    // *     returns 1: 'Kevin van Zonneveld'
    // *     example 2: implode(' ', {first:'Kevin', last: 'van Zonneveld'});
    // *     returns 2: 'Kevin van Zonneveld'
    var i = '',
        retVal = '',
        tGlue = '';

    if (arguments.length === 1) {
        pieces = glue;
        glue = '';
    }
    if (typeof pieces === 'object') {
        if (Object.prototype.toString.call(pieces) === '[object Array]') {
            return pieces.join(glue);
        }
        for (i in pieces) {
            retVal += tGlue + pieces[i];
            tGlue = glue;
        }
        return retVal;
    }
    return pieces;
}

/**
 * Funcao utilizada para somar os valores dos campos com a mesma classe.
 *
 * @param string classeSoma - Nome da classe dos campos a serem somados
 * @autor: Orion Teles de Mesquita
 * @since: 26/05/2014
 */
function somarCampos(classeSoma) {
    var soma = 0;
    jQuery('.' + classeSoma).each(function (i, obj) {
        var valor = jQuery(obj).val() ? str_replace(['.', ','], ['', '.'], jQuery(obj).val()) : 0;
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
 * @autor: Orion Teles de Mesquita
 * @since: 26/05/2014
 */
function atualizaTotal(classeSoma, idCampoTotal, tipoCampo) {
    var soma = somarCampos(classeSoma);
    if ('campo' == tipoCampo) {
        jQuery('#' + idCampoTotal).val(number_format(soma, 2, ',', '.'));
    } else {
        jQuery('#' + idCampoTotal).html(number_format(soma, 2, ',', '.'));
    }
}

function utf8_decode(str_data) {
    //  discuss at: http://phpjs.org/functions/utf8_decode/
    // original by: Webtoolkit.info (http://www.webtoolkit.info/)
    //    input by: Aman Gupta
    //    input by: Brett Zamir (http://brett-zamir.me)
    // improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // improved by: Norman "zEh" Fuchs
    // bugfixed by: hitwork
    // bugfixed by: Onno Marsman
    // bugfixed by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // bugfixed by: kirilloid
    //   example 1: utf8_decode('Kevin van Zonneveld');
    //   returns 1: 'Kevin van Zonneveld'

    var tmp_arr = [],
        i = 0,
        ac = 0,
        c1 = 0,
        c2 = 0,
        c3 = 0,
        c4 = 0;

    str_data += '';

    while (i < str_data.length) {
        c1 = str_data.charCodeAt(i);
        if (c1 <= 191) {
            tmp_arr[ac++] = String.fromCharCode(c1);
            i++;
        } else if (c1 <= 223) {
            c2 = str_data.charCodeAt(i + 1);
            tmp_arr[ac++] = String.fromCharCode(((c1 & 31) << 6) | (c2 & 63));
            i += 2;
        } else if (c1 <= 239) {
            // http://en.wikipedia.org/wiki/UTF-8#Codepage_layout
            c2 = str_data.charCodeAt(i + 1);
            c3 = str_data.charCodeAt(i + 2);
            tmp_arr[ac++] = String.fromCharCode(((c1 & 15) << 12) | ((c2 & 63) << 6) | (c3 & 63));
            i += 3;
        } else {
            c2 = str_data.charCodeAt(i + 1);
            c3 = str_data.charCodeAt(i + 2);
            c4 = str_data.charCodeAt(i + 3);
            c1 = ((c1 & 7) << 18) | ((c2 & 63) << 12) | ((c3 & 63) << 6) | (c4 & 63);
            c1 -= 0x10000;
            tmp_arr[ac++] = String.fromCharCode(0xD800 | ((c1 >> 10) & 0x3FF));
            tmp_arr[ac++] = String.fromCharCode(0xDC00 | (c1 & 0x3FF));
            i += 4;
        }
    }

    return tmp_arr.join('');
}


function utf8_encode(argString) {
    //  discuss at: http://phpjs.org/functions/utf8_encode/
    // original by: Webtoolkit.info (http://www.webtoolkit.info/)
    // improved by: Kevin van Zonneveld (http://kevin.vanzonneveld.net)
    // improved by: sowberry
    // improved by: Jack
    // improved by: Yves Sucaet
    // improved by: kirilloid
    // bugfixed by: Onno Marsman
    // bugfixed by: Onno Marsman
    // bugfixed by: Ulrich
    // bugfixed by: Rafal Kukawski
    // bugfixed by: kirilloid
    //   example 1: utf8_encode('Kevin van Zonneveld');
    //   returns 1: 'Kevin van Zonneveld'

    if (argString === null || typeof argString === 'undefined') {
        return '';
    }

    var string = (argString + ''); // .replace(/\r\n/g, "\n").replace(/\r/g, "\n");
    var utftext = '',
        start, end, stringl = 0;

    start = end = 0;
    stringl = string.length;
    for (var n = 0; n < stringl; n++) {
        var c1 = string.charCodeAt(n);
        var enc = null;

        if (c1 < 128) {
            end++;
        } else if (c1 > 127 && c1 < 2048) {
            enc = String.fromCharCode(
                (c1 >> 6) | 192, (c1 & 63) | 128
            );
        } else if ((c1 & 0xF800) != 0xD800) {
            enc = String.fromCharCode(
                (c1 >> 12) | 224, ((c1 >> 6) & 63) | 128, (c1 & 63) | 128
            );
        } else { // surrogate pairs
            if ((c1 & 0xFC00) != 0xD800) {
                throw new RangeError('Unmatched trail surrogate at ' + n);
            }
            var c2 = string.charCodeAt(++n);
            if ((c2 & 0xFC00) != 0xDC00) {
                throw new RangeError('Unmatched lead surrogate at ' + (n - 1));
            }
            c1 = ((c1 & 0x3FF) << 10) + (c2 & 0x3FF) + 0x10000;
            enc = String.fromCharCode(
                (c1 >> 18) | 240, ((c1 >> 12) & 63) | 128, ((c1 >> 6) & 63) | 128, (c1 & 63) | 128
            );
        }
        if (enc !== null) {
            if (end > start) {
                utftext += string.slice(start, end);
            }
            utftext += enc;
            start = end = n + 1;
        }
    }

    if (end > start) {
        utftext += string.slice(start, stringl);
    }

    return utftext;
}

/**
 * Funcao utilizada para evento mouseOver.
 *
 * @param object elemento - Elemento que recebera o evento
 * @autor: Fellipe Esteves
 * @since: 26/05/2014
 */
function MouseOver(objeto) {
    if (objeto.type == "text" || objeto.type == "password") {
        if (objeto.className != 'clsMouseFocus') {
            objeto.className = objeto.className.replace("normal", "clsMouseOver");
        }

    } else if (objeto.type == "textarea") {
        if (objeto.className != 'txareaclsMouseFocus') {
            objeto.className = objeto.className.replace("txareanormal", "txareaclsMouseOver");
        }
    }
    return true;
}

/**
 * Funcao utilizada para evento mouseOut.
 *
 * @param object elemento - Elemento que recebera o evento
 * @autor: Fellipe Esteves
 * @since: 26/05/2014
 */
function MouseOut(objeto) {
    if (objeto.type == "text" || objeto.type == "password") {
        if (objeto.className != 'clsMouseFocus') {
            objeto.className = objeto.className.replace("clsMouseOver", "normal");
        }

    } else if (objeto.type == "textarea") {
        if (objeto.className != 'txareaclsMouseFocus') {
            objeto.className = objeto.className.replace("txareaclsMouseOver", "txareanormal");
        }
    }
    return true;
}

/**
 * Funcao utilizada para evento mouseClick.
 *
 * @param object elemento - Elemento que recebera o evento
 * @autor: Fellipe Esteves
 * @since: 26/05/2014
 */
function MouseClick(objeto) {
    if (objeto.type == "text" || objeto.type == "password") {
        objeto.className = objeto.className.replace("clsMouseOver", "clsMouseFocus");
        objeto.className = objeto.className.replace("normal", "clsMouseFocus");
    } else if (objeto.type == "textarea") {
        objeto.className = objeto.className.replace("txareanormal", "txareaclsMouseFocus");
        objeto.className = objeto.className.replace("txareaclsMouseOver", "txareaclsMouseFocus");
    }
}

/**
 * Funcao utilizada para evento mouseBlur.
 *
 * @param object elemento - Elemento que recebera o evento
 * @autor: Fellipe Esteves
 * @since: 26/05/2014
 */
function MouseBlur(objeto) {
    if (objeto.type == "text" || objeto.type == "textarea" || objeto.type == "password") {
        if (objeto.type == "textarea") {
            objeto.className = objeto.className.replace("txareaclsMouseOver", "txareanormal");
            objeto.className = objeto.className.replace("txareaclsMouseFocus", "txareanormal");
        }
        else {
            objeto.className = objeto.className.replace("clsMouseOver", "normal");
            objeto.className = objeto.className.replace("clsMouseFocus", "normal");
        }
    }
}

/**
 * Verifica campos n?o preenchidos.
 *
 * O campo ? dito n?o preenchido caso somente o caracter ' '. Verifica
 * tamb?m o campo do tipo select, neste caso deve haver pelo menos um
 * item selecionado e nenhum deles deve possuir valor vazio.
 *
 * @param campo
 * @param label
 * @return void
 */
function validaBranco(campo, label) {
    var i = 0;
    var teste_campo = "false"; //variavel para teste de espacos em branco
    // campo instanceof HTMLSelectElement  d? problema no Microsoft Internet Explorer
    if (campo.tagName == 'SELECT') {
        var tamanho_select = campo.options.length;
        var possui_selecionado = false;
        var possui_vazio = false;
        for (i = 0; i < tamanho_select; i++) {
            if (campo.options[i].selected) {
                possui_selecionado = true;
                if (campo.options[i].value == '') {
                    possui_vazio = true;
                }
            }
        }
        if (possui_selecionado && !possui_vazio) {
            return true;
        }
        alert("Campo obrigat?rio: " + label + ".");
        return false;
    }
    else {
        var tamanho_campo = campo.value.length;
        if (tamanho_campo != 0) {
            for (i = 0; i < tamanho_campo; i++) {
                if (campo.value.charAt(i) != " ") {
                    teste_campo = "true"; // existe caracter diferente de branco
                }
            }
            if (teste_campo == "false") // todos os caracteres digitados s?o brancos
            {
                alert("Campo obrigat?rio: " + label + ".");
                campo.focus();
                return false;
            }
            else {
                return true;
            }
        }
        else {
            alert("Campo obrigat?rio: " + label + ".");
            campo.focus();
            return false;
        }
    }
}

function validaRadio(campo, label) {
    var radiogroup = campo;
    var itemchecked = false;
    for (var j = 0; j < radiogroup.length; ++j) {
        if (radiogroup[j].checked) {
            itemchecked = true;
            break;
        }
    }
    if (!itemchecked) {
        alert("Escolha uma op??o para o Campo: " + label + ".");
        if (campo[0].focus)
            campo[0].focus();
        return false;
    }
    return true;
}

/**
 * Funcao utilizada para contar caracteres restantes.
 *
 * @param object field - Elemento que recebera a contagem
 * @param object countfield - Numero de caracteres restantes
 * @param object maxlimit - Numero maximo de caracteres
 * @autor: Fellipe Esteves
 * @since: 26/05/2014
 */
function textCounter(field, countfield, maxlimit) {
    if (!field || !field.value) {
        countfield.value = maxlimit;
        return;
    }
    if (field.value.length > maxlimit)
        field.value = field.value.substring(0, maxlimit);
    else
        countfield.value = maxlimit - field.value.length;
}

function trim(str) {
    if (!!str)
        return str.replace(/^\s+|\s+$/g, "");
    else
        return '';
}

/*
 objetivo: mascarar de acordo com a mascara passada
 caracteres: # - caracter a ser mascarado | - separador de mascaras
 modos (exemplos):
 mascara simples: "###-####"                  mascara utilizando a mascara passada
 mascara composta: "###-####|####-####"       mascara de acordo com o tamanho (length) do valor passado
 mascara dinamica: "[###.]###,##"             multiplica o valor entre colchetes de acordo com o length do valor para que a mascara seja din?mica ex: ###.###.###.###,##
 utilizar no onkeyup do objeto
 ex: onkeyup="this.value = mascara_global('#####-###',this.value);"
 tratar o maxlength do objeto na pagina (a função nao trata isso)
 */
function mascaraglobal(mascara, valor) {
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
    //tirando mascara do valor j? existente
    for (i = 0; i < valor.length; i++) {
        if (!isNaN(valor.substr(i, 1))) {
            tvalor = tvalor + valor.substr(i, 1);
        }
    }
    valor = tvalor;

    //formatar mascara dinamica
    for (i = 0; i < temp.length; i++) {
        mult = "";
        validar = 0;
        for (j = 0; j < temp[i].length; j++) {
            if (temp[i].substr(j, 1) == "]") {
                temp[i] = temp[i].substr(j + 1);
                break;
            }
            if (validar == 1)mult = mult + temp[i].substr(j, 1);
            if (temp[i].substr(j, 1) == "[")validar = 1;
        }
        for (j = 0; j < valor.length; j++) {
            temp[i] = mult + temp[i];
        }
    }


    //verificar qual mascara utilizar
    if (temp.length == 1) {
        mascara_utilizar = temp[0];
        mascara_limpa = "";
        for (j = 0; j < mascara_utilizar.length; j++) {
            if (mascara_utilizar.substr(j, 1) == caracter) {
                mascara_limpa = mascara_limpa + caracter;
            }
        }
        tam = mascara_limpa.length;
    } else {
        //limpar caracteres diferente do caracter da m?scara
        for (i = 0; i < temp.length; i++) {
            mascara_limpa = "";
            for (j = 0; j < temp[i].length; j++) {
                if (temp[i].substr(j, 1) == caracter) {
                    mascara_limpa = mascara_limpa + caracter;
                }
            }

            if (valor.length > mascara_limpa.length) {
                if (dif > (valor.length - mascara_limpa.length)) {
                    dif = valor.length - mascara_limpa.length;
                    mascara_utilizar = temp[i];
                    tam = mascara_limpa.length;
                }
            } else if (valor.length < mascara_limpa.length) {
                if (dif > (mascara_limpa.length - valor.length)) {
                    dif = mascara_limpa.length - valor.length;
                    mascara_utilizar = temp[i];
                    tam = mascara_limpa.length;
                }
            } else {
                mascara_utilizar = temp[i];
                tam = mascara_limpa.length;
                break;
            }
        }
    }

    //validar tamanho da mascara de acordo com o tamanho do valor
    if (valor.length > tam) {
        valor = valor.substr(0, tam);
    } else if (valor.length < tam) {
        masct = "";
        j = valor.length;
        for (i = mascara_utilizar.length - 1; i >= 0; i--) {
            if (j == 0) break;
            if (mascara_utilizar.substr(i, 1) == caracter) {
                j--;
            }
            masct = mascara_utilizar.substr(i, 1) + masct;
        }
        mascara_utilizar = masct;
    }

    //mascarar
    j = mascara_utilizar.length - 1;
    for (i = valor.length - 1; i >= 0; i--) {
        if (mascara_utilizar.substr(j, 1) != caracter) {
            ret = mascara_utilizar.substr(j, 1) + ret;
            j--;
        }
        ret = valor.substr(i, 1) + ret;
        j--;
    }

    return ret;
}

/**
 * Busca dados no formato JSON em um URL passada como parametro e alimenta um select
 *
 * @param {string} requisicao Caminho onde sera buscada as informacoes para o preenchimento do select. Seu retorno deve ser um json
 * @param {JqueryObject} select Campo que sera alimentado com o retorno da url
 * @param {string} campoDescricao Nome do atributo no json do texto do option
 * @param {string} campoCodigo Nome do atributo no json do valor do option
 * @param {Object} parametros Valores a serem passados via POST pela URL
 * @param {string} primeiraOpcao Primeiro option do select.
 * @param {mixed} valorSelecionado Valor que vira pre-preenchido no select alimentado
 * @param {function} funcao Função que será executada após o carregamento do Select
 * @return {void} description
 */
function carregarSelectPorJson(requisicao, select, campoCodigo, campoDescricao, parametros, primeiraOpcao, valorSelecionado, funcao){
    parametros = parametros? parametros: {};
    $(select).empty();
    if($(select).hasClass('chosen') || $(select).hasClass('chosen-select')){
        $(select).trigger("chosen:updated");
    }
    $.getJSON(
        requisicao,
        parametros,
        function(result){
            if(result){
                if(primeiraOpcao){
                    $("<option>").html(primeiraOpcao).val("").appendTo($(select));
                }

                $.each(result, function(i, data){
                    var option = $("<option>").val(data[ campoCodigo ])
                        .html(data[ campoDescricao ])
                        .appendTo($(select));

                    if(valorSelecionado && valorSelecionado == data[ campoCodigo ]){
                        option.attr("selected", true);
                    }
                });
                if($(select).hasClass('chosen') || $(select).hasClass('chosen-select')){
                    $(select).trigger("chosen:updated");
                }
                if(funcao){
                    funcao();
                }
            }
        });
}