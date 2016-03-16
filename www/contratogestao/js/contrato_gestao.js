$(document).ready(function () {
    $('.exibirItem').off('click').on('click', function () {
        exibirNivel($(this), true);
    });

    /** TAB BOOTSTRAP*/
    $('#tab_gestao_contrato a[href="#conteudo_principal"]').not('.disabled').click(function (e) {
        e.preventDefault();
        $(this).tab('show');
    });

    /*** ABRE O FORMULARIO PARA ADICIONAR CONTRATO ***/
    $('.adicionarContrato').off('click').on('click', function () {
        $.post(window.location.href, {controller: 'default', action: 'adicionar', id: $(this).attr('id')}, function (data) {
            $('#form_contrato').html(data);
            $('#dialogo_cadastrar_contrato').modal('show');
        });
    });

    /*** ABRE O FORMULARIO PARA ADICIONAR FILHOS DOCONTRATO ***/
    $('.adicionar_contrato_item').off('click').on('click', function () {
        $.post(window.location.href, {controller: 'default', action: 'adicionar', id: $(this).attr('id')}, function (data) {
            $('#form_contrato_item').html(data);
            $('#dialogo_cadastrar_contrato_item').modal('show');
        });
    });

    /*** ABRE O FORMULARIO PARA EDITAR CONTRATO ***/
    $('.editarContrato').off('click').on('click', function () {
        $.post(window.location.href, {controller: 'default', action: 'editar', id: $(this).attr('id')}, function (data) {
            $('#form_contrato').html(data);
            $('#dialogo_cadastrar_contrato').modal('show');
        });
    });

    /*** ABRE O FORMULARIO PARA EDITAR FILHOS DO CONTRATO ***/
    $('.editar_item').off('click').on('click', function () {
        $.post(window.location.href, {controller: 'default', action: 'editar', id: $(this).attr('id')}, function (data) {
            $('#form_contrato_item').html(data);
            $('#dialogo_cadastrar_contrato_item').modal('show');
        });
    });

    /*** EXCLUIR REGISTRO ***/
    $('.removerContrato').off('click').on('click', function () {
        $.deleteItem({controller: 'default', action: 'excluir', retorno: true, text: 'Deseja realmente deletar este contrato?', id: $(this).attr('id'), functionSucsess: 'fecharModal'});
    });

    /*** GERA PLANILHA EXCEL ***/
    $('input#planilha').click(function () {
        var url = 'contratogestao.php?modulo=relatorio&acao=A&tipo=xls';
        window.open(url, 'relatorio', 'width=940,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1');
    });

    $("#arvore_contrato tbody").off('click').on("mousedown", "tr", function () {
        $(".success").not(this).removeClass("success");
        $(this).toggleClass("success");
    });

    /** BOTOES DE ESCONDER E EXIBIR TODOS */
    $('#esconder_todos').off('click').on('click', function () {
        $('#arvore_contrato').treetable('collapseAll');
    });

    $('#exibir_todos').off('click').on('click', function () {
        $('#arvore_contrato').treetable('expandAll');
    });

    /*** ORDENAR DA TABELA */
    var fixHelper = function (e, ui) {
        ui.children().each(function () {
            $(this).width($(this).width());
        });
        return ui;
    };

    $(".body2").sortable({
        helper: fixHelper,
        items: 'tr.tr_item',
        axis: "y",
        revert: true,
        cursor: "move",
        handle: '.item_ordernar',
        delay: 150,
        connectWith: ".body2",
        update: function (event, ui) {
            updateItem(ui, $(this))
        }
    }).disableSelection();

    /***ABRIR TELA DE FATOR AVALIADO */
    $('.fator_avaliado').off('click').on('click', function (e) {
        e.preventDefault();
        $.post(window.location.href, {controller: 'fatorAvaliado', action: 'fatorAvaliado', id: $(this).attr('id')}, function (data) {
            $('#fator_avaliado_conteudo').html(data);
            $('#tab_gestao_contrato a[href="#fator_avaliado_conteudo"]').tab('show');
        });
    });


});

function exibirNivel(icon, async) {
    var tr = icon.closest('tr');
    var trNext = tr.next('tr');
    var hqcid = tr.data("ttId");
    var proxNivel = tr.data("ttNivel") + 1;

    if (trNext.hasClass('tr_hqcid' + hqcid)) {
        if (trNext.is(":visible")) {
            icon.removeClass('glyphicon-chevron-up').addClass('glyphicon-chevron-down');
            trNext.hide();
        } else {
            icon.removeClass('glyphicon-chevron-down').addClass('glyphicon-chevron-up');
            trNext.show();
        }
    } else {
        icon.removeClass('glyphicon-chevron-down').addClass('glyphicon-chevron-up');

        $.ajax({
            type: 'POST',
            url: window.location.href,
            data: {controller: 'default', action: 'visalizarItem', hqcid: hqcid, level: proxNivel, },
            success: function (data) {
                if (data) {
                    tr.after('<tr class="tr_hqcid' + hqcid + ' tr_new"><td colspan="13">' + data + '</td></tr>');
                } else {
                    tr.after('<tr class="tr_hqcid' + hqcid + ' tr_new"><td colspan="13" class="text-center"> <div class="alert alert-warning" role="alert">Sem itens cadastrados</div> </td></tr>');
                }
            },
            async:async
        });
    }
}

function updateItem(ui, obj){
    var objetoMovido = ui.item;
    var idPaiAntigo = objetoMovido.data("ttParentId");
    var objetoDestino = ui.item.next();
    var possuiFilhos = ui.item.data("ttPossuiFilhos");;
    var itensDoPai = $("table").find('tr[data-tt-parent-id=' + objetoMovido.data("ttParentId") + ']');
    if (empty(objetoDestino.data('ttId'))) {
        objetoDestino = ui.item.prev();
    }

    if (!empty(objetoDestino.data('ttId'))) {

        if (validarMudanca(objetoDestino, objetoMovido) ) {
            if( !possuiFilhos ){
                console.log('moverContrato');
                moverContrato(objetoDestino, objetoMovido, idPaiAntigo)
                if ($('#arvore_contrato' + idPaiAntigo).find('td').length == 0) {
                    $('#arvore_contrato' + idPaiAntigo).closest('tr').remove();
                }
               obj.sortable("refreshPositions");
            }else{
                console.log('moverContrato cancel');
                obj.sortable('cancel');
                obj.sortable("refreshPositions");
                alert('Este item possui registro vinculados.')
            }

        } else {
            if (cancelarOrdenarNos(objetoMovido)) {
                console.log('cancelarOrdenarNos');
                obj.sortable('cancel');
                obj.sortable("refreshPositions");
            } else {
                console.log('ordenarNos');
                ordenarNos(objetoMovido, obj);
                atualizarOrdemHierarquiaContrato(itensDoPai)
            }
        }
    }
}

function abrirItemInteragido() {
    var hqcidpai_interagido = $('#hqcidpai_interagido').val();
    var path_hqcidpai = $('#path_hqcidpai').val();

    if(!empty(path_hqcidpai)){
        var lines = path_hqcidpai.split(",");
        console.log(lines)
        $.each(lines, function(indice, hqcid) {
            var icon = $('.exibirItem'+hqcid);
            exibirNivel( icon, false  )
        });
    }
}