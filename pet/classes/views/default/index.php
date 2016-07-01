<div class="page-header">
    <h1><?= $this->titulo; ?></h1>
</div>

<div class="row">
    <div class="col-lg-12" id="div_msg_ordenacao" style="display:none">
        <div class="alert alert-dismissable alert-success">
            <strong>Sucesso! </strong><span id="msg_retorno"></span>
        </div>
    </div>
</div>

<div class="row">
    <button type="button" style="display: none;" class="btn btn-primary btn_visualizar_lista_grupo">
        <i class="glyphicon glyphicon-list"></i> Visualizar Lista de Grupos</button>

    <div class="col-lg-12" id="div_lista_grupo">
        <fieldset>
            <legend>Lista de Grupos</legend>
            <?= $this->grupo->getListaCursos(); ?>
        </fieldset>
    </div>
</div>

<hr>

<div class="row">
    <div class="col-lg-12" id="div_grupo"></div>
</div>

<script type="text/javascript">
    $(function () {
        $('.btn_selecionar').on('click', function () {
            $('.btn_visualizar_lista_grupo').show();
            $('#div_lista_grupo').hide();

            atualializaGrupo($(this).data('id'))
        });

        $('.btn_visualizar_lista_grupo').on('click', function () {
            $('.btn_visualizar_lista_grupo').hide();
            $('#div_lista_grupo').show();
        })
    });
</script>

