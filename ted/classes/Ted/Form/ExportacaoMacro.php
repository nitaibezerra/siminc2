<?php

class Ted_Form_ExportacaoMacro extends Ted_Form_Abstract
{
	public function init()
	{
		parent::init();
		
		$this->setName('filtro-ted')
		->setAttrib('id', 'form-filtro-ted')
		->setMethod(Zend_Form::METHOD_POST)
		->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);
		
		$lotid = new Zend_Form_Element_Select('lotid');
		$lotid->setAttrib('id', 'lotid')
		->setAttrib('class', 'form-control chosen-select')
		->addMultiOption('','Selecione')
		->addMultiOptions(array());
		
		$lotdsc = new Zend_Form_Element_Text('lotdsc');
		$lotdsc->setRequired(FALSE)
		->setAttrib('id', 'lotdsc')
		->setAttrib('value', $_REQUEST['lotdsc'])
		->setAttrib('class', 'form-control');
		
		$lotdata = new Zend_Form_Element_Text('lotdata');
		$lotdata->setRequired(FALSE)
		->setAttrib('id', 'lotdata')
		->setAttrib('value', $_REQUEST['lotdata'])
		->setAttrib('class', 'form-control campoData');

		$usucpf = new Zend_Form_Element_Text('usucpf');
		$usucpf->setRequired(FALSE)
		->setAttrib('id', 'usucpf')
		->setAttrib('value', $_REQUEST['usucpf'])
		->setAttrib('class', 'form-control campoCpf');
		
		$usunome = new Zend_Form_Element_Text('usunome');
		$usunome->setRequired(FALSE)
		->setAttrib('id', 'usunome')
		->setAttrib('value', $_REQUEST['usunome'])
		->setAttrib('class', 'form-control');
		
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'exportacao-macro.php'))));
		$this->addElements(array($lotid, $lotdsc, $lotdata, $usucpf, $usunome));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		
		$this->_loadDefaultSets();
		
	}
}