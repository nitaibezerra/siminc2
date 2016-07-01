$(document).ready(function() {
    /*** DATAS ***/
    $("#fatprazo").mask("99/99/9999");
    $('#fatvalordesembolso').mask('000.000.000.000.000,00', {reverse: true});
    
    $("#fatprazo").datepicker({
        defaultDate: "+1w",
        changeMonth: true,
        numberOfMonths: 1,
        showAnim: 'fadeIn'
    });

    /*** SALVA O FORMULARIO ***/
    $('#bt-salvar-fator-avaliado').on('click', function() {
        $('#form-fator-avaliado').saveAjax({controller: 'fatorAvaliado', action: 'salvar', retorno: true, displayErrorsInput: true, functionSucsess: 'fatorAvaliado'});
    });

    /*** ABRE O FORMULARIO ETAPAS DE CONTROLE ***/
    $('.bt_etapas_de_controle').on('click', function() {
        $.post(window.location.href, {controller: 'fatorAvaliado', action: 'formEtapaControle', etapa: $(this).data('etapa') }, function(data) {
            $('#form_etapas_de_controle').html(data);
            $('#dialogo_etapas_de_controle').modal('show');
        });
    });

    /***ABRIR TELA DE ETAPAS DE CONTROLE / MOSTRAR BOTOES*/
    $('#etapas_de_controle_execucao').on('change', function(e) {
        if ($(this).is(':checked')) {
            $('#etapas_de_controle_validacao').closest('label').removeClass('disabled');
            $('#' + $(this).data('etapa')).show();
        } else {
            $('#etapas_de_controle_validacao').prop('checked', false).closest('label').addClass('disabled').removeClass('active');
            $('#etapas_de_controle_certificacao').prop('checked', false).closest('label').addClass('disabled').removeClass('active');
            $('#' + $(this).data('etapa')).hide();
            $('#certificador').hide();
            $('#validador').hide();
        }
    });

    $('#etapas_de_controle_validacao').on('change', function(e) {
        if ($(this).is(':checked')) {
            $('#etapas_de_controle_certificacao').closest('label').removeClass('disabled');
            $('#' + $(this).data('etapa')).show();
        } else {
            $('#etapas_de_controle_certificacao').prop('checked', false).closest('label').addClass('disabled').removeClass('active');
            $('#' + $(this).data('etapa')).hide();
            $('#certificador').hide();
        }
    });

    $('#etapas_de_controle_certificacao').on('change', function(e) {
        if ($(this).is(':checked')) {
            $('#' + $(this).data('etapa')).show();
        } else {
            $('#' + $(this).data('etapa')).hide();
        }
    });

    /*** EXCLUIR PESSOA SELECIONADA DO FATOR AVALIADO***/
    $('.bt_remover_pessoa').on('click', function() {
        var etapa = $(this).data('etapa');
        $('#usucpf' + etapa).val('');
        $('#entid' + etapa).val('');

        $('#nome_' + etapa).html('');
        $('#span_nome_' + etapa).hide();
    });

    /*** RESETAR FORMULARIO FATOR AVALIADO ************/
    $('#bt_reset_fator_avaliado').on('click', function() {
        $('#executor, #validador, #certificador').hide();
        $('#nome_executor, #nome_validador, #nome_certificador').html('');
        $('#etapas_de_controle_execucao, #etapas_de_controle_validacao, #etapas_de_controle_certificacao').closest('label').removeClass('active');
        $("#form-fator-avaliado input")
                .val('')
                .removeAttr('checked')
                .removeAttr('selected');
    });
    
});
