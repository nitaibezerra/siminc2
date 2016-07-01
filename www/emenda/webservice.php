<?php
set_time_limit(30000);
ini_set("memory_limit", "3000M");

$_REQUEST['baselogin'] = "simec_espelho_producao";

// carrega as funções gerais
include_once "config.inc";
include_once "_funcoes.php";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
include_once APPRAIZ . 'includes/workflow.php';
require_once APPRAIZ . "includes/classes/Fnde_Webservice_Client.class.inc";
//require_once(APPRAIZ . 'www/webservice/painel/nusoap.php');


if(!$_SESSION['usucpf'])
	$_SESSION['usucpforigem'] = '';

// abre conexão com o servidor de banco de dados
$db = new cls_banco();

include '../../includes/nusoap/lib/nusoap.php';

$usuario = 'simec';
$senha = 'sisuab';
	$wsdl = "http://sisuab.homolog.capes.gov.br/sisuab/services/WebServiceSimec?wsdl";

	$configuracao = array(
		"encoding" => "ISO-8859-1",
		"compression" => SOAP_COMPRESSION_ACCEPT | SOAP_COMPRESSION_GZIP,
		"trace" => true
	);

	$conexao = new SoapClient( $wsdl, $configuracao );
	
	$parametros = array('user' => $usuario, 'senha' => $senha);
						
	//$objRetorno = $conexao->call("getEscola", $parametros);
	$objRetorno = $conexao->__call("consultaSisuabSimec", $parametros);
	
die;


$usuario = 'simec';
$senha = 'sisuab';
$urlWS = 'http://sisuab.homolog.capes.gov.br/sisuab/services/WebServiceSimec?wsdl';
	
$endpoint = $urlWS;
$wsdl = true;
$proxyhost = false;
$proxyport = false;
$proxyusername = false;
$proxypassword = false;
$timeout = 0;
$response_timeout = 30;

// Create the client instance
$client = new soapcliente($endpoint, $wsdl, $proxyhost, $proxyport, $proxyusername, $proxypassword, $timeout, $response_timeout);

// Check for an error
$err = $client->getError();

if ($err) {
    // Display the error
    echo '<h2>Constructor error</h2><pre>' . $err . '</pre>';
    // At this point, you know the call that follows will fail
}

$autenticacao = $client->call('consultaSisuabSimec', array('user' => $usuario, 'senha' => $senha) );

// Check for a fault
if ($client->fault) {
    echo '<h2>Fault</h2><pre>';
    print_r($result);
    echo '</pre>';
} else {
    // Check for errors
    $err = $client->getError();
    if ($err) {
        // Display the error
        echo '<h2>Error</h2><pre>' . $err . '</pre>';
    } else {
        // Display the result
        echo '<h2>Autenticacao</h2><pre>';
        print_r($autenticacao);
    echo '</pre>';
    }
}
// Display the request and response
echo '<h2>Request</h2>';
echo '<pre>' . simec_htmlspecialchars($client->request, ENT_QUOTES) . '</pre>';
echo '<h2>Response</h2>';
echo '<pre>' . simec_htmlspecialchars($client->response, ENT_QUOTES) . '</pre>';
// Display the debug messages
echo '<h2>Debug</h2>';
echo '<pre>' . simec_htmlspecialchars($client->debug_str, ENT_QUOTES) . '</pre>';

echo '----------------------------------------------------------------------';
?>