<?php
set_time_limit(0);

define( 'BASE_PATH_SIMEC', realpath( dirname( __FILE__ ) . '/../../../' ) );

$_REQUEST['baselogin']  = 'simec_espelho_producao';//simec_desenvolvimento
// $_REQUEST['baselogin']  = "simec_desenvolvimento";//simec_desenvolvimento

// carrega as funções gerais
require_once BASE_PATH_SIMEC . '/global/config.inc';

require_once APPRAIZ . 'includes/classes_simec.inc';
require_once APPRAIZ . 'includes/funcoes.inc';
include_once APPRAIZ . 'includes/classes/Modelo.class.inc';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = 194;

$db = new cls_banco();

$strSQL = "
    SELECT DISTINCT
        vTable.tcpid,
        vTable.prazo_expirado,
        vTable.deadline,
        vTable.proponente,
        vTable.concedente,
        vTable.ungcodpoliticafnde,
        vTable.esdid,
        vTable.cooid
    FROM (
        SELECT
            (SELECT tcpid FROM ted.termocompromisso WHERE docid = hd.docid) AS tcpid,
            (SELECT cooid FROM ted.termocompromisso WHERE docid = hd.docid) AS cooid,
            CASE WHEN DATE_PART('days', NOW() - hd.htddata) = 60 THEN
                FALSE
            WHEN DATE_PART('days', NOW() - hd.htddata) = 30 THEN
                FALSE
            WHEN DATE_PART('days', NOW() - hd.htddata) = 15 THEN
                FALSE
            WHEN DATE_PART('days', NOW() - hd.htddata) = 5 THEN
                FALSE
            WHEN DATE_PART('days', NOW() - hd.htddata) > 60 THEN
                TRUE
            END AS prazo_expirado,
            TO_CHAR(hd.htddata + INTERVAL '60' DAY, 'DD/MM/YYYY') AS deadline,
            (SELECT ungcodproponente FROM ted.termocompromisso WHERE tcpid = (SELECT tcpid FROM ted.termocompromisso WHERE docid = hd.docid)) AS proponente,
            (SELECT ungcodconcedente FROM ted.termocompromisso WHERE tcpid = (SELECT tcpid FROM ted.termocompromisso WHERE docid = hd.docid)) AS concedente,
            (SELECT ungcodpoliticafnde FROM ted.termocompromisso WHERE tcpid = (SELECT tcpid FROM ted.termocompromisso WHERE docid = hd.docid)) AS ungcodpoliticafnde,
            (SELECT esdid FROM workflow.documento where docid = hd.docid) as esdid
        FROM
            workflow.historicodocumento hd
        WHERE
            aedid = 1652
        ORDER BY hstid DESC
    ) vTable
    WHERE vTable.prazo_expirado = 'f' AND vTable.esdid <> 640
    ORDER BY vTable.tcpid
";

//ver($strSQL,d);
$termos = $db->carregar($strSQL);

$strSQL = "
    select distinct
        u.usuemail, ur.ungcod
    from ted.usuarioresponsabilidade ur
    join seguranca.usuario u on (u.usucpf = ur.usucpf)
    join seguranca.usuario_sistema us on (us.usucpf = u.usucpf)
    where ur.pflcod = 1265 --Perfil Coordenador da Secretaria Autarquia
    and ur.rpustatus = 'A'
    and ur.ungcod = '%s'
    and ur.cooid = %d
    and us.suscod = 'A'
";

$emailOptions = array();
$emailOptions['remetente'] = array('nome' => SIGLA_SISTEMA. ' - Termo de Execução Descentralizada', 'email' => $_SESSION['email_sistema']);
$emailOptions['email_simec_dev'] = $_SESSION['email_sistema'];
$emailOptions['assunto'] = 'Aviso - TED: {#tcpid} - Coordenador, atenção ao prazo de aprovação do Relatório de Cumprimento do Objeto';
$emailOptions['cc'] = null;
$emailOptions['conteudo'] = '
    <table witdh="100%" cellspacing="2" cellpadding="2">
        <tr>
            <td>Prezado Coordenador,</td>
        </tr>
        <tr>
            <td>O prazo de aprovação do Relatório de Cumprimento do Objeto
            por parte da Coordenação, chega ao seu fim em: {#deadline}</td>
        </tr>
        <tr>
            <td>
                <p>
                    Atenciosamente,<br />
                    CGSO/SPO/SE<br />
                    Ministério da Educação<br />
                </p>
            </td>
        </tr>
    </table>
';

$errors = $oks = array();


if (is_array($termos)) {

    foreach ($termos as $ted) {

        /**
         * Secretarias do MEC
         * SETEC, SEB, SECADI
         */
        $secretaria_mec = array(
            '150028',
            '150016',
            '150019'
        );

        /**
         * Se o concedente for FNDE e a politica for algumas das secretarias acima (array $secretaria_mec)
         * aplicar regra abaixo
         */
        $concedente = $ted['concedente'];
        if ($ted['concedente'] == '153173' && in_array($ted['ungcodpoliticafnde'], $secretaria_mec)) {
            $concedente = $ted['ungcodpoliticafnde'];
        }

        $usuarios = $db->carregar(sprintf($strSQL, $concedente, $ted['cooid']));

        //ver($usuarios);
        $assunto = str_replace('{#tcpid}', $ted['tcpid'], $emailOptions['assunto']);
        $conteudo = str_replace(
            array('{#deadline}')
          , array($ted['deadline'])
          , $emailOptions['conteudo']
        );

        if (is_array($usuarios)) {
            foreach ($usuarios as $row) {
                if (enviar_email($emailOptions['remetente'], $row['usuemail'], $assunto, $conteudo, $emailOptions['cc'], $emailOptions['email_simec_dev'])) {
                    $oks[] = true;
                } else {
                    $errors[] = true;
                }
            }
        }
    }

    ver($oks);
    ver($errors);
    ver(date('d-m-Y H:i:s', time()));
}

$db->close();