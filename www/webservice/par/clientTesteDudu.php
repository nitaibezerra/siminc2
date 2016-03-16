<?php

ini_set("memory_limit", "3000M");
set_time_limit(0);

// Pull in the NuSOAP code
require_once('nusoap.php');

// Create the client instance
//$client = new soapcliente('http://simec-d.mec.gov.br/webservice/par/serverSimecSigarp.php?wsdl', true);
$client = new soapcliente('http://simec-d.mec.gov.br/webservice/par/server.php?wsdl', true);

// Check for an error
$err = $client->getError();

if ($err) {
    // Display the error
    echo '<h2>Constructor error</h2><pre>' . $err . '</pre>';
    // At this point, you know the call that follows will fail
}


// Call the SOAP method
$autenticacao = $client->call('autenticarUsuario', array('cpf' => '','senha' => '221083'));

// Check for a fault
if ($client->fault) {
    echo '<h2>Fault</h2><pre>';
    print_r($result);
    echo '</pre>';
} else {
	/*
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
*/
}
/*
// Display the request and response
echo '<h2>Request</h2>';
echo '<pre>' . simec_htmlspecialchars($client->request, ENT_QUOTES) . '</pre>';
echo '<h2>Response</h2>';
echo '<pre>' . simec_htmlspecialchars($client->response, ENT_QUOTES) . '</pre>';
// Display the debug messages
echo '<h2>Debug</h2>';
echo '<pre>' . simec_htmlspecialchars($client->debug_str, ENT_QUOTES) . '</pre>';
*/

/*
echo '----------------------------------------------------------------------';

// Gravar uma categoria

$param = Array( 'PHPSESSID' 		=> $autenticacao, 
				'id_categoria' 		=> 3, 
				'ds_categoria'		=> 'Categoria 03');

// Call the SOAP method
$xx = $client->call('gravaCategoriaSigarp', $param );

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
*/

/*
echo '----------------------------------------------------------------------';

// Gravar um item
$param = Array( 'PHPSESSID' 		=> $autenticacao, 
				'id_item' 			=> 2810, 
				'ds_item' 			=> 'Item SIMEC 2',     			
				'ds_especificacao' 	=> 'Especificação do item teste 02',     			
				'id_categoria'		=> 308);

// Call the SOAP method
$xx = $client->call('gravaItemSigarp', $param );

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
/*

// Gravar os pregões
/*
$param = Array( 'PHPSESSID' 		=> $autenticacao, 
				'id_pregao' 		=> '232013', 
				'uf' 				=> array( 'AL', 'BA', 'CE', 'MA', 'PB', 'PE', 'PI', 'RN', 'SE' ),     			
				'dt_inicio' 		=> '2013-08-08',     			
				'dt_fim'	 		=> '2013-12-31',     			
				'id_item'			=> array( 1,2 )
				);
*/
/*
$param = Array( 'PHPSESSID' 		=> $autenticacao, 
				'nu_pregao' 		=> '09/2013', 
				'nu_seq_pregao' 	=> 969, 
				'dt_inicio' 		=> '2013-08-08',     			
				'dt_fim'	 		=> '2013-12-31',     			
				'regiao' 			=> array( 0 => array(
														'uf' => array('AL', 'BA', 'CE', 'MA', 'PB', 'PE', 'PI', 'RN', 'SE'),
														'item' => array(
																		0 => array(
																				'id_item' => 2809,
																				'vlr_item' => 111.10
													  								),
													  					1 => array(
																				'id_item' => 2810,
																				'vlr_item' => 222.11
													  								),	
														)
													),
												  1 => array(
										  				'uf' => array('AC','AP','AM','ES','GO','MT','MS','MG','PA','PR','RJ','RS','RO','RR','SC','SP','TO','DF'),
														'item' => array(
													  					0 => array(
																				'id_item' => 2809,
																				'vlr_item' => 456.45
													  								),
													  					1 => array(
																				'id_item' => 2810,
																				'vlr_item' => 789.45
													  								),		
														)
											  		) 
												  )   			
				);
// Call the SOAP method
$xx = $client->call('gravaPregaoSigarp', $param );

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
*/
/*

// Verifica Secretário


$param = Array( 'PHPSESSID' 		=> $autenticacao, 
				'cpf'		 		=> '', //
				'uf'				=> 'AC', // AC
				'ibge'				=> '4209409' // 4209409
				);
				
				
// Call the SOAP method
$xx = $client->call('verificaSecretario', $param );

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
*/

// Recupera todos os secretários


$param = Array( 'PHPSESSID' 		=> $autenticacao //, 
//				'esfera'		 	=> 'M'
				);
				
				
// Call the SOAP method
$xx = $client->call('retornaSecretarios', $param );

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


 
/*
// consultar Situação de Obra

$param = Array( 'PHPSESSID' 		=> $autenticacao, 
				'preid'				=> 10679,
				'CNPJ_FORNECEDOR' 	=> '',
				'NOME_FORNECEDOR' 	=> 'CASAALTA CONSTRUCOES LTDA',
				'COD_SITUACAO_FASE' => 22,
				'DT_ALTERACAO_FASE' => '2014-03-07T17:42:45',
				'SITUACAO_ADESAO' 	=> 'SOLICITAÇÃO DE ADESÃO CANCELADA'			
				);
// Call the SOAP method
$xx = $client->call('consultarSituacaoObra', $param );

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
*/
?>