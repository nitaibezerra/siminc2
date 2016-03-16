<?php

class Ted_Form_SolicitarNotaCredito extends Ted_Form_Abstract
{
    public function init()
    {
        parent::init();

        $this->setName('solicitarNC');
        $this->setAttrib('id', 'solicitarNC');
        $this->setMethod(Zend_Form::METHOD_POST);
        $this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

        $tcpid = new Zend_Form_Element_Hidden('tcpid');		

        $tcpid->setAttribs(array('id'=>'tcpid'));
        
        $evento_contabil = new Zend_Form_Element_Hidden('evento_contabil');
        $evento_contabil->setAttrib('id', 'evento_contabil');
        $evento_contabil->setRequired(true);
        $evento_contabil->setValue(300300);

        $especie = new Zend_Form_Element_Hidden('especie');
        $especie->setAttrib('id', 'especie');
        $especie->setRequired(true);
        $especie->setValue(3);

        $sistema = new Zend_Form_Element_Hidden('sistema');
        $sistema->setAttrib('id', 'sistema');
        $sistema->setRequired(true);
        $sistema->setValue(3);

        $sigefusername = new Zend_Form_Element_Text('sigefusername');
        $sigefusername->setAttrib('class', 'form-control');
        $sigefusername->setRequired(true);
        $sigefusername->setAttrib('id', 'sigefusername');
        $sigefusername->setAttrib('required', 'true');
        $sigefusername->addErrorMessage('Valor é necessário e não pode ser vazio');

        $sigefpassword = new Zend_Form_Element_Password('sigefpassword');
        $sigefpassword->setRequired(true);
        $sigefpassword->setAttrib('id', 'sigefpassword');
        $sigefpassword->setAttrib('class', 'form-control');       
        $sigefpassword->setAttrib('required', 'true');
        $sigefpassword->addErrorMessage('Valor é necessário e não pode ser vazio');

        $tcpnumtransfsiafi = new Zend_Form_Element_Text('tcpnumtransfsiafi');
        $tcpnumtransfsiafi->setRequired(true);
        $tcpnumtransfsiafi->setAttrib('id', 'tcpnumtransfsiafi');
        $tcpnumtransfsiafi->setAttrib('class', 'form-control');
        $tcpnumtransfsiafi->setAttrib('required', 'true');
        $tcpnumtransfsiafi->addErrorMessage('Valor é necessário e não pode ser vazio');

        $tcpnumprocessofnde = new Zend_Form_Element_Text('tcpnumprocessofnde');
        $tcpnumprocessofnde->setRequired(true);
        $tcpnumprocessofnde->setAttrib('id', 'tcpnumprocessofnde');
        $tcpnumprocessofnde->setAttrib('class', 'form-control');
        $tcpnumprocessofnde->setAttrib('required', 'true');
        $tcpnumprocessofnde->addErrorMessage('Valor é necessário e não pode ser vazio');

        $unicod = new Zend_Form_Element_Text('unicod');
        $unicod->setRequired(true);
        $unicod->setAttrib('id', 'unicod');
        $unicod->setAttrib('class', 'form-control');
        $unicod->setAttrib('required', 'true');
        $unicod->addErrorMessage('Valor é necessário e não pode ser vazio');

        $ungcodemitente = new Zend_Form_Element_Text('ungcodemitente');
        $ungcodemitente->setRequired(true);
        $ungcodemitente->setAttrib('id', 'ungcodemitente');
        $ungcodemitente->setAttrib('class', 'form-control');
        $ungcodemitente->setAttrib('required', 'true');
        $ungcodemitente->addErrorMessage('Valor é necessário e não pode ser vazio');

        $gescodemitente = new Zend_Form_Element_Text('gescodemitente');
        $gescodemitente->setRequired(true);
        $gescodemitente->setAttrib('id', 'gescodemitente');
        $gescodemitente->setAttrib('required', 'true');
        $gescodemitente->setAttrib('class', 'form-control');
        $gescodemitente->addErrorMessage('Valor é necessário e não pode ser vazio');

        $tcpprogramafnde = new Zend_Form_Element_Select('tcpprogramafnde');
        $tcpprogramafnde->setAttrib('class', 'form-control chosen-container');
        $tcpprogramafnde->setAttrib('id','tcpprogramafnde');
        //$tcpprogramafnde->setAttrib('required', 'true');
        $tcpprogramafnde->addMultiOption('','Selecione o programa');
        $notaCredito = new Ted_Model_NotaCredito();
        $tcpprogramafnde->addMultiOptions($notaCredito->pegaListaPrograma());

        $tcpobsfnde = new Zend_Form_Element_Select('tcpobsfnde');
        $tcpobsfnde->setRequired(true);
        $tcpobsfnde->setAttrib('id', 'tcpobsfnde');
        $tcpobsfnde->setAttrib('class', 'form-control chosen-container');
        //$tcpobsfnde->setAttrib('required', 'true');
        $tcpobsfnde->addMultiOption('','Selecione a observação');

        $tcpobscomplemento = new Zend_Form_Element_Textarea('tcpobscomplemento');
        $tcpobscomplemento->setRequired(true);
        $tcpobscomplemento->setAttrib('rows', '4');
        $tcpobscomplemento->setAttrib('cols', '80');
        $tcpobscomplemento->setAttrib('class', 'form-control');
        $tcpobscomplemento->setAttrib('required', 'true');
        $tcpobscomplemento->setAttrib('maxlength', '240');

        $this->setDecorators(array(array('ViewScript', array('viewScript' => 'solicitar-notacredito-form.php'))));

        $this->addElements(array($tcpid,$evento_contabil, $especie, $sistema, $sigefusername, $sigefpassword,
            $tcpnumtransfsiafi, $tcpnumprocessofnde, $unicod, $ungcodemitente,
            $gescodemitente, $tcpprogramafnde, $tcpobsfnde, $tcpobscomplemento));

        $this->setElementDecorators(array('ViewHelper', 'Errors'));
        $this->_loadDefaultSets();
    }

    /**
     * @param $programa
     * @return array
     */
    public function carregaObsFnde($programa)
    {
    	global $db;

    	$sql = "
	    	SELECT
		    	DISTINCT obscod as codigo,
		    	obscod as descricao
	    	FROM ted.dadosprogramasfnde
	    	WHERE eventocontabil = '300300' and prgcodfnde = '{$programa}'
	    	ORDER BY obscod;
    	";
    	
    	$list = $db->carregar($sql);
		$options = array();
		if ($list) {
			foreach($list as $item) {
				$options[$item['codigo']] = $item['descricao'];
			}
		}
    	    	
    	return ($options) ? $options : array();
    }

}
