<?php

class Ted_Form_EnviarNotaCredito extends Ted_Form_Abstract
{
	public function init()
	{
		parent::init();

		$this->setName('enviarNC')
		->setAttrib('id', 'enviarNC')
		->setMethod(Zend_Form::METHOD_POST)
		->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

		$tcpid = new Zend_Form_Element_Hidden('tcpid');
		$tcpid->setAttribs(array('id'=>'tcpid'));
		$tcpid->setValue($_GET['ted']);

		$sigefusername = new Zend_Form_Element_Text('sigefusername');
		$sigefusername->setAttrib('class', 'form-control');
		$sigefusername->setRequired(true);
		$sigefusername->setAttrib('id', 'sigefusername');
		$sigefusername->setAttrib('required', 'true');
		$sigefusername->addErrorMessage('Valor é necessário e não pode ser vazio');

		$sigefpassword = new Zend_Form_Element_Password('sigefpassword');
		$sigefpassword->setRequired(true);
		$sigefpassword->setAttrib('id', 'sigefpassword');
		$sigefpassword->setAttrib('class', 'form-control');
		$sigefpassword->setAttrib('required', 'true');
		$sigefpassword->addErrorMessage('Valor é necessário e não pode ser vazio');
		
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'enviar-notacredito-form.php'))));
		
		$this->addElements(array($tcpid, $sigefusername, $sigefpassword));
		
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		$this->_loadDefaultSets();
	}
}

