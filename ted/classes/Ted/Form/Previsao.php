<?php

class Ted_Form_Previsao extends Ted_Form_Abstract
{			
	public function init()
	{
		$previsaoOrcamentaria = new Ted_Model_PrevisaoOrcamentaria();
		parent::init();
		
		$this->setName('previsao')
		->setAttrib('id', 'previsao')
		->setMethod('post')
		->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $proid = new Zend_Form_Element_Hidden('proid');
        $proid->setAttribs(array('id' => 'proid'));

		$tcpid = new Zend_Form_Element_Hidden('tcpid');		
		$tcpid->setAttribs(array('id'=>'tcpid'));
		
		$ptrid = new Zend_Form_Element_Hidden('ptrid');
		$ptrid->setAttribs(array('id'=>'ptrid'));

		$proanoreferencia = new Zend_Form_Element_Select('proanoreferencia');
        $proanoreferencia->setRequired(true);
		$proanoreferencia->setAttribs(array(
            'id'=>'ano',
            'class'=>'form-control chosen-select ',
            'style'=>'width:100%;',
            'required'=>'true'
        ))
		->addMultiOption('','Selecione o Ano')
		->addMultiOptions($this->_getYears())
        ->addErrorMessage('Valor é necessário e não pode ser vazio');
		
		$programaTrabalho = new Zend_Form_Element_Select('programaTrabalho');
        $programaTrabalho->setRequired(true);
		$programaTrabalho->setAttribs(array(
            'id'=>'programaTrabalho',
            'class'=>'form-control chosen-select ',
            'style'=>'width:100%;',
            'required'=>'true'
        ))
		->addMultiOption('','Selecione o programa de trabalho')
		->addMultiOptions($previsaoOrcamentaria->buscaPtres())
        ->addErrorMessage('Valor é necessário e não pode ser vazio');

		$ndpid = new Zend_Form_Element_Select('ndpid');
        $ndpid->setRequired(true);
		$ndpid->setAttribs(array(
            'id'=>'naturezaDespesa',
            'class'=>'form-control chosen-select',
            'style'=>'width:100%;',
            'required'=>'true'
        ))
		->addMultiOption('','Selecione a naturezada da despesa')
		->addMultiOptions(Ted_Utils_Model::capturaListaNaturezaDespesa())
        ->addErrorMessage('Valor é necessário e não pode ser vazio');

        $provalor = new Zend_Form_Element_Text('provalor');
        $provalor->setRequired(true);
        $provalor->setAttrib('onkeyup', 'this.value=mascaraglobal(\'###.###.###.###,##\',this.value)');
        $provalor->setAttrib('onblur', 'this.value=mascaraglobal(\'###.###.###.###,##\',this.value)');
        //$provalor->setAttrib('onchange', 'this.value=mascaraglobal(\'###.###.###.###,##\',this.value)');
        $provalor->setAttrib('placeholder', 'Valor em Reais');
        $provalor->setAttrib('class', 'form-control');
        $provalor->addErrorMessage('Valor é necessário e não pode ser vazio');

		$crdmesexecucao = new Zend_Form_Element_Select('crdmesexecucao');
        $crdmesexecucao->setRequired(true);
		$crdmesexecucao->setAttribs(array(
            'id'=>'crdmesexecucao',
            'class'=>'form-control chosen-select',
            'style'=>'width:100%;'
        ))
		->addMultiOption('','Selecione um Mês para execução')
		->addMultiOptions($this->_getMonths())
        ->addErrorMessage('Valor é necessário e não pode ser vazio');
		
		$crdmesliberacao = new Zend_Form_Element_Select('crdmesliberacao');
        $crdmesliberacao->setRequired(true);
		$crdmesliberacao->setAttribs(array(
            'id'=>'mesliberacao',
            'class'=>'form-control chosen-select',
            'style'=>'width:100%;',
            'required'=>'true'
        ))->addMultiOption('','Selecione um Mês para liberação')
		  ->addMultiOptions($this->_getMonthSpelled())
          ->addErrorMessage('Valor é necessário e não pode ser vazio');
		
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'previsao-form.php'))));
		$this->addElements(array($proid,$ndpid,$tcpid,$ptrid,$provalor,$programaTrabalho,$crdmesliberacao,$crdmesexecucao,$proanoreferencia));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		$this->_loadDefaultSets();
	}

    /**
     * @return array
     */
    private function _getYears()
    {
        $anos = array();
        for($i = 2013; $i<= 2023; $i++){
            $anos[$i] = $i;
        }
        return $anos;
    }

    /**
     * @return array
     */
    private function _getMonths()
    {
        $meses = array('1' => '1 Mês');
        for($i = 2; $i<= 50; $i++){
            $meses[$i] = $i ." Mêses";
        }
        return $meses;
    }

    /**
     * @return array
     */
    private function _getMonthSpelled()
    {
        $meses = array(
            '1' => 'Janeiro',
            '2'=>'Fevereiro',
            '3'=>'Março',
            '4'=>'Abril',
            '5'=>'Maio',
            '6'=>'Junho',
            '7'=>'Julho',
            '8'=>'Agosto',
            '9'=>'Setembro',
            '10'=>'Outubro',
            '11'=>'Novembro',
            '12'=>'Dezembro'
        );
        return $meses;
    }
}