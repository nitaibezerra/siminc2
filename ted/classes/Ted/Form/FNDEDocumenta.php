<?php

/**
 * Class Ted_Form_FNDEDocumenta
 */

class Ted_Form_FNDEDocumenta extends Ted_Form_Abstract
{
    /**
     * @initialize()
     */
    public function init()
    {
        parent::init();
        $this->setName('documenta');
        $this->setAttrib('id', 'documenta');
        $this->setMethod(Zend_Form::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $tcpid = new Zend_Form_Element_Hidden('tcpid');
        $tcpid->setAttribs(array('id' => 'tcpid'));
        
        $logindoc = new Zend_Form_Element_Text('logindoc');
        $logindoc->setAttrib('class', 'form-control');
        $logindoc->setAttrib('id', 'logindoc');
        $logindoc->setRequired();
        $logindoc->addErrorMessage('Valor é necessário e não pode ser vazio');

        $senhadoc = new Zend_Form_Element_Password('senhadoc');
        $senhadoc->setAttrib('class', 'form-control');
        $senhadoc->setAttrib('id', 'senhadoc');
        $senhadoc->setRequired();
        $senhadoc->addErrorMessage('Valor é necessário e não pode ser vazio');

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'documenta-form.php'))));
        $this->addElements(array($tcpid, $logindoc, $senhadoc));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));

        $this->_loadDefaultSets();
    }
}
