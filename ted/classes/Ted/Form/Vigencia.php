<?php

class Ted_Form_Vigencia extends Ted_Form_Abstract
{
    public function init()
    {
        parent::init();
        $this->setName('vigencia');
        $this->setAttrib('id', 'vigencia');
        $this->setMethod(Zend_Form::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $vigid = new Zend_Form_Element_Hidden('vigid');
        $vigid->setAttribs(array('id' => 'vigid'));

        $tcpid = new Zend_Form_Element_Hidden('tcpid');
        $tcpid->setRequired(true);
        $tcpid->setAttribs(array('id' => 'tcpid'));

        $vigdata = new Zend_Form_Element_Text('vigdata');
        $vigdata->setRequired(true);
        $vigdata->setAttrib('id', 'vigdata');
        $vigdata->setAttrib('class', 'form-control widget-date-control');
        $vigdata->addErrorMessage('Valor é necessário e não pode ser vazio');

        $vigjustificativa = new Zend_Form_Element_Textarea('vigjustificativa');
        $vigjustificativa->setRequired(true);
        $vigjustificativa->setAttrib('rows', '5');
        $vigjustificativa->setAttrib('cols', '30');
        $vigjustificativa->setAttrib('id', 'vigjustificativa');
        $vigjustificativa->setAttrib('class', 'form-control');
        $vigjustificativa->addErrorMessage('Valor é necessário e não pode ser vazio');

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'vigencia-form.php'))));
        $this->addElements(array($vigid, $tcpid, $vigdata, $vigjustificativa));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));

        $this->_loadDefaultSets();
    }
}
