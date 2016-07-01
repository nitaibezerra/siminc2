<?php

class Ted_Form_FiltraTermo extends Ted_Form_Abstract
{

    public function init()
    {
        parent::init();

        $this->setName('filtro-ted')
            ->setAttrib('id', 'form-filtro-ted')
            ->setMethod(Zend_Form::METHOD_POST)
            ->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $ungcod = new Zend_Form_Element_Hidden('ungcod');
        $export = new Zend_Form_Element_Hidden('export');

        $unicod = new Zend_Form_Element_Select('unicod');
        $unicod->setAttrib('id', 'unicod');
        $unicod->setAttrib('class', 'form-control chosen-select');
        $unicod->addMultiOption('','Selecione');
        $unicod->addMultiOptions(array());


        $tcpid = new Zend_Form_Element_Text('tcpid');
        $tcpid->setRequired(FALSE)
            ->setAttrib('id', 'tcpid')
            ->setAttrib('maxlength', '4')
            ->setAttrib('onKeyUp', 'this.value=mascaraglobal(\'#######\',this.value);')
            ->setAttrib('class', 'form-control');

        $tcpnumtransfsiafi = new Zend_Form_Element_Text('tcpnumtransfsiafi');
        $tcpnumtransfsiafi->setAttrib('id', 'tcpnumtransfsiafi');
        $tcpnumtransfsiafi->setAttrib('maxlength', '6');
        $tcpnumtransfsiafi->setAttrib('onKeyUp', 'this.value=mascaraglobal(\'######\',this.value);');
        $tcpnumtransfsiafi->setAttrib('class', 'form-control');

        $ungcodproponente = new Zend_Form_Element_Select('ungcodproponente');
        $ungcodproponente->setRequired(FALSE)
            ->setAttrib('id', 'message')
            ->setAttrib('class', 'form-control chosen-select')
            ->addMultiOption('', 'Selecione')            
            ->addMultiOptions(array());

        $ungcodconcedente = new Zend_Form_Element_Select('ungcodconcedente');
        $ungcodconcedente->setRequired(FALSE)
            ->setAttrib('id', 'ungcodconcedente')
            ->setAttrib('class', 'form-control chosen-select')
            ->addMultiOption('', 'Selecione')            
            ->addMultiOptions(array());

        $esdid = new Zend_Form_Element_Select('esdid');
        $esdid->setRequired(FALSE)
            ->setAttrib('id', 'esdid')
            ->setAttrib('class', 'form-control chosen-select')
            ->addMultiOption('', 'Selecione')            
            ->addMultiOptions(array());

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'filtro-termos.php'))));
        $this->addElements(array($ungcod, $export, $tcpid, $tcpnumtransfsiafi, $unicod, $ungcodproponente, $ungcodconcedente, $esdid));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));

        $this->_loadDefaultSets();
    }

}