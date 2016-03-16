<?php
/**
 * @versao $Id: Action.php 100116 2015-07-15 13:31:22Z fellipesantos $
 */

/**
 *
 */
abstract class Simec_Controller_Action extends Zend_Controller_Action
{
    protected $mensagens;
    protected $isXmlHttpRequest = false;

    public function init()
    {
        parent::init();
        $this->view->mensagens = $this->getMessages();
    }

    protected function ehRequisicaoAjax()
    {
        return $this->isXmlHttpRequest || $this->getRequest()->isXmlHttpRequest();
    }

    protected function getMessages()
    {
        if (!$this->ehRequisicaoAjax()) {
            $this->mensagens = $this->_helper->FlashMessenger->getMessages();
        }

        return $this->mensagens;
    }

    protected function addMessage($msg, $tipo = 'info')
    {
        if (is_array($msg)) { // -- Array de mensagens
            foreach ($msg as $_msg) {
                $msg = is_array($_msg)?current($_msg):$_msg;
                $this->addMessage($msg, $tipo);
            }
        } else {
            if (is_numeric($msg)) { // -- Mensagem identificada por constante
                // -- @todo Implementar constantes
                $msg = (string)$msg;
            }
            // -- Processamento de mensagem textual
            // -- Identificando o tipo da requisição
            if (!$this->ehRequisicaoAjax()) {
                $this->_helper->FlashMessenger(array($tipo => $msg));
            } else {
                $this->mensagens[$tipo][] = $msg;
            }
        }
    }
    
    /**
     * @param string $url
     * @param null $msg
     * @param string $tipo
     * @param array $options
     */
    public function _redirect($url, array $options = array())
    {
        if (!isset($options['msg'])) {
            $this->addMessage($options['msg'], $options['tipo']);
        }
        parent::_redirect($url, $options);
    }
    
    /**
     * Add message
     * Recomended without redirections.
     *
     * @access protected
     * @param string $namespace
     * @param string $msg
     */
    final protected function _message($namespace, $msg)
    {
    	$this->view->messages = array();
    	$this->view->messages[ $namespace ][] = $msg;
    }
    
    /**
     * Encode for ajax action
     * Recomended to redirections.
     *
     * @access protected
     * @param $result
     */
    final protected function _encode($result)
    {
    	$this->_helper->layout()->disableLayout();
    	$this->_helper->viewRenderer->setNoRender(true);
    
    	header('Content-Type: application/json; charset=UTF-8');
    	echo json_encode($result);
    	die;
    }
    
    /**
     * Partial html page
     *
     * @access protected
     * @param $result
     */
    final protected function _partial($page)
    {
    	$controller = strtolower($this->getRequest()->getControllerName());
    
    	$this->_helper->layout->disableLayout();
    	$this->_helper->viewRenderer($controller . '/partial/' . $page, null, true);
    }
    
    /**
     * Add message
     * Recomended to redirections.
     *
     * @access protected
     * @param $msg
     * @param redirect String
     */
    final protected function _transport($namespace, $msg, $redirect = null)
    {
    	$this->_helper->flashMessenger->addMessage(array($namespace => $msg));
    
    	$this->_redirect($redirect);
    }
    
    
    /**
     * Get page param
     * Default value is 15.
     *
     * @access protected
     *
     * @return string $page
     */
    final protected function _count($count = 15)
    {
    	return $count;
    }
    
    /**
     * Get the index count page
     *
     * @return int $index
     */
    final protected function _offset()
    {
    	return $this->_getParam('iDisplayStart', 0);
    }
    
    /**
     * Get the order param
     * Control ASC/DESC order.
     *
     * @access protected
     * @param $default
     *
     * @return string $order
     */
    final protected function _order($default)
    {
    	$this->order = $this->_getParam('order');
    
    	$this->type = $this->_getParam('direction');
    
    	if (empty($this->order))
    		return $default;
    
    	return $this->order . ' ' . $this->type;
    }
    
    /**
     * Get the filter param
     *
     * @access protected
     * @param $default
     *
     * @return string $order
     */
    final protected function _filter(array $params)
    {
    	$filter = null;

    	if ($this->getRequest()->getParam('sSearch'))
    	{
	    	foreach ($params as $param => $index) 
	    	{
	   			$filter .= "{$index} LIKE '%{$this->getRequest()->getParam('sSearch')}%' OR ";
	    	}
	    	
	    	return substr_replace($filter, '', strrpos($filter, 'OR'), 4);
    	}
    	    	
    	return $filter;
    }
    
    /**
     * Get the decoded param
     *
     * @access protected
     * @param $param
     */
    final protected function _getDecodedParam($param) {
    	return base64_decode($this->_getParam($param));
    }
    
    
    /**
     * Set the enoded param
     *
     * @access protected
     * @param $param
     */
    final public function setEncodeParam($param) {
    	return base64_encode($param);
    }
    
}
