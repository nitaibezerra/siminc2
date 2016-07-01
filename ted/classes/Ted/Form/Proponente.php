<?php

/**
 * Class Ted_Form_Proponente
 */
class Ted_Form_Proponente extends Ted_Form_Abstract
{
			
	public function init()
	{
		parent::init();
		$unidadeGestora = new Ted_Model_UnidadeGestora();
	
		$this->setName('proponente');
		$this->setAttrib('id', 'proponente');
		$this->setMethod('post');
		$this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

		$tcpid = new Zend_Form_Element_Hidden('tcpid');
		
		$ungcodproponente = new Zend_Form_Element_Select('ungdsc');
		$ungcodproponente->setAttrib('id', 'ungdsc');
		$ungcodproponente->setAttrib('class', 'form-control chosen-select');
		$ungcodproponente->setAttrib('required', 'true');
		$ungcodproponente->addMultiOption('','Selecione o Proponente');
		$ungcodproponente->addMultiOptions($unidadeGestora->pegaListaProponente());
		
		$unicod = new Zend_Form_Element_Text('ungcod');
        $unicod->setRequired(TRUE);
        $unicod->addErrorMessage('Valor é necessário e não pode ser vazio');
        $unicod->setAttrib('id', 'ungcod');
        $unicod->setAttrib('class', 'form-control ');
        //$unicod->setAttrib('disabled', '');
		
		$razaoSocial = new Zend_Form_Element_Text('razao');
		$razaoSocial->setRequired(TRUE);
		$razaoSocial->addErrorMessage('Valor é necessário e não pode ser vazio');
		$razaoSocial->setAttrib('id', 'razao');
		$razaoSocial->setAttrib('class', 'form-control');
        //$razaoSocial->setAttrib('disabled', '');
		
		$codigoGestao = new Zend_Form_Element_Text('gescod');
		$codigoGestao->setRequired(TRUE);
		$codigoGestao->addErrorMessage('Valor é necessário e não pode ser vazio');
		$codigoGestao->setAttrib('id', 'gescod');
		$codigoGestao->setAttrib('class', 'form-control');
        //$codigoGestao->setAttrib('disabled','');
		
		$cnpj = new Zend_Form_Element_Text('ungcnpj');
		$cnpj->setRequired(TRUE);
		$cnpj->addErrorMessage('Valor é necessário e não pode ser vazio');
		$cnpj->setAttrib('id', 'ungcnpj');
		$cnpj->setAttrib('class', 'form-control');
        $cnpj->setAttrib('onkeyup', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);');
        $cnpj->setAttrib('onblur', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);');
        //$cnpj->setAttrib('disabled', '');
		
		$ungendereco = new Zend_Form_Element_Text('ungendereco');
		$ungendereco->setRequired(true);
		$ungendereco->addErrorMessage('Valor é necessário e não pode ser vazio');
		$ungendereco->setAttrib('id', 'ungendereco');
		$ungendereco->setAttrib('class', 'form-control');
		
		$ungbairro = new Zend_Form_Element_Text('ungbairro');
		$ungbairro->setRequired(true);
		$ungbairro->addErrorMessage('Valor é necessário e não pode ser vazio');
		$ungbairro->setAttrib('id', 'ungbairro');
		$ungbairro->setAttrib('class', 'form-control');
		
		$uf = new Zend_Form_Element_Select('estuf');
		$uf->setRequired(TRUE);
		$uf->addErrorMessage('Valor é necessário e não pode ser vazio');
		$uf->setAttrib('id', 'estuf');
		$uf->setAttrib('class', 'form-control chosen-select');
		$uf->addMultiOption('','Selecione o Estado');
		$uf->addMultiOptions(Ted_Utils_Model::pegaUF());
		
		$muncod= new Zend_Form_Element_Select('muncod');
		$muncod->setRequired(true);
		$muncod->addErrorMessage('Valor é necessário e não pode ser vazio');
        $muncod->addMultiOption('','Selecione o Município');
        $muncod->setAttrib('class', 'form-control chosen-select');
		$muncod->setAttrib('id', 'muncod');
		
		$ungcep = new Zend_Form_Element_Text('ungcep');
		$ungcep->setRequired(true);
		$ungcep->addErrorMessage('Valor é necessário e não pode ser vazio');
		$ungcep->setAttrib('id', 'ungcep');
		$ungcep->setAttrib('class', 'form-control ');

        $ungddd = new Zend_Form_Element_Text('ungddd');
        $ungddd->setRequired(true);
        $ungddd->addErrorMessage('Valor é necessário e não pode ser vazio');
        $ungddd->setAttrib('id', 'ungddd');
        $ungddd->setAttrib('maxlength', '2');
        $ungddd->setAttrib('class', 'form-control');

        $ungfone = new Zend_Form_Element_Text('ungfone');
		$ungfone->setRequired(true);
		$ungfone->addErrorMessage('Valor é necessário e não pode ser vazio');
		$ungfone->setAttrib('id', 'ungfone');
		$ungfone->setAttrib('maxlength', '9');
		$ungfone->setAttrib('class', 'form-control');
		
		$ungemail = new Zend_Form_Element_Text('ungemail');
		$ungemail->setRequired(true);
		$ungemail->addErrorMessage('Valor é necessário e não pode ser vazio');
		$ungemail->setAttrib('id', 'ungemail');
		$ungemail->setAttrib('class', 'form-control');

        //area tecnica responsavel
        $corid = new Zend_Form_Element_Hidden('corid');
        $corid->setAttrib('id', 'corid');
        $corid->setAttrib('class', 'form-control perfil-coordenacao');
        $corid->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($corid);

        $nomecoordenacao = new Zend_Form_Element_Text('nomecoordenacao');
        $nomecoordenacao->setAttrib('id', 'nomecoordenacao');
        $nomecoordenacao->setAttrib('class', 'form-control perfil-coordenacao');

        $dddcoordenacao = new Zend_Form_Element_Text('dddcoordenacao');
        $dddcoordenacao->setAttrib('onkeyup', 'this.value=mascaraglobal(\'##\',this.value);');
        $dddcoordenacao->setAttrib('onblur', 'this.value=mascaraglobal(\'##\',this.value);');
        $dddcoordenacao->setAttrib('id', 'dddcoordenacao');
        $dddcoordenacao->setAttrib('class', 'form-control perfil-coordenacao');

        $telefonecoordenacao = new Zend_Form_Element_Text('telefonecoordenacao');
        $telefonecoordenacao->setAttrib('onkeyup', 'this.value=mascaraglobal(\'####-####\',this.value);');
        $telefonecoordenacao->setAttrib('onblur', 'this.value=mascaraglobal(\'####-####\',this.value);');
        $telefonecoordenacao->setAttrib('id', 'telefonecoordenacao');
        $telefonecoordenacao->setAttrib('class', 'form-control perfil-coordenacao');
        //Fim area tecnica responsavel

        //Representante Legal
        $rlid = new Zend_Form_Element_Hidden('rlid');
        $rlid->setAttrib('id', 'rlid');
        $rlid->setAttrib('class', 'form-control input-rl');
        $rlid->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($rlid);

        $substituto = new Zend_Form_Element_Hidden('substituto');
        $substituto->setValue(1);
        $substituto->setAttrib('id', 'substituto');
        $substituto->setAttrib('class', 'form-control input-rl');
        //$substituto->setRequired(true);
        //$substituto->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($substituto);

        $cpf = new Zend_Form_Element_Text('cpf');
        $cpf->setAttrib('id', 'cpf');
        $cpf->setAttrib('onkeyup', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);');
        $cpf->setAttrib('onblur', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);');
        $cpf->setAttrib('class', 'form-control input-rl');
        //$cpf->setRequired(true);
        //$cpf->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($cpf);

        $nome = new Zend_Form_Element_Text('nome');
        $nome->setAttrib('id', 'nome');
        $nome->setAttrib('maxlenght', '255');
        $nome->setAttrib('class', 'form-control input-rl');
        //$nome->setRequired(true);
        //$nome->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($nome);

        $funcao = new Zend_Form_Element_Text('funcao');
        $funcao->setAttrib('id', 'funcao');
        $funcao->setAttrib('maxlenght', '255');
        $funcao->setAttrib('class', 'form-control input-rl');
        //$funcao->setRequired(true);
        $funcao->setValue('Representante Legal Substituto');
        //$funcao->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($funcao);

        $email = new Zend_Form_Element_Text('email');
        $email->setAttrib('id', 'email');
        $email->setAttrib('maxlenght', '70');
        $email->setAttrib('class', 'form-control input-rl');
        //$email->setRequired(true);
        //$email->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($email);
        //fim do form de representante legal substituto
		
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'proponente-form.php'))));
		$this->addElements(array(
            $tcpid,$ungbairro,$ungcep,$cnpj,$codigoGestao,$unicod,$ungemail,$ungendereco,
            $muncod,$razaoSocial,$ungddd,$ungfone,$uf,$ungcodproponente,
            $rlid, $substituto, $cpf, $nome, $funcao, $email,
            $nomecoordenacao, $dddcoordenacao, $telefonecoordenacao
        ));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		$this->_loadDefaultSets();
	}
}