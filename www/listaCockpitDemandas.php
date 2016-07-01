<?php
// Lista
header('Access-Control-Allow-Origin: *');
// carrega as bibliotecas internas do sistema

$_REQUEST['baselogin']  = "simec_espelho_producao";//simec_desenvolvimento

include_once "config.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "includes/funcoes.inc";

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

$tipo = 12;

if(1 == $tipo){

    // 2,48 ss
    $sql = "SELECT DISTINCT
                u.usucpf,
                u.usunome,
                (
                SELECT count(*) as qtd
                FROM demandas.demanda as d
                    LEFT JOIN workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE d.usucpfexecutor = u.usucpf
                AND d.usucpfdemandante is not null
                AND d.dmdstatus = 'A'
                AND ed.esdstatus = 'A'
                AND doc.esdid in (91,92,107,108)
                AND d.dmddatafimprevatendimento < CURRENT_DATE
                and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ) as atrasadas,
                (
                SELECT count(*) as qtd
                FROM demandas.demanda as d
                    LEFT JOIN workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE
                d.usucpfexecutor = u.usucpf
                AND d.usucpfdemandante is not null
                AND d.dmdstatus = 'A'
                AND ed.esdstatus = 'A'
                AND doc.esdid in (91,92,107,108)
                AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') = to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ) as nodia,
                (
                SELECT
                    count(*) as qtd
                FROM
                    demandas.demanda as d
                LEFT JOIN
                    workflow.documento doc ON doc.docid       = d.docid
                LEFT JOIN
                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE
                    d.usucpfexecutor = u.usucpf
                    AND d.usucpfdemandante is not null
                    AND d.dmdstatus = 'A'
                    AND ed.esdstatus = 'A'
                    AND doc.esdid in (91,92,107,108)
                    AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') > to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ) avencer,
                (
                SELECT
                    count(*) as qtd
                FROM
                    demandas.demanda as d
                LEFT JOIN
                    workflow.documento doc ON doc.docid       = d.docid
                LEFT JOIN
                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE
                    d.usucpfexecutor = u.usucpf
                    AND d.usucpfdemandante is not null
                    AND d.dmdstatus = 'A'
                    AND ed.esdstatus = 'A'
                    AND doc.esdid in (91,92,107,108)
                    AND d.dmdid in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ) pausadas
            FROM seguranca.usuario AS u
                INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
                INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
            WHERE
            ur.rpustatus = 'A' AND
            us.susstatus = 'A' AND
            us.suscod = 'A'
            and ur.pflcod in ('238')
            and ur.celid = 2
            ORDER BY u.usunome
    ";

    $dados = $db->carregar($sql);

} elseif(2 == $tipo) {

    // 1,55 ss
    $sql = "SELECT DISTINCT
                    u.usucpf,
                    u.usunome
                FROM
                    seguranca.usuario AS u
                INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
                INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
                WHERE
                    ur.rpustatus = 'A' AND
                    us.susstatus = 'A' AND
                    us.suscod = 'A'
                    and ur.pflcod in ('238')
                    and ur.celid = 2
                ORDER BY u.usunome";

    $usuarios = $db->carregar( $sql );

    foreach ($usuarios as $count => $usuario) {
        //total demandas atrasadas
        $sql = "SELECT
                    count(*) as qtd
                FROM
                    demandas.demanda as d
                LEFT JOIN
                    workflow.documento doc ON doc.docid       = d.docid
                LEFT JOIN
                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE
                    d.usucpfexecutor = '".$usuario['usucpf']."'
                    AND d.usucpfdemandante is not null
                    AND d.dmdstatus = 'A'
                    AND ed.esdstatus = 'A'
                    AND doc.esdid in (91,92,107,108)
                    AND d.dmddatafimprevatendimento < CURRENT_DATE
                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ";
        $atrasados = $db->PegaUm( $sql );

        //total demandas que vencem hoje
        $sql = "SELECT
                    count(*) as qtd
                FROM
                    demandas.demanda as d
                LEFT JOIN
                    workflow.documento doc ON doc.docid       = d.docid
                LEFT JOIN
                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE
                    d.usucpfexecutor = '".$usuario['usucpf']."'
                    AND d.usucpfdemandante is not null
                    AND d.dmdstatus = 'A'
                    AND ed.esdstatus = 'A'
                    AND doc.esdid in (91,92,107,108)
                    AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') = to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ";
        $emDia = $db->PegaUm( $sql );

        //total demandas em dia
        $sql = "SELECT
                    count(*) as qtd
                FROM
                    demandas.demanda as d
                LEFT JOIN
                    workflow.documento doc ON doc.docid       = d.docid
                LEFT JOIN
                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE
                    d.usucpfexecutor = '".$usuario['usucpf']."'
                    AND d.usucpfdemandante is not null
                    AND d.dmdstatus = 'A'
                    AND ed.esdstatus = 'A'
                    AND doc.esdid in (91,92,107,108)
                    AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') > to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                    and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                                ";
        $aVencer = $db->PegaUm( $sql );

        //total demandas pausadas
        $sql = "SELECT
                    count(*) as qtd
                FROM
                    demandas.demanda as d
                LEFT JOIN
                    workflow.documento doc ON doc.docid       = d.docid
                LEFT JOIN
                    workflow.estadodocumento ed ON ed.esdid = doc.esdid
                WHERE
                    d.usucpfexecutor = '".$usuario['usucpf']."'
                    AND d.usucpfdemandante is not null
                    AND d.dmdstatus = 'A'
                    AND ed.esdstatus = 'A'
                    AND doc.esdid in (91,92,107,108)
                    AND d.dmdid in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ";
        $pausadas = $db->PegaUm( $sql );

        $usuarios[$count]['atrasadas'] = $atrasados;
        $usuarios[$count]['nodia'] = $emDia;
        $usuarios[$count]['avencer'] = $aVencer;
        $usuarios[$count]['pausadas'] = $pausadas;
    }
} else {

    // 1,45 ss

    $sql = "SELECT DISTINCT
                    u.usucpf,
                    u.usunome
                FROM
                    seguranca.usuario AS u
                INNER JOIN demandas.usuarioresponsabilidade ur ON u.usucpf = ur.usucpf
                INNER JOIN seguranca.usuario_sistema us ON u.usucpf = us.usucpf
                WHERE
                    ur.rpustatus = 'A' AND
                    us.susstatus = 'A' AND
                    us.suscod = 'A'
                    and ur.pflcod in ('238')
                    and ur.celid = 2
                ORDER BY u.usunome";

    $usuarios = $db->carregar( $sql );

    foreach ($usuarios as $count => $usuario) {

        $sql = "SELECT sum(atrasadas) as atrasadas, sum(nodia) as nodia, sum(avencer) as avencer, sum(pausadas) as pausadas from
                (
                SELECT count(*) as atrasadas, 0 as nodia, 0 as avencer, 0 as pausadas
                    FROM
                        demandas.demanda as d
                    LEFT JOIN
                        workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN
                        workflow.estadodocumento ed ON ed.esdid = doc.esdid
                    WHERE
                        d.usucpfexecutor = '".$usuario['usucpf']."'
                        AND d.usucpfdemandante is not null
                        AND d.dmdstatus = 'A'
                        AND ed.esdstatus = 'A'
                        AND doc.esdid in (91,92,107,108)
                        AND d.dmddatafimprevatendimento < CURRENT_DATE
                        and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

                    union all

                    SELECT 0, count(*), 0, 0
                    FROM
                        demandas.demanda as d
                    LEFT JOIN
                        workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN
                        workflow.estadodocumento ed ON ed.esdid = doc.esdid
                    WHERE
                        d.usucpfexecutor = '".$usuario['usucpf']."'
                        AND d.usucpfdemandante is not null
                        AND d.dmdstatus = 'A'
                        AND ed.esdstatus = 'A'
                        AND doc.esdid in (91,92,107,108)
                        AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') = to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                        and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

                    union all

                    SELECT 0, 0, count(*), 0
                    FROM
                        demandas.demanda as d
                    LEFT JOIN
                        workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN
                        workflow.estadodocumento ed ON ed.esdid = doc.esdid
                    WHERE
                        d.usucpfexecutor = '".$usuario['usucpf']."'
                        AND d.usucpfdemandante is not null
                        AND d.dmdstatus = 'A'
                        AND ed.esdstatus = 'A'
                        AND doc.esdid in (91,92,107,108)
                        AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') > to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
                        and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

                    union all

                    SELECT 0, 0, 0, count(*)
                    FROM
                        demandas.demanda as d
                    LEFT JOIN
                        workflow.documento doc ON doc.docid       = d.docid
                    LEFT JOIN
                        workflow.estadodocumento ed ON ed.esdid = doc.esdid
                    WHERE
                        d.usucpfexecutor = '".$usuario['usucpf']."'
                        AND d.usucpfdemandante is not null
                        AND d.dmdstatus = 'A'
                        AND ed.esdstatus = 'A'
                        AND doc.esdid in (91,92,107,108)
                        AND d.dmdid in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
                ) as total
                ";

        $dados = $db->pegaLinha( $sql );
        $usuarios[$count]['usunome'] = utf8_encode($usuarios[$count]['usunome']);
        $usuarios[$count]['atrasadas'] = $dados['atrasadas'];
        $usuarios[$count]['nodia'] = $dados['nodia'];
        $usuarios[$count]['avencer'] = $dados['avencer'];
        $usuarios[$count]['pausadas'] = $dados['pausadas'];
    }
}

echo simec_json_encode($usuarios);