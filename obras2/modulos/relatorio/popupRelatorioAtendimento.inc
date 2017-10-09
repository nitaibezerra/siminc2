<?php
set_time_limit(50000);
ini_set("memory_limit", "5000M");
extract($_REQUEST);

$param = $_POST;
$where = array();

if ($param['obrid']) {
    $where[] = " o.obrid = {$param['obrid']} ";
}
if ($param['preid']) {
    $where[] = " o.preid = {$param['preid']} ";
}

if ($param['empesfera']) {
    $where[] = " emp.empesfera = '" . $param['empesfera'] . "' ";
}

if ($param['estuf']) {
    $where[] = " m.estuf = '" . $param['estuf'] . "' ";
}

if ($param['muncod']) {
    $where[] = " m.muncod = '" . $param['muncod'] . "' ";
}

// Termo/Convenio
if ($param['convenio']) {
    $where[] = " p_conv.termo_convenio = '{$param['convenio']}' ";
}
// Ano Termo/Convenio
if ($param['ano_convenio']) {
    $where[] = " p_conv.ano_termo_convenio = '{$param['ano_convenio']}' ";
}

// processo
if ($param['processo']) {
    $where[] = "
                (
                    p_conv.pronumeroprocesso = Replace(Replace(Replace( '{$param['processo']}','.',''),'/',''),'-','')
                )
            ";
}


$sql = "
        SELECT
            o.obrid,
            o.preid,
            CASE WHEN o.tpoid IN (104, 105) THEN 'SIM' ELSE 'NÃO' END as mi,
            CASE WHEN (
                    SELECT COUNT(*)
                    FROM obras2.restricao r
                    JOIN workflow.documento d ON d.docid = r.docid AND d.esdid IN (1140, 1141, 1144)
                    WHERE r.rstitem = 'R' AND rststatus = 'A' AND r.obrid = o.obrid) > 0 THEN 'SIM' ELSE 'NÃO' END as r,
            p_conv.pronumeroprocesso,
            p_conv.termo_convenio,
            m.mundescricao,
            m.estuf,
            '(' || o.obrid || ') ' || o.obrnome as obrnome,
            tpo.tpodsc,
            pf.prfdesc,
            too.toodescricao,
            CASE WHEN emp.empesfera = 'M' THEN 'Municipal' ELSE 'Estadual' END empesfera,
            et.esddsc,
            TO_CHAR(o.obrdtultvistoria, 'DD/MM/YYYY') as obrdtultvistoria,
            ((((100 - coalesce(obrperccontratoanterior,0)) * coalesce(obrpercentultvistoria,0)) / 100) + coalesce(obrperccontratoanterior,0))::numeric(20,2)  as percentual_execucao,
            (1+
                ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatushomologacao = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
                ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus25exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END )+
                ( CASE WHEN (select count(vldid) from obras2.validacao where vldstatus50exec = 'S' and obrid = o.obrid) > 0 THEN 1 ELSE 0 END ))||' / 4' as exec_fisica,
            perc.perc_pag,
            CASE WHEN ff.diff_number >= -5 AND ff.diff_number <= 5 THEN 'SIM' ELSE 'NÃO' END AS equilibrio,
            replace(cast(to_char(o.obrvalorprevisto, 'D99')::money as varchar),'R$','') obrvalorprevisto,
            replace(cast(to_char(( SELECT sum(saldo) FROM par.vm_saldo_empenho_por_obra WHERE preid = pre.preid GROUP BY preid), 'D99')::money as varchar),'R$','') as valor_empenhado,
            replace(cast(to_char(pg.valorpagamento, 'D99')::money as varchar),'R$','') valorpagamento,
            replace(cast(to_char((o.obrvalorprevisto - pg.valorpagamento), 'D99')::money as varchar),'R$','') valorreceber,
            (
                SELECT replace(cast(to_char((pro.dfisaldoconta + pro.dfisaldofundo + pro.dfisaldopoupanca + pro.dfisaldordbcdb), 'D99')::money as varchar),'R$','') AS saldo
                FROM painel.dadosfinanceirosconvenios pro
                WHERE pro.dfiprocesso = p_conv.pronumeroprocesso
                ORDER BY pro.dfidatasaldo desc, pro.dfiid DESC
                LIMIT 1
            ) saldo_conta
        FROM obras2.obras o
        LEFT JOIN obras.preobra pre                          ON pre.preid = o.preid
        LEFT JOIN obras2.empreendimento emp                  ON emp.empid = o.empid
        LEFT JOIN entidade.endereco ed                       ON ed.endid = o.endid
        LEFT JOIN territorios.municipio m                    ON m.muncod = ed.muncod
        LEFT JOIN territorios.estado est                     ON est.estuf = m.estuf
        LEFT JOIN obras2.programafonte pf                    ON pf.prfid = emp.prfid
        LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
        LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
        LEFT JOIN workflow.documento d                       ON d.docid = o.docid
        LEFT JOIN workflow.estadodocumento et                ON et.esdid   = d.esdid
            -- Dados do Processo e Termo
        LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid
        LEFT JOIN (
            SELECT
            *,
                    CASE WHEN diff = 'N/A' OR diff IS NULL THEN '0 ' ELSE formataMonetario(trunc(diff::numeric, 2))  END as diff_formatado,
                    CASE WHEN diff = 'N/A' OR diff IS NULL THEN 0 ELSE trunc(diff::numeric, 2)  END as diff_number
                FROM obras2.vm_fisico_financeiro_processo) ff ON ff.pronumeroprocesso = p_conv.pronumeroprocesso

        LEFT JOIN (
            ( SELECT
                SUM( po.pobvalorpagamento ) valorpagamento, pe.preid
                FROM par.pagamento p
                INNER JOIN par.pagamentoobra po ON po.pagid = p.pagid
                INNER JOIN obras.preobra pe ON pe.preid = po.preid
                WHERE p.pagstatus = 'A'
                AND p.pagsituacaopagamento ILIKE '%EFETIVADO%'
                GROUP BY pe.preid
            )
                UNION
            ( SELECT
                    SUM( po.popvalorpagamento ) valorpagamento, pe.preid
                    FROM par.pagamento p
                    INNER JOIN par.pagamentoobrapar po ON po.pagid = p.pagid
                    INNER JOIN obras.preobra pe ON pe.preid = po.preid
                    WHERE p.pagstatus = 'A'
                    AND p.pagsituacaopagamento ilike '%EFETIVADO%'
                    GROUP BY pe.preid
            )
        ) as pg ON pg.preid = o.preid
        LEFT JOIN (

                       SELECT
                            CASE WHEN pre.prevalorobra != 0 THEN ROUND((pag.popvalorpagamento / pre.prevalorobra) * 100) ELSE 0 END as perc_pag, pre.preid
                        FROM (

                        SELECT
                           sum(pao.popvalorpagamento) as popvalorpagamento, pao.preid
                        FROM par.pagamentoobrapar pao
                        INNER JOIN par.pagamento pag1 ON pag1.pagid = pao.pagid
                        WHERE pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento ilike '%EFETIVADO%'
                        GROUP BY pao.preid

                        ) pag
                        INNER JOIN obras.preobra pre ON pre.preid = pag.preid

                        UNION


                        SELECT
                            CASE WHEN pre.prevalorobra != 0 THEN ROUND((pag.pobvalorpagamento / pre.prevalorobra) * 100) ELSE 0 END as perc_pag, pre.preid
                        FROM (

                        SELECT
                           sum(pao.pobvalorpagamento) as pobvalorpagamento, pao.preid
                        FROM par.pagamentoobra pao
                        INNER JOIN par.pagamento pag1 ON pag1.pagid = pao.pagid
                        WHERE pag1.pagstatus = 'A' AND pag1.pagsituacaopagamento ilike '%EFETIVADO%'
                        GROUP BY pao.preid

                        ) pag
                        INNER JOIN obras.preobra pre ON pre.preid = pag.preid


                   ) as perc ON perc.preid = o.preid
        WHERE
            o.obridpai IS NULL
        AND o.obrstatus = 'A'
    
    " . (count($where) ? " AND " . implode(' AND ', $where) : "") . "
";
$cabecalho = array("ID-Obra", "ID Pre-Obra", "Obra MI", "Possui Restrição?", "Nº Processo", "Nº Termo de Compromisso/Convênio", "Município", "UF", "Nome", "Tipologia", "Programa", "Fonte", "Esfera", "Situação", "Dt. Últ. Vistoria", "Perc. Últ. Vistoria", "Parcela Validada", "% Pago", "Equilíbrio Fis-Fin", "Valor Pactuado", "Valor Empenhado", "Valor Pago", "Valor a Receber", "Saldo em Conta");
// Gera o XLS do relatório
//ver($sql, d);
if ( $_REQUEST['pesquisa'] == 'xls' ){
    $db->sql_to_xml_excel($sql, 'Relatorio_Atendimento', $cabecalho, '');
}

?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>
<link rel="stylesheet" type="text/css" href="/includes/superTitle.css" />
<script type="text/javascript" src="/includes/remedial.js"></script>
<script type="text/javascript" src="/includes/superTitle.js"></script>
<html>
<head>
    <title> <?php echo NOME_SISTEMA; ?> </title>
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
