<?php

if (empty($_SESSION['obras2']['obrid']) && !empty($_GET['obrid'])) {
    if (!empty($_GET['obrid'])) {
        $_SESSION['obras2']['obrid'] = $_GET['obrid'];
    }
    if (empty($_SESSION['obras2']['empid'])) {
        $o = new Obras($_SESSION['obras2']['obrid']);
        $_SESSION['obras2']['empid'] = $o->empid;
    }
}

// empreendimento || obra || orgao
verificaSessao('obra');

$obrid = $_SESSION['obras2']['obrid'];

include_once APPRAIZ . "includes/cabecalho.inc";

// Monta as abas e o título da tela
print "<br/>";
if ($_SESSION['obras2']['orgid'] == ORGID_EDUCACAO_BASICA) {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA_FNDE, $url, $parametros);
} else {
    $db->cria_aba(ID_ABA_OBRA_CADASTRADA, $url, $parametros);
}
echo cabecalhoObra($obrid);
monta_titulo("Solicitação de Desembolso", "");

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>

<?

$where[] = "o.obrid = $obrid";

$sql = "

            SELECT
                sv.sldid,
                o.obrid,
                o.obrnome,
                m.estuf,
                m.mundescricao,
                sv.sldjustificativa,
                u.usunome usunome1,
                TO_CHAR(sv.slddatainclusao, 'DD/MM/YYYY') slddatainclusao,
                e.esddsc,
                e.esdid,
                (
                    SELECT ud.usunome FROM workflow.historicodocumento h
                    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                    LEFT JOIN seguranca.usuario ud ON ud.usucpf = h.usucpf
                    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as usunome,
                (
                    SELECT TO_CHAR(h.htddata, 'DD/MM/YYYY') FROM workflow.historicodocumento h WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as htddata,
                (
                    SELECT c.cmddsc FROM workflow.historicodocumento h
                    LEFT JOIN workflow.comentariodocumento  c ON c.hstid = c.hstid AND c.docid = d.docid
                    WHERE h.hstid = d.hstid ORDER BY h.htddata DESC LIMIT 1
                ) as cmddsc,
                sldpercsolicitado,
                supid,
                sldpercpagamento,
                sldobstec,
                sv.docid,
                ps.situacao_pagamento
            FROM obras2.solicitacao_desembolso sv
            JOIN obras2.obras o ON o.obrid = sv.obrid AND o.obridpai IS NULL AND o.obrstatus IN ('A', 'P')
            JOIN obras2.empreendimento emp ON emp.empid = o.empid
            JOIN entidade.endereco ed ON ed.endid = o.endid
            JOIN territorios.municipio m ON m.muncod = ed.muncod
            JOIN seguranca.usuario u ON u.usucpf = sv.usucpf
            JOIN workflow.documento d ON d.docid = sv.docid
            JOIN workflow.estadodocumento e ON e.esdid = d.esdid

             LEFT JOIN (
                    SELECT
                        sldid,
                        CASE WHEN situacao_pagamento IN ('8 - SOLICITAÇÃO APROVADA', 'SOLICITAÇÃO APROVADA') THEN 'Solicitação Aprovada'
                        WHEN situacao_pagamento IN ('Enviado ao SIGEF', '6 - VALA SIAFI', '0 - AUTORIZADO') THEN 'Pagamento Solicitado'
                        WHEN situacao_pagamento IN ('2 - EFETIVADO') THEN 'Efetivado'
                        ELSE ''
                        END as situacao_pagamento
                    FROM (

                        select pdo.sldid, pg.pagsituacaopagamento as situacao_pagamento
                            from par.pagamentodesembolsoobras pdo
                            inner join par.pagamentoobra po on po.pobid = pdo.pobid
                            inner join par.pagamento pg on pg.pagid = po.pagid
                            inner join obras2.solicitacao_desembolso sld on sld.sldid = pdo.sldid AND sld.sldstatus = 'A'
                        where pdo.pdostatus = 'A'
                        UNION ALL
                        select pdo.sldid, pg.pagsituacaopagamento as situacao_pagamento
                            from par.pagamentodesembolsoobras pdo
                            inner join par.pagamentoobrapar pop on pop.popid = pdo.popid
                            inner join par.pagamento pg on pg.pagid = pop.pagid
                            inner join obras2.solicitacao_desembolso sld on sld.sldid = pdo.sldid AND sld.sldstatus = 'A'
                            where pdo.pdostatus = 'A'
                    ) as p
            ) ps ON ps.sldid = sv.sldid


            WHERE sv.sldstatus = 'A' " . (count($where) ? ' AND ' . implode(' AND ',$where) : "") . " ORDER BY sv.sldid DESC";

$solicitacoes = $db->carregar($sql);
?>
<table  align="center" bgcolor="#f5f5f5" border="0" class="Tabela" cellpadding="3" cellspacing="1">
    <tbody>
<?
if($solicitacoes):
?>
<? foreach($solicitacoes as $solicitacao): ?>


        <tr>
            <td class="SubTituloCentro" colspan="3">
                Solicitação Nº <?=$solicitacao['sldid']?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">
                Situação
            </td>
            <td width="80%">
                <img
                    align="absmiddle"
                    src="/imagens/alterar.gif"
                    style="cursor: pointer"
                    onclick="javascript: abreSolicitacao('<?=$solicitacao['sldid']?>');"
                    title="Abrir Solicitação">


                    <? if (possui_perfil(array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC))) : ?>
                        <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO) : ?>
                            <b style="color:#00AA00"><?=$solicitacao['esddsc']?></b>
                        <? elseif ( $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO) : ?>
                            <b style="color:red"><?=$solicitacao['esddsc']?></b>
                        <? else: ?>
                            <b><?=$solicitacao['esddsc']?></b>
                        <? endif; ?>
                    <? else: ?>
                        <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO) : ?>
                            <b style="color:#00AA00"><?=$solicitacao['esddsc']?></b>
                        <? elseif ( $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO) : ?>
                            <b style="color:red"><?=$solicitacao['esddsc']?></b>
                        <? elseif ( $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO) : ?>
                            <b><?=$solicitacao['esddsc']?></b>
                        <? else: ?>
                            <b style="">Aguardando Análise FNDE</b>
                        <? endif; ?>
                    <? endif; ?>

            </td>

        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">
                Data da Solicitação
            </td>
            <td width="80%">
                <?=$solicitacao['slddatainclusao']?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">
                Percentual Solicitado
            </td>
            <td width="80%">
                <?=number_format($solicitacao['sldpercsolicitado'], 2, ',', '.')?>%
            </td>
        </tr>

        <? if ($solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_DEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_INDEFERIDO || $solicitacao['esdid'] == ESDID_SOLICITACAO_DESEMBOLSO_AGUARDANDO_CORRECAO) : ?>
        <tr>
            <td class="SubTituloDireita" width="20%">
                Percentual Validado
            </td>
            <td width="80%">
                <?=number_format($solicitacao['sldpercpagamento'], 2, ',', '.')?>%
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">
                Análisado Por
            </td>
            <td width="80%">
                <?=$solicitacao['usunome']?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">
                Observações da Análise
            </td>
            <td width="80%">
                <?= ($solicitacao['sldobstec']) ? $solicitacao['sldobstec'] : $solicitacao['cmddsc'] ?>
            </td>
        </tr>

        <tr>
            <td class="SubTituloDireita" width="20%">
                Situação do Pedido
            </td>
            <td width="80%">
                <?= ($solicitacao['situacao_pagamento']) ? $solicitacao['situacao_pagamento'] : '-' ?>
            </td>
        </tr>
        <? endif; ?>

<? endforeach; ?>
<? else: ?>
    <tr>
        <td class="SubTituloCentro" colspan="2">
            Nenhuma solicitação feita.
        </td>
    </tr>
<? endif; ?>
    </tbody>
</table>

<script type="text/javascript">
    function abreSolicitacao(sldid){
        var url = "/obras2/obras2.php?modulo=principal/popupSolicitarDesembolso&acao=A" +
            "&sldid=" + sldid;
        popup1 = window.open(
            url,
            "solicitarDesembolso",
            "width=1200,height=500,scrollbars=yes,scrolling=no,resizebled=no"
        );
        return false;
    }
</script>