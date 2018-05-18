$(document).ready(function(){

    toggleItem();
    recuperarValoresLimitesPtres();
    recuperarValoresLimitesSubUnidade();
    // recuperarMetasEIniciativaPPA();

});

$('#eqdid').change(function(){
    $('#span-item').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-item&eqdid=' + $(this).val(), function(){
        toggleItem();
    });
});

$('#eqdid, #suoid').change(function(){
    $('#span-funcional').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-funcional&eqdid=' + $('#eqdid').val() + '&suoid=' + $('#suoid').val());
});

$('#suoid').change(function(){
    $('#span-metapnc').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-metapnc&suoid=' + $('#suoid').val());
    recuperarValoresLimitesSubUnidade();
})

$('#mdeid').change(function(){
    $('#span-segmento').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-segmento&mdeid=' + $(this).val());
});

$('body').on('change', '#maiid', function(){
    $('#span-subitem').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-subitem&maiid=' + $(this).val());
});

$('body').on('change', '#mpnid', function(){
    $('#span-indicadorpnc').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-indicadorpnc&mpnid=' + $(this).val());
});

$('#oppid').change(function(){
    $('#span-metappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-metappa&oppid=' + $('#oppid').val() + '&suoid=' + $('#suoid').val());
    $('#span-iniciativappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-iniciativappa&oppid=' + $('#oppid').val());
});

$('.valorPI').keyup(function(){
    calcularValores();
});

$('.valorPI').change(function(){
    calcularValores();
});

$('body').on('change', '#ptrid', function(){
    $.ajax({
        url: '?modulo=principal/preplanointerno_form&acao=A&req=recuperar-objetivoppa&ptrid=' + $(this).val(),
        success: function(oppid){
            if(oppid){
                $('#oppid').val(oppid).prop('readonly', 'readonly').trigger("chosen:updated").change();
            } else {
                $('#oppid').val('').prop('readonly', false).trigger("chosen:updated").change();
            }
        }
    });
    recuperarValoresLimitesPtres();
});

$('#esfid').change(function(){
    $('.select-localizacao').hide('slow');
    $('#div-localizacao_' + $('#esfid').val()).show('slow');
}).change();

$('#btn-salvar').click(function(){

    valorDisponivel = $('#td_disponivel_sub_unidade').html() ? str_replace(['.', ','], ['', '.'], $('#td_disponivel_sub_unidade').html()) : 0;
    if(valorDisponivel < 0){
        swal('Atenção', 'O Limite Disponível na Unidade foi ultrapassado. Favor rever valores preenchidos no Custeio e Capital', 'error');
        return false;
    }

    $('#formulario').submit();
});


function calcularValores(){

    // Calculando valor Disponível
    totalPi = somarCampos('valorPI');
    limiteDisponivel = $('#td_autorizado_sub_unidade').html() ? str_replace(['.', ','], ['', '.'], $('#td_autorizado_sub_unidade').html()) : 0;

    valorDisponivel = parseFloat(limiteDisponivel) - parseFloat(totalPi);

    if(valorDisponivel < 0){
        swal('Atenção', 'O Limite Disponível na Unidade foi ultrapassado. Favor rever valores preenchidos no Custeio e Capital', 'error');
    }

    $('#td_disponivel_sub_unidade').html(number_format(valorDisponivel, 2, ',', '.'));
}


function toggleItem(){
    if($('#maiid option').size() > 1){
        $('#span-item, #span-subitem').show();
    } else {
        $('#span-item, #span-subitem').hide();
    }
}

function recuperarValoresLimitesSubUnidade(){
    $.ajax({
        url: '?modulo=principal/preplanointerno_form&acao=A&req=recuperar-limite&suoid=' + $('#suoid').val(),
        dataType: 'json',
        success: function(dados){
            $('#td_autorizado_sub_unidade').html(number_format(parseFloat(dados.lmuvlr), 2, ',', '.'));
            $('#td_disponivel_sub_unidade').html(number_format(parseFloat(dados.disponivelunidade), 2, ',', '.'));
        }
    });
}

function recuperarValoresLimitesPtres(){
    $.ajax({
        url: '?modulo=principal/preplanointerno_form&acao=A&req=recuperar-valores-ptres&ptrid=' + $('#ptrid').val(),
        dataType: 'json',
        success: function(dados){
            $('#td_disponivel_funcional_custeio').html(number_format(parseFloat(dados.custeioptres), 2, ',', '.'));
            $('#td_disponivel_funcional_capital').html(number_format(parseFloat(dados.capitalptres), 2, ',', '.'));
        }
    });
}

$('#importar-pi-btn').on('click', function () {
    var modal = $('#preplanointerno_modal');
    modal.modal();
    modal.find('.modal-body').load('proposta.php?modulo=principal/preplanointerno_form&acao=A&req=proposta_modal', function () {
        $('#preplanointerno_modal').find('table').DataTable({
            responsive: true,
            dom: '<"html5buttons"B>lTfgitp',
            "language": {
                "url": "/zimec/public/temas/simec/js/plugins/dataTables/Portuguese-Brasil.json"
            }
        });
    });

    modal.on('hide.bs.modal', function () {
        modal.find('.modal-body').html('');
    });
});

$(document).on('click', 'a.pi-importer', function () {
    var piID = $(this).data('pi-id');
    importarPIDeAnosAnteriores( piID );
});

function importarPIDeAnosAnteriores(piID) {
    $('#preplanointerno_modal').modal('hide');
    console.log('Foobar', piID);

    $.ajax({
        url: '?modulo=principal/preplanointerno_form&acao=A&req=importar-pi&pliid=' + piID,
        success: function(oppid){
            /*if(oppid){
                $('#oppid').val(oppid).prop('readonly', 'readonly').trigger("chosen:updated").change();
            } else {
                $('#oppid').val('').prop('readonly', false).trigger("chosen:updated").change();
            }*/
        }
    });
    recuperarValoresLimitesPtres();
}

function recuperarMetasEIniciativaPPA() {
    $('#span-metappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-metappa&oppid=' + $('#oppid').val() + '&suoid=' + $('#suoid').val());
    $('#span-iniciativappa').load('?modulo=principal/preplanointerno_form&acao=A&req=carregar-iniciativappa&oppid=' + $('#oppid').val());
}