/*********** SOLUCAO ***********/
function excluir(id) {
    $.deleteItem({
        controller: 'default',
        action: 'excluir',
        retorno: true,
        text: 'Deseja realmente excluir este registro?',
        id: id,
        functionSucsess: 'atualizaGrid'
    });
}

function editar(id) {
    $.post(window.location.href, {'controller': 'default', 'action': 'cadastrar', 'id': id}, function (html) {
        $('#tab_solucao a[href="#cadastro_solucao"]').tab('show');
        $('#div_cadastro_solucao').html(html);
    });
}

function atualizaGrid(data) {
    $.post(window.location.href, {'controller': 'default', 'action': 'listar'}, function (html) {
        $('#div_listar').html(html);
    });
}

function aposInserirSolucao(data) {
    $('#aba_cadastro_etapa').closest('li').removeClass('disabled');
    $('#aba_anexar_boletim').closest('li').removeClass('disabled');
    $('#div_solucao_selecionada').closest('.row').show();
    $('#div_solucao_selecionada').html(data.tituloSolucao);
    editar(data.idSolucao);
}

function cadastrarSolucao() {
    $.post(window.location.href, {'controller': 'default', 'action': 'cadastrar'}, function (html) {
        $('#tab_solucao a[href="#cadastro_solucao"]').tab('show');
        $('#div_cadastro_solucao').html(html);
    });
}

function limparSolucao() {
    $.post(window.location.href, {controller: 'default', action: 'index'}, function (html) {
        $('#etpid').val('');
        $('#etpdsc').val('');
        $('#etpordem').val('');
        $('#acaid_etapa').prop('selectedIndex', 0);
        $('#etpobs').val('');
        $('#div_etapa_selecionada').html('');
    });
}

function painel(id, obj) {
    $.post(window.location.href, {'controller': 'painel', 'action': 'index', solid: id}, function (html) {
        $('#conteudo_principal').html(html);
    });
}

function enviarOrdenacaoSolucao(event, ui, sortedIDs) {
    $.post(window.location.href, {controller: 'default', action: 'ordenar', novaOrdem: sortedIDs}, function (html) {
        $('#div_listar').html(html);
    });

}
/***********     Anexar Boletim  ***********/

function boletim(id, solid) {
    $.post(window.location.href, {'controller': 'boletim', 'action': 'index', solid: solid, id: id}, function (html) {
        $('#tab_solucao a[href="#anexar_boletim"]').tab('show')
        $('#div_anexar_boletim').html(html);
    });
}

function atualizaGridBoletim(data) {
    $.post(window.location.href, {
        'controller': 'boletim',
        'action': 'listar',
        solid: $('#solid_anexo_boletim').val()
    }, function (html) {
        $('#div_listar_boletim').html(html);
    });
}


/*********** ETAPA ***********/
function limparEtapa() {
    $.post(window.location.href, {controller: 'etapa', action: 'limpar'}, function (html) {
        $('#etpid').val('');
        $('#etpdsc').val('');
        $('#etpordem').val('');
        $('#acaid_etapa').prop('selectedIndex', 0);
        $('#etpobs').val('');
        $('#div_etapa_selecionada').html('');
    });
}

function cadastrarEtapa(id, solid) {
    $.post(window.location.href, {'controller': 'etapa', 'action': 'cadastrar', solid: solid, id: id}, function (html) {
        $('#tab_solucao a[href="#cadastro_etapa"]').tab('show')
        $('#div_cadastro_etapa').html(html);
    });
}

function atualizaGridEtapaEmSolucao(dados) {
    $.post(window.location.href, {'controller': 'etapa', 'action': 'listar', 'id': dados.solid}, function (html) {
        $('#tr_etapa_' + dados.solid).html(html);
    });
}

function atualizaGridEtapa(data) {
    limparEtapa();
    $.post(window.location.href, {'controller': 'etapa', 'action': 'listar'}, function (html) {
        $('#div_listar_etapa').html(html);
    });
}

function editarEtapa(objLink) {
    $.post(window.location.href, {
        'controller': 'etapa',
        'action': 'editar',
        'id': objLink.data('id')
    }, function (html) {
        $('#div_form_etapa').html(html);

        $('#div_solucao_selecionada').closest('.row').show();
        if ($('#tituloSolucao').val()) {
            $('#div_solucao_selecionada').html($('#tituloSolucao').val());
        }

        if ($('.btn_excluir_etapa').length) {
            var etapaSelecionada = '<b>Etapa: </b>' + objLink.closest('td').next().next().text();
        } else {
            var etapaSelecionada = '<b>Etapa: </b>' + objLink.closest('td').next().text();
        }

        $('#div_etapa_selecionada').html(etapaSelecionada);
        $('#aba_cadastro_atividade').closest('li').removeClass('disabled');
    });
}

function enviarOrdenacaoEtapa(event, ui, solid, sortedIDs) {
    $.post(window.location.href, {
        controller: 'etapa',
        action: 'ordenar',
        novaOrdem: sortedIDs,
        solid: solid
    }, function (html) {
        if (solid != 0 && solid != null) {
            $('#tr_etapa_' + solid).html(html);
        } else {
            $('#div_listar_etapa').html(html);
        }
    });
}


/*********** ATIVIDADE ***********/
function atualizaGridAtividade(data) {
    $.post(window.location.href, {'controller': 'atividade', 'action': 'listar'}, function (html) {
        $('#div_listar_atividade').html(html);
        $('#nome_executor').html('');
        $('#atvprazo').val('');
        $('#atvcritico').removeAttr('checked');
        $('#span_nome_executor').hide();
    });
}

function atualizaGridAtividadeEmSolucao(dados) {
    $.post(window.location.href, {'controller': 'atividade', 'action': 'listar', 'id': dados.etpid}, function (html) {
        $('#tr_atividade_' + dados.etpid).html(html);
    });
}

function editarAtividadePelaSolucao(id, solid, etpid) {
    $('#aba_cadastro_etapa').closest('li').removeClass('disabled');
    $('#aba_anexar_boletim').closest('li').removeClass('disabled');
    $('#aba_cadastro_atividade').closest('li').removeClass('disabled');
    cadastrarAtividade(id, solid, etpid);
}

function cadastrarAtividade(id, solid, etpid) {
    $.post(window.location.href, {
        'controller': 'atividade',
        'action': 'cadastrar',
        solid: solid,
        etpid: etpid,
        id: id
    }, function (html) {
        $('#tab_solucao a[href="#cadastro_atividade"]').tab('show');
        $('#div_cadastro_atividade').html(html);

        $('#div_solucao_selecionada').closest('.row').show();
        if ($('#tituloSolucao_atividade').val()) {
            $('#div_solucao_selecionada').html($('#tituloSolucao_atividade').val());
        }

        if ($('#tituloEtapa_atividade').val()) {
            $('#div_etapa_selecionada').html($('#tituloEtapa_atividade').val());
        }
    });
}

function enviarOrdenacaoAtividade(event, ui, etpid, sortedIDs) {
    $.post(window.location.href, {
        controller: 'atividade',
        action: 'ordenar',
        novaOrdem: sortedIDs,
        etpid: etpid
    }, function (html) {
        if (etpid != 0 && etpid != null) {
            $('#tr_atividade_' + etpid).html(html);
        } else {
            $('#div_listar_atividade').html(html);
        }
    });
}


/*********** LISTAS ***********/
function visualizar_etapa(id, element) {
    $.post(window.location.href, {controller: 'etapa', action: 'listar', id: id}, function (html) {
        $(element).closest('tr').after(function () {
            return $('<tr class="new_tr_etapa"><td style="width: 5%; background-color: #fff;"></td><td  colspan="9"><span class="label label-info">ETAPA</span><span id="tr_etapa_' + id + '">' + html + '</span></td></tr>').hide().toggle(300);
        });
        $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);" onclick="fechar_visualizacao_etapa(\'' + id + '\' , this)"><i class="glyphicon glyphicon-chevron-up"></i></a>');
    });
}

function fechar_visualizacao_etapa(id, element) {
    $($(element).closest('tr').next()[0]).hide(function () {
        $(this).remove()
    });
    $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);" onclick="visualizar_etapa(\'' + id + '\' , this)"><i class="glyphicon glyphicon-chevron-down"></i></a>');
}

function visualizar_atividade(id, element) {
    $.post(window.location.href, {controller: 'atividade', action: 'listar', id: id}, function (html) {
        $(element).closest('tr').after(function () {
            return $('</div><tr class="new_tr_atividade"><td style="width: 5%; background-color: #fff;"></td><td  colspan="8"><span class="label label-default">ATIVIDADE</span><span id="tr_atividade_' + id + '">' + html + '</span></td></tr>').hide().toggle(300);
        });
        $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);" onclick="fechar_visualizacao_atividade(\'' + id + '\' , this)"><i class="glyphicon glyphicon-chevron-up"></i></a>');
    });
}

function fechar_visualizacao_atividade(id, element) {
    $($(element).closest('tr').next()[0]).hide(function () {
        $(this).remove()
    });
    $(element).closest('tr').find('td:first').html('<a href="javascript:void(0);" onclick="visualizar_atividade(\'' + id + '\' , this)"><i class="glyphicon glyphicon-chevron-down"></i></a>');
}

/*********** GERAIS ***********/
function carregarIndicador(acaid, marcador) {
    $.post(window.location.href, {
        'controller': 'default',
        'action': 'carregarIndicador',
        acaid: acaid,
        marcador: marcador
    }, function (html) {
        $('.div_indicador').html(html);
    });
}

function retornoCadastroExecutor(retorno) {
    $('.usucpf_atividade').val(retorno.cpf);

    $('#nome_executor').html(' -- <b>Selecionado:</b> ' + retorno.nome);
    $('#span_nome_executor').show();

    if (retorno.novoUsuario == 1) {
        $('#span_nome_executor').append("<span> - Novo usuário criado, seus dados de acesso foram enviado por e-mail!</span>")
    }
    $('#dialogo_formulario_executor').modal('hide');
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
                    $.post(window.location.href, {
                        controller: 'usuario',
                        action: 'getMunicipios',
                        muncod: data.muncod,
                        regcod: data.regcod
                    }, function (data) {
                        $('#muncod').html(data);
                        $('#div_tpocod').show();
                        $('#div_entidade').show();
                    });
                }

                if (index === 'tpocod' && value) {
                    $.post(window.location.href, {
                        controller: 'usuario',
                        action: 'getOrgaos',
                        entid: data.entid,
                        tpocod: data.tpocod,
                        regcod: data.regcod,
                        muncod: data.muncod
                    }, function (data) {
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

/***************** FILTROS ************************/
function carregarIndicador(acaid, marcador) {
    $.post(window.location.href, {
        'controller': 'default',
        'action': 'carregarIndicador',
        acaid: acaid,
        marcador: marcador
    }, function (html) {
        $('.div_indicador').html(html);
    });
}

function filtroTmeid(filtraAcoesEstrategicas) {
    temids = $('#ms-temid .ms-selectable').find('.ms-selected span');
    mpneids = $('#ms-mpneid .ms-selectable').find('.ms-selected span');

    var tmeid = new Array();
    temids.each(function (index) {
        tmeid[index] = $('#temid').find('option[title="' + $(this).text() + '"]').val();
    });
    strTmeid = tmeid.join();

    var mpneid = new Array();
    mpneids.each(function (index) {
        mpneid[index] = $('#mpneid').find('option[title="' + $(this).text() + '"]').val();
    });
    strMpneid = mpneid.join();

    if(filtraAcoesEstrategicas){
       carregarAcoesEstrategicas(strTmeid);
    }
    carregarDispositivosPne(strTmeid);
    carregarArtigos(strTmeid)
    carregarIniciativa(strTmeid, null);
    carregarObjetivosEstrategicos(strTmeid, strMpneid);
}

function filtroTema(codigo, visualizacao) {
    var codigoTemaSuporte = 6;
    if (codigo == codigoTemaSuporte) {
        $('#ms-temid .ms-selectable').find('.ms-elem-selectable').removeClass('disabled');
        if(visualizacao == 'hide'){
            $('#div_metaSolucao, .div_estrategia').hide();
        }else if(visualizacao == 'show') {
            $('#div_metaSolucao, .div_estrategia').show();
        }
        filtroTmeid(false);
    } else {
        filtroTmeid(true);
        $('#div_metaSolucao, .div_estrategia').show();
    }
}

function filtroMpneid() {
    temids = $('#ms-temid .ms-selectable').find('.ms-selected span');
    mpneids = $('#ms-mpneid .ms-selectable').find('.ms-selected');

    var tmeid = new Array();
    temids.each(function (index) {
        tmeid[index] = $('#temid').find('option[title="' + $(this).text() + '"]').val();
    });
    strTmeid = tmeid.join();

    var mpneid = new Array();
    mpneids.each(function (index) {
        mpneid[index] = $('#mpneid').find('option[title="' + $(this).attr('title') + '"]').val();
    });
    strMpneid = mpneid.join();
    carregarObjetivosEstrategicos(strTmeid, strMpneid);
    carregarEstrategia(strMpneid);
}

function fitroObeid() {
    selectableObeidSpan = $('#ms-obeid .ms-selectable').find('.ms-selected span');
    var obeid = new Array();

    selectableObeidSpan.each(function (index) {
        obeid[index] = $('#obeid').find('option[title="' + $(this).text() + '"]').val();
    });
    strObeid = obeid.join();
    carregarIniciativa(null, strObeid);
}

function carregarAcoesEstrategicas(temid) {
    $.post(window.location.href, {
        'controller': 'default',
        'action': 'formularioAcoesEstrategica',
        'temid': temid
    }, function (html) {
        $('.div_acoes_estrategica').html(html);
    });
}

function carregarDispositivosPne(temid) {
    $.post(window.location.href, {
        'controller': 'default',
        'action': 'formularioMetaSolucao',
        'temid': temid
    }, function (html) {
        $('.div_dispositivo_pne').html(html);
    });
}

function carregarIniciativa(temid, obeid) {
    $.post(window.location.href, {
        'controller': 'default',
        'action': 'formularioIniciativa',
        'temid': temid,
        'obeid': obeid
    }, function (html) {
        $('.div_iniciativa').html(html);
    });
}

function carregarObjetivosEstrategicos(temid, mpneid) {
    $.post(window.location.href, {
        'controller': 'default',
        'action': 'formularioObjetivoEstrategico',
        'temid': temid,
        'mpneid': mpneid
    }, function (html) {
        $('.div_objetivo_estrategico').html(html);
    });
}

function carregarEstrategia(mpneid) {
    $.post(window.location.href, {
        'controller': 'default',
        'action': 'formularioEstrategia',
        'mpneid': mpneid
    }, function (html) {
        $('.div_estrategia').html(html);
    });
}
function carregarArtigos(temid) {
    $.post(window.location.href, {
        'controller': 'default',
        'action': 'formularioArtigo',
        'temid': temid
    }, function (html) {
        $('.div_artigo').html(html);
    });
}

function regrasAoCarregar(){
    var codigoTemaSuporte = 6;
    $('#temid :selected').each(function(id, selected){
        if ($(selected).val() == codigoTemaSuporte){
            $('#div_metaSolucao, .div_estrategia').hide();
        }
    });

    var codigoNenhuma = 'nenhuma';
    var codigoCorpoDaLei = '99999999';
    $('#mpneid :selected').each(function(id, selected){
        if ($(selected).val() == codigoNenhuma) {
            $('#div_justificativa').show();
            $('#solmetajustificativa').prop('disabled', false);
            $('.div_estrategia, .div_objetivo_estrategico, .div_iniciativa').hide();
        }
        if ($(selected).val() == codigoCorpoDaLei) {
            $('.div_estrategia, .div_objetivo_estrategico, .div_iniciativa').hide();
            $('.div_artigo').show();
        }
    });
}