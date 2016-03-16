<?php

function getDadosOrcamentariosPorAcao($acaid) {
    global $db;

//    $html = "<link rel='stylesheet' type='text/css' href='../includes/Estilo.css'/><link rel="stylesheet" type="text/css" href="../includes/listagem.css"/>*/";
    $html = '<style>.modal-dialog{width:80%;};</style><table class="table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center"><tr><th width="150">&nbsp;</th>';

    $labels = array('Despesas Empenhadas:', 'Valores Pagos:', 'RAP não-Proc. Pagos:', 'RAP Processados Pagos:');
    $keys = array('empenho', 'pagamento', 'RapNPPago', 'rp_processado_pago');
    $years = array('2011', '2012', '2013', '2014');
    $acao = $subacao = $pis = $pos = $totalH = $totalL = $resultSet = array();

    if (!empty($acaid)) {
        foreach ($years as $year) {
            $strSQL = "SELECT v.vacid, vaeid, v.acaid, exercicio, v.vaetituloorcamentario, ve.vaedescricao
                FROM planacomorc.vinculacaoacaoestrategicaexercicio ve
                    join planacomorc.vinculacaoacaoestrategica v using(vacid)
                    join painel.acao a on a.acaid = v.acaid
                WHERE exercicio = {$year}
                and a.acaid = {$acaid}";
            $resultSet[] = $db->pegaLinha($strSQL);
        }
    }

    foreach ($resultSet as $k => $result) {
        if (!empty($result['vaeid'])) {
            array_push($acao, resultado_soma_acoes($result['vaeid'], $years[$k]));
            array_push($subacao, carrega_soma_subacoes($result['vaeid'], $years[$k]));
            array_push($pis, carrega_soma_pi($result['vaeid'], $years[$k]));
            array_push($pos, carrega_soma_ptres($result['vaeid'], $years[$k]));
        }
    }

    foreach ($keys as $k) {
        foreach ($years as $i => $year) {
            $totalH[$k][$year] = $acao[$i]['total'][$k] + $subacao[$i]['total'][$k] + $pis[$i]['total'][$k] + $pos[$i]['total'][$k];
        }
    }

    $i = 0;

    foreach ($years as $k => $year) :
        $html .= '<th width="150" title="' . $resultSet[$k]['vaedescricao'] . '">'.$year.'</th>';
    endforeach;
    $html .= '</tr>';
    foreach ($totalH as $columnArray) :
        $html .= '<tr><td width = "150"><strong>' . $labels[$i] . '</strong></td>';
        $k = 0;
        foreach ($columnArray as $column) :
            $html .= '<td align = "right" title = "' . $resultSet[$k]['vaedescricao'] . '">' . number_format($column, 2, ',', '.') . '</td>';
            $k++;
        endforeach;
        $html .= '</tr>';
        $i++;
    endforeach;
    $html .= '</table>';

    return array('tabela'=>$html, 'total'=>$totalH);
}

function carrega_soma_ptres($vaeid, $ano) {
    global $db;
    $rs_ptres = $db->carregar("SELECT ptres FROM planacomorc.vinculacaoestrategicapos WHERE vaeid={$vaeid}");

    if ($rs_ptres) {
        $i = 0;
        foreach ($rs_ptres as $row) {
            $ptres[$i] = $row['ptres'];
            $i++;
        }
        /// ################ Só falta passar o ano CERTO
        $rs = calcula_execucao_vinculcacao($ano, $acoes, $unidades, $subacaoes, $pis, $ptres);

        $table = array();
        if ($rs) {
            criaTabelinhaExecucao($rs, $table);
            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }
    return $table;
}

function carrega_soma_pi($vaeid, $ano) {
    global $db;
    $sqlFindPi = $db->carregar("SELECT plicod FROM planacomorc.vinculcaoestrategicapis WHERE vaeid={$vaeid}");

    if ($sqlFindPi) {
        $i = 0;
        foreach ($sqlFindPi as $row) {
            $pis[$i] = $row['plicod'];
            $i++;
        }
        /// ################ Só falta passar o ano CERTO
        $rs = calcula_execucao_vinculcacao($ano, $acoes, $unidades, $subacaoes, $pis, $ptres);
        $table = array();

        if ($rs) {
            criaTabelinhaExecucao($rs, $table);
            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }
    return $table;
}

function carrega_soma_subacoes($vaeid, $ano) {
    global $db;
//Unidades
    $rsUnicod = $db->carregar("select distinct unicod from planacomorc.vinculacaoestrategicasubacoes where vaeid = {$vaeid}");
    if ($rsUnicod) {
        $i = 0;
        foreach ($rsUnicod as $row) {
            $unicods[$i] = $row['unicod'];
            $i++;
        }
    }

//Subacoes
    $rsSubacoes = $db->carregar("select distinct sbacod from planacomorc.vinculacaoestrategicasubacoes where vaeid = {$vaeid}");
    if ($rsSubacoes) {
        $i = 0;
        foreach ($rsSubacoes as $row) {
            $subacaoes[$i] = $row['sbacod'];
            $i++;
        }
    }

    if (!empty($unicods) && !empty($subacaoes)) {

        $rs = calcula_execucao_vinculcacao($ano, $acoes, $unicods, $subacaoes, $pis, $ptres);

        $table = array();
        if ($rs) {
            criaTabelinhaExecucao($rs, $table);
            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }
    return $table;
}

function resultado_soma_acoes($vaeid, $ano) {
    global $db;

    $sql = "SELECT DISTINCT acacod FROM planacomorc.vinculacaoestrategicaacoes WHERE vaeid = {$vaeid}";
    $acoes = $db->carregar($sql);
    if ($acoes) {
        $sqlFinanceira = "SELECT * FROM dblink(
                            'dbname= hostaddr= user= password= port=',
                            'SELECT sum(Empenho), sum(Pagamento), sum(rp_processado_pago) rp_processado_pago, sum(rp_nao_processado_pago_ate_2012), sum(rp_nao_processado_pago_apos_2012) FROM (";
        foreach ($acoes as $acaoatual) {

            $unidadesdaacao = "SELECT DISTINCT unicod FROM planacomorc.vinculacaoestrategicaacoes WHERE acacod = '{$acaoatual['acacod']}' and vaeid = {$vaeid}";
            $sqlunidades = $db->carregar($unidadesdaacao);

            $unidades = '';
            foreach ($sqlunidades as $unidadeatual) {
                $unidades .= "''{$unidadeatual['unicod']}'',";
            }
            $unidades = substr($unidades, 0, -1);


            $sqlFinanceira.= "SELECT
    acacod      AS cod_agrupador1,
    acadsc      AS dsc_agrupador1,
    unicod      AS cod_agrupador2,
    SUM(valor1) AS empenho,
    SUM(valor2) AS pagamento,
    SUM(valor3) AS rp_processado_pago,
    SUM(valor4) AS rp_nao_processado_pago_ate_2012,
    SUM(valor5) AS rp_nao_processado_pago_apos_2012
FROM
    (
        SELECT
            sld.acacod,
            aca.acadsc,
            sld.unicod,
            CASE
                WHEN sld.sldcontacontabil IN (''292130100'',
                                              ''292130201'',
                                              ''292130202'',
                                              ''292130203'',
                                              ''292130301'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor1,
            CASE
                WHEN sld.sldcontacontabil IN (''292130301'',
                                              ''292410403'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor2,
            CASE
                WHEN sld.sldcontacontabil IN (''295210201'',
                                              ''295210202'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor3,
            CASE
                WHEN sld.sldcontacontabil IN (''295110300'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor4,
            CASE
                WHEN sld.sldcontacontabil IN (''295110301'',
                                              ''295110302'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor5
        FROM
            dw.saldo$ano sld
        LEFT JOIN
            dw.acao aca
        ON
            aca.acacod = sld.acacod

                                    WHERE sld.unicod in ($unidades) AND sld.acacod in (''{$acaoatual['acacod']}'') AND sld.sldcontacontabil in (''292130100'',''292130201'',''292130202'',''292130203'',''292130301'',''292130301'',''292410403'',''295110300'',
                                    ''295110301'',            ''295110302''           ,''295210201'')) as foo
                                 WHERE

                                 valor1 <> 0
OR  valor2 <> 0
OR  valor3 <> 0
OR  valor4 <> 0
OR  valor5 <> 0
GROUP BY
    acacod,
    acadsc,
    unicod

                                  union ";
        }
        $sqlFinanceira = substr($sqlFinanceira, 0, -6);
        $sqlFinanceira.= ") buscafinanceira;'  )
                        AS aca
                        (
                            Empenho NUMERIC(15,2),
                            Pagamento NUMERIC(15,2),
                            rp_processado_pago NUMERIC(15,2),
                rp_nao_processado_pago_ate_2012 NUMERIC(15,2),
                rp_nao_processado_pago_apos_2012 NUMERIC(15,2)

                        )";

        $table = array();
        $rs = $db->carregar($sqlFinanceira);
        if ($rs) {

            $rapnprocessadopago = ($ano < 2013) ? $rs[0]["rp_nao_processado_pago_ate_2012"] : $rs[0]["rp_nao_processado_pago_apos_2012"];

            $rs[0]['RapNPPago'] = $rapnprocessadopago;

            $table['html'] = '<table class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">';
            $table['html'].= '<tr>';
            $table['html'].= '<td><span class="red">Despesas Empenhadas:</span> <span class="bold">' . number_format($rs[0]['empenho'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">Valores Pagos:</span> <span class="bold">' . number_format($rs[0]['pagamento'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['rp_processado_pago'], 2, ',', '.') . '</span></td>';
            $table['html'].= '<td><span class="red">RAP Processados Pagos:</span> <span class="bold">' . number_format($rapnprocessadopago, 2, ',', '.') . '</span></td>';
            $table['html'].= '</tr>';
            $table['html'].= '</table>';

            $table['total'] = current($rs);
        } else {
            $table['html'] = '';
        }
    } else {
        $table['html'] = '';
    }

    return $table;
}

function criaTabelinhaExecucao(&$rs, &$table) {
    $table['html'] = '<table class="tabela table table-striped table-bordered table-hover" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" align="center">';
    $table['html'].= '<tr>';
    $table['html'].= '<td><span class="red">Despesas Empenhadas:</span> <span class="bold">' . number_format($rs[0]['empenho'], 2, ',', '.') . '</span></td>';
    $table['html'].= '<td><span class="red">Valores Pagos:</span> <span class="bold">' . number_format($rs[0]['pagamento'], 2, ',', '.') . '</span></td>';
    $table['html'].= '<td><span class="red">RAP Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['rp_processado_pago'], 2, ',', '.') . '</span></td>';
    $table['html'].= '<td><span class="red">RAP não-Processados Pagos:</span> <span class="bold">' . number_format($rs[0]['RapNPPago'], 2, ',', '.') . '</span></td>';
    $table['html'].= '</tr>';
    $table['html'].= '</table>';
}

function calcula_execucao_vinculcacao($ano, $acoes, $unidades, $subacaoes, $pis, $ptres) {
    global $db;
    $filtros = "";

    if (!empty($acoes)) {
        $filtros = " and sld.acacod IN ( ''" . implode("'',''", $acoes) . "'') ";
    }
    if (!empty($unidades)) {
        $filtros .= " and sld.unicod IN ( ''" . implode("'',''", $unidades) . "'') ";
    }
    if (!empty($subacaoes)) {
        $filtros .= " and substr(sld.plicod, 2, 4) IN ( ''" . implode("'',''", $subacaoes) . "'') ";
    }
    if (!empty($pis)) {
        $filtros .= " and sld.plicod IN ( ''" . implode("'',''", $pis) . "'') ";
    }
    if (!empty($ptres)) {
        $filtros .= " and sld.ptres IN ( ''" . implode("'',''", $ptres) . "'') ";
    }


    if (!empty($filtros)) {
        $strSQL = "SELECT * FROM dblink(
            'dbname= hostaddr= user= password= port=',
            'Select
	SUM(empenhado) AS empenhado,
    SUM(pago) AS pago,
    SUM(rp_processado_pago) AS rp_processado_pago,
    SUM(rp_nao_processado_pago_ate_2012) AS rp_nao_processado_pago_ate_2012,
    SUM(rp_nao_processado_pago_apos_2012) AS rp_nao_processado_pago_apos_2012
from
(

SELECT
    acacod      AS cod_agrupador1,
    acadsc      AS dsc_agrupador1,
    unicod      AS cod_agrupador2,
    SUM(valor1) AS empenhado,
    SUM(valor2) AS pago,
    SUM(valor3) AS rp_processado_pago,
    SUM(valor4) AS rp_nao_processado_pago_ate_2012,
    SUM(valor5) AS rp_nao_processado_pago_apos_2012
FROM
    (
        SELECT
            sld.acacod,
            aca.acadsc,
            sld.unicod,
            CASE
                WHEN sld.sldcontacontabil IN (''292130100'',
                                              ''292130201'',
                                              ''292130202'',
                                              ''292130203'',
                                              ''292130301'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor1,
            CASE
                WHEN sld.sldcontacontabil IN (''292130301'',
                                              ''292410403'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor2,
            CASE
                WHEN sld.sldcontacontabil IN (''295210201'',
                                              ''295210202'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor3,
            CASE
                WHEN sld.sldcontacontabil IN (''295110300'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor4,
            CASE
                WHEN sld.sldcontacontabil IN (''295110301'',
                                              ''295110302'')
                THEN
                    CASE
                        WHEN sld.ungcod=''154004''
                        THEN (sld.sldvalor)*2.3274
                        ELSE (sld.sldvalor)
                    END
                ELSE 0
            END AS valor5
        FROM
            dw.saldo$ano sld
        LEFT JOIN
            dw.acao aca
        ON
            aca.acacod = sld.acacod
        WHERE true
        $filtros

        AND sld.sldcontacontabil IN (''292130100'',
                                     ''292130201'',
                                     ''292130202'',
                                     ''292130203'',
                                     ''292130301'',
                                     ''292130301'',
                                     ''292410403'',
                                     ''295210201'',
                                     ''295210202'',
                                     ''295110300'',
                                     ''295110301'',
                                     ''295110302'')) AS foo
WHERE
    valor1 <> 0
OR  valor2 <> 0
OR  valor3 <> 0
OR  valor4 <> 0
OR  valor5 <> 0
GROUP BY
    acacod,
    acadsc,
    unicod
ORDER BY
    acacod,
    acadsc,
    unicod

) x; ' )
            AS sba
            (
                empenho NUMERIC(15,2),
                pagamento NUMERIC(15,2),
                rp_processado_pago NUMERIC(15,2),
                rp_nao_processado_pago_ate_2012 NUMERIC(15,2),
                rp_nao_processado_pago_apos_2012 NUMERIC(15,2)
            )";


        $rs = $db->carregar($strSQL);

        if ($rs) {
            if ($ano < 2013)
                $rs[0]["RapNPPago"] = $rs[0]["rp_nao_processado_pago_ate_2012"];
            else
                $rs[0]["RapNPPago"] = $rs[0]["rp_nao_processado_pago_apos_2012"];
            //print_r($rs);
            return $rs;
        }
    }
}
