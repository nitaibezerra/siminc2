<?php

/**
 * Class Ted_Form_TramitaLote
 */
class Ted_Form_TramitaLote extends Ted_Form_Abstract
{
    /**
     * init()
     */
    public function init()
    {
        parent::init();
        $this->setName('tramitaLote');
        $this->setAttrib('id', 'tramitaLote');
        $this->setMethod(Ted_Form_Abstract::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $action = new Zend_Form_Element_Hidden('action');
        $action->setAttrib('id', 'action');

        $docid = new Zend_Form_Element_Hidden('docid');
        $docid->setAttrib('id', 'docid');

        $esdid = new Zend_Form_Element_Select('esdid');
        $esdid->setAttrib('class', 'form-control chosen-select');
        $esdid->setAttrib('id', 'esdid');
        $esdid->setAttrib('required', 'true');
        $esdid->addMultiOption('', 'Selecione a situação');
        $esdid->setRequired(true);
        $esdid->addMultiOptions(array());

        $aedid = new Zend_Form_Element_Select('aedid');
        $aedid->setRequired(true);
        $aedid->setAttrib('id', 'aedid');
        $aedid->setAttrib('class', 'form-control chosen-select');
        $aedid->setAttrib('required', 'true');
        $aedid->addMultiOption('', 'Selecione a ação');

        $comment = new Zend_Form_Element_Textarea('cmddsc');
        $comment->setAttrib('id', 'cmddsc');
        $comment->setAttrib('class', 'form-control');
        $comment->setAttrib('rows', 5);
        $comment->setAttrib('cols', 25);

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'tramita-lote-form.php'))));

        $this->addElements(array($action, $docid, $esdid, $aedid, $comment));

        $this->setElementDecorators(array('ViewHelper', 'Errors'));
        $this->_loadDefaultSets();
    }
}