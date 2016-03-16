<?php 
$controllerDefault = new Controller_Default();
$entity = $this->entity;

?>
<br />
<div class="well">
    <form id="formSavePostit" name="formSave" method="POST" class="form-horizontal">
        <fieldset>
            <input type="hidden" name="entid" id="entid" value="<?php echo $entity['entid']['value'] ?>" />
            <div class="form-group">
                <label for="prgid" class="col-lg-2 control-label">Projeto</label>
                <div class="col-lg-10">
                    <select name="prgid" id="prgid" class="form-control chosen-select" data-placeholder="Selecione">
                        <option value=""></option>
                        <?php foreach ($this->programs as $program): ?>
                            <option <?php if($entity['prgid']['value'] == $program['prgid']) echo 'selected="selected"' ?>value="<?php echo $program['prgid'] ?>"><?php echo $program['prgdsc'] ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>
            <div class="form-group" id="container_select_subprograma">
                <?php $_POST['prgid'] = $entity['prgid']['value']; $_POST['subprgid'] = $entity['subprgid']['value']; $controllerDefault->selectSubProgramAction() ?>
            </div>
            <div class="form-group has-warning" id="container_select_story">
                <?php $_POST['estid'] = $entity['estid']['value']; $controllerDefault->selectStoryAction() ?>
            </div>
            <div class="form-group " >
                <label for="usucpfsol" class="col-lg-2 control-label">Solicitante</label>
                <div class="col-lg-10">
                    <input type="text" name="" value="<?php echo $_SESSION['usunome'] ?>" class="form-control" disabled="disabled">
                    <input name="usucpfsol" id="usucpfsol" type="hidden" value="<?php echo $_SESSION['usucpf'] ?>" >
                </div>
            </div>
            <div class="form-group has-warning">
                <label for="entstid" class="col-lg-2 control-label">Status</label>
                <div class="col-lg-10">
                    <select name="entstid" id="entstid" class="form-control chosen-select" required="required" data-placeholder="Selecione">
                        <?php foreach ($this->status as $status): ?>
                            <option value="<?php echo $status['entstid'] ?>"><?php echo $status['entstdsc'] ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>
            <div class="form-group has-warning">
                <label for="enthrsexec" class="col-lg-2 control-label">Horas</label>
                <div class="col-lg-10">
                    <input name="enthrsexec" id="enthrsexec" class="form-control" type="number" value="<?php echo $entity['enthrsexec']['value'] ?>" required="required"/>
                    <span class="help-block"> Duração do entregável (em horas).</span>
                </div>
            </div>
            <div class="form-group has-warning">
                <label for="entdsc" class="col-lg-2 control-label">Descrição</label>
                <div class="col-lg-10">
                    <textarea name="entdsc" id="entdsc" class="form-control" rows="3"  required><?php echo $entity['entdsc']['value'] ?></textarea>
                    <span class="help-block"> Detalhamento da estória solicitada.</span>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <?php if($entity['entid']['value']) : ?>
                    <button class="btn btn-danger" id="buttonCancel" >Cancelar</button>
                    <button class="btn btn-info" id="buttonSave" >Atualizar</button>
                    <?php else: ?>
                    <button class="btn btn-warning" id="buttonClear" type="reset">Limpar</button> 
                    <button class="btn btn-success" id="buttonSave">Inserir</button>
                    <button class="btn btn-primary" id="buttonSearch">Buscar</button>
                    <?php endif ?>
                </div>
            </div>
        </fieldset>
    </form>
</div>
<script lang="javascript">
    $('#enthrsexec').mask('999');
    
    $('#formSavePostit #buttonCancel').click(function() {
        $.renderAjax({controller: 'postit', action: 'form', container: 'container_form_postit'});
        return false;
    });
    
    $('#formSavePostit #buttonClear').click(function() {
        $(this).parents('form:first').find('.has-error').removeClass('has-error');
        $(this).parents('form:first').clearForm();
//        return false;
    });
    
    $('#formSavePostit #buttonSave').click(function() {
        $(this).parents('form:first').saveAjax({controller: 'postit', action: 'save', functionSucsess: 'submitSucsess'});
        return false;
    });
    
    $('#formSavePostit #buttonSearch').click(function() {
        $(this).parents('form:first').find('.has-error').removeClass('has-error');
        search($(this).parents('form:first'));
        return false;
    });
    
    $("form #prgid").change(function() {
        
        var form = $(this).parents('form:first');
        
        $.post(window.location.href, {controller: 'default', action: 'selectSubProgram', prgid: $(this).val()}, function(html) {
            form.find('#container_select_subprograma').hide().html(html).fadeIn();
        });
    });
    
    function submitSucsess(element)
    {
        $.renderAjax({controller: 'postit', action: 'form', container: 'container_form_postit'});
        search(element);
    }
    
    
    function search(element)
    {
//        $.renderAjax({controller: 'program', action: 'form', container: 'container_form_program'});
        $(element).searchAjax({controller: 'postit', action: 'list', container: 'container_list_postit'});
    }
    
    setTimeout(function() {
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
    }, 300);
//</script>