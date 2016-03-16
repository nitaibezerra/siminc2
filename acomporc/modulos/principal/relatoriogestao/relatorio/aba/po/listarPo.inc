<?php
/**
 * Lista de POs associados a uma ação.
 *
 * @see listarPo.inc
 * @version $Id: listarPo.inc 95729 2015-03-24 17:43:21Z lindalbertofilho $
 */

$whereResp = '';
if (in_array(PFL_RELATORIO_TCU, $perfis)) {
    $whereResp = <<<DML
    AND EXISTS
        (SELECT 1
            FROM planacomorc.relgdados rld2
            INNER JOIN planacomorc.usuarioresponsabilidade rpu2 ON(rld2.rldid = rpu2.rldid)
            WHERE rld2.rlgid = rld.rlgid
                AND rld2.rldid = rld.rldid
                AND rpu.usucpf = '%s'
                AND rpu.pflcod = '%s'
                AND rpu2.rpustatus = 'A'
                AND rld2.rldtipo = 'po')
DML;
    $whereResp = sprintf($whereResp, $_SESSION['usucpf'], PFL_RELATORIO_TCU);
}

$select = <<<DML
    SELECT
        rld.rldid,
        rld.rldcod,
        rld.rldtitulo,
        rld.rlddotacaoinicial,
        rld.rlddotacaofinal,
        rld.rlddespempenhada,
        rld.rlddespliquidada,
        rld.rlddesppaga,
        usu.usunome
    FROM acomporc.relgdados rld
    LEFT JOIN acomporc.usuarioresponsabilidade rpu ON(rld.rldid = rpu.rldid AND rpu.rpustatus = 'A')
    LEFT JOIN seguranca.usuario usu USING(usucpf)
    WHERE rld.rlgid = %d
        AND rld.rldtipo = 'po'
        {$whereResp}
DML;
$stmt = sprintf($select, $rlgid);
$cabecalho = array(
    'Código',
    'Título',
    'Dotação (R$)' => array('Inicial', 'Final'),
    'Despesa (R$)' => array('Empenhada', 'Liquidada', 'Paga'),
    'Responsável'
);
$camposMonetarios = array(
    'rlddotacaoinicial', 'rlddotacaofinal', 'rlddespempenhada', 'rlddespempenhada', 'rlddespliquidada', 'rlddesppaga'
);

$list = new Simec_Listagem();
$list->setQuery($stmt)
    ->setCabecalho($cabecalho)
    ->addCallbackDeCampo($camposMonetarios, 'mascaraMoeda')
    ->addCallbackDeCampo('rldtitulo', 'alinhaParaEsquerda');
if (!$usuarioTCU) {
    $list->addAcao('edit', 'editarPo');
}
$list->addAcao('edit2', 'editarJustificativas')
    ->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
?>
<script type="text/javascript" lang="JavaScript">
function editarPo(rldid)
{
    var url = window.location.href;
    url = url.replace(/aba=po/, 'aba=detalhamentopo&rldid=' + rldid);
    window.location.href = url;
}
function editarJustificativas(rldid)
{
    var url = window.location.href;
    url = url.replace(/aba=po/, 'aba=justificativapo&rldid=' + rldid);
    window.location.href = url;
}

$(document).ready(function(){
    $('#novo-po').click(function(){
        editarPo('');
    });
});
</script>
<?php if (!$usuarioTCU): ?>
<button type="button" class="btn btn-success" id="novo-po">
    <span class="glyphicon glyphicon-plus"></span> Novo Plano Orçamentário</span>
</button>
<?php endif;