<?php

print "cuidado";
exit();

set_time_limit( 0 );
require_once( 'SnoopyBufferSocket.class.php' );

$objNewSnoopy =  new SnoopyBufferSocket();
$objNewSnoopy->_isproxy = true;
$objNewSnoopy->proxy_host = 'proxy.mec.gov.br';
$objNewSnoopy->proxy_port = '8080';
$objNewSnoopy->proxy_user = 'thiagomata';
$objNewSnoopy->proxy_pass = 'bilunga';

$objNewSnoopy->changeOutputType( 'file' , 'resultado.txt' , true , true  );
$objNewSnoopy->sendFilesAsPostAttributes
( 
	"http://www.sigplan.gov.br/infrasig/INFRASIG.ASMX/recebeDadoFisico" 	, 
	Array( 'usuario' => 'leo.kessel' , 'senha' => 'kessel' )				, 
	Array( 'dados' => 'RetornaDadoFisico-12-01-07.xml' ) 
);

?>