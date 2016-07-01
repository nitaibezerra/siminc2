<?php

	require_once('SOAP/Client.php');
	
	function _log($msg) {
		echo("$msg\n");
		echo("<br />\n");
	}
	
	class SSDConnector {

		private $wsdl;
		private $tmpDir;
		private $clientCert;
		private $clientPrivateKey;
		private $clientPrivateKeyPassword;
		private $trustedCa;
		
		public function __construct($tmpDir, $clientCert, $clientPrivateKey, $clientPrivateKeyPassword, $trustedCa) {
			$this->tmpDir = $tmpDir;
			$this->clientCert = $clientCert;
			$this->clientPrivateKey = $clientPrivateKey;
			$this->clientPrivateKeyPassword = $clientPrivateKeyPassword;
			$this->trustedCa = $trustedCa;
		}
		
		public function getWsClient($wsdlUrl, $type) {
			$wsdlFilePath = $this->retriveWsdlXml($wsdlUrl, $type);
			$wsClient = $this->createWsClientFromWsdlFile($wsdlFilePath);
			return $wsClient;
		}
		
		public function getWSDL() {
			return $this->wsdl;
		}
		
		public function getAppletHtmlSampleCode($ticketId, $appletUrl) {
			return '<div><applet height="340" width="650" archive="' . $appletUrl . '" ' .
				'code="br.gov.mec.ssd.client.applet.BootApplet.class" mayscript> ' .
				'<param name="appletTicketId" value="' . $ticketId . '" /></applet><br /> ' .
				'<p>Problemas com o login? Clique <a href="http://www.java.com/pt_BR/download/index.jsp" ' .
				'target="blank">aqui</a> para instalar o java.</p></div>';
		}
		
		public function post($postUrl, $postData, $headerData = null, $isUpload = false, & $headerReturn = null) {
			if ($isUpload == false) {
				$arrPost = array();
				foreach($postData as $key => $data) {
					$arrPost[] = "$key=$data";
				}
				$postData = implode("&", $arrPost);
			}
			$ch = curl_init();
			curl_setopt($ch, CURLOPT_URL, $postUrl);
			curl_setopt($ch, CURLOPT_SSLCERT, $this->clientCert);
			curl_setopt($ch, CURLOPT_SSLKEY, $this->clientPrivateKey);
			curl_setopt($ch, CURLOPT_SSLKEYPASSWD, $this->clientPrivateKeyPassword);
			curl_setopt($ch, CURLOPT_CAINFO, $this->trustedCa);
			curl_setopt($ch, CURLOPT_POSTFIELDS, $postData);
			curl_setopt($ch, CURLOPT_POST, 1);
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
			curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
			curl_setopt($ch, CURLOPT_RETURNTRANSFER , 1);
			curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 25);
			curl_setopt($ch, CURLOPT_TIMEOUT, 25);
			if(is_array($headerData)) {
				curl_setopt($ch, CURLOPT_HTTPHEADER, $headerData);
			}
			$result = curl_exec($ch);
			$curlError = curl_error($ch);
			if ($curlError) {
				throw new Exception("post: Impossivel obter resposta. Erro obtido: $curlError.");
			}
			$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
			$headerReturn = curl_getinfo($ch);
			if ($httpCode != 200) {
				throw new Exception("post: Impossivel obter resposta. Codigo HTTP: $httpCode.");
			}
			curl_close($ch);
			if (!$result) {
				throw new Exception('post: Impossivel obter resposta.');
			}
			return $result;
		}
		
		private function retriveWsdlXml($wsdlUrl, $type) {
			$wsdlFilePath = $this->tmpDir . $this->getSystemFileSeparator() . "WSDL_{$type}_" . date("Y_m_j_H") . '.xml';
			if (file_exists($wsdlFilePath) && is_file($wsdlFilePath)) {
				return $wsdlFilePath;
			}
			echo($this->clientCert . '<br />');
			$fp = fopen($wsdlFilePath, 'w');
			$ch = curl_init();
			curl_setopt($ch, CURLOPT_FILE, $fp);
			curl_setopt($ch, CURLOPT_URL, $wsdlUrl);
			curl_setopt($ch, CURLOPT_SSLCERT, $this->clientCert);
			curl_setopt($ch, CURLOPT_SSLKEY, $this->clientPrivateKey);
			curl_setopt($ch, CURLOPT_SSLKEYPASSWD, $this->clientPrivateKeyPassword);
			curl_setopt($ch, CURLOPT_CAINFO, $this->trustedCa);
			curl_setopt($ch, CURLOPT_VERBOSE, 1);
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
			curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
			curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 25);
			curl_setopt($ch, CURLOPT_TIMEOUT, 25);
			$result = curl_exec($ch);
			$curlError = curl_error($ch);
			if ($curlError) {
				return array("erro" => "SSD : Impossivel obter resposta. Erro obtido: $curlError.");
			}
			$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
			if ($httpCode != 200) {
				return array("erro" => "SSD : Impossivel obter resposta. Codigo HTTP: $httpCode.");
			}
			fclose($fp);
			curl_close($ch);
			if ($result && is_file($wsdlFilePath)) {
				return $wsdlFilePath;
			} else {
				return array("erro" => "SSD : Impossivel obter o arquivo WSDL. Codigo HTTP: $httpCode.");
			}
		}
		
		private function createWsClientFromWsdlFile($wsdlFilePath) {
			$wsdl = new SOAP_WSDL($wsdlFilePath);
			if (!$wsdl) {
				return array("erro" => "SSD : Impossivel obter o Cliente de Web Service.");
			}
			$this->setWSDL($wsdl);
			$proxy = $wsdl->getProxy();
			$proxy->setOpt('curl', CURLOPT_SSLCERT,$this->clientCert);
			$proxy->setOpt('curl', CURLOPT_SSLKEY,$this->clientPrivateKey);
			$proxy->setOpt('curl', CURLOPT_SSLKEYPASSWD,$this->clientPrivateKeyPassword);
			$proxy->setOpt('curl', CURLOPT_CAINFO,$this->trustedCa);
			$proxy->setOpt('curl', CURLOPT_VERBOSE, 1);
			$proxy->setOpt('curl', CURLOPT_SSL_VERIFYPEER, 1);
			$proxy->setOpt('curl', CURLOPT_SSL_VERIFYHOST, 0);
			$proxy->setOpt('curl', CURLOPT_CONNECTTIMEOUT, 25);
			$proxy->setOpt('curl', CURLOPT_TIMEOUT, 25);
			if (!$proxy) {
				return array("erro" => "SSD : Impossivel obter o Cliente de Web Service.");
			}
			return $proxy;
		}
		
		private function getSystemFileSeparator() {
			return "/";
		}
		
		private function setWSDL($param) {
			$this->wsdl = $param;
		}

	}

?>
