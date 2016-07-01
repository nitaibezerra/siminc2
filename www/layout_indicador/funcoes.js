$(document).ready(function () {
    var $tabs = $('#horizontalTab');
    $tabs.responsiveTabs({
        rotate: false,
        startCollapsed: 'accordion',
        collapsible: 'accordion',
        setHash: true,
        activate: function (e, tab) {
            $('.info').html('Tab <strong>' + tab.id + '</strong> activated!');
        },
        activateState: function (e, state) {
            //console.log(state);
            $('.info').html('Switched from <strong>' + state.oldState + '</strong> state to <strong>' + state.newState + '</strong> state!');
        }
    });

    $('#start-rotation').on('click', function () {
        $tabs.responsiveTabs('startRotation', 1000);
    });
    $('#stop-rotation').on('click', function () {
        $tabs.responsiveTabs('stopRotation');
    });
    $('#start-rotation').on('click', function () {
        $tabs.responsiveTabs('active');
    });

    $('.detalhe_linha').click(function(){
        var linha = $(this).attr('linha');
        var $span = $(this).children().children().children().eq(0);
        if ($span.hasClass('glyphicon-plus')) {
            $span.removeClass('glyphicon-plus').addClass('glyphicon-minus');
            $('#' + linha).show();
        } else {
            $span.removeClass('glyphicon-minus').addClass('glyphicon-plus');
            $('#' + linha).hide();
        }
    });

    $('.detalhe_regionalizacao').click(function(){
        var indid = $(this).attr('indid');
        $('#div_cruzamento').load('index.php?carregarCruzamento=1&indid=' + $(this).attr('indid'));
    });

    $('#div_cruzamento').on('click', '#limpar-cruzamento', function () {
        $('#div_cruzamento').load('index.php?limparCruzamento=1', function() {
        });
    });

    $('#div_cruzamento').on('click', '#btn-pesquisar', function () {
        options = {
            type: 'POST',
            success: function (result) {
                $('#div-resultado-cruzamento').html(result);
            }
        }
        $("#form_cruzamento").ajaxForm(options).submit();
    });

    $('.chosen').chosen();
});