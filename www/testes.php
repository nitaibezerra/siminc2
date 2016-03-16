<?php
echo "<pre>";
$client = new SoapClient("https://homsigplan.serpro.gov.br/infrasig/sigtoinfra.asmx?WSDL");

var_dump($client->__getFunctions());


?>