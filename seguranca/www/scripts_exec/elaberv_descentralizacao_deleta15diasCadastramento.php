<?php
// configurações
ini_set('memory_limit', '3000M');
set_time_limit(30000);

$_REQUEST['baselogin'] = 'simec_espelho_producao';

// carrega as funções gerais
//include_once "config.inc";
include_once '/var/www/simec/global/config.inc';
include_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes_simec.inc';

// CPF do administrador de sistemas
if (!$_SESSION['usucpf']) $_SESSION['usucpforigem'] = '00000000191';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include_once APPRAIZ . 'elabrev/www/_constantes.php';

$strSQL = "
    UPDATE ted.termocompromisso t
	  SET tcpstatus = 'I'
    WHERE t.docid NOT IN (
        SELECT DISTINCT h.docid FROM workflow.historicodocumento h WHERE h.docid = t.docid
    ) AND (
        ((SELECT count(*) FROM ted.previsaoorcamentaria WHERE
            tcpid = t.tcpid AND
            prostatus = 'A' AND
            ndpid IS NULL AND
            provalor IS NULL AND
            crdmesexecucao IS NULL) = 0)
        OR
        ((select count(*) from ted.previsaoorcamentaria where
            tcpid = t.tcpid AND
            prostatus = 'A' and
            ptrid is null and
            pliid is null and
            crdmesliberacao is null ) = 0)
    ) AND t.tcpid IN (
        SELECT DISTINCT tcp.tcpid FROM ted.termocompromisso tcp
        JOIN workflow.documento doc ON (doc.docid = tcp.docid)
        LEFT JOIN ted.arquivoprevorcamentaria apo ON (apo.tcpid = tcp.tcpid AND apo.arptipo = 'A')
        JOIN (
            SELECT j.tcpid FROM ted.justificativa j WHERE
            j.identificacao IS NULL OR j.objetivo IS NULL
        ) j2 ON (j2.tcpid = tcp.tcpid)
        JOIN (
            SELECT tcpid FROM ted.parecertecnico WHERE
            considentproponente IS NULL AND
            considproposta IS NULL AND
            considobjeto IS NULL AND
            considobjetivo IS NULL AND
            considjustificativa IS NULL AND
            considvalores IS NULL AND
            considcabiveis IS NULL
        ) pa ON (pa.tcpid = tcp.tcpid)
        WHERE now()::date-docdatainclusao::date > 15 AND
        (ungcodproponente IS NULL OR ungcodconcedente IS NULL OR apo.arqid IS NULL)
    )
";

$db->executar($strSQL);
$db->commit();
$db->close();
?>