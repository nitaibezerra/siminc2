<div class="row">
    <input type="hidden"  name="tituloSolucao" id="tituloSolucao" value="<?= $this->tituloSolucao; ?>" >
    <div class="col-lg-12" id="div_form_etapa">
        <?php require_once('formulario_etapa.php'); ?>
    </div>
</div>
<hr>
<fieldset>
    <legend> Lista de Etapas</legend>
    <div class="row">
        <div class="col-lg-12" id="div_listar_etapa">
            <?php require_once('listar.php'); ?>
        </div>
    </div>
</fieldset>

<script type="text/javascript">
    $(function () {
        $('#div_listar_etapa').on('click', '.btn_editar_etapa', function (event) {
            editarEtapa($(this))
        });

        $('.btn_excluir_etapa').on('click', function () {
            $.deleteItem({ controller: 'etapa', action: 'excluir', retorno: true, text: 'Deseja realmente excluir esta Etapa?', id: $(this).data('id'), functionSucsess: 'atualizaGridEtapa' });
        });

        $('#div_listar_etapa').on('click', '.btn_editar_atividade', function (event) {
            var solid = <?= $_SESSION['solid']; ?>;
            var etpid = $(this).closest('table').closest('tr').prev().find('.btn_editar_etapa').data('id');
            editarAtividadePelaSolucao($(this).data('id'), solid, etpid);
        });

        <?php if ( !empty($_SESSION['atvid']) ){ ?>
        $('#aba_cadastro_atividade').closest('li').removeClass('disabled');
        <?php } ?>

        <?php  if (is_int($_SESSION['etpid']) ):  ?>
        var objLink = $("#div_listar_etapa").find('.btn_editar_etapa[data-id="<?= $_SESSION['etpid']; ?>"]');
        editarEtapa(objLink);
        <?php else: ?>
        $('#tituloSolucao').val('')
        $('#div_etapa_selecionada').html('');
        <?php endif; ?>
    });
</script>
