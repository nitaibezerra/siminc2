<?php
$listagem = new Simec_Listagem();
$listagem->setCabecalho(
    array(
        'Programática',
        'Descrição',
        'Físico' => array('Atual', 'Alteração'),
        'Status do Localizador'
    ))
    ->setAcoes(
        array(
            'plus' => array(
                'func' => 'detalharLocalizador',
                'extra-params' => array('paoid')
            )
        ))
    ->addCallbackDeCampo('ajustefisico', 'ajusteFisico')
    ->addCallbackDeCampo('locstatus', 'statusLocalizador')
    ->esconderColuna('paoid');

// -- Filtros do formulário
$params = array($dadospao['paoid'], $_SESSION['exercicio'], $dadospao['unicod']); // -- parametros fixos da query
$whereFiltros = '';
if (isset($_REQUEST['dados'])) {
    $whereFiltros = array();
    if (chaveTemValor($_REQUEST['dados'], 'acacod')) {
        $whereFiltros[] = "sna.acacod = '%s'";
        $params[] = $_REQUEST['dados']['acacod'];
    }
    if (chaveTemValor($_REQUEST['dados'], 'prgcod')) {
        $whereFiltros[] = "sna.prgcod = '%d'";
        $params[] = $_REQUEST['dados']['prgcod'];
    }
    if (!empty($whereFiltros)) {
        $whereFiltros = 'AND ' . implode(' AND ', $whereFiltros);
    } else {
        $whereFiltros = '';
    }
}

$sql = <<<DML
SELECT sna.snaid,
       esfcod || '.' || unicod || '.' || funcod || '.' || sfucod || '.' || prgcod || '.' || acacod || '.' || loccod AS programatica,
       acadsc || ' - ' || COALESCE(locdsc, '???') || ' - ' || tic.ticdsc,
       sna.proquantidade AS a,
       (SUM(paf.valacrescimo) - SUM(paf.valreducao)) AS ajustefisico,
       COALESCE(paf.paoid, -1) AS locstatus,
       paf.paoid
  FROM altorc.snapshotacao sna
    INNER JOIN altorc.tipoinclusao tic ON tic.ticid = tilcod::INTEGER
    LEFT JOIN altorc.pedidoalteracaofisico paf ON (sna.snaid = paf.snaid AND paf.paoid = %d)
  WHERE sna.snaexercicio = '%d'
    AND sna.unicod = '%s'
    {$whereFiltros}
    AND paf.paoid IS NOT NULL
  GROUP BY sna.snaid, sna.esfcod, unicod, funcod, sfucod, prgcod, acacod, loccod, acadsc, locdsc, ticdsc, sna.proquantidade, paf.paoid
  ORDER BY locstatus DESC
DML;
$sql = vsprintf($sql, $params);
$listagem->setQuery($sql)
    ->setId('dados-programatica')
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
<style>
    @media (min-width: 992px) {.modal-lg {width:1200px}}
</style>
<script>
/**
 * detalheLocalizador
 */
function detalharLocalizador(snaid, paoid) {
    var arowid = '#arow-' + snaid;
    var $parentTR = $(arowid).parents('tr');

    if ($(arowid + ' span').hasClass('glyphicon-plus')) {
        $.post(
            window.location,
            {requisicao: 'detalharLocalizador', snaid: snaid, paoid: paoid},
            function(html){
                $(arowid + ' span').removeClass('glyphicon-plus').addClass('glyphicon-minus');
                var numCols = $('td', $parentTR).length;
                $parentTR.after('<tr><td colspan="' + numCols + '">' + html + '</td></tr>');
            }
        );
    } else {
        $parentTR.next().remove();
        $(arowid + ' span').removeClass('glyphicon-minus').addClass('glyphicon-plus');
    }
}
</script>
<div class="modal fade" id="modal-po">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Detalhe do pedido</h4>
            </div>
            <div class="modal-body">
                <div id="conteudoModalPo"></div>
            </div>
        </div>
    </div>
</div>