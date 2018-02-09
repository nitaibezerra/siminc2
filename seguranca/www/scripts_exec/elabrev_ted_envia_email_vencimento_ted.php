<?php
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../'));

$_REQUEST['baselogin']  = 'simec_espelho_producao';

// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';
require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';
include_once APPRAIZ . 'www/ted/_constantes.php';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 194;

$db = new cls_banco();

function PegaEmailsDestinatarios($usucpfs) {
    global $db;

    if (!$usucpfs)
        return false;

    $emails = array();
    foreach ($usucpfs as $u) {
        if ($email = $db->pegaUm("select usuemail from seguranca.usuario where usucpf = '{$u['usucpf']}'")) {
            array_push($emails, $email);
        }
    }

    return (count($emails)) ? $emails : false;
}

$strSQL = "
    WITH tmp_ted_historico AS (
        SELECT htddata, docid, hstid, aedid, usucpf
        FROM workflow.historicodocumento WHERE aedid IN (1609, 1618, 2440)
    )
    SELECT
        vTable.tcpid,
        vTable.unidadegestorap,
        vTable.unidadegestorac,
        coalesce(vTable.identificacao, ' - ') as titulo_obj_despesa,
        vTable.esddsc,
        vTable.coodsc,
        vTable.vigencia
    FROM (
        SELECT DISTINCT
            tcp.tcpid,
            unp.ungcod as unidadegestorap,
            unc.ungcod as unidadegestorac,
            jv.identificacao,
            esd.esddsc as esddsc,
            coalesce(cdn.coodsc, '-') as coodsc,
            (select
                    case when a.vigdata is not null then
                        a.vigdata
                    when t.dtvigenciafinal is not null then
                        t.dtvigenciafinal
                    else
                        null
                    end as vigencia
                from ted.termocompromisso t
                left join ted.aditivovigencia a on (a.tcpid = t.tcpid)
                where t.tcpid = tcp.tcpid
                order by a.vigid desc limit 1) as vigencia
        FROM ted.termocompromisso tcp

        LEFT JOIN ted.coordenacao cdn            ON cdn.cooid = tcp.cooid
        LEFT JOIN public.unidadegestora unp      ON unp.ungcod = tcp.ungcodproponente
        LEFT JOIN public.unidadegestora unc      ON unc.ungcod = tcp .ungcodconcedente
        LEFT JOIN ted.representantelegal rpp     ON rpp.ug = tcp.ungcodproponente
        LEFT JOIN ted.representantelegal rpc     ON rpc.ug = tcp.ungcodconcedente
        LEFT JOIN workflow.documento doc         ON doc.docid = tcp.docid
        LEFT JOIN workflow.estadodocumento esd   ON esd.esdid = doc.esdid
        LEFT JOIN ted.justificativa jv           ON (jv.tcpid = tcp.tcpid)
        WHERE
            tcp.tcpid in (
                select distinct tc.tcpid from ted.termocompromisso tc
                left join ted.previsaoorcamentaria po on tc.tcpid = po.tcpid
                where tcpstatus = 'A'
            )

        ORDER BY tcpid DESC
    ) vTable
    WHERE
	CASE WHEN vTable.vigencia IS NOT NULL AND vTable.vigencia > now() then
		DATE_PART('days', vTable.vigencia - NOW()) = %d
	END
";

$termos60dias = sprintf($strSQL, 60);
$termos45dias = sprintf($strSQL, 45);

$remetente = array('nome' => SIGLA_SISTEMA. ' - Termo de Execução Descentralizada', 'email' => $_SESSION['email_sistema']);;
$assunto = 'Aviso de %s dias para vencimento do TED - %s';
$conteudo = "
    <p>Prezado, <br/>
        Faltam %s dias para encerrar o prazo do Termo de Execução Descentralizada %s.<br/>
        A vigência do Termo poderá ser prorrogada até %s dias antes do seu término.
    </p>
    <p>Atenciosamente,<br/>
        CGSO/SPO/SE<br/>
        Ministério da Educação
    </p>
";
$emailCC = null;
$emailCCO = null;
$envios = 0;

/**
 * Envio para 60 dias do vencimento do TED
 */
$teds60 = $db->carregar($termos60dias);
if ($teds60) {
    foreach ($teds60 as $k => $linha) {

        $unidades = "'{$linha['unidadegestorap']}', '{$linha['unidadegestorac']}'";

        $strSQL = "
            select distinct ur.usucpf from ted.usuarioresponsabilidade ur
            join seguranca.usuario_sistema us on (us.usucpf = ur.usucpf)
            where ur.prsano = '%s' and ur.pflcod = 1271 and ur.ungcod in (%s) and us.suscod = 'A'
        ";
        $strSQL = sprintf($strSQL, $_SESSION['exercicio'], $unidades);
        $tecnicos = $db->carregar($strSQL);
        $emails = PegaEmailsDestinatarios($tecnicos);

        $strSQL = "
            select distinct ur.usucpf from ted.usuarioresponsabilidade ur
            join seguranca.usuario_sistema us on (us.usucpf = ur.usucpf)
            where ur.prsano = '%s' and ur.pflcod = 1266 and ur.ungcod = '%s' and us.suscod = 'A'
        ";
        $strSQL = sprintf($strSQL, $_SESSION['exercicio'], $linha['unidadegestorac']);
        $diretoriaAutarquia = $db->carregar($strSQL);
        $emails2 = PegaEmailsDestinatarios($diretoriaAutarquia);

        $arrEmails = array_merge(is_array($emails) ? $emails : array(), is_array($emails2)? $emails2 : array());
        if (!$arrEmails) return false;

        //ver($arrEmails);
        foreach ($arrEmails as $email) {
            $assunto = sprintf($assunto, 60, $linha['tcpid'], 30);
            $conteudo = sprintf($conteudo, 60, $linha['tcpid'], 30);

            if (enviar_email($remetente, $email, $assunto, $conteudo, $emailCC, $emailCCO))
                $envios++;
        }
    }
}

/**
 * Envio para 45 dias do vencimento do TED
 */
$teds45 = $db->carregar($termos45dias);
if ($teds45) {
    foreach ($teds45 as $k => $linha) {

        $unidades = "'{$linha['unidadegestorap']}', '{$linha['unidadegestorac']}'";

        $strSQL = "
            select distinct ur.usucpf from ted.usuarioresponsabilidade ur
            join seguranca.usuario_sistema us on (us.usucpf = ur.usucpf)
            where ur.prsano = '%s' and ur.pflcod = 1271 and ur.ungcod in (%s) and us.suscod = 'A'
        ";
        $strSQL = sprintf($strSQL, $_SESSION['exercicio'], $unidades);
        $tecnicos = $db->carregar($strSQL);
        $emails = PegaEmailsDestinatarios($tecnicos);

        $strSQL = "
            select distinct ur.usucpf from ted.usuarioresponsabilidade ur
            join seguranca.usuario_sistema us on (us.usucpf = ur.usucpf)
            where ur.prsano = '%s' and ur.pflcod = 1266 and ur.ungcod = '%s' and us.suscod = 'A'
        ";
        $strSQL = sprintf($strSQL, $_SESSION['exercicio'], $linha['unidadegestorac']);
        $diretoriaAutarquia = $db->carregar($strSQL);
        $emails2 = PegaEmailsDestinatarios($diretoriaAutarquia);

        $arrEmails = array_merge(is_array($emails) ? $emails : array(), is_array($emails2)? $emails2 : array());
        if (!$arrEmails) return false;

        //ver($arrEmails);
        foreach ($arrEmails as $email) {
            if (!empty($email)) {
                $assunto = sprintf($assunto, 45, $linha['tcpid'], 30);
                $conteudo = sprintf($conteudo, 45, $linha['tcpid'], 30);

                if (enviar_email($remetente, $email, $assunto, $conteudo, $emailCC, $emailCCO))
                    $envios++;
            }
        }
    }
}

ver('60/45 DIAS ==> e-mails para tecnicos: ' . $envios);
$db->close();
