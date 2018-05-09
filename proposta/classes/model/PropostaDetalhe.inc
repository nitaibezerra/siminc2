<?php
/**
 * Classe de mapeamento da entidade proposta.propostadetalhe
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <teste@teste.com.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-04-2018
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Proposta_Model_PropostaDetalhe
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Proposta_Model_PropostaDetalhe extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'proposta.propostadetalhe';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'prdid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'idoid' => array('tabela' => 'idoc', 'pk' => 'idoid'),
        'foncod' => array('tabela' => 'fonterecurso', 'pk' => 'foncod'),
        'iducod' => array('tabela' => 'identifuso', 'pk' => 'iducod'),
        'ndpid' => array('tabela' => 'naturezadespesa', 'pk' => 'ndpid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'prdid' => null,
        'ndpid' => null,
        'iducod' => null,
        'foncod' => null,
        'idoid' => null,
        'prdvalor' => null,
        'prdvalorexpansao' => null,
        'proid' => null,
        'prdstatus' => null,
    );

}//end Class
?>