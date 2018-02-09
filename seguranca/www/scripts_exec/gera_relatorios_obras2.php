<?php

ini_set("memory_limit", "3024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));
$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '147';


$db = new cls_banco();


include_once APPRAIZ . 'www/obras2/_constantes.php';
include_once APPRAIZ . 'www/obras2/_funcoes.php';
include_once APPRAIZ . 'www/obras2/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/Modelo.class.inc";


if ($_REQUEST['type'] == 'relatorio_distrato') {
    $sql = "
        SELECT
            o.obrid,
            o.preid,
            o.obrnome,
            mun.estuf,
            mun.mundescricao,
            tpo.tpodsc,
            too.toodescricao,
            pf.prfdesc,
            ed.esddsc,
            p_conv.pronumeroprocesso,
            CASE WHEN doc.tipo = '1' THEN 'Tipo do arquivo' ELSE 'Nome do Arquivo' END filtro
        FROM obras2.obras o
        INNER JOIN (

            SELECT
                DISTINCT oa.obrid, '1' as tipo
            FROM
                obras2.obras_arquivos oa
            JOIN obras2.obras o ON o.obrid = oa.obrid AND o.obridpai IS NULL and o.obrstatus = 'A'
            JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid AND ta.tpaid = 30
            JOIN public.arquivo      a ON a.arqid = oa.arqid
            WHERE
                oarstatus = 'A' AND (arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')

            UNION


            SELECT
                oa.obrid, '2' as tipo
            FROM
                obras2.obras_arquivos oa
            JOIN obras2.obras o ON o.obrid = oa.obrid AND o.obridpai IS NULL and o.obrstatus = 'A'
            JOIN obras2.tipoarquivo ta ON ta.tpaid = oa.tpaid
            JOIN public.arquivo      a ON a.arqid = oa.arqid
            WHERE
                oarstatus = 'A'
                AND (arqtipo != 'image/jpeg' AND arqtipo != 'image/gif' AND arqtipo != 'image/png')
                AND (oardesc ILIKE 'distrato' OR arqdescricao ILIKE 'distrato' OR arqnome ILIKE 'distrato')
                AND  ta.tpaid != 30
        ) as doc ON doc.obrid = o.obrid
        LEFT JOIN obras2.empreendimento e                    ON e.empid = o.empid
        LEFT JOIN entidade.endereco edr                      ON edr.endid = o.endid
        LEFT JOIN territorios.municipio mun                  ON mun.muncod = edr.muncod
        LEFT JOIN territorios.estado est                     ON mun.estuf = est.estuf
        LEFT JOIN obras2.programafonte pf                    ON pf.prfid = e.prfid
        LEFT JOIN obras2.tipologiaobra tpo                   ON tpo.tpoid = o.tpoid
        LEFT JOIN obras2.tipoorigemobra too                  ON too.tooid = o.tooid
        LEFT JOIN workflow.documento d                       ON d.docid = o.docid
        LEFT JOIN workflow.estadodocumento ed                ON ed.esdid   = d.esdid
        LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = o.obrid
        WHERE o.obridpai IS NULL AND o.obrstatus = 'A'
        ORDER BY doc.tipo ASC
    ";
    $db->sql_to_xml_excel($sql, 'obras_distrato', array('ID', 'Pré-ID', 'Nome', 'UF', 'Município', 'Tipologia', 'Fonte', 'Programa', 'Situação', 'Nº Processo', 'Método de Filtragem'));
}


if ($_REQUEST['type'] == 'relatorio_termo_obra') {

    $cabecalho = array(
        "ID Obra",
        "Preid",
        "Número do Termo",
        "Nome",
        "Município",
        "UF",
        "Fonte",
        "Programa",
        "Situação",
        "% Ult. Vistoria",
        "Dt. Ult. Vistoria",
        "Data Primeiro Pagamento",
        "Data Último Pagamento",
        "Pagamento Solicitado Por",
        "Início Vigência da Obra",
        "Fim Vigência da Obra",
        "Houve Prorrogação?",
        "Prorrogação Solicitada Por",
        "Data de Validação do Termo",
        "Validado Por",
        "Início Vigência Termo",
        "Fim Vigência Termo"
    );

    $sql = "
                SELECT
                    o.obrid as \"ID Obra\",
                    f.\"Preid\",
                    f.\"Número do Termo\",
                    o.obrnome as \"Nome\",
                    mun.mundescricao as \"Município\",
                    mun.estuf as \"UF\",
                    f.\"Fonte\",
                    pf.prfdesc as \"Programa\",
                    ed.esddsc as \"Situação\",
                    CASE WHEN o.obrdtultvistoria IS NOT NULL THEN coalesce(o.obrpercentultvistoria, 0) ELSE o.obrpercentultvistoria END as \"% Ult. Vistoria\",
                    TO_CHAR(o.obrdtultvistoria, 'DD/MM/YYYY') as \"Dt. Ult. Vistoria\",
                    f.\"Data Primeiro Pagamento\",
                    f.\"Data Último Pagamento\",
                    f.\"Pagamento Solicitado Por\",
                    f.\"Data Primeiro Pagamento\" as \"Início Vigência da Obra\",
                    f.\"Fim Vigência\" as \"Fim Vigência da Obra\",
                    f.\"Houve Prorrogação?\",
                    f.\"Prorrogação Solicitada Por\",
                    f.\"Data de Validação do Termo\",
                    f.\"Validado Por\",
                    f.\"Início Vigência Termo\",
                    f.\"Fim Vigência Termo\"

                FROM (

                -- PAR
                select
                               tc.preid as \"Preid\",
                               dp.dopdatainiciovigencia as \"Início Vigência Termo\",
                           dp.dopdatafimvigencia as \"Fim Vigência Termo\",
                           TO_CHAR(\"Início Vigência\", 'dd/mm/YYYY') as \"Data Primeiro Pagamento\",
                               TO_CHAR(\"Fim Vigência\", 'dd/mm/YYYY') as \"Fim Vigência\",
                               dp.dopnumerodocumento::character varying as \"Número do Termo\",
                                'PAR' as \"Fonte\",
                           '-' as \"Houve Prorrogação?\",
                           '-' as \"Prorrogação Solicitada Por\",
                           TO_CHAR( dpvdatavalidacao, 'dd/mm/YYYY' ) as \"Data de Validação do Termo\",
                           usu.usunome as \"Validado Por\",
                           TO_CHAR(p.pagdatapagamento, 'dd/mm/YYYY') as \"Data Último Pagamento\",
                           usup.usunome as \"Pagamento Solicitado Por\"
                from
                                par.documentopar dp
                inner join par.termocomposicao tc ON tc.dopid = dp.dopid AND tc.preid is not null
                inner join par.documentoparvalidacao dpv ON dpv.dopid = dp.dopid AND dpvstatus = 'A'
                LEFT JOIN seguranca.usuario usu ON usu.usucpf = dpv.dpvcpf
                LEFT JOIN ( select
                                               MIN(p.pagdatapagamento) as \"Início Vigência\",
                                               ( MIN(p.pagdatapagamento) + 720 ) as \"Fim Vigência\",
                                               pro.proid
                                from
                                               par.pagamentoobrapar pop
                                inner join par.pagamento p on p.pagid = pop.pagid AND p.pagsituacaopagamento <> 'CANCELADO' AND p.pagstatus = 'A' AND p.pagparcela = 1
                                inner join par.empenho emp on emp.empid = p.empid AND emp.empsituacao <> 'CANCELADO' AND emp.empstatus = 'A'
                                inner join par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso AND pro.prostatus = 'A'
                                GROUP BY pro.proid
                                ) as dadosp on dadosp.proid = dp.proid
                LEFT JOIN ( select
                                               MAX(p.pagid) as pagid,
                                               pro.proid
                                from
                                               par.pagamentoobrapar pop
                                inner join par.pagamento p on p.pagid = pop.pagid AND p.pagsituacaopagamento <> 'CANCELADO' AND p.pagstatus = 'A' AND p.pagparcela = 1
                                inner join par.empenho emp on emp.empid = p.empid AND emp.empsituacao <> 'CANCELADO' AND emp.empstatus = 'A'
                                inner join par.processoobraspar pro ON pro.pronumeroprocesso = emp.empnumeroprocesso AND pro.prostatus = 'A'
                                GROUP BY pro.proid
                                ) as dadosup on dadosup.proid = dp.proid
                JOIN par.pagamento p ON p.pagid = dadosup.pagid and p.pagstatus = 'A'
                JOIN seguranca.usuario usup ON p.usucpf = usup.usucpf
                where
                                dp.dopstatus = 'A'

                UNION ALL

                -- PAC
                select DISTINCT
                               teo.preid as \"Preid\",
                               TO_CHAR(ppg.dtprimeiropagamentotermo, 'dd/mm/YYYY') as \"Início Vigência Termo\",
                           TO_CHAR(upg.dtultpagamentotermo, 'dd/mm/YYYY') as \"Fim Vigência Termo\",
                               TO_CHAR(\"Início Vigência\", 'dd/mm/YYYY') as \"Data Primeiro Pagamento\",
                               CASE WHEN pro.preid IS NOT NULL THEN TO_CHAR( dataprorrogada, 'dd/mm/YYYY' ) ELSE TO_CHAR(\"Fim Vigência\", 'dd/mm/YYYY') END as \"Fim Vigência\",
                               tcp.terid || '/' || TO_CHAR( tcp.terdatainclusao, 'YYYY' ) as \"Número do Termo\",
                               'PAC' as \"Fonte\",
                           CASE WHEN pro.preid IS NOT NULL THEN 'Sim' ELSE 'Não' END as \"Houve Prorrogação?\",
                           usu2.usunome as \"Prorrogação Solicitada Por\",
                           TO_CHAR( terdataassinatura, 'dd/mm/YYYY' ) as \"Data de Validação do Termo\",
                           usu.usunome as \"Validado Por\",
                           TO_CHAR(p.pagdatapagamento, 'dd/mm/YYYY') as \"Data Último Pagamento\",
                           usup.usunome as \"Pagamento Solicitado Por\"

                from
                                par.termocompromissopac tcp
                LEFT JOIN seguranca.usuario usu ON usu.usucpf = tcp.usucpfassinatura
                INNER JOIN par.termoobra teo on teo.terid = tcp.terid
                INNER JOIN obras.preobra pre ON pre.preid = teo.preid
                inner join territorios.municipio mun on mun.muncod = pre.muncod
                inner join obras2.obras o ON o.preid = pre.preid AND o.obrstatus = 'A' AND o.obridpai is null
                inner join workflow.documento d ON d.docid = o.docid
                inner join workflow.estadodocumento esd ON esd.esdid = d.esdid
                inner JOIN par.pagamentoobra po on po.preid = teo.preid
                LEFT JOIN ( select distinct MAX(popdataprazoaprovado) as dataprorrogada, preid, usucpf from obras.preobraprorrogacao where popvalidacao = 't' group by preid, usucpf  ) pro ON pro.preid = teo.preid
                LEFT JOIN seguranca.usuario usu2 ON usu2.usucpf = pro.usucpf
                LEFT JOIN (
                    SELECT tcp1.terid,  MAX(pg1.dtultpagamentotermo) as dtultpagamentotermo
                    FROM
                        par.termocompromissopac tcp1
                    INNER JOIN par.termoobra teo1 ON teo1.terid = tcp1.terid
                    INNER JOIN obras.preobra pre1 ON pre1.preid = teo1.preid
                    INNER JOIN (select
                                      MAX(p1.pagdatapagamento) as dtultpagamentotermo,
                                      teo1.terid
                            from
                                       par.pagamentoobra po1
                            inner join par.pagamento p1 on p1.pagid = po1.pagid AND p1.pagsituacaopagamento <> 'CANCELADO' AND p1.pagstatus = 'A' AND p1.pagparcela = 1
                            inner join par.empenho emp1 on emp1.empid = p1.empid AND emp1.empsituacao <> 'CANCELADO' AND emp1.empstatus = 'A'
                            inner join par.processoobra pro1 ON pro1.pronumeroprocesso = emp1.empnumeroprocesso AND pro1.prostatus = 'A'
                            INNER JOIN par.termoobra teo1 ON teo1.preid = po1.preid
                            GROUP BY teo1.terid) pg1 ON pg1.terid = teo1.terid
                    GROUP BY tcp1.terid
                ) as upg ON upg.terid = tcp.terid
                LEFT JOIN (
                    SELECT tcp1.terid, MIN(pg1.dtprimeiropagamentotermo) as dtprimeiropagamentotermo
                    FROM
                        par.termocompromissopac tcp1
                    INNER JOIN par.termoobra teo1 ON teo1.terid = tcp1.terid
                    INNER JOIN obras.preobra pre1 ON pre1.preid = teo1.preid
                    INNER JOIN (select
                                      MIN(p1.pagdatapagamento) as dtprimeiropagamentotermo,
                                      teo1.terid
                            from
                                       par.pagamentoobra po1
                            inner join par.pagamento p1 on p1.pagid = po1.pagid AND p1.pagsituacaopagamento <> 'CANCELADO' AND p1.pagstatus = 'A' AND p1.pagparcela = 1
                            inner join par.empenho emp1 on emp1.empid = p1.empid AND emp1.empsituacao <> 'CANCELADO' AND emp1.empstatus = 'A'
                            inner join par.processoobra pro1 ON pro1.pronumeroprocesso = emp1.empnumeroprocesso AND pro1.prostatus = 'A'
                            INNER JOIN par.termoobra teo1 ON teo1.preid = po1.preid
                            GROUP BY teo1.terid) pg1 ON pg1.terid = teo1.terid
                    GROUP BY tcp1.terid
                ) as ppg ON ppg.terid = tcp.terid
                LEFT JOIN ( select
                                               MIN(p.pagdatapagamento) as \"Início Vigência\",
                                               ( MIN(p.pagdatapagamento) + 720 ) as \"Fim Vigência\",
                                               pro.proid
                                from
                                               par.pagamentoobra po
                                inner join par.pagamento p on p.pagid = po.pagid AND p.pagsituacaopagamento <> 'CANCELADO' AND p.pagstatus = 'A' AND p.pagparcela = 1
                                inner join par.empenho emp on emp.empid = p.empid AND emp.empsituacao <> 'CANCELADO' AND emp.empstatus = 'A'
                                inner join par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso AND pro.prostatus = 'A'
                                GROUP BY pro.proid
                                ) as dadosp on dadosp.proid = tcp.proid
                LEFT JOIN ( select
                                               MAX(p.pagid) as pagid,
                                               pro.proid
                                from
                                               par.pagamentoobra po
                                inner join par.pagamento p on p.pagid = po.pagid AND p.pagsituacaopagamento <> 'CANCELADO' AND p.pagstatus = 'A' AND p.pagparcela = 1
                                inner join par.empenho emp on emp.empid = p.empid AND emp.empsituacao <> 'CANCELADO' AND emp.empstatus = 'A'
                                inner join par.processoobra pro ON pro.pronumeroprocesso = emp.empnumeroprocesso AND pro.prostatus = 'A'
                                GROUP BY pro.proid
                                ) as dadosup on dadosup.proid = tcp.proid
                JOIN par.pagamento p ON p.pagid = dadosup.pagid and p.pagstatus = 'A'
                JOIN seguranca.usuario usup ON p.usucpf = usup.usucpf
                WHERE
                                tcp.terstatus = 'A'
                 ) as f
                JOIN obras2.obras o ON o.preid = f.\"Preid\" AND o.obridpai IS NULL AND o.obrstatus = 'A'
                JOIN obras2.empreendimento ep ON ep.empid = o.empid
                LEFT JOIN workflow.documento doc ON doc.docid = o.docid
                LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = doc.esdid
                LEFT JOIN obras2.tipoorigemobra too ON too.tooid = o.tooid
                LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid
                LEFT JOIN entidade.endereco edo on edo.endid = o.endid
                LEFT JOIN territorios.municipio mun on mun.muncod = edo.muncod
                ORDER BY 3, 1, 2
        ";
    $obras = $db->carregar($sql);
    $db->sql_to_xml_excel($obras, 'relatorio_termo_obras', $cabecalho);
    exit;
}

if ($_REQUEST['type'] == 'relatorio_validacao_sem_pagamento') {
    $cabecalho = array("ID", "Pré ID", "Nome", "UF", "Município", "Programa", "Fonte", "Situação", "Percentual", "Nº Processo/Convênio", "Validação 25%", "Dt Validação 25%", "Validação 50%", "Dt Validação 50%", "Dt Últ. Pagamento");
    $sql = "
                SELECT 
                    o.obrid as \"ID Obra\",
                    o.preid as \"Pré ID\",
                    o.obrnome as \"Nome\",
                    mun.estuf as \"UF\",
                    mun.mundescricao as \"Município\",
                    pf.prfdesc as \"Programa\",
                    too.toodescricao as \"Fonte\",
                    ed.esddsc as \"Situação\",
                    o.obrpercentultvistoria as \"Percentual\",
                    CASE WHEN o.tooid = 2 AND TRIM(o.obrnumprocessoconv) != '' THEN 
                        to_char(Replace(Replace(Replace(o.obrnumprocessoconv,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                    WHEN po1.pronumeroprocesso IS NOT NULL THEN 
                        to_char(Replace(Replace(Replace( po1.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                    WHEN po2.pronumeroprocesso IS NOT NULL THEN 
                        to_char(Replace(Replace(Replace(po2.pronumeroprocesso,'.',''),'/',''),'-','')::bigint, 'FM00000\".\"000000\"/\"0000\"-\"00')
                    END as \"Nº Processo/Convênio\",
                    CASE WHEN vldstatus25exec = 'S' THEN 'SIM' ELSE 'NÃO' END AS \"Validação 25%\",
                    CASE WHEN vldstatus25exec = 'S' THEN v.vlddtinclusaost25exec::date ELSE NULL END AS \"Dt Validação 25%\",
                    CASE WHEN vldstatus50exec = 'S' THEN 'SIM' ELSE 'NÃO' END AS \"Validação 50%\",
                    CASE WHEN vldstatus50exec = 'S' THEN v.vlddtinclusaost50exec::date ELSE NULL END AS \"Dt Validação 50%\",
                    (SELECT p.pagdatapagamento FROM par.pagamentoobra po
                      JOIN par.pagamento p ON p.pagid = po.pagid
                      WHERE p.pagstatus = 'A' AND pagsituacaopagamento <> 'CANCELADO' AND po.preid = o.preid ORDER BY  p.pagid DESC limit 1) AS \"Dt Últ. Pagamento\"
                FROM obras2.obras o 
                JOIN obras2.validacao v ON v.obrid = o.obrid
                JOIN obras2.empreendimento ep ON ep.empid = o.empid
                
                LEFT JOIN par.processoobrasparcomposicao pop1 ON pop1.preid = o.preid
                LEFT JOIN par.processoobraspar po1 on po1.proid = pop1.proid and po1.prostatus = 'A'
                
                LEFT JOIN par.processoobraspaccomposicao pop2 ON pop2.preid = o.preid
                LEFT JOIN par.processoobra po2 on po2.proid = pop2.proid and po2.prostatus = 'A'
                LEFT JOIN workflow.documento doc ON doc.docid = o.docid
                LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = doc.esdid
                LEFT JOIN obras2.tipoorigemobra too ON too.tooid = o.tooid
                LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid
                LEFT JOIN entidade.endereco edo on edo.endid = o.endid
                LEFT JOIN territorios.municipio mun on mun.muncod = edo.muncod
                WHERE 
                o.obridpai IS NULL AND 
                o.obrstatus = 'A' AND 
                (vldstatus50exec = 'S' OR vldstatus25exec = 'S') AND
                CASE WHEN vldstatus50exec = 'S' THEN v.vlddtinclusaost50exec::date ELSE v.vlddtinclusaost25exec::date END > (SELECT p.pagdatapagamento FROM par.pagamentoobra po
                                                                          JOIN par.pagamento p ON p.pagid = po.pagid
                                                                          WHERE p.pagstatus = 'A' AND pagsituacaopagamento <> 'CANCELADO' AND po.preid = o.preid ORDER BY  p.pagid DESC limit 1)
                
                ORDER BY 4, 5, 10
        ";
    $obras = $db->carregar($sql);
    $db->sql_to_xml_excel($obras, 'relatorio_prazo_obra', $cabecalho);
    exit;
}

if ($_REQUEST['type'] == 'relatorio_evolucao_obra') {
    $cabecalho = array("ID", "Nome", "Programa", "Fonte", "Situação", "UF", "Município");
    $sql = "
            SELECT
                o.obrid,
                o.obrnome,
                pf.prfdesc as programa,
                too.toodescricao as fonte,
                ed.esddsc as situacao,
                mun.estuf,
                mun.mundescricao,
                (SELECT ARRAY_TO_STRING(ARRAY(
                SELECT
                    TO_CHAR(s.supdata, 'DD/MM/YYYY')::text || ';' ||
                    (SELECT
                          CASE
                          WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                          ELSE 0::numeric
                          END AS total
                           FROM obras2.itenscomposicaoobra i
                        LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                           WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND i.obrid = o1.obrid)::text

                FROM obras2.obras o1
                JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
                WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                ORDER BY supdata DESC), ';')) as vistorias

            FROM obras2.obras o
            JOIN obras2.empreendimento ep ON ep.empid = o.empid
            LEFT JOIN workflow.documento doc ON doc.docid = o.docid
            LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = doc.esdid
            LEFT JOIN obras2.tipoorigemobra too ON too.tooid = o.tooid
            LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid
            LEFT JOIN entidade.endereco edo on edo.endid = o.endid
            LEFT JOIN territorios.municipio mun on mun.muncod = edo.muncod
            WHERE o.obridpai IS NULL AND o.obrstatus = 'A'

    ";

    $obras = $db->carregar($sql);
    $maxVst = 0;
    foreach ($obras as $key => $obra) {
        $vistorias = $obras[$key]['vistorias'];
        unset($obras[$key]['vistorias']);
        $arV = explode(';', $vistorias);
        $obras[$key] = $obras[$key] + $arV;
        $maxVst = (count($arV) > $maxVst) ? count($arV) : $maxVst;
    }

    for ($x = 1; $x <= $maxVst; $x++){
        if(($x % 2) != 0)
            $cabecalho[] = 'Dt. Vistoria';
        else
            $cabecalho[] = '% Vistoria';
    }

    $db->sql_to_xml_excel($obras, 'relatorio_evolucao_obra', $cabecalho);
    exit;
}

if ($_REQUEST['type'] == 'relatorio_prazo_obra') {
    $cabecalho = array("ID", "Nome", "Programa", "Fonte", "Situação", "UF", "Município", "Valor do Contrato", "Data do contrato", "Valor Pactuado", "Data da Licitação", "Data Primeira Vistoria", "Fata Última Vistoria", "Entrou em paralisação");
    $sql = "
                SELECT DISTINCT
                    o.obrid,
                    o.obrnome,
                    pf.prfdesc as programa,
                    too.toodescricao as fonte,
                    ed.esddsc as situacao,
                    mun.estuf,
                    mun.mundescricao,
                    c.crtvalorexecucao as valorcontrato,
                    c.crtdtassinatura as datacontrato,
                    o.obrvalorprevisto as valorpactuado,
                    (SELECT TO_CHAR(fl.flchomlicdtprev, 'DD/MM/YYYY') FROM obras2.obralicitacao ol
                        JOIN obras2.faselicitacao fl ON fl.licid = ol.licid AND fl.flcstatus = 'A' AND fl.tflid = 9
                        WHERE ol.obrid = o.obrid AND ol.oblstatus = 'A' LIMIT 1) as dtlicitacao,
                    (SELECT TO_CHAR(s.supdata, 'DD/MM/YYY')
                        FROM obras2.obras o1
                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o1.obrid AND s.rsuid = 1
                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                        ORDER BY o1.obrid, s.supdata ASC
                        OFFSET 0
                        LIMIT 1) as dtprimeiravistoria,
                    (SELECT TO_CHAR(s.supdata, 'DD/MM/YYY')
                        FROM obras2.obras o1
                        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o1.obrid AND s.rsuid = 1
                        WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid
                        ORDER BY o1.obrid, s.supdata DESC
                        OFFSET 0
                        LIMIT 1) as dtultimavistoria,
                    CASE WHEN (SELECT COUNT(*)
                            FROM obras2.obras o1
                            JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o1.obrid AND s.rsuid = 1
                            WHERE o1.obridpai IS NULL AND o1.obrstatus = 'A' AND o1.obrid = o.obrid AND s.staid = 2 ) > 0 THEN 'SIM' ELSE 'NÃO' END as paralisacao
                FROM obras2.obras o
                JOIN obras2.empreendimento ep ON ep.empid = o.empid
                LEFT JOIN obras2.obrascontrato oc ON oc.obrid = o.obrid AND oc.ocrstatus = 'A'
                LEFT JOIN obras2.contrato c ON c.crtid = oc.crtid AND c.crtstatus = 'A'
                LEFT JOIN workflow.documento doc ON doc.docid = o.docid
                LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = doc.esdid
                LEFT JOIN obras2.tipoorigemobra too ON too.tooid = o.tooid
                LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid
                LEFT JOIN entidade.endereco edo on edo.endid = o.endid
                LEFT JOIN territorios.municipio mun on mun.muncod = edo.muncod
                WHERE o.obridpai IS NULL AND o.obrstatus = 'A'
        ";
    $obras = $db->carregar($sql);
    $db->sql_to_xml_excel($obras, 'relatorio_prazo_obra', $cabecalho);
    exit;
}


if ($_REQUEST['type'] == 'relatorio_obras_supervisao') {
    $cabecalho = array("ID Obra", "Nome da Obra", "Nº da OS", "Pergunta", "Questão", "Outros");
    $sql = "
                SELECT
                    o.obrid as \"ID Obra\",
                    o.obrnome as \"Nome da Obra\",
                    sos.sosnum as \"Nº da OS\",
                    dvq.dvqnumero || ' ' || dvq.dvqdsc as \"Pergunta\",
                    qst.qstnumero || ' ' || qst.qstdsc as \"Questão\",
                    rsq.rsqobs as \"Outros\"
                FROM obras2.supervisaoempresa sue
                JOIN obras2.supervisao_os_obra  soo ON sue.sosid = soo.sosid AND soo.soostatus = 'A' AND sue.empid = soo.empid
                JOIN obras2.supervisao_os sos ON sos.sosid = soo.sosid AND sos.sosstatus = 'A'
                JOIN workflow.documento d ON d.docid = sue.docid
                --JOIN obras2.supervisao sup ON sup.sueid = sue.sueid AND sup.supstatus = 'A'
                JOIN obras2.empreendimento e ON e.empid = sue.empid
                JOIN obras2.obras o ON e.empid = o.empid AND o.obridpai IS NULL AND o.obrstatus = 'A'
                JOIN obras2.questaosupervisao qts ON qts.sueid = sue.sueid AND qts.qtsstatus = 'A'
                JOIN obras2.questao qst ON qst.qstid = qts.qstid AND qst.qststatus = 'A'
                JOIN obras2.divisaoquestao dvq ON dvq.dvqid = qst.dvqid
                JOIN obras2.subquestao sqt ON sqt.qstid = qst.qstid AND sqt.sqtstatus = 'A'
                JOIN obras2.respostasubquestao rsq ON rsq.sqtid = sqt.sqtid AND rsq.rsqstatus = 'A' AND qts.qtsid = rsq.qtsid
                WHERE
                    d.esdid IN (733, 734, 756, 757)
                    AND sue.suestatus = 'A'
                    AND sqt.sqtdsc ILIKE '%outro%'
                ORDER BY o.obrid, sue.sueid, sos.sosnum, qst.qstid;
    ";

    $obras = $db->carregar($sql);
    $db->sql_to_xml_excel($obras, 'relatorio_obras_supervisao', $cabecalho);
    exit;
}

if ($_REQUEST['type'] == 'relatorio_obra_vinculada') {
    $cabecalho = array('ID', 'Pré-ID', 'Nome', 'Situação', 'UF', 'Município', 'Esfera', 'Unidade Implantadora', 'Nº Processo/Convênio', 'Programa', 'Fonte', 'Tipologia', 'ID Vinculada', 'Perc. Contrato Anterior', 'Perc. Atual', 'Percentual Total');
    $sql = "
                SELECT
                    o.obrid as \"ID\",
                    o.preid as \"Pré-ID\",
                    o.obrnome as \"Nome\",
                    et.esddsc as \"Situação\",
                    m.estuf as \"UF\",
                    m.mundescricao as \"Município\",
                    e.empesfera as \"Esfera\",
                    ent.entnome as \"Unidade Implantadora\",
                    CASE WHEN too.tooid = 2 THEN
                        o.obrnumprocessoconv
                    ELSE
                        po.numeroprocesso
                    END as \"Nº Processo/Convênio\",
                    prf.prfdesc as \"Programa\",
                    too.toodescricao as \"Fonte\",
                    tpo.tpodsc as \"Tipologia\",
                    o.obridvinculado as \"ID Vinculada\",
                    o.obrperccontratoanterior as \"Perc. Contrato Anterior\",
                    coalesce(o.obrpercentultvistoria, 0) as \"Perc. Atual\",
                    ((((100 - coalesce(o.obrperccontratoanterior,0)) * coalesce(o.obrpercentultvistoria,0)) / 100) + coalesce(o.obrperccontratoanterior,0))::numeric(20,2) as \"Percentual Total\"
                FROM obras2.obras o
                JOIN obras2.empreendimento e ON e.empid = o.empid
                JOIN entidade.endereco ed ON ed.endid = o.endid
                JOIN entidade.entidade ent ON ent.entid = e.entidunidade
                JOIN territorios.municipio m ON ed.muncod = m.muncod
                JOIN workflow.documento d ON d.docid = o.docid
                JOIN workflow.estadodocumento et ON et.esdid = d.esdid
                LEFT JOIN obras2.tipologiaobra tpo ON tpo.tpoid = o.tpoid
                LEFT JOIN obras2.programafonte prf ON prf.prfid  = e.prfid
                LEFT JOIN obras2.tipoorigemobra too ON e.tooid = too.tooid
                LEFT JOIN (SELECT po.pronumeroprocesso numeroprocesso , pop.preid
                    FROM par.processoobraspaccomposicao pop
                    JOIN par.processoobra po on po.proid = pop.proid and po.prostatus = 'A'
                    UNION
                    SELECT po.pronumeroprocesso numeroprocesso, pop.preid
                    FROM par.processoobrasparcomposicao pop
                    JOIN par.processoobraspar po on po.proid = pop.proid and po.prostatus = 'A') po ON po.preid = o.preid

                WHERE
                o.obridpai IS NULL
                AND o.obrstatus = 'A'
                AND o.obridvinculado IS NOT NULL
                ORDER BY m.estuf, m.mundescricao;
";

    $obras = $db->carregar($sql);
    $db->sql_to_xml_excel($obras, 'relatorio_obras_vinculadas', $cabecalho);
}


if ($_REQUEST['type'] == 'relatorio_ult_vist') {
    $sql = "
        SELECT
        oi.obrid as \"ID\",
        oi.obrnome as \"Obra\",
        pf.prfdesc as \"Programa\",
        too.toodescricao as \"Fonte\",
        ed.esddsc as \"Situação\",
        (SELECT ( SELECT
                              CASE
                                  WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                  ELSE 0::numeric
                              END AS total
                               FROM obras2.itenscomposicaoobra i
                                LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                               WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND i.obrid = o.obrid) AS percentual
        FROM obras2.obras o
        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
        WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.obrid = oi.obrid
        ORDER BY o.obrid, s.supdata DESC
        OFFSET 0
        LIMIT 1) as \"Perc. Última Vist.\",

        (SELECT ( SELECT
                              CASE
                                  WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                  ELSE 0::numeric
                              END AS total
                               FROM obras2.itenscomposicaoobra i
                                LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                               WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND i.obrid = o.obrid) AS percentual
        FROM obras2.obras o
        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
        WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.obrid = oi.obrid
        ORDER BY o.obrid, s.supdata DESC
        OFFSET 1
        LIMIT 1) as \"Perc. Penúltima Vist.\",

        (SELECT ( SELECT
                              CASE
                                  WHEN sum(i.icovlritem) > 0::numeric THEN round(sum(sic.spivlrfinanceiroinfsupervisor) / sum(i.icovlritem) * 100::numeric, 2)
                                  ELSE 0::numeric
                              END AS total
                               FROM obras2.itenscomposicaoobra i
                                LEFT JOIN obras2.supervisaoitem sic ON sic.icoid = i.icoid AND sic.supid = s.supid AND sic.icoid IS NOT NULL AND sic.ditid IS NULL
                               WHERE i.icostatus = 'A'::bpchar AND i.relativoedificacao = 'D'::bpchar AND i.obrid = o.obrid) AS percentual
        FROM obras2.obras o
        JOIN obras2.supervisao s ON s.emsid IS NULL AND s.smiid IS NULL AND s.supstatus = 'A'::bpchar AND s.validadapelosupervisorunidade = 'S'::bpchar AND s.obrid = o.obrid AND s.rsuid = 1
        WHERE o.obridpai IS NULL AND o.obrstatus = 'A' AND o.obrid = oi.obrid
        ORDER BY o.obrid, s.supdata DESC
        OFFSET 2
        LIMIT 1) as \"Perc. Antepenúltima Vist.\"

        FROM obras2.obras oi
        JOIN obras2.empreendimento ep ON ep.empid = oi.empid
        LEFT JOIN workflow.documento doc ON doc.docid = oi.docid
        LEFT JOIN workflow.estadodocumento 	 ed ON ed.esdid = doc.esdid
        LEFT JOIN obras2.tipoorigemobra too ON too.tooid = oi.tooid
        LEFT JOIN obras2.programafonte pf ON pf.prfid = ep.prfid
        LEFT JOIN entidade.endereco edo on edo.endid = oi.endid
        LEFT JOIN territorios.municipio mun on mun.muncod = edo.muncod
        WHERE oi.obridpai IS NULL AND oi.obrstatus = 'A'

";

    $obras = $db->carregar($sql);
    $db->sql_to_excel($obras, 'relatorioEvolcao', $cabecalho, '');
}

/**
 * Arquivo responsável por gerar alguns relatórios que são solicitados em determinadas situações
 */

if ($_REQUEST['type'] == 'relatorio_cgu') {
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
              esd.esddsc

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
            WHERE o.obrstatus = 'A' AND o.obridpai IS NULL AND e.orgid IN(3) AND o.tpoid IN (104, 105)

            ORDER BY 2";

    $obras = $db->carregar($sql);

    foreach ($obras as $key => $obra) {
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

    ob_clean();
    ini_set("memory_limit", "1024M");
    header('content-type: text/html; charset=ISO-8859-1');

    $cabecalho = array(
        'ID',
        'ID Pré-Obra',
        'Obra',
        'Município',
        'UF',
        'Tipologia',
        'Data de Inclusão',
        'Situação');

    $db->sql_to_excel($obras, 'relatorioCGU', $cabecalho, '');
}


?>
