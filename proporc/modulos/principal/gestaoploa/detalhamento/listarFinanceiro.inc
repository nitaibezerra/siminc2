<?php
/**
 * Formulário de cadastramento de novos detalhamentos de receita.
 * $Id: listarFinanceiro.inc 101265 2015-08-14 13:05:26Z maykelbraz $
 */
?>
<script language="JavaScript" src="js/gestaoploa.js"></script>
<script type="text/javascript" lang="JavaScript">
$(document).ready(function(){
    $('#novoPO').click(novoFinanceiro);
    $('#modal-confirm .modal-body').text(
        'Você tem certeza que deseja apagar o registro selecionado? Apenas os dados referentes a este registro serão apagados.'
    );
    $('#modal-confirm .btn-primary').click(function(){
        $('#formfinanceiro').submit();
    });
});
</script>
<div class="col-md-10 col-md-offset-1">
    <div class="well well-sm row form-group">
        <div class="col-md-2">
            <label class="control-label" for="filtro_ploid">Plano orçamentário:</label>
        </div>
        <div class="col-md-9"><?php inputComboPOPLOA($dados, 'filtro_ploid'); ?></div>
        <div class="col-md-1">
            <button type="submit" class="btn btn-default">Filtrar</button>
        </div>
    </div>
</div>
<br style="clear:both" />
<?php
$query = <<<DML
SELECT dpa.dpaid,
       dpa.ploid,
       dpa.ungcod,
       COALESCE(sba.sbacod, '-') AS sbacod,
       dpa.sbaid,
       plo.plocodigo,
       plo.plotitulo,
       ndp.ndpcod,
       dpa.ndpid,
       dpa.foncod,
       COALESCE(plf.plfvalor, 0) as plfvalor,
       dpa.dpavalor,
       CASE WHEN plf.plfid IS NULL THEN 'S' ELSE 'A' END AS stsdespesa
  FROM elabrev.despesaacao dpa
    INNER JOIN public.naturezadespesa ndp USING(ndpid)
    LEFT JOIN elabrev.subacao sba USING(sbaid)
    INNER JOIN elabrev.planoorcamentario plo ON (dpa.ploid = plo.ploid AND plo.plostatus = 'A' AND dpa.acaid = plo.acaid)
    LEFT JOIN proporc.ploafinanceiro plf ON(plf.dpaid = dpa.dpaid AND plf.mtrid = %d)
  WHERE dpa.acaid = %d
    AND dpa.ppoid = %d
    AND EXISTS (SELECT 1
                  FROM proporc.despesafonterecurso dfr
                  WHERE dfr.dspid = plf.mtrid
                    AND dfr.foncod = dpa.foncod)
DML;

$stmt = sprintf($query, $dados['mtrid'], $dados['acaid'], $dados['ppoid']);

$cabecalho = array(
    'Plano Orçamentário',
    'Natureza',
    'Fonte',
    'Valor (R$)',
    'Total (R$)',
    'Status'
);
$colunasOcultas = array('ploid', 'ungcod', 'sbacod', 'sbaid', 'plocodigo', 'ndpid');
if (in_array($dados['unicod'], array('26101','26291', '26290', '26298', '74902', '73107'))) {
    $cabecalho = array(
        'UG',
        'Subação',
        'Plano Orçamentário',
        'Natureza',
        'Fonte',
        'Valor (R$)',
        'Total (R$)',
        'Status'
    );
    $colunasOcultas = array('ploid', 'plocodigo', 'ndpid', 'sbaid');
}

$list = new Simec_Listagem();
$list->setQuery($stmt)
    ->setCabecalho($cabecalho)
    ->addCallbackDeCampo(array('plfvalor', 'dpavalor'), 'mascaraNumero')
    ->addCallbackDeCampo('stsdespesa', 'cbStatusDespesa')
    ->addCallbackDeCampo('plotitulo', 'concatenaPO')
    ->esconderColunas($colunasOcultas)
    ->setTotalizador(Simec_Listagem::TOTAL_SOMATORIO_COLUNA, array('plfvalor'));

if ($podeEditarDados) {
    $list->addAcao(
        'edit',
        array(
            'func' => 'editarFinanceiro',
            'extra-params' => array('ploid', 'ungcod', 'sbaid', 'ndpid', 'ndpcod', 'foncod', 'plfvalor')
        )
    )->addAcao('delete', 'excluirFinanceiro');
}
$list->addAcao(
    'info',
    array(
        'func' => 'detalharFinanceiro',
        'external-params' => array('mtrid' => $dados['mtrid'])
    )
);
$list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

if ($podeEditarDados): ?>
<button type="button" class="btn btn-success" id="novoPO">Novo</button>
<?php else: echo <<<HTML
<div class="alert alert-danger text-center col-lg-6 col-lg-offset-3"><strong>Sem permissão para edição!</strong></div>
HTML;
endif; ?>
<br style="clear:both" />
<br />
<?php require(dirname(__FILE__) . '/formFinanceiro.inc'); ?>
<div class="modal fade" id="modal-info">
    <div class="modal-dialog modal-lg">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">&times;</button>
                <h4 class="modal-title">Detalhamento da despesa</h4>
            </div>
            <div class="modal-body">
            </div>
            <div class="modal-footer">
                <button type="button" class="btn btn-warning" data-dismiss="modal">Fechar</button>
            </div>
        </div>
    </div>
</div>