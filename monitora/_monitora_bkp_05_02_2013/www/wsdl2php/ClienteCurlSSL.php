<?php

class ClienteCurlSSL {
	
	function execute($url, $caminho_certificado, $senha_certificado) {
		echo $url;

		// Initialize session and set URL.
		$ch = curl_init();
		curl_setopt($ch, CURLOPT_URL, $url);
		
		// Desabilita a verificação da CA do servidor
		curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, false);
		
		// Set so curl_exec returns the result instead of outputting it.
		curl_setopt($ch, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($ch, CURLOPT_CERTINFO, true);
		
		//seta o caminho do arquivo do chave privada
//		curl_setopt($ch, CURLOPT_SSLKEY, $caminho_certificado);
//		curl_setopt($ch, CURLOPT_SSH_PRIVATE_KEYFILE, $caminho_certificado);
		curl_setopt($ch, CURLOPT_SSLCERT, $caminho_certificado);
//		curl_setopt($ch, CURLOPT_KEYPASSWD, $senha_certificado);
//		curl_setopt($ch, CURLOPT_SSLKEYPASSWD, $senha_certificado);
		curl_setopt($ch, CURLOPT_SSLCERTPASSWD, $senha_certificado); 
		
		// Get the response and close the channel.
		$response = curl_exec($ch);
		curl_close($ch);
		
		return $response;
		
	}
	
}
/*
echo "<pre>";
print_r(html_entity_decode(CurlClient::execute(
	"https://homologacao.siop.planejamento.gov.br/services/WSAlteracoesOrcamentarias?wsdl",
	dirname(__FILE__)."\mec.pem", 
	"mec2011")));
echo "</pre>";
*/

?>