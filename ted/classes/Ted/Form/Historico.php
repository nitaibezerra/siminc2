<?php

/**
 * Class Ted_Form_Historico
 */
class Ted_Form_Historico extends Ted_Form_Abstract
{
    public function init()
    {
        parent::init();

        $selectVersion = new Zend_Form_Element_Select('version');
        $selectVersion->setRequired(true);
        $selectVersion->setAttrib('id', 'version');
        $selectVersion->setAttrib('class', 'form-control chosen-select');
        $selectVersion->setAttrib('style', 'width:100%;');
        $selectVersion->addMultiOption('', '- Selecione -');
        $selectVersion->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($selectVersion);

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'version-form.php'))));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));
        $this->_loadDefaultSets();
    }
}
