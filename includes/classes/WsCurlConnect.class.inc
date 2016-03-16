<?php
class wsCurlConnect extends cls_banco{
	
	public function __construct(){
		// Setar o header para aceitar array como POST
		$headers = array(
				'Content-Type: multipart/form-data'
		);
	}
	
	public function __destruct(){
	}
	
	public function retornaResposta( $param, $wsdl ){
		
		$response = $this->executarServico( $param, $wsdl );
		// inicia XML parser, converte XML em Array e limpa obj XML parser;
		$p = xml_parser_create();
		xml_parse_into_struct($p, $response, $vals, $index);
		xml_parser_free($p);
		
		return $response;
	}
	
	public function retornaValores( $param, $wsdl ){
		
		$response = $this->executarServico( $param, $wsdl );
		// inicia XML parser, converte XML em Array e limpa obj XML parser;
		$p = xml_parser_create();
		xml_parse_into_struct($p, $response, $vals, $index);
		xml_parser_free($p);
		ver($p, $response, $vals, $index, d);
		return $vals;
	}
	
	public function retornaIndices( $param, $wsdl ){
		
		$response = $this->executarServico( $param, $wsdl );
		// inicia XML parser, converte XML em Array e limpa obj XML parser;
		$p = xml_parser_create();
		xml_parse_into_struct($p, $response, $vals, $index);
		xml_parser_free($p);
		
		return $index;
	}
	
	private function executarServico( $param, $wsdl ){
		
		// Inicio conexão Curl
		$handle = curl_init();
		
		// Setando parâmetros do Curl
		curl_setopt($handle, CURLOPT_URL, $wsdl);
		curl_setopt($handle, CURLOPT_RETURNTRANSFER, true);
		curl_setopt($handle, CURLOPT_SSL_VERIFYHOST, false);
		curl_setopt($handle, CURLOPT_SSL_VERIFYPEER, false);
		curl_setopt($handle, CURLOPT_POST, true);
		curl_setopt($handle, CURLOPT_POSTFIELDS, $param);
		
		// Executa a consulta no WS
		return curl_exec($handle);
	}
}
?>