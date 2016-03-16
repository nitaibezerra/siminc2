<?php

class Ted_Form_ParecerRCO extends Ted_Form_Abstract
{
    public function init()
    {
        parent::init();

        $this->setName('parecerrco');
        $this->setAttrib('id', 'parecerrco');
        $this->setMethod(Ted_Form_Abstract::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $tcpobsrelatorio = new Zend_Form_Element_Textarea('tcpobsrelatorio');
        $tcpobsrelatorio->setAttrib('id', 'tcpobsrelatorio');
        $tcpobsrelatorio->setAttrib('class', 'form-control');
        $tcpobsrelatorio->setAttrib('maxlength', '5000');
        $tcpobsrelatorio->setAttrib('rows', '4');
        $tcpobsrelatorio->setRequired(true);
        $tcpobsrelatorio->addErrorMessage('Valor é necessário e não pode ser vazio');

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'parecer-rco-form.php'))));
        $this->addElements(array($rco_parecer, $tcpid, $tcpobsrelatorio));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));
        $this->_loadDefaultSets();
    }
}