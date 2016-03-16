<script type="text/javascript">
$(document).ready(function(){
    $('#periodo').on('change',function(){
       if($('#periodo').val() != ''){
           window.location.href = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A&tipo='+$('input[name="tp"]').val()+'&periodo='+$('#periodo').val();
       }
    });
    $('.salvar-usuario-responsabilidade').on('click', function(e) {
        e.preventDefault();
        salvarUsuarioResponsabilidade($(this).data('form'));
    });
});

function salvarUsuarioResponsabilidade(form) {
$.ajax({
        type: "POST",
        url: window.location.href,
        data: $('#' + form).serialize(),
        async: false,
        success: function(resp) {
            console.log(resp);
            //window.location.reload();
        }
    });
}

function alterarResponsavel(unicod, prfid, rpuid, usucpf){
    var url = 'acomporc.php?modulo=principal/acompanhamento/acao&acao=A',
    params = { requisicao: 'carregarUsuarios', unicod: unicod, rpuid: rpuid, prfid: prfid, usucpf: usucpf};
    $.post(url, params, function(response){
        $('#modal-confirm .modal-body').html(response);
        $('.modal-dialog').css('width', '60%');
        $('#modal-confirm .modal-title').html('Alteração de Responsável: Monitor Interno');
        $('#modal-confirm .btn-primary').attr('id','salvar-responsavel').html('<span class="glyphicon glyphicon-ok"></span> Salvar');
        $('#modal-confirm .btn-default').html('Cancelar');
        $('.modal-dialog').show();
        $('#modal-confirm').modal();
    });
}

function enviarAcaoParaSiop(sslid, acacod, unicod)
{
    $.post(window.location.href, {requisicao:'resumoLocalizadores', unicod:unicod, acacod:acacod}, function(response){
        if (response.podeEnviar) {
            bootbox.confirm(response.html, function(ok){
                if (!ok) {
                    return;
                }
                // -- Enviando a ação selecionada para o SIOP
                var $form = $('<form />', {method:'POST'});
                $('<input/>', {type:'hidden',name:'requisicao',value:'enviarSiop'}).appendTo($form);
                $('<input/>', {type:'hidden',name:'dados[acacod]',value:response.acacod}).appendTo($form);
                $('<input/>', {type:'hidden',name:'dados[unicod]',value:response.unicod}).appendTo($form);
                $form.appendTo('body').submit();
            });
        } else {
            bootbox.alert(response.html);
        }
    }, 'json');
}
</script>
<link rel="stylesheet" href="/includes/spo.css">
<section class="well">
    <form class="form-horizontal" method="get" action="">
        <input type="hidden" name="tp" value="<?=$tipo?>">
        <div class="form-group">
            <label for="periodo" class="col-lg-2 control-label">Período de Referência:</label>
            <div class="col-lg-10">
                <?php
                inputCombo('prfid', sprintf(retornaQueryComboPeriodo(),$_SESSION['exercicio'],$tipo), $prfid, 'periodo',array('mantemSelecaoParaUm' => false));
                ?>
            </div>
        </div>
        <div class="form-group">
            <div class="col-lg-2"></div>
            <div class="col-lg-10"></div>
        </div>
    </form>
</section>
<?php
require(dirname(__FILE__) . "/listarAcao.inc");
