<script language="javascript" src="/contratogestao/js/fator_avaliado_execucao.js" charset="ISO-8859-1"></script>

<div class="row">
    <div class="col-lg-6">
        <?php if ($this->dataExecutor): ?>
            <div id="div_executor">
                <h4>Pendências como Executor</h4>
                <br>
                <?php $this->listing->listing($this->dataExecutor); ?>
            </div>
        <?php endif ?> 

        <?php if ($this->dataValidador): ?>
            <div id="div_validador">
                <h4>Pendências como Validador</h4>
                <br>
                <?php $this->listing->listing($this->dataValidador); ?>
            </div>
        <?php endif ?> 

        <?php if ($this->dataCertificador): ?>
            <div id="div_certificador">
                <h4>Pendências como Certificador</h4>
                <br>
                <?php $this->listing->listing($this->dataCertificador); ?>
            </div>
        <?php endif ?> 

        <?php if (empty($this->dataExecutor) && empty($this->dataValidador) && empty($this->dataCertificador)): ?>
            <div class="alert alert-warning" role="alert">Você não possui nenhum Fator Avaliado para Execução</div>
        <?php endif; ?>
    </div>

    <div class="col-lg-6">
        <div id="container-form-fator-avaliado-execucao">
            <?php if ($this->fatorAvaliado->getAttributeValue('fatid')): ?>
                <?php include_once 'formulario.php'; ?>
            <?php endif; ?>
        </div>
    </div>
</div>

<script type="text/javascript">
    $(document).ready(function() {
        $('.pagination').hide();
        $('.load-listing-ajax-order').unbind();
        $('.historico_workflow').closest('td').addClass('text-center');
    });
</script>
