$(document).ready(function() {
    /*** DATAS ***/
    $(".data").mask("99/99/9999");

    $("#datainicial, .data_inicio").datepicker({
        defaultDate: "+1w",
        changeMonth: true,
        numberOfMonths: 2,
        onClose: function(selectedDate) {
            $("#datafinal").datepicker("option", "minDate", selectedDate);
        }
        , 'showAnim': 'fadeIn'
    });
    $("#datafinal, .data_fim").datepicker({
        defaultDate: "+1w",
        changeMonth: true,
        numberOfMonths: 2,
        onClose: function(selectedDate) {
            $("#datainicial").datepicker("option", "maxDate", selectedDate);
        }
        , 'showAnim': 'fadeIn'
    });

    /*** SALVA O FORMULARIO CONTRATO***/
    $('#bt-salvar-contrato').on('click', function() {
        $('#form-gestao-contrato').saveAjax({action: 'salvar', controller: 'default', retorno:true, displayErrorsInput: true, functionSucsess: 'fecharModal'});
    });

    /*** SALVA O FORMULARIO ITEM ***/
    $('#bt-salvar-contrato-item').on('click', function() {
        $('#form-gestao-contrato-item').saveAjax({action: 'salvar', controller: 'default', retorno:true, displayErrorsInput: true, functionSucsess: 'fecharModal'});
    });
});
