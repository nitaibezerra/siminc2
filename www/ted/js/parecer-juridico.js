function desativarAnexo(id) {
    bootbox.confirm("Deseja realmente apagar o arquivo?", function(result){
        if (!result) return false;

        var url = settings.endpoint+"&ted="+settings.ted+"&removerAnexo="+id;
        $.post(url,function(data){
            if (data == 1) {
                bootbox.alert('Anexo excluído com sucesso!');
                document.location.href= location.href;
            }
        });
    });
}

function downloadAnexo(id) {
    return window.open(settings.endpoint+"&ted="+settings.ted+"&download=s&arqid="+id,"blank", "height=350,width=500,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes").focus();
}

function fechar(id) {
    $('#campo-anexo'+id).remove().fadeOut();
    quantidadeCampos--;
    if (quantidadeCampos == 0) {
        $('#btn-salvar-anexo').fadeOut();
        $('#anexo-form').fadeOut();
    }
}

$(function(){
    $("#obsparecer").limit({
        limit: 1000,
        id_result: "counter-obsparecer",
        alertClass: "warning"
    });

    var quantidadeCampos = 0;
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
});