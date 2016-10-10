<?php

print phpinfo();
$client = new SoapClient( 'http://www.sigplan.gov.br/infrasig/sigtoinfra.asmx?wsdl' ,
	array
	(
		'proxy_host'    => "proxy.mec.gov.br",
		'proxy_port'    => 8080,
		'proxy_login'    => "thiagomata",
		'proxy_password' => "bilunga"
	)
);

$client->geracaoPorOrgao( array( 'usuario' => 'leo.kessel' , 'senha' => 'kessel' , 'PRGAno' => '2006', 'ORGCod' => CODIGO_ORGAO_SISTEMA ) );
//print_r($client->__getFunctions());

?>