<?php


class Ted_Form_RelatorioCumprimentoObjeto extends Ted_Form_Abstract
{
    /**
     *
     */
    public function init()
    {
        parent::init();

        $this->setName('rcoobjeto');
        $this->setAttrib('id', 'rcoobjeto');
        $this->setMethod(Zend_Form::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $tcpid = new Zend_Form_Element_Hidden('tcpid');
        $tcpid->setAttrib('id', 'tcpid');
        $tcpid->setRequired(true);
        $tcpid->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($tcpid);

        $recid = new Zend_Form_Element_Hidden('recid');
        $recid->setAttrib('id', 'recid');
        $this->addElement($recid);

        $reccnpj = new Zend_Form_Element_Text('reccnpj');
        $reccnpj->setAttrib('id', 'reccnpj');
        $reccnpj->setAttrib('onkeyup', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);');
        $reccnpj->setAttrib('onblur', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);');
        $reccnpj->setAttrib('maxlength', '18');
        $reccnpj->setAttrib('class', 'form-control');
        $reccnpj->setRequired(true);
        $reccnpj->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($reccnpj);

        $recnome = new Zend_Form_Element_Text('recnome');
        $recnome->setAttrib('maxlength', '255');
        $recnome->setAttrib('id', 'recnome');
        $recnome->setAttrib('class', 'form-control');
        $recnome->setRequired(true);
        $recnome->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recnome);

        $recendereco = new Zend_Form_Element_Text('recendereco');
        $recendereco->setRequired();
        $recendereco->setAttrib('maxlength', '500');
        $recendereco->setAttrib('id', 'recendereco');
        $recendereco->setAttrib('class', 'form-control');
        $recendereco->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recendereco);

        $estuf = new Zend_Form_Element_Select('estuf');
        $estuf->setRequired(true);
        $estuf->setAttrib('id', 'estuf');
        $estuf->setAttrib('class', 'form-control chosen-select');
        $estuf->setAttrib('style', 'width:100%;');
        $estuf->setAttrib('required', 'true');
        $estuf->addMultiOption('', 'Selecione a UF');
        $estuf->addMultiOptions(Ted_Utils_Model::pegaUF());
        $estuf->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($estuf);

        $muncod = new Zend_Form_Element_Select('muncod');
        $muncod->setRequired(true);
        $muncod->setAttrib('id', 'muncod');
        $muncod->setAttrib('class', 'form-control chosen-select');
        $muncod->setAttrib('style', 'width:100%;');
        $muncod->setAttrib('required', 'true');
        $muncod->addMultiOption('', 'Selecione');
        $muncod->addMultiOptions(array());
        $muncod->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($muncod);

        $reccep = new Zend_Form_Element_Text('reccep');
        $reccep->setAttrib('id', 'reccep');
        $reccep->setAttrib('onkeyup', 'this.value=mascaraglobal(\'##.###-###\',this.value);');
        $reccep->setAttrib('onblur', 'this.value=mascaraglobal(\'##.###-###\',this.value);');
        $reccep->setAttrib('maxlength', '10');
        $reccep->setAttrib('class', 'form-control');
        $reccep->setRequired(true);
        $reccep->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($reccep);

        $rectelefoneddd = new Zend_Form_Element_Text('rectelefoneddd');
        $rectelefoneddd->setAttrib('id', 'rectelefoneddd');
        $rectelefoneddd->setAttrib('onkeyup', 'this.value=mascaraglobal(\'########\',this.value);');
        $rectelefoneddd->setAttrib('maxlength', '2');
        $rectelefoneddd->setAttrib('class', 'form-control');
        $rectelefoneddd->setRequired(true);
        $rectelefoneddd->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($rectelefoneddd);

        $rectelefone = new Zend_Form_Element_Text('rectelefone');
        $rectelefone->setAttrib('id', 'rectelefone');
        $rectelefone->setAttrib('onkeyup', 'this.value=mascaraglobal(\'####-####\',this.value);');
        $rectelefone->setAttrib('onblur', 'this.value=mascaraglobal(\'####-####\',this.value);');
        $rectelefone->setAttrib('maxlength', '11');
        $rectelefone->setAttrib('class', 'form-control');
        $rectelefone->setRequired(true);
        $rectelefone->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($rectelefone);

        $uocod = new Zend_Form_Element_Text('uocod');
        $uocod->setAttrib('id', 'uocod');
        $uocod->setAttrib('onkeyup', 'this.value=mascaraglobal(\'########\',this.value);');
        $uocod->setAttrib('maxlength', '15');
        $uocod->setAttrib('class', 'form-control');
        $uocod->setRequired(true);
        $uocod->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($uocod);

        $ugcod = new Zend_Form_Element_Text('ugcod');
        $ugcod->setAttrib('id', 'ugcod');
        $ugcod->setAttrib('onkeyup', 'this.value=mascaraglobal(\'########\',this.value);');
        $ugcod->setAttrib('maxlength', '15');
        $ugcod->setAttrib('class', 'form-control');
        $ugcod->setRequired(true);
        $ugcod->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($ugcod);

        $gestaocod = new Zend_Form_Element_Text('gestaocod');
        $gestaocod->setAttrib('id', 'gestaocod');
        $gestaocod->setAttrib('onkeyup', 'this.value=mascaraglobal(\'########\',this.value);');
        $gestaocod->setAttrib('maxlength', '15');
        $gestaocod->setAttrib('class', 'form-control');
        $gestaocod->setRequired(true);
        $gestaocod->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($gestaocod);

        $recnomeresponsavel = new Zend_Form_Element_Text('recnomeresponsavel');
        $recnomeresponsavel->setAttrib('id', 'recnomeresponsavel');
        $recnomeresponsavel->setAttrib('maxlength', '255');
        $recnomeresponsavel->setAttrib('class', 'form-control');
        $recnomeresponsavel->setRequired(true);
        $recnomeresponsavel->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recnomeresponsavel);

        $reccpfresponsavel = new Zend_Form_Element_Text('reccpfresponsavel');
        $reccpfresponsavel->setAttrib('id', 'reccpfresponsavel');
        $reccpfresponsavel->setAttrib('onkeyup', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);');
        $reccpfresponsavel->setAttrib('onblur', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);');
        $reccpfresponsavel->setAttrib('maxlength', '14');
        $reccpfresponsavel->setAttrib('class', 'form-control');
        $reccpfresponsavel->setRequired(true);
        $reccpfresponsavel->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($reccpfresponsavel);

        $recsiaperesponsavel = new Zend_Form_Element_Text('recsiaperesponsavel');
        $recsiaperesponsavel->setAttrib('id', 'recsiaperesponsavel');
        $recsiaperesponsavel->setAttrib('onkeyup', 'this.value=mascaraglobal(\'####################\',this.value);');
        $recsiaperesponsavel->setAttrib('maxlength', '20');
        $recsiaperesponsavel->setAttrib('class', 'form-control');
        $recsiaperesponsavel->setRequired(true);
        $recsiaperesponsavel->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recsiaperesponsavel);

        $recrgresponsavel = new Zend_Form_Element_Text('recrgresponsavel');
        $recrgresponsavel->setAttrib('id', 'recrgresponsavel');
        $recrgresponsavel->setAttrib('maxlength', '20');
        $recrgresponsavel->setAttrib('class', 'form-control');
        $recrgresponsavel->setRequired(true);
        $recrgresponsavel->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recrgresponsavel);

        $recdtemissaorgresposavel = new Zend_Form_Element_Text('recdtemissaorgresposavel');
        $recdtemissaorgresposavel->setAttrib('id', 'recdtemissaorgresposavel');
        $recdtemissaorgresposavel->setAttrib('onblur', 'this.value=mascaraglobal(\'##/##/####\',this.value);');
        $recdtemissaorgresposavel->setAttrib('maxlength', '12');
        $recdtemissaorgresposavel->setAttrib('class', 'form-control widget-date-control');
        $recdtemissaorgresposavel->setRequired(true);
        $recdtemissaorgresposavel->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recdtemissaorgresposavel);

        $recexpedidorrgresposavel = new Zend_Form_Element_Text('recexpedidorrgresposavel');
        $recexpedidorrgresposavel->setAttrib('id', 'recexpedidorrgresposavel');
        $recexpedidorrgresposavel->setAttrib('maxlength', '15');
        $recexpedidorrgresposavel->setAttrib('class', 'form-control');
        $recexpedidorrgresposavel->setRequired(true);
        $recexpedidorrgresposavel->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recexpedidorrgresposavel);

        $reccargo = new Zend_Form_Element_Text('reccargo');
        $reccargo->setAttrib('id', 'reccargo');
        $reccargo->setAttrib('maxlength', '255');
        $reccargo->setAttrib('class', 'form-control');
        $reccargo->setRequired(true);
        $reccargo->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($reccargo);

        $recemailresposavel = new Zend_Form_Element_Text('recemailresposavel');
        $recemailresposavel->setAttrib('id', 'recemailresposavel');
        $recemailresposavel->setAttrib('maxlength', '80');
        $recemailresposavel->setAttrib('class', 'form-control');
        $recemailresposavel->setRequired(true);
        $recemailresposavel->addValidator(new Zend_Validate_EmailAddress());
        $recemailresposavel->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recemailresposavel);

        $recnumportaria = new Zend_Form_Element_Text('recnumportaria');
        $recnumportaria->setAttrib('id', 'recnumportaria');
        $recnumportaria->setAttrib('maxlength', '150');
        $recnumportaria->setAttrib('class', 'form-control');
        $recnumportaria->setRequired(true);
        $recnumportaria->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recnumportaria);

        $recdtpublicacao = new Zend_Form_Element_Text('recdtpublicacao');
        $recdtpublicacao->setAttrib('id', 'recdtpublicacao');
        $recdtpublicacao->setAttrib('onblur', 'this.value=mascaraglobal(\'##/##/####\',this.value);');
        $recdtpublicacao->setAttrib('maxlength', '12');
        $recdtpublicacao->setAttrib('class', 'form-control widget-date-control');
        $recdtpublicacao->setRequired(true);
        $recdtpublicacao->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recdtpublicacao);

        $recnumnotacredito = new Zend_Form_Element_Text('recnumnotacredito');
        $recnumnotacredito->setAttrib('id', 'recnumnotacredito');
        $recnumnotacredito->setAttrib('maxlength', '25');
        $recnumnotacredito->setAttrib('class', 'form-control');
        $recnumnotacredito->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recnumnotacredito);

        $objetoOptions = Ted_Model_RelatorioCumprimento::getOptionsExecucaoObjeto();
        $recexecucaoobjeto = new Zend_Form_Element_Select('recexecucaoobjeto');
        $recexecucaoobjeto->setRequired(true);
        $recexecucaoobjeto->setAttrib('id', 'recexecucaoobjeto');
        $recexecucaoobjeto->setAttrib('class', 'form-control chosen-select');
        $recexecucaoobjeto->setAttrib('style', 'width:100%;');
        $recexecucaoobjeto->setAttrib('required', 'true');
        $recexecucaoobjeto->addMultiOption('', 'Selecione');
        $recexecucaoobjeto->addMultiOptions($objetoOptions);
        $recexecucaoobjeto->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recexecucaoobjeto);

        $recatividadesprevistas = new Zend_Form_Element_Textarea('recatividadesprevistas');
        $recatividadesprevistas->setAttrib('rows', '3');
        $recatividadesprevistas->setAttrib('cols', '75');
        $recatividadesprevistas->setAttrib('id', 'recatividadesprevistas');
        $recatividadesprevistas->setAttrib('class', 'form-control');
        $recatividadesprevistas->setRequired();
        $recatividadesprevistas->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recatividadesprevistas);

        $recmetaprevista = new Zend_Form_Element_Textarea('recmetaprevista');
        $recmetaprevista->setAttrib('rows', '3');
        $recmetaprevista->setAttrib('cols', '75');
        $recmetaprevista->setAttrib('id', 'recmetaprevista');
        $recmetaprevista->setAttrib('class', 'form-control');
        $recmetaprevista->setRequired();
        $recmetaprevista->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recmetaprevista);

        $recatividadesexecutadas = new Zend_Form_Element_Textarea('recatividadesexecutadas');
        $recatividadesexecutadas->setAttrib('rows', '3');
        $recatividadesexecutadas->setAttrib('cols', '75');
        $recatividadesexecutadas->setAttrib('id', 'recatividadesexecutadas');
        $recatividadesexecutadas->setAttrib('class', 'form-control');
        $recatividadesexecutadas->setRequired();
        $recatividadesexecutadas->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recatividadesexecutadas);

        $recmetaexecutada = new Zend_Form_Element_Textarea('recmetaexecutada');
        $recmetaexecutada->setAttrib('rows', '3');
        $recmetaexecutada->setAttrib('cols', '75');
        $recmetaexecutada->setAttrib('id', 'recmetaexecutada');
        $recmetaexecutada->setAttrib('class', 'form-control');
        $recmetaexecutada->setRequired();
        $recmetaexecutada->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recmetaexecutada);

        $recdificuldades = new Zend_Form_Element_Textarea('recdificuldades');
        $recdificuldades->setAttrib('rows', '3');
        $recdificuldades->setAttrib('cols', '75');
        $recdificuldades->setAttrib('id', 'recdificuldades');
        $recdificuldades->setAttrib('class', 'form-control');
        $recdificuldades->setRequired();
        $recdificuldades->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recdificuldades);

        $recmetasadotadas = new Zend_Form_Element_Textarea('recmetasadotadas');
        $recmetasadotadas->setAttrib('rows', '3');
        $recmetasadotadas->setAttrib('cols', '75');
        $recmetasadotadas->setAttrib('id', 'recmetasadotadas');
        $recmetasadotadas->setAttrib('class', 'form-control');
        $recmetasadotadas->setRequired();
        $recmetasadotadas->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recmetasadotadas);

        $reccomentarios = new Zend_Form_Element_Textarea('reccomentarios');
        $reccomentarios->setAttrib('rows', '3');
        $reccomentarios->setAttrib('cols', '75');
        $reccomentarios->setRequired();
        $reccomentarios->setAttrib('id', 'reccomentarios');
        $reccomentarios->setAttrib('class', 'form-control');
        $reccomentarios->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($reccomentarios);

        $recvlrrecebido = new Zend_Form_Element_Text('recvlrrecebido');
        $recvlrrecebido->setAttrib('id', 'recvlrrecebido');
        $recvlrrecebido->setAttrib('onkeyup', 'this.value=mascaraglobal(\'[.###],##\',this.value);');
        $recvlrrecebido->setAttrib('onblur', 'this.value=mascaraglobal(\'[.###],##\',this.value);');
        $recvlrrecebido->setAttrib('maxlength', '');
        $recvlrrecebido->setAttrib('class', 'form-control');
        $recvlrrecebido->setRequired(true);
        //$recvlrrecebido->setValue('3524588');
        $recvlrrecebido->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recvlrrecebido);

        $recvlrutilizado = new Zend_Form_Element_Text('recvlrutilizado');
        $recvlrutilizado->setAttrib('id', 'recvlrutilizado');
        $recvlrutilizado->setAttrib('onkeyup', 'this.value=mascaraglobal(\'[.###],##\',this.value);');
        $recvlrutilizado->setAttrib('onblur', 'this.value=mascaraglobal(\'[.###],##\',this.value);');
        $recvlrutilizado->setAttrib('class', 'form-control');
        $recvlrutilizado->setRequired(true);
        //$recvlrutilizado->setValue('3524588');
        $recvlrutilizado->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recvlrutilizado);

        $recvlrdevolvido = new Zend_Form_Element_Text('recvlrdevolvido');
        $recvlrdevolvido->setAttrib('id', 'recvlrdevolvido');
        $recvlrdevolvido->setAttrib('onkeyup', 'this.value=mascaraglobal(\'[.###],##\',this.value);');
        $recvlrdevolvido->setAttrib('onblur', 'this.value=mascaraglobal(\'[.###],##\',this.value);');
        $recvlrdevolvido->setAttrib('class', 'form-control');
        //$recvlrdevolvido->setRequired(true);
        $recvlrdevolvido->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($recvlrdevolvido);

        $recnumnotacredito_dev = new Zend_Form_Element_Text('recnumnotacredito_dev');
        $recnumnotacredito_dev->setAttrib('id', 'recnumnotacredito_dev');
        $recnumnotacredito_dev->setAttrib('maxlength', '25');
        $recnumnotacredito_dev->setAttrib('class', 'form-control');
        $this->addElement($recnumnotacredito_dev);

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'relatorioobjeto-form.php'))));
        $this->setElementDecorators(array('ViewHelper', 'Errors'));

        $this->_loadDefaultSets();
    }
}
