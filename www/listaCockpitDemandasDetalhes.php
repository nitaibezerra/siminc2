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

$sql = "select usunome, usucpf from seguranca.usuario where usucpf = '{$_REQUEST['usucpf']}'";
$usuario = $db->pegaLinha($sql);

//total demandas atrasadas
$sql = "SELECT
            d.dmdid,
            d.dmdtitulo,
            u.usunome,
            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatainiprevatendimento,
            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatafimprevatendimento,
            ed.esddsc,
            'atrasadas' as tipo
        FROM
            demandas.demanda as d
        LEFT JOIN
            workflow.documento doc ON doc.docid       = d.docid
        LEFT JOIN
            workflow.estadodocumento ed ON ed.esdid = doc.esdid
        LEFT JOIN
            seguranca.usuario u ON u.usucpf = d.usucpfanalise
        WHERE
            d.usucpfexecutor = '".$_REQUEST['usucpf']."'
            AND d.usucpfdemandante is not null
            AND d.dmdstatus = 'A'
            AND ed.esdstatus = 'A'
            AND doc.esdid in (91,92,107,108)
            AND d.dmddatafimprevatendimento < CURRENT_DATE
            and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

        union

        SELECT
            d.dmdid,
            d.dmdtitulo,
            u.usunome,
            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatainiprevatendimento,
            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatafimprevatendimento,
            ed.esddsc,
            'hoje' as tipo
        FROM
            demandas.demanda as d
        LEFT JOIN
            workflow.documento doc ON doc.docid       = d.docid
        LEFT JOIN
            workflow.estadodocumento ed ON ed.esdid = doc.esdid
        LEFT JOIN
            seguranca.usuario u ON u.usucpf = d.usucpfanalise
        WHERE
            d.usucpfexecutor = '".$_REQUEST['usucpf']."'
            AND d.usucpfdemandante is not null
            AND d.dmdstatus = 'A'
            AND ed.esdstatus = 'A'
            AND doc.esdid in (91,92,107,108)
            AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') = to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
            and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

        union

        SELECT
            d.dmdid,
            d.dmdtitulo,
            u.usunome,
            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatainiprevatendimento,
            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatafimprevatendimento,
            ed.esddsc,
            'avencer' as tipo
        FROM
            demandas.demanda as d
        LEFT JOIN
            workflow.documento doc ON doc.docid       = d.docid
        LEFT JOIN
            workflow.estadodocumento ed ON ed.esdid = doc.esdid
        LEFT JOIN
            seguranca.usuario u ON u.usucpf = d.usucpfanalise
        WHERE
            d.usucpfexecutor = '".$_REQUEST['usucpf']."'
            AND d.usucpfdemandante is not null
            AND d.dmdstatus = 'A'
            AND ed.esdstatus = 'A'
            AND doc.esdid in (91,92,107,108)
            AND to_char(d.dmddatafimprevatendimento::date,'YYYY-MM-DD HH24:MI:SS') > to_char(CURRENT_DATE::date,'YYYY-MM-DD HH24:MI:SS')
            and d.dmdid not in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )

        union

        SELECT
            d.dmdid,
            d.dmdtitulo,
            u.usunome,
            to_char(d.dmddatainiprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatainiprevatendimento,
            to_char(d.dmddatafimprevatendimento,'DD/MM/YYYY HH24:MI:SS') as dmddatafimprevatendimento,
            ed.esddsc,
            'pausadas' as tipo
        FROM
            demandas.demanda as d
        LEFT JOIN
            workflow.documento doc ON doc.docid       = d.docid
        LEFT JOIN
            workflow.estadodocumento ed ON ed.esdid = doc.esdid
        LEFT JOIN
            seguranca.usuario u ON u.usucpf = d.usucpfanalise
        WHERE
            d.usucpfexecutor = '".$_REQUEST['usucpf']."'
            AND d.usucpfdemandante is not null
            AND d.dmdstatus = 'A'
            AND ed.esdstatus = 'A'
            AND doc.esdid in (91,92,107,108)
            AND d.dmdid in ( select dmdid from demandas.pausademanda where pdmdatafimpausa is null group by dmdid )
        ";
$dados = $db->carregar( $sql );

if($dados){
    foreach($dados as $count => $dado){
        $dados[$count]['usunome'] = utf8_encode($dados[$count]['usunome']);
        $dados[$count]['dmdtitulo'] = utf8_encode($dados[$count]['dmdtitulo']);
        $dados[$count]['nome'] = utf8_encode($usuario['usunome']);
        $dados[$count]['cpf'] = $usuario['usucpf'];
    }
}

echo simec_json_encode($dados);