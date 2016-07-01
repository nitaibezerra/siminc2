<?php

/**
 * Class Ted_Form_ParecerJuridico
 */
class Ted_Form_ParecerJuridico extends Ted_Form_Abstract
{
    public function init()
    {
        parent::init();

        $this->setName('parecejuridico');
        $this->setAttrib('id', 'parecejuridico');
        $this->setMethod(Ted_Form_Abstract::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $pcjid = new Zend_Form_Element_Hidden('pcjid');
        $pcjid->setAttribs(array('id' => 'pcjid'));

        $tcpid = new Zend_Form_Element_Hidden('tcpid');
        $tcpid->setAttribs(array('id' => 'tcpid'));

        $ungcod = new Zend_Form_Element_Hidden('ungcod');
        $ungcod->setAttribs(array('id' => 'ungcod'));

        $obsparecer = new Zend_Form_Element_Textarea('obsparecer');
        $obsparecer->setAttrib('id', 'obsparecer');
        $obsparecer->setAttrib('class', 'form-control');
        $obsparecer->setAttrib('maxlength', '1000');
        $obsparecer->setAttrib('rows', '4');
        $obsparecer->setRequired(true);
        $obsparecer->addErrorMessage('Valor é necessário e não pode ser vazio');

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'parecer-juridico-form.php'))));
        $this->addElements(array($pcjid, $tcpid, $ungcod, $obsparecer));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));
        $this->_loadDefaultSets();
    }
}