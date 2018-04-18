<?php
/**
 * Classe de mapeamento da entidade public.naturezadespesa
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
 * Public_Model_Naturezadespesa
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Public_Model_NaturezaDespesa extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'public.naturezadespesa';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'ndpid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'gndcod' => array('tabela' => 'gnd', 'pk' => 'gndcod'),
        'mapcod' => array('tabela' => 'modalidadeaplicacao', 'pk' => 'mapcod'),
        'edpcod' => array('tabela' => 'elementodespesa', 'pk' => 'edpcod'),
        'cagcod' => array('tabela' => 'categoriagasto', 'pk' => 'cagcod'),
        'ctecod' => array('tabela' => 'categoriaeconomica', 'pk' => 'ctecod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'ndpid' => null,
        'gndcod' => null,
        'ndpano' => null,
        'ndpcod' => null,
        'ndpdsc' => null,
        'ndpstatus' => null,
        'edpcod' => null,
        'mapcod' => null,
        'ctecod' => null,
        'sbecod' => null,
        'cagcod' => null,
    );

}//end Class
?>