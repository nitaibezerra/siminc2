/**
 * Created by LucasGomes on 09/12/14.
 */
$(function() {
    $("#collapseOne").collapse("show");

    $("#estuf").livequery("change", function(){
        $.post(location.href+"&estuf="+$("#estuf").val(), function(data){
            $("#div-muncod").html(data);
            $('#muncod').chosen();
        });
    });

    if ($("#estuf").val()) {

        var endpoint = location.href+"&estuf="+$("#estuf").val();
        if (rcoInit.muncod) {
            endpoint = endpoint+"&muncod="+rcoInit.muncod;
        }

        $.post(endpoint, function(data) {
            $("#div-muncod").html(data);
            setTimeout(function() {
                $(".chosen-container").attr("style", "width:100%;");
                $('#muncod').chosen();
            }, 1200);
        });
    }

    var counterText = 5000
      , inputsKey = ["recatividadesprevistas", "recmetaprevista", "recatividadesexecutadas", "recmetaexecutada",
            "recdificuldades", "recmetasadotadas", "reccomentarios"];

    $(inputsKey).each(function(i, el) {
        $("#"+el).limit({
            limit: counterText,
            id_result: "counter-"+el,
            alertClass: "warning"
        });
    });

    $("#reccnpj, #reccpfresponsavel, #recvlrrecebido, #recvlrutilizado, #recvlrdevolvido, #reccep, #rectelefone")
        .focus().blur();

    /**
     * Adiciona Nota de Crédito
     */
    $(".add-nc").on("click", function(e) {
        e.preventDefault();
        var str = $("#recnumnotacredito").val();
        if (/^\d{4}(NC|nc)\d{6}$/g.test(str)) {
            var $tpl = $('<tr>' +
                '<td class="info">' +
                '<div class="col-md-1">' +
                '<span id="" class="glyphicon glyphicon-remove remove-nc"></span>' +
                '</div>' +
                '<div class="col-md-8">' +
                '<input type="text" name="tmp_recnumnotacredito[]" value="" class="form-control">' +
                '</div>' +
                '</td>' +
                '</tr>');
            $tpl.find("[name='tmp_recnumnotacredito[]']").val(str);
            $("#div-nc").find(".table tbody").append($tpl);
            $("#recnumnotacredito").val("");
        } else {
            bootbox.alert("O valor da NC não é um formato válido. Exemplo: '2015NC123456'");
        }
    });

    /**
     * Adiciona o NC de devolução
     */
    $(".add-nc-dev").on("click", function(e) {
        e.preventDefault();
        var str = $("#recnumnotacredito_dev").val();
        if (/^\d{4}(NC|nc)\d{6}$/g.test(str)) {
            var $tpl = $('<tr>' +
                '<td class="info">' +
                '<div class="col-md-1">' +
                '<span id="" class="glyphicon glyphicon-remove remove-nc-dev"></span>' +
                '</div>' +
                '<div class="col-md-8">' +
                '<input type="text" name="tmp_recnumnotacredito_dev[]" value="" class="form-control">' +
                '</div>' +
                '</td>' +
                '</tr>');
            $tpl.find("[name='tmp_recnumnotacredito_dev[]']").val(str);
            $("#div-nc-devolucao").find(".table tbody").append($tpl);
            $("#recnumnotacredito_dev").val("");
        } else {
            bootbox.alert("O valor da NC não é um formato válido. Exemplo: '2015NC123456'");
        }
    });

    /**
     * Remove NC e NC de devolução
     */
    $(".remove-nc, .remove-nc-dev").livequery("click", function() {
        var $that = $(this);
        bootbox.confirm("Deseja realmente excluir a Nota de Crédito?", function(result){
            if (result)
                $that.parent().parent().parent().remove();
        });
    });

    /**
     * Ação para botão que submete o formulário de RCO
     */
    $("#enviar").livequery("click", function(e) {
        e.preventDefault();
        $("#rcoobjeto").submit();
    });

    //controle de caracteres para textarea do parecer do rco
    $("#tcpobsrelatorio").limit({
        limit: 5000,
        id_result: "counter-tcpobsrelatorio",
        alertClass: "warning"
    });

    $(".chosen-container").attr("style", "width: 100%;");

    $(".widget-date-control").datepicker({
        language: "pt-BR",
        autoclose: true,
        todayHighlight: true,
        format: "dd/mm/yyyy"
    }).on("change", function(e) {
        var strDate = $(e.currentTarget).val();
        if (!strDate.match(/^(\d{2})\/(\d{2})\/(\d{4})$/)) {
            bootbox.alert("O formato da data é inválido", function(){
                $(e.currentTarget).val("");
            });
        }
    });
});

function downloadAnexo(id) {
    return window.open('ted.php?modulo=principal/termoexecucaodescentralizada/relatoriocuprimentoobjeto&acao=A&ted='+rcoInit.ted+'&download=s&arqid='+id,'blank','height=350,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' ).focus();
}

function desativarAnexo(id) {

    if (!rcoInit.gestor) {
        bootbox.alert("O arquivo não pode ser apagado, favor entrar em contato com a CGSO/MEC");
        return false;
    }

    var url ="ted.php?modulo=principal/termoexecucaodescentralizada/relatoriocuprimentoobjeto&acao=A&ted="+rcoInit.ted+"&removerAnexo="+id;
    bootbox.confirm("Deseja realmente apagar o arquivo?", function(result){

        if (!result) return false;

        $.post(url,function(data) {
            if (data == 1) {
                bootbox.alert('Anexo excluído com sucesso!');
                document.location.href= location.href;
            }
        });
    });
}
