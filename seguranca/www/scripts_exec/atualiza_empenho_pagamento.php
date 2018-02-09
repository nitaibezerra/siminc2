<?php
// echo 'aqui';
// die();
$_REQUEST['baselogin'] = "simec_espelho_producao";

/* configurações */
ini_set("memory_limit", "2048M");
set_time_limit(30000);

define('BASE_PATH_SIMEC', realpath(dirname(__FILE__) . '/../../../'));

// carrega as funções gerais
require_once BASE_PATH_SIMEC . "/global/config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . "par/classes/Consulta_Empenho_Pagamento_WS.class.inc";

function getmicrotime(){
	list($usec, $sec) = explode(" ", microtime());
 	return ((float)$usec + (float)$sec);
}

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] = '00000000191';
$_SESSION['usucpf'] = '00000000191';
 
$wsusuario	= 'USAP_WS_SIGARP';
$wssenha	= '03422625';

$limitEmp 	= $_REQUEST['limitEmp'];
$limitPag 	= $_REQUEST['limitPag'];
$cont 		= $_REQUEST['cont'];

$db = new cls_banco();

$sql = "select empenho from par.empenho_temp where codigo = $cont";
$ar = $db->pegaLinha($sql);

$sqlu = "UPDATE par.empenho_temp SET dataini = now() WHERE codigo = $cont";
$db->executar($sqlu);
$db->commit();

$arrParam = array(
				'sistema' 	=> 'PAC',
				'wsusuario' => $wsusuario,
				'wssenha' 	=> $wssenha,
				'offset'	=> $cont,
				'empenho'	=> $ar['empenho']
			);

$obWS = new Consulta_Empenho_Pagamento_WS( $arrParam );

$sql = "UPDATE par.empenho_temp SET datafim = now() WHERE codigo = $cont";
$db->executar($sql);

$sql = "UPDATE par.empenho_temp SET tempoexec = (select datafim - dataini from par.empenho_temp where codigo = $cont) WHERE codigo = $cont";
$db->executar($sql);
$db->commit();

$db->close();

die;

$Tinicio = getmicrotime();



//$db->commit();

$Tfinal= getmicrotime() - $Tinicio;

require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
$mensagem = new PHPMailer();
$mensagem->persistencia = $db;
$mensagem->Host         = "localhost";
$mensagem->Mailer       = "smtp";
$mensagem->FromName		= "SCRIPT AUTOMATICO";
$mensagem->From 		= $_SESSION['email_sistema'];
$mensagem->AddAddress( $_SESSION['email_sistema'], SIGLA_SISTEMA );
$mensagem->Subject = "Atualização do PAR - Conta Corrente, Empenho ou Pagamento";
$mensagem->Body = "<p>A atualização das Contas Correntes, Empenhos e Pagamentos foram realizados com sucesso! ".date("d/m/Y h:i:s")."</p>
				   <p>O tempo de execução das atualizações foi de ".$Tfinal." segundos</p>";

$mensagem->IsHTML( true );
$mensagem->Send();

?>