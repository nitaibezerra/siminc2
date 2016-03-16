<?php
/**
* Authentication class
*
* @uses	   Zend_Auth_Adapter_Interface
* @package	Core_Auth_Db
* @subpackage Auth\Db
*/

import('seguranca.business.AutenticacaoBusiness');

class Simec_Auth_Db implements Zend_Auth_Adapter_Interface 
{
	private $_request;

	private $businessAutenticacao;
	
	/**
	 * Class constructor
	 *
	 * @param Zend_Controller_Request_Abstract $_request
	 */
	public function __construct(Zend_Controller_Request_Abstract $_request) 
	{
		$this->_request = $_request;
		
		$this->businessAutenticacao = new Seguranca_Business_AutenticacaoBusiness();
	}
	
	/**
	 * Execute authentication
	 *
	 * @throws Zend_Auth_Adapter_Exception exception for non authorized actions
	 * @return Zend_Auth_Result
	 */
	public function authenticate() 
	{
		if (!$this->_request->getParam("usucpf") && !$this->_request->getParam("ususenha"))
		{
			$message = "Parametros nao informados corretamente para a consulta.";
			
			return new Zend_Auth_Result(Zend_Auth_Result::FAILURE_CREDENTIAL_INVALID, null, array($message));
		}
		else 
		{
			try 
			{
				$usuario = $this->businessAutenticacao->authenticate($this->_request->getParam("usucpf"), $this->_request->getParam("ususenha"));
			} 
			catch (Exception $e) 
			{
				throw new Zend_Auth_Exception($e->getMessage());
			}

			if (!isset($usuario['usucpf']))
			{
				$message = "Nenhum usuario encontrado para os dados informados ou a conta esta inativa.";
				
				return new Zend_Auth_Result(Zend_Auth_Result::FAILURE_IDENTITY_NOT_FOUND, null, array($message));
			}
				
			$auth['usuario']['usucpf'] = $usuario['usucpf'];
			$auth['usuario']['usunome'] = $usuario['usunome'];
			$auth['usuario']['usufuncao'] = $usuario['usufuncao'];

			
			/*
			 * @todo 
			 * Remover este tipo de autenticação de uma forma mais elagante
			 * Estas variaveis abaixo permite autenticar pelo Zend e redirecionar para o simec antigo
			 */ 
			$auth['usuario']['sisid'] = $usuario['sisid'];
			$auth['usuario']['sisantigo'] = $usuario['sisantigo'];
			$auth['usuario']['sisdiretorio'] = $usuario['sisdiretorio'];
			$auth['usuario']['sisarquivo'] = $usuario['sisarquivo'];
			$auth['usuario']['paginainicial'] = $usuario['paginainicial'];
			
			try 
			{
				$resources = $this->businessAutenticacao->recuperarPermissoes($usuario['usucpf']);
				
				$navigation = $this->businessAutenticacao->recuperarMenu($usuario['usucpf'], $usuario['sisid']);
				
				if (!$resources) 
				{
					$message = "Nenhum perfil encontrado para o usuario: " . $auth['usuario']['usunome'] . ", CPF: " . $auth['usuario']['usucpf'] . ". Favor vincular um perfil ao usuario.";

					return new Zend_Auth_Result(Zend_Auth_Result::FAILURE_CREDENTIAL_INVALID, null, array($message));
				}
			}
			catch (Zend_Auth_Exception $e) 
			{
				throw new Zend_Auth_Exception($e->getMessage());
			}
			
			$auth = array("auth" => $auth, "resources" => $resources, "navigation" => $navigation);
				
			$message = "Autenticacao realizada com sucesso.";
			
			return new Zend_Auth_Result(Zend_Auth_Result::SUCCESS, $auth, array($message));
		}
	}
}