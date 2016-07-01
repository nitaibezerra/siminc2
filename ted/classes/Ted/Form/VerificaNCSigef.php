<?php

/**
 * Class Ted_Form_VerificaNCSigef
 */
class Ted_Form_VerificaNCSigef extends Ted_Form_Abstract
{
    /**
     * @initialize()
     */
    public function init()
    {
        parent::init();
        $this->setName('verificaSigef');
        $this->setAttrib('id', 'verificaSigef');
        $this->setMethod(Zend_Form::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $tcpid = new Zend_Form_Element_Hidden('tcpid');
        $tcpid->setAttribs(array('id' => 'tcpid'));

        $funcao = new Zend_Form_Element_Hidden('funcao');
        $funcao->setAttribs(array('id' => 'verifica_nc'));
        $funcao->setValue('verifica_nc');

        $sigefusername = new Zend_Form_Element_Text('sigefusername');
        $sigefusername->setAttrib('class', 'form-control');
        $sigefusername->setAttrib('id', 'sigefusername');
        $sigefusername->setRequired();
        $sigefusername->addErrorMessage('Valor é necessário e não pode ser vazio');
        //$sigefusername->setValue('luciab');

        $sigefpassword = new Zend_Form_Element_Password('sigefpassword');
        $sigefpassword->setAttrib('class', 'form-control');
        $sigefpassword->setAttrib('id', 'sigefpassword');
        $sigefpassword->setRequired();
        $sigefpassword->addErrorMessage('Valor é necessário e não pode ser vazio');

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'verificaSigef-form.php'))));
        $this->addElements(array($tcpid, $funcao, $sigefusername, $sigefpassword));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));

        $this->_loadDefaultSets();
    }
}

