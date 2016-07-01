<?php
$ptrid = $_REQUEST['ptrid'];
if (isset($_REQUEST['ptrid'])) {
    $ptres = $db->pegaUm("SELECT ptres FROM monitora.ptres WHERE ptrid = {$_REQUEST['ptrid']}");
    $sql = <<<DML
        SELECT
            dtl.ptres,
            trim(aca.prgcod || '.' || aca.acacod || '.' || aca.unicod || '.' || aca.loccod || ' - ' || aca.acadsc) AS descricao,
            uni.unidsc,
            dtl.plocod,
            plo.plotitulo,
            COALESCE(SUM(DISTINCT dtl.ptrdotacao)+0.00, 0.00)                   AS dotacaoatual,
            COALESCE(SUM(dt.valor), 0.00)                                       AS detalhadosubacao,
            COALESCE((pemp.total), 0.00)                                        AS empenhado,
            COALESCE(SUM(DISTINCT dtl.ptrdotacao) - COALESCE(SUM(dt.valor), 0.00), 0.00) AS naodetalhadosubacao,
            COALESCE(SUM(DISTINCT dtl.ptrdotacao) - COALESCE(pemp.total, 0.00), 0.00) AS naoempenhado
        FROM monitora.acao aca
        INNER JOIN monitora.ptres dtl ON aca.acaid = dtl.acaid
        INNER JOIN public.unidade uni ON uni.unicod = dtl.unicod
        LEFT JOIN
            (
                SELECT
                    ptrid,
                    SUM(sadvalor) AS valor
                FROM
                    monitora.pi_subacaodotacao
                GROUP BY
                    ptrid) dt
            ON dtl.ptrid = dt.ptrid
        LEFT JOIN
            (
                SELECT
                    ptrid,
                    SUM(dtl.valorpi) AS valorpi
                FROM
                    monitora.v_pi_detalhepiptres dtl
                GROUP BY
                    dtl.ptrid) dt2
            ON dtl.ptrid = dt2.ptrid
        LEFT JOIN siafi.ptrempenho pemp ON (pemp.ptres = dtl.ptres AND pemp.exercicio = aca.prgano)
        LEFT JOIN monitora.planoorcamentario plo ON plo.prgcod = aca.prgcod
            AND plo.acacod = aca.acacod
            AND plo.unicod = aca.unicod
            AND dtl.plocod = plo.plocodigo
        WHERE 
            aca.prgano ='{$_SESSION['exercicio']}'
            AND dtl.ptrano='{$_SESSION['exercicio']}'
            AND dtl.ptres = '{$ptres}'
            AND plo.exercicio = '{$_SESSION['exercicio']}'
            AND aca.acasnrap = FALSE
        GROUP BY
            dtl.ptrid,
            dtl.ptres,
            descricao,
            uni.unidsc,
            pemp.total,
            dtl.plocod,
            plo.plotitulo
DML;
//ver($sql);
    $dadosPtres = $db->carregar($sql);
    $dadosPtres = $dadosPtres[0];
}
?>
<table class="table table-bordered table-striped table-condensed table-hover" align="center" cellspacing="1" cellpadding="3">
    <tr>
        <th width="200px">PTRES:</th>
        <td><b style="font-size:14px"><?php echo $dadosPtres['ptres']; ?></b></td>
    </tr>
    <tr>
        <th>PO:</th>
        <td><?php echo $dadosPtres['plocod'] . ' - ' . $dadosPtres['plotitulo']; ?></td>
    </tr>
    <tr>
        <th>Unidade:</th>
        <td><?php echo $dadosPtres['unidsc']; ?></td>
    </tr>
    <tr>
        <th width="200px">Dotação atual:</th>
        <td><b><?php echo number_format2($dadosPtres['dotacaoatual']); ?></b></td>
    </tr>
    <tr>
        <th width="200px">Detalhado em Subação:</th>
        <td style="color:<?php echo(($dadosPtres['detalhadosubacao'] >= 0) ? 'black' : 'red'); ?>">
            <b>
                <span id="saldo_pi_detalhado_ptres">
                    <?php echo number_format2($dadosPtres['detalhadosubacao']); ?>
                </span>
            </b>
        </td>
    </tr>
    <tr>
        <th width="200px">Não Detalhado em Subação:</th>
        <td style="color:<?php echo(($dadosPtres['naodetalhadosubacao'] >= 0) ? 'black' : 'red'); ?>">
            <b>
                <?php echo number_format2($dadosPtres['naodetalhadosubacao']); ?>
            </b>
        </td>
    </tr>
    <tr>
        <th width="200px">Empenhado do PTRES:</th>
        <td>
            <b>
                <span id="saldo_nao_orcado">
                    <?php echo number_format2($dadosPtres['empenhado']); ?>
                </span>
            </b>
        </td>
    </tr>
    <tr>
        <th class="SubTituloDireita" width="200px">Não Empenhado:</th>
        <td style="color:<?php echo(($dadosPtres['naoempenhado'] >= 0) ? 'black' : 'red'); ?>">
            <b>
                <?php echo number_format2($dadosPtres['naoempenhado']); ?>
            </b>
        </td>
    </tr>
    <tr>
        <td colspan="2">
            <div style="background-color:#00ced1; color:#FFF; margin-top: 10px; padding: 5px;">
                <b>
                    Subações <?php echo $_SESSION['exercicio']; ?>
                </b>
            </div>
            <?php
            $cabecalho = array(
                "Código",
                "Subação",
                "Orçamento total da Subação (R$)"
            );
            $cabecalho[] = "Orçamento da Subação neste PTRES (R$)";
            $cabecalho[] = "Empenhado nesta Subação e neste PTRES(R$)";
            
            $select = <<<SELECT
                SELECT  
                    foo.codigo AS sbacod,
                    foo.sbatitulo,
                    COALESCE(foo.dotacao, 0.00) AS dotacao,
                    COALESCE((
                        SELECT
                            sadvalor
                        FROM monitora.pi_subacaodotacao
                        WHERE sbaid = foo.sbaid
                            AND ptrid = {$ptrid} )
                    ,0.00)  as dotacao_no_ptres,
                    COALESCE((
                        SELECT
                            SUM(total)
                        FROM siafi.pliptrempenho
                        WHERE exercicio = '{$_SESSION['exercicio']}'
                            AND SUBSTR(plicod, 2,4) = foo.sbacod
                            AND ptres = '{$dadosPtres['ptres']}'        
                    ), '0.00') AS empenhado_no_ptres
SELECT;
            $groupby = array(
                'foo.codigo',
                'foo.sbatitulo',
                'foo.dotacao',
                'foo.sbaid',
                'foo.empenhado',
                'foo.sbacod'
            );
            $orderby = array('1');
            $where = "AND ptr.ptrid = {$ptrid}";
            $sql = retornaConsultaSubacao(
                array(
                    'SELECT' => $select,
                    'groupby' => $groupby,
                    'orderby' => $orderby,
                    'where' => $where
                )
            );
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setQuery($sql);
            $listagem->setCabecalho($cabecalho);
            $listagem->addCallbackDeCampo(array('dotacao','dotacao_no_ptres','empenhado_no_ptres'), 'mascaraMoeda');
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            //$db->monta_lista($sql, $cabecalho, 50, 5, 'S', '100%', 'S');
            ?>

            <div style="background-color:#FF6347; color:#FFF; margin-top: 10px; padding: 5px;">
                <b>
                    Plano Interno <?php echo $_SESSION['exercicio']; ?>
                </b>
            </div>
            <?php
            $cabecalho = array(
                "PI's subtraindo recursos<br />do PTRES",
                "Título do PI",
                "Empenhado do PI no PTRES (R$)"
            );
            $params['SELECT'] = <<<SQL
                SELECT 
                    gmb.codigo,
                    gmb.titulo,
                    COALESCE((
                        SELECT
                            total
                        FROM siafi.pliptrempenho ppe
                        WHERE plicod = gmb.codigo
                        AND ptres = '{$dadosPtres['ptres']}' AND exercicio = '{$_SESSION['exercicio']}')
                    ,0.00) as empenhado_pi_ptres
SQL;
            /* Variáveis da Consulta */
            $params['v_ptrid'] = $ptrid;
            /* Filtros da Consulta */
            $params['where'] = " AND pip.ptrid= {$ptrid} ";
            $params['filtroNoUnionSiafi'] = " AND ptr.ptrid = {$ptrid} ";

            $sql = retornaConsultaPI($params);
            #ver($sql,d);
            $listagem = new Simec_Listagem(Simec_Listagem::RELATORIO_CORRIDO);
            $listagem->setQuery($sql);
            $listagem->setCabecalho($cabecalho);
            $listagem->addCallbackDeCampo(array('empenhado_pi_ptres'), 'mascaraMoeda');
            $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
            //$db->monta_lista($sql, $cabecalho, 50, 5, 'S', '100%');
            ?>
        </td>
    </tr>
</table>
<? exit();
