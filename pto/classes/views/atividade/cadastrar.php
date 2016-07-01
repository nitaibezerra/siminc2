<div class="modal fade" id="dialogo_formulario_executor">
    <div class="modal-dialog-large">
        <div class="modal-content" id="formulario_executor"></div>
    </div>
</div>

<div class="row">
    <input type="hidden"  name="tituloSolucao_atividade" id="tituloSolucao_atividade" value="<?= $this->tituloSolucao; ?>" >
    <input type="hidden"  name="tituloEtapa_atividade" id="tituloEtapa_atividade" value="<?= $this->tituloEtapa_atividade; ?>" >

    <div class="col-lg-12" id="div_form_atividade">
        <?php require_once('formulario_atividade.php'); ?>
    </div>
</div>
<hr>
<fieldset>
    <legend> Lista de Atividades</legend>
    <div class="row">
        <div class="col-lg-12" id="div_listar_atividade">
            <?php require_once('listar.php'); ?>
        </div>
    </div>
</fieldset>