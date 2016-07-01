<?php

class Ted_Form_Parecer extends Ted_Form_Abstract
{			
	public function init()
	{
		parent::init();		

		$this->setName('parecer');
		$this->setAttrib('id', 'parecer');
		$this->setMethod('post');
		$this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

		$tcpid = new Zend_Form_Element_Hidden('tcpid');
        $tcpid->setRequired(true);
        $tcpid->addErrorMessage('Valor é necessário e não pode ser vazio');
		$tcpid->setAttribs(array('id'=>'tcpid'));
		
		$ptecid = new Zend_Form_Element_Hidden('ptecid');
		$ptecid->setAttrib('id','ptecid');
			
		$considentproponente = new Zend_Form_Element_Textarea('considentproponente');
        $considentproponente->setRequired(true);
        $considentproponente->addErrorMessage('Valor é necessário e não pode ser vazio');
		$considentproponente->setAttribs(array('id'=>'considentproponente','class'=>'form-control','maxlength'=>'1000','rows'=>'4'));
		
		$considproposta = new Zend_Form_Element_Textarea('considproposta');
		$considproposta->setRequired(true);
		$considproposta->addErrorMessage('Valor é necessário e não pode ser vazio');
		$considproposta->setAttribs(array('id'=>'considproposta','class'=>'form-control','maxlength'=>'1000','rows'=>'4'));
		
		$considobjeto = new Zend_Form_Element_Textarea('considobjeto');
		$considobjeto->setRequired(true);
		$considobjeto->addErrorMessage('Valor é necessário e não pode ser vazio');
		$considobjeto->setAttribs(array('id'=>'considobjeto','class'=>'form-control','maxlength'=>'1000','rows'=>'4'));
		
		$considobjetivo = new Zend_Form_Element_Textarea('considobjetivo');
		$considobjetivo->setRequired(true);
		$considobjetivo->addErrorMessage('Valor é necessário e não pode ser vazio');
		$considobjetivo->setAttribs(array('id'=>'considobjetivo','class'=>'form-control','maxlength'=>'1000','rows'=>'4'));
		
		$considjustificativa = new Zend_Form_Element_Textarea('considjustificativa');
		$considjustificativa->setRequired(true);
		$considjustificativa->addErrorMessage('Valor é necessário e não pode ser vazio');
		$considjustificativa->setAttribs(array('id'=>'considjustificativa','class'=>'form-control','maxlength'=>'1000','rows'=>'4'));
		
		$considvalores = new Zend_Form_Element_Textarea('considvalores');
		$considvalores->setRequired(true);
		$considvalores->addErrorMessage('Valor é necessário e não pode ser vazio');
		$considvalores->setAttribs(array('id'=>'considvalores','class'=>'form-control','maxlength'=>'1000','rows'=>'4'));
		
		$considcabiveis = new Zend_Form_Element_Textarea('considcabiveis');
		$considcabiveis->setRequired(true);
		$considcabiveis->addErrorMessage('Valor é necessário e não pode ser vazio');
		$considcabiveis->setAttribs(array('id'=>'considcabiveis','class'=>'form-control','maxlength'=>'1000','rows'=>'4'));
		
		$usucpfparecer = new Zend_Form_Element_Hidden('usucpfparecer');
        $usucpfparecer->setRequired(true);
        $usucpfparecer->addErrorMessage('Valor é necessário e não pode ser vazio');
		$usucpfparecer->setAttribs(array('id'=>'usucpfparecer'));
		
		/*$anexo = new Zend_Form_Element_File('anexo[]');
		$anexo->setAttribs(array('id'=>'anexo'));*/
		
		$usunome = new Zend_Form_Element_Text('usunome');
		$usunome->setAttribs(array('id'=>'usunome','class'=>'form-control','disabled'=>''));
		
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'parecer-form.php'))));
		$this->addElements(array($tcpid,$ptecid,$considentproponente,$considproposta,$considobjeto,$considobjetivo,
		$considjustificativa,$considvalores,$considcabiveis,$usucpfparecer,$usunome));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		
		$this->_loadDefaultSets();
	}

}