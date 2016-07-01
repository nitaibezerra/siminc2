<?php

/**
 * Classe responsável pela autorização de segurança do
 * sistema utilizando o serviço do SSD.
 *
 * Sistema de Segurança Digital
 *
 * @package library-Essencial-Adapter
 */

set_include_path(APPRAIZ . 'includes/SSD2/PEAR/' . get_include_path());
    	
require_once(APPRAIZ . "includes/SSD2/SSD/SSDConnector.php");
require_once(APPRAIZ . "includes/SSD2/SSD/SSDWsAuth.php");
require_once(APPRAIZ . "includes/SSD2/SSD/SSDWSSignDocs.php");
require_once(APPRAIZ . "includes/SSD2/SSD/SSDWsUser.php");
class Essencial_Adapter_SSD
{
    /**
     *
     * Objeto stdClass para links do certificado.
     *
     * @var stdClass
     */
    private $_certificado;

    public function __construct()
    {
    	$urlCertificado = array();
    	
		if( $_SERVER['HTTP_HOST'] == 'simec-local' ){
			$pasta = 'simec-d';
	    	$urlCertificado['method_urls'] 			= 'useHomologationSSDServices';
		}elseif( $_SERVER['HTTP_HOST'] == 'simec-d.mec.gov.br' || $_SERVER['HTTP_HOST'] == 'simec-d' ){
			$pasta = 'simec-d';
	    	$urlCertificado['method_urls'] 			= 'useHomologationSSDServices';
		}else{
			$pasta = 'simec';
    		$urlCertificado['method_urls'] 			= 'useProductionSSDServices';
		}
		
		if(!is_dir(APPRAIZ."arquivos/fiesabatimento")) {
			mkdir(APPRAIZ."arquivos/fiesabatimento", 0777);
		}
		
    	$urlCertificado['tmp_dir'] 				= APPRAIZ."arquivos/fiesabatimento";
		
    	$urlCertificado['client_cert'] 			= APPRAIZ . "includes/SSD2/certificado_ssd/fiesabatimento/$pasta/cert.pem";
    	$urlCertificado['private_key'] 			= APPRAIZ . "includes/SSD2/certificado_ssd/fiesabatimento/$pasta/chave.pem";
		$urlCertificado['private_key_password'] = APPRAIZ . "includes/SSD2/certificado_ssd/fiesabatimento/$pasta/pass.pem";
    	$urlCertificado['trusted_ca_chain'] 	= APPRAIZ . "includes/SSD2/certificado_ssd/fiesabatimento/$pasta/chain.pem";
    	$urlCertificado['marker'] 				= 1;

        $this->_certificado = (object) $urlCertificado;
    }

    public function solicitarAcesso($codigoServico = '50', $flag = '')
    {
            $userAction = new SSDWsUser($this->_certificado->tmp_dir,
                                        $this->_certificado->client_cert,
                                        $this->_certificado->private_key,
                                        $this->_certificado->private_key_password,
                                        $this->_certificado->trusted_ca_chain);

            /**
             * Verifica se a chave privada vai para homologa��o ou produção conforme est� em application.ini
             * $userAction->useHomologationSSDServices();
             */
            $userAction->{$this->_certificado->method_urls}();
            $resposta = $userAction->getUserMaintenanceUrl((string) $flag, (string) $codigoServico);

            return $resposta->getUrl();
    }

    public function retornarTicket($ticket)
    {
            $userAction = new SSDWsUser($this->_certificado->tmp_dir,
                                        $this->_certificado->client_cert,
                                        $this->_certificado->private_key,
                                        $this->_certificado->private_key_password,
                                        $this->_certificado->trusted_ca_chain);

            $userAction->{$this->_certificado->method_urls}();
            $resposta = $userAction->getUserMaintenanceTicket($ticket);

            $userId = (integer) $resposta->userId;
            $dadosUsuario = $userAction->getUserInfo($userId);

            if(isset($dadosUsuario->message)){
                return $dadosUsuario;
            }
            
            $dados = array();
            $dados['dadosUsuario'] = $dadosUsuario;
            $dados['retornoLogin'] = $resposta;

            return $dados;
    }

    public function solicitarLogin()
    {
            $ssdWsAuth = new SSDWsAuth($this->_certificado->tmp_dir,
									   $this->_certificado->client_cert,
									   $this->_certificado->private_key,
									   $this->_certificado->private_key_password,
									   $this->_certificado->trusted_ca_chain);  
            $ssdWsAuth->{$this->_certificado->method_urls}();
            $resposta = $ssdWsAuth->getAuthenticationByIdServletUrl('1');
            return $resposta->getUrl();
    }
    
    
}
