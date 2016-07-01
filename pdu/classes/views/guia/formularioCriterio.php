<?PHP
    $tipo_acao = $this->tipoAcao;
?>

<div class="modal-dialog" style=" width: 65%;">
    <div class="modal-content">
        <div class="modal-header">
            <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
            <h5 class="modal-title ">Novo Critério</h5>
        </div>
        <form class="form-horizontal" method="post" name="form_save" id="form_save">
            <input name="controller" type="hidden" value="guia">
            <input name="action" type="hidden" value="salvarCriterio">
            <input name="tipo_acao" id="tipo_acao" type="hidden" value="<?=$tipo_acao?>">
            <input name="indid" id="indid" type="hidden" value="<?=$this->entityIndicador['indid']['value'];?>">
            <input name="crtid" id="crtid" type="hidden" value="<?=$this->entity['crtid']['value'];?>">

            <div class="modal-body">
                <div class="row">
                    <div class="col-md-12">
                        <div class="well">
                            <fieldset>
                                <legend>Indicador</legend>
                                
                                <div class="form-group has-warning">
                                    <label for="inddsc" class="col-lg-2 control-label">Descrição</label>
                                    <div class="col-lg-10">
                                        <textarea id="inddsc" name="inddsc" disabled maxlength="500" class="form-control" cols="10" placeholder="" required="required" value=""><?php echo $this->entityIndicador['inddsc']['value'] ?></textarea>
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
                                <legend>Critério</legend>
                                <div class="form-group has-warning">
                                    <label for="crtpontuacao" class="col-lg-2 control-label">Pontuação</label>
                                    <div class="col-lg-4">
                                        <input id="crtpontuacao" name="crtpontuacao" type="text" maxlength="3" class="form-control" placeholder="" required="required" value="<?php echo $this->entity['crtpontuacao']['value'] ?>">
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="crtdsc" class="col-lg-2 control-label">Descrição</label>
                                    <div class="col-lg-10">
                                        <textarea id="crtdsc" name="crtdsc" maxlength="500" class="form-control" cols="10" placeholder="" required="required" value=""><?php echo $this->entity['crtdsc']['value'] ?></textarea>
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="crtpeso" class="col-lg-2 control-label">Peso</label>
                                    <div class="col-lg-4">
                                        <input id="crtpeso" name="crtpeso" type="text" maxlength="3" class="form-control" placeholder="" required="required" value="<?php echo $this->entity['crtpeso']['value'] ?>">
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
    $('#crtpontuacao').mask('999');
    $('#crtpeso').mask('999');

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