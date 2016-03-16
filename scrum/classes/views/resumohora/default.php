<?php
global $db;
$sqlPrograma = 'SELECT prg.prgid AS codigo, prg.prgdsc AS descricao FROM scrum.programa prg';
$prgid = $this->prgid;
?>
<!--<div class="page-header">
    <h1 id="forms">SCRUM</h1>
</div>-->
<br />
<div class="well">
    <form id="form-search" name="form-search" method="post"  class="form-horizontal">
        <fieldset>
<!--            <div class="form-group">
                <legend>Projetos Ágeis (SCRUM)</legend>
            </div>-->
            <div class="form-group">
                <label for="inputEmail" class="col-lg-2 control-label" for="Programa">Selecione um projeto</label>
                <div class="col-lg-10">
                    <?php $db->monta_combo('prgid', $sqlPrograma, 'S', 'Selecione..', 'search', '', null, null, 'N', 'prgid', null, $prgid, null, 'class="chosen-select form-control" style="width=100%;"'); ?>
                </div>
            </div>
        </fieldset>
    </form>
    <fieldset id="listKanban"></fieldset>
</div>
<script lang="javascript">
    search();
    
    function search()
    {
        var dataForm = $('#form-search').serialize();
        var data = 'controller=resumohora&action=list&' + dataForm;
        $.post(window.location.href, data, function(html) {
            $('#listKanban').hide().html(html).fadeIn();
        });
    }
    
    setTimeout(function(){
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
    },300);
</script>