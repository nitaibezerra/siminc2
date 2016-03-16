/**
 * Created by LucasGomes on 22/12/14.
 */
var quantidadeCampos = 0;
function desativarAnexo(id) {

    if (!ptInit.gestor) {
        bootbox.alert("O arquivo não pode ser apagado, favor entrar em contato com a CGSO/MEC");
        return false;
    }

    bootbox.confirm("Deseja realmente apagar o arquivo?", function(result){
        if (!result) return false;

        var url ="ted.php?modulo=principal/termoexecucaodescentralizada/parecer&acao=A&ted="+ptInit.ted+"&removerAnexo="+id;
        $.post(url,function(data){
            if (data == 1) {
                bootbox.alert('Anexo excluído com sucesso!');
                document.location.href= location.href;
            }
        });
    });
}

function downloadAnexo(id) {
    return window.open( 'ted.php?modulo=principal/termoexecucaodescentralizada/parecer&acao=A&ted='+ptInit.ted+'&download=s&arqid='+id,'blank','height=350,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' ).focus();
}

function fechar(id) {
    $('#campo-anexo'+id).remove().fadeOut();
    quantidadeCampos--;
    if (quantidadeCampos == 0) {
        $('#btn-salvar-anexo').fadeOut();
        $('#anexo-form').fadeOut();
    }
}

$(document).ready(function() {

    var inputTextLength = ["considentproponente", "considproposta", "considobjeto", "considobjetivo",
            "considjustificativa", "considvalores", "considcabiveis"]
        , counterText = 1000;

    $(inputTextLength).each(function(i, el) {
        $("#"+el).limit({
            limit: counterText,
            id_result: "counter-"+el,
            alertClass: "warning"
        });
    });

    $('#anexo-form').hide();
    $('#btn-salvar-anexo').hide();
    $('#novo-anexo').click(function() {
            if (quantidadeCampos == 0) {
                $('#anexo-form').fadeIn();
            }
            $('#anexo-form').append(
                '<section id="campo-anexo'+quantidadeCampos+'">'
                    +'<section class="form-group">'
                    +'	<div class="col-md-offset-11">'
                    +'		<button type="button" id="fechar'+quantidadeCampos+'" onclick="fechar('+quantidadeCampos+');" class="btn btn-warning btn-xs">'
                    +'           <span class=" glyphicon glyphicon-warning-sign"></span> Cancelar</button>'
                    +'	</div>'
                    +'</section>'
                    +'<section class="form-group">'
                    +'	<label class="control-label col-md-2" for="anexo'+quantidadeCampos+'">Anexo:</label>'
                    +'	<div class="col-md-10">'
                    +'		<input type="hidden" name="anexoCod[]" value="'+quantidadeCampos+'"/>'
                    +'		<input type="file" required name="anexo_'+quantidadeCampos+'" id="anexo'+quantidadeCampos+'" />'
                    +'	</div>'
                    +'</section>'
                    +'<section class="form-group">'
                    +'	<label class="control-label col-md-2" for="descricao'+quantidadeCampos+'">Descrição:</label>'
                    +'	<div class="col-md-10">'
                    +'		<textarea class="form-control" cols="2" name="descricaoanexo[]" id="descricao'+quantidadeCampos+'" maxlength="255"></textarea>'
                    +'	</div>'
                    +'</section>'
                    +'</section>');
            quantidadeCampos++;
            $('#btn-salvar-anexo').fadeIn();
        }
    );

    $("#print").on("click", function(e){
        e.preventDefault();

        var urlBse = "ted.php?modulo=principal/termoexecucaodescentralizada/parecer&acao=A&ted="
          , $tcpid = $("#tcpid");

        if ($tcpid.val()) {
            return window.open(urlBse+$tcpid.val()+"&action=print", 'blank','height=350,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' ).focus();
            //location.href=urlBse+$tcpid.val()+"&action=print";
        }
    });
});