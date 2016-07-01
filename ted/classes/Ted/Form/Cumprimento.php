<?php
class Ted_Form_Cumprimento extends Ted_Form_Abstract{

	public function init()
	{
		parent::init();
		$this->setName('cumprimento')
		->setAttrib('enctype', 'multipart/form-data')
		->setAttrib('id', 'cumprimento')
		->setMethod(Zend_Form::METHOD_POST)
		->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);
		
		$cnpj = new Zend_Form_Element_Text('cnpj');
		
		$nmEntidade = new Zend_Form_Element_Text('nmEntidade');
		
		$endereco = new Zend_Form_Element_Text('endereco');
		
		$uf = new Zend_Form_Element_Select('uf');
		
		$municipio = new Zend_Form_Element_Select('municipio');
		
		$cep = new Zend_Form_Element_Text('cep');
		
		$telefone = new Zend_Form_ElementText('telefone');
		
		$codigoUO = new Zend_Form_Element_Text('codigoUO');
		
		$codigoUG = new Zend_Form_Element_Text('codigoUG');
		
		$codigoGestao = new Zend_Form_Element_Text('codigoGestao');
		
		$nmResponsavel = new Zend_Form_Element_Text('nmResponsavel');
		
		$cpf = new Zend_Form_Element_Text('cpf');
		
		$siape = new Zend_Form_Element_Text('siape');
		
		$idt = new Zend_Form_Element_Text('idt');
		
		$dtEmissao = new Zend_Form_Element_Text('dtEmissao');
		
		$orgExpedidor = new Zend_Form_Element_Text('orgExpedidor');
		
		$cargo = new Zend_Form_Element_Text('cargo');
		
		$email = new Zend_Form_Element_Text('email');
		
		$nPortaria = new Zend_Form_Element_Text('nPortaria');
		
		$dtPublicacao = new Zend_Form_Element_Text('dtPublicacao');
		
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'cumprimento-form.php'))));
		$this->addElements(array());
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		
		$this->_loadDefaultSets();
	}
}