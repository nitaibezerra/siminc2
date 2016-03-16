<?php
/**
 * Classe de mapeamento da entidade aspar.situacao
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_Situacao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Model_Situacao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.situacao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'sitid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'sitid' => null,
        'sitdsc' => null,
    );
    
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'sitid' => array(  'Digits'  ),
            'sitdsc' => array(  new Zend_Validate_StringLength(array('max' => 100))  ),
        );
    }//end getCamposValidacao($dados)
  
    /**
     * Retorna uma lista de situação de projetos
     */
    public function retornaLista(){
        $sql = "
                SELECT  sitid, sitdsc
                FROM aspar.situacao
                ORDER BY sitid
            ";
        $cabecalho = array("Descrição");

        $listagem = new Simec_Listagem();
        $listagem->setCabecalho($cabecalho);
        $listagem->setQuery($sql);
        $listagem->addAcao('edit', 'editarPopup');
        $listagem->addAcao('delete', 'excluir');

        $listagem->turnOffForm();
        $listagem->render(Simec_Listagem::SEM_REGISTROS_MENSAGEM);
    }
    
    /**
     * Trata o charset da página
     * @return boolean
     */
    public function antesSalvar(){         
        $this->sitdsc = utf8_decode($this->sitdsc);
        return true;
    }
    
}//end Class
?>