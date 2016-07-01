window.onload = function() {

    var tableRow = $(settings.template)
      , previsaoLinha = $(tableRow).html()
      , previsaoNC = $("#table-row-nc").html()
      , templateNC = Handlebars.compile(previsaoNC)
      , templateLinha = Handlebars.compile(previsaoLinha);

    setDefaulMesLiberacao();
    populateNaturezaDespesa();
    populatePI();
    populateYear();
    populateMonth();
    getPtres();
    populateAcao();

    insertNovaPrevisao(templateLinha);
    adicionaNotaCredito(templateNC);

    new ControleCheckbox("#tb_render", "#ckboxPai", ".ckboxChild");

    deletePrevisao();
    searchPtrid();

    submitFormListagem();
    cadastrarNotaCrédito();

    transferirCredito();
    checkInputValor();

    /**
     * Fix to input screen size
     * @type {*}
     */
    var idInterval = setInterval(function() {
        if ($.active == 0) {
            clearInterval(idInterval);
            setTimeout(function() {
                $(".container-ndpid .chosen-container").attr("style", "width: 155px;");
                $(".container-ptrid .chosen-container").attr("style", "width: 135px;");
                $(".container-pliid .chosen-container").attr("style", "width: 140px;");
                $("[name='provalor[]']").attr("style", "width: 120px;");
                $('[data-toggle="tooltip"]').tooltip();
            }, 400);
        }
    }, 1);
};

/**
 * Ação para submeter o formulário
 * chama validação de campos
 * @return boolean false|event submit
 */
var submitFormListagem = function() {
    $button = $("button[type='submit']");

    $button.on("click", function() {
        $("#formacao").val(($(this).attr('id') == 'submitcontinue') ? 'submitcontinue' : 'submit');

        if (formIsValid()) {
            $("#formListagem").submit();
        }

        return false;
    });
};

/**
 * Ação para adicionar previsão, usando saldo remanejado ou apenas fazendo um aditivo
 * @param context
 * @param template
 */
var actionPrevisao = function(context, template) {
    var html
      , callMethods = ["getMesesExecucao", "getPtres", "getNaturezaDespesa"]
      , calls=[]
      , objData={}
      , ELEMENT_TARGET = "A";

    objData["proid"] = new Date().getTime();
    html = template(objData);
    if ($("#tb_render tbody tr").length) {
        $("#tb_render tbody tr").last().after(html);
    } else {
        $("#tb_render tbody").append(html);
    }

    if ($(context).hasClass("useBalanceAvailable")) {
        $("#tr_"+objData["proid"]).addClass("info addRelocatedCredit");
        $("#transfer-"+objData["proid"]).val("t");

        if (context.tagName == ELEMENT_TARGET) {
            $(context).removeClass("useBalanceAvailable");
        }

    } else {
        $("#transfer-"+objData["proid"]).val("f");
    }

    $("[name='proanoreferencia[]']:last").val(settings.year);

    for (var i=0; i < callMethods.length; i++) {
        calls.push($.ajax({
            url:settings.endpoint+"&action="+callMethods[i]+"&ted="+settings.ted
        }));
    }

    $.when.apply($, calls).done(function() {
        $(arguments).each(function(i, v) {
            if (i == 0)
                $("#tb_render tbody [name='crdmesexecucao[]']").last().append(v);
            if (i == 1)
                $("#ptrid_"+objData["proid"]).last().append(v);
            if (i == 2)
                $("#tb_render tbody [name='ndpid[]']").last().append(v);
        });

        $("#tb_render tbody .tr_new select").trigger('chosen:updated');
        $("#tb_render tbody .tr_new select").chosen();

        $(".container-ndpid .chosen-container").attr("style", "width: 155px;");
        $(".container-ptrid .chosen-container").attr("style", "width: 135px;");
        $(".container-pliid .chosen-container").attr("style", "width: 140px;");
        $("[name='provalor[]']").attr("style", "width: 120px;");

        organizaMesExecucao();

    }).fail(function() {
        console.log('fail, insert new line');
    });
};

/**
 * Opção de chamada ao metodo de acão para inserir nova previsão
 * @param template
 */
var insertNovaPrevisao = function(template) {

    var $button = $(".insertNewLine, .useBalanceAvailable")
      , ELEMENT_TARGET = "SPAN";

    $button.on("click", function(e) {
        e.preventDefault();
        var that = this;

        if ((that.tagName == ELEMENT_TARGET) || (!$(that).hasClass("useBalanceAvailableOptions"))) {
            actionPrevisao(that, template);
            return;
        }

        bootbox.dialog({
            title: "Alerta",
            message: "Existe saldo disponível a ser remanejado!",
            buttons: {
                aditivo: {
                    label: "Criar nova programação",
                    className: "btn-success",
                    callback: function() {
                        actionPrevisao(that, template);
                    }
                },
                usarSaldo: {
                    label: "Usar saldo",
                    className: "btn-success",
                    callback: function() {
                        $(that).addClass("useBalanceAvailable");
                        actionPrevisao(that, template);
                    }
                },
                cancel: {
                    label: "Cancelar",
                    className: "btn-danger",
                    callback: function() {

                    }
                }
            }
        });
    });
};

/**
 * Checa se o valor da ND inserido ao fazer uma transferencia de crédito é válido
 * @return void(0)
 */
var checkInputValor = function() {
    $("[name='provalor[]']").livequery("blur", function() {
        var $el = $(this)
          , tr = $(this).parent().parent()
          , promisse
        ;

        if ($(tr).hasClass("addRelocatedCredit") && $el.val()) {
            promisse = $.ajax({url:settings.endpoint+"&action=checkSaldo&ted="+settings.ted+"&valor="+$el.val()});
            promisse.done(function(response) {
                if (response.fail) {
                    bootbox.alert("Valor inserido maior do que saldo disponível", function() {
                        $el.val("");
                    });
                }
            });
        }
    });
};

/**
 * Exclui uma linha na tela de previsão orçamentária
 * @return void(0)
 */
var deletePrevisao = function() {
    var $button = $(".remove-nc");

    $button.livequery("click", function() {
        var trRef = $(this).parent().parent()
          , that = this
          , qstring = ""
        ;

        bootbox.confirm("Deseja realmente apagar a previsão orçamentária?", function(result){
            if (result) {
                if ($(that).hasClass("newLine")) {
                    $(trRef).remove();
                } else {
                    qstring = ($(trRef).hasClass("info")) ? "&deletatransferencia=true" : "";

                    var promisse = $.ajax({url:settings.endpoint+"&action=deletePrevisao&ted="+settings.ted+"&proid="+$(that).attr("data-remove-prev")+qstring});
                    promisse.done(function(response) {
                        if (response.success) {
                            $(trRef).remove();
                            location.href=settings.endpoint+"&ted="+settings.ted;
                        }
                    });
                }
            }
        });

    });
};

/**
 * Mostra extrato de valores relacionados a uma nota de crédito
 * @return void(0)
 */
var extratoNotaCredito = function() {
    var $button = $(".extrato-nc")
      , templateExtrato = $("#table-extrato").html()
      , template = Handlebars.compile(templateExtrato);

    $button.livequery("click", function() {

        if (!$(this).attr("id")) return false;

        $.ajax({
            url:settings.endpoint+"&action=pegaExtratoNotaCredito&ted="+settings.ted+"&nc="+$(this).attr("id")
          , success: function(data) {
                //Faz a carga no template
                var html = template(data);
                $("#tb_extrato").html(html);
                $("#extratoNc").modal("show");
            }
        });
    });
};

/**
 * Chama o modal para cadastro de Nota de Crédito
 * Dentro da tela de previsões orçamentárias
 * @return void(0)
 */
var cadastrarNotaCrédito = function() {
    $("#addNc").livequery("click", function(e) {
        e.preventDefault();
        $("#registerNC").modal("show");
        submitNotaCredito();
    });
};

/**
 * Ação para formulario da modal, submete NC para cadastro
 * @return void(0)
 */
var submitNotaCredito = function() {
    var $button = $("#salvarNC")
      , inputs=["#tcpnumtransfsiafi", "#codncsiafi"]
      , errors;

    $button.livequery("click", function() {
        $("#form_nc .form-group").removeClass("has-error");
        errors = false;

        for (var i=0; inputs.length>i; i++) {
            if (!$(inputs[i]).val()) {
                var formGroup = $(inputs[i]).parent().parent();
                $(formGroup).addClass("has-error");
                errors = true;
            }
        }

        if (errors) return false;

        $("#salvarNC").attr("disabled", true);
        enviaDadosNotaCredito();
    });
};

/**
 * Faz o envio com os dados para cadastro da Nota de Crédito
 * @return void(0)
 */
var enviaDadosNotaCredito = function() {
    var proids = []
      , tcpnumtransfsiafi = $("#tcpnumtransfsiafi").val()
      , codncsiafi = $("#codncsiafi").val();

    $("[name='nc_proid[]']:checked").each(function(i, el) {
        if ($(el).val()) proids.push($(el).val());
    });

    $.ajax({
        url:settings.endpoint+"&action=salvarNotaCredito&ted="+settings.ted,
        type:'POST',
        data:{
            tcpnumtransfsiafi:tcpnumtransfsiafi
          , codncsiafi:codncsiafi
          , proid:proids.join(",")
          , ted:settings.ted
        },
        success: function(data) {
            location.reload();
        }
    });
};

/**
 * Popula os campos de plano interno, ação e descrição da ação
 * baseado no programa de trabalho selecionado (PTRES)
 * @param objeto
 */
var searchPtrid = function() {
    $("[name^='ptrid[]']").livequery("change", function(e) {
        var proid = $($(this).parent().parent()).attr("id").split("_")[1]
          , ted = window.ted = {"param": proid}
          , callMethods = ["getPlanoInterno", "getAcaoPtrid", "getDescricaoAcao"]
          , calls = []
          , eleIds = ["#pliid_", "#td_acao_", "#td_acaodsc_"];

        for (var i=0; i < callMethods.length; i++) {
            calls.push($.ajax({
                url:settings.endpoint+"&action="+callMethods[i]+"&ted="+settings.ted+"&ptrid="+$(this).val()
            }));
        }

        $.when.apply($, calls).done(function() {
            $(arguments).each(function(i, v) {
                if (i == 0) {
                    $(eleIds[i]+ted.param).append(v[0]);
                } else {
                    $(eleIds[i]+ted.param).html(v[0]);
                }
            });

            $("#tb_render tbody #tr_"+ted.param).find("select").trigger('chosen:updated').chosen();

        }).fail(function() {
            console.log('fail, populate dependent data, based with ptres');
        });
    });
};

/**
 * Carrega o combo de ano referencia e seta o valor default, se houver
 * @return void(0)
 */
var populateYear = function() {
    var $element = $("[name='proanoreferencia[]']");

    $element.each(function(i, el) {
        if ($(el).attr('data-proanoreferencia-value'))
            $(el).val($(el).attr('data-proanoreferencia-value'));

        if (i == ($element.length-1)) {
            $element.trigger('chosen:updated');
            setTimeout(function() {
                $element.chosen();
            }, 100);
        }
    });
};

/**
 * Carrega o combo de mes execução e seta o valor default, se houver
 * @return void(0)
 */
var populateMonth = function() {
    var $element = $("[name='crdmesexecucao[]']")
      , $comboData;

    $.ajax({
        url:settings.endpoint+"&action=getMesesExecucao&ted="+settings.ted,
        success:function(data) {
            $comboData = data;
        },
        complete:function() {
            $element.each(function(i, el) {
                $(el).append($comboData);
                if ($(el).attr('data-crdmesexecucao-value'))
                    $(el).val($(el).attr('data-crdmesexecucao-value'));

                if (i == ($element.length-1)) {
                    organizaMesExecucao();
                }
            });

            onChangeExecucao();
        }
    });
};

/**
 * Organiza os combos de meses para execução do objeto
 * @return void(0)
 */
var organizaMesExecucao = function() {
    var valor = $("[name='crdmesexecucao[]']:first").val();
    $("[name='crdmesexecucao[]']").each(function(i, el) {
        $(el).val(valor).attr('disabled', true);
    });

    $("[name='crdmesexecucao[]']:first").attr("disabled", false);
    $("[name='crdmesexecucao[]']").trigger('chosen:updated');
    setTimeout(function() {
        $("[name='crdmesexecucao[]']").chosen();
    }, 100);
};

/**
 * Evento onChange para o combo principal de mes de execução do objeto
 * @return void(0)
 */
var onChangeExecucao = function() {
    $("[name='crdmesexecucao[]']:first").livequery("change", function() {
        if ($(this).val()) {
            organizaMesExecucao();
        }
    });
};

/**
 * Faz a carga nos combos de Plano Interno (PI), e seta o valor default, se houver
 * @return void(0)
 */
var populatePI = function() {
    var $element = $("[name='pliid[]']"), calls=[], eleId=[];

    //fila de chamadas ajax
    $element.each(function(i, el) {
        calls.push($.ajax({
            url:settings.endpoint+"&action=getPlanoInterno&ted="+settings.ted+"&ptrid="+$(el).attr("data-ptrid-value")
        }));
        eleId.push($(el).attr("id"));
    });

    //Tratamento das chamadas ajax
    $.when.apply($, calls).done(function() {
        $(arguments).each(function(i, v) {
            var $seletor = $("#"+eleId[i]);
            $seletor.append(v);
            if ($seletor.attr("data-pliid-value")) {
                $seletor.val($seletor.attr("data-pliid-value"));
            }
        });

        //aplica o "chosen" nos elemementos carregados
        $element.trigger('chosen:updated');
        setTimeout(function() {
            $element.chosen();
        }, 100);
    }).fail(function() {
        console.log('fail');
    });
};

/**
 * Popula os combos de Natureza de Despesas
 * @return void(0)
 */
var populateNaturezaDespesa = function() {
    var $element = $("[name='ndpid[]']")
      , $optionsNatDesp;

    $.ajax({
        url:settings.endpoint+"&action=getNaturezaDespesa&ted="+settings.ted,
        success:function(data) {
            $optionsNatDesp = data;
        },
        complete:function() {
            $element.each(function(i, el) {
                $(el).append($optionsNatDesp);
                if ($(el).attr("data-ndpid-value")) {
                    $(el).val($(el).attr("data-ndpid-value"));
                }

                if (i == ($element.length-1)) {
                    $element.trigger('chosen:updated');
                    setTimeout(function(){
                        $element.chosen();
                    }, 100);
                }
            });
        }
    });
};

/**
 *
 */
var getPtres = function() {
    var $elements = $("[name='ptrid[]']");

    $.ajax({
        url:settings.endpoint+"&action=getPtres&ted="+settings.ted+"&onlypopulate=true",
        success:function(data) {},
        complete:function(data) {
            $elements.each(function(i, el) {
                $(el).append(data.responseText);
                if ($(el).attr("data-ptrid-value")) {
                    $(el).val($(el).attr("data-ptrid-value"));
                }
            });

            $elements.trigger('chosen:updated');
            setTimeout(function(){
                $elements.chosen();
            }, 100);
        }
    });
};

/**
 * Faz a carga na coluna de Ação
 * @return void(0)
 */
var populateAcao = function() {
    var $element = $("[name='ptrid[]']"), acoes=[], dscAcoes=[], eleId=[];

    //fila de chamadas ajax
    $element.each(function(i, el) {
        acoes.push($.ajax({
            url:settings.endpoint+"&action=getAcaoPtrid&ted="+settings.ted+"&ptrid="+$(el).attr("data-ptrid-value")
        }));

        if (!$("#td_acaodsc_"+$(el).attr("data-proid-value")).html()) {
            dscAcoes.push($.ajax({
                url:settings.endpoint+"&action=getDescricaoAcao&ted="+settings.ted+"&ptrid="+$(el).attr("data-ptrid-value")
            }));
        }

        eleId.push($(el).attr("data-proid-value"));
    });

    //Tratamento das chamadas ajax
    $.when.apply($, acoes).done(function() {
        $(arguments).each(function(i, v) {
            if ($("#td_acao_"+eleId[i]).length) {
                $("#td_acao_"+eleId[i]).html($.isArray(v) ? v.shift() : v);
            }
        });

        $(".container-ndpid .chosen-container").attr("style", "width: 155px;");
        $(".container-ptrid .chosen-container").attr("style", "width: 135px;");
        $(".container-pliid .chosen-container").attr("style", "width: 140px;");
    }).fail(function() {
        console.log('fail populate ações');
    });

    //Tratamento das chamadas ajax
    $.when.apply($, dscAcoes).done(function() {
        $(arguments).each(function(i, v) {
            $("#td_acaodsc_"+eleId[i]).html(v.shift());
        });
    }).fail(function() {
        console.log('fail populate descrição ação');
    });
};

/**
 * Popula os combos com os meses de liberação, e seta o valor default e houver
 * @return void(0)
 */
var setDefaulMesLiberacao = function() {
    var $element = $("[name='crdmesliberacao[]']");

    $element.each(function(i, el) {
        if ($(el).attr("data-crdmesliberacao-value")) {
            $(el).val($(el).attr("data-crdmesliberacao-value"));
        }

        if (i == ($element.length-1)) {
            $element.trigger('chosen:updated');
            setTimeout(function(){
                $element.chosen(); //{max_selected_options: 1}
            }, 100);
        }
    });
};

/**
 * Adiciona linha com valor da Nota de crédito cadastrada
 * @return void(0)
 */
var adicionaNotaCredito = function(template) {
    for (var i in settings.nc) {
        if (settings.nc.hasOwnProperty(i)) {
            var html = template({
                lote: settings.nc[i]
              , provalor: settings.valor[i]
            });
            $("[data-lote-nc='"+settings.nc[i]+"']:last").after(html);
        }
    }

    extratoNotaCredito();
};

/**
 *
 * @return {boolean}
 */
var formIsValid = function() {
    var erro = [];

    $.each($("#formListagem input[type=\"text\"], #formListagem select"), function(i, el) {
        if ($(el).attr("autocomplete") != "off" && $(el).val() == "") {
            erro.push($(el).attr("id"));
        }
    });

    if (erro.length) {
        bootbox.alert("Existem campos em branco no formulário, todos os campos devem ser preenchidos!");
        $("#"+erro.shift()).focus();
        return false;
    } else {
        return true;
    }
};

/**
 * Configura e popula modal para transferencia de crédito
 * @return void(0)
 */
var transferirCredito = function() {
    var $button = $(".tranferir-credito")
      , templateFormTransfer = $("#table-form-transfer").html()
      , formTemplate = Handlebars.compile(templateFormTransfer)
      , infoTemplate = Handlebars.compile($("#table-info-nc").html())
    ;

    $button.livequery("click", function(e) {
        e.preventDefault();

        var $request = $.ajax({url:settings.endpoint+"&action=getPrevisao&ted="+settings.ted+"&proid="+$(this).attr("data-target-proid")});
        $request.done(function(data) {
            $("#table-original-nc").html(infoTemplate(data));
            $("#ghost_proid").val(data.proid);
            validFormTransfer();
        });

        $("#table-form").html(formTemplate({}));
        $("#transferValor").modal("show");
    });
};

/**
 * Validação do formulário de transferencia de crédito
 * @return {boolean}
 */
var validFormTransfer = function() {
    $("#salvarRm").livequery("click", function() {
        var error = [];

        ["nc_devolucao", "valor_remanejar", "observacao"].forEach(function(el, i) {
            if (!$("#"+el).val()) {
                error.push(el);
            }
        });

        if (error.length) {
            bootbox.alert("Existe(m) campo(s) pendente(s) de preenchimento!");
            return false;
        }

        enviaFormTransfer();
    });
};

/**
 * Enviar form de transferencia de crédito
 */
var enviaFormTransfer = function() {
    $("#salvarRm").attr("disabled", true);

    var promisse = $.ajax({
        url: settings.endpoint+"&action=transferencia&ted="+settings.ted
      , type: "post"
      , data: $("#web-form-transfer").serialize()
    });

    promisse.done(function(response) {
        bootbox.alert(response.mensagem, function(){
            if (response.redirect) {
                location.href=settings.endpoint+"&ted="+settings.ted;
            } else {
                $("#salvarRm").attr("disabled", false);
            }
        });
    });
};