<?php

include_once "config.inc";
include_once APPRAIZ . "includes/funcoes.inc";
include_once APPRAIZ . "includes/classes_simec.inc";
//include_once APPRAIZ . "www/temp/webservice/lib_blaine/nusoap.php";
//include_once APPRAIZ . "www/temp/webservice/lib_blaine/class.soapclient.php";


$client = new SoapClient('http://example.com/webservice.php?wsdl');
/*
$client->__setLocation('http://www.somethirdparty.com');

$old_location = $client->__setLocation(); // unsets the location option

echo $old_location;
*/
?>