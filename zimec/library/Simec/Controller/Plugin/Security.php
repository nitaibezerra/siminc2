<?php
/**
* Class responsable for security rules
*
* @uses	   Zend_Controller_Plugin_Abstract
* @package	Core_Plugin
* @subpackage Plugin
*/

class Simec_Controller_Plugin_Security extends Zend_Controller_Plugin_Abstract 
{
	/**
	 * Singleton instance
	 *
	 * @var Core_Acl
	 */
	private $_acl = null;

	/**
	 * Singleton instance
	 *
	 * @var Zend_Auth
	 */
	private $_auth = null;

	/**
	 * Singleton instance
	 *
	 * @var Core_Plugin_Security
	 */
	protected static $_instance = null;

	/**
	 * Return an instance of Core_Acl
	 *
	 * @return Zend_Acl Uma abstração do Zend_Auth
	 */
	public static function getInstance() 
	{
		if (self::$_instance === null) 
			self::$_instance = new Simec_Controller_Plugin_Security(Zend_Auth::getInstance(), Zend_Acl::getInstance());

		return self::$_instance;
	}

	/**
	* Class constructor
	*
	* @param Zend_Acl $acl instance of Zend_Acl
	* @param Zend_Auth $auth instance of Zend_Auth
	*/
	public function __construct(Zend_Acl $acl, Zend_Auth $auth) 
	{
		$this->_acl = $acl;
		$this->_auth = $auth;
	}

	/**
	 * Verify permission of access to controller
	 *
	 * @param Zend_Controller_Request_Abstract $request
	 */
	public function preDispatch(Zend_Controller_Request_Abstract $request) 
	{
		$auth = Zend_Registry::get('config')->auth;
		$modules = $auth->skip->module ? $auth->skip->module->toArray() : array();
		$controllers = $auth->skip->controller ? $auth->skip->controller->toArray() : array();
		$actions = $auth->skip->action ? $auth->skip->action->toArray() : array();
		
		if (!in_array($this->_request->getModuleName(), $modules) &&
			!in_array($this->_request->getControllerName(), $controllers) &&
			!in_array($this->_request->getActionName(), $actions)) {
				
			$credentials = $this->_auth->getStorage()->read();
			
			$role = $credentials['auth']['usuario']['usucpf'];
			$resource = $this->_request->getModuleName() . ':' . $this->_request->getControllerName();
			$privilege = $this->_request->getActionName();
			
			if (!$this->_acl->getRoles() ||
				!$this->_acl->hasRole($role) ||
				!$this->_acl->has($resource) ||
				!$this->_acl->isAllowed($role, $resource, $privilege))
			{
				$this->getResponse()->setRedirect(Zend_Controller_Front::getInstance()->getBaseUrl() . $auth->url)->sendResponse();
			}
		}
	}
}