<?php
/**
 * @versao $Id: Rest.php 83754 2014-07-29 22:22:09Z FellipeSantos $
 */

/**
 *
 */
abstract class  Simec_Controller_Rest extends Zend_Controller_Action
{
	/**
	 * Class constructor
	 *
	 * The request and response objects should be registered with the
	 * controller, as should be any additional optional arguments; these will be
	 * available via {@link getRequest()}, {@link getResponse()}, and
	 * {@link getInvokeArgs()}, respectively.
	 *
	 * When overriding the constructor, please consider this usage as a best
	 * practice and ensure that each is registered appropriately; the easiest
	 * way to do so is to simply call parent::__construct($request, $response,
	 * $invokeArgs).
	 *
	 * After the request, response, and invokeArgs are set, the
	 * {@link $_helper helper broker} is initialized.
	 *
	 * Finally, {@link init()} is called as the final action of
	 * instantiation, and may be safely overridden to perform initialization
	 * tasks; as a general rule, override {@link init()} instead of the
	 * constructor to customize an action controller's instantiation.
	 *
	 * @param Zend_Controller_Request_Abstract $request
	 * @param Zend_Controller_Response_Abstract $response
	 * @param array $invokeArgs Any additional invocation arguments
	 * @return void
	 */
	public function __construct(Zend_Controller_Request_Abstract $request, Zend_Controller_Response_Abstract $response, array $invokeArgs = array())
	{
		$actionNotFound = true;
		
		$this->classReflector = new ReflectionClass(get_class($this));

		$delimiter = Zend_Controller_Router_Route_Regex::URI_DELIMITER;
		
		$pathUri = $delimiter . $request->getParam('module') . $delimiter . $request->getParam('controller') . $delimiter . $request->getParam('action');
		
		foreach ($this->classReflector->getMethods(ReflectionMethod::IS_PUBLIC) as $methodReflector) {
			$methodAnnotations = new Simec_Annotations($methodReflector);
			$annotations = $methodAnnotations->asArray();

			if (isset($annotations['rest']) && isset($annotations['rest']['uri']))
			{
				if ($annotations['rest']['uri'] == $pathUri && 
					$annotations['rest']['method'] == $request->getMethod())
				{
					$request->setActionName(str_replace('Action', null, $methodReflector->getName()));
					
					$this->setRequest($request)
						 ->setResponse($response)
						 ->_setInvokeArgs($invokeArgs);
					$this->_helper = new Zend_Controller_Action_HelperBroker($this);
					$this->init();
					
					$actionNotFound = false;
				}
			}
		}
		
		if ($actionNotFound)
		{
			require_once 'Zend/Controller/Exception.php';
			throw new Zend_Controller_Exception('Invalid action/method specified for URI');
		}
	}
	
	public function _encode($var) 
	{
		print json_encode($var);
		die;
	}
	
    public function init()
    {
        parent::init();
    }
}