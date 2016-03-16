
<div class="row">
    <div class="col-lg-12 well">
        <h4>Filtro:</h4>
        <form method="get" id="form-busca-fator-avaliado-execucao" class="form-horizontal">
            <div class="form-group">
                <label for="atividade" class="col-lg-4 control-label">Selecione um Contrato:</label>
                <div class="col-lg-5">
                    <select name="atividade" id="atividade" class="form-control">
                        <?= $this->hierarquiaContrato->getOptionsContrato(); ?>
                    </select>
                </div>
            </div>

            <div class="text-right">
                <button id="filtraContrato" type="button" class="btn btn-primary"><span class="glyphicon glyphicon-search"></span> Pesquisar</button>
            </div>
        </form>
    </div>
</div>

<script type="text/javascript">
    $(document).ready(function() {
        $('#filtraContrato').on('click', function() {
            $.post(window.location.href, {controller: 'execucaoFatorAvaliado', action: 'pesquisar', conid: $('#atividade').val() }, function(data) {
                $('#div_pesquisar').html(data);
            });
        });
    });
</script>

<div id="div_pesquisar">
    <?php include_once 'pesquisar.php'; ?>
</div>
