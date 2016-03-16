<?php
global $db;
$sqlPrograma = 'SELECT prg.prgid AS codigo, prg.prgdsc AS descricao FROM scrum.programa prg';
$sqlSprint = "SELECT sptid as codigo, (TO_CHAR(sptinicio, 'DD/MM/YYYY') || ' - ' || TO_CHAR(sptfim, 'DD/MM/YYYY'))  as descricao FROM scrum.sprint ORDER BY sptinicio , sptfim";

$prgid = $this->prgid;
$sptid = $this->sptid;





?>
<!--<div class="page-header">
    <h1 id="forms">SCRUM</h1>
</div>-->
<br />
<div class="well">
    <form id="form-search-sprint" name="form-search-sprint" method="post"  class="form-horizontal">
        <fieldset>
<!--            <div class="form-group">
                <legend>Definir Ciclo</legend>
            </div>-->
            <div class="form-group">
                <label for="inputEmail" class="col-lg-2 control-label" for="Programa">Selecione um projeto</label>
                <div class="col-lg-10">
                    <?php $db->monta_combo('prgid', $sqlPrograma, 'S', 'Selecione..', 'search', '', null, null, 'N', 'prgid', null, $prgid, null, 'class="chosen-select form-control" style="width=100%;"'); ?>
                </div>
            </div>
            <div class="form-group">
                <label for="inputEmail" class="col-lg-2 control-label" for="Programa">Selecione um ciclo</label>
                <div class="col-lg-10">
                    <?php $db->monta_combo('sptid', $sqlSprint, 'S', 'Selecione..', 'search', '', null, null, 'N', 'sptid', null, $sptid, null, 'class="chosen-select form-control" style="width=100%;"'); ?>
                </div>
                <!--<p class="help-block">Selecione um programa.</p>-->
            </div>
            <div class="form-group">
        </fieldset>
    </form>
    <fieldset id="listSprint"></fieldset>
</div>
<script lang="javascript">
    
    search();
    
    function search()
    {
        var dataForm = $('#form-search-sprint').serialize();
        var data = 'controller=sprint&action=list&' + dataForm;
        $.post(window.location.href, data, function(html) {
            $('#listSprint').hide().html(html).fadeIn();
        });
    }
    
    setTimeout(function(){
        for (var selector in config) {
            $(selector).chosen(config[selector]);
        }
    },200);
</script>