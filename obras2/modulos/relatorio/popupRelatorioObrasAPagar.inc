<?php
set_time_limit(50000);
ini_set("memory_limit", "5000M");
extract($_REQUEST);

$param = $_POST;
$where = array();
$where2 = array();


if ($param['obrid']) {
    $where[] = " o.obrid = '" . $param['obrid'] . "' ";
}

if ($param['preid']) {
    $where[] = " o.preid = '" . $param['preid'] . "' ";
}

if ($param['empesfera']) {
    $where[] = " e.empesfera = '" . $param['empesfera'] . "' ";
}

if ($param['estuf']) {
    $where[] = " m.estuf = '" . $param['estuf'] . "' ";
}

if ($param['muncod']) {
    $where[] = " m.muncod = '" . $param['muncod'] . "' ";
}

// processo
if ($param['processo']) {
    $where[] = "
                    vo.processo = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')
            ";
}

if ($param['obraapta'] == 'S'){
    $where2[] = " CASE WHEN conta_bancaria_bloqueada = 'NÃO' AND distribuir_empenho = 'NÃO' AND f.restricoes = 'NÃO' AND f.pendencias = 'NÃO' AND f.termo_valido = 'SIM' AND f.entidade_habilitada = 'SIM' AND f.obra_totalmente_empenhada = 'SIM' THEN true ELSE false END ";
}
if ($param['obraapta'] == 'N'){
    $where2[] = " CASE WHEN conta_bancaria_bloqueada = 'SIM' AND distribuir_empenho = 'SIM' AND f.restricoes = 'SIM' AND f.pendencias = 'SIM' AND f.termo_valido = 'SIM' AND f.entidade_habilitada = 'SIM' AND f.obra_totalmente_empenhada = 'SIM' THEN false ELSE true END ";
}

$sql = "
  SELECT
  f.*,
  CASE WHEN f.restricoes = 'NÃO' AND f.pendencias = 'NÃO' AND f.termo_valido = 'SIM' AND f.entidade_habilitada = 'SIM' AND f.obra_totalmente_empenhada = 'SIM' THEN 'SIM' ELSE 'NÃO' END
  FROM (
        SELECT
            o.obrid as id,
            o.preid as preid,
            m.estuf as uf,
            m.mundescricao as minicipio,
            esd.esddsc as situacao,
            ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2) || '%' as percentual_execucao,
            CASE WHEN e.empesfera = 'M' THEN 'Municipal' ELSE 'Estadual' END as esfera,
            vo.processo as processo,
            too.toodescricao as fonte,
            prf.prfdesc as programa,
            CASE WHEN o.obrid IN (
                SELECT
                        r.obrid
                    FROM obras2.restricao r
                    JOIN workflow.documento d ON d.docid = r.docid
                    JOIN workflow.estadodocumento e ON e.esdid = d.esdid AND e.esdid NOT IN (1142, 1143, 1497)
                    WHERE r.obrid = o.obrid AND rstitem IN ('R') AND rststatus = 'A'
            ) THEN 'SIM' ELSE 'NÃO' END as restricoes,
            CASE WHEN o.obrid IN (
                SELECT obrid FROM obras2.vm_pendencia_obras WHERE obrid = o.obrid
            ) THEN 'SIM' ELSE 'NÃO' END as pendencias,
            (
                SELECT CASE WHEN prosituacaoconta IS NULL THEN '-' WHEN prosituacaoconta IN (25,24,14) THEN 'SIM' ELSE 'NÃO' END prosituacaoconta FROM (

                    SELECT prosituacaoconta, pronumeroprocesso FROM par.processoobraspar
                    UNION
                    SELECT prosituacaoconta, pronumeroprocesso FROM par.processoobra

                ) as p
                WHERE pronumeroprocesso = vo.processo ) as conta_bancaria_bloqueada,
            CASE WHEN ((string_to_array(ter.data_vigencia, '/'))[3] || '-' || (string_to_array(ter.data_vigencia, '/'))[2] || '-' || (string_to_array(ter.data_vigencia, '/'))[1])::date >= NOW () THEN 'SIM' ELSE 'NÃO' END as termo_valido,
            ter.data_vigencia as validade_do_termo,
            (SELECT CASE WHEN iue.iuesituacaohabilita = 'Habilitado' THEN 'SIM' ELSE 'NÃO' END
                FROM par.instrumentounidadeentidade iue
                JOIN par.instrumentounidade iu ON iue.inuid = iu.inuid
                WHERE iue.iuedefault = 't' AND iu.muncod = ed.muncod
            ) as entidade_habilitada,

            CASE WHEN vo.saldo >= o.obrvalorprevisto THEN 'SIM' ELSE 'NÃO' END as obra_totalmente_empenhada,
            CASE WHEN vo.saldo > 0 AND o.obrvalorprevisto > 0 THEN (vo.saldo / o.obrvalorprevisto ) * 100 ELSE 0 END::numeric(20,2) as perc_empenhado,

            CASE WHEN emppac.temdivergente > 0 OR emppar.temdivergente > 0 THEN 'SIM' ELSE 'NÃO' END as distribuir_empenho,




            CASE WHEN
                            (
                              SELECT * FROM (
                                SELECT
                                    COUNT(empid)
                                FROM
                                    (
                                    SELECT emp.empid,  pob.preid,  eob.eobvalorempenho, sum( pob.popvalorpagamento )
                                    FROM
                                        par.empenho emp
                                    INNER JOIN par.empenhoobrapar 	eob ON eob.empid = emp.empid and eobstatus = 'A'
                                    INNER JOIN par.pagamento	 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' and parnumseqob is not null
                                    INNER JOIN par.pagamentoobrapar pob ON pob.preid = eob.preid AND pag.pagid = pob.pagid
                                    WHERE
                                        emp.empnumeroprocesso = vo.processo
                                        AND emp.empsituacao <> 'CANCELADO'
                                        AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
                                        AND emp.empstatus = 'A'

                                    GROUP BY
                                        emp.empid,
                                        pob.preid,
                                        eob.eobvalorempenho
                                    HAVING
                                        sum( pob.popvalorpagamento ) > (eob.eobvalorempenho+0.01)
                                ) as foo
                                UNION

                                SELECT
                                    COUNT(empid)
                                FROM
                                    (
                                        SELECT emp.empid,  pob.preid,  eob.eobvalorempenho, sum( pob.pobvalorpagamento )
                                        FROM
                                            par.empenho emp
                                        INNER JOIN par.empenhoobra 	eob ON eob.empid = emp.empid and eobstatus = 'A'
                                        INNER JOIN par.pagamento	 		pag ON pag.empid = emp.empid AND pag.pagstatus = 'A' and parnumseqob is not null
                                        INNER JOIN par.pagamentoobra pob ON pob.preid = eob.preid AND pag.pagid = pob.pagid
                                        WHERE
                                            emp.empnumeroprocesso = vo.processo
                                            AND emp.empsituacao <> 'CANCELADO'
                                            AND pag.pagsituacaopagamento not ilike '%CANCELADO%'
                                            AND emp.empstatus = 'A'

                                        GROUP BY
                                            emp.empid,
                                            pob.preid,
                                            eob.eobvalorempenho
                                        HAVING
                                            sum( pob.pobvalorpagamento ) > (eob.eobvalorempenho+0.01)
                                    ) as foo
                              ) as f LIMIT 1
                            ) > 0 THEN 'SIM' ELSE 'NÃO' END
             as distribuir_pagamento,

            CASE WHEN (SELECT COUNT (*) FROM (
                        ( SELECT p.*
                        FROM par.pagamento p
                        INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                        WHERE p.pagstatus = 'A'
                            AND p.pagsituacaopagamento NOT IN ('%CANCELADO%')
                            AND p.pagparcela = 1
                            AND po.preid = 37416
                        )
                        UNION
                        ( SELECT
                        p.*
                        FROM par.pagamento p
                        INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid
                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                        WHERE p.pagstatus = 'A'
                            AND p.pagsituacaopagamento NOT IN ('%CANCELADO%')
                            AND p.pagparcela = 1
                            AND po.preid = 37416
                        )
                    ) as sp) > 0 THEN 'SIM' ELSE 'NÃO' END pagamento_1_parcela,

            (
                SELECT (

                    ( SELECT case when pe.prevalorobra > 0 then
                        ROUND(SUM( po.pobvalorpagamento * 100 ) / pe.prevalorobra)
                        else 0 end
                        FROM par.pagamento p
                        INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                        WHERE p.pagstatus = 'A'
                            AND p.pagsituacaopagamento NOT IN ('%CANCELADO%')
                            AND po.preid = pre.preid
                        GROUP BY pe.prevalorobra
                    )
                    UNION
                    ( SELECT
                        CASE WHEN pe.prevalorobra > 0 then
                        ROUND(SUM( po.popvalorpagamento * 100 ) / pe.prevalorobra)
                        ELSE 0 END
                        FROM par.pagamento p
                        INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid
                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                        WHERE p.pagstatus = 'A'
                            AND p.pagsituacaopagamento NOT IN ('%CANCELADO%')
                            AND po.preid = pre.preid
                        GROUP BY pe.prevalorobra
                    )
                )
            )::numeric(10,2) as percentual_pago_total,

            (1+
            ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatushomologacao = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
            ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus25exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
            ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus50exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END ))||' / 4' as perc_validado,

            (
                SELECT
                    CASE WHEN vlddtinclusaost50exec IS NOT NULL THEN TO_CHAR(vlddtinclusaost50exec, 'dd/mm/YYYY')
                    WHEN vlddtinclusaost25exec IS NOT NULL THEN TO_CHAR(vlddtinclusaost25exec, 'dd/mm/YYYY')
                    WHEN vlddtinclusaosthomo IS NOT NULL THEN TO_CHAR(vlddtinclusaosthomo, 'dd/mm/YYYY')
                    END
                FROM obras2.validacao
                WHERE obrid = o.obrid LIMIT 1
            ) as dt_ultima_validacao,
            o.obrvalorprevisto as valor_da_obra,
            vo.saldo as valor_empenhado,
            (
                SELECT (

                    ( SELECT SUM( po.pobvalorpagamento )
                        FROM par.pagamento p
                        INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                        WHERE p.pagstatus = 'A'
                            AND p.pagsituacaopagamento NOT IN ('%CANCELADO%')
                            AND po.preid = pre.preid
                        GROUP BY pe.prevalorobra
                    )
                    UNION
                    ( SELECT SUM( po.popvalorpagamento )
                        FROM par.pagamento p
                        INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid
                        INNER JOIN obras.preobra pe ON pe.preid = po.preid
                        WHERE p.pagstatus = 'A'
                            AND p.pagsituacaopagamento NOT IN ('%CANCELADO%')
                            AND po.preid = pre.preid
                        GROUP BY pe.prevalorobra
                    )
                )
            ) as valor_pago,
            vo.saldo as valor_pactuado

        FROM obras2.obras o
        JOIN obras2.empreendimento e ON e.empid = o.empid
        JOIN obras.preobra pre ON pre.preid = o.preid
        JOIN entidade.endereco ed ON ed.endid = o.endid
        JOIN territorios.municipio m ON m.muncod = ed.muncod
        JOIN obras2.programafonte prf ON prf.prfid = e.prfid
        JOIN obras2.tipoorigemobra too ON too.tooid = o.tooid
        JOIN par.vm_saldo_empenho_por_obra vo ON vo.obrid = o.obrid
        JOIN workflow.documento d ON d.docid = o.docid
        JOIN workflow.estadodocumento esd ON esd.esdid = d.esdid

        LEFT JOIN (

                SELECT
                    pronumeroprocesso, doc::text as ternum,
                    (SELECT date_part('days',(SELECT (date_trunc('MONTH', ((string_to_array( data_vigencia , '/'))[2] ||'-'|| (string_to_array(data_vigencia, '/'))[1] ||'-'|| '01')::date) + INTERVAL '1 MONTH - 1 day')::date)) || '/' || data_vigencia) as data_vigencia
                FROM (
                    SELECT
                        (
                            SELECT
                                to_char(vigencia, 'MM/YYYY') -- seleciona maior vigência entre documento validado e ex-ofício
                            FROM (
                                SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia --Seleciona maior vigência entre termos validados
                                 FROM par.documentopar  d
                                 INNER JOIN par.documentoparvalidacao v ON d.dopid = v.dopid AND v.dpvstatus = 'A'
                                 WHERE d.proid = dp.proid
                                 AND dopstatus <> 'E'
                                 AND mdoid NOT IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                                UNION ALL
                                SELECT to_date(dopdatafimvigencia, 'MM/YYYY') as vigencia -- Seleciona maior vigência de Ex-Ofício
                                 FROM par.documentopar  d
                                 WHERE d.proid = dp.proid
                                 AND dopstatus <> 'E'
                                 AND mdoid IN (69,82,81,41,80,68,42,67,65,76,79,74,44,78,56,62,52,71,66,73,75,77)
                            ) as foo
                            GROUP BY vigencia
                            ORDER BY vigencia DESC LIMIT 1
                        ) as data_vigencia,
                        dopstatus,
                        (select dopnumerodocumento from par.documentopar where proid = dp.proid and dopstatus <> 'E' order by dopid asc LIMIT 1) as doc,
                        pp.pronumeroprocesso

                    FROM par.documentopar  dp
                    INNER JOIN par.modelosdocumentos   d ON d.mdoid = dp.mdoid
                    INNER JOIN par.processoobraspar pp ON pp.proid = dp.proid and pp.prostatus = 'A'
                    INNER JOIN par.instrumentounidade iu ON iu.inuid = pp.inuid

                ) as foo
                WHERE dopstatus = 'A'

                UNION

                SELECT
                    pro.pronumeroprocesso as pronumeroprocesso,
                    to_char(tc.terid,'00000') || '/' || to_char(tc.terdatainclusao,'YYYY') as ternum,
                    TO_CHAR(vig.data, 'DD/MM/YYYY') as datafimvigencia
                FROM
                    par.termocompromissopac  tc
                INNER JOIN par.processoobra 	pro ON pro.proid = tc.proid and pro.prostatus = 'A'
                LEFT JOIN (
                        select distinct
                            terid,
                            max(prazo) as data
                        from(
                            SELECT
                            po.preid,
                            case when vig.data is not null then vig.data else (MIN(pag.pagdatapagamentosiafi) + 720) end  as prazo,
                            tc.terid, popvalidacao
                            FROM
                            par.pagamentoobra po
                            inner join par.pagamento pag ON pag.pagid = po.pagid AND pag.pagstatus = 'A'
                            inner join par.empenho emp on emp.empid = pag.empid and emp.empstatus = 'A'
                            inner join par.termoobraspaccomposicao tc on tc.preid = po.preid

                            left join obras.preobraprorrogacao pp on pp.preid = po.preid and pp.popdataprazoaprovado is not null and pp.popvalidacao = 't'
                            left join(
                                SELECT popdataprazoaprovado as data, preid FROM obras.preobraprorrogacao WHERE popstatus = 'A'
                            ) vig on vig.preid = po.preid
                        group by po.preid, vig.data, tc.terid, popvalidacao
                        ) as foo
                        group by terid) vig on vig.terid = tc.terid
                WHERE
                    tc.terstatus = 'A'
                GROUP BY
                    tc.proid, tc.terid, tc.terdatainclusao, vig.data, pro.pronumeroprocesso


        ) as ter ON ter.pronumeroprocesso = vo.processo

        left join(
                    select
                        count(empenho) as temdivergente, processo
                    from (
                    select
                                coalesce(e.empvalorempenho, 0) as vrlempenho,
                                coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
                                (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
                                e.empid as empenho,
                                e.empnumeroprocesso as processo,
                                e.empnumero as notaempenho,
                                e.empprotocolo as sequencial,
                                e.empcodigoespecie as especie,
                                'OBRA' as tipo
                                from
                                par.processoobraspar pp
                                inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
                                left join par.empenhoobrapar es on es.empid = e.empid and es.eobstatus = 'A'
                                where
                                pp.prostatus = 'A'
                                and e.empstatus = 'A'
                                and e.empsituacao <> 'CANCELADO'
                                group by
                                e.empid,
                                e.empnumeroprocesso,
                                e.empnumero,
                                e.empcodigoespecie,
                                e.empprotocolo,
                                e.empvalorempenho
                     ) as foo
                     where
                            vrlempenho <> vrlempcomposicao
                                    and diferenca <> 0.01
                     group by processo
                    ) emppar on emppar.processo = vo.processo

        left join(
                    select
                    count(empenho) as temdivergente, processo
                    from (
                    select
                                coalesce(e.empvalorempenho, 0) as vrlempenho,
                                coalesce(sum(es.eobvalorempenho), 0) as vrlempcomposicao,
                                (coalesce(e.empvalorempenho, 0) - coalesce(sum(es.eobvalorempenho), 0)) as diferenca,
                                e.empid as empenho,
                                e.empnumeroprocesso as processo,
                                e.empnumero as notaempenho,
                                e.empprotocolo as sequencial,
                                e.empcodigoespecie as especie,
                                'PAC' as tipo
                                from
                                par.processoobra pp
                                inner join par.empenho e on e.empnumeroprocesso = pp.pronumeroprocesso
                                left join par.empenhoobra es on es.empid = e.empid and es.eobstatus = 'A'
                                where
                                pp.prostatus = 'A'
                                and e.empstatus = 'A'
                                and e.empsituacao <> 'CANCELADO'
                                group by
                                e.empid,
                                e.empnumeroprocesso,
                                e.empnumero,
                                e.empcodigoespecie,
                                e.empprotocolo,
                                e.empvalorempenho
                    ) as foo
                    where
                        vrlempenho <> vrlempcomposicao
                                    and diferenca <> 0.01
                    group by processo
                    ) emppac on emppac.processo = vo.processo


        WHERE
            o.obridpai IS NULL
            AND o.obrstatus = 'A'
            " . (count($where) ? " AND " . implode(' AND ', $where) : "") . "
        ORDER BY 3
     ) as f
     WHERE 1=1 " . (count($where2) ? " AND (" . implode(' OR ', $where2) . ")" : "") . "
";
$cabecalho = array("ID", "Pré-ID", "UF", "Município", "Situação", "% Execução", "Esfera", "Processo", "Fonte", "Programa", "Possui Restrições no Obras 2.0", "Possui Pendências no PAR", "Conta Bancária Bloqueada", "Termo Válido", "Validade do Termo", "Entidade Habilitada", "Obra Totalmente Empenhada?", "% Empenhado", "Distribuir de Empenho", "Distribuir de Pagamento", "Pagamento 1ª Parcela", "Percentual Pago Total", "% Validado", "Data da Última Validação", "Valor da Obra", "Valor Empenhado", "Valor Pago", "Valor Empenhado", "Obra Apta a Pagar" );
// Gera o XLS do relatório
//ver($sql, d);
if ( $_REQUEST['pesquisa'] == 'xls' ){
    $db->sql_to_xml_excel($sql, 'Relatorio_Obras_a_Pagar', $cabecalho, '');
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<html>
<head>
    <title> SIMEC - Sistema Integrado de Monitoramento do Ministério da Educação </title>
    <link rel="stylesheet" type="text/css" href="../includes/Estilo.css">
    <link rel="stylesheet" type="text/css" href="../includes/listagem.css">
</head>
<body>
<center>
    <!--  Cabeçalho Brasão -->
    <?php echo monta_cabecalho_relatorio( '95' ); ?>
</center>
</body>
</html>
<?php
//ver($sql, d);
$db->monta_lista( $sql, $cabecalho, 50, 10, 'N', 'center', 'N', '', '','');
?>
