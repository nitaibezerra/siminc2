<?php

class Ted_Form_Tramite extends Ted_Form_Abstract
{			
	public function init()
	{		
		parent::init();
	
		$this->setName('tramite')
		->setAttrib('id', 'tramite')
		->setMethod('post')
		->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

		$tcpid = new Zend_Form_Element_Hidden('tcpid');		
		$tcpid->setAttribs(array('id'=>'tcpid'));

		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'tramite-form.php'))));
		$this->addElements(array($tcpid, ));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		
		$this->_loadDefaultSets();
	}
}