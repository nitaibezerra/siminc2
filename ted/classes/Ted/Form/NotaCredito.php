<?php

/**
 * Class Ted_Form_NotaCredito
 */
class Ted_Form_NotaCredito extends Ted_Form_Abstract
{
    public function init()
    {
        parent::init();

        $this->setName('notacredito');
        $this->setAttrib('id', 'notacredito');
        $this->setMethod(Zend_Form::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $proid = new Zend_Form_Element_Hidden('_proid');
        $proid->setAttrib('id', '_proid');
        $proid->setRequired(true);
        $proid->addErrorMessage('Valor é necessário e não pode ser vazio');

        $tcpnumtransfsiafi = new Zend_Form_Element_Text('tcpnumtransfsiafi');
        $tcpnumtransfsiafi->setRequired(true);
        $tcpnumtransfsiafi->setAttrib('id', 'tcpnumtransfsiafi');
        $tcpnumtransfsiafi->setAttrib('class', 'form-control');
        $tcpnumtransfsiafi->addErrorMessage('Valor é necessário e não pode ser vazio');

        $codncsiafi = new Zend_Form_Element_Text('codncsiafi');
        $codncsiafi->setRequired(true);
        $codncsiafi->setAttrib('id', 'codncsiafi');
        $codncsiafi->setAttrib('class', 'form-control');
        $codncsiafi->addErrorMessage('Valor é necessário e não pode ser vazio');

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'notacredito-form.php'))));
        $this->addElements(array($proid, $tcpnumtransfsiafi, $codncsiafi));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));
        $this->_loadDefaultSets();
    }
}