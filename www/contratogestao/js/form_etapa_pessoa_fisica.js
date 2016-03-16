$(document).ready(function () {
    $(".data").mask("99/99/9999");
    $("#usucpf").mask("999.999.999-99");
    $("#usufoneddd").mask("999");
    $("#usufonenum").mask("9999-9999");

    $(".data").datepicker({
        defaultDate: "-25y +1w",
        changeMonth: true,
        changeYear: true,
        numberOfMonths: 1,
        showAnim: 'fadeIn'
    });

    $('#usucpf').on('blur', function (e) {
        getCpf($(this).val());
    });

    $('#pessoa').on('change', function (e) {
        getCpf($(this).val());
    });

    $('#regcod').on('change', function (e) {
        $.post(window.location.href, {
            controller: 'usuario',
            action: 'getMunicipios',
            regcod: $(this).val()
        }, function (data) {
            $('#muncod').html(data);
            $('#div_tpocod, #div_entidade').show();
        });
    });

    $('#tpocod').on('change', function (e) {

        valueSelected = $(this).val();
        if (valueSelected == 4) {
            $('#orgao').show();
            $('#entid').hide();
        } else {
            $('#orgao').hide();
            $('#entid').show();

            $.post(window.location.href, {
                controller: 'usuario',
                action: 'getOrgaos',
                tpocod: $('#tpocod').val(),
                regcod: $('#regcod').val(),
                muncod: $('#muncod').val()
            }, function (data) {
                $('#entid').html(data);
            });
        }


    });
});