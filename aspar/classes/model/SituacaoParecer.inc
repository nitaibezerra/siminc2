<?php
/**
 * Classe de mapeamento da entidade aspar.situacaoparecer
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_Situacao_Parecer
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Model_SituacaoParecer extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.situacaoparecer';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'stpid',
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
        'stpid' => null,
        'stpdsc' => null,
    );
    
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'stpid' => array(  'Digits'  ),
            'stpdsc' => array(  new Zend_Validate_StringLength(array('max' => 100))  ),
        );
    }//end getCamposValidacao($dados)
  
    /**
     * Retorna uma lista de situação de projetos
     */
    public function retornaLista(){
        $sql = "
                SELECT  stpid, stpdsc
                FROM aspar.situacaoparecer
                ORDER BY stpid
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
        $this->stpdsc = utf8_decode($this->stpdsc);
        return true;
    }
    
}//end Class
?>