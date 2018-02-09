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

session_start();
 
// CPF do administrador de sistemas
$_SESSION['usucpforigem'] 	= '00000000191';
$_SESSION['usucpf'] 		= '00000000191';
 
$wsusuario	= 'USAP_WS_SIGARP';
$wssenha	= '03422625';

$cont 		= $_REQUEST['cont'];
$sistema	= $_REQUEST['sistema'];

$db = new cls_banco();

$sql = "select pagamento from par.pagamento_temp where codigo = $cont and sistema = '$sistema'";
$ar = $db->pegaLinha($sql);

$sqlu = "UPDATE par.pagamento_temp SET dataini = now() WHERE codigo = $cont and sistema = '$sistema'";
$db->executar($sqlu);
$db->commit();

$arrParam = array(
				'sistema' 	=> $sistema,
				'wsusuario' => $wsusuario,
				'wssenha' 	=> $wssenha,
				'offset'	=> $cont,
				'pagamento'	=> $ar['pagamento']
			);

if( $ar['pagamento'] ){
	$obWS = new Consulta_Empenho_Pagamento_WS( $arrParam );
	$obWS->consultaPagamento();
}

$sql = "UPDATE par.pagamento_temp SET datafim = now() WHERE codigo = $cont and sistema = '$sistema'";
$db->executar($sql);

$sql = "UPDATE par.pagamento_temp SET tempoexec = (select datafim - dataini from par.pagamento_temp where codigo = $cont and sistema = '$sistema') WHERE codigo = $cont and sistema = '$sistema'";
$db->executar($sql);
$db->commit();

$db->close();

?>