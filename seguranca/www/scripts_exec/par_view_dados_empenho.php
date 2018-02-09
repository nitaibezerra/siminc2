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

$cTmp = "empid, empnumero, processo, valorempenho, valorcancelado, valorreforco, saldo";
$dataS = "TO_CHAR(NOW(), 'YYMMDDHH24' ) as data";
$dataW = "data = TO_CHAR(NOW(), 'YYMMDDHH24' )";

try{
    $data = new Data();
    $data = $data->formataData($data->dataAtual(), 'HH');

    //$isset = $db->pegaUm("SELECT count(*) tot FROM par.vm_dadosempenhos_tmp WHERE $dataW");

    //if($data == '18' || $isset > 0)
     executar(" TRUNCATE par.vm_dadosempenhos_tmp ");

    executar("INSERT INTO par.vm_dadosempenhos_tmp (
			                empid,
			                empnumero,
			                processo,
			                valorempenho,
			                valorcancelado, 
			                valorreforco,
			                saldo, data) 
			 (SELECT e.empid, e.empnumero, e.empnumeroprocesso AS processo, e.empvalorempenho AS valorempenho, COALESCE(ep.vrlcancelado, 0::numeric) AS valorcancelado, COALESCE(er.vlrreforco, 0::numeric) AS valorreforco, e.empvalorempenho + COALESCE(er.vlrreforco, 0::numeric) - COALESCE(ep.vrlcancelado, 0::numeric) AS saldo, $dataS
			   FROM par.empenho e
			   LEFT JOIN ( SELECT empenho.empnumeroprocesso, empenho.empidpai, sum(empenho.empvalorempenho) AS vrlcancelado
			           FROM par.empenho
			          WHERE (empenho.empcodigoespecie = ANY (ARRAY['03'::bpchar, '13'::bpchar, '04'::bpchar])) AND empenho.empstatus = 'A'::bpchar AND empenho.empsituacao <> 'CANCELADO'::bpchar
			          GROUP BY empenho.empnumeroprocesso, empenho.empidpai) ep ON ep.empidpai = e.empid
			   LEFT JOIN ( SELECT empenho.empnumeroprocesso, empenho.empidpai, sum(empenho.empvalorempenho) AS vlrreforco
			      FROM par.empenho
			     WHERE empenho.empcodigoespecie = '02'::bpchar AND empenho.empstatus = 'A'::bpchar AND empenho.empsituacao <> 'CANCELADO'::bpchar
			     GROUP BY empenho.empnumeroprocesso, empenho.empidpai) er ON er.empidpai = e.empid
			  WHERE (e.empcodigoespecie <> ALL (ARRAY['03'::bpchar, '13'::bpchar, '02'::bpchar, '04'::bpchar])) AND e.empstatus = 'A'::bpchar AND e.empsituacao <> 'CANCELADO'::bpchar)");
	
    $sql = "SELECT empid, empnumero, processo, valorempenho, valorcancelado, valorreforco, saldo, DATA 
			FROM par.vm_dadosempenhos_tmp 
			WHERE $dataW 
				AND empid || '-' || processo || '-' || saldo NOT IN (SELECT empid || '-' || processo || '-' || saldo 
			    														FROM par.vm_dadosempenhos) order by empid";
    $arDiff_tmp = $db->carregar($sql);
    
    $sql = "SELECT empid, empnumero, processo, valorempenho, valorcancelado, valorreforco, saldo 
			FROM par.vm_dadosempenhos 
			WHERE 
				empid || '-' || processo || '-' || saldo NOT IN (SELECT empid || '-' || processo || '-' || saldo 
			    													FROM par.vm_dadosempenhos_tmp 
			                                                        WHERE $dataW) order by empid";
    $arDiff = $db->carregar($sql);

    executar(" INSERT INTO par.vm_dadosempenhos($cTmp)
    	        (SELECT $cTmp FROM par.vm_dadosempenhos_tmp WHERE $dataW AND empid || '-' || processo || '-' || saldo NOT IN (SELECT empid || '-' || processo || '-' || saldo FROM par.vm_dadosempenhos)) ");

    executar(" DELETE FROM par.vm_dadosempenhos WHERE empid || '-' || processo || '-' || saldo NOT IN ( SELECT empid || '-' || processo || '-' || saldo FROM par.vm_dadosempenhos_tmp WHERE $dataW ) ");

    $msg .= "<br />";
    $msg .= "<table width='100%'";
    $msg .= "<tr style='text-align: center'><th colspan=5>Materialização da view de dados de empenho feito com sucesso!</th></tr>
    		<tr><td width='50%'>
    			<table width='100%' border='1'>
    				<tr style='text-align: center'><th colspan=5>view_dados_empenho</th>
    				<tr style='text-align: center'>
    					<td>EMPID</td>
	    				<td>NUMERO</td>
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
    			<tr style='text-align: center'><th colspan=5>view_dados_empenho</th>
    			<tr style='text-align: center'>
    				<td>EMPID</td>
    				<td>NUMERO</td>
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
enviar_email($remetente, $destinatarios, "Materialização da view de dados de empenho", $msg, '', ''); */

$strAssunto = "Materialização da view de saldo empenho";
$strMensagem = $msg;
$remetente = array("nome"=>SIGLA_SISTEMA. " - VIEW_PAR", "email"=>"noreply@mec.gov.br");
$strEmailTo = array($_SESSION['email_sistema']);
$retorno =  enviar_email($remetente, $strEmailTo, $strAssunto, $strMensagem, '', '', array() );

$db->close();


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
