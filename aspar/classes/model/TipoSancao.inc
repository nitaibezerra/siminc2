<?php
/**
 * Classe de mapeamento da entidade aspar.tiposancao
 *
 * @category Class
 * @package  A1
 * @author   RAFAEL FREITAS CARNEIRO <rafaelfreitas@mec.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 08-10-2015
 * @link     no link
 */



/**
 * Aspar_Model_TipoSancao
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Aspar_Model_TipoSancao extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'aspar.tiposancao';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'tpsid',
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
        'tpsid' => null,
        'tpsdsc' => null,
    );
    
    /**
     * Atributos
     * @var $dados array
     * @access protected
     */
    public function getCamposValidacao($dados = array())
    {
        return array(
            'tpsid' => array(  'Digits'  ),
            'tpsdsc' => array(  new Zend_Validate_StringLength(array('max' => 100))  ),
        );
    }//end getCamposValidacao($dados)
  
    /**
     * Retorna uma lista de tpsuação de projetos
     */
    public function retornaLista(){
        $sql = "
                SELECT  tpsid, tpsdsc
                FROM aspar.tiposancao
                ORDER BY tpsid
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
        $this->tpsdsc = utf8_decode($this->tpsdsc);
        return true;
    }
    
}//end Class
?>