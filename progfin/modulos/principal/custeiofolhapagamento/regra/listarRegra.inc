<?php
/**
 * Listagem de momentos de crédito.
 * $Id: listarregra.inc 78244 2014-04-01 18:26:00Z maykelbraz $
 */
$query = <<<DML
SELECT
    rcc.rccid,
    rcc.rccdsccoluna,
    rcc.rccnomecoluna,
    rcc.rccelementodespesa,
    rcc.rccrubrica
FROM
    progfin.regrascargacusteiofolhapagamento rcc
DML;

$listagem = new Simec_Listagem();
$listagem->turnOnPesquisator();
$listagem->setQuery($query)
        ->setCabecalho(
                array(
                    'Descrição da Coluna',
                    'Nome da Coluna',
                    'Elementos de Despesa',
                    'Rubricas'
                )
        )->setAcoes(array('edit' => 'editarregra', 'delete' => 'apagarregra'));
?>
<script type="text/javascript" language="javascript">
    function apagarregra(rccid)
    {
        $('#requisicao-regra').val('apagarRegra');
        $('#rccid').val(rccid);

        $('#modal-confirm .modal-body p').html('Tem certeza que quer excluir a regra selecionado?');
        $('#modal-confirm').modal();
    }

    function editarregra(rccid)
    {
        $('#requisicao-regra').val('editarRegra');
        $('#rccid').val(rccid);
        $('#form-regra').submit();
    }

    $('document').ready(function() {
        $('#modal-confirm .btn-primary').click(function() {
            $('#form-regra').submit();
        });
    });
</script>
<?php
// -- Saída do relatório
if (false === $listagem->render()):
    ?>
    <div class="alert alert-info col-md-4 col-md-offset-4 text-center">Nenhum registro encontrado</div>
    <?php
endif;
?>