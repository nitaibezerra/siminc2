$(document).ready(function() {
    /***ABRIR TELA DE ETAPAS DE CONTROLE */
    $('#funcao').on('change', function(e) {
        $.post(window.location.href, {
            controller: 'fatorAvaliado',
            action: 'getPessoas',
            tipoPessoa: $('#form-etapa-controle #funcao').val(),
            etapa: $('#form-etapa-controle #etapa').val()
        }, function(data) {
            $('#divTipoPessoa').html(data);
        });
    });

    /*** SALVA O ETAPA CONTROLE***/
    $('#bt-salvar-etapa-controler').on('click', function() {
        
        if ($('#funcao').val() === 'fisica') {
            resp = true;
            if ($('#novoUsuario').val() === '1') {
                var resp = confirm("Este usu\u00e1rio n\u00e3o existe no sistema SIMEC, deseja cri\u00e1-lo?");
            }
            if (resp === true) {
                $('#form-etapa-controle').saveAjax({
                    controller: 'usuario',
                    action: 'salvarUsuarioFatorAvaliado',
                    retorno: true,
                    etapa: $('#form-etapa-controle #etapa').val(),
                    displayErrorsInput: true,
                    functionSucsess: 'fecharModalEtapaControle'}
                );
            } else {
                return false;
            }
        } else if ($('#funcao').val() === 'juridica') {

            $('#form-etapa-controle').saveAjax({
                controller: 'entidade',
                action: 'salvarEntidadeFatorAvaliado',
                retorno: true,
                etapa: $('#form-etapa-controle #etapa').val(),
                displayErrorsInput: true,
                functionSucsess: 'fecharModalEtapaControle'}
            );
        }
    });

});
