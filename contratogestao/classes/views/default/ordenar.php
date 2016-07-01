<script language="javascript" src="/contratogestao/js/contrato_gestao.js"></script>

<?php if ($this->view->msg): ?>
    <script type="text/javascript">
        $('#modal-alert').modal('show').children('.modal-dialog').children('.modal-content').children('.modal-body').
            html('<div class="alert alert-danger"><?= $this->view->msg ?></div>');
    </script>
<?php endif; ?>
<input name="hqcidpai_interagido" id="hqcidpai_interagido" type="hidden" value="<?php echo $this->hqcidpai_interagido ?>">
<?php //echo $this->hierarquiaContrato->getArvore(); ?>