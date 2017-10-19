<?php

    if(isset($_REQUEST['ptrid'])) {
        $ptres = $db->pegaUm("SELECT ptres FROM monitora.ptres WHERE ptrid = {$_REQUEST['ptrid']}");
        $sql = "
            SELECT
                ptr.ptres,
                trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc) AS descricao,
                uni.unicod,
                uni.unidsc,
                ptr.plocod,
                plo.plotitulo,
                COALESCE(SUM(DISTINCT ptr.ptrdotacao)+0.00, 0.00)                            AS dotacaoatual,
                COALESCE(SUM(dt.valor), 0.00)                                       AS detalhadosubacao,
                COALESCE((SUM(pemp.total)), 0.00)                                        AS empenhado,
                COALESCE(SUM(DISTINCT ptr.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS naodetalhadosubacao,
                COALESCE(SUM(DISTINCT ptr.ptrdotacao) - COALESCE(pemp.total, 0.00), 0.00) AS naoempenhado
            FROM monitora.acao aca
                JOIN monitora.ptres ptr ON aca.acaid = ptr.acaid
                JOIN public.unidade uni ON uni.unicod = ptr.unicod
                LEFT JOIN(
                    SELECT
                        ptrid,
                        SUM(sadvalor) AS valor
                    FROM monitora.pi_subacaodotacao
                    GROUP BY ptrid) dt ON ptr.ptrid = dt.ptrid
                LEFT JOIN siafi.uo_ptrempenho pemp ON(
                    pemp.ptres = ptr.ptres
                    AND pemp.exercicio = aca.prgano
                    AND pemp.unicod = aca.unicod)
                LEFT JOIN monitora.planoorcamentario plo ON(
                    plo.prgcod = aca.prgcod
                    AND plo.acacod = aca.acacod
                    AND plo.unicod = aca.unicod
                    AND ptr.plocod = plo.plocodigo
                )
            WHERE
                aca.prgano ='{$_SESSION['exercicio']}'
                AND ptr.ptrano='{$_SESSION['exercicio']}'
                AND ptr.ptres = '{$ptres}'
                AND plo.exercicio = '{$_SESSION['exercicio']}'
                AND aca.acasnrap = FALSE
            GROUP BY
                ptr.ptrid,
                ptr.ptres,
                descricao,
                uni.unicod,
                uni.unidsc,
                pemp.total,
                ptr.plocod,
                plo.plotitulo
        ";

        $dadosPtres = $db->carregar($sql);
        $dadosPtres = $dadosPtres[0];
        if(!isset($dadosPtres['dotacaoatual']) || $dadosPtres['dotacaoatual'] == '' ){
            $dadosPtres['dotacaoatual'] = 0.00;
        }
        /* Monta o HTML do grafico de Execução */
        $sqlGrafico = "
            SELECT
                'Dotação Atual PO' AS descricao,
                'Total' as categoria,
                {$dadosPtres['dotacaoatual']} AS valor

            UNION ALL
            SELECT
                'Empenhado'  AS descricao,
                'Total' as categoria,
                vlrempenhado AS valor
            FROM
                (
                    SELECT
                        SUM(vlrempenhado) AS vlrempenhado
                    FROM
                        spo.siopexecucao sex
                    WHERE
                        sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                    GROUP BY
                        ptres ) foo
            UNION ALL
            SELECT
                'Liquidado'  AS descricao,
                'Total' as categoria,
                vlrliquidado AS valor
            FROM
                (
                    SELECT
                        SUM(vlrliquidado) AS vlrliquidado
                    FROM
                        spo.siopexecucao sex
                    WHERE
                        sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                    GROUP BY
                        ptres ) foo
            UNION ALL
            SELECT
                'Pago'  AS descricao,
                'Total' as categoria,
                vlrpago AS valor
            FROM
                (
                    SELECT
                        SUM(vlrpago) AS vlrpago
                    FROM
                        spo.siopexecucao sex
                    WHERE
                        sex.exercicio = '{$_SESSION['exercicio']}'
                    AND ptres IN ('{$ptres}')
                    GROUP BY
                        ptres ) foo";

      $grafico = new Grafico(null, false);

    }
    $html = '<style>.modal-dialog{width:80%;};</style>

        <div style="width: 50%; float: left;">
    <table class="table table-striped table-bordered table-hover" align="center" cellspacing="1" cellpadding="3">
    <tr>
        <td class="SubTituloDireita" width="200px">PTRES:</td>
        <td><b style="font-size:14px">' . $dadosPtres['ptres'] . '</b></td>
    </tr>
    <tr>
        <td class="SubTituloDireita">PO:</td>
        <td>' . $dadosPtres['plocod'] . ' - ' . $dadosPtres['plotitulo'] . '</td>
    </tr>
    <tr>
        <td class="SubTituloDireita">Unidade:</td>
        <td>' . $dadosPtres['unicod'] . ' - ' . $dadosPtres['unidsc'] . '</td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="200px">Dotação atual:</td>
        <td><b>' . number_format2($dadosPtres['dotacaoatual']) . '</b></td>
    </tr>
    ';

    if ($dadosPtres['naodetalhadosubacao'] >= 0) {
        $html .= 'black';
    } else {
        $html .= 'red';
    }

    $html .='
    <tr>
        <td class="SubTituloDireita" width="200px">Empenhado do PTRES:</td>
        <td>
            <b>
                <span id="saldo_nao_orcado">
                    ' . number_format2($dadosPtres['empenhado']) . '
                </span>
            </b>
        </td>
    </tr>
    <tr>
        <td class="SubTituloDireita" width="200px">Não Empenhado:</td>
        <td style="color:';
    
    if ($dadosPtres['naoempenhado'] >= 0) {
        $html .= 'black';
    } else {
        $html .= 'red';
    }

        $html .='">
            <b>
                        <span id="saldo_nao_empenhado">
                ' . number_format2($dadosPtres['naoempenhado']) . '
                    </span>
            </b>
        </td>
    </tr>
    </table>
    </div>
    <div id="graficoDestino" style="width: 50%; float: left;">
        '.$grafico->setTipo(Grafico::K_TIPO_COLUNA)->setId('graficoPtres')->setTitulo('Execução Orçamentária')->gerarGrafico($sqlGrafico).'
    </div>
    <div style="clear: both;"/>
    <table class="table table-striped table-bordered table-hover">';

    $html.='
        <tr>
            <td>
              <div style="background-color:#ff6347; color:#FFF; margin-top: 10px; padding: 5px;">
                <b>
                    PIs subtraindo recursos do PTRES - ' . $_SESSION['exercicio'] . ' que possuem valores empenhados:
                </b>
            </div>';
    
    # Consulta  de PI
    $sqlPi = "
        SELECT
            ppe.plicod,
            uni.unicod || ' - ' || uni.unidsc AS unidsc,
            SUM(ppe.total) AS empenhado
        FROM monitora.pi_planointerno pli
            JOIN siafi.uo_pliptrempenho ppe ON(
                pli.plicod = ppe.plicod
                AND pli.unicod = ppe.unicod
                AND pli.pliano = '{$_SESSION['exercicio']}')
            JOIN public.unidade uni ON pli.unicod = uni.unicod
        WHERE
            exercicio = '{$_SESSION['exercicio']}'
            AND ppe.plicod <> ''
            AND pli.pliid IN(
                SELECT
                    pliid
                FROM monitora.pi_planointernoptres
                WHERE ptrid = {$_REQUEST['ptrid']}
            )
        GROUP BY
            ppe.plicod,
            uni.unicod || ' - ' || uni.unidsc
    ";
#ver($sqlPi,d);
    $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_PAGINADO, Simec_Listagem::RETORNO_BUFFERIZADO);
    $listagem->setCabecalho(array(
        "PI's subtraindo recursos<br />do PTRES",
        "Título do PI",
        "Empenhado do PI no PTRES (R$)"
        ));
    $listagem->setQuery($sqlPi);
    $listagem->addCallbackDeCampo(array('empenhado'), 'mascaraMoeda');
    $listagem->setTotalizador(Simec_Listagem::TOTAL_QTD_REGISTROS);
    $retornoPi = $listagem->render();
    $html .= $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    $html .='</td>
    </tr>

</table>';

    # Chamada do componente de Grafico
    $html.='<script>jQuery("#graficoPtres").appendTo("#graficoDestino");</script>';

    echo $html;
