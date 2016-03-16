<?php
	require_once(SSD_PATH."SOAP/Client.php");
	
	function _log($msg) {
		echo $msg . "<br />";
	}
	
	class SSDConnector {
		private $wsdl;
		private $tmpDir;
		private $clientCert;
		private $trustedCa;
		private $clienPrivateKeyPassword;
		private $clientPrivateKey;
		
		public function __construct($tmpDir, $certClient, $privateKey, $passPrivateKey, $trustedCa) {
			$this->tmpDir = $tmpDir;
			$this->clientCert = $certClient;
			$this->clientPrivateKey = $privateKey;
			$this->clienPrivateKeyPassword = $passPrivateKey;
			$this->trustedCa = $trustedCa;
		}
		
		public function getWsClient($wsdlUrl) {
			$wsdlFilePath = $this->retriveWsdlXml($wsdlUrl);
			$wsClient = $this->createWsClientFromWsdlFile($wsdlFilePath);
			return $wsClient;
		}
		
		public function getWSDL(){
			return $this->wsdl;
		}
		
		public function getAppletHtmlSampleCode($ticketId, $appletUrl) {
			return "<div><applet height=340 width=650 archive=\"".$appletUrl."\" ".
			" code=\"br.gov.mec.ssd.client.applet.BootApplet.class\" mayscript> ".
			" <param name=\"appletTicketId\" value=\"".$ticketId."\" /></applet><br /><br />
				Problemas com o login ? Clique <a href='http://www.java.com/pt_BR/download/index.jsp' target='blank'>aqui</a> para instalar o java.</div>
				";
		}
		
		public function post($postUrl, $postData, $headerData = null , $isUpload = FALSE , & $headerReturn = null ) {
			if ($isUpload == FALSE) {
				$arrPost = array();
				foreach($postData as $key => $data) {
					$arrPost[] = "$key=$data";
				}
				
				$postData = implode("&" , $arrPost);
			}
			
			$ch = curl_init();
			curl_setopt($ch , CURLOPT_POST				, 1);
			curl_setopt($ch , CURLOPT_URL				, $postUrl);
			curl_setopt($ch , CURLOPT_SSL_VERIFYPEER	, true);
			curl_setopt($ch , CURLOPT_SSL_VERIFYHOST	, 0);
			curl_setopt($ch , CURLOPT_SSLCERT			, $this->clientCert);
			curl_setopt($ch , CURLOPT_SSLKEY			, $this->clientPrivateKey);
			curl_setopt($ch , CURLOPT_SSLKEYPASSWD		, $this->clienPrivateKeyPassword);
			curl_setopt($ch , CURLOPT_CAINFO			, $this->trustedCa);
			curl_setopt($ch , CURLOPT_RETURNTRANSFER	, 1);
			curl_setopt($ch , CURLOPT_TIMEOUT			, 30);
			curl_setopt($ch , CURLOPT_POSTFIELDS		, $postData);
			
			if( is_array($headerData)) {
				curl_setopt($ch, CURLOPT_HTTPHEADER	, $headerData);
			}
			
			//curl_setopt($ch, CURLOPT_UPLOAD, 1);
			//curl_setopt($ch, CURLOPT_FILE, $fp);
			//curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 1);
			//curl_setopt($ch, CURLOPT_HEADER, 2);
			//curl_setopt($ch, CURLOPT_VERBOSE, 0);
			
			$result = curl_exec($ch);
			$curlError = curl_error($ch);
			if ($curlError) {
				throw new Exception('Impossivel obter resposta. '.
				'Erro obtido: '. $curlError);
			}
			$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
			
			$headerReturn = curl_getinfo($ch);
			
			if ($httpCode == 200) {
				//do nothing
			} else {
				throw new Exception('Nao foi possivel obter resposta. '
				. 'Codigo HTTP '. $httpCode . '.');
			}
			
			curl_close($ch);
			
			if ($result) {
				//do nothing
			} else {
				throw new Exception('Nao foi possivel obter resposta.');
			}
			return $result;
		}
		
		private function retriveWsdlXml($wsdlUrl) {
			$wsdlFilePath = $this->tmpDir .
			'wsdl_' . time() . rand() . rand() . '.xml';
			$fp = fopen($wsdlFilePath, "w");
			$ch = curl_init();
			curl_setopt($ch, CURLOPT_FILE, $fp);
			curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 1);
			curl_setopt($ch, CURLOPT_URL, $wsdlUrl);
			curl_setopt($ch, CURLOPT_VERBOSE, 1);
			curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
			curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
			curl_setopt($ch, CURLOPT_SSLCERT, $this->clientCert);
			curl_setopt($ch, CURLOPT_SSLKEY, $this->clientPrivateKey);
			curl_setopt($ch, CURLOPT_SSLKEYPASSWD, $this->clienPrivateKeyPassword);
			curl_setopt($ch, CURLOPT_CAINFO, $this->trustedCa);
			curl_setopt($ch, CURLOPT_TIMEOUT, 120);
			$result = curl_exec($ch);
			$curlError = curl_error($ch);
			if ($curlError) {
				echo "<pre>".$curlError."</pre>";
				exit;
			}
			
			$httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
			if ($httpCode == 200) {
				//do nothing
			} else {
				echo "<pre>Nao foi possivel obter resposta. Codigo HTTP ". $httpCode .".</pre>";
				exit;
			}
			
			fclose($fp);
			curl_close($ch);
			if ($result && is_file($wsdlFilePath)) {
				return $wsdlFilePath;
			} else {
				echo "<pre>Nao foi possivel obter o wsdl.</pre>";
				exit;
			}
		}
		
		private function createWsClientFromWsdlFile($wsdlFilePath) {
			$wsdl = new SOAP_WSDL($wsdlFilePath);
			if ($wsdl) {
				//do nothing
			} else {
				throw new Exception('Impossivel obter o cliente de web service.');
			}
			$this->setWSDL($wsdl);
			$proxy = $wsdl->getProxy();
			$proxy->setOpt('curl', CURLOPT_VERBOSE, 1);
			$proxy->setOpt('curl', CURLOPT_SSL_VERIFYPEER, 1);
			$proxy->setOpt('curl', CURLOPT_SSL_VERIFYHOST, 0);
			$proxy->setOpt('curl', CURLOPT_CAINFO,$this->trustedCa);
			$proxy->setOpt('curl', CURLOPT_SSLCERT,$this->clientCert);
			$proxy->setOpt('curl', CURLOPT_SSLKEY,$this->clientPrivateKey);
			$proxy->setOpt('curl', CURLOPT_SSLKEYPASSWD,$this->clienPrivateKeyPassword);
			$proxy->setOpt('curl', CURLOPT_TIMEOUT, 120);
			
			if ($proxy) {
				//do nothing
			} else {
				throw new Exception('Impossivel obter o cliente de web service.');
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