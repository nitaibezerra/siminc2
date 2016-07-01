<?php
/**
 * Faz a listagem das metas dos POs de uma ação.
 * $Id: listarMetas.inc 100520 2015-07-28 13:03:38Z maykelbraz $
 */
?>
<script type="text/javascript" lang="JavaScript">
function editarMetaPO(ploid, plodsc, prddsc, unmdsc, mpovalor, memcalculo)
{
    $('#ploid').val(ploid);
    $('#plodsc_l').text(plodsc);
    $('#prddsc_l').text(prddsc);
    $('#unmdsc_l').text(unmdsc);
    $('#mpovalor').val(mpovalor);
    $('#memcalculo').val(memcalculo);

    $('#modal-metas').modal();
}
</script>
<?php
$query = <<<DML
SELECT pa.ploid,
       pa.plocodigo || ' - ' || pa.plotitulo AS descricao,
       COALESCE(CASE WHEN '' = pa.ploproduto THEN '-' ELSE pa.ploproduto END, '-') AS ploproduto,
       COALESCE(CASE WHEN '' = pa.plounidademedida THEN '-' ELSE pa.plounidademedida END, '-') AS plounidademedida,
       COALESCE(mp.mpovalor, 0) AS mpovalor,
       SUM(COALESCE(dpa.dpavalor, 0)) AS dpavalor,
       (CASE WHEN COALESCE(mp.mpovalor, 0) = 0 THEN 0
             ELSE SUM(COALESCE(dpa.dpavalor, 0)) / COALESCE(mp.mpovalor, 0)
         END)::NUMERIC(15,2) AS vlrmedio,
       CASE WHEN mp.mpoid IS NULL THEN 'S' ELSE 'A' END AS statuspo,
       mp.memcalculo
  FROM elabrev.planoorcamentario pa
    LEFT JOIN elabrev.metaplanoorcamentario mp ON(mp.ploid = pa.ploid AND mp.mpostatus = 'A')
    LEFT JOIN elabrev.despesaacao dpa ON(pa.ploid = dpa.ploid)
  WHERE pa.acaid = %d
    AND pa.plostatus = 'A'
  GROUP BY pa.ploid,
           pa.plocodigo,
           pa.plotitulo,
           mp.mpovalor,
           pa.ploproduto,
           pa.plounidademedida,
           mp.mpoid,
           mp.memcalculo
  ORDER BY pa.plocodigo
DML;

$stmt = sprintf($query, $dados['acaid']);

$list = new Simec_Listagem();
$list->setQuery($stmt)
    ->setCabecalho(array('Plano Orçamentário', 'Produto', 'Unidade de medida', 'Quantidade', 'Valor (R$)', 'Valor médio (R$)', 'Status'))
    ->addCallbackDeCampo('mpovalor', 'mascaraNumero')
    ->addCallbackDeCampo('statuspo', 'cbStatusDespesa')
    ->addCallbackDeCampo('descricao', 'alinharEsquerda')
    ->addCallbackDeCampo('dpavalor', 'mascaraNumero')
    ->addCallbackDeCampo('vlrmedio', 'mascaraMoeda')
    ->esconderColunas('memcalculo');
if ($podeEditarDados) {
    $list->setAcoes(
        array(
            'edit' => array(
                'func' => 'editarMetaPO',
                'extra-params' => array('descricao', 'ploproduto', 'plounidademedida', 'mpovalor', 'memcalculo')
            )
        )
    );

    $list->setAcaoComoCondicional('edit', array(
        array('campo' => 'ploproduto', 'valor' => '-', 'op' => 'diferente'),
        array('campo' => 'dpavalor', 'valor' => '0', 'op' => 'diferente')
    ));

} else {
    $list->esconderColunas(array('ploid', 'memcalculo'));
}
$list->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);

if (!$podeEditarDados) {
    echo <<<HTML
<div class="alert alert-danger text-center col-lg-6 col-lg-offset-3"><strong>Sem permissão para edição!</strong></div>
HTML;
}

/**
 * Formulário de alteração de metas do PO - popup.
 * @see formMetas.inc
 */
require(dirname(__FILE__) . '/formMetas.inc');
