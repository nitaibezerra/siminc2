<div class="row">
    <div class="col-md-6"  id="container-listar">
        <?php $this->listarAction(); ?>
    </div>
    <div class="col-md-6"  id="container-listarreitoria">
        <?php $this->listarreitoriaAction(); ?>
    </div>
</div>
<script language="JavaScript">
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