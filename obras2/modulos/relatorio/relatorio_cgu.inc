<?php

$sql = "SELECT
              DISTINCT
              o.docid,
              o.obrid AS id,
              o.preid AS preid,
              '(' || o.obrid || ') ' || obrnome AS descricao,
              mun.mundescricao AS municipio,
              mun.estuf AS uf,
              tpo.tpodsc,
              to_char(doc.docdatainclusao, 'DD/MM/YYYY') AS \"Data de Inclusão\",
              esd.esddsc,

              (SELECT st.stadesc
                                FROM obras2.supervisao s
                                JOIN obras2.situacaoatividade st ON st.staid = s.staid
                                WHERE s.obrid = o.obrid
                                      AND s.smiid IS NULL
                                      AND s.supstatus = 'A'::bpchar
                                      AND s.validadapelosupervisorunidade = 'S'::bpchar
                                      AND s.supid NOT IN (SELECT s.supid
                                    FROM obras2.supervisao s
                                    JOIN obras2.situacaoatividade st ON st.staid = s.staid
                                    WHERE s.obrid = o.obrid
                                          AND s.emsid IS NULL
                                          AND s.smiid IS NULL
                                          AND s.supstatus = 'A'::bpchar
                                          AND s.validadapelosupervisorunidade = 'S'::bpchar
                                    ORDER BY s.supdata DESC LIMIT 1)
                                ORDER BY s.supdata DESC LIMIT 1) as stadesc1,

                (SELECT TO_CHAR(s.supdata, 'DD/MM/YYYY') as supdata
                                FROM obras2.supervisao s
                                JOIN obras2.situacaoatividade st ON st.staid = s.staid
                                WHERE s.obrid = o.obrid
                                      AND s.smiid IS NULL
                                      AND s.supstatus = 'A'::bpchar
                                      AND s.validadapelosupervisorunidade = 'S'::bpchar
                                      AND s.supid NOT IN (SELECT s.supid
                                    FROM obras2.supervisao s
                                    JOIN obras2.situacaoatividade st ON st.staid = s.staid
                                    WHERE s.obrid = o.obrid
                                          AND s.emsid IS NULL
                                          AND s.smiid IS NULL
                                          AND s.supstatus = 'A'::bpchar
                                          AND s.validadapelosupervisorunidade = 'S'::bpchar
                                    ORDER BY s.supdata DESC LIMIT 1)
                                ORDER BY s.supdata DESC LIMIT 1) as supdata1,

              (SELECT st.stadesc
                        FROM obras2.supervisao s
                        JOIN obras2.situacaoatividade st ON st.staid = s.staid
                        WHERE s.obrid = o.obrid
                              AND s.emsid IS NULL
                              AND s.smiid IS NULL
                              AND s.supstatus = 'A'::bpchar
                              AND s.validadapelosupervisorunidade = 'S'::bpchar
                        ORDER BY s.supdata DESC LIMIT 1) as stadesc,

                (SELECT TO_CHAR(s.supdata, 'DD/MM/YYYY') as supdata
                                FROM obras2.supervisao s
                                JOIN obras2.situacaoatividade st ON st.staid = s.staid
                                WHERE s.obrid = o.obrid
                                      AND s.emsid IS NULL
                                      AND s.smiid IS NULL
                                      AND s.supstatus = 'A'::bpchar
                                      AND s.validadapelosupervisorunidade = 'S'::bpchar
                                ORDER BY s.supdata DESC LIMIT 1) as supdata

            FROM
                obras2.obras o
                INNER JOIN entidade.endereco    ende ON ende.endid = o.endid AND ende.endstatus = 'A' AND ende.tpeid = 4
                LEFT JOIN obras2.empresami_uf euf ON euf.estuf = ende.estuf AND euf.eufstatus = 'A'
                LEFT JOIN obras2.empresami    emi ON emi.emiid = euf.emiid AND emi.emistatus = 'A'
                LEFT JOIN territorios.municipio  mun ON mun.muncod = ende.muncod
                LEFT JOIN entidade.entidade    ent ON ent.entid = o.entid
                LEFT JOIN obras2.tipologiaobra    tpo ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.empreendimento    e ON e.empid = o.empid AND e.empstatus = 'A'
                LEFT JOIN obras2.obrascontrato     oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
                LEFT JOIN workflow.documento      doc ON doc.docid = o.docid
                LEFT JOIN workflow.estadodocumento esd ON esd.esdid = doc.esdid
            -- JOIN
            WHERE o.obrstatus = 'A' AND o.obridpai IS NULL AND e.orgid IN(3) -- AND o.tpoid IN (104, 105) -- Removido regra para apenas MI
            ORDER BY 2";

$obras = $db->carregar($sql);
foreach ($obras as $key => $obra) {

//    $ultVistoria = array();
//    $pultVistoria = array();
//
//    // Pega ultima vistoria
//    $sqlUltVistoria = "SELECT TO_CHAR(s.supdata, 'DD/MM/YYYY') as supdata, s.supid, st.stadesc
//                        FROM obras2.supervisao s
//                        JOIN obras2.situacaoatividade st ON st.staid = s.staid
//                        WHERE s.obrid = {$obra['id']}
//                              AND s.emsid IS NULL
//                              AND s.smiid IS NULL
//                              AND s.supstatus = 'A'::bpchar
//                              AND s.validadapelosupervisorunidade = 'S'::bpchar
//                        ORDER BY s.supdata DESC LIMIT 1";
//
//    $ultVistoria = $db->pegaLinha($sqlUltVistoria);
//
//    if($ultVistoria) {
//        // Pega penultima vistoria
//        $sqlPUltVistoria = "SELECT TO_CHAR(s.supdata, 'DD/MM/YYYY') as supdata, s.supid, st.stadesc
//                        FROM obras2.supervisao s
//                        JOIN obras2.situacaoatividade st ON st.staid = s.staid
//                        WHERE s.obrid = {$obra['id']}
//                              AND s.emsid IS NULL
//                              AND s.smiid IS NULL
//                              AND s.supstatus = 'A'::bpchar
//                              AND s.validadapelosupervisorunidade = 'S'::bpchar
//                              AND s.supid NOT IN({$ultVistoria['supid']})
//                        ORDER BY s.supdata DESC LIMIT 1";
//        $pultVistoria = $db->pegaLinha($sqlPUltVistoria);
//    }
//    $obras[$key]['sitpvisto'] = $pultVistoria['stadesc'];
//    $obras[$key]['dtpvisto'] = $pultVistoria['supdata'];
//    $obras[$key]['situvisto'] = $ultVistoria['stadesc'];
//    $obras[$key]['dtuvisto'] = $ultVistoria['supdata'];


    $sqlHistorico = "SELECT e1.*, a1.*, to_char(h1.htddata, 'DD/MM/YYYY') as htddata FROM workflow.historicodocumento h1
                        LEFT JOIN workflow.acaoestadodoc a1 ON a1.aedid = h1.aedid
                        LEFT JOIN workflow.estadodocumento e1 ON e1.esdid = a1.esdiddestino
                        WHERE h1.docid = {$obra['docid']}";
    $historicos = $db->carregar($sqlHistorico);
    $tramitacao = "";

    if ($historicos) {
        foreach ($historicos as $hist) {
            $obras[$key][] = "Eviado para " . $hist['esddsc'];
            $obras[$key][] = $hist['htddata'];
        }
    }
    $obras[$key]['Tramitação'] = $tramitacao;
    unset($obras[$key]['docid']);
}

ini_set("memory_limit", "1024M");

$cabecalho = array(
    'ID',
    'ID Pré-Obra',
    'Obra',
    'Município',
    'UF',
    'Tipologia',
    'Data de Inclusão',
    'Situação',
    'Situação Penúltima Vistoria',
    'Data Penúltima Vistoria',
    'Situação Última Vistoria',
    'Data  Última Vistoria',
    );

$db->sql_to_xml_excel($obras, 'relatorioCGU', $cabecalho);