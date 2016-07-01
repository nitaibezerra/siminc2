<?php 

$entity = $this->entity;

global $db;
$sqlResponsaveis = "SELECT DISTINCT u.usucpf AS codigo, u.usunome AS descricao
  FROM seguranca.usuario AS u
    INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
    INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
    INNER JOIN demandas.celula cel ON cel.celid = ur.celid
  WHERE us.sisid = 44
    AND us.suscod = 'A'
    AND ur.rpustatus = 'A'
    and ur.pflcod in (238, 237)
    AND cel.celid = 2
  ORDER BY u.usunome";
$responsaveis = $db->carregar($sqlResponsaveis);
?>
<br />
<div class="well">
    <form id="formSaveProgram" name="formSave" method="POST" class="form-horizontal">
        <fieldset>
            <input type="hidden" name="prgid" id="prgid" value="<?php echo $entity['prgid']['value'] ?>" />
            <div class="form-group has-warning">
                <label for="inputEmail" class="col-lg-2 control-label">Nome</label>
                <div class="col-lg-10">
                    <input name="prgdsc" id="prgdsc" class="form-control" type="text" value="<?php echo $entity['prgdsc']['value'] ?>" required="required"/>
                    <!--<span class="help-block">Nome do Projeto.</span>-->
                </div>
            </div>
            <div class="form-group has-warning">
                <label for="inputEmail" class="col-lg-2 control-label">Duração da sprint (em horas)</label>
                <div class="col-lg-10">
                    <input name="prghrsprint" id="prghrsprint" class="form-control" type="number" value="<?php echo $entity['prghrsprint']['value'] ?>" required="required"/>
                </div>
            </div>
            <div class="form-group has-warning" id="container_select_subprograma">
                <label for="usucpf" class="col-lg-2 control-label">Equipe</label>
                <div class="col-lg-10">
                    <select name="usucpf[]" id="usucpf" multiple class="form-control chosen-select-no-single" required="required" data-placeholder="Selecione">
                        <option value=""></option>
                        <?php foreach ($responsaveis as $responsavel): ?>
                        <option <?php if(in_array($responsavel['codigo'], $this->responsibles)) echo 'selected="selected"' ?> value="<?php echo $responsavel['codigo'] ?>"><?php echo $responsavel['descricao'] ?></option>
                        <?php endforeach; ?>
                    </select>
                </div>
            </div>
            <div class="form-group">
                <div class="col-lg-10 col-lg-offset-2">
                    <?php if($entity['prgid']['value']) : ?>
                    <button class="btn btn-danger" id="buttonCancel" >Cancelar</button>
                    <button class="btn btn-info" id="buttonSave" >Atualizar</button>
                    <?php else: ?>
                    <button class="btn btn-warning" id="buttonCancel" type="reset">Limpar</button> 
                    <button class="btn btn-success" id="buttonSave">Inserir</button>
                    <button class="btn btn-primary" id="buttonSearch">Buscar</button>
                    <?php endif ?>
                </div>
            </div>
        </fieldset>
    </form>
</div>
<script lang="javascript">
    $('#prghrsprint').mask('999');
    
    $('#formSaveProgram #buttonCancel').click(function() {
        $.renderAjax({controller: 'program', action: 'form', container: 'container_form_program'});
        return false;
    });
    
    $('#formSaveProgram #buttonSave').click(function() {
        $(this).parents('form:first').saveAjax({controller: 'program', action: 'save', functionSucsess: 'saveSucsess'});
        return false;
    });
    
    $('#formSaveProgram #buttonSearch').click(function() {
        $(this).parents('form:first').find('.has-error').removeClass('has-error');
        search($(this).parents('form:first'));
        return false;
    });
    
    function saveSucsess(element)
    {
        $.renderAjax({controller: 'program', action: 'form', container: 'container_form_program'});
        search(element);
    }
    
    
    function search(element)
    {
//        $.renderAjax({controller: 'program', action: 'form', container: 'container_form_program'});
        $(element).searchAjax({controller: 'program', action: 'list', container: 'container_list_program'});
    }
    
    setTimeout(function() {
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
        
        
    }, 200);
//</script>