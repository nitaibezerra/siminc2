<?php

class Ted_Form_RemanejarCredito extends Ted_Form_Abstract
{
    public function init()
    {
        parent::init();

        $this->setName('remanejar-credito')
             ->setAttrib('id', 'remanejar-credito')
             ->setMethod(Zend_Form::METHOD_POST)
             ->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $proid = new Zend_Form_Element_Hidden('_proid_');
        $proid->setAttrib('id', '_proid_');
        $proid->setRequired(true);
        $proid->addErrorMessage('Valor é necessário e não pode ser vazio');

        $nc_devolucao = new Zend_Form_Element_Text('nc_devolucao');
        $nc_devolucao->setRequired(true);
        $nc_devolucao->setAttrib('id', 'nc_devolucao');
        $nc_devolucao->setAttrib('class', 'form-control');
        $nc_devolucao->addErrorMessage('Valor é necessário e não pode ser vazio');

        $provalor = new Zend_Form_Element_Hidden('_provalor_');
        $provalor->setAttrib('id', '_provalor_');

        $valor_remanejar = new Zend_Form_Element_Text('valor_remanejar');
        $valor_remanejar->setRequired(true);
        $valor_remanejar->setAttrib('id', 'valor_remanejar');
        $valor_remanejar->setAttrib('class', 'form-control');
        $valor_remanejar->setAttrib('onkeyup', "this.value=mascaraglobal('[.###],##',this.value);");
        $valor_remanejar->setAttrib('onblur', "this.value=mascaraglobal('[.###],##',this.value);");
        $valor_remanejar->addErrorMessage('Valor é necessário e não pode ser vazio');

        $observacao = new Zend_Form_Element_Textarea('observacao');
        $observacao->setAttrib('id', 'observacao');
        $observacao->setAttrib('rows', '5');
        $observacao->setAttrib('cols', '30');
        $observacao->setAttrib('class', 'form-control');

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'remanejarcredito-form.php'))));
        $this->addElements(array($proid, $nc_devolucao, $provalor, $valor_remanejar, $observacao));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));
        $this->_loadDefaultSets();
    }
}