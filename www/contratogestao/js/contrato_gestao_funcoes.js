/******** ORDERNAR CONTRATO **********/
function atualizarOrdemHierarquiaContrato(itensDoPai) {
    var novaOrdem = Array();
    if (itensDoPai.length > 0) {
        $.each(itensDoPai, function (index) {
            novaOrdem[index] = $(this).attr('data-tt-id');
        });
    }

    $.post(window.location.href, {
        controller: 'default',
        action: 'ordenar',
        novaOrdem: novaOrdem
    }, function (data) {
        $('#msg_retorno').text(data.msg);
        $('#div_msg_ordenacao').show();
    }, 'json');
}

/**
 * Ordena somente no mesmo nível e se possuir o mesmo ID do Item PAI
 * @param {objeto} objetoMovido objeto movido
 */
function cancelarOrdenarNos(objetoMovido) {

    var mesmoNivelNext = true;
    var mesmoNivelPrev = true;
    var mesmoPaiNext = true;
    var mesmoPaiPrev = true;

    if (objetoMovido.next().length > 0) {
        mesmoNivelNext = objetoMovido.next().data("ttNivel") !== objetoMovido.data("ttNivel");
        mesmoPaiNext = objetoMovido.next().data("ttParentId") !== objetoMovido.data("ttParentId");
    }
    if (objetoMovido.prev().length > 0) {
        mesmoNivelPrev = objetoMovido.prev().data("ttNivel") !== objetoMovido.data("ttNivel");
        mesmoPaiPrev = objetoMovido.prev().data("ttParentId") !== objetoMovido.data("ttParentId");
    }
    var mesmoNivel = (mesmoNivelNext && mesmoNivelPrev);
    var mesmoPai = (mesmoPaiNext && mesmoPaiPrev);
    return mesmoNivel || mesmoPai;
}

function ordenarNos(objetoMovido, obj) {
    objetoMovido.closest('table').find('.tr_new').each(function () {
        obj.closest('table').find('.exibirItem').removeClass('glyphicon-chevron-up').addClass('glyphicon-chevron-down');
        $(this).remove();
    });

    var nosFilhos = $("#arvore_contrato").find('tr[data-tt-parent-id=' + objetoMovido.data("ttId") + ']');
    if (nosFilhos.length > 0) {
        objetoMovido.after(nosFilhos);
        var nosFilhos = $("#arvore_contrato").find('tr[data-tt-parent-id=' + objetoMovido.data("ttId") + ']');
        $.each(nosFilhos, function () {
            recursivaOrdenarNos($(this));
        });
    }
}

function recursivaOrdenarNos(objetoFilho) {
    var nosFilhos = $("#arvore_contrato").find('tr[data-tt-parent-id=' + objetoFilho.data("ttId") + ']');
    if (nosFilhos.length > 0) {
        $.each(nosFilhos, function () {
            objetoFilho.after(nosFilhos);
            recursivaOrdenarNos($(this));
        });
    }
}

/******** MOVER CONTRATO **********/
function moverContrato(objetoDestino, objetoMovido, idPaiAntigo) {

    objetoMovido.attr("data-tt-parent-id", objetoDestino.data("ttId"));
    objetoMovido.attr("data-tt-nivel", objetoDestino.data("ttNivel") + 1);
    removerLinkFilhos(idPaiAntigo);
    atualizarHierarquiaContrato(objetoDestino, objetoMovido);
}

/******** VALIDA O MOVIMENTOS DOS ITENS **********/
function validarMudanca(objetoDestino, objetoMovido) {
    /**
     * não mover a raiz;
     * não move ao adicionar no mesmo nivel
     * move somente entre o mesmo contrato raiz
     * move somente quando o id do pai e igua ao raiz do filho
     * move somente quando o id do pai e igua ao raiz do filho
     * não move quando move sem alterar a hierarquia
     */
//    console.log(objetoDestino.data("ttNivel") + '!==' + objetoMovido.data("ttNivel"));
//    console.log(objetoDestino.data("ttRaiz") + '===' + objetoMovido.data("ttRaiz"));
//    console.log(objetoDestino.data("ttParentId") + '!==' + objetoMovido.data("ttParentId"));

    var niveisDiferentes = objetoDestino.data("ttNivel") === objetoMovido.data("ttNivel");
    var mesmoPaiRaiz = objetoDestino.data("ttRaiz") === objetoMovido.data("ttRaiz");
    var paisDiferentes = objetoDestino.data("ttParentId") !== objetoMovido.data("ttParentId");

    return  niveisDiferentes && mesmoPaiRaiz && paisDiferentes;
}

function atualizarHierarquiaContrato(objetoDestino, objetoMovido) {
    $.post(window.location.href, {
        controller: 'default',
        action: 'ordenar',
        conid: objetoDestino.data("ttConid"),
        hqcidpai: objetoDestino.data("ttId"),
        hqcid: objetoMovido.data("ttId"),
        hqcnivel: objetoDestino.data("ttNivel") + 1
    }, function (retorno) {
        var status = parseInt(retorno['status']);
        alert(retorno['msg']);
//        $('#arvore_contrato67').hide().fadeIn().html(html);
    }, 'json');
}

function removerLinkFilhos(idPaiAntigo) {
    var nosPaiAntigo = $("#arvore_contrato").find('tr[data-tt-parent-id=' + idPaiAntigo + ']');
    if (nosPaiAntigo.length === 0) {
        $('#arvore_contrato tr[data-tt-id=' + idPaiAntigo + ']').find('a').remove();
    }
}

function abrirTodosNosAPartirDoPai(noPai) {
    var item_pai = $("tr[data-tt-id=" + noPai + "]");
    var itens = $("tr[data-tt-raiz=" + item_pai.data("ttRaiz") + "], tr[data-tt-id=" + item_pai.data("ttRaiz") + "] ");

    var listarExibir = new Array();
    listarExibir[0] = parseInt(noPai);

    var i = 1;
    while (item_pai.data("ttRaiz")) {
        var parent = $("tr[data-tt-id=" + item_pai.data("ttParentId") + "]");
        listarExibir[i] = parent.data("ttId");
        item_pai = parent;
        i++;
    }

    listarExibir.reverse();

//    for	(index = 0; index < listarExibir.length; index++) {
//        $("#arvore_contrato").treetable("expandNode", listarExibir[index] );
//    }
}

function fecharModal(data) {
    $('#dialogo_cadastrar_contrato').modal('hide');
    $('#dialogo_cadastrar_contrato_item').modal('hide');
    $('#modal').modal('hide');

    var data = {controller: 'default', action: 'listar', hqcidpai_interagido: data.hqcidpai_interagido};
    $.post(window.location.href, data, function (html) {
        $('#container-listar').hide().fadeIn().html(html);
    });
}

function fatorAvaliado(retorno) {
    $('#executor, #validador, #certificador').hide();
    $('#nome_executor, #nome_validador, #nome_certificador').html('');

    var data = {controller: 'fatorAvaliado', action: 'listar', retorno: retorno};
    $.post(window.location.href, data, function (html) {
        $('#container-listar-fator-avaliado').hide().fadeIn().html(html);
    });
}

function functionSucsessFatorAvaliadoExecucao() {
    location.reload();
}

function fecharModalEtapaControle(retorno) {
    $('#dialogo_etapas_de_controle').modal('hide');
    $('#usucpf' + retorno.etapa).val(retorno.cpf);
    $('#entid' + retorno.etapa).val(retorno.entid);

    $('#nome_' + retorno.etapa).html(' -- <b>Selecionado:</b> ' + retorno.nome);
    $('#span_nome_' + retorno.etapa).show();

    if (retorno.novoUsuario == 1) {
        $('#span_nome_' + retorno.etapa).append("<span> - Novo usuário criado, seus dados de acesso foram enviado por e-mail!</span>")
    }
}

/**
 * excluir fator avaliado
 * @param {integer} id
 */
function excluir(id) {
    $.deleteItem({controller: 'fatorAvaliado', action: 'excluir', retorno: true, text: 'Deseja realmente excluir este fator?', id: id, functionSucsess: 'fatorAvaliado'});
}

/**
 * editar fator avaliado
 * @param {integer} id
 */
function editar(id) {
    $.post(window.location.href, {'controller': 'fatorAvaliado', 'action': 'editar', 'id': id}, function (html) {
        $('#container-form-fator-avaliado').html(html);
    });
}


function getCpf(cpf) {
    $.post(window.location.href, {controller: 'usuario', action: 'getUsuarioCpf', cpf: cpf}, function (data) {
        if (data.length === 0) {
            objs = $("#fieldset_pessoa_fisica input, #fieldset_pessoa_fisica select, #novoUsuario").not('#usucpf, #ususexo, :button, :submit, :reset, :hidden')
                .val('')
                .removeAttr('checked')
                .removeAttr('selected');
        } else {
            $.each(data, function (index, value) {

                if (index === 'regcod' && value) {
                    $.post(window.location.href, {controller: 'usuario', action: 'getMunicipios', muncod: data.muncod, regcod: data.regcod}, function (data) {
                        $('#muncod').html(data);
                        $('#div_tpocod').show();
                        $('#div_entidade').show();
                    });
                }

                if (index === 'orgao' && value) {
                    $('#orgao').show();
                    $('#entid').hide();
                }

                if (index === 'entid' && value) {
                    $('#orgao').hide();
                    $('#entid').show();
                }

                if (index === 'tpocod' && value) {
                    $.post(window.location.href, {controller: 'usuario', action: 'getOrgaos', entid: data.entid, tpocod: data.tpocod, regcod: data.regcod, muncod: data.muncod}, function (data) {
                        $('#entid').html(data);
                    });
                }

                $('#' + index + '[type="text"]').val(value);
                $('#' + index).not('input').val(value);

                if ($('input[name="' + index + '"][value="' + value + '"]').length > 0) {
                    $('.label_sexo').removeClass('active');
                    $('input[name="' + index + '"][value="' + value + '"]').attr('checked', 'checked').closest('label').addClass('active');
                }
            });

            $('#div_tpocod').show();
            $('#div_entidade').show();
            $("#novoUsuario").val(data.novoUsuario);
        }
    }, 'json');
}

function rolar_para(elemento) {
    $('html, body').animate({
        scrollTop: $(elemento).offset().top
    }, 2000);
}

function abrirRegras(mnuid) {
    window.open(
        '../geral/regra_tela.php?mnuid=' + mnuid,
        'usuariosonline',
        'height=500,width=800,scrollbars=yes,top=50,left=200'
    );
}

function empty(data) {
    if (typeof(data) == 'number' || typeof(data) == 'boolean') {
        return false;
    }
    if (typeof(data) == 'undefined' || data === null) {
        return true;
    }
    if (typeof(data.length) != 'undefined') {
        return data.length == 0;
    }
    var count = 0;
    for (var i in data) {
        if (data.hasOwnProperty(i)) {
            count++;
        }
    }
    return count == 0;
}