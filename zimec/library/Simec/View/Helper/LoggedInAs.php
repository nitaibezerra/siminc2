<?php
/**
* Helper for print user logged
*
* @uses       Zend_View_Helper_Abstract
* @package    Simec_View_Helper_Logged
* @subpackage View\Helper
*/

class Simec_View_Helper_LoggedInAs extends Zend_View_Helper_Abstract 
{
	/*
	 * var Zend_Auth $_auth
	 */
	private $_auth;
	
	/*
	 * Get information from Zend_Auth
	 * 
	 * @return array 
	 */
	public function loggedInAs() 
	{
		$auth = Zend_Auth::getInstance();
		
		if ($auth->hasIdentity()) 
		{
			$this->_auth = $auth->getIdentity();

			return $this->_auth['auth'];
		}

		return null;
	}
}