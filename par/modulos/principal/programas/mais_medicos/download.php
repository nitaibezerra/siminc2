<script language="javascript" src="/contratogestao/js/fator_avaliado_execucao.js" charset="ISO-8859-1"></script>
<div class="row">
    <div class="col-lg-6">
        <?php if ($this->dataExecutor): ?>
            <h4>Pendências como Executor</h4>
            <br>
            <?php $this->listing->listing($this->dataExecutor); ?>
        <?php endif ?> 
            
        <?php if ($this->dataValidador): ?>
            <h4>Pendências como Validador</h4>
            <br>
            <?php $this->listing->listing($this->dataValidador); ?>
        <?php endif ?> 

        <?php if ($this->dataCertificador): ?>
            <h4>Pendências como Certificador</h4>
            <br>
            <?php $this->listing->listing($this->dataCertificador); ?>
        <?php endif ?> 

    </div>
    
    <div class="col-lg-6">
        <div id="container-form-fator-avaliado-execucao"> </div>
    </div>
</div>

<script type="text/javascript">
    $(document).ready(function() {
        $('.pagination').hide();
    });
</script>
