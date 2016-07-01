<?php
$fp = fsockopen("www.example.com", 80, $errno, $errstr, 30);
if (!$fp) {
   echo "$errstr ($errno)<br />\n";
} else {
   $out = "GET / HTTP/1.1\r\n";
   $out .= "Host: www.example.com\r\n";
   $out .= "Connection: Close\r\n\r\n";

   fwrite($fp, $out);
   while (!feof($fp)) {
       echo fgets($fp, 128);
   }
   fclose($fp);
}
?> 
<?
exit();

	include "config.inc";
	include APPRAIZ . "includes/classes_simec.inc";
	include APPRAIZ . "includes/funcoes.inc";
	include APPRAIZ . "includes/SigplanCliente.php";
	
	$objTemp = new SigplanCliente( 'leo.kessel' , 'kessel' );
	$objTemp->usarProducao();
	
	$objCurl = new Curl_HTTP_Client( true );
//	print file_get_contents("http://www.google.com",FALSE,NULL,0,20);
//	print file_get_contents( 'http://www.sigplan.gov.br/infrasig/sigtoinfra.asmx/geracaoPorUO' );
//	print $objCurl->send_post_data( 'http://www.sigplan.gov.br/infrasig/sigtoinfra.asmx/geracaoPorUO?usuario=leo.kessel&senha=kessel&PRGAno=2006&UNICod=1234' , Array() );	
	//print $objTemp->executar( 'geracaoPorUO' , Array( 'PRGAno' => '2006' , 'UNICod' => '1234' ) );
?>