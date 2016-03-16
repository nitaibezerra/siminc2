<?php 
/**
* This script will be the first to be initiated
* It will call Google
*/

require_once('oauth2_config_anonimo.php');

//construct POST object for access token fetch request
$post = array('client_id' => CLIENT_ID,
              'client_secret' => CLIENT_SECRET,
              'grant_type' => 'client_credentials');

//get JSON access token object (with refresh_token parameter)
$token = json_decode(runCurl(ACCESS_TOKEN_ENDPOINT, 'POST', $post));

//##################################################
//codigo do cliente
//##################################################

$objeto = array("Description" => "Teste de Cadastro 15"
				, "Loss" => "3"
				, "Title" => "Teste de Cadastro 15"
				, "Urgency" => "2");

//fetch profile of current user
$codigo_evento = runCurlJson(WF_CREATE_EVENT, 'POST', simec_json_encode($objeto), $token->access_token);

var_dump($codigo_evento);



//##################################################
?>