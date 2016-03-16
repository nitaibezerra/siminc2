<?php

/**
 * Classe de controle da entidade
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */

/**
 * Par_Controle_Entidade
 *
 * @category Class/Render
 * @package  A1
 * @author   Eduardo Dunice <eduardoneto@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 25/09/2015
 * @link     no link
 */
class Par3_Controller_Entidade extends Controle
{

    public function __construct()
    {
        parent::__construct();
    }


    /**
     * Função formPessoaJuridica
     * - monta o formulario de dados de pessoa juridica
     *
     * @return string escreve a lista.
     *
     */
    public function formPessoaJuridica($disabled = 'disabled', $objPessoaJuridica = null)
    {
        global $simec;

        echo '<input type="hidden" name="entid" value="'.$objPessoaJuridica->entid.'"/>';
        echo '<input type="hidden" name="entcnpj_old" value="'.formatar_cnpj($objPessoaJuridica->entcnpj).'"/>';
        echo $simec->cnpj('entcnpj', 'CNPJ', $objPessoaJuridica->entcnpj, array('class' => 'cnpj', $disabled, 'required'));
        echo $simec->input('entnome', 'Nome', $objPessoaJuridica->entnome, array('maxlength' => '255', $disabled, 'required'));
        echo $simec->input('entrazaosocial', 'Razão Social', $objPessoaJuridica->entrazaosocial, array('maxlength' => '255', $disabled, 'required'));
        echo $simec->input('entinscricaoestadual', 'Inscrição Estadual', $objPessoaJuridica->entinscricaoestadual, array('maxlength' => '255', $disabled, 'required'));
        echo $simec->email('entemail', 'E-mail', $objPessoaJuridica->entemail, array('class' => 'email', $disabled, 'required'));
        echo $simec->input('entsigla', 'Sigla', $objPessoaJuridica->entsigla, array('maxlength' => '15', $disabled, 'required'));
        echo $simec->telefone('enttelefonecomercial', 'Telefone Comercial', $objPessoaJuridica->enttelefonecomercial, array('class' => 'telefone', $disabled, 'required'));
    }//end formPessoaJuridica()


    /**
     * Função formPessoaFisica
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formPessoaFisica($disabled = 'disabled', $objPessoaFisica = null)
    {
        global $simec;

        echo '<input type="hidden" name="entid" value="'.$objPessoaFisica->entid.'"/>';
        echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';
        echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"entnome": "no_pessoa_rf"}'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entnome', 'Nome', $objPessoaFisica->entnome, array('maxlength' => '255', $disabled, 'required', 'readonly' => 'readonly'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->email('entemail', 'E-mail', $objPessoaFisica->entemail, array('class' => 'email', $disabled, 'required'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entrg', 'Registro Geral (RG)', $objPessoaFisica->entrg, array($disabled), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entorgexpedidor', 'Orgão Expedidor', $objPessoaFisica->entorgexpedidor, array($disabled), array('label-size' => 3, 'input-size' => 9));
        echo $simec->data('entdtnascimento', 'Data de Nascimento', $objPessoaFisica->entdtnascimento, array($disabled), array('label-size' => 3, 'input-size' => 9));
        echo $simec->telefone('enttelefonecomercial', 'Telefone Comercial', $objPessoaFisica->enttelefonecomercial, array('class' => 'telefone', 'required', $disabled), array('label-size' => 3, 'input-size' => 9));
        echo $simec->telefone('enttelefonecelular', 'Telefone Celular', $objPessoaFisica->enttelefonecelular, array('class' => 'telefone', 'required', $disabled), array('label-size' => 3, 'input-size' => 9));
        echo $simec->telefone('enttelefonefax', 'Telefone Fax', $objPessoaFisica->enttelefonefax, array('class' => 'telefone', $disabled), array('label-size' => 3, 'input-size' => 9));
    }//end formPessoaFisica()


    /**
     * Função formDirigente
     * - monta o formulario de dados de pessoa fisica
     *
     * @return string escreve a lista.
     *
     */
    public function formDirigente($disabled = 'disabled', $objPessoaFisica = null)
    {
    	global $simec;

    	/*
    	 * @todo Verificar tabela que possui estas informações
    	 */
    	$niveisEnsino = array(
    		'1' => 'Ensino fundamental',
			'2' => 'Ensino médio',
			'3' => 'Ensino superior incompleto',
			'4' => 'Ensino superior completo',
			'5' => 'Especialização',
			'6' => 'Mestrado',
			'7' => 'Doutorado'
		);

    	$radioBoolean = array('t' => 'Sim', 'f' => 'Não');

    	echo $simec->select('entnivelensino', 'Nível de escolaridade', $objPessoaFisica->entnivelensino, $niveisEnsino, array('maxlength' => '255', $disabled), array('label-size' => 3, 'input-size' => 9));
    	echo $simec->input('entempoatuacao', 'Tempo de atuação', $objPessoaFisica->entempoatuacao, array('maxlength' => '255', $disabled, 'help' => 'Quanto tempo de atuação como dirigente municipal de educação?'),
    						array('label-size' => 3, 'input-size' => 9));
    	echo $simec->radio('entcursomec', 'Curso de formação MEC', $objPessoaFisica->entcursomec ? $objPessoaFisica->entcursomec : 'f', $radioBoolean,
				   			array('maxlength' => '255', $disabled, 'help' => 'Já participou de cursos de formação ofertados pelo MEC?'), array('label-size' => 3, 'input-size' => 9));
    	echo $simec->input('entcursomecdescricao', 'Qual curso?', $objPessoaFisica->entcursomecdescricao, array('maxlength' => '255', $disabled), array('label-size' => 3, 'input-size' => 9, 'visible' => false));
    	 
    }//end formDirigente()

    /**
     * Função formEnderecoEntidade
     * - monta o formulario de endereço de pessoa juridica ou fisica
     *
     * @return string escreve o formulario.
     *
     */
    public function formEnderecoEntidade($disabled = 'disabled', $objEndereco = null)
    {
        global $simec;

        echo '<input type="hidden" name="endid" value="'.$objPessoaFisica->endid.'"/>';
        echo $simec->cep('endcep', 'CEP', $objEndereco->endcep, array('class' => 'cep', $disabled, 'required'));
        echo $simec->input('endlogradouro', 'Logradouro', $objEndereco->endlogradouro, array('maxlength' => '255', $disabled, 'required'));
        echo $simec->input('endcomplemento', 'Complemento', $objEndereco->endcomplemento, array('maxlength' => '255', $disabled));
        echo $simec->input('endnumero', 'Número', $objEndereco->endnumero, array('maxlength' => '10', 'class' => 'inteiro', $disabled, 'required'));
        echo $simec->input('endbairro', 'Bairro', $objEndereco->endbairro, array('maxlength' => '255', $disabled, 'required'));
    }//end formEnderecoEntidade()


    /**
     * Função formConselho
     * - monta o formulario de dados do conselho
     *
     * @return string escreve a lista.
     *
     */
    public function formConselho($disabled = 'disabled', $objPessoaJuridica = null)
    {
    	global $simec;

        echo '<input type="hidden" name="entid" id="entid" value="'.$objPessoaFisica->entid.'"/>';
        echo '<input type="hidden" name="entcpf_old" value="'.formatar_cpf($objPessoaFisica->entcpf).'"/>';
        echo $simec->cpf('entcpf', 'CPF', $objPessoaFisica->entcpf, array('class' => 'cpf', $disabled, 'data-pessoa' => true, 'data-pessoa-campos' => '{"entnome": "no_pessoa_rf"}'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entnome', 'Nome', $objPessoaFisica->entnome, array('maxlength' => '255', $disabled, 'required', 'readonly' => 'readonly'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->email('entemail', 'E-mail', $objPessoaFisica->entemail, array('class' => 'email', $disabled, 'required'), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entatuacao', 'Atuação', $objPessoaFisica->entatuacao, array($disabled), array('label-size' => 3, 'input-size' => 9));
        echo $simec->input('entcargo', 'Cargo', $objPessoaFisica->entcargo, array($disabled), array('label-size' => 3, 'input-size' => 9));
    }//end formConselho()
    
    
}//end class

?>