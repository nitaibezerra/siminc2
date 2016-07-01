<div class="row" id="container-bt-inserir">
    <div class="col-lg-12 text-center">
        <button class="bt-inserir btn btn-primary" >Inserir</button>
    </div>
</div>

<div class="row">
    <div class="col-md-6"  id="container-listar">
        <?php $this->listarAction(); ?>
    </div>
    <div class="col-md-6"  id="container-listarreitoria">
        <?php $this->listarreitoriaAction(); ?>
    </div>
</div>

<script language="JavaScript">
    $('.bt-inserir').click(function() {
        $('#container-formulario').fadeIn();
        $.post(window.location.href, {'controller': 'campus', 'action': 'formulario', 'entid': $(this).attr('sprintPostitId')}, function(html) {
            $('#modal').html(html).modal('show');
        });
    });

    function fecharModal()
    {
        $('#modal').modal('hide');
        var data = {controller: 'campus', action: 'listar'};
        $.post(window.location.href, data, function(html) {
            $('#container-listar').hide().fadeIn().html(html);
        });

        var data = {controller: 'campus', action: 'listarreitoria'};
        $.post(window.location.href, data, function(html) {
            $('#container-listarreitoria').hide().fadeIn().html(html);
        });
    }
</script>