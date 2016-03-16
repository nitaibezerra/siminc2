<div class="row">
    <div class="col-md-12">
        <div id="container_save">
            <form id="form_save" method="post" class="form-horizontal" enctype="multipart/form-data">
                <div class="col-md-12">
                    <div class="well">
                        <input name="controller" type="hidden" value="documentoarquivo">
                        <input name="action" type="hidden" value="salvar">
                        <input name="dmaid" type="hidden" value="<?php echo $this->entity['dmaid']['value'] ?>">
                        <input id="dmdid" name="dmdid" type="hidden" value="<?php echo $this->entity['dmdid']['value'] ?>">
                        <?php if(!$this->entity['dmaid']['value']): ?>
                            <div class="form-group" id="div_arqid">
                                <label for="arqid" class="col-lg-4 control-label">Arquivo</label>
                                <div class="col-lg-5">
                                    <input id="arqid" name="arqid" type="file"  required="required">
                                </div>
                            </div>
                        <?php endif ?>
                        <div class="form-group">
                            <label for="dmadsc" class="col-lg-4 col-md-4 control-label" >Descrição:</label>

                            <div class="col-lg-8 col-md-8 ">
                                <input id="dmadsc" name="dmadsc" type="text" class="form-control" placeholder=""
                                       value="<?php echo $this->entity['dmadsc']['value'] ?>" required="required">
                            </div>
                        </div>
                        <div class="text-right">
                            <?php if($this->entity['dmaid']['value']): ?>
                                <button id="bt-cancelarArquivo" type="button" class="btn btn-warning">Cancelar</button>
                            <?php endif ?>
                            <button id="bt-salvarArquivo" type="button" class="btn btn-success">Salvar</button>
                        </div>
                    </div>
                </div>
            </form>
        </div>

        <div class="clearfix"></div>

        <!--        --><?php //$modelDemanda->recuperarListagem(); ?>
    </div>
</div>
<script type="text/javascript">
    $('#bt-salvarArquivo').click(function () {


        $('#form_save').isValid(function(isValid){
            if(isValid){
//                $('#form_save').saveAjax({clearForm: true, functionSucsess: 'listarArquivos'});
                $('#form_save').ajaxSubmit(function(){
                    carregarFormulario();
                    listarArquivos();
                    html = '<div class="col-lg-12"><div class="alert alert-dismissable alert-success"><strong>Sucesso! </strong>Os dados foram salvos!<a class="alert-link" href="#"></a></div></div>';
                    $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').html(html);
                });
            }
        });
    });

    $('#bt-cancelarArquivo').click(function(){
        carregarFormulario();
    });

    function carregarFormulario()
    {
        dmdid = $('#form_save #dmdid').val();

        $.post(window.location.href, {controller: 'documentoarquivo' , action: 'formulario' , id: dmdid}, function (html) {
            $('#container_save').hide().fadeIn().html(html);
        });
    }
</script>