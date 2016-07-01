$(document).ready(function() {
    $("#entnumcpfcnpj").mask("99.999.999/9999-99");

    $('#entnumcpfcnpj').on('blur', function(e) {
        $.post(window.location.href, {controller: 'entidade', action: 'getEntidadeCnpj', entnumcpfcnpj: $('#entnumcpfcnpj').val() }, function(data) {
            if (data.length === 0) {
                objs = $("input[name^='ent'], #novoUsuario").not('#entnumcpfcnpj, :button, :submit, :reset, :hidden')
                        .val('')
                        .removeAttr('checked')
                        .removeAttr('selected');
            } else {
                $.each(data, function(index, value) {
                    $('#' + index).val(value);
                });
                $("#novoUsuario").val(data.novoUsuario);
            }
        }, 'json');
    });

    $('#pessoa').on('change', function(e) {
        $.post(window.location.href, {controller: 'entidade', action: 'getEntidadeCnpj', entnumcpfcnpj: $(this).val() }, function(data) {
            if (data.length === 0) {
                objs = $("input[name^='ent'], #novoUsuario").not('#entnumcpfcnpj, :button, :submit, :reset, :hidden')
                        .val('')
                        .removeAttr('checked')
                        .removeAttr('selected');
            } else {
                $.each(data, function(index, value) {
                    $('#' + index).val(value);
                });
                $("#novoUsuario").val(data.novoUsuario);
            }
        }, 'json');
    });
});