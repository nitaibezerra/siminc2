<?php
//$modelDemanda = new Demanda();

require_once APPRAIZ . 'includes/library/simec/Listagem.php';
//include_once APPRAIZ . "demandasfies/classes/Demanda.class.inc";
//require_once 'demandaPorUsuarioControle.php';
include APPRAIZ . "includes/cabecalho.inc";

if ($_POST) {
    $area = (int)$_POST ['area'];
    $responsavel = $_POST ['responsavel'];
}

?>


<div class="row">
    <div class="col-md-12">
        <div class="page-header">
            <h3 id="forms">Relatório Geral</h3>
        </div>

        <form id="form-save" method="post" class="form-horizontal">

            <div class="well">
                <input name="action" type="hidden" value="pesquisar">

                <div class="form-group">
                    <label for="dmdid" class="col-lg-2 control-label">Área:</label>

                    <div class="col-lg-3 ">
                        <?php echo $db->monta_combo("area", $areas, 'S', "Selecione", "", "", "", " ", "N", "", "", $area , '', '', 'form-control chosen'); ?>
                    </div>
                </div>
                <div class="form-group">
                    <label for="dmddsc" class="col-lg-2 control-label">Responsável:</label>

                    <div class="col-lg-4">
                        <input id="responsavel" name="responsavel" type="text" class="form-control" placeholder="" value="<?= $responsavel; ?>">
                    </div>
                </div>
            </div>

            <div class="text-right">
                <button title="Salvar" class="btn btn-success" type="submit"><span
                        class="glyphicon glyphicon-search"></span> Pequisar
                </button>
            </div>
            <br>
        </form>
        <div class="clearfix"></div>
    </div>
</div>

<div class="row">
    <div class="col-lg-12">
        <?= $modelDemanda->getDadosRelatorioDemandasPorUsuario(); ?>
    </div>
</div>

<script type="text/javascript">
    $('.bt_hoje').on('click', function () {
        $("#labelModelDemandaUsuario").text('Lista de Demandas para Hoje - ' + $(this).data('nome'));
        var obj = $(this);
        $.post(window.location.href, {'cpf': obj.data('cpf'), 'campo': obj.data('campo'), 'destalharDemandas': '1'}, function (data) {
            $("#listaDemandaModal").html(data);
        });
    });
    $('.bt_posse').on('click', function () {
        $("#labelModelDemandaUsuario").text('Lista em Posse - ' + $(this).data('nome'));
        var obj = $(this);
        $.post(window.location.href, {'cpf': obj.data('cpf'), 'campo': obj.data('campo'), 'destalharDemandas': '1'}, function (data) {
            $("#listaDemandaModal").html(data);
        });
    });
    $('.bt_geral').on('click', function () {
        $("#labelModelDemandaUsuario").text('Lista Total de Demandas - ' + $(this).data('nome'));
        var obj = $(this);
        $.post(window.location.href, {'cpf': obj.data('cpf'), 'campo': obj.data('campo'), 'destalharDemandas': '1'}, function (data) {
            $("#listaDemandaModal").html(data);
        });
    });
</script>

<!-- Modal -->
<div class="modal fade" id="modal_demanda_por_usuario" tabindex="-1" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog" style="width: 1320px; ">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal">
                    <span aria-hidden="true">&times;</span><span class="sr-only">Fechar</span></button>
                <h4 class="modal-title" id="labelModelDemandaUsuario">Modal title</h4>
            </div>
            <div class="modal-body" id="listaDemandaModal"></div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>