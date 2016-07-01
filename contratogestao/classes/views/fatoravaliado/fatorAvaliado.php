<div class="modal fade" id="dialogo_etapas_de_controle">
    <div class="modal-dialog-large">
        <div class="modal-content" id="form_etapas_de_controle"></div>
    </div>
</div>

<?php if ($this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) or is_null( $this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) ) )  : ?>
    <div id="container-form-fator-avaliado">
        <?php include_once 'formulario_fator_avaliado.php'; ?>
    </div>
    <hr>
<?php endif; ?>

<div id="container-listar-fator-avaliado">
    <?php $this->listing->listing($this->data); ?>
</div>

<script type="text/javascript">
    $(document).ready(function() {
        $('.pagination').hide();
        $('.historico_workflow').closest('td').addClass('text-center');
        $('.download_grid').closest('td').addClass('text-center');
        $('.load-listing-ajax-order').unbind();
<?php if ($this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']) === false or is_null( $this->view->perfilUsuario->validarAcessoModificacao($_SESSION['conid']))) : ?>
            $('.btn_editar, .btn_excluir').remove();
<?php endif; ?>
    });
</script>
