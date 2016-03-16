<?php 

class Ws_Unasus_Soap extends Modelo{

	const TOKEN_WS = "d40319f5bd0de2903330bd5a2a9712ff2464ab12";

	protected $ws;
	
	protected $antigo_ws;
	
	/**
	 * Método de conexão com o WebService
	 *
	 * @return SoapClient
	 */
	protected function conexaoWS()
	{
		include_once APPRAIZ.'includes/nusoap/lib/nusoap.php';
		if(!$this->ws)
		{
			$this->ws = $client = new SoapClient("https://sistemas.unasus.gov.br/ws/services/ebserh",array('encoding'=>'ISO-8859-1'));
		}
		return $this->ws;
	}
	
	protected function conexaoAntigaWS()
	{
		include_once APPRAIZ.'includes/nusoap/lib/nusoap.php';
		if(!$this->antigo_ws)
		{
			$this->antigo_ws = $client = new SoapClient(null, array(
					'location' 	=> "http://sistemas.unasus.gov.br/ws/interface/ebserh",
					'uri' 		=> "http://sistemas.unasus.gov.br/ws/interface/ebserh",
					'trace'    	=> 1,
					'encoding' 	=> 'ISO-8859-1' )
			);
		}
		return $this->antigo_ws;
	}
	
	public function getFunctions()
	{
		$conexao_ws = $this->conexaoWS();
		return $conexao_ws->__getFunctions();
	}
	
	public function getTypes()
	{
		$conexao_ws = $this->conexaoWS();
		return $conexao_ws->__getTypes();
	}
	
	public function escapeStringWs($text)
	{
		$text = pg_escape_string(trim(str_replace(array("'", "\\",'"'),array('',"/",''),$text)));
		 
		return $text;
	}
	
	/**
	 * Envia e-mail
	 *
	 * @param string $assunto
	 * @param string $conteudo
	 */
	public function enviarEmailAuditoriaRotinas($assunto, $conteudo)
	{
			
		require_once APPRAIZ . 'includes/phpmailer/class.phpmailer.php';
		require_once APPRAIZ . 'includes/phpmailer/class.smtp.php';
		$mensagem = new PHPMailer();
		$mensagem->persistencia = $db;
		$mensagem->Host         = "localhost";
		$mensagem->Mailer       = "smtp";
		$mensagem->FromName		= "WS Mais Médicos";
		$mensagem->From 		= "simec@mec.gov.br";
		$mensagem->AddAddress("wescley.lima@mec.gov.br", "Wescley Lima");
		$mensagem->AddAddress("henrique.couto@ebserh.gov.br", "Henrique Couto");
		$mensagem->Subject = $assunto;
		
		$mensagem->Body = $conteudo;
		$mensagem->IsHTML( true );
		if(strpos( $_SERVER['SERVER_NAME'], 'simec.mec.gov.br' ) !== false)
			$mensagem->Send();
	
	}
}