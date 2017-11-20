<?php
/**
 * Classe de mapeamento da entidade emendas.emendadetalhe
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 13-11-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_EmendaDetalhe
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_EmendaDetalhe extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.emendadetalhe';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'emdid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'foncod' => array('tabela' => 'public.fonterecurso', 'pk' => 'foncod'),
        'mapcod' => array('tabela' => 'modalidadeaplicacao', 'pk' => 'mapcod'),
        'gndcod' => array('tabela' => 'gnd', 'pk' => 'gndcod'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'emdid' => null,
        'gndcod' => null,
        'mapcod' => null,
        'foncod' => null,
        'emdvalor' => null,
    );

}//end Class
?>