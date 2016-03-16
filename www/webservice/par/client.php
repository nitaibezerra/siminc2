<?php

ini_set("memory_limit", "3000M");
set_time_limit(0);

// Pull in the NuSOAP code
require_once('nusoap.php');

// Create the client instance
$client = new soapcliente('http://simec-local/webservice/par/server.php?wsdl', true);

// Check for an error
$err = $client->getError();

if ($err) {
    // Display the error
    echo '<h2>Constructor error</h2><pre>' . $err . '</pre>';
    // At this point, you know the call that follows will fail
}


// Call the SOAP method
$autenticacao = $client->call('autenticarUsuario', array('cpf' => '','senha' => 'asenhaa'));

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


// Call the SOAP method
$xx = $client->call('pegarRelatorioPlanejamento', array('PHPSESSID'=> $autenticacao));

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