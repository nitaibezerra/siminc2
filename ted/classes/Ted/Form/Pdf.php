<?php

/**
 * Class Ted_Form_Pdf
 */
class Ted_Form_Pdf extends Ted_Form_Abstract
{
	public function init()
	{
		parent::init();		
	
		$this->setName('pdf');
		$this->setAttrib('id', 'pdf');
		$this->setMethod('post');
		$this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

		$tcpid = new Zend_Form_Element_Hidden('tcpid');		
		$tcpid->setAttribs(array('id'=>'tcpid'));
				
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'pdf-form.php'))));
		$this->addElements(array($tcpid));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		$this->_loadDefaultSets();
	}
}