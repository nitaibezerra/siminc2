<?php
$atiid = $_REQUEST['atiid'];

$sql = "select
        a.atidescricao,
        ue.uexcod || ' - ' || ue.uexsigla as unidadeexecutora,
        a.atinumeropi,
        (a.atiorcamentocusteio + a.atiorcamentocapital) as atiorcamentototal,
        prg.prgcod || ' - ' || prg.prgdsc AS prgdsc,
        aca.acacod || ' - ' || aca.acadsc AS acadsc,
        l.loccod || ' - ' || l.locdsc AS locdsc
    FROM pde.atividade a
    LEFT JOIN monitora.acao aca on aca.acaid = a.acaid
    LEFT JOIN monitora.programa prg ON prg.prgcod = aca.prgcod
    LEFT JOIN planointerno.unidadeexecutora ue ON ue.uexid = a.uexid
    LEFT JOIN public.localizador l ON l.loccod = aca.loccod
    where
        a.atiid = {$atiid}
        ";

$dadosPa = $db->pegaLinha($sql);
?>
<script type="text/javascript">

    function abrirDadosPA( id )
    {
        window.open(
        '?modulo=principal/dadospa&acao=C&atiid=' + id,
        'dadospa',
        'width=670,height=580,scrollbars=yes,scrolling=yes,resizebled=yes'
        );
    }

</script>
<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
    <tr>
        <td colspan="2" align='right' class="SubTituloDireita" style="text-align: center; font-weight: bold">
            Dados do Plano de Ação
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Título:</td>
        <td>
            <a href="javascript: abrirDadosPA('<?= $atiid; ?>')"><?= $dadosPa['atidescricao']; ?></a>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Unidade Executora:</td>
        <td>
            <?= $dadosPa['unidadeexecutora']; ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Plano Interno:</td>
        <td>
            <?= sprintf( '%05s', $dadosPa['atinumeropi'] ) ?> - Valor Global: <?= formata_valor($dadosPa['atiorcamentototal']); ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Programa:</td>
        <td>
            <?= $dadosPa['prgdsc']; ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Ação:</td>
        <td>
            <?= $dadosPa['acadsc']; ?>
        </td>
    </tr>
    <tr>
        <td align='right' class="SubTituloDireita">Localizador:</td>
        <td>
            <?= $dadosPa['locdsc']; ?>
        </td>
    </tr>
</table>