<?php
/**
 * Classe de mapeamento da entidade emendas.beneficiariodetalhe
 *
 * @category Class
 * @package  A1
 * @author   ORION TELES DE MESQUITA <orion.mesquita@cultura.gov.br>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 07-12-2017
 * @link     no link
 */


require_once APPRAIZ .'includes/classes/Modelo.class.inc';


/**
 * Emendas_Model_BeneficiarioDetalhe
 *
 * @category Class
 * @package  A1
 * @author    <>
 * @license  GNU simec.mec.gov.br
 * @version  Release: 
 * @link     no link
 */
class Emendas_Model_BeneficiarioDetalhe extends Modelo
{
    /**
     * Nome da tabela especificada
     * @var string
     * @access protected
     */
    protected $stNomeTabela = 'emendas.beneficiariodetalhe';

    /**
     * Chave primaria.
     * @var array
     * @access protected
     */
    protected $arChavePrimaria = array(
        'bedid',
    );
    /**
     * Chaves estrangeiras.
     * @var array
     */
    protected $arChaveEstrangeira = array(
        'emdid' => array('tabela' => 'emendas.emendadetalhe', 'pk' => 'emdid'),
        'benid' => array('tabela' => 'emendas.beneficiario', 'pk' => 'benid'),
    );

    /**
     * Atributos
     * @var array
     * @access protected
     */
    protected $arAtributos = array(
        'bedid' => null,
        'benid' => null,
        'emdid' => null,
        'bedvalor' => null,
        'bedvalorpriorizado' => null,
        'bedstatus' => null,
    );

}//end Class
?>