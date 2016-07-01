<?php ?>
<br />
            <!--            <div class="row">
                            <div class="col-lg-12">
                                <div class="page-header">
                                    <h1 id="forms">Estória</h1>
                                </div>
                            </div>
                        </div>-->
                    <div class="well">
                        <form id="formSaveStory" name="formSave" method="POST" class="form-horizontal">
                            <fieldset>
    <!--                            <input type="hidden" name="controller" value="story"/>
                                <input type="hidden" name="action" value="save"/>-->
                                <input type="hidden" name="estid" id="estid" value="<?php echo isset($retorno['estid']) ? $retorno['estid'] : ''; ?>" />
                                <div class="form-group">
                                    <label for="inputEmail" class="col-lg-2 control-label" for="Programa">Projeto</label>
                                    <div class="col-lg-10">
                                        <select name="prgid" id="prgid" class="form-control" data-placeholder="Selecione">
                                            <option value="">Selecione</option>
                                            <?php foreach ($this->programs as $program): ?>
                                                <option value="<?php echo $program['prgid'] ?>"><?php echo $program['prgdsc'] ?></option>
                                            <?php endforeach; ?>
                                        </select>
                                    </div>
                                </div>
                                <div class="form-group has-warning" id="container_select_subprograma">
                                    <label for="inputEmail" class="col-lg-2 control-label">Sub-projeto</label>
                                    <div class="col-lg-10">
                                        <select name="subprgid" id="subprgid" class="form-control chosen-select" required="required" data-placeholder="Selecione">
                                            <option value=""></option>
                                        </select>
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="inputEmail" class="col-lg-2 control-label">Título</label>
                                    <div class="col-lg-10">
                                        <input name="esttitulo" id="esttitulo" class="form-control" type="text" value="" required="required"/>
                                        <span class="help-block">Digite uma descrição resumida da solicitação (Estória).</span>
                                    </div>
                                </div>
                                <div class="form-group has-warning">
                                    <label for="inputEmail" class="col-lg-2 control-label">Descrição</label>
                                    <div class="col-lg-10">
                                        <textarea class="form-control" rows="3" id="estdsc" name="estdsc" required><?php echo (isset($retorno['estdsc'])) ? $retorno['estdsc'] : null ?></textarea>
                                        <span class="help-block"> Detalhamento da estória solicitada.</span>
                                    </div>
                                </div>
                                <div class="form-group">
                                    <div class="col-lg-10 col-lg-offset-2">
                                        <button class="btn btn-danger" id="buttonCancel" style="display: none">Cancelar</button>
                                        <button class="btn btn-info" id="buttonEdit" style="display: none">Atualizar</button>
                                        <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button> 
                                        <button class="btn btn-success" id="buttonSave">Inserir</button>
                                        <!--<input class="botao" type="reset" value="Limpar" />-->
                                        <button class="btn btn-primary" id="buttonSearch">Buscar</button>
                                    </div>
                                </div>
                            </fieldset>
                        </form>
                    </div>
                <div id="container_list">
                </div>
<script lang="javascript">
//    $('#buttonSave').click(function() {
//        
//        form.find('.has-error').removeClass('has-error');
//        return false;
//    });
    
    
    search($(this).parents('form:first'));

    $('#buttonEdit').click(function() {
        var form = $(this).parents('form:first');
        form.find('.has-error').removeClass('has-error');
        
        $(this).parents('form:first').saveAjax({controller: 'story', action: 'save', functionSucsess: 'search'});
        
        $('#buttonClear').fadeIn();
        $('#buttonSave').fadeIn();
        $('#buttonSearch').fadeIn();
        $('#buttonCancel').hide();
        $('#buttonEdit').hide();
        
        return false;
    });
    
    $('#buttonCancel').click(function() {
        
        var form = $(this).parents('form:first');
        
        form.find('.has-error').removeClass('has-error');
        form.clearForm();
        
        form.find('#buttonClear').fadeIn();
        form.find('#buttonSave').fadeIn();
        form.find('#buttonSearch').fadeIn();
        form.find('#buttonCancel').hide();
        form.find('#buttonEdit').hide();
        
        return false;
    });

    $('#buttonSave').click(function() {
        $(this).parents('form:first').find('.has-error').removeClass('has-error');
        $(this).parents('form:first').saveAjax({controller: 'story', action: 'save', functionSucsess: 'search'});
        return false;
    });

    $('#buttonSearch').click(function() {
        $(this).parents('form:first').find('.has-error').removeClass('has-error');
        search($(this).parents('form:first'));
        return false;
    });
    
    $('#buttonClear').click(function() {
        $(this).parents('form:first').find('.has-error').removeClass('has-error');
        $(this).parents('form:first').clearForm();
        return false;
    });
    
    /**
     * Comment
     */
    function search(element)
    {
        $(element).searchAjax({controller: 'story', action: 'list'});
    }

    $("form #prgid").change(function() {
        $.post(window.location.href, {controller: 'story', action: 'selectSubProgram', prgid: $(this).val()}, function(html) {
            $('#container_select_subprograma').hide().html(html).fadeIn();
        });
    });

    setTimeout(function() {
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
    }, 200);
</script>