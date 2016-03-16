function radioBtn(seletor){
    $('body').on('click', seletor, function () {
        var sel = $(this).data('title');
        var tog = $(this).data('toggle');
        console.log(sel);
        $('#' + tog).prop('value', sel);

        $('a[data-toggle="' + tog + '"]').not('[data-title="' + sel + '"]').removeClass('active').addClass('notActive');
        $('a[data-toggle="' + tog + '"][data-title="' + sel + '"]').removeClass('notActive').addClass('active');
    })
}

function atualializaGrupo(id){
    $.post(window.location.href, {controller: 'default', action: 'selecionarGrupo', id: id}, function (data) {
        $('#div_grupo').html(data);
    });
}

function atualializaListaGrupo(id){
    $.post(window.location.href, {controller: 'default', action: 'listagrupos', id: id}, function (data) {
        $('#div_lista_grupo').html(data);
    });
}

function retornoSucessoQuestoes(retorno) {
    var html = '';
    if (retorno.result) {
        $('.has-error').removeClass('has-error');

        var form = $(retorno.idform);
        form.find('.erro_input').remove();

        $(retorno['result']).each(function () {
            element = form.find('.' + this.name);

            label = form.find('label[for=' + this.name + ']').eq(0).text();
            if (label) {
                html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">Campo <strong>' + label + ':</strong> ' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
            } else {
                html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + this.msg + '.<a class="alert-link" href="#"></a></div></div>'
            }
            element.closest('.form-group').addClass('has-error');
        });
        if (html === '') {
            html += '<div class="col-lg-12"><div class="alert alert-dismissable alert-danger">' + result['msg'] + '</div></div>'
        }
        $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
        requestSent = false;
    }
}