<?php
/**
* Class to set up rules of security and generate navigation
*
* @uses	   	Zend_Acl
* @package	Core_Acl
*/

class Simec_Acl extends Zend_Acl 
{
	/**
	 * Singleton instance
	 *
	 * @var Core_Acl
	 */	
	protected static $_instance = null;

	/**
	 * Return instance of Core_Acl
	 *
	 * @return Core_Acl abstraction of Zend_Acl
	 */
	public static function getInstance() 
	{
		if( self::$_instance === null ) 
		{
			self::$_instance = new Core_Acl
			(
				Zend_Auth::getInstance()->setStorage
				(
					new Zend_Auth_Storage_Session
					(
						Zend_Registry::get('config')->app->name
					)
				)
			);
		}
		
		return self::$_instance;
	}

	/**
	 * Class constructor
	 *
	 * @param Zend_Auth $auth intance of Zend_Auth
	 * @return Zend_Registry Uinstance of Zend_Registry
	 */
	public function __construct(Zend_Auth $auth) 
	{
		try 
		{
			if ($auth->getIdentity()) {
				$auth = $auth->getStorage()->read();
					
				$cpf = $auth['auth']['usuario']['usucpf'];
					
				$this->removeAll();
					
				$this->addRole($cpf);
			
				$navigation = array();
			
				foreach ($auth['resources'] as $role)
				{
					$sisid = $role['sisid'];
					$module = $role['rscmodulo'];
					$controller = $role['rsccontroller'];
					$action = $role['rscaction'];
			
					$resource = $module . ':' . $controller;
					$previlege = $action;
			
					if ($resource) {
						if (!in_array($resource, $this->getResources())) {
							$this->addResource($resource);
						}
					}
			
					if ($resource && $previlege)
						$this->allow($cpf, $resource, $previlege);
				}

				Zend_Registry::getInstance()->set("navigation", $auth['navigation']);
			}
		} catch (Exception $e) {
			throw new Exception($e->getMessage());
		}
		
	}
}