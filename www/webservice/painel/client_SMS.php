<?php
// Pull in the NuSOAP code
require_once('nusoap.php');

// Create the client instance
$client = new soapcliente('https://webservice.cgi2sms.com.br/axis/services/VolaSDKSecure?wsdl', true);

// Check for an error
$err = $client->getError();

if ($err) {
    // Display the error
    echo '<h2>Constructor error</h2><pre>' . $err . '</pre>';
    // At this point, you know the call that follows will fail
}

echo strlen("CEF informa Parabéns! Você ganhou na MegaSena acumulada, foi creditado R$35.000.000,00 na sua conta do BB");

// Call the SOAP method
$envio = $client->call('sendMessage', array('user' => 'inep',
											'password' => 'tmmjee', 
											'testMode' => true, 
											'sender' => 'CEF informa', 
											'target' => '556191526442', 
											'body' => 'Parabéns! Você ganhou na MegaSena acumulada, foi creditado R$35.000.000,00 na sua conta do BB', 
											'ID' => 'A1S2D3F4')
					  );
					  
print_r($client);

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
        echo '<h2>Result</h2><pre>';
        print_r($envio);
    echo '</pre>';
    }
}

echo "fim";
exit;

// Display the request and response
echo '<h2>Request</h2>';
echo '<pre>' . simec_htmlspecialchars($client->request, ENT_QUOTES) . '</pre>';
echo '<h2>Response</h2>';
echo '<pre>' . simec_htmlspecialchars($client->response, ENT_QUOTES) . '</pre>';
// Display the debug messages
echo '<h2>Debug</h2>';
echo '<pre>' . simec_htmlspecialchars($client->debug_str, ENT_QUOTES) . '</pre>';

echo '----------------------------------------------------------------------';

// Call the SOAP method
$formato = $client->call('pegarFormatoIndicadorCSV', array('PHPSESSID'=> $autenticacao,'indid' => '1'));

// Check for a fault
if ($client->fault) {
    echo '<h2>Fault</h2><pre>';
    print_r($formato);
    echo '</pre>';
} else {
    // Check for errors
    $err = $client->getError();
    if ($err) {
        // Display the error
        echo '<h2>Error</h2><pre>' . $err . '</pre>';
    } else {
        // Display the result
        echo '<h2>Result</h2><pre>';
        print_r($formato);
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

$dadosdetalhesserihistorica['agddataprocessamento'] = '23-09-2009';
$dadosdetalhesserihistorica['indid'] = '147';
$dadosdetalhesserihistorica['csvarray']= array(0 => 'id do indicador;id da periodicidade;quantidade;código do munícipio',
											   1 => '147;351;10;1100023',
						  					   2 => '147;352;10;1100023',
						  					   3 => '147;353;10;1100023',
						  					   4 => '147;354;10;1100023',
						  					   5 => '147;355;10;1100023',
						  					   6 => '147;356;10;1100023',
						  					   7 => '147;357;10;1100023');

// Call the SOAP method
$xx = $client->call('inserirAgendamentoSerieHistorica', array('PHPSESSID'=> $autenticacao,'dadosdetalhesserihistorica' => $dadosdetalhesserihistorica));

// Check for a fault
if ($client->fault) {
    echo '<h2>Fault</h2><pre>';
    print_r($xx);
    echo '</pre>';
} else {
    // Check for errors
    $err = $client->getError();
    if ($err) {
        // Display the error
        echo '<h2>Error</h2><pre>' . $err . '</pre>';
    } else {
        // Display the result
        echo '<h2>Result</h2><pre>';
        print_r($xx);
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