<table id="previsao" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
    <tr id="tr_title">
        <td colspan="11" class="subtitulocentro" align="center">
            <h3>Célula(s) Orçamentária(s) Com Valor(es) Zerado(s)</h3>
        </td>
    </tr>
    <tr id="tr_titulo">
        <td class="subtitulocentro">&nbsp;</td>
        <td class="subtitulocentro">&nbsp;</td>
        <td class="subtitulocentro" width="10%">Ano</td>
        <td class="subtitulocentro" width="">Ação</td>
        <td class="subtitulocentro" width="14%">Programa de Trabalho</td>
        <td class="subtitulocentro" width="10%">Plano Interno</td>
        <td class="subtitulocentro" width="">Descrição da Ação Constante da LOA</td>
        <td class="subtitulocentro" width="20%">Nat.da Despesa</td>
        <td class="subtitulocentro" width="13%">Valor (em R$ 1,00)</td>
        <td class="subtitulocentro" width="8%">Mês da Liberação</td>
        <td class="subtitulocentro" width="20%">Prazo para o cumprimento do objeto</td>
    </tr>
    <?php
    $sql = "
        SELECT DISTINCT
            pro.proid,
            ptres || ' - ' || p.funcod||'.'||p.sfucod||'.'||p.prgcod||'.'||p.acacod||'.'||p.unicod||'.'||p.loccod as ptrid_descricao,
            substr(pi.plicod||' - '||pi.plidsc, 1, 45)||'...' as pliid_descricao,
            substr(ndp.ndpcod, 1, 6) || ' - ' || ndp.ndpdsc as ndp_descricao,
            pro.ptrid,
            a.acacod,
            pro.pliid,
            case when a.acatitulo is not null then substr(a.acatitulo, 1, 70)||'...' else substr(a.acadsc, 1, 70)||'...' end as acatitulo,
            pro.ndpid,
            to_char(pro.provalor, '999G999G999G999G999D99') as provalor,
            case
                when (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid) IS NOT NULL then
                    coalesce(pro.provalor - (select sum(cr.valor) from elabrev.creditoremanejado cr where cr.proid = pro.proid), 0)
                else
                    coalesce(pro.provalor, 0)
            end as valor,
            crdmesliberacao,
            crdmesexecucao,
            pro.proid,
            pro.proanoreferencia,
            pro.prodata,
            (select ppa2.codncsiafi from elabrev.previsaoparcela ppa2 where ppa2.ppaid = (select max(ppa1.ppaid) from elabrev.previsaoparcela ppa1 where ppa1.proid = pro.proid) and ppa2.ppacancelarnc = 'f') as lote,
            pp.codsigefnc,
            pp.codncsiafi,
            tc.ungcodconcedente
        FROM monitora.previsaoorcamentaria pro
        LEFT JOIN monitora.pi_planointerno pi 		ON pi.pliid = pro.pliid
        LEFT JOIN monitora.pi_planointernoptres pts ON pts.pliid = pi.pliid
        LEFT JOIN public.naturezadespesa ndp 		ON ndp.ndpid = pro.ndpid
        LEFT JOIN monitora.ptres p 					ON p.ptrid = pro.ptrid
        LEFT JOIN monitora.acao a 					ON a.acaid = p.acaid
        LEFT JOIN public.unidadegestora u 			ON u.unicod = p.unicod
        LEFT JOIN monitora.pi_planointernoptres pt 	ON pt.ptrid = p.ptrid
        LEFT JOIN elabrev.previsaoparcela pp		ON (pp.proid = pro.proid)
        LEFT JOIN monitora.termocooperacao tc       ON (tc.tcpid = pro.tcpid)
        LEFT JOIN public.unidadegestora unc         ON (unc.ungcod = tc.ungcodconcedente)
        WHERE pro.statusvalorzerado = 't'
        AND pro.tcpid = ".$_SESSION['elabrev']['tcpid']."
        AND pp.ppanumcancelanc is null
        ORDER BY lote, pro.proanoreferencia DESC, crdmesliberacao
    ";
    //ver($sql,d);
    $enviarPagamento = false;
    $dados = !empty($_SESSION['elabrev']['tcpid']) ? $db->carregar($sql) : array();

    if ($dados[0] != '') {

        $arAnosPrevisao = array();
        $arLote = array();
        $totalPrevisao = count($dados)-1;
        //ver($dados, d);

        foreach ($dados as $k => $dado) {


            ?>
            <tr id="tr_<?=$dado['proid']?>">

                <td align="center"></td>

                <td align="center">
                    <img border="0" id="<?=$k ?>" title="Excluir" src="/imagens/excluir_01.gif">
                    <input type="hidden" name="linha[]" value="<?=$dado['proid'] ?>"/>
                </td>

                <td align="center" id="td_anoref_<?=$dado['proid']; //$k ?>" width="14%">
                    <?php echo $dado['proanoreferencia'] ? $dado['proanoreferencia'] : '-'; ?>
                    <input type="hidden" name="proanoreferencia[]" id="proanoreferencia_<?php echo $dado['proid']; ?>" value="<?=$dado['proanoreferencia'] ?>"/>
                </td>

                <td align="center" id="td_acao_<?=$k ?>">
                    <?=$dado['acacod'] ?>
                </td>

                <td align="center" id="td_prg_<?=$k ?>">
                    <input type="hidden" name="ptrid[]" value="<?=$dado['ptrid']?>">
                    <?=$dado['ptrid_descricao']?$dado['ptrid_descricao']:'-'?>
                </td>

                <td align="center" id="td_pi_<?=$dado['proid']; //$k ?>">
                    <input type="hidden" name="pliid[]" value="<?=$dado['pliid']?>">
                    <?=$dado['pliid_descricao']?$dado['pliid_descricao']:'-'?>
                </td>

                <td align="center" id="td_acaodsc_<?=$dado['proid']; ?>">
                    <?=$dado['acatitulo']?$dado['acatitulo']:'-'?>
                </td>

                <td align="center">
                    <input type="hidden" name="ndpid[]" value="<?=$dado['ndpid']?>">
                    <?=$dado['ndp_descricao']?$dado['ndp_descricao']:'-'?>
                </td>

                <td align="center">
                    <input type="hidden" name="provalor[]" value="<?=$dado['provalor']?>">
                    <?=$dado['provalor']?>
                </td>

                <td align="center"  width="50%">
                    <input type="hidden" name="crdmesliberacao[]" id="'crdmesliberacao[]" value="<?=$dado['crdmesliberacao']?>">
                    <?php echo $dado['crdmesliberacao'] ? mes_extenso($dado['crdmesliberacao']) : '-';?>
                </td>

                <td align="center">
                    <input type="hidden" name="crdmesexecucao[]" id="'crdmesexecucao[]" value="<?=$dado['crdmesexecucao']?>">
                    <?php echo $dado['crdmesexecucao'].' Mês(s)' ?>
                </td>
            </tr>
            <?php
        }
    }
    ?>
</table>