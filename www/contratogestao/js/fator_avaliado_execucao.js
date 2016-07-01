$(document).ready(function() {
    /** TAB BOOTSTRAP*/
    $('#tab_gestao_contrato a[href="#conteudo_principal"]').not('.disabled').click(function(e) {
        e.preventDefault();
        $(this).tab('show');
    });

    $('#div_executor .btn_editar').on('click', function() {
        $.post(window.location.href, {controller: 'execucaoFatorAvaliado', action: 'formulario', idFatorAvaliado: $(this).data('id')}, function(data) {
            $('#container-form-fator-avaliado-execucao').html(data);
            $('#acao').val('execucao');
            regrasOpcoes('execucao');
        });
    });

    $('#div_validador table thead').removeClass('btn-primary').addClass('btn-warning');
    $('#div_validador .btn_editar').on('click', function() {
        $.post(window.location.href, {controller: 'execucaoFatorAvaliado', action: 'formulario', idFatorAvaliado: $(this).data('id')}, function(data) {
            $('#container-form-fator-avaliado-execucao').html(data);
            $('#acao').val('validacao');
            $('#div_arqid').hide();
            $('#div_downloadFile').show();
            $('#bt-recusar-fator-avaliado-execucao').show();
            regrasOpcoes('validacao');
        });
    });

    $('#div_certificador table thead').removeClass('btn-primary').addClass('btn-success');
    $('#div_certificador .btn_editar').on('click', function() {
        $.post(window.location.href, {controller: 'execucaoFatorAvaliado', action: 'formulario', idFatorAvaliado: $(this).data('id')}, function(data) {
            $('#container-form-fator-avaliado-execucao').html(data);
            $('#acao').val('certificacao');
            $('#div_arqid').hide();
            $('#div_downloadFile').show();
            $('#bt-recusar-fator-avaliado-execucao').show();
            $('#bt-recusar-fator-avaliado-validacao').show();
            regrasOpcoes('certificacao');
        });
    });
});

function regrasOpcoes(tipo) {
    if ( tipo !== 'validacao' && tipo !== 'certificacao') {
        $('.div_certificador_e_validador').remove();
    }
}