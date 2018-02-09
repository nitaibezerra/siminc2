<?php
ini_set( 'display_errors', 1 );
ini_set("memory_limit", "4024M");
set_time_limit(0);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

//$_REQUEST['baselogin'] = "simec_espelho_producao"; //simec_desenvolvimento
// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
require_once APPRAIZ . "includes/classes_simec.inc";
require_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/human_gateway_client_api/HumanClientMain.php";
require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';

// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
$_SESSION['sisid'] = '23';


$db = new cls_banco();


include_once APPRAIZ . 'www/par/_constantes.php';
include_once APPRAIZ . 'www/par/_funcoes.php';
include_once APPRAIZ . 'www/par/_componentes.php';
include_once APPRAIZ . "www/autoload.php";
include_once APPRAIZ . "includes/classes/dateTime.inc";

$msg = "";
$diff = array();

$cTmp = "preid, obrid, esfera, uf, muncod, nomeobra, processo, valorempenho, valorcancelado, valorreforco, saldo, tipo";
$dataS = "TO_CHAR(NOW(), 'YYMMDDHH24' ) as data";
$dataW = "data = TO_CHAR(NOW(), 'YYMMDDHH24' )";

try{
    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'HH');

    //$isset = $db->pegaUm("SELECT count(*) tot FROM par.vm_saldo_empenho_por_obra_tmp WHERE $dataW");

    //if($data == '18' || $isset > 0)
    executar(" TRUNCATE par.vm_saldo_empenho_por_obra_tmp ");

    executar(" INSERT INTO par.vm_saldo_empenho_por_obra_tmp (SELECT es.preid, o.obrid, p.preesfera AS esfera, p.estuf AS uf, p.muncod, p.predescricao AS nomeobra, e.empnumeroprocesso AS processo, sum(es.eobvalorempenho) AS valorempenho, sum(ep.vrlcancelado) AS valorcancelado, sum(er.vlrreforco) AS valorreforco, sum(es.eobvalorempenho + COALESCE(er.vlrreforco, 0::numeric) - COALESCE(ep.vrlcancelado, 0::numeric)) AS saldo, 'PAC' AS tipo, $dataS
															           FROM par.empenho e
															      JOIN par.empenhoobra es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
															   JOIN obras.preobra p ON p.preid = es.preid
															   LEFT JOIN obras2.obras o ON o.preid = p.preid AND o.obrstatus = 'A'::bpchar AND o.obridpai IS NULL
															   LEFT JOIN ( SELECT e.empnumeroprocesso, e.empidpai, es.preid, sum(es.eobvalorempenho) AS vrlcancelado
															    FROM par.empenho e
															  JOIN par.empenhoobra es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
															  WHERE (e.empcodigoespecie = ANY (ARRAY['03'::bpchar, '13'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
															  GROUP BY e.empnumeroprocesso, e.empidpai, es.preid) ep ON ep.empidpai = e.empid AND ep.preid = es.preid
															   LEFT JOIN ( SELECT e.empnumeroprocesso, e.empidpai, es.preid, sum(es.eobvalorempenho) AS vlrreforco
															   FROM par.empenho e
															   JOIN par.empenhoobra es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
															  WHERE e.empcodigoespecie = '02'::bpchar AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
															  GROUP BY e.empnumeroprocesso, e.empidpai, es.preid) er ON er.empidpai = e.empid AND er.preid = es.preid
															  WHERE e.empsituacao <> 'CANCELADO'::bpchar AND (e.empcodigoespecie <> ALL (ARRAY['03'::bpchar, '13'::bpchar, '02'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar
															  GROUP BY es.preid, p.preesfera, p.estuf, p.muncod, p.predescricao, e.empnumeroprocesso, o.obrid
															UNION ALL 
															         SELECT es.preid, o.obrid, p.preesfera AS esfera, p.estuf AS uf, p.muncod, p.predescricao AS nomeobra, e.empnumeroprocesso AS processo, sum(es.eobvalorempenho) AS valorempenho, sum(ep.vrlcancelado) AS valorcancelado, sum(er.vlrreforco) AS valorreforco, sum(es.eobvalorempenho + COALESCE(er.vlrreforco, 0::numeric) - COALESCE(ep.vrlcancelado, 0::numeric)) AS saldo, 'PAR' AS tipo, $dataS
															           FROM par.empenho e
															      JOIN par.empenhoobrapar es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
															   JOIN obras.preobra p ON p.preid = es.preid
															   LEFT JOIN obras2.obras o ON o.preid = p.preid AND o.obrstatus = 'A'::bpchar AND o.obridpai IS NULL
															   LEFT JOIN ( SELECT e.empnumeroprocesso, e.empidpai, es.preid, sum(es.eobvalorempenho) AS vrlcancelado
															    FROM par.empenho e
															   JOIN par.empenhoobrapar es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
															  WHERE (e.empcodigoespecie = ANY (ARRAY['03'::bpchar, '13'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
															  GROUP BY e.empnumeroprocesso, e.empidpai, es.preid) ep ON ep.empidpai = e.empid AND ep.preid = es.preid
															   LEFT JOIN ( SELECT e.empnumeroprocesso, e.empidpai, es.preid, sum(es.eobvalorempenho) AS vlrreforco
															   FROM par.empenho e
															   JOIN par.empenhoobrapar es ON es.empid = e.empid AND es.eobstatus = 'A'::bpchar
															  WHERE e.empcodigoespecie = '02'::bpchar AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar
															  GROUP BY e.empnumeroprocesso, e.empidpai, es.preid) er ON er.empidpai = e.empid AND er.preid = es.preid
															  WHERE e.empsituacao <> 'CANCELADO'::bpchar AND (e.empcodigoespecie <> ALL (ARRAY['03'::bpchar, '13'::bpchar, '02'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar
															  GROUP BY es.preid, p.preesfera, p.estuf, p.muncod, p.predescricao, e.empnumeroprocesso, o.obrid) "); 
	
    /* $diff = $db->carregar("SELECT q1.obrid as obr1, q1.preid as preid1, q1.nomeobra as nome1, q1.processo as p1, q1.saldo as saldo1, 
    							  q2.obrid as obr2, q2.preid as preid2, q2.nomeobra as nome2, q2.processo as p2, q2.saldo as saldo2 
    						FROM par.vm_saldo_empenho_por_obra q1
                            FULL OUTER JOIN ( SELECT * FROM par.vm_saldo_empenho_por_obra_tmp WHERE $dataW ) q2 ON (q1.preid || '-' || q1.processo || '-' || q1.saldo = q2.preid || '-' || q2.processo|| '-' || q2.saldo)
                            --WHERE q2.saldo >= 0 IS NULL OR q1.saldo >= 0
    						"); */
    
    $sql = "SELECT preid, obrid, esfera, uf, muncod, nomeobra, processo, valorempenho, valorcancelado, valorreforco, saldo, tipo 
			FROM par.vm_saldo_empenho_por_obra_tmp 
			WHERE $dataW 
				AND preid || '-' || processo || '-' || saldo NOT IN (SELECT preid || '-' || processo || '-' || saldo 
																											FROM par.vm_saldo_empenho_por_obra) order by preid";
    $arDiff_tmp = $db->carregar($sql);
    
    $sql = "SELECT preid, obrid, esfera, uf, muncod, nomeobra, processo, valorempenho, valorcancelado, valorreforco, saldo, tipo 
			FROM par.vm_saldo_empenho_por_obra 
			WHERE preid || '-' || processo || '-' || saldo NOT IN (SELECT preid || '-' || processo || '-' || saldo 
																	FROM par.vm_saldo_empenho_por_obra_tmp 
			                                                        where $dataW) order by preid";
    $arDiff = $db->carregar($sql);

    executar(" INSERT INTO par.vm_saldo_empenho_por_obra($cTmp)
    	        (SELECT $cTmp FROM par.vm_saldo_empenho_por_obra_tmp WHERE $dataW AND preid || '-' || processo || '-' || saldo NOT IN (SELECT preid || '-' || processo || '-' || saldo FROM par.vm_saldo_empenho_por_obra)) ");

    executar(" DELETE FROM par.vm_saldo_empenho_por_obra WHERE preid || '-' || processo || '-' || saldo NOT IN ( SELECT preid || '-' || processo || '-' || saldo FROM par.vm_saldo_empenho_por_obra_tmp WHERE $dataW ) ");

    $msg .= "<br />";
    $msg .= "<table width='100%'";
    $msg .= "<tr style='text-align: center'><th colspan=5>Materialização da view de saldo empenho por obra feito com sucesso!</th></tr>
    		<tr><td width='50%'>
    			<table width='100%' border='1'>
    				<tr style='text-align: center'><th colspan=5>view_saldo_empenho_por_obra</th>
    				<tr style='text-align: center'>
    					<td>OBRID</td>
    					<td>PREID</td>
    					<td>OBRA</td>
    					<td>PROCESSO</td>
    					<td>SALDO</td>
    				</tr>";

    if($arDiff){
        foreach ($arDiff as $obras){
            $msg .= "<tr>
            			<td>{$obras['obrid']}</td><td>{$obras['preid']}</td><td>{$obras['nomeobra']}</td><td>{$obras['processo']}</td><td>{$obras['saldo']}</td>
            		</tr>";
        }
    } else {
    	$msg .= "<tr>
    				<td colspan=5>Sem alteração</td>
    			</tr>";
    }
    $msg .= "</table></td><td width='50%'>
    		<table width='100%' border='1'>
    			<tr style='text-align: center'><th colspan=5>view_saldo_empenho_por_obra_tmp</th>
    			<tr style='text-align: center'>
    				<td>OBRID</td>
    				<td>PREID</td>
    				<td>OBRA</td>
    				<td>PROCESSO</td>
    				<td>SALDO</td>
    			</tr>";
	if($arDiff_tmp){
        foreach ($arDiff_tmp as $obras){
            $msg .= "<tr>
            			<td>{$obras['obrid']}</td><td>{$obras['preid']}</td><td>{$obras['nomeobra']}</td><td>{$obras['processo']}</td><td>{$obras['saldo']}</td>
            		</tr>";
        }
    } else {
    	$msg .= "<tr>
    				<td colspan=5>Sem alteração</td>
    			</tr>";
    }
    
    $msg .= "</table></td></tr>";
    $msg .= "</table>";

} catch (Exception $e){
    $msg = "Ocorreu um erro durante a materialização <br /><br /> " . $e->getmessage();
}
$db->commit();
 
$destinatarios = array($_SESSION['email_sistema']);
/* $remetente = array("nome" => SIGLA_SISTEMA. " - PAR", "email" => $_SESSION['email_sistema']);
enviar_email($remetente, $destinatarios, "Materialização da view de saldo empenho", $msg, '', ''); */

$strAssunto = "Materialização da view de saldo empenho";
$strMensagem = $msg;
$remetente = array("nome"=>SIGLA_SISTEMA. " - VIEW_PAR", "email"=>"noreply@mec.gov.br");
$strEmailTo = array($_SESSION['email_sistema']);
$retorno =  enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', array() );

$db->close();
//ver($msg, 'FIM', d);

function executar($SQL)
{
    global $db;
    if (gettype( cls_banco::$link[$db->nome_bd] ) != "resource") {
        cls_banco::$link[$db->nome_bd] = null;
        cls_banco::cls_banco();
    }

    $SQL = trim($SQL);
    //detecta operacao e tabela (Insert, Update ou Delete)
    preg_match('/(CREATE\s+TABLE|ALTER\s+TABLE|DROP\s+TABLE|SELECT.*FROM|INSERT\s+INTO|UPDATE|DELETE\s+FROM)\s+([A-Za-z0-1.]+).*/smui', utf8_encode($SQL), $matches);
    $audtipoCompleto = strtoupper($matches[1]);
    $audtipo         = substr($audtipoCompleto, 0, 1);

    $_SESSION['sql'] = $SQL;

    // Inicia a transação quando nao estiver iniciada e obrigatoriamente quando
    // a operação for diferente de SELECT
    if (!isset($_SESSION['transacao']) && $audtipo != 'S') {
        $db->resultado = pg_query(cls_banco::$link[$db->nome_bd], 'begin transaction; ');
        $_SESSION['transacao'] = '1';
    }

    $db->resultado = @pg_query(cls_banco::$link[$db->nome_bd], $SQL);

    if ($db->resultado == null)
        throw new Exception( $SQL . pg_errormessage( cls_banco::$link[$db->nome_bd] ) );

    return $db->resultado;
}
