<?PHP
    $tipo_acao = $this->tipoAcao;
    $ordem = $this->ordem;
?>

<div class="modal-dialog" style=" width: 65%;">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
            <h5 class="modal-title ">Novo Indicador</h5>
        </div>
        <form class="form-horizontal" method="post" name="form_save" id="form_save">
            <input name="controller" type="hidden" value="guia">
            <input name="action" type="hidden" value="salvarIndicador">
            <input name="tipo_acao" id="tipo_acao" type="hidden" value="<?=$tipo_acao?>">
            <input name="areid" id="areid" type="hidden" value="<?=$this->entityArea['areid']['value'];?>">
            <input name="indid" id="indid" type="hidden" value="<?=$this->entity['indid']['value'];?>">            
            
            <div class="modal-body">
                <div class="row">
                    <div class="col-md-12">
                        <div class="well">
                            <fieldset>
                                <legend>Área</legend>
                                
                                <div class="form-group has-warning">
                                    <label for="aredsc" class="col-lg-2 control-label">Descrição</label>
                                    <div class="col-lg-10">
                                        <textarea id="aredsc" name="aredsc" disabled maxlength="500" class="form-control" cols="10" placeholder="" required="required" value=""><?php echo $this->entityArea['aredsc']['value'] ?></textarea>
                                    </div>
                                </div>
                                
                            </fieldset>
                        </div>
                    </div>
                </div>
            </div>

            <div class="modal-body">
                <div class="row">
                    <div class="col-md-12">
                        <div class="well">
                            <fieldset>
                                <legend>Indicador</legend>

                                <div class="form-group has-warning">
                                    <label for="inddsc" class="col-lg-2 control-label">Descrição</label>
                                    <div class="col-lg-10">
                                        <textarea id="inddsc" name="inddsc" maxlength="500" class="form-control" cols="10" placeholder="" required="required" value=""><?php echo $this->entity['inddsc']['value']; ?></textarea>
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="indcod" class="col-lg-2 control-label">Ordem</label>
                                    <div class="col-lg-10">
                                        <input id="indcod" name="indcod" type="text" maxlength="3" class="form-control" placeholder="" required="required" value="<?php echo $ordem; ?>">
                                    </div>
                                </div>

                            </fieldset>
                        </div>
                    </div>
                </div>
            </div>
        </form>
        <div class="modal-footer">
            <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            <button id="bt-salvar" type="button" class="btn btn-success">Salvar</button>
        </div>
    </div><!-- /.modal-content -->
</div><!-- /.modal-dialog -->


<script type="text/javascript">
    $('#indcod').mask('999');

    $('#bt-salvar').click(function () {
        var tipo_acao = $('#tipo_acao').val();

        if( tipo_acao == 'up' ){
            $('#form_save').saveAjax({clearForm: false, functionSucsess: 'fecharModal' });
            return false;
        }else{
            $('#form_save').saveAjax({clearForm: false, functionSucsess: 'carregarArvore' });
            return false;
        }

    });
</script>