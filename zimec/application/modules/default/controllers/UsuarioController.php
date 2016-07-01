<?php
import('seguranca.business.AutenticacaoBusiness');

class Default_UsuarioController extends Simec_Controller_Action
{
	private $businessAutenticacao;

    public function init()
    {
        $this->businessAutenticacao = new Seguranca_Business_AutenticacaoBusiness();
    }
	
    public function indexAction()
    {
    	$this->_redirect('/');
    }
    
    public function loginAction()
    {
    	$this->_helper->layout->setLayout('login');
    	
    	Zend_Auth::getInstance()->clearIdentity();
    	
		try 
		{
			if ($this->getRequest()->isPost())
			{
				$data = $this->getRequest()->getPost();
				
				if ($data['usucpf'] && $data['ususenha'])
				{
					$auth = Zend_Auth::getInstance();
					
					if (!$auth->hasIdentity())
					{
						try
						{
							$result = $auth->authenticate(new Simec_Auth_Db($this->getRequest()));
							
							if ($result->getCode() == Zend_Auth_Result::SUCCESS) 
							{
								$auth = Zend_Auth::getInstance();
									
								$identity = $auth->getIdentity();

								$_SESSION['exercicio_atual'] = '2015';//$db->pega_ano_atual();
								$_SESSION['exercicio'] = '2015';//$db->pega_ano_atual();
								$_SESSION['superuser'] = true; //$db->testa_superuser( $usuario->usucpf );
								$_SESSION['usucpforigem'] = $identity["auth"]["usuario"]["usucpf"];
								$_SESSION['baselogin'] = $data['baselogin'];
								$_SESSION['usucpf'] = $identity["auth"]["usuario"]["usucpf"];
								$_SESSION['sisid'] =  $identity["auth"]["usuario"]["sisid"];
								$_SESSION["NUM_IP_CLIENTE"] = $_SERVER["REMOTE_ADDR"];
								$_SESSION["DES_BROWSER"] = $_SERVER["HTTP_USER_AGENT"];
								$_SESSION['usuacesso'] = date( 'Y/m/d H:i:s' );
								$_SESSION["evHoraUltimoAcesso"] = time();
								
								if ($identity["auth"]["usuario"]['sisantigo'])
								{
									foreach ( $identity["auth"]["usuario"] as $attribute => $value ) {
										$_SESSION[$attribute] = $value;
									}
									
									/*
									foreach ( $sistema as $attribute => $value ) {
										$_SESSION[$attribute] = $value;
									}
									*/
									
									unset($_SESSION['superuser']);
									
									$header = sprintf(
										"Location: ../../%s/%s.php?modulo=%s",
										$identity["auth"]["usuario"]['sisdiretorio'],
										$identity["auth"]["usuario"]['sisarquivo'],
										$identity["auth"]["usuario"]['paginainicial']
									);
									
									header($header);
									die;
								}
								
								$this->_redirect($identity["auth"]["usuario"]['paginainicial']);
							}

							$_SESSION['MSG_AVISO'] = array(current($result->getMessages()));
							
							$this->_transport(MSG_ERROR, current($result->getMessages()), '/');
						}
						catch (Zend_Auth_Exception $e)
						{
							$_SESSION['MSG_AVISO'] = array($e->getMessage());
							
							$this->_transport(MSG_ERRO, $e->getMessage(), '/');
						}
					}
					$this->_redirect("/");
				}
			
				$_SESSION['MSG_AVISO'] = array('Email ou senha nao informados.');
				
				$this->_transport(MSG_ERROR, 'Email ou senha nao informados.', '/');
			}
		}
		catch (Exception $e)
		{
			die($e->getMessage());
			
			$_SESSION['MSG_AVISO'] = array($e->getMessage());
			
			$this->_transport(MSG_ERRO, $e->getMessage(), '/');
		}
    }
    
    public function logoutAction()
    {
    	Zend_Auth::getInstance()->clearIdentity();
    	Zend_Registry::getInstance()->_unsetInstance();
    	
    	$this->_redirect("/");
    }
}