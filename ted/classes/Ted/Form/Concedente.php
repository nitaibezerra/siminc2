<?php

/**
 * Class Ted_Form_Concedente
 */
class Ted_Form_Concedente extends Ted_Form_Abstract
{

	public function init()
	{
		parent::init();
		$unidadeGestora = new Ted_Model_UnidadeGestora();
		$this->setName('concedente');
		$this->setAttrib('id', 'concedente');
		$this->setMethod(Zend_Form::METHOD_POST);
		$this->setAction('http://'.$_SERVER['SERVER_NAME'] . $_SERVER['REQUEST_URI']);

		$tcpid = new Zend_Form_Element_Hidden('tcpid');
		
		$ungcodconcedente = new Zend_Form_Element_Select('ungcodconcedente');		
		$ungcodconcedente->setAttrib('id', 'ungcod');
		$ungcodconcedente->setRequired(TRUE);
		$ungcodconcedente->setAttrib('class', 'form-control chosen-select');
		//$ungcodconcedente->setAttrib('required', 'true');
		$ungcodconcedente->addMultiOption('', 'Selecione o Concedente');
		$ungcodconcedente->addMultiOptions($unidadeGestora->pegaListaConcedente());

		$ungcodpoliticafnde = new Zend_Form_Element_Select('ungcodpoliticafnde');
		$ungcodpoliticafnde->setAttrib('id', 'fnderespcod');
        $ungcodpoliticafnde->setAttrib('class', 'form-control chosen-select');
		$ungcodpoliticafnde->addMultiOption('','Selecione o responsável pela política');
		$ungcodpoliticafnde->addMultiOptions($unidadeGestora->pegaListaResponsavelPolitica());
		
		$codigoUG = new Zend_Form_Element_Text('unicod');
		$codigoUG->setRequired(TRUE);
		$codigoUG->setAttrib('id', 'unicod');
		$codigoUG->addErrorMessage('Valor é necessário e não pode ser vazio');
		$codigoUG->setAttrib('class', 'form-control ');
		//$codigoUG->setAttrib('disabled', '');
		
		$codigoGestao = new Zend_Form_Element_Text('gescod');
		$codigoGestao->setRequired(TRUE);
		$codigoGestao->addErrorMessage('Valor é necessário e não pode ser vazio');
		$codigoGestao->setAttrib('id', 'gescod');
		$codigoGestao->setAttrib('class', 'form-control');
        //$codigoGestao->setAttrib('disabled', '');
		
		$cnpj = new Zend_Form_Element_Text('ungcnpj');
		$cnpj->setRequired(TRUE);
		$cnpj->addErrorMessage('Valor é necessário e não pode ser vazio');
		$cnpj->setAttrib('id', 'ungcnpj');
		$cnpj->setAttrib('class', 'form-control ');
        $cnpj->setAttrib('onkeyup', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);');
        $cnpj->setAttrib('onblur', 'this.value=mascaraglobal(\'##.###.###/####-##\',this.value);');
        //$cnpj->setAttrib('disabled', '');
		
		$endereco = new Zend_Form_Element_Text('ungendereco');
		$endereco->setRequired(true);
		$endereco->addErrorMessage('Valor é necessário e não pode ser vazio');
        $endereco->setAttrib('id', 'ungendereco');
        $endereco->setAttrib('class', 'form-control ');
        //$endereco->setAttrib('disabled', '');
		
		$bairro = new Zend_Form_Element_Text('ungbairro');
		$bairro->setRequired(true);
		$bairro->addErrorMessage('Valor é necessário e não pode ser vazio');
        $bairro->setAttrib('id', 'ungbairro');
        $bairro->setAttrib('class', 'form-control ');
        //$bairro->setAttrib('disabled', '');

        $uf = new Zend_Form_Element_Select('estuf');
        $uf->setRequired(TRUE);
        $uf->addErrorMessage('Valor é necessário e não pode ser vazio');
        $uf->setAttrib('id', 'estuf');
        $uf->setAttrib('class', 'form-control chosen-select');
        $uf->addMultiOption('','Selecione o Estado');
        $uf->addMultiOptions(Ted_Utils_Model::pegaUF());

        $municipio= new Zend_Form_Element_Select('muncod');
        $municipio->setRequired(true);
        $municipio->addErrorMessage('Valor é necessário e não pode ser vazio');
        $municipio->addMultiOption('','Selecione o Município');
        $municipio->setAttrib('class', 'form-control chosen-select');
        $municipio->setAttrib('id', 'muncod');
		
		$cep = new Zend_Form_Element_Text('ungcep');
		$cep->setRequired(true);
		$cep->addErrorMessage('Valor é necessário e não pode ser vazio');
        $cep->setAttrib('id', 'ungcep');
        $cep->setAttrib('class', 'form-control');
        //$cep->setAttrib('disabled', '');
		
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
        $ungfone->setAttrib('class', 'form-control ');
		
		$ungemail = new Zend_Form_Element_Text('ungemail');
        $ungemail->setRequired(true);
        $ungemail->addErrorMessage('Valor é necessário e não pode ser vazio');
        $ungemail->setAttrib('id', 'ungemail');
        $ungemail->setAttrib('class', 'form-control');
        $ungemail->addValidator(new Zend_Validate_EmailAddress());

        //area tecnica responsavel
        $corid = new Zend_Form_Element_Hidden('corid');
        $corid->setAttrib('id', 'corid');
        $corid->setAttrib('class', 'form-control perfil-coordenacao');
        $corid->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($corid);

        $nomecoordenacao = new Zend_Form_Element_Text('nomecoordenacao');
        $nomecoordenacao->setRequired(FALSE);
        $nomecoordenacao->setAttrib('id', 'nomecoordenacao');
        $nomecoordenacao->setAttrib('class', 'form-control perfil-coordenacao');
        $this->addElement($nomecoordenacao);

        $dddcoordenacao = new Zend_Form_Element_Text('dddcoordenacao');
        $dddcoordenacao->setRequired(FALSE);
        $dddcoordenacao->setAttrib('onkeyup', 'this.value=mascaraglobal(\'##\',this.value);');
        $dddcoordenacao->setAttrib('onblur', 'this.value=mascaraglobal(\'##\',this.value);');
        $dddcoordenacao->setAttrib('id', 'dddcoordenacao');
        $dddcoordenacao->setAttrib('class', 'form-control perfil-coordenacao');
        $this->addElement($dddcoordenacao);

        $telefonecoordenacao = new Zend_Form_Element_Text('telefonecoordenacao');
        $telefonecoordenacao->setRequired(FALSE);
        $telefonecoordenacao->setAttrib('onkeyup', 'this.value=mascaraglobal(\'####-####\',this.value);');
        $telefonecoordenacao->setAttrib('onblur', 'this.value=mascaraglobal(\'####-####\',this.value);');
        $telefonecoordenacao->setAttrib('id', 'telefonecoordenacao');
        $telefonecoordenacao->setAttrib('class', 'form-control perfil-coordenacao');
        $this->addElement($telefonecoordenacao);
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
        //$substituto->setRequired(false);
        //$substituto->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($substituto);

        $cpf = new Zend_Form_Element_Text('cpf');
        $cpf->setAttrib('id', 'cpf');
        $cpf->setAttrib('onkeyup', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);');
        $cpf->setAttrib('onblur', 'this.value=mascaraglobal(\'###.###.###-##\',this.value);');
        $cpf->setAttrib('class', 'form-control input-rl');
        //$cpf->setRequired(false);
        //$cpf->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($cpf);

        $nome = new Zend_Form_Element_Text('nome');
        $nome->setAttrib('id', 'nome');
        $nome->setAttrib('maxlenght', '255');
        $nome->setAttrib('class', 'form-control input-rl');
        //$nome->setRequired(false);
        //$nome->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($nome);

        $funcao = new Zend_Form_Element_Text('funcao');
        $funcao->setAttrib('id', 'funcao');
        $funcao->setAttrib('maxlenght', '255');
        $funcao->setAttrib('class', 'form-control input-rl');
        //$funcao->setRequired(false);
        $funcao->setValue('Representante Legal Substituto');
        //$funcao->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($funcao);

        $email = new Zend_Form_Element_Text('email');
        $email->setAttrib('id', 'email');
        $email->setAttrib('maxlenght', '70');
        $email->setAttrib('class', 'form-control input-rl');
        //$email->setRequired(false);
        //$email->addErrorMessage('Valor é necessário e não pode ser vazio');
        $this->addElement($email);
        //fim do form de representante legal substituto
		
		$this->setDecorators(array(array('ViewScript', array('viewScript' => 'concedente-form.php'))));
		$this->addElements(array(
            $tcpid,$bairro,$cep,$cnpj,$codigoGestao,$codigoUG,$email,$endereco,
            $municipio,$telefone,$uf,$ungcodconcedente,$ungcodpoliticafnde, $ungddd, $ungfone, $ungemail
        ));
		$this->setElementDecorators(array('ViewHelper', 'Errors'));
		$this->_loadDefaultSets();
	}
	
}