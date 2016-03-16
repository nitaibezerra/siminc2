<script language="javascript" src="/contratogestao/js/form_etapa_pessoa_fisica.js" charset="ISO-8859-1"></script>
<script language="javascript" src="/contratogestao/js/form_etapa_pessoa_juridica.js" charset="ISO-8859-1"></script>

<fieldset>
    <div class="row">
        <div class="col-lg-12">
            <div class="form-group">
                <label for="pessoa" class="col-lg-2 control-label"><?= ucfirst($this->etapa); ?></label>
                <div class="col-lg-4">
                    <select class="form-control" id="pessoa" name="pessoa">
                        <?= $this->fatorAvaliado->getCombo(array( $this->etapa => true, 'tipo' => $this->tipoPessoa)) ?>
                    </select> 
                </div>
            </div>
        </div>
    </div>
</fieldset>

<?php if ($this->tipoPessoa === 'fisica'): ?>
    <?php include_once 'formEtapaControleFisica.php'; ?>
<?php elseif ($this->tipoPessoa === 'juridica'): ?>
    <?php include_once 'formEtapaControleJuridica.php'; ?>
<?php endif; ?>