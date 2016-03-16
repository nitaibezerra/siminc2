<?php

class Bootstrap extends Zend_Application_Bootstrap_Bootstrap
{
	/**
	 * Initialize doctype
	 *
	 * @return void
	 */
    protected function _initDoctype()
    {
        $this->bootstrap('view');
        $view = $this->getResource('view');
        $view->doctype('XHTML1_STRICT');
    }

    /**
     * Register translate system
     *
     * @return void
     */
    protected function _initTranslate()
    {
		$translate = new Zend_Translate('Array', APPLICATION_PATH . '/languages/pt_BR/Zend_Validate.php', 'pt_BR');
		
        Zend_Validate_Abstract::setDefaultTranslator($translate);
    }

	/**
	 * Register configuration of system
	 *
	 * @return Zend_Registry 'config'
	 */
	protected function _initConfig() 
	{
		$config = new Zend_Config_Ini(APPLICATION_PATH.'/configs/application.ini', APPLICATION_ENV);

		$mobile = isset($_SERVER["HTTP_USER_AGENT"]) ? preg_match("/(android|avantgo|blackberry|bolt|boost|cricket|docomo|fone|hiptop|mini|mobi|palm|phone|pie|tablet|up\.browser|up\.link|webos|wos)/i", $_SERVER["HTTP_USER_AGENT"]) : false;

		Zend_Registry::set('mobile', $mobile);

		Zend_Registry::getInstance()->set('config', $config);
	}

	/**
	 * Init component of security
	 *
	 * @return void
	 */
	protected function _initSecurity()
	{
		$auth = Zend_Auth::getInstance();
		$acl  = new Simec_Acl($auth);

		$front = Zend_Controller_Front::getInstance();
		$front->registerPlugin(new Simec_Controller_Plugin_Security($acl, $auth));
	}

	/**
	 * Init cache engine
	 *
	 * @return void
	protected function _initCache()
	{
		$cache = null;

		$resources = Zend_Registry::get('config')->cache;

		if ($resources->enable)
		{
			$frontendOptions = array('automatic_serialization' => $resources->frontend->automatic_serialization, 'lifetime' => $resources->frontend->lifetime, 'debug_header' => true);
			$backendOptions = array('lifetime' => $resources->backend->lifetime);
			$cache = Zend_Cache::factory($resources->frontend->name, $resources->backend->name, $frontendOptions, $backendOptions, true, true);
		}

		Zend_Registry::set('cache', $cache);
	}
	*/
}