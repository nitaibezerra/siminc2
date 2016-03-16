<?php

class Ted_Form_Anexo extends Ted_Form_Abstract
{
	public function init()
	{
		parent::init();
		$this->setName('anexo');
		$this->setAttrib('id', 'anexo');
		$this->setMethod(Ted_Form_Abstract::METHOD_POST);
		$this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

		$tcpid = new Zend_Form_Element_Hidden('tcpid');
		$tcpid->setAttribs(array('id'=>'tcpid'));

		$anexo = new Zend_Form_Element_File('anexo[]');
		$anexo->setAttribs(array('id'=>'anexo'));

		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'anexo-form.php'))));
		$this->addElements(array($tcpid,$anexo));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		
		$this->_loadDefaultSets();
	}
}