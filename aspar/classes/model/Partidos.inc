<?php
/**
 * Classe de mapeamento da entidade aspar.partidos
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_Partidos
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Model_Partidos extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.partidos';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prtid',
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
        'prtid' => null,
        'prtdsc' => null,
    );
    
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'prtid' => array(  'Digits'  ),
            'prtdsc' => array(  new Zend_Validate_StringLength(array('max' => 100))  ),
        );
    }//end getCamposValidacao($dados)
  
    /**
     * Retorna uma lista de prtuação de projetos
     */
    public function retornaLista(){
        $sql = "
                SELECT  prtid, prtdsc
                FROM aspar.partidos
                ORDER BY prtid
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
        $this->prtdsc = utf8_decode($this->prtdsc);
        return true;
    }
    
}//end Class
?>