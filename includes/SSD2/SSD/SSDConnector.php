<?php

//require_once('SOAP/Client.php');
require_once(APPRAIZ . "includes/SSD2/PEAR/SOAP/Client.php");

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

//		public function getAppletHtmlSampleCode($ticketId, $appletUrl, $appletClass, $codebase) {
//			return '<div><applet height="340" width="650" archive="'.$appletUrl.'" codebase="http://ssd.mec.gov.br" code="br.gov.mec.ssd.applet.auth.bycert.AuthByCertApplet.class" mayscript>
//                <param name="appletTicketId" value="'.$ticketId.'"/>
//            </applet><br /><br />
//                Problemas com o login ? Clique <a href="http://www.java.com/pt_BR/download/index.jsp" target="blank">aqui</a> para instalar o java.</div>
//                ';
//		}

    public function getAppletHtmlSampleCode($ticketId, $appletUrl, $appletClass, $codebase) {
        return

                '<div>
			<applet height="340" width="650" archive="' . $appletUrl . '" codebase="http://ssdhmg.mec.gov.br" code="br.gov.mec.ssd.applet.auth.bycert.AuthByCertApplet.class" mayscript>
				<param name="appletTicketId" value="' . $ticketId . '"/>
			</applet><br /><br />
				Problemas com o login ? Clique <a href="http://www.java.com/pt_BR/download/index.jsp" target="blank">aqui</a> para instalar o java.</div>
				';
    }

    public function post($postUrl, $postData, $headerData = null, $isUpload = FALSE, & $headerReturn = null) {
        if ($isUpload == FALSE) {
            $arrPost = array();
            foreach ($postData as $key => $data) {
                $arrPost[] = "$key=$data";
            }
            $postData = implode("&", $arrPost);
        }
        $ch = curl_init();
        curl_setopt($ch, CURLOPT_POST, 1);
        curl_setopt($ch, CURLOPT_URL, $postUrl);
        curl_setopt($ch, CURLOPT_SSL_VERIFYPEER, 1);
        curl_setopt($ch, CURLOPT_SSL_VERIFYHOST, 0);
        curl_setopt($ch, CURLOPT_SSLCERT, $this->clientCert);
        curl_setopt($ch, CURLOPT_SSLKEY, $this->clientPrivateKey);
        curl_setopt($ch, CURLOPT_SSLKEYPASSWD, $this->clientPrivateKeyPassword);
        curl_setopt($ch, CURLOPT_CAINFO, $this->trustedCa);
        curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
        curl_setopt($ch, CURLOPT_CONNECTTIMEOUT, 25);
        curl_setopt($ch, CURLOPT_TIMEOUT, 25);
        curl_setopt($ch, CURLOPT_POSTFIELDS, $postData);
        if (is_array($headerData)) {
            curl_setopt($ch, CURLOPT_HTTPHEADER, $headerData);
        }
        $result = curl_exec($ch);
        $curlError = curl_error($ch);
        if ($curlError) {
            throw new Exception('post: Impossivel obter resposta. Erro obtido: ' . $curlError . '.');
        }
        $httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        $headerReturn = curl_getinfo($ch);
        if ($httpCode == 200) {
            //do nothing
        } else {
            throw new Exception('post: Impossivel obter resposta. Codigo HTTP ' . $httpCode . '.');
        }
        curl_close($ch);
        if ($result) {
            //do nothing
        } else {
            throw new Exception('post: Impossivel obter resposta.');
        }
        return $result;
    }

    private function retriveWsdlXml($wsdlUrl, $type) {
    	
// 		$wsdlFilePath = $this->tmpDir . $this->getSystemFileSeparator() . "WSDL_{$type}_" . date("Y_m_j_H_i_s_u") . '.xml';
// 		$wsdlFilePath = $this->tmpDir . $this->getSystemFileSeparator() . "WSDL_{$type}_teste.xml";
// 		if (file_exists($wsdlFilePath) && is_file($wsdlFilePath)) {
// 			return $wsdlFilePath;
// 		}
//			echo($this->clientCert . '<br />');

//     	return APPRAIZ."fiesabatimento/WSDL_USER.xml";
    	
        $wsdlFilePath = $this->tmpDir .
                $this->getSystemFileSeparator() .
                "WSDL_{$type}_" . time() . rand() . rand() . '.xml';

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
        	fclose($fp);
            throw new Exception("retriveWsdlXml: Impossivel obter resposta. Erro obtido: $curlError.");
        }
        $httpCode = curl_getinfo($ch, CURLINFO_HTTP_CODE);
        if ($httpCode != 200) {
        	fclose($fp);
            throw new Exception("retriveWsdlXml: Impossivel obter resposta. Codigo HTTP: $httpCode.");
        }
        fclose($fp);
        curl_close($ch);
        if ($result && is_file($wsdlFilePath)) {
            return $wsdlFilePath;
        } else {
            throw new Exception("retriveWsdlXml: Impossivel obter o arquivo WSDL.");
        }
    }

    private function createWsClientFromWsdlFile($wsdlFilePath) {
        $wsdl = new SOAP_WSDL($wsdlFilePath);
        if (!$wsdl) {
            throw new Exception('createWsClientFromWsdlFile: Impossivel obter o Cliente de Web Service.');
        }

        $this->setWSDL($wsdl);
        $proxy = $wsdl->getProxy();

        if ($proxy instanceof SOAP_Fault) {
            throw new Exception(__METHOD__ . " - " . $proxy->message);
        }

        $proxy->setOpt('curl', CURLOPT_SSLCERT,$this->clientCert);
        $proxy->setOpt('curl', CURLOPT_SSLKEY,$this->clientPrivateKey);
        $proxy->setOpt('curl', CURLOPT_SSLKEYPASSWD,$this->clientPrivateKeyPassword);
        $proxy->setOpt('curl', CURLOPT_CAINFO,$this->trustedCa);
        $proxy->setOpt('curl', CURLOPT_VERBOSE, 1);
        $proxy->setOpt('curl', CURLOPT_SSL_VERIFYPEER, 1);
        $proxy->setOpt('curl', CURLOPT_SSL_VERIFYHOST, 0);
        $proxy->setOpt('curl', CURLOPT_CONNECTTIMEOUT, 25);
        $proxy->setOpt('curl', CURLOPT_TIMEOUT, 25);

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