<div class="col-lg-12">
    <!--<div class="page-header">-->
    <!--    <h3 id="forms">-->
    <!--                Dados da universidade --->
    <!--        <small>-->
    <!--            Salvar dados da universidade-->
    <!--        </small>-->
    <!--    </h3>-->
    <!--</div>-->
    <div class="well bs-component">
        <form class="form-horizontal">
            <fieldset>
                <legend>Salvar Dirigentes</legend>
                <?php foreach($this->tiposDirigente as $tipoDirigente): ?>
                    <div class="form-group">
                        <label for="" class="col-lg-2 control-label"><?php echo $tipoDirigente['tpddsc'] ?></label>
                        <div class=" input-group col-lg-10">
                            <?php if($tipoDirigente['drgid']): ?>
                                <span class="input-group-addon">
                                    <a href="javascript:void(0);" onclick="javascript:formularioDirigente('<?php echo $tipoDirigente['tpdid'] ?>' , '<?php echo $tipoDirigente['drgid'] ?>')"><i class="glyphicon glyphicon-pencil"></i></a>
                                </span>
                                <span class="input-group-addon">
                                    <a href="javascript:void(0);" onclick="javascript:excluir( '<?php echo $tipoDirigente['tpdid'] ?>' )"><i class="glyphicon glyphicon-remove"></i></a>
                                </span>
                            <?php else: ?>
                                <span class="input-group-addon">
                                    <a href="javascript:void(0);" onclick="javascript:formularioDirigente('<?php echo $tipoDirigente['tpdid'] ?>' , '')"><i class="glyphicon glyphicon-plus"></i></a>
                                </span>
                            <?php endif; ?>
                            <input disabled="disabled" type="text" class="form-control" id="" placeholder="" value="<?php echo $tipoDirigente['drgnome'] ?>">
                        </div>
                    </div>
                <?php endforeach; ?>
            </fieldset>
        </form>
    </div>
</div>
<script language="javascript">
    function formularioDirigente(tpdid , drgid)
    {
        $.post(window.location.href, {'controller': 'dirigente', 'action': 'formulario', 'tpdid': tpdid , drgid: drgid}, function(html) {
            $('#modal').html(html).modal('show');
        });
    }

    function excluir(id)
    {
        $.deleteItem({controller: 'dirigente', action: 'deletar', text : 'Deseja realmente deletar este dirigente?', id: id, functionSucsess: 'fecharModal'});
    }

    function fecharModal()
    {
        $('#modal').modal('hide');
        var data = {controller: 'dirigente', action: 'index'};
        $.post(window.location.href, data, function(html) {
            $('#dirigente').hide().fadeIn().html(html);
        });
    }
</script>
