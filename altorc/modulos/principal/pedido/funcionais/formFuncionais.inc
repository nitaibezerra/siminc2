<?php
/**
 * Filtro de funcionais.
 * $Id: formFuncionais.inc 85317 2014-08-22 17:53:21Z werteralmeida $
 */
?>
<script type="text/javascript">
function carregarProgramas(acacod)
{
    // -- requisicao de dados do tipo de crédito, por momento
    $.post(
        window.location,
        {'requisicao':'programas',
         'dados[acacod]':acacod,
         'dados[unicod]':'<?php echo $dadospao['unicod']; ?>'
        },
        function(data){
            var _data = JSON.parse(data);
            // -- limpando o select
            $('#prgcod').find('option').remove().end().append($('<option>', {value: '', text:'Todos os programas'}));
            if (_data) {
                $.each(_data, function(index, value){
                    $('#prgcod').append($('<option>', {value:value.codigo, text:value.descricao}));
                });
            }
            $('#prgcod.chosen-select').trigger('chosen:updated');
        },
        'text'
    );
}
</script>
<?php // não submeter unicod com o formulário; ?>
<form name="filtrarfuncionais" id="filtrarfuncionais" method="POST" role="form">
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="acacod">Ação:</label>
        </div>
        <div class="col-md-10">
        <?php
        $sql = <<<DML
SELECT DISTINCT sna.acacod AS codigo,
                sna.acacod || ' - ' || sna.acadsc AS descricao
  FROM altorc.snapshotacao sna
  WHERE sna.snaexercicio = '%d'
    AND sna.unicod = '%s'
DML;
        $sql = sprintf($sql, $_SESSION['exercicio'], $dadospao['unicod']);
        $db->monta_combo('dados[acacod]', $sql, 'S', 'Todas as ações', 'carregarProgramas', null, null, null, 'N', 'acacod', null, $_REQUEST['dados']['acacod'], null, 'class="form-control chosen-select" style="width=100%"');
        ?>
        </div>
    </div>
    <div class="form-group row">
        <div class="col-md-2">
            <label class="control-label" for="prgcod">Programa:</label>
        </div>
        <div class="col-md-10">
        <?php
        $dadosprg = carregarProgramas($_REQUEST['dados']['acacod'], $_SESSION['exercicio'], false, $dadospao['unicod']);
        $db->monta_combo('dados[prgcod]', $dadosprg, 'S', 'Todos os programas', null, null, null, null, 'N', 'prgcod', null, $_REQUEST['dados']['prgcod'], null, 'class="form-control chosen-select" style="width=100%"');
        ?>
        </div>
    </div>
    <button type="submit" class="btn btn-primary">Buscar</button>
    <button type="reset" class="btn btn-warning">Limpar</button>
</form>