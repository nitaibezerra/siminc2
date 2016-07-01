<?php

class Siafi_Service_DadosExecucaoFinanceira extends Spo_Service_Abstract {
    /*
     * Método que retorna a listagem das OBs.
     * Parametros de requisição:
     *      credencial (*), paginacao (*), exercicio (*), unicod, ungcod, obcod, cpfcnpjfavorecido
     * Retorno:
     *      paginacao, datatransacao, unicod, unidsc, ungcod, ungdsc, obcod, cpfcnpjfavorecido, valortotal
     */

    public function retornarListaOb($dados) {

        switch ($_SESSION['exercicio']) {
            case 2015:
                /* Filtros */

                $where = "";
                if ($dados['datainicio'] != '') {
                    if ($dados['datafim'] == '') {
                        $dados['datafim'] = strftime('%d/%m/%Y', strtotime('today'));
                    }
                    $dados['datainicio'] = formata_data_sql($dados['datainicio']);
                    $dados['datafim'] = formata_data_sql($dados['datafim']);
                    $where .= " o15.itdatransacao BETWEEN '{$dados['datainicio']}' AND '{$dados['datafim']}'";
                }
//                if($dados['unicod']!=''){
//                    $where .=  " AND ung.orgcod = '{$dados['unicod']}' ";
//                }
                if ($dados['ungcod'] != '') {
                    $where .= " AND o15.itcougoperador = '{$dados['ungcod']}' ";
                }
                if ($dados['obcod'] != '') {
                    $where .= " AND o15.gruggestaoannumeroobuq LIKE '%{$dados['obcod']}%' ";
                }
                if ($dados['cpfcnpjfavorecido'] != '') {
                    $dados['cpfcnpjfavorecido'] = str_replace(array('.', '/', '-'), '', $dados['cpfcnpjfavorecido']);
                    $where .= " AND o15.itcofavorecido = '{$dados['cpfcnpjfavorecido']}' ";
                }
                if ($where != "") {
                    $where = " WHERE {$where}";
                }

                $sql = "
                SELECT
                    o15.gruggestaoannumeroobuq AS codigo,
                    TO_CHAR(o15.itdatransacao, 'dd/mm/yyyy') AS datatransacao,
                    ung.orgcod                 AS unicod,
                    uni.unidsc                 AS unidsc,
                    o15.itcougoperador         AS ungcod,
                    ung.ungdsc                 AS ungdsc,
                    o15.gruggestaoannumeroobuq AS codob,
                    o15.itcofavorecido         AS cpfcnpjfavorecido,
                    o15.itvaeventosistema      AS valortotal
                FROM
                    siafi2015.ob o15
                JOIN
                    dw.ug ung
                ON
                    (
                        o15.itcougoperador::INTEGER = ung.ungcod::INTEGER )
                JOIN
                    dw.uo uni
                ON
                    (
                        ung.orgcod = uni.unicod)
                {$where} ORDER BY o15.itdatransacao DESC ";

                $pagina = $dados['paginacao']['pagina'];
                $limit = 100;
                $offset = ($pagina - 1) * $limit;
                if ($offset < 0)
                    $offset = 0;
                $resultado = array();
                $resultado['dados'] = adapterConnection::siafi()->carregar("{$sql} OFFSET {$offset} LIMIT {$limit}");
                $resultado['query'] = $sql;
                $resultado['registros'] = adapterConnection::siafi()->pegaUm("SELECT COUNT(1) AS total FROM ({$sql}) registros");
                $resultado['pagina'] = $pagina;
                break;
            default:
                $resultado = array();
                break;
        }


        return $resultado;
    }

    /*
     * Método que retorna os dados detalhados de uma OB.
     * Parametros de requisição:
     *      credencial (*), exercicio (*), obcod (*)
     * Retorno:
     *      unicod, unidsc, ungcod, ungdsc, necod, obcod, cpfcnpjfavorecido,
     *      datatransacao, agenciaorigem, contaorigem, bancodestino, agenciadestino,
     *      contadestino, numprocesso, codrelatorio, rpcod, obs1, obs2, valortotal
     */

    public function retornarDetalheOb($dados) {

        switch ($_SESSION['exercicio']) {
            case 2015:
                $sql = "SELECT
                    ung.orgcod                    AS unicod,
                    uni.unidsc                    AS unidsc,
                    o15.itcougoperador            AS ungcod,
                    ung.ungdsc                    AS ungdsc,
                    o15.itcoinscricao_a_1         AS codne,
                    o15.gruggestaoannumeroobuq    AS codob,
                    TO_CHAR(o15.itdatransacao, 'dd/mm/yyyy') AS datatransacao,
                    o15.itcofavorecido            AS cpfcnpjfavorecido,
                    o15.itvaeventosistema         AS valortotal,
                    o15.itcoagenciaorigem         AS agenciaorigem,
                    o15.itnucontacorrenteorigem   AS contaorigem,
                    o15.itcobancodestino          AS bancodestino,
                    o15.itcoagenciadestino        AS agenciadestino,
                    o15.itnucontacorrentedestino  AS contadestino,
                    o15.itnuprocesso              AS numprocesso,
                    o15.itnuanorelatorio          AS codrelatorio,
                    o15.grannudocumentoreferencia AS codrp,
                    o15.ittxobservacaodocumento_1 AS obs1,
                    o15.ittxobservacaodocumento_2 AS obs2,
                    o15.itvaeventosistema         AS valortotal
                FROM
                    siafi2015.ob o15
                JOIN
                    dw.ug ung
                ON
                    (
                        o15.itcougoperador::INTEGER = ung.ungcod::INTEGER )
                JOIN
                    dw.uo uni
                ON
                    (
                        ung.orgcod = uni.unicod)
                WHERE
                    o15.gruggestaoannumeroobuq = '{$dados['obcod']}' LIMIT 1";
                $resultado = adapterConnection::siafi()->carregar($sql);
                return $resultado;
                break;
            default:
                return array();
                break;
        }
    }

    /*
     * Método que retorna a listagem das Nes.
     * Parametros de requisição:
     *      credencial (*), paginacao (*), exercicio (*), unicod, ungcod, necod, cpfcnpjfavorecido
     * Retorno:
     *      paginacao, datatransacao, unicod, unidsc, ungcod, ungdsc, necod, cpfcnpjfavorecido, valortotal
     */

    public function retornarListaNe($dados) {

        switch ($_SESSION['exercicio']) {
            case 2015:
                /* Filtros */

                $where = "";
                if ($dados['datainicio'] != '') {
                    if ($dados['datafim'] == '') {
                        $dados['datafim'] = strftime('%d/%m/%Y', strtotime('today'));
                    }
                    $dados['datainicio'] = formata_data_sql($dados['datainicio']);
                    $dados['datafim'] = formata_data_sql($dados['datafim']);
                    $where .= " n15.itdatransacao BETWEEN '{$dados['datainicio']}' AND '{$dados['datafim']}'";
                }
//                if($dados['unicod']!=''){
//                    $where .=  " AND ung.orgcod = '{$dados['unicod']}' ";
//                }
                if ($dados['ungcod'] != '') {
                    $where .= " AND n15.itcougoperador = '{$dados['ungcod']}' ";
                }
                if ($dados['necod'] != '') {
                    $where .= " AND n15.gruggestaoannumeroneuq LIKE '%{$dados['necod']}%' ";
                }
                if ($dados['cpfcnpjfavorecido'] != '') {
                    $dados['cpfcnpjfavorecido'] = str_replace(array('.', '/', '-'), '', $dados['cpfcnpjfavorecido']);
                    $where .= " AND n15.itcofavorecido = '{$dados['cpfcnpjfavorecido']}' ";
                }
                if ($where != "") {
                    $where = " WHERE {$where}";
                }

                $sql = "
                SELECT
                    n15.gruggestaoannumeroneuq                  AS codigo,
                    TO_CHAR(n15.itdatransacao, 'dd/mm/yyyy')    AS datatransacao,
                    ung.orgcod                                  AS unicod,
                    uni.unidsc                                  AS unidsc,
                    n15.itcougoperador                          AS ungcod,
                    ung.ungdsc                                  AS ungdsc,
                    n15.gruggestaoannumeroneuq                  AS necod,
                    n15.itcofavorecido                          AS cpfcnpjfavorecido,
                    n15.itvatransacao                           AS valortotal
                FROM
                    siafi2015.ne n15
                JOIN
                    dw.ug ung
                ON
                    (
                        n15.itcougoperador::INTEGER = ung.ungcod::INTEGER )
                JOIN
                    dw.uo uni
                ON
                    (
                        ung.orgcod = uni.unicod)
                {$where}
                ORDER BY n15.itdatransacao DESC";
                $pagina = $dados['paginacao']['pagina'];
                $limit = 100;
                $offset = ($pagina - 1) * $limit;
                if ($offset < 0)
                    $offset = 0;
                $resultado = array();
                $resultado['dados'] = adapterConnection::siafi()->carregar("{$sql} OFFSET {$offset} LIMIT {$limit}");
                $resultado['query'] = $sql;
                $resultado['registros'] = adapterConnection::siafi()->pegaUm("SELECT COUNT(1) AS total FROM ({$sql}) registros");
                $resultado['pagina'] = $pagina;

                break;
            default:
                $resultado = array();
                break;
        }


        return $resultado;
    }

    /*
     * Método que retorna os dados detalhados de uma NE.
     * Parametros de requisição:
     *      credencial (*), exercicio (*), necod (*)
     * Retorno:
     *      unicod, unidsc, ungcod, ungdsc, necod
     */

    public function retornarDetalheNe($dados) {
        #ver($dados,d);
        switch ($_SESSION['exercicio']) {
            case 2015:
                $sql = "SELECT
                        TO_CHAR(n15.itdatransacao, 'dd/mm/yyyy')    AS datatransacao,
                        n15.gruggestaoannumeroneuq    AS necod,
                        ung.orgcod                       AS unicod,
                        uni.unidsc                       AS unidsc,
                        n15.itcougoperador               AS ungcod,
                        ung.ungdsc                       AS ungdsc,
                        n15.itcofavorecido               AS cpfcnpjfavorecido,
                        n15.itcoprogramatrabalhoresumido AS ptres,
                        n15.ittxamparolegal              AS amparolegal,
                        n15.ittxreferenciadispensa       AS refdispensa,
                        n15.ittxobservacao               AS observacao,
                        n15.itcocontacorrentefinanceiro  AS contafinanceiro,
                        n15.itvatransacao                AS valortotal
                    FROM
                        siafi2015.ne n15
                    JOIN
                        dw.ug ung
                    ON
                        (
                            n15.itcougoperador::INTEGER = ung.ungcod::INTEGER )
                    JOIN
                        dw.uo uni
                    ON
                        (
                            ung.orgcod = uni.unicod)
                    WHERE
                        n15.gruggestaoannumeroneuq = '{$dados['necod']}'";

                $resultado = adapterConnection::siafi()->carregar($sql);
                return $resultado;
                break;
            default:
                return array();
                break;
        }
    }

    /*
     * Método que retorna a listagem das NS.
     * Parametros de requisição:
     *      credencial (*), paginacao (*), exercicio (*), unicod, ungcod, nscod, cpfcnpjfavorecido
     * Retorno:
     *      paginacao, datatransacao, unicod, unidsc, ungcod, ungdsc, nscod, cpfcnpjfavorecido, observacaovalortotal
     */

    public function retornarListaNs($dados) {

        switch ($_SESSION['exercicio']) {
            case 2015:
                /* Filtros */
                $where = "";
                if ($dados['datainicio'] != '') {
                    if ($dados['datafim'] == '') {
                        $dados['datafim'] = strftime('%d/%m/%Y', strtotime('today'));
                    }
                    $dados['datainicio'] = formata_data_sql($dados['datainicio']);
                    $dados['datafim'] = formata_data_sql($dados['datafim']);
                    $where .= " n15.itdatransacao BETWEEN '{$dados['datainicio']}' AND '{$dados['datafim']}'";
                }
                if ($dados['ungcod'] != '') {
                    $where .= " AND n15.itcougoperador = '{$dados['ungcod']}' ";
                }
                if ($dados['nscod'] != '') {
                    $where .= " AND n15.gruggestaoannumeronsuq LIKE '%{$dados['nscod']}%' ";
                }
                if ($dados['cpfcnpjfavorecido'] != '') {
                    $dados['cpfcnpjfavorecido'] = str_replace(array('.', '/', '-'), '', $dados['cpfcnpjfavorecido']);
                    $where .= " AND n15.itcofavorecido = '{$dados['cpfcnpjfavorecido']}' ";
                }
                if ($where != "") {
                    $where = " WHERE {$where}";
                }

                $sql = "
                SELECT
                    n15.gruggestaoannumeronsuq  AS codigo,
                    TO_CHAR(n15.itdatransacao, 'dd/mm/yyyy')    AS datatransacao,
                    ung.orgcod                  AS unicod,
                    uni.unidsc                  AS unidsc,
                    n15.itcougoperador          AS ungcod,
                    ung.ungdsc                  AS ungdsc,
                    n15.gruggestaoannumeronsuq  AS nscod,
                    n15.itcofavorecido          AS cpfcnpjfavorecido,
                    n15.itvatransacao_1         AS valortotal
                FROM
                    siafi2015.ns n15
                JOIN
                    dw.ug ung
                ON
                    (
                        n15.itcougoperador::INTEGER = ung.ungcod::INTEGER )
                JOIN
                    dw.uo uni
                ON
                    (
                        ung.orgcod = uni.unicod)
                {$where}
                ORDER BY n15.itdatransacao DESC";

                $pagina = $dados['paginacao']['pagina'];
                $limit = 100;
                $offset = ($pagina - 1) * $limit;
                if ($offset < 0)
                    $offset = 0;
                $resultado = array();
                $resultado['dados'] = adapterConnection::siafi()->carregar("{$sql} OFFSET {$offset} LIMIT {$limit}");
                $resultado['query'] = $sql;
                $resultado['registros'] = adapterConnection::siafi()->pegaUm("SELECT COUNT(1) AS total FROM ({$sql}) registros");
                $resultado['pagina'] = $pagina;
                break;
            default:
                $resultado = array();
                break;
        }
        return $resultado;
    }

    /*
     * Método que retorna o detalhe das NS.
     * Parametros de requisição:
     *      credencial (*), exercicio (*), nscod
     * Retorno:
     *      datatransacao, unicod, unidsc, ungcod, ungdsc, necod, cpfcnpjfavorecido, observacao, valortotal
     */

    public function retornarDetalheNs($dados) {
        #ver($dados,d);
        switch ($_SESSION['exercicio']) {
            case 2015:
                $sql = "SELECT
                            TO_CHAR(n15.itdatransacao, 'dd/mm/yyyy')    AS datatransacao,
                            n15.gruggestaoannumeronsuq  AS nscod,
                            ung.orgcod                  AS unicod,
                            uni.unidsc                  AS unidsc,
                            n15.itcougoperador          AS ungcod,
                            ung.ungdsc                  AS ungdsc,
                            n15.itcofavorecido          AS cpfcnpjfavorecido,
                            n15.ittxobservacaodocumento AS observacao,
                            n15.itcoinscricao01         AS nes,
                            n15.itvatransacao_1         AS valortotal
                        FROM
                            siafi2015.ns n15
                        JOIN
                            dw.ug ung
                        ON
                            (
                                n15.itcougoperador::INTEGER = ung.ungcod::INTEGER )
                        JOIN
                            dw.uo uni
                        ON
                            (
                                ung.orgcod = uni.unicod)
                                            WHERE
                                                n15.gruggestaoannumeronsuq = '{$dados['nscod']}'";

                $resultado = adapterConnection::siafi()->carregar($sql);
                return $resultado;
                break;
            default:
                return array();
                break;
        }
    }

}
