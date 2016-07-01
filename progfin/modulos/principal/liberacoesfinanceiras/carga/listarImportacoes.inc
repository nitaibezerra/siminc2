<?php
/**
 * Lista as importações de liberações financeiras realizadas no sistema.
 * $Id: listarImportacoes.inc 84387 2014-08-08 20:22:42Z maykelbraz $
 */
?>
<script type="text/javascript" language="javascript">
function detalharImportacao(llfid)
{
    var urlAtual = window.location.href;
    var urlSeguinte = urlAtual.replace('aba=importacoes', 'aba=detalhes&lote=' + llfid);
    window.location = urlSeguinte;
}
</script>
<?php
$sql = <<<DML
SELECT llf.llfid,
       llf.llfid AS loteid,
       TO_CHAR(llf.llfinclusao, 'DD/MM/YYYY às HH24:MI:SS') as llfinclusao,
       COUNT(1) AS qtdliberacoes,
       usu.usunome,
       esd.esdid,
       esd.esddsc
  FROM progfin.loteliberacoesfinanceiras llf
    INNER JOIN seguranca.usuario usu USING(usucpf)
    INNER JOIN workflow.documento doc USING(docid)
    INNER JOIN workflow.estadodocumento esd USING(esdid)
    LEFT JOIN progfin.liberacoesfinanceiras lfn USING(llfid)
  WHERE llf.llfano = '%s'
    GROUP BY llf.llfid,
             llf.llfinclusao,
             usu.usunome,
             esd.esdid,
             esd.esddsc
    ORDER BY llf.llfinclusao DESC
DML;
$stmt = sprintf($sql, $_SESSION['exercicio']);

$listagem = new Simec_Listagem();
$listagem->setCabecalho(array('Lote', 'Criação do lote', 'Qtd. liberações', 'Criador', 'Situação do lote'));
$listagem->setQuery($stmt);
$listagem->setAcoes(array('view' => 'detalharImportacao'))
    ->addCallbackDeCampo('esdid', 'formatarEstadoLote')
    ->addCallbackDeCampo('loteid', 'formatarNumeroLote')
    ->esconderColuna('esddsc');
$listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);

if (false === $listagem->render()): ?>
    <div class="alert alert-info col-md-4 col-md-offset-4 text-center">Nenhum registro encontrado</div>
<?php
endif;
